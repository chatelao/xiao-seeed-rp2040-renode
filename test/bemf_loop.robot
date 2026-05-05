*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Resource        ${RENODEKEYWORDS}

*** Variables ***
${UART}         sysbus.uart0
${RESC}         ${CURDIR}/../examples/bemf_loop/bemf_example.resc

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
    # We look for a line where BEMF is significantly above 0
    Wait For Line On Uart     DUTY:200  timeout=10
    Wait For Line On Uart     BEMF:([1-9][0-9]*)    treatAsRegexp=true

    # Verify that BEMF follows duty cycle ramp (roughly)
    ${line}=    Wait For Line On Uart     DUTY:300  timeout=10
    Should Match Regexp    ${line}    BEMF:([1-9][0-9]*)
