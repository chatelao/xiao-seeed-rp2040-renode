# PIO Simulation Concept

This document outlines the strategy for adding Programmable I/O (PIO) support to the XIAO Seeed RP2040 simulation model.

## Goal
Integrate the RP2040 PIO blocks into the Renode simulation to enable testing of custom high-speed serial protocols and precise timing applications.

## Architecture
The PIO simulation follows a co-simulation approach to balance performance and accuracy.

### 1. PIO Engine (`libpiosim`)
Instead of a pure C# implementation, we utilize `libpiosim`, an external simulator written in C++.
- **Reasoning:** PIO state machines execute at high frequencies. A C++ engine provides the necessary throughput for real-time or near-real-time emulation within Renode.
- **Interface:** Renode communicates with `libpiosim` via a native bridge (`NativeBinder`) in the C# peripheral model.

### 2. Renode Peripheral Model (`RP2040PIOCPU`)
The `RP2040PIOCPU` class in C# acts as the bridge between the Renode system bus and the `libpiosim` engine.
- **CPU Modeling:** Each PIO block (PIO0, PIO1) is modeled as a specialized "CPU" in Renode.
- **Register Access:** The model handles standard PIO register offsets, including the XOR, SET, and CLEAR atomic aliases.
- **GPIO Interaction:** The PIO engine interacts with the `RP2040GPIO` model to drive or read pin states.

### 3. Synchronization
Renode's time-stepping (quanta) can lead to drift between the main Cortex-M0+ cores and the PIO blocks.
- **Mechanism:** The `RP2040GPIO` model includes a `ReevaluatePioActions` list. Whenever the GPIO state changes or a specific number of instructions are executed, the PIO engine is "pushed" to catch up with the system time.

## Integration Plan

### Phase 1: Infrastructure
- [x] Integrate `libpiosim` binaries and `fetch_piosim.py` script.
- [x] Implement `RP2040PIOCPU` C# peripheral model.
- [x] Define PIO blocks in `rp2040.repl`.

### Phase 2: Functional Support
- [ ] Connect PIO outputs to XIAO RP2040 pins in `.repl`.
- [ ] Implement DMA requests (DREQ) from PIO to the DMA controller.
- [ ] Implement PIO interrupt (IRQ) routing to the NVIC.

## Test Cases & Verification
We will reuse validated test cases from the `Renode_RP2040` project:
- **`hello_pio`:** Verifies basic PIO instruction execution and LED blinking via PIO.
- **`pio_blink`:** Tests timing accuracy and frequency scaling.
- **`addition`:** Validates data processing and FIFO interaction.

### New Verification for XIAO Seeed RP2040
- **Pin Mapping Test:** Ensure PIO can drive XIAO-specific pins (e.g., the RGB LED or the standard D-series headers).
- **Integration Test:** Run a PIO-based UART or SPI alongside the hardware UART/SPI to verify concurrent operation.

## Source & References
- **Primary Source:** [chatelao/Renode_RP2040](https://github.com/chatelao/Renode_RP2040)
- **Engine Source:** [matgla/Renode_RP2040_PioSim](https://github.com/matgla/Renode_RP2040_PioSim)
- **RP2040 Datasheet:** Section 3 (PIO)

## Known Limitations
- **DMA/IRQ:** The current `RP2040PIOCPU` model has stubs for DMA and IRQ triggers which need to be fully implemented.
- **Performance:** High-frequency PIO programs may require lowering the Renode `emulation SetGlobalQuantum` for stable execution.
