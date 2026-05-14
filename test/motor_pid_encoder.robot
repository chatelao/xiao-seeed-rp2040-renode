*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${MOTOR}                      sysbus.motor
${FIRMWARE}                   ${CURDIR}/../examples/motor_pid_encoder/.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/../examples/motor_pid_encoder/motor_encoder.resc

*** Test Cases ***
Should Stabilize Speed And Handle Load In PID Loop
    [Documentation]           Verifies that the PID controller reaches target velocity and increases output when load is applied.
    [Timeout]                 180 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     Motor PID Encoder Example Started
    Wait For Line On Uart     System Initialized

    # 1. Set target velocity
    Write Char On Uart        t
    Write Line To Uart        50
    Wait For Line On Uart     New Target: 50

    # 2. Enable PID Loop
    Write Char On Uart        e
    Wait For Line On Uart     PID Enabled

    # 3. Wait for stability (Target 50 counts/100ms)
    Wait For Line On Uart     VEL: (4[8-9]|5[0-2]) TGT: 50  timeout=60  treatAsRegex=true

    # 4. Capture current output
    ${line}=                  Wait For Line On Uart     VEL: .* TGT: 50 OUT: ([0-9]+)  treatAsRegex=true
    ${initial_output}=        Set Variable  ${line['Groups'][0]}
    Log                       Initial PID output: ${initial_output}

    # 5. Apply Load Torque
    Execute Command           ${MOTOR} LoadTorque 0.05
    Log                       Applied 0.05 N*m Load Torque

    # 6. Verify that PID output increases to compensate
    # It might drop temporarily but should recover or increase PWM to maintain
    Wait For Line On Uart     VEL: (4[8-9]|5[0-2]) TGT: 50  timeout=60  treatAsRegex=true
    ${line}=                  Wait For Line On Uart     VEL: .* TGT: 50 OUT: ([0-9]+)  treatAsRegex=true
    ${new_output}=            Set Variable  ${line['Groups'][0]}
    Log                       New PID output: ${new_output}

    Should Be True            ${new_output} > ${initial_output}

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}
