*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/test/renode-config/run_xiao.resc

*** Test Cases ***
Should Integrate Motor Model with PWM and ADC
    [Documentation]           Verifies that PWM output affects Motor Model velocity and ADC readings.
    [Timeout]                 60 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 64

    Sleep                     2s

    Write Char On Uart        G
    ${res}=                   Wait For Line On Uart     Motor ADC: ([0-9]+)  treatAsRegex=true
    ${adc_val}=               Get Regexp Matches  ${res['Groups'][0]}  ([0-9]+)
    Log                       ADC value: ${adc_val[0]}
    Should Be True            ${adc_val[0]} > 0

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
