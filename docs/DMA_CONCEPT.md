# DMA Simulation Concept

This document outlines the strategy for implementing and verifying Direct Memory Access (DMA) support in the XIAO Seeed RP2040 simulation model, leveraging best practices from existing Renode DMA models and addressing RP2040-specific architectural nuances.

## Goal
Integrate a fully functional RP2040 DMA controller into the Renode simulation to enable autonomous data movement between memory and peripherals, reducing CPU overhead and enabling complex scenarios like high-speed ADC sampling, UART background transmission, and real-time CRC calculation.

## RP2040 DMA Architecture Overview
The RP2040 DMA is characterized by:
- **12 Independent Channels:** Each with full 32-bit address range for both read and write.
- **Credit-based DREQ:** A unique mechanism to prevent FIFO overflow/underflow without requiring large buffers.
- **Atomic Register Aliases:** Four alias windows per channel allowing different trigger behaviors (Trigger on CTRL, TRANS_COUNT, WRITE_ADDR, or READ_ADDR).
- **Channel Chaining:** Allows one channel to start another upon completion, facilitating complex "gather/scatter" operations.
- **Address Wrapping (Ring Buffers):** Support for power-of-2 boundary wrapping (2 bytes to 32 KiB).
- **Sniffer (CRC):** A passive observer of data transfers that calculates checksums (CRC-32, CRC-16, Sum, XOR).

## Simulation Strategy

### 1. Peripheral Model (`RPDMA`)
The `RPDMA` model handles the register interface, interrupt routing, and DREQ management.
- **Register Aliases:** Implement the mapping of offsets 0x00, 0x10, 0x20, and 0x30 to the same physical registers but with different trigger side effects.
- **Interrupt Routing:** Simulate two independent system-level IRQs (`IRQ0`, `IRQ1`) with associated enable (`INTE0/1`) and status (`INTS0/1`) registers.
- **Credit Counters (`DBG_CTDREQ`):** Implement 6-bit saturation counters for each channel to simulate the credit-based DREQ scheme.

### 2. DMA Engine (`RPDmaEngine`)
A specialized engine is required to handle RP2040's unique transfer behaviors that deviate from the standard Renode `DmaEngine`.
- **Ring Buffers:** Implement logic to wrap `ReadAddress` or `WriteAddress` at `1 << RING_SIZE` boundaries.
- **CRC Sniffing:** Passive integration where the engine "feeds" data to the sniffer logic during the bus transfer phase.
- **Atomic Size Support:** Strict adherence to 8/16/32-bit transfer sizes as configured in `DATA_SIZE`.

### 3. DREQ Pacing Mechanism
The simulation will move beyond simple binary triggers to a credit-based approach:
- **Increment:** Each pulse from a peripheral (e.g., UART RX FIFO not empty) increments the channel's credit counter.
- **Decrement:** The DMA engine performs a transfer only when credit > 0, decrementing the counter.
- **Sync:** Use Renode's `ExecuteInNearestSyncedState` to ensure DREQ pulses from peripherals are processed at the correct virtual time relative to the CPU.

## Comparison with STM32 DMA
While the STM32 DMA (common in Renode) focuses on fixed stream-to-peripheral mappings and circular buffers, the RP2040 DMA is more flexible:
- **Triggering:** STM32 uses dedicated request lines per channel; RP2040 allows any DREQ source to be mapped to any channel via `TREQ_SEL`.
- **Control Blocks:** RP2040's alias/trigger system is specifically designed for loading control blocks from memory into DMA registers, which is less "native" in many STM32 DMA models.
- **Pacing:** RP2040's X/Y fractional pacing timers offer more granular periodic transfer control than standard STM32 timer-triggering.

## Integration & Roadmap

### Phase 1: Core Registers & Aliases (Complete)
- Basic register implementation for 12 channels.
- Support for Alias 0/1/2/3 trigger logic.

### Phase 2: Advanced Transfer Logic (Ongoing)
- **Ring Buffers:** Implementation of power-of-2 wrapping logic in `RPDmaEngine`.
- **Sniffer/CRC:** Integration of CRC-32 and CRC-16-CCITT with bit/byte reversal support.
- **Channel Chaining:** Ensuring completion of Channel A triggers Channel B without CPU intervention.

### Phase 3: Hardware Refinement (Planned)
- **Credit Counters:** Implementing `DBG_CTDREQ` logic.
- **Pacing Timers:** Implementing `TIMER0`-`TIMER3` X/Y fractional dividers.
- **Abort Logic:** Support for `CHAN_ABORT` and the associated `BUSY` flag behavior.

## Test Strategy & Verification
We will reuse and expand tests from the `Renode_RP2040` project, integrated into our Robot Framework suite:
1. **`hello_dma`:** Simple memory-to-memory block move.
2. **`sniff_crc`:** Verification of all CRC modes and bit-reversal transformations.
3. **`control_blocks`:** Gathering data from multiple non-contiguous memory buffers to UART.
4. **`channel_irq`:** Complex interrupt handling and reconfiguration within the ISR.
5. **`dma_capture` (ADC):** High-speed ADC data capture into a ring buffer.

## References
- **RP2040 Datasheet:** Section 2.5 (DMA)
- **Renode Source:** `DmaEngine.cs`, `StandardDma.cs`
- **External:** [chatelao/Renode_RP2040](https://github.com/chatelao/Renode_RP2040)
