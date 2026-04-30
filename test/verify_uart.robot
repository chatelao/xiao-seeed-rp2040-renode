*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Resource                      ${CURDIR}/renode-config/tests/common.resource

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Print Hello World
    Create Machine
    Start Emulation
    Wait For Line On Uart     Hello from XIAO RP2040!

*** Keywords ***
Setup
    ${RENODE_PATH}=           Set Variable    ${CURDIR}/renode/renode
    Setup Renode              ${RENODE_PATH}

Teardown
    Teardown Renode

Create Machine
    Execute Command           $global.FIRMWARE = @${FIRMWARE}
    Execute Command           include @${RESC}
