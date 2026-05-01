*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${PWM}                        sysbus.pwm
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Cycle PWM Duty Cycle
    Create Machine
    Start Emulation

    # Wait for the initial UART message
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Cycle 1: Set PWM to 64
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 64
    # Slice 0, Channel B (GPIO 17)
    # 64 / 255 approx 25% duty cycle
    # Arduino uses TOP=255 for analogWrite
    Verify Duty Cycle         0  B  0.25

    # Cycle 2: Set PWM to 128
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 128
    # 128 / 255 approx 50% duty cycle
    Verify Duty Cycle         0  B  0.50

    # Cycle 3: Set PWM to 192
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 192
    # 192 / 255 approx 75% duty cycle
    Verify Duty Cycle         0  B  0.75

    # Cycle 4: Set PWM to 0 (back to start but 0)
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 0
    Verify Duty Cycle         0  B  0.0

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}

Verify Duty Cycle
    [Arguments]               ${slice}  ${channel}  ${expected_dc}
    ${res}=                   Execute Command    sysbus.pwm GetDutyCycle${channel} ${slice}
    # Log the result for debugging
    Log                       Raw Duty Cycle: ${res}
    ${res}=                   Evaluate    """${res}""".strip()
    Log                       Stripped Duty Cycle: ${res}
    # Use Should Be True for float comparison with tolerance
    # Renode Execute Command returns a string, so we convert it
    Should Be True            abs(float("${res}") - ${expected_dc}) < 0.05
