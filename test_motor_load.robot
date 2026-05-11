*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/test/renode-config/run_xiao.resc

*** Test Cases ***
Should Decrease Velocity Under Load (Open Loop)
    [Tags]                    motor
    [Documentation]           Verifies that increasing LoadTorque decreases motor velocity in open loop.
    [Timeout]                 60 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Set PWM to a fixed value
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 64

    Sleep                     2s

    Write Char On Uart        G
    ${res1}=                  Wait For Line On Uart     Motor ADC: ([0-9]+)  treatAsRegex=true
    ${val1}=                  Set Variable  ${res1['Groups'][0]}
    Log                       ADC value (no load): ${val1}

    # Apply load
    Execute Command           motor LoadTorque 0.05
    Sleep                     2s

    Write Char On Uart        G
    ${res2}=                  Wait For Line On Uart     Motor ADC: ([0-9]+)  treatAsRegex=true
    ${val2}=                  Set Variable  ${res2['Groups'][0]}
    Log                       ADC value (with load): ${val2}

    Should Be True            ${val2} < ${val1}

Should Recover Velocity Under Load (Closed Loop)
    [Documentation]           Verifies that PID controller recovers target velocity under load.
    [Timeout]                 120 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Enable PWM Interrupts
    Write Char On Uart        M
    Wait For Line On Uart     PWM Interrupt Enabled for Slice

    # Enable Sync ADC
    Write Char On Uart        H
    Wait For Line On Uart     Sync ADC Enabled

    # Enable PID loop (Target is 1000 by default)
    Write Char On Uart        l
    Wait For Line On Uart     PID Loop: Enabled

    # Enable Telemetry
    Write Char On Uart        v
    Wait For Line On Uart     PID Telemetry: Enabled

    # Wait for stabilization
    Sleep                     10s

    Write Char On Uart        V
    ${res1}=                  Wait For Line On Uart     Last Sync ADC: ([0-9]+)  treatAsRegex=true
    ${val1}=                  Set Variable  ${res1['Groups'][0]}
    Log                       ADC value (stabilized): ${val1}
    # DEBUG: Log exact diff
    ${exact_diff1}=           Evaluate      abs(int(${val1}) - 1000)
    Log                       Exact diff 1: ${exact_diff1}

    # Check if we are near target 1000 (allowing some error)
    ${diff1}=                 Evaluate      abs(int(${val1}) - 1000)
    Should Be True            ${diff1} < 200

    # Apply load
    Execute Command           motor LoadTorque 0.02
    Log                       Applied LoadTorque 0.02

    # Wait for recovery
    Sleep                     10s

    Write Char On Uart        V
    ${res2}=                  Wait For Line On Uart     Last Sync ADC: ([0-9]+)  treatAsRegex=true
    ${val2}=                  Set Variable  ${res2['Groups'][0]}
    Log                       ADC value (after load): ${val2}
    # DEBUG: Log exact diff
    ${exact_diff2}=           Evaluate      abs(int(${val2}) - 1000)
    Log                       Exact diff 2: ${exact_diff2}

    # Check if we recovered near target 1000
    ${diff2}=                 Evaluate      abs(int(${val2}) - 1000)
    Should Be True            ${diff2} < 200

*** Keywords ***
Create Machine
    Execute Command           $platform_file = @${CURDIR}/test/renode-config/boards/seeed_xiao_rp2040_motor.repl
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
