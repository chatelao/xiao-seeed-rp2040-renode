*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${PWM}                        sysbus.pwm
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Adjust PWM Phase
    [Documentation]           Verifies that PH_ADV and PH_RET affect the counter value.
    [Timeout]                 60 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    Write Char On Uart        H
    ${res}=                   Wait For Line On Uart     PWM Phase Test: C1=([0-9]+) C2=([0-9]+) C3=([0-9]+)  treatAsRegex=true
    Log                       C1=${res['Groups'][0]} C2=${res['Groups'][1]} C3=${res['Groups'][2]}

    ${c1}=                    Convert To Integer    ${res['Groups'][0]}
    ${c2}=                    Convert To Integer    ${res['Groups'][1]}
    ${c3}=                    Convert To Integer    ${res['Groups'][2]}

    Should Be Equal As Integers    ${c1}  1000
    Should Be Equal As Integers    ${c2}  1001
    Should Be Equal As Integers    ${c3}  1000

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
