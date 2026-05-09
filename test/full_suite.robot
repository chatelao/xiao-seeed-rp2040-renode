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

    # 11. PWM Interrupts and Double Buffering
    Write Char On Uart        M
    Wait For Line On Uart     PWM Interrupt Enabled for Slice 0
    Wait For Line On Uart     PWM Interrupt Handled
    Wait For Line On Uart     PWM Interrupt Handled
    Wait For Line On Uart     PWM Interrupt Handled

    # 12. PWM RISE Mode and Counter
    Write Char On Uart        J
    Wait For Line On Uart     PWM RISE Mode Enabled
    Write Char On Uart        N
    Wait For Line On Uart     PWM CTR: 0
    # PWM Slice 0 (LED_PIN 17) B input is GPIO 1
    # On RP2040, slice = pin >> 1. 17 >> 1 = 8? Wait.
    # RP2040 has 8 slices (0-7). Slice 0 is GPIO 0,1. Slice 1 is GPIO 2,3...
    # LED_PIN 17 is Slice 0? No, 17/2 = 8, but it wraps.
    # 17 % 16 = 1. 1 >> 1 = 0. So GPIO 16,17 is Slice 0.
    # Input for Slice 0 is GPIO 1 (A) or 1? No.
    # GPIO 0,1 -> Slice 0
    # GPIO 16,17 -> Slice 0
    # For Slice 0, B input is GPIO 1 or GPIO 17.
    # If we use LED_PIN 17, it is B pin of Slice 0.
    # Wait, the C# model uses NumberOfSlices = 8.
    # We added ExternalTrigger(slice, value) to be sure.
    Execute Command           sysbus.pwm ExternalTrigger 0 true
    Execute Command           sysbus.pwm ExternalTrigger 0 false
    Write Char On Uart        N
    Wait For Line On Uart     PWM CTR: 1
    Execute Command           sysbus.pwm ExternalTrigger 0 true
    Execute Command           sysbus.pwm ExternalTrigger 0 false
    Write Char On Uart        N
    Wait For Line On Uart     PWM CTR: 2

    # 13. PWM Phase Adjustment
    Write Char On Uart        L
    Wait For Line On Uart     PWM Phase Advanced
    Write Char On Uart        N
    Wait For Line On Uart     PWM CTR: 1

    # 14. DMA
    Write Char On Uart        D
    Wait For Line On Uart     DMA Transfer Success: DMA TRANSFER TEST  timeout=60
    Wait For Line On Uart     DMA Interrupt Handled  timeout=60

    # DMA Channels
    Write Char On Uart        Z
    Wait For Line On Uart     DMA Channels: 12

    # DMA Pacing
    Write Char On Uart        Y
    Wait For Line On Uart     DMA Pacing Success: PACED DMA

    # DMA Debug
    Write Char On Uart        O
    Wait For Line On Uart     DMA Debug: CH=[0-9]+ TC_PRE=1 TCR_PRE=1 TC_POST=0 TCR_POST=1  treatAsRegex=true

    # 13. Watchdog
    # Enable watchdog
    Write Char On Uart        W
    Wait For Line On Uart     Watchdog Enabled (500ms)

    # Kick watchdog after 200ms
    Sleep                     0.2
    Write Char On Uart        K
    Wait For Line On Uart     Watchdog Kicked

    # Wait for reboot (expecting "Watchdog Reboot Detected")
    Wait For Line On Uart     Watchdog Reboot Detected
    Wait For Line On Uart     Claimed Alarm ID:
    Wait For Line On Uart     UART Bidirectional Communication Ready

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
