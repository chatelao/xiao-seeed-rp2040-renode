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
- [ ] Create `TESTCASES.md` with documented test scenarios
## Phase 6: Continuous Documentation
- [x] Initialize MkDocs documentation structure [2026-05-03]
- [x] Create `mkdocs.yml` configuration [2026-05-03]
- [x] Create `docs/requirements.txt` for documentation dependencies [2026-05-03]
- [x] Create `.readthedocs.yaml` configuration [2026-05-03]
- [x] Integrate ReadTheDocs with GitHub Actions [2026-05-03]
- [ ] Verify automatic documentation updates on ReadTheDocs

## Phase 7: I2C Peripheral Support
- [ ] Implement I2C firmware driver using Arduino Wire or Pico SDK
- [ ] Configure I2C peripherals in Renode `.repl` and `.resc` files
- [ ] Integrate an existing I2C peripheral model (e.g., PCF8523 or BMP280) for verification
- [ ] Create Robot Framework tests for I2C communication and sensor reading

## Phase 8: SPI Peripheral Support
- [ ] Implement SPI firmware driver
- [ ] Configure SPI peripherals and chip selects in Renode `.repl`
- [ ] Integrate a simulated SPI device (e.g., external flash or display)
- [ ] Create Robot Framework tests for SPI bidirectional data transfer

## Phase 9: Transition to Pico SDK (Technical Debt)
- [ ] Setup Pico SDK build environment in `src/install.sh`
- [ ] Port UART and GPIO drivers to native Pico SDK
- [ ] Port ADC and PWM drivers to native Pico SDK
- [ ] Port Timer and Interrupt handling to native Pico SDK
- [ ] Verify full system functionality with native Pico SDK firmware

## Phase 10: PIO Integration
- [x] Draft `docs/PIO_CONCEPT.md` for PIO integration [2026-05-03]
- [ ] Implement PIO (Programmable I/O) state machine examples in firmware
- [ ] Connect PIO outputs to XIAO RP2040 pins in Renode `.repl`
- [ ] Implement DMA requests (DREQ) and IRQ routing for PIO in Renode
- [ ] Reuse `hello_pio` and `pio_blink` tests from `Renode_RP2040`
- [ ] Create Robot Framework tests for PIO driving XIAO Seeed RP2040 pins

## Phase 11: Advanced Simulation & Performance
- [ ] Implement DMA-based data transfer examples
- [ ] Optimize Renode simulation parameters for better host performance
- [ ] Expand `full_suite.robot` with advanced stress tests

## Phase 12: Full PWM Feature Support
- [ ] Implement 16-bit dynamic counter in `RP2040PWM` Renode model
- [ ] Implement double buffering for `CC` and `TOP` registers (latched update on wrap)
- [ ] Implement `DIVMODE` support for LEVEL, RISE, and FALL input modes
- [ ] Implement IRQ and DREQ signal assertion on counter wrap
- [ ] Implement `PH_ADV` and `PH_RET` phase adjustment logic
- [ ] Create Robot Framework tests for advanced PWM features (interrupts, inputs)

## Phase 12: Full ADC Feature Support
- [ ] Fix round-robin logic in `RP2040ADC` to correctly cycle through enabled channels
- [ ] Implement error generation logic and propagate `ERR` bits to status and FIFO
- [ ] Correct `READY` flag behavior to remain high during pacing timer delays
- [ ] Refactor pacing timer to define the total sampling interval (96 cycles vs `DIV` setting)
- [ ] Implement correct `FIFO` register bit packing including bit 15 (`ERR`)
- [ ] Align `DMARequest` (DREQ) signaling with `FCS.THRESH` and `FCS.DREQ_EN` logic
- [ ] Create Robot Framework tests for round-robin sampling and pacing timer accuracy
