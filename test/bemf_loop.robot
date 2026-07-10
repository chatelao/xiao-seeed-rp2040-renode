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

    Wait For Line On Uart     Bidirectional bEMF Loop Example Started  timeout=60

    # Wait for a few log lines and verify the format and values
    # DIR:F DUTY:100 bEMF_A:0 (initially)
    Wait For Line On Uart     DIR:F DUTY:100  timeout=30

    # After some time, BEMF should increase as duty cycle increases
    Wait For Line On Uart     DIR:F DUTY:200  timeout=120

    # Verify that BEMF_A is non-zero
    Wait For Line On Uart     bEMF_A:[1-9][0-9]*    timeout=60  treatAsRegex=true

    # Verify that BEMF follows duty cycle ramp (roughly)
    Wait For Line On Uart     DIR:F DUTY:300  timeout=300
    Wait For Line On Uart     bEMF_A:[1-9][0-9]*    timeout=60  treatAsRegex=true

    # Direction Swap
    Wait For Line On Uart     Direction: REVERSE  timeout=600
    Wait For Line On Uart     DIR:R DUTY:100  timeout=60
