*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Resource        ${RENODEKEYWORDS}

*** Variables ***
${UART}         sysbus.uart0
${RESC}         ${CURDIR}/../examples/pio_blink/pio_blink.resc
${FIRMWARE}     ${CURDIR}/../examples/pio_blink/.pio/build/seeed-xiao-rp2040/firmware.elf

*** Test Cases ***
Verify PIO Blink Example
    [Documentation]    Verifies that the PIO program starts and we see some activity.
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}

    Wait For Line On Uart     PIO Blink Example Started  timeout=30
    Wait For Line On Uart     Loaded PIO program at offset  timeout=10
    Wait For Line On Uart     PIO Blinking Enabled  timeout=10

    # Verify periodic heartbeat as a sign of continued execution
    Wait For Line On Uart     PIO Blink Example Running  timeout=20
