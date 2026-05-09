# Feature Coverage

This document tracks the implementation status of RP2040 features in the Renode simulation, based on the [RP2040 Datasheet](specification/rp2040-datasheet/03_chapter_1_introduction.md).

## Status Legend
* ✅ **Implemented**: Feature is implemented and verified with tests.
* ⚠️ **Partial**: Feature is partially implemented or has known limitations.
* ❌ **Not Implemented**: Feature is not yet implemented or is a stub.
* **n/a**: Details not applicable for non-implemented parts.

---

## 2. System Description

| Feature | Status | Details |
| --- | --- | --- |
| Address Map | ✅ | Basic system bus and peripheral mapping implemented. |
| Processor Subsystem | ✅ | Dual Cortex-M0+ cores supported by Renode. |
| DMA | ✅ | Channel logic, memory-to-memory, pacing timers (TIMER0-3), and debug registers implemented. |
| Memory | ✅ | Flash and SRAM mapping supported. |
| Bootrom | ❌ | n/a |
| Clocks | ⚠️ | Basic clock structures defined; fixed frequencies used in simulation. |
| GPIO / Pads | ✅ | Digital IO and interrupt support verified on selected pins (e.g., GPIO 21). |
| Resets | ❌ | n/a |

## 3. PIO (Programmable I/O)

| Feature | Status | Details |
| --- | --- | --- |
| PIO State Machines | ⚠️ | Co-simulation with `libpiosim` supports basic program execution; verified GPIO interaction. |
| PIO Instruction Set | ✅ | Most instructions supported via the piosim engine. |
| PIO IRQ / DMA | ❌ | IRQ and DMA request routing not yet implemented in Renode model. |

## 4. Peripherals

| Feature | Status | Details |
| --- | --- | --- |
| 4.1 USB | ❌ | n/a |
| 4.2 UART | ✅ | Bidirectional communication, interrupt support, and Robot Framework integration. |
| 4.3 I2C | ✅ | Verified with BMP280 sensor reading in Robot Framework. |
| 4.4 SPI | ⚠️ | Loopback mode supported and verified. |
| 4.5 PWM | ⚠️ | Functional slices with double buffering, dynamic 16-bit counter, and wrap IRQs. Missing: DIVMODE and phase adjustment. |
| 4.6 Timer | ✅ | 64-bit counter, 4 alarms with IRQ support (0-3). |
| 4.7 Watchdog | ✅ | Watchdog timer with reboot support verified. |
| 4.8 RTC | ✅ | Real-time clock with alarm support verified. |
| 4.9 ADC | ✅ | Full feature support (round-robin, error bits, FIFO packing, DREQ) verified with stabilized tests. |
| 4.10 SSI | ❌ | n/a |

---

*Last updated: 2026-05-10*
