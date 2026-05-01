Raspberry Pi Pico-series Python SDK

## **Chapter 1. The MicroPython Environment**

Python is the fastest way to get started with embedded software on Pico-series devices. This book is about the official MicroPython port for RP-series microcontroller-based boards.

MicroPython is a Python 3 implementation for microcontrollers and small embedded systems. Because MicroPython is highly efficient, and RP-series microcontrollers are designed with a disproportionate amount of system memory and processing power for their price, MicroPython is a serious tool for embedded systems development, which does not compromise on approachability.

For exceptionally demanding pieces of software, you can fall back on the SDK (covered in **Getting started with Raspberry Pi Pico-series** and **Raspberry Pi Pico-series C/C++ SDK** ), or an external C module added to your MicroPython firmware, to wring out the very last drop of performance. For every other project, MicroPython handles a lot of heavy lifting for you, and lets you focus on writing the code that adds value to your project. The accelerated floating point libraries in RP-series microcontrollers' on-board ROM storage are used automatically by your Python code, so you should find arithmetic performance quite snappy.

Most on-chip hardware is exposed through the standard machine module, so existing MicroPython projects can be ported without too much trouble. The second processor core is exposed through the _thread module.

RP-series microcontrollers have some unique hardware you won’t find on other microcontrollers, with the programmable I/O system (PIO) being the prime example of this: a versatile hardware subsystem that lets you create new I/O interfaces and run them at high speed. In the rp2 module you will find a comprehensive PIO library which lets you write new PIO programs at the MicroPython prompt, and interact with them in real time, to develop interfaces for new or unusual pieces of hardware (or indeed if you just find yourself wanting an extra few serial ports).

MicroPython implements the entire Python 3.4 syntax (including exceptions, with, yield from, etc., and additionally async /await keywords from Python 3.5). The following core datatypes are provided: str (including basic Unicode support), bytes, bytearray, tuple, list, dict, set, frozenset, array.array, collections.namedtuple, classes and instances. Builtin modules include sys, time, and struct, etc. Note that only a subset of Python 3 functionality is implemented for the data types and modules.

MicroPython can execute scripts in textual source form (.py files) or from precompiled bytecode, in both cases either from an on-device filesystem or "frozen" into the MicroPython executable.

## **1.1. Getting MicroPython for RP-series Microcontrollers**

**Pre-built Binary** A pre-built binary of the latest MicroPython firmware is available from the MicroPython section of the documentation.

The fastest way to get MicroPython is to download the pre-built release binary from the Documentation pages. If you can’t or don’t want to use the pre-built release — for example, if you want to develop a C module for MicroPython — you can follow the instructions in Section 1.3 to get the source code for MicroPython, which you can use to build your own MicroPython firmware binary.

1.1. Getting MicroPython for RP-series Microcontrollers

**4**

Raspberry Pi Pico-series Python SDK

## **1.2. Installing MicroPython on a Pico-series Device**

Pico-series devices have a BOOTSEL mode for programming firmware over the USB port. Holding the BOOTSEL button when powering up your board will put it into a special mode where it appears as a USB Mass Storage Device. First make sure your Pico-series device is not plugged into _any_ source of power: disconnect the micro USB cable if plugged in, and disconnect any other wires that might be providing power to the board, e.g. through the VSYS or VBUS pin. Now hold down the BOOTSEL button, and plug in the micro USB cable (which hopefully has the other end plugged into your computer).

A drive called RPI-RP2 should pop up. Go ahead and drag the MicroPython firmware.uf2 file onto this drive. This programs the MicroPython firmware onto the flash memory on your Pico-series device.

It should take a few seconds to program the UF2 file into the flash. The board will automatically reboot when finished, causing the RPI-RP2 drive to disappear, and boot into MicroPython.

By default, MicroPython doesn’t _do_ anything when it first boots. It sits and waits for you to type in further instructions. Chapter 2 shows how you can connect with the MicroPython firmware now running on your board. You can read on to see how a custom MicroPython firmware file can be built from the source code.

The **Getting started with Raspberry Pi Pico-series** book has detailed instructions on getting your Pico-series device into BOOTSEL mode and loading UF2 files, in case you are having trouble. There is also a section going over loading ELF files with the debugger, in case your board doesn’t have an easy way of entering BOOTSEL, or you would like to debug a MicroPython C module you are developing.

##  **NOTE**

