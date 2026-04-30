*** Settings ***
Resource                      ${CURDIR}/renode-config/tests/common.resource

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Report Correct ADC Value
    Create Machine
    Create Terminal Tester    ${UART}
    Start Emulation

    # Set ADC value to 1.65V (midpoint of 3.3V)
    # Channel 0 is GPIO26
    Execute Command           sysbus.adc FeedVoltageSampleToChannel 0 1.65 5

    Wait For Line On Uart     ADC Raw: 2047, Voltage: 1.65

    # Set ADC value to 3.3V (max)
    Execute Command           sysbus.adc FeedVoltageSampleToChannel 0 3.3 5
    Wait For Line On Uart     ADC Raw: 4095, Voltage: 3.30

    # Set ADC value to 0V (min)
    Execute Command           sysbus.adc FeedVoltageSampleToChannel 0 0.0 5
    Wait For Line On Uart     ADC Raw: 0, Voltage: 0.00

*** Keywords ***
Create Machine
    Execute Command           $global.FIRMWARE = @${FIRMWARE}
    Execute Command           include @${RESC}
