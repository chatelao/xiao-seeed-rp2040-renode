# Renode Simulation Issues

## Performance Bottlenecks
During the implementation of ADC support, significant performance issues were encountered in the Renode simulation for the XIAO Seeed RP2040.

### Symptoms
- Virtual time progress is extremely slow compared to host time (e.g., 1s virtual time takes ~50s host time).
- Robot Framework tests frequently time out even with 300s or 600s limits.
- Log outputs show frequent warnings about unimplemented registers (`VREG_AND_CHIP_RESET`, `SYSCFG`, `USBCTRL_REGS`, etc.) and `xip_ssi.xip_flash` unexpected states.

### Attempted Mitigations
- **Sampling Frequency:** Increased ADC sampling rate in firmware from 2s to 50ms to trigger more virtual events.
- **Renode Quantum:** Adjusted `emulation SetGlobalQuantum` between `0.0001` and `0.01`. Higher values (e.g., `0.01`) improved virtual time progress slightly but still didn't reach real-time.
- **Repeat Samples:** Used `FeedVoltageSampleToChannel` with high repeat counts to ensure samples are available when the firmware eventually polls the ADC.

### Observations
The `Arduino` framework for RP2040 seems to perform many register accesses that Renode's current RP2040 model doesn't fully handle or handles with high overhead. Specifically, the XIP flash and clock management seem to generate a high volume of warnings/unimplemented access logs.

## Test Status
- `test/verify_uart.robot`: **PASS** (re-verified after ADC changes).
- `test/test_uart_bidirectional.robot`: **PASS** (re-verified after ADC changes).
- `test/verify_adc.robot`: **TIMEOUT** (fails to reach all expected lines within 5-10 minutes of host time, despite the firmware being correctly implemented and printing initial ADC lines in the log).

### Recommendation
- Investigate Renode's RP2040 model performance with the Arduino Earle Philhower core.
- Consider switching to a more minimal SDK or optimizing the board description if the overhead remains too high for CI/CD.
