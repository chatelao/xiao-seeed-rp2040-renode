# Design

The detailed design of the solution, including the architecture, used tech stack for development, production and testing, etc.

## Technological Choices

### Choice 3: Programming SDK
- **Option A: Arduino Framework (Earle Philhower Core) (Selected)** - Provides a high-level API compatible with Arduino while allowing access to the underlying Pico SDK. It was chosen for its excellent support for the Seeed XIAO RP2040 in PlatformIO and ease of integration.

### Choice 4: Testing Framework
- **Option A: Robot Framework (Selected)** - Integration and system testing framework used for Renode simulations. Provides clear test reports and is used in the reference `Renode_RP2040` repository.

### Choice 5: CI/CD Platform
- **Option A: GitHub Actions (Selected)** - Integrated CI/CD platform for GitHub repositories, allowing for automated builds and simulation runs. Explicitly mentioned in the `ROADMAP.md`.

### Choice 6: Documentation Generator
- **Option A: MkDocs (Selected)** - Fast and simple static site generator that's geared towards building project documentation. Documentation source files are written in Markdown, and configured with a single YAML configuration file.

## Detailed Architecture
The system follows a layered architecture to bridge the physical hardware abstraction with the simulation environment:

1.  **Firmware Layer (C++/Arduino):**
    *   Developed using PlatformIO with the `arduino` framework for RP2040.
    *   Uses standard Arduino APIs for GPIO, ADC (`analogRead`), and PWM (`analogWrite`).
    *   Uses the `hardware/timer.h` and `pico/time.h` from the underlying Pico SDK for low-level hardware timer alarm management.
2.  **Simulation Layer (Renode):**
    *   **Custom Peripherals:** Implemented in C# to accurately model RP2040-specific behavior (e.g., atomic register aliases via `IRP2040Peripheral`).
    *   **Configuration:** `.repl` (platform) and `.resc` (script) files define the SoC layout and interconnects.
3.  **Verification Layer (Robot Framework):**
    *   Orchestrates Renode to load the firmware ELF, simulate peripheral events (like GPIO toggles or ADC voltage feeds), and verify UART output.

## Technical Interfaces
*   **UART:** Configured at 115200 baud, 8N1. Mapped to UART0.
*   **ADC:** 12-bit resolution. Channel 0 is connected to pin A0 (GPIO 26).
*   **PWM:** Slice 0, Channel B is mapped to GPIO 17 (RED LED).
*   **Interrupts:** GPIO 2 (XIAO D8) triggers an interrupt on the RISING edge.
*   **Timer:** Hardware alarms are used to trigger periodic (100ms) and one-shot events.

## Implementation Choices
- **UART Peripheral:** The project utilizes a custom `RP2040Uart` model (located at `test/renode-config/emulation/peripherals/uart/rp2040_uart.cs`) rather than the stock Renode `UART.PL011`. This choice is driven by the RP2040 hardware specification, which requires support for atomic register aliases (XOR, SET, CLEAR) and specific integration with the DMA and PIO subsystems.
- **PWM Peripheral:** The `RP2040PWM` model handles 8 slices with 2 channels each. Frequency and duty cycle are calculated based on CSR, DIV, TOP, and CC registers.
- **Timer Peripheral:** The `RP2040Timer` model implements the 64-bit counter and 4 hardware alarms with IRQ mapping to NVIC indices 0-3.
- **ADC Peripheral:** The `RP2040ADC` model supports 5 channels (4 external + 1 temperature) and interrupt generation on FIFO events.

## Discarded Alternatives
### Choice 3: Programming SDK
- **Option B: Raspberry Pi Pico SDK** - Initially considered, but moved to technical debt to simplify initial setup. Remains a future goal.
- **Option C: Bare Metal (C/C++ without SDK)** - Maximum control but requires significant effort to implement basic functionality.

### Choice 4: Testing Framework
- **Option B: Unity** - Lightweight C unit testing framework, but less suitable for high-level simulation orchestration compared to Robot Framework.
- **Option C: Pytest** - Python testing framework, powerful but lacks native integration with Renode's built-in testing capabilities compared to Robot Framework.

### Choice 5: CI/CD Platform
- **Option B: GitLab CI/CD** - Requires external hosting or mirroring for this repository.
- **Option C: Local Bash Scripts** - Not suitable for automated, cloud-based continuous integration and reporting.

### Choice 6: Documentation Generator
- **Option B: Sphinx** - Powerful but more complex to set up than MkDocs for simple Markdown projects.
- **Option C: Doxygen** - Less focused on high-level conceptual and design documentation.
