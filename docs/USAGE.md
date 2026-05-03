# Usage Guide

This guide provides instructions for setting up the environment, building the firmware, and running the simulation tests for the XIAO Seeed RP2040 project.

## Prerequisites

- Python 3.8 or newer
- `pip` (Python package installer)

## Environment Setup

### 1. Install Build Tools
To install PlatformIO and other necessary build tools, run:
```bash
chmod +x src/install.sh
./src/install.sh
```

### 2. Install Test Tools
To install Renode and the required Python libraries for testing, run:
```bash
chmod +x test/install.sh
./test/install.sh
```

## Building the Firmware

To compile the firmware using PlatformIO, execute:
```bash
pio run
```
The compiled ELF file will be located at `.pio/build/seeed-xiao-rp2040/firmware.elf`.

## Running Simulations and Tests

### 1. Run Unified Test Suite
We use Robot Framework with Renode for automated testing. To run the full suite of peripheral tests:
```bash
./test/renode/renode-test test/full_suite.robot
```

### 2. Manual Simulation
To run the simulation manually in Renode and interact with the UART:
```bash
./test/renode/renode -e "include @test/renode-config/run_xiao.resc"
```

## Peripheral Support Status
- **UART:** Bidirectional communication verified.
- **ADC:** Analog reading from A0 (GPIO 26) verified.
- **PWM:** Frequency and duty cycle verification on LED_PIN (GPIO 17) verified.
- **Interrupts:** GPIO interrupt handling (GPIO 2) verified.
- **Timer:** Hardware timer alarms (one-shot and periodic) verified.
