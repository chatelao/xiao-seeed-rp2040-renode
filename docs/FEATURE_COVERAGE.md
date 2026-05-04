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
| DMA | ❌ | n/a |
| Memory | ✅ | Flash and SRAM mapping supported. |
| Bootrom | ❌ | n/a |
| Clocks | ⚠️ | Basic clock structures defined; fixed frequencies used in simulation. |
| GPIO / Pads | ✅ | Digital IO and interrupt support verified on selected pins (e.g., D8/GPIO2). |
| Resets | ❌ | n/a |

## 3. PIO (Programmable I/O)

| Feature | Status | Details |
| --- | --- | --- |
| PIO State Machines | ❌ | n/a |
| PIO Instruction Set | ❌ | n/a |
| PIO IRQ / DMA | ❌ | n/a |

## 4. Peripherals

| Feature | Status | Details |
| --- | --- | --- |
| 4.1 USB | ❌ | n/a |
| 4.2 UART | ✅ | Bidirectional communication, interrupt support, and Robot Framework integration. |
| 4.3 I2C | ❌ | n/a |
| 4.4 SPI | ❌ | n/a |
| 4.5 PWM | ⚠️ | Functional slices with CSR, DIV, CTR, TOP, CC registers. Missing: double buffering, dynamic 16-bit counter, phase adjustment, and wrap IRQs. |
| 4.6 Timer | ✅ | 64-bit counter, 4 alarms with IRQ support (0-3). |
| 4.7 Watchdog | ❌ | n/a |
| 4.8 RTC | ❌ | n/a |
| 4.9 ADC | ⚠️ | Voltage sampling on 5 channels. Missing: functional round-robin, error logic, and DREQ signaling. |
| 4.10 SSI | ❌ | n/a |

---

*Last updated: 2026-05-04*
