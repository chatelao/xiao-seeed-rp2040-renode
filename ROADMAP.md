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
- [x] Extract necessary parts from `Renode_RP2040` repository [2026-04-29]
- [x] Configure PlatformIO for XIAO Seeed RP2040 [2026-04-29]
- [x] Create Renode script for XIAO Seeed RP2040 [2026-04-29]
- [ ] Verify basic PlatformIO build and Renode execution

## Phase 4: Peripheral Implementation
- [ ] Implement UART support in Renode/PlatformIO
- [ ] Implement ADC support in Renode/PlatformIO
- [ ] Implement PWM support in Renode/PlatformIO

## Phase 5: Verification & Documentation
- [ ] Finalize test cases for UART, ADC, and PWM
- [ ] Run full CI/CD suite
- [ ] Update documentation with final design and usage instructions
