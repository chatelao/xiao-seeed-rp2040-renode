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
Should Verify Dual Core Operation
    [Documentation]           Verifies CPUID, FIFO communication between cores, and spinlock functionality.
    [Timeout]                 60 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Send 'x' command for dual core verification
    Write Char On Uart        x

    # Verify Core 0 CPUID
    Wait For Line On Uart     CPUID: 0

    # Verify FIFO echo from Core 1
    Wait For Line On Uart     FIFO Echo: 101

    # Verify Spinlock test
    Wait For Line On Uart     Spinlock Test Passed

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
