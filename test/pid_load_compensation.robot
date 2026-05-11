*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${MOTOR}                      sysbus.motor
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/pid_load_compensation.resc

*** Test Cases ***
Should Compensate For Load Change In PID Loop
    [Documentation]           Verifies that the PID controller increases output when load is applied to maintain speed.
    [Timeout]                 180 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # 1. Enable PWM Interrupts
    Write Char On Uart        M
    Wait For Line On Uart     PWM Interrupt Enabled for Slice 0

    # 2. Enable Synchronized ADC Sampling
    Write Char On Uart        H
    Wait For Line On Uart     Sync ADC Enabled

    # 3. Enable PID Loop and Telemetry
    Write Char On Uart        l
    Wait For Line On Uart     PID Loop: Enabled
    Write Char On Uart        v
    Wait For Line On Uart     PID Telemetry: Enabled

    # 4. Wait for stability
    Wait For Line On Uart     TELE: T=1000  timeout=30
    ${line}=                  Wait For Line On Uart     TELE: T=1000 A=([0-9]+).*O=([0-9]+)  treatAsRegex=true
    ${initial_output}=        Set Variable  ${line['Groups'][1]}
    Log                       Initial PID output: ${initial_output}

    # 5. Apply Load Torque
    Execute Command           ${MOTOR} LoadTorque 0.1
    Log                       Applied 0.1 N*m Load Torque

    # 6. Verify that PID output increases
    Wait For Line On Uart     TELE: T=1000  timeout=30
    ${line}=                  Wait For Line On Uart     TELE: T=1000 A=([0-9]+).*O=([0-9]+)  treatAsRegex=true
    ${new_output}=            Set Variable  ${line['Groups'][1]}
    Log                       New PID output: ${new_output}

    Should Be True            ${new_output} >= ${initial_output}

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}
