# Design

The detailed design of the solution, including the architecture, used tech stack for development, production and testing, etc.

## Technological Choices

### Choice 3: Programming SDK
- **Option A: Raspberry Pi Pico SDK (Selected)** - Native SDK for RP2040, providing low-level peripheral access and high performance. Used extensively in the reference `Renode_RP2040` repository.

### Choice 4: Testing Framework
- **Option A: Robot Framework (Selected)** - Integration and system testing framework used for Renode simulations. Provides clear test reports and is used in the reference `Renode_RP2040` repository.

### Choice 5: CI/CD Platform
- **Option A: GitHub Actions (Selected)** - Integrated CI/CD platform for GitHub repositories, allowing for automated builds and simulation runs. Explicitly mentioned in the `ROADMAP.md`.

## Detailed Architecture
### Interrupt Controller (NVIC)
The RP2040 uses a standard ARM Nested Vectored Interrupt Controller (NVIC). In Renode, this is modeled by `IRQControllers.NVIC`. The 26 system-level interrupts are routed to both cores (though usually handled by Core 0 in our setup).

The interrupt mapping follows the RP2040 datasheet (Table 80):
- **IRQs 0-3:** Timer Alarms
- **IRQ 4:** PWM Wrap
- **IRQ 13:** GPIO Bank 0
- **IRQ 15-16:** SIO (FIFOs)
- **IRQ 20-21:** UART0 and UART1
- **IRQ 22:** ADC FIFO

### Timer Peripheral
The RP2040 Timer provides a 64-bit microsecond timebase. It is modeled in Renode using `Timers.RP2040Timer`. It supports 4 alarms, each triggering a dedicated IRQ.

## Technical Interfaces
- **Interrupts:** Exposed via the `nvic0` and `nvic1` peripherals in Renode. Firmware uses `attachInterrupt()` (Arduino) or `irq_set_exclusive_handler()` (Pico SDK) to register handlers.
- **GPIO Interrupts:** Triggered by changes on GPIO pins. In Renode, `RP2040GPIO` implements `IGPIOReceiver` and manages `IO_IRQ_BANK0`.

## Implementation Choices
- **Arduino Framework Integration:** We use `attachInterrupt()` for GPIO interrupts to maintain compatibility with the selected Arduino framework.
- **Renode IRQ Mappings:** We explicitly define IRQ connections in the `.repl` files to match the hardware specification, allowing the NVIC to correctly route signals from peripherals to the CPU cores.

## Discarded Alternatives
### Choice 3: Programming SDK
- **Option B: Arduino Framework** - Higher level abstraction, but may hide details needed for precise peripheral simulation.
- **Option C: Bare Metal (C/C++ without SDK)** - Maximum control but requires significant effort to implement basic functionality.

### Choice 4: Testing Framework
- **Option B: Unity** - Lightweight C unit testing framework, but less suitable for high-level simulation orchestration compared to Robot Framework.
- **Option C: Pytest** - Python testing framework, powerful but lacks native integration with Renode's built-in testing capabilities compared to Robot Framework.

### Choice 5: CI/CD Platform
- **Option B: GitLab CI/CD** - Requires external hosting or mirroring for this repository.
- **Option C: Local Bash Scripts** - Not suitable for automated, cloud-based continuous integration and reporting.
