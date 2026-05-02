*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${ADC}                        sysbus.adc
${PWM}                        sysbus.pwm
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Print Hello World Periodically
    Create Machine
    Start Emulation
    Wait For Line On Uart     Hello from XIAO RP2040!

Should Echo UART Input
    Create Machine
    Start Emulation
    Wait For Line On Uart     UART Bidirectional Communication Ready

    Write Char On Uart        X
    Wait For Line On Uart     Echo: X

    Write Char On Uart        Y
    Wait For Line On Uart     Echo: Y

Should Read Analog Value From ADC
    Create Machine
    Start Emulation

    # 1.65V should be approx 2048 (half of 4095 for 12-bit)
    Execute Command           ${ADC} FeedVoltageSampleToChannel 0 1.65 1

    # Wait for the initial UART message
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Trigger ADC read
    Write Char On Uart        A

    # Verify ADC result
    Wait For Line On Uart     ADC0: 2048

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

Should Trigger GPIO Interrupt
    Create Machine
    Start Emulation

    # Wait for the initial UART message
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Initial state should be High (Pullup)
    Execute Command           sysbus.gpio.gpio2 SetValue true

    # Trigger Falling Edge
    Execute Command           sysbus.gpio.gpio2 SetValue false

    # Verify Interrupt handler output
    Wait For Line On Uart     GPIO Interrupt Triggered!

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}

Verify Duty Cycle
    [Arguments]               ${slice}  ${channel}  ${expected_dc}
    ${res}=                   Execute Command    sysbus.pwm GetDutyCycle${channel} ${slice}
    Log                       Raw Duty Cycle: ${res}
    ${res}=                   Evaluate    """${res}""".strip()
    Log                       Stripped Duty Cycle: ${res}
    Should Be True            abs(float("${res}") - ${expected_dc}) < 0.05
