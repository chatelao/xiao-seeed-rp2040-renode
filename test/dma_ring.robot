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
Should Verify DMA Ring Buffer and Sniffer Global Enable
    [Documentation]           Verifies that DMA ring wrapping works correctly and sniffer respects global enable.
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Trigger 'C' command for Ring and Sniffer tests
    Write Char On Uart        C

    # 0-255 sum is 32640. Wrapping twice (512 bytes) should be 65280.
    Wait For Line On Uart     DMA Ring Test: SUM=65280

    # Global disable should prevent sniffer from updating even if channel sniff is enabled
    Wait For Line On Uart     DMA Sniff Disable Test: SUM=0

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