If you are not following these instructions on a Pico-series device, you may not have a BOOTSEL button. If this is the case, you should check if there is some other way of grounding the flash CS pin, such as a jumper, to tell the RPseries microcontroller to enter the BOOTSEL mode on boot. If there is no such method, you can load code using the Serial Wire Debug interface.

## **1.3. Building MicroPython From Source**

The prebuilt binary which can be downloaded from the MicroPython section of the documentation should serve most use cases, but you can build your own MicroPython firmware from source if you’d like to customise its low-level aspects.

##  **TIP**

If you have already downloaded and installed a prebuilt MicroPython UF2 file, you can skip ahead to Chapter 2 to start using your board.

##  **IMPORTANT**

These instructions for getting and building MicroPython assume you are using Raspberry Pi OS running on a Raspberry Pi 4, or an equivalent Debian-based Linux distribution running on another platform.

It’s a good idea to create a pico directory to keep all pico-related checkouts in. These instructions create a pico directory at /home/pi/pico.

$ cd ~/ $ mkdir pico $ cd pico

Then clone the micropython git repository. These instructions will fetch the latest version of the source code.

1.2. Installing MicroPython on a Pico-series Device

**5**

Raspberry Pi Pico-series Python SDK

$ git clone https://github.com/micropython/micropython.git --branch master

Once the download has finished, the source code for MicroPython should be in a new directory called micropython. The MicroPython repository also contains pointers ( _submodules_ ) to specific versions of libraries it needs to run on a particular board, like the SDK in the case of RP-series microcontroller. We need to fetch these submodules too:

$ cd micropython $ make -C ports/rp2 submodules

##  **NOTE**

The following instructions assume that you are using a Pico-series device. Some details may differ if you are building firmware for a different RP-series microcontroller-based board. The board vendor should detail any extra steps needed to build firmware for that particular board. The version we’re building here is fairly generic, but there might be some differences like putting the default serial port on different pins, or including extra modules to drive that board’s hardware.

To build the RP-series microcontroller MicroPython port, you’ll need to install some extra tools. To build projects you’ll need CMake, a cross-platform tool used to build the software, and the GNU Embedded Toolchain for Arm, which turns MicroPython’s C source code into a binary program RP-series microcontrollers' processors can understand. buildessential is a bundle of tools you need to build code native to your own machine — this is needed for some internal tools in MicroPython and the SDK. You can install all of these via apt from the command line. Anything you already have installed will be ignored by apt.

$ sudo apt update $ sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

First we need to bootstrap a special tool for MicroPython builds, that ships with the source code:

$ make -C mpy-cross

We can now build the _port_ we need for RP-series microcontroller, that is, the version of MicroPython that has specific support for Raspberry Pi chips.

$ cd ports/rp2 $ make

If everything went well, there will be a new directory called build-PICO (ports/rp2/build-PICO relative to the micropython directory), which contains the new firmware binaries. The most important ones are:

> firmware.uf2 A UF2 binary file which can dragged onto the RPI-RP2 drive that pops up once your Pico-series device enters BOOTSEL mode. The firmware binary you can download from the documentation page is a UF2 file, because they’re the easiest to install.

> firmware.elf A different type of binary file, which can be loaded by a debugger (such as gdb with openocd) over RP-series microcontroller’s SWD debug port. This is useful for debugging either a native C module you’ve added to MicroPython, or the MicroPython core interpreter itself. The actual binary contents is the same as firmware.uf2.

1.3. Building MicroPython From Source

**6**

Raspberry Pi Pico-series Python SDK

You can take a look inside your new firmware.uf2 using picotool, see the Appendix B in the **Getting started with Raspberry Pi Pico-series** book for details of how to use picotool, e.g.

$ picotool info -a build-PICO/firmware.uf2 File build-PICO/firmware.uf2: Program Information name:            MicroPython version:         v1.18-412-g965747bd9 features:        USB REPL thread support frozen modules:  _boot, rp2, _boot_fat, ds18x20, onewire, dht, uasyncio, uasyncio/core, uasyncio/event, uasyncio/funcs, uasyncio/lock, uasyncio/stream, neopixel binary start:    0x10000000 binary end:      0x1004ba24 embedded drive:  0x100a0000-0x10200000 (1408K): MicroPython Fixed Pin Information none Build Information sdk version:       1.3.0 pico_board:        pico boot2_name:        boot2_w25q080 build date:        May  4 2022 build attributes:  MinSizeRel

1.3. Building MicroPython From Source

**7**
