# Feature Coverage

This section tracks the implementation status of RP2040 features in the Renode simulation.

## Status Legend
* ✅ **Implemented**: Feature is implemented and verified with tests.
* ⚠️ **Partial**: Feature is partially implemented or has known limitations.
* ❌ **Not Implemented**: Feature is not yet implemented or is a stub.
* **n/a**: Details not applicable for non-implemented parts.

## System Description

| Feature | Status | Details |
| --- | --- | --- |
| Address Map | ✅ | Basic system bus and peripheral mapping implemented. |
| Processor Subsystem | ✅ | Dual Cortex-M0+ cores supported by Renode. |
| DMA | ✅ | Channel logic, memory-to-memory, pacing timers (TIMER0-3), ring buffers, and debug registers implemented. |
| Memory | ✅ | Flash and SRAM mapping supported. |
| Bootrom | ❌ | n/a |
| Clocks | ⚠️ | Basic clock structures defined; fixed frequencies used in simulation. |
| GPIO / Pads | ✅ | Digital IO and interrupt support verified on selected pins (e.g., GPIO 21). |
| Resets | ⚠️ | `RESET`, `WDSEL`, and `RESET_DONE` registers implemented. |

## PIO (Programmable I/O)

| Feature | Status | Details |
| --- | --- | --- |
| PIO State Machines | ✅ | Co-simulation with `libpiosim` supports basic program execution; verified GPIO interaction. |
| PIO Instruction Set | ✅ | Most instructions supported via the piosim engine. |
| PIO IRQ / DMA | ✅ | IRQ and DMA request routing implemented and wired in `rp2040.repl`. |

## Peripherals

| Feature | Status | Details |
| --- | --- | --- |
| USB | ❌ | n/a |
| UART | ✅ | Bidirectional communication, interrupt support, and Robot Framework integration. |
| I2C | ✅ | Verified with BMP280 sensor reading in Robot Framework. |
| SPI | ✅ | Loopback mode supported and verified via 'S' command. |
| PWM | ✅ | Functional slices with 16-bit dynamic counter, double buffering, wrap IRQs, DREQ, DIVMODE, and phase adjustment. |
| Timer | ✅ | 64-bit counter, 4 alarms with IRQ support (0-3). |
| Watchdog | ✅ | Watchdog timer with reboot support verified. |
| RTC | ✅ | Real-time clock with alarm support verified. |
| ADC | ✅ | Full feature support (round-robin, error bits, FIFO packing, DREQ) verified with stabilized tests. |
| Motor Model | ✅ | Brushed DC physics model, PWM/ADC synchronization, and BEMF-based PID control for load compensation. |
| SSI | ❌ | n/a |

*Last updated: 2026-05-25*
