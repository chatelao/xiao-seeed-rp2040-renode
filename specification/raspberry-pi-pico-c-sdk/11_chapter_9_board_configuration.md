Raspberry Pi Pico-series C/C++ SDK

## **Chapter 9. Board configuration**

Board configuration is the process of customising the SDK to run on a specific board design. The SDK comes with some predefined configurations for boards produced by Raspberry Pi and other manufacturers, the main (and default) example being the Raspberry Pi Pico.

Configurations specify a number of parameters that could vary between hardware designs. For example, default UART ports, on-board LED locations and flash capacities etc.

This chapter will go through where these configurations files are, how to make changes and set parameters, and how to build your SDK using CMake with your customisations.

## **9.1. The Configuration files**

Board specific configuration files are stored in the SDK source tree, at …/src/boards/include/boards/<boardname>.h. The default configuration file is that for the Raspberry Pi Pico, and at the time of writing is:

<sdk_path>/src/boards/include/boards/pico.h

This relatively short file contains overrides from default of a small number of parameters used by the SDK when building code.

_SDK: https://github.com/raspberrypi/pico-sdk/blob/master/src/boards/include/boards/pico.h_

1 _/*_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _// -----------------------------------------------------_ 8 _// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO_ 9 _//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES_ 10 _// -----------------------------------------------------_ 11 12 _// This header may be included by other board headers as "boards/pico.h"_ 13 14 _#ifndef _BOARDS_PICO_H_ 15 _#define _BOARDS_PICO_H_ 16 17 pico_board_cmake_set(PICO_PLATFORM, rp2040) 18 19 _// For board detection_ 20 _#define RASPBERRYPI_PICO_ 21 22 _// --- UART ---_ 23 _#ifndef PICO_DEFAULT_UART_ 24 _#define PICO_DEFAULT_UART 0_ 25 _#endif_ 26 _#ifndef PICO_DEFAULT_UART_TX_PIN_ 27 _#define PICO_DEFAULT_UART_TX_PIN 0_ 28 _#endif_ 29 _#ifndef PICO_DEFAULT_UART_RX_PIN_ 30 _#define PICO_DEFAULT_UART_RX_PIN 1_ 31 _#endif_ 32 33 _// --- LED ---_ 34 _#ifndef PICO_DEFAULT_LED_PIN_

9.1. The Configuration files

**584**

Raspberry Pi Pico-series C/C++ SDK

35 _#define PICO_DEFAULT_LED_PIN 25_ 36 _#endif_ 37 _// no PICO_DEFAULT_WS2812_PIN_ 38 39 _// --- I2C ---_ 40 _#ifndef PICO_DEFAULT_I2C_ 41 _#define PICO_DEFAULT_I2C 0_ 42 _#endif_ 43 _#ifndef PICO_DEFAULT_I2C_SDA_PIN_ 44 _#define PICO_DEFAULT_I2C_SDA_PIN 4_ 45 _#endif_ 46 _#ifndef PICO_DEFAULT_I2C_SCL_PIN_ 47 _#define PICO_DEFAULT_I2C_SCL_PIN 5_ 48 _#endif_ 49 50 _// --- SPI ---_ 51 _#ifndef PICO_DEFAULT_SPI_ 52 _#define PICO_DEFAULT_SPI 0_ 53 _#endif_ 54 _#ifndef PICO_DEFAULT_SPI_SCK_PIN_ 55 _#define PICO_DEFAULT_SPI_SCK_PIN 18_ 56 _#endif_ 57 _#ifndef PICO_DEFAULT_SPI_TX_PIN_ 58 _#define PICO_DEFAULT_SPI_TX_PIN 19_ 59 _#endif_ 60 _#ifndef PICO_DEFAULT_SPI_RX_PIN_ 61 _#define PICO_DEFAULT_SPI_RX_PIN 16_ 62 _#endif_ 63 _#ifndef PICO_DEFAULT_SPI_CSN_PIN_ 64 _#define PICO_DEFAULT_SPI_CSN_PIN 17_ 65 _#endif_ 66 67 _// --- FLASH ---_ 68 69 _#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1_ 70 71 _#ifndef PICO_FLASH_SPI_CLKDIV_ 72 _#define PICO_FLASH_SPI_CLKDIV 2_ 73 _#endif_ 74 75 pico_board_cmake_set_default(PICO_FLASH_SIZE_BYTES, (2 * 1024 * 1024)) 76 _#ifndef PICO_FLASH_SIZE_BYTES_ 77 _#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)_ 78 _#endif_ 79 _// Drive high to force power supply into PWM mode (lower ripple on 3V3 at light loads)_ 80 _#define PICO_SMPS_MODE_PIN 23_ 81 82 _#ifndef PICO_RP2040_B0_SUPPORTED_ 83 _#define PICO_RP2040_B0_SUPPORTED 1_ 84 _#endif_ 85 86 _// The GPIO Pin used to read VBUS to determine if the device is battery powered._ 87 _#ifndef PICO_VBUS_PIN_ 88 _#define PICO_VBUS_PIN 24_ 89 _#endif_ 90 91 _// The GPIO Pin used to monitor VSYS. Typically you would use this with ADC._ 92 _// There is an example in adc/read_vsys in pico-examples._ 93 _#ifndef PICO_VSYS_PIN_ 94 _#define PICO_VSYS_PIN 29_ 95 _#endif_ 96 97 _#endif_

9.1. The Configuration files

**585**

Raspberry Pi Pico-series C/C++ SDK

As can be seen, it sets up the default UART to uart0, the GPIO pins to be used for that UART, the GPIO pin used for the on-board LED, and the flash size.

To create your own configuration file, create a file in the board ../source/folder with the name of your board, for example, myboard.h. Enter your board specific parameters in this file.

## **9.2. Building applications with a custom board configuration**

The CMake system is what specifies which board configuration is going to be used.

To create a new build based on a new board configuration (we will use the myboard example from the previous section) first create a new build folder under your project folder. For our example we will use the pico-examples folder.

$ cd pico-examples $ mkdir myboard_build $ cd myboard_build

then run cmake as follows:

$ cmake -D"PICO_BOARD=myboard" ..

This will set up the system ready to build so you can simply type make in the myboard_build folder and the examples will be built for your new board configuration.

## **9.3. Available configuration parameters**

Table 36 lists all the available configuration parameters available within the SDK. You can set any configuration variable in a board configuration header file, however the convention is to limit that to configuration items directly affected by the board design (e.g. pins, clock frequencies etc.) Other configuration items should generally be overridden in the CMake configuration (or another configuration header) for the application being built.

9.2. Building applications with a custom board configuration

**586**
