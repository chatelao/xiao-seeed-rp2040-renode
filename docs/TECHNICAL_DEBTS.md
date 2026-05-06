# Technical Debts

List of technical debts identified during the project.

- **Framework Deviation:** Using `arduino` framework instead of `pico-sdk` as initially planned in `DESIGN.md`. This was chosen because the Seeed Studio PlatformIO platform recommends it and it was easier to verify the basic setup. Transitioning to `pico-sdk` remains a future goal.

- **PWM Implementation Gaps:** The `RP2040PWM` Renode model was updated in May 2026 to address previous gaps. Remaining items include:
    - **Input Modes:** While `LEVEL` mode is implemented, `RISE` and `FALL` settings (which use the B pin as a clock source) are not yet implemented cycle-accurately.

- **ADC Implementation Gaps:** The `RP2040ADC` Renode model was improved in May 2026 to address previous gaps in error handling, READY flag behavior, pacing timer accuracy, and DREQ signaling. Remaining items include:
    - **Pacing Timer Precision:** While the interval logic is implemented, the model uses a simple cycle counter which may drift under heavy simulation load.
