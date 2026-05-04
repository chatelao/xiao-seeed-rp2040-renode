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
Should Pass Full Test Suite
    [Documentation]           Runs all peripheral tests in a single session to avoid Renode assembly reload issues.
    [Timeout]                 1200 seconds
    Create Machine
    Start Emulation

    # 1. Initialization and Claimed Alarm ID
    Wait For Line On Uart     Claimed Alarm ID:
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # 2. UART Periodic Message
    Wait For Line On Uart     Hello from XIAO RP2040!

    # 3. UART Bidirectional
    Write Char On Uart        X
    Wait For Line On Uart     Echo: X

    # 4. ADC
    Execute Command           ${ADC} FeedVoltageSampleToChannel 0 1.65 1
    Write Char On Uart        A
    Wait For Line On Uart     ADC0: 2048

    # 5. PWM
    Write Char On Uart        P
    Wait For Line On Uart     PWM set to: 64
    Verify Duty Cycle         0  B  0.25

    # 6. I2C (BMP280)
    Write Char On Uart        I
    Wait For Line On Uart     BMP280 ID: 0x58

    # 7. GPIO Interrupt
    Execute Command           sysbus.gpio OnGPIO 21 true
    Wait For Line On Uart     GPIO Interrupt Handled

    # 8. SPI Loopback
    Write Char On Uart        S
    Wait For Line On Uart     SPI Loopback Success: 0xBC

    # 9. Timer Alarms
    # One-shot
    Write Char On Uart        T
    Wait For Line On Uart     One-shot Timer Alarm Set for 100ms
    Wait For Line On Uart     Timer Alarm Handled

    # Periodic
    Write Char On Uart        U
    Wait For Line On Uart     Periodic Timer Started (100ms)
    Wait For Line On Uart     Timer Alarm Handled
    Wait For Line On Uart     Timer Alarm Handled
    Wait For Line On Uart     Timer Alarm Handled
    Write Char On Uart        U
    Wait For Line On Uart     Periodic Timer Stopped

    # 10. PIO Blink
    Write Char On Uart        B
    Wait For Line On Uart     PIO Blinking Started
    Create LED Tester         sysbus.gpio.led_green
    Assert LED Is Blinking    testDuration=2  onDuration=0.5  offDuration=0.5  tolerance=0.1
    Write Char On Uart        B
    Wait For Line On Uart     PIO Blinking Stopped

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
