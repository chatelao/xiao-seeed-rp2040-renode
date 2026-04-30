*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Echo UART Input
    Create Machine
    Start Emulation
    Wait For Line On Uart     UART Bidirectional Communication Ready

    Write Char On Uart        X
    Wait For Line On Uart     Echo: X

    Write Char On Uart        Y
    Wait For Line On Uart     Echo: Y

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
