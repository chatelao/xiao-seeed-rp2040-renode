*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${FIRMWARE}                   ${CURDIR}/../examples/motor_pid_encoder/.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/../examples/motor_pid_encoder/motor_encoder.resc

*** Test Cases ***
Should Initialize Motor PID Encoder Example
    [Documentation]           Verifies that the motor PID encoder example starts and initializes correctly.
    [Timeout]                 120 seconds
    Create Machine
    Start Emulation

    Wait For Line On Uart     Motor PID Encoder Example Started  timeout=60
    Wait For Line On Uart     System Initialized  timeout=60

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}
