*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/motor_load_compensation.resc

*** Test Cases ***
Should Compensate For Motor Load
    [Documentation]           Verifies that PID controller increases PWM output when load is applied.
    [Timeout]                 180 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Configure ADC for sync mode (calls on_pwm_interrupt and update_pid)
    Write Char On Uart        H
    Wait For Line On Uart     Sync ADC Enabled

    # Enable PWM Interrupt (which triggers the sync loop)
    Write Char On Uart        M
    Wait For Line On Uart     PWM Interrupt Enabled for Slice 0

    # Set target to 2000
    Repeat Keyword    5 times    Write Char On Uart    t

    # Enable PID Loop
    Write Char On Uart        l
    Wait For Line On Uart     PID Loop: Enabled

    # Enable Telemetry
    Write Char On Uart        v
    Wait For Line On Uart     PID Telemetry: Enabled

    # Initial telemetry
    ${line}=                  Wait For Line On Uart     TELE: T=2000 A=([0-9]+) E=-?[0-9.]+ O=([0-9]+)  treatAsRegex=true
    ${initial_a}=             Set Variable  ${line['Groups'][0]}
    ${initial_o}=             Set Variable  ${line['Groups'][1]}
    Log                       Initial A: ${initial_a}, Initial O: ${initial_o}

    # Apply MASSIVE load to immediately drop A
    Execute Command           sysbus.motor LoadTorque 100.0

    # Wait for PID to react and increase output
    # Since A should drop to 0, error = 2000 - 0 = 2000.
    # Output should increase.
    Sleep                     10s

    ${line}=                  Wait For Line On Uart     TELE: T=2000 A=([0-9]+) E=-?[0-9.]+ O=([0-9]+)  treatAsRegex=true
    ${loaded_a}=              Set Variable  ${line['Groups'][0]}
    ${loaded_o}=              Set Variable  ${line['Groups'][1]}
    Log                       Loaded A: ${loaded_a}, Loaded O: ${loaded_o}

    Should Be True            ${loaded_o} > ${initial_o}

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}
