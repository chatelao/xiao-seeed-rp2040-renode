# DMA Simulation Concept

This document outlines the strategy for adding and refining Direct Memory Access (DMA) support in the XIAO Seeed RP2040 simulation model.

## Goal
Integrate a fully functional RP2040 DMA controller into the Renode simulation to enable complex data transfer scenarios, such as peripheral-to-memory (ADC capture), memory-to-peripheral (PIO/UART transmission), and memory-to-memory (CRC calculation, block moves).

## Architecture
The DMA simulation utilizes a two-layer approach consisting of a peripheral model and a specialized DMA engine.

### 1. Peripheral Model (`RPDMA`)
The `RPDMA` class handles the register interface and channel logic.
- **Channels:** Simulates 12 independent DMA channels, each with its own set of control/status registers (READ_ADDR, WRITE_ADDR, TRANS_COUNT, CTRL).
- **Register Aliases:** Implements the four alias windows per channel, where different register offsets act as "triggers" to start transfers.
- **Interrupt Routing:** Manages two system-level IRQs (IRQ0, IRQ1) with independent masking (INTE0, INTE1) and status (INTS0, INTS1) registers.
- **Sniffer (CRC):** Implements the "sniff" hardware that passively observes data passing through a selected channel and calculates checksums (CRC-32, CRC-16, Sum, XOR).

### 2. DMA Engine (`RPDmaEngine`)
A custom `RPDmaEngine` extends the standard Renode DMA capabilities to support RP2040-specific features.
- **Atomic Transfers:** Supports byte, halfword, and word transfer sizes.
- **Address Wrapping (Ring Buffers):** Implements power-of-2 boundary wrapping for both read and write pointers.
- **Checksum Integration:** Integrates directly with the sniffer logic to calculate CRC on-the-fly during bus transfers.

## Implementation Details

### Data Request (DREQ) Pacing
The DMA controller listens for DREQ signals from other peripherals (PIO, UART, ADC, PWM, I2C, SPI).
- **Current State:** Basic rising-edge DREQ triggers are implemented via the `IGPIOReceiver` interface.
- **Gap:** The RP2040 uses a credit-based DREQ scheme. The simulation currently approximates this by triggering a single transfer unit per DREQ edge.

### Channel Chaining
When a channel completes, it can trigger another channel (specified in `CHAIN_TO`).
- **Mechanism:** Upon completion, the `RPDMA` model uses `ExecuteInNearestSyncedState` to trigger the next channel in the chain, ensuring timing consistency.

### Pacing Timers
Four internal pacing timers can be used as TREQ sources instead of external DREQs.
- **Requirement:** Implement `TIMER0`-`TIMER3` registers as fractional (X/Y) dividers that generate periodic transfer requests.

## Integration Plan

### Phase 1: Core Functionality (Current)
- [x] Basic channel register implementation and aliases.
- [x] Simple memory-to-memory and peripheral-to-memory transfers.
- [x] CRC/Sniffer logic for basic checksums.

### Phase 2: Advanced Features & Refinement
- [x] Implement `CHAN_ABORT` logic to safely terminate in-progress sequences.
- [x] Implement `N_CHANNELS` register for channel discovery.
- [x] Implement Pacing Timers (`TIMER0`-`TIMER3`) for periodic transfers.
- [x] Implement full interrupt masking and forcing logic (INTF0, INTF1).
- [x] Add Debug registers (`DBG_CTDREQ`, `DBG_TCR`) for channel introspection.
- [ ] Refine DREQ logic to support the credit-based scheme.

## Test Cases & Verification
We will reuse and adapt test cases from the `Renode_RP2040` project:
- **`hello_dma`:** Basic memory-to-memory transfer and completion interrupt.
- **`sniff_crc`:** Verification of the CRC/Sniffer hardware with various polynomials and transformations (bit reversal, inversion).
- **`channel_irq`:** Tests complex interrupt routing and reconfiguration within the ISR.
- **`control_blocks`:** Validates channel chaining and complex "gather" operations using control blocks in memory.

## Source & References
- **Primary Source:** [chatelao/Renode_RP2040](https://github.com/chatelao/Renode_RP2040)
- **RP2040 Datasheet:** Section 2.5 (DMA)
- **Reference:** STM32 DMA implementations in Renode for best practices in handling circular buffers and peripheral synchronization.
