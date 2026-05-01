*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${ADC}                        sysbus.adc
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Read Analog Value From ADC
    Create Machine
    Start Emulation

    # 1.65V should be approx 2048 (half of 4095 for 12-bit)
    Execute Command           ${ADC} FeedVoltageSampleToChannel 0 1.65 1

    # Wait for the initial UART message
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Trigger ADC read
    Write Char On Uart        A

    # Verify ADC result
    Wait For Line On Uart     ADC0: 2048

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
