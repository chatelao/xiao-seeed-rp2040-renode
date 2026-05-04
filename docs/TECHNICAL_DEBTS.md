# Technical Debts

List of technical debts identified during the project.

- **Framework Deviation:** Using `arduino` framework instead of `pico-sdk` as initially planned in `DESIGN.md`. This was chosen because the Seeed Studio PlatformIO platform recommends it and it was easier to verify the basic setup. Transitioning to `pico-sdk` remains a future goal.

- **PWM Implementation Gaps:** The current `RP2040PWM` Renode model is a simplified functional representation and lacks several hardware features:
    - **Double Buffering:** `CC` and `TOP` registers update immediately instead of being latched and updated only on counter wrap.
    - **Dynamic Counter:** The model uses a static frequency/duty cycle calculation rather than a cycle-accurate 16-bit incrementing counter.
    - **Input Modes:** `DIVMODE` settings (LEVEL, RISE, FALL) that use the B pin as a clock gate or clock source are not implemented.
    - **Phase Adjustment:** `PH_ADV` and `PH_RET` bits are non-functional.
    - **Interrupt/DMA Triggering:** IRQ and DREQ signals are not asserted on counter wrap.

- **ADC Implementation Gaps:** The `RP2040ADC` Renode model was improved in May 2026 to address previous gaps in error handling, READY flag behavior, pacing timer accuracy, and DREQ signaling. Remaining items include:
    - **Pacing Timer Precision:** While the interval logic is implemented, the model uses a simple cycle counter which may drift under heavy simulation load.
