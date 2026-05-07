# Roadmap

The final plan to implement the `CONCEPT.md` and `DESIGN.md` to achieve the top goal.

## Phase 1: Foundation
- [x] Setup Project Structure (specification, src, test) [2026-04-28]
- [x] Initialize Documentation (CONCEPT, DESIGN, ROADMAP, TECHNICAL_DEBTS) [2026-04-28]
- [x] Define Business & Use Cases in `CONCEPT.md` [2026-04-28]
- [x] Define High-Level Architecture in `CONCEPT.md` [2026-04-28]
- [x] Select Technological Choices in `DESIGN.md` [2026-04-28]

## Phase 2: Environment Setup
- [x] Create `src/install.sh` for build tools [2026-04-28]
- [x] Create `test/install.sh` for test tools [2026-04-28]
- [x] Setup CI/CD Pipeline (Github Actions) [2026-04-28]
- [x] Verify CI/CD Pipeline with a dummy test [2026-04-28]

## Phase 3: PlatformIO & Renode Integration
- [x] Extract necessary parts from `Renode_RP2040` repository [2026-04-30]
- [x] Configure PlatformIO for XIAO Seeed RP2040 [2026-04-30]
- [x] Create Renode script for XIAO Seeed RP2040 [2026-04-30]
- [x] Verify basic PlatformIO build and Renode execution [2026-04-30]

## Phase 4: Peripheral Implementation
### UART Support
- [x] Verify basic UART output in Renode [2026-04-30]
- [x] Implement UART input handling in simulation [2026-04-30]
- [x] Add Robot Framework tests for UART bidirectional communication [2026-04-30]

### ADC Support
- [x] Configure ADC in PlatformIO (Arduino/Pico SDK) [2026-05-01]
- [x] Map ADC pins in Renode `.repl` file [2026-05-01]
- [x] Create Robot Framework test for ADC reading with simulated analog values [2026-05-01]

### PWM Support
- [x] Configure PWM in PlatformIO [2026-05-22]
- [x] Implement basic PWM peripheral model in Renode (register stub) [2026-05-01]
- [x] Implement PWM slice register handling (CSR, DIV, CTR, TOP, CC) [2026-05-01]
- [x] Map PWM pins in Renode `.repl` file [2026-05-22]
   - [x] Create Robot Framework test for PWM frequency and duty cycle verification [2026-05-22]

### Interrupt Support
- [x] Configure NVIC and interrupt vectors in Renode [2026-05-02]
- [x] Implement basic interrupt handling in firmware [2026-05-02]
- [x] Create Robot Framework test for interrupt-driven events [2026-05-02]

### Timer Support
- [x] Configure RP2040 Timer peripheral in Renode [2026-05-03]
- [x] Implement timer-based delays and alarms in firmware [2026-05-03]
- [x] Create Robot Framework test for timer accuracy and periodic interrupts [2026-05-03]

## Phase 5: Verification & Documentation
- [x] Finalize test cases for UART, ADC, PWM, Interrupts, and Timer [2026-05-03]
   - [x] Create a unified Robot Framework test suite `test/full_suite.robot` [2026-05-02]
   - [x] Verify the unified test suite passes [2026-05-03]
- [x] Run full CI/CD suite [2026-05-03]
- [x] Update documentation with final design and usage instructions [2026-05-03]
   - [x] Create `USAGE.md` with instructions for building and testing [2026-05-03]
   - [x] Finalize `CONCEPT.md` and `DESIGN.md` [2026-05-03]
- [x] Create `TESTCASES.md` with documented test scenarios [2026-05-04]

## Phase 6: Continuous Documentation
- [x] Initialize MkDocs documentation structure [2026-05-03]
- [x] Create `mkdocs.yml` configuration [2026-05-03]
- [x] Create `docs/requirements.txt` for documentation dependencies [2026-05-03]
- [x] Create `.readthedocs.yaml` configuration [2026-05-03]
- [x] Integrate ReadTheDocs with GitHub Actions [2026-05-03]
- [x] Verify automatic documentation updates on ReadTheDocs [2026-05-06]

## Phase 7: I2C Peripheral Support
- [x] Implement I2C firmware driver using Arduino Wire or Pico SDK [2026-05-04]
- [x] Configure I2C peripherals in Renode `.repl` and `.resc` files [2026-05-04]
- [x] Integrate an existing I2C peripheral model (e.g., PCF8523 or BMP280) for verification [2026-05-04]
- [x] Create Robot Framework tests for I2C communication and sensor reading [2026-05-04]

## Phase 8: Motor Control and bEMF Support
- [x] Draft `docs/BEMF_LOOP.md` for bEMF loop calculation concept [2026-05-05]
- [x] Phase 8.1: Integration of `MotorModel` Renode peripheral [2026-05-06]
    - [x] Create `MotorModel` Renode peripheral model [2026-05-06]
    - [x] Wire the `MotorModel` to PWM outputs and ADC input channels in the XIAO RP2040 `.repl` configuration. [2026-05-06]
    - [x] Implement UART command 'G' in firmware to read Motor ADC channel. [2026-05-06]
    - [x] Verify `MotorModel` integration with Robot Framework tests. [2026-05-06]
