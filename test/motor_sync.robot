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
Should Verify Synchronized ADC Sampling
    [Documentation]           Verifies that ADC samples are captured automatically on PWM wrap interrupts.
    [Timeout]                 60 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # 1. Feed an ADC value to Channel 1 (Motor ADC)
    Execute Command           ${ADC} FeedVoltageSampleToChannel 1 2.5 1000

    # 2. Enable PWM Interrupts
    Write Char On Uart        M
    Wait For Line On Uart     PWM Interrupt Enabled for Slice 0

    # 3. Enable Synchronized ADC Sampling
    Write Char On Uart        H
    Wait For Line On Uart     Sync ADC Enabled

    # 4. Check for the value
    Sleep                     2
    Write Char On Uart        V
    Wait For Line On Uart     Last Sync ADC: 3102

    # 5. Disable synchronization
    Write Char On Uart        H
    Wait For Line On Uart     Sync ADC Disabled

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
