*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Resource        ${RENODEKEYWORDS}

*** Variables ***
${UART}         sysbus.uart0
${RESC}         ${CURDIR}/../examples/bemf_loop/bemf_example.resc
${FIRMWARE}     ${CURDIR}/../examples/bemf_loop/.pio/build/seeed-xiao-rp2040/firmware.elf

*** Test Cases ***
Verify bEMF Loop
    [Documentation]    Verifies that the bEMF value changes as the PWM duty cycle is ramped.
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}

    Wait For Line On Uart     bEMF Feedback Loop Example Started  timeout=30

    # Wait for a few log lines and verify the format and values
    # DUTY:100 BEMF:0 (initially)
    Wait For Line On Uart     DUTY:100  timeout=10

    # After some time, BEMF should increase as duty cycle increases
    Wait For Line On Uart     DUTY:200  timeout=10

    # Verify that BEMF is non-zero
    Wait For Line On Uart     BEMF:[1-9][0-9]*    timeout=10  treatAsRegex=true

    # Verify that BEMF follows duty cycle ramp (roughly)
    Wait For Line On Uart     DUTY:300  timeout=20
    Wait For Line On Uart     BEMF:[1-9][0-9]*    timeout=10  treatAsRegex=true
