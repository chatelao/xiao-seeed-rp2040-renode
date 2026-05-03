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
## Phase 6: Continuous Documentation
- [x] Initialize MkDocs documentation structure [2026-05-03]
- [x] Create `mkdocs.yml` configuration [2026-05-03]
- [x] Create `docs/requirements.txt` for documentation dependencies [2026-05-03]
- [x] Create `.readthedocs.yaml` configuration [2026-05-03]
- [x] Integrate ReadTheDocs with GitHub Actions [2026-05-03]
- [ ] Verify automatic documentation updates on ReadTheDocs
