Hardware design with RP2040

## **Chapter 1. About the RP2040**

RP2040 is a low-cost, high-performance microcontroller device with flexible digital interfaces. Key features:

- [Dual Cortex M0+ processors, up to 133MHz]

- [264kB of embedded SRAM in 6 banks]

- [30 multifunction GPIO]

- [6 dedicated I/O for SPI flash (supporting XIP)]

- [Dedicated hardware for commonly used peripherals]

- [Programmable I/O for extended peripheral support]

- [4 channel ADC with internal temperature sensor, 500ksps, 12-bit conversion]

- [USB 1.1 host/device]

_Figure 1. A system overview of the RP2040 chip_

**==> picture [425 x 305] intentionally omitted <==**

Code may be executed directly from external memory, through a dedicated SPI, DSPI or QSPI interface. A small cache improves performance for typical applications.

Debug is available via the SWD interface.

Internal SRAM is arranged in banks which can contain code or data and is accessed via dedicated AHB bus fabric connections, allowing bus masters to access separate bus slaves without being stalled.

DMA bus masters are available to offload repetitive data transfer tasks from the processors.

GPIO pins can be driven directly, or from a variety of dedicated logic functions.

Dedicated peripheral IP provides fixed functions such as SPI, I2C, UART.

Flexible configurable PIO controllers can be used to provide a wide variety of I/O functions.

A simple USB controller with embedded PHY can be used to provide FS/LS host or device connectivity under software

Chapter 1. About the RP2040

**4**

Hardware design with RP2040

## control.

4 GPIOs also share package pins with ADC inputs.

2 PLLs are available to provide a USB or ADC fixed 48MHz clock, and a flexible system clock up to 133MHz

An internal voltage regulator will supply the core voltage so the end product only needs supply the I/O voltage.

Chapter 1. About the RP2040

**5**
