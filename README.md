# XIAO Seeed RP2040 Renode Project

[![Documentation Status](https://readthedocs.org/projects/xiao-seeed-rp2040-renode/badge/?version=latest)](https://xiao-seeed-rp2040-renode.readthedocs.io/en/latest/?badge=latest)

## Overview
Welcome to the XIAO Seeed RP2040 Renode project. This project provides a simulation environment for the Seeed Studio XIAO RP2040 using Renode, integrated with PlatformIO.

## Documentation
The full documentation is available at:
**[https://xiao-seeed-rp2040-renode.readthedocs.io/](https://xiao-seeed-rp2040-renode.readthedocs.io/)**

## Goal
Create a setup for the XIAO Seeed RP2040 able to run the UART, the ADC, the PWM, the Interrupts and the Timer on Renode over PlatformIO.

## Project Structure
- `docs/`: Project documentation and specifications.
- `examples/`: Example projects and BEMF loop implementation.
- `src/`: Firmware source code.
- `test/`: Renode configuration files, peripheral models, and Robot Framework tests.
- `specification/`: Datasheets and technical specifications (converted to Markdown).

## License
This project is licensed under the MIT License - see the LICENSE file for details (if available).

## Feature Coverage

This section tracks the implementation status of RP2040 features in the Renode simulation.

### Status Legend
* ✅ **Implemented**: Feature is implemented and verified with tests.
* ⚠️ **Partial**: Feature is partially implemented or has known limitations.
* ❌ **Not Implemented**: Feature is not yet implemented or is a stub.
* **n/a**: Details not applicable for non-implemented parts.

### System Description

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

### PIO (Programmable I/O)

| Feature | Status | Details |
| --- | --- | --- |
| PIO State Machines | ⚠️ | Co-simulation with `libpiosim` supports basic program execution; verified GPIO interaction. |
| PIO Instruction Set | ✅ | Most instructions supported via the piosim engine. |
| PIO IRQ / DMA | ❌ | IRQ and DMA request routing not yet implemented in Renode model. |

### Peripherals

| Feature | Status | Details |
| --- | --- | --- |
| USB | ❌ | n/a |
| UART | ✅ | Bidirectional communication, interrupt support, and Robot Framework integration. |
| I2C | ✅ | Verified with BMP280 sensor reading in Robot Framework. |
| SPI | ⚠️ | Loopback mode supported and verified. |
| PWM | ⚠️ | Functional slices with double buffering, dynamic 16-bit counter, and wrap IRQs. Missing: DIVMODE and phase adjustment. |
| Timer | ✅ | 64-bit counter, 4 alarms with IRQ support (0-3). |
| Watchdog | ✅ | Watchdog timer with reboot support verified. |
| RTC | ✅ | Real-time clock with alarm support verified. |
| ADC | ✅ | Full feature support (round-robin, error bits, FIFO packing, DREQ) verified with stabilized tests. |
| SSI | ❌ | n/a |

---

*Last updated: 2026-05-10*
