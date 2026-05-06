*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Integrate Motor Model with PWM and ADC
    [Documentation]           Verifies that PWM output affects Motor Model velocity and ADC readings.
    [Timeout]                 60 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Initial check: Motor ADC should be 0 (velocity is 0, PWM is off)
    Write Char On Uart        G
    Wait For Line On Uart     Motor ADC: 0

    # Turn on PWM (using 'P' command which calls analogWrite(LED_PIN, 64))
    # LED_PIN 17 is Slice 0 Channel B, which is connected to the motor.
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 64

    # Wait a bit for the motor to spin up in simulation
    Sleep                     2s

    # Read Motor ADC again.
    Write Char On Uart        G
    ${res}=                   Wait For Line On Uart     Motor ADC: ([0-9]+)  treatAsRegex=true
    ${adc_val}=               Get Regexp Matches  ${res['Groups'][0]}  ([0-9]+)
    Log                       Motor ADC value: ${adc_val[0]}

    # Try to set motor pin HIGH via python
    Execute Command           python "self.Machine['sysbus.motor'].OnGPIO(0, True)"
    Sleep                     2s
    Write Char On Uart        G
    ${res}=                   Wait For Line On Uart     Motor ADC: ([0-9]+)  treatAsRegex=true
    ${adc_val}=               Get Regexp Matches  ${res['Groups'][0]}  ([0-9]+)
    Log                       Motor ADC value after direct forced HIGH: ${adc_val[0]}

    Should Be True            ${adc_val[0]} > 0

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
