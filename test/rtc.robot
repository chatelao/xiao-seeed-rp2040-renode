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
Should Verify RTC Timekeeping
    [Documentation]           Verifies that RTC can be set and increments correctly.
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    Write Char On Uart        R
    Wait For Line On Uart     RTC Time Set: Monday 6 May 12:00:00 2024
    Wait For Line On Uart     RTC Time After 1s: Monday 6 May 12:00:01 2024

Should Verify RTC Alarm
    [Documentation]           Verifies that RTC alarm interrupt fires after 2 seconds.
    Create Machine
    Start Emulation

    Wait For Line On Uart     UART Bidirectional Communication Ready

    Write Char On Uart        Q
    Wait For Line On Uart     Setting RTC Alarm...
    Wait For Line On Uart     RTC Alarm Set for +2s
    # Wait for the alarm to fire (2 seconds simulated time)
    Wait For Line On Uart     RTC Alarm Handled  timeout=5

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
