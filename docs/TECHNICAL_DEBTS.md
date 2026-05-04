# Technical Debts

List of technical debts identified during the project.

- **Framework Deviation:** Using `arduino` framework instead of `pico-sdk` as initially planned in `DESIGN.md`. This was chosen because the Seeed Studio PlatformIO platform recommends it and it was easier to verify the basic setup. Transitioning to `pico-sdk` remains a future goal.

- **PWM Implementation Gaps:** The current `RP2040PWM` Renode model is a simplified functional representation and lacks several hardware features:
    - **Double Buffering:** `CC` and `TOP` registers update immediately instead of being latched and updated only on counter wrap.
    - **Dynamic Counter:** The model uses a static frequency/duty cycle calculation rather than a cycle-accurate 16-bit incrementing counter.
    - **Input Modes:** `DIVMODE` settings (LEVEL, RISE, FALL) that use the B pin as a clock gate or clock source are not implemented.
    - **Phase Adjustment:** `PH_ADV` and `PH_RET` bits are non-functional.
    - **Interrupt/DMA Triggering:** IRQ and DREQ signals are not asserted on counter wrap.
    - **CSR Bit Mapping:** The `CHx_CSR` register bit mapping is currently shifted: Bits 2-3 are mapped to `DIVMODE` (should be 4-5), Bit 4 to `A_INV` (should be 2), and Bit 5 to `B_INV` (should be 3).

- **ADC Implementation Gaps:** The current `RP2040ADC` Renode model has several functional gaps compared to the RP2040 datasheet:
    - **Broken Round-Robin Logic:** The `IterateInput` method starts searching from the current `selectedInput`, causing it to get stuck on the same channel if it is included in the `RROBIN` mask.
    - **Missing Error Handling:** There is no logic to generate or propagate the `ERR` bit in the `CS` status or the `FIFO` register, although the registers and fields are defined.
    - **Inaccurate READY Behavior:** The `READY` flag remains low during pacing timer delays, whereas it should be high whenever the ADC is not actively performing a conversion.
    - **Pacing Timer Implementation:** The model adds the pacing delay *before* the 96-cycle sampling period instead of using it to define the total trigger interval (`1 + INT + FRAC / 256`).
    - **FIFO Register Bit Packing:** The `FIFO` register read logic does not correctly pack the `ERR` bit (bit 15) when `FCS.ERR` is enabled.
    - **DREQ Logic:** The `DMARequest` signal toggles on every sample produced, ignoring the `FCS.THRESH` setting.