- [x] Phase 8.2: Firmware logic for high-precision ADC/PWM synchronization [2026-05-06]
    - [x] Implement synchronization using PWM wrap interrupts.
    - [x] Ensure sample timing alignment with PWM cycles.
- [ ] Phase 8.3: BEMF zero-crossing detection logic
    - [ ] Develop the BEMF zero-crossing detection algorithm and commutation state machine.
- [ ] Create a UART-based logging system for BEMF data and a host-side tool (e.g., Python/Matplotlib) for graphical analysis.
- [ ] Add Robot Framework test cases to verify closed-loop motor commutation and speed stability.

## Phase 9: SPI Peripheral Support
- [x] Implement SPI loopback test in firmware and verify in Renode [2026-05-04]
- [ ] Configure SPI pins and an external SPI device in Renode `.repl`
- [ ] Implement SPI device communication in firmware (e.g., reading JEDEC ID)
- [ ] Create Robot Framework tests for SPI bidirectional data transfer

## Phase 11: PIO Integration
- [x] Draft `docs/PIO_CONCEPT.md` for PIO integration [2026-05-03]
- [x] Implement PIO (Programmable I/O) state machine examples in firmware [2026-05-04]
- [x] Connect PIO outputs to XIAO RP2040 pins in Renode `.repl` [2026-05-04]
- [ ] Implement DMA requests (DREQ) and IRQ routing for PIO in Renode
- [x] Reuse `hello_pio` and `pio_blink` tests from `Renode_RP2040` [2026-05-06]
- [x] Create Robot Framework tests for PIO driving XIAO Seeed RP2040 pins [2026-05-06]

## Phase 12: Advanced Simulation & Performance
- [x] Draft `docs/DMA_CONCEPT.md` for DMA integration [2026-05-04]
- [ ] Implement advanced DMA features (Abort, Pacing Timers, Debug Registers)
- [x] Implement DMA-based data transfer examples (CRC calculation, Block move) [2026-05-06]
- [x] Create Robot Framework tests for basic DMA transfers and interrupts [2026-05-06]
- [ ] Optimize Renode simulation parameters for better host performance
- [ ] Expand `full_suite.robot` with advanced stress tests

## Phase 13: Full PWM Feature Support
- [x] Implement 16-bit dynamic counter in `RP2040PWM` Renode model [2026-05-06]
- [x] Implement double buffering for `CC` and `TOP` registers (latched update on wrap) [2026-05-06]
- [ ] Implement `DIVMODE` support for LEVEL, RISE, and FALL input modes
- [x] Implement IRQ and DREQ signal assertion on counter wrap [2026-05-06]
- [ ] Implement `PH_ADV` and `PH_RET` phase adjustment logic
- [x] Create Robot Framework tests for advanced PWM features (interrupts, inputs) [2026-05-06]

## Phase 14: Full ADC Feature Support
- [x] Fix round-robin logic in `RP2040ADC` to correctly cycle through enabled channels [2026-05-04]
- [x] Implement error generation logic and propagate `ERR` bits to status and FIFO [2026-05-04]
- [x] Correct `READY` flag behavior to remain high during pacing timer delays [2026-05-04]
- [x] Refactor pacing timer to define the total sampling interval (96 cycles vs `DIV` setting) [2026-05-04]
- [x] Implement correct `FIFO` register bit packing including bit 15 (`ERR`) [2026-05-04]
- [x] Align `DMARequest` (DREQ) signaling with `FCS.THRESH` and `FCS.DREQ_EN` logic [2026-05-04]
- [x] Fix stability of ADC error detection and threshold logic tests [2026-05-05]

## Phase 15: Watchdog and RTC Support
- [x] Implement `RP2040Watchdog` Renode model for system supervisor [2026-05-05]
- [x] Implement `RP2040RTC` Renode model for real-time clock functionality [2026-05-06]
- [x] Create firmware examples for Watchdog timeout [2026-05-05]
- [x] Create firmware examples for RTC alarm [2026-05-06]
- [x] Create Robot Framework tests for Watchdog [2026-05-05]
- [x] Create Robot Framework tests for RTC [2026-05-06]

## Phase 16: System Resets and Power Management
- [ ] Implement `RP2040Resets` Renode model for peripheral reset control
- [ ] Implement `RP2040Power` Renode model for power-on reset and sleep states
- [ ] Create firmware examples for peripheral reset and low-power modes
- [ ] Create Robot Framework tests for reset and power management

## Phase 17: USB Support
- [ ] Implement `RP2040USB` Renode model for USB controller
- [ ] Integrate USB core logic and endpoint management
- [ ] Create firmware examples for USB Serial and HID
- [ ] Create Robot Framework tests for USB communication

## Phase 10: Transition to Pico SDK (Technical Debt)
- [ ] Setup Pico SDK build environment in `src/install.sh`
- [ ] Port UART and GPIO drivers to native Pico SDK
- [ ] Port ADC and PWM drivers to native Pico SDK
- [ ] Port Timer and Interrupt handling to native Pico SDK
- [ ] Verify full system functionality with native Pico SDK firmware
