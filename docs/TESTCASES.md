# Test Cases

This document describes the test cases implemented for the XIAO Seeed RP2040 simulation in Renode, as executed by the `test/full_suite.robot` Robot Framework suite.

## Automated Test Suite: `test/full_suite.robot`

The full test suite validates the integration of the firmware with various RP2040 peripheral models in Renode.

### 1. System Initialization
- **Goal**: Verify the firmware boots correctly and initializes basic subsystems.
- **Verification**:
  - Wait for "Claimed Alarm ID:" on UART (Hardware Timer initialization).
  - Wait for "UART Bidirectional Communication Ready" on UART.

### 2. UART Peripheral
- **UART Periodic Message**:
  - **Goal**: Verify UART output is working correctly.
  - **Verification**: Wait for "Hello from XIAO RP2040!" periodic message.
- **UART Bidirectional Communication**:
  - **Goal**: Verify UART input handling and echo.
  - **Action**: Write character 'X' to UART.
  - **Verification**: Wait for "Echo: X" response.

### 3. ADC (Analog-to-Digital Converter)
- **Goal**: Verify ADC reading from a simulated voltage source.
- **Action**:
  - Feed 1.65V to ADC Channel 0 (GPIO 26) via Renode monitor.
  - Write character 'A' to UART to trigger ADC read.
- **Verification**: Wait for "ADC0: 2048" on UART (1.65V is mid-range for 12-bit ADC with 3.3V VREF).

### 4. PWM (Pulse Width Modulation)
- **Goal**: Verify PWM duty cycle calculation for the RED LED (GPIO 17).
- **Action**: Write character 'P' to UART to set PWM value to 64.
- **Verification**:
  - Wait for "PWM set to: 64" on UART.
  - Use Renode's `GetDutyCycle` command to verify the duty cycle is approximately 0.25 (25%).

### 5. I2C (Inter-Integrated Circuit)
- **Goal**: Verify I2C communication with a simulated BMP280 sensor.
- **Action**: Write character 'I' to UART to request BMP280 ID.
- **Verification**: Wait for "BMP280 ID: 0x58" on UART (0x58 is the default Chip ID for BMP280).

### 6. GPIO Interrupts
- **Goal**: Verify GPIO interrupt handling on a specific pin.
- **Action**: Trigger a RISING edge on GPIO 21 (XIAO D6) via Renode monitor (`sysbus.gpio OnGPIO 21 true`).
- **Verification**: Wait for "GPIO Interrupt Handled" on UART.

### 7. SPI (Serial Peripheral Interface)
- **SPI Loopback**:
  - **Goal**: Verify SPI bidirectional data transfer using loopback mode.
  - **Action**: Write character 'S' to UART.
  - **Verification**: Wait for "SPI Loopback Success: 0xBC" on UART.

### 8. Hardware Timer
- **One-shot Alarm**:
  - **Goal**: Verify one-shot timer alarm triggering.
  - **Action**: Write character 'T' to UART.
  - **Verification**:
    - Wait for "One-shot Timer Alarm Set for 100ms".
    - Wait for "Timer Alarm Handled" after the delay.
- **Periodic Alarm**:
  - **Goal**: Verify periodic timer alarm rescheduling.
  - **Action**: Write character 'U' to UART to start.
  - **Verification**: Wait for multiple "Timer Alarm Handled" messages.
  - **Action**: Write character 'U' again to stop.
  - **Verification**: Wait for "Periodic Timer Stopped".
