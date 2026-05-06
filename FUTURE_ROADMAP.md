# Future phasese - DO NOT IMPLEMENT YET

## Phase 17: USB Support
- [ ] Implement `RP2040USB` Renode model for USB controller
- [ ] Integrate USB core logic and endpoint management
- [ ] Create firmware examples for USB Serial and HID
- [ ] Create Robot Framework tests for USB communication
- [ ] 
## Phase 9: SPI Peripheral Support
- [x] Implement SPI loopback test in firmware and verify in Renode [2026-05-04]
- [ ] Configure SPI pins and an external SPI device in Renode `.repl`
- [ ] Implement SPI device communication in firmware (e.g., reading WHO_AM_I)
- [ ] Create Robot Framework tests for SPI bidirectional data transfer

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
