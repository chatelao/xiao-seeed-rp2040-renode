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
    Execute Command           motor LoadTorque 0.1
    Sleep                     2s

    Write Char On Uart        G
    ${res2}=                  Wait For Line On Uart     Motor ADC: ([0-9]+)  treatAsRegex=true
    ${val2}=                  Set Variable  ${res2['Groups'][0]}
    Log                       ADC value (with load): ${val2}

    Should Be True            ${val2} < ${val1}

*** Keywords ***
Create Machine
    Execute Command           $platform_file = @${CURDIR}/test/renode-config/boards/seeed_xiao_rp2040_motor.repl
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
