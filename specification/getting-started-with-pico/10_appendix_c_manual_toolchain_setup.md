Getting started with Raspberry Pi Pico-series

## **Appendix C: Manual toolchain setup**

## **Configure your environment via Script**

If you are developing for a Pico-series device on the Raspberry Pi 5, the Raspberry Pi 4B, or the Raspberry Pi 400, most of the installation steps in this Getting Started guide can be skipped by running the pico_setup.sh script.

The script automates the following setup:

- [Creates a directory called ][pico][ in the folder where you ] _[run]_[ the ][pico_setup.sh][ script]

- [Installs required dependencies]

- [Downloads the ][pico-sdk][, ][pico-examples][, ][pico-extras][, and ][pico-playground][ repositories]

- [Defines ][PICO_SDK_PATH][, ][PICO_EXAMPLES_PATH][, ][PICO_EXTRAS_PATH][, and ][PICO_PLAYGROUND_PATH][ in your ][~/.bashrc]

- [Builds the ][blink][ and ][hello_world][ examples in ][pico-examples/build/blink][ and ][pico-examples/build/hello_world]

- [Downloads and builds ][picotool][ (see ][Appendix B][), and copy it to ][/usr/local/bin][.]

- [Downloads and builds ][debugprobe][ (see ][Appendix A][).]

- [Downloads and compiles OpenOCD (for debug support)]

- [Configures your development Raspberry Pi UART for use with Pico-series devices]

##  **TIP**

This setup script requires approximately 2.5GB of disk space on your SD card, so make sure you have enough free space before running it. You can check how much free disk space you have with the df -h command.

First, run the following command to install wget:

$ sudo apt install wget

You can get this script by running the following command in a terminal:

$ wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh ①

Then mark the script as executable with chmod:

$ chmod +x pico_setup.sh

Run the script with the following command:

$ ./pico_setup.sh

Finally, reboot your Raspberry Pi to load your UART configuration changes:

Configure your environment via Script

**31**

Getting started with Raspberry Pi Pico-series

$ sudo reboot

## **Manually Configure your Environment**

## **Get the SDK and examples**

The Pico Examples repository provides a set of example applications written using the SDK. To clone these repositories, create a pico directory where you can store pico-related files. The following commands create a subdirectory named pico in your home directory:

$ mkdir ~/pico

Then, clone the pico-sdk and pico-examples git repositories:

$ cd ~/pico $ git clone https://github.com/raspberrypi/pico-sdk.git --branch master $ cd pico-sdk $ git submodule update --init $ cd .. $ git clone https://github.com/raspberrypi/pico-examples.git --branch master

## **Install the Toolchain**

To build the applications in pico-examples, you’ll need to install some extra tools. To build projects you’ll need CMake, a cross-platform tool used to build the software, and the ARM GNU Toolchain. Run the following command to install these dependencies:

$ sudo apt update $ sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

Ubuntu and Debian users might additionally need to do:

apt install g++ libstdc++-arm-none-eabi-newlib

## **Enable UART serial communications**

To enable UART serial communications on your development device. To do so on a Raspberry Pi running Raspberry Pi OS, run raspi-config:

$ sudo raspi-config

Manually Configure your Environment

**32**

Getting started with Raspberry Pi Pico-series

1. Navigate to Interfacing Options > Serial.

2. When asked "Would you like a login shell to be accessible over serial?", answer "No".

3. When asked "Would you like the serial port hardware to be enabled?", answer "Yes".

You should see something like Figure 11:

_Figure 11. Enabling a serial UART using_ raspi-config _on the Raspberry Pi._

**==> picture [319 x 204] intentionally omitted <==**

Exit raspi-config with **Esc** . Choose "Yes" and reboot your Raspberry Pi to enable the serial port.

##  **IMPORTANT**

Raspberry Pi 5 makes the UART on the 3-pin debug header the default for serial0. To use use GPIO pins 15 and 14 instead, append dtparam=uart0_console to /boot/firmware/config.txt.

## **Update the SDK**

When a new version of the SDK is released, you must update your copy of the SDK. To update, navigate into pico-sdk and run the following command:

$ cd pico-sdk $ git pull $ git submodule update

##  **NOTE**

To be informed of new releases, set up a custom watch on the pico-sdk GitHub repository. Navigate to https://github.com/raspberrypi/pico-sdk and select Watch → Custom → Releases. You will receive an email notification when a new SDK release occurs.

## **Use the CLI to Blink an LED in C**

When you’re writing software for hardware, turning an LED on, off, and then on again, is typically the first program that gets run in a new programming environment. Learning how to blink an LED gets you half way to anywhere.

So let’s blink the LED on a Pico-series device.

Use the CLI to Blink an LED in C

**33**

Getting started with Raspberry Pi Pico-series

The following example blinks the LED connected to pin 25 of a Raspberry Pi Pico or Pico 2:

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/blink_simple/blink_simple.c Lines 31 - 39_

31 int main() { 32     pico_led_init(); 33     while (true) { 34         pico_set_led(true); 35         sleep_ms(LED_DELAY_MS); 36         pico_set_led(false); 37         sleep_ms(LED_DELAY_MS); 38     } 39 }

The actual code for the "blink" example is slightly complicated as it also supports blinking the LED connected to the Infineon 43439 wireless chip on the Pico W. The full code can be found here sdkexampleref::blink/blink.c

## **Building "Blink"**

From the pico directory we created earlier, navigate into pico-examples and create a build directory:

$ cd pico-examples $ mkdir build $ cd build

Then, set the PICO_SDK_PATH, assuming you cloned the pico-sdk and pico-examples repositories into the same directory:

$ export PICO_SDK_PATH=../../pico-sdk

##  **TIP**

Throughout this book we use the relative path ../../pico-sdk to the SDK repository for PICO_SDK_PATH. Depending on the location of your repository, you could replace this with an absolute path.

## **Build "Blink"**

Prepare your cmake build directory by running the following command:

$ cmake .. Using PICO_SDK_PATH from environment ('../../pico-sdk') PICO_SDK_PATH is /home/pi/pico/pico-sdk _. . ._ -- Build files have been written to: /home/pi/pico/pico-examples/build

Use the CLI to Blink an LED in C

**34**

Getting started with Raspberry Pi Pico-series

##  **IMPORTANT**

The SDK builds binaries for the Raspberry Pi Pico by default. To build a binary for a different board, pass the -DPICO_BOARD=<board> option to CMake, replacing the <board> placeholder with the name of the board you’d like to target. To build a binary for Pico 2, pass -DPICO_BOARD=pico2. To build a binary for Pico W, pass -DPICO_BOARD=pico_w. You can specify a Wi-Fi network and password that your Pico W examples should connect to, by passing -DWIFI_SSID="Your Network" -DWIFI_PASSWORD="Your Password" too.

You can now type make to build all example applications. However, for this example we only need to build blink. To build a specific subtree of examples, navigate into the corresponding subtree before running make. In this case, we can build only the blink task by first navigating into the blink directory, then running make:

$ cd blink $ make -j4 Scanning dependencies of target ELF2UF2Build Scanning dependencies of target boot_stage2_original [  0%] Creating directories for 'ELF2UF2Build' _. . ._ [100%] Linking CXX executable blink.elf [100%] Built target blink

##  **TIP**

Invoking make with -j4 speeds the build up by running four jobs in parallel. A Raspberry Pi 5 has four cores, so four jobs spreads the build evenly across the entire SoC.

Amongst other targets, this builds:

blink.elf

used by the debugger

blink.uf2

the file we’ll copy onto the USB Mass Storage Device that represents your Raspberry Pi microcontroller

## **Load and run "Blink"**

To load software onto a Raspberry Pi microcontroller-based board, mount it as a USB Mass Storage Device and copy a uf2 file onto the board to program the flash.

Hold down the BOOTSEL button (Figure 12) while plugging in your device using a micro-USB cable to force it into USB Mass Storage Mode.

The device will reboot, unmount itself as a Mass Storage Device, and run the flashed code, see Figure 12.

Use the CLI to Blink an LED in C

**35**

Getting started with Raspberry Pi Pico-series

_Figure 12. Blinking the on-board LED on the Raspberry Pi Pico. Arrows point to the onboard LED, and the_ BOOTSEL _button._

**==> picture [319 x 213] intentionally omitted <==**

## **Using the command line**

##  **TIP**

You can use picotool to load a UF2 binary onto your Pico-series device, see Appendix B.

Depending on the platform you use to compile binaries, you may have to mount the mass storage device manually:

$ dmesg | tail [  371.973555] sd 0:0:0:0: [sda] Attached SCSI removable disk $ sudo mkdir -p /mnt/pico $ sudo mount /dev/sda1 /mnt/pico

If you can see files in /mnt/pico, the USB Mass Storage Device has mounted correctly:

$ ls /mnt/pico/ INDEX.HTM  INFO_UF2.TXT

Copy your blink.uf2 onto the device:

$ sudo cp blink.uf2 /mnt/pico $ sudo sync

The microcontroller automatically disconnects as a USB Mass Storage Device and runs your code, but just to be safe, you should unmount manually as well:

$ sudo umount /mnt/pico

Use the CLI to Blink an LED in C

**36**

Getting started with Raspberry Pi Pico-series

##  **NOTE**

Removing power from the board does not remove the code. When you restore power to the board, the flashed code will run again.

## **Aside: Other Boards**

If you are not following these instructions on a Raspberry Pi Pico-series device, you may not have a BOOTSEL button (as labelled in Figure 12). Your board may have some other way of loading code, which the board supplier should have documented:

- [Most boards expose the SWD interface (][[debug_probe_section]][) which can reset the board and load code without] any button presses

- [There may be some other way of pulling down the flash CS pin (which is how the ][BOOTSEL][ button works on Pico-] series devices), such as shorting together a pair of jumper pins

- [Some boards have a reset button, but no ][BOOTSEL][; they might detect a double-press of the reset button to enter the] bootloader

In all cases you should consult the documentation for the specific board you are using, which should describe the best way to load firmware onto that board.

## **Manually Create your own Project**

Go ahead and create a directory to house your test project sitting alongside the pico-sdk directory,

$ cd ~/pico $ ls -la total 16 drwxr-xr-x   7 aa  staff   224  6 Apr 10:41 ./ drwx------@ 27 aa  staff   864  6 Apr 10:41 ../ drwxr-xr-x  10 aa  staff   320  6 Apr 09:29 pico-examples/ drwxr-xr-x  13 aa  staff   416  6 Apr 09:22 pico-sdk/ $ mkdir test $ cd test

and then create a test.c file in the directory,

1 _#include <stdio.h>_ 2 _#include "pico/stdlib.h"_ 3 _#include "hardware/gpio.h"_ 4 _#include "pico/binary_info.h"_ 5 6 const uint LED_PIN = 25;① 7 8 int main() { 9 10     bi_decl(bi_program_description("This is a test binary."));② 11     bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED")); 12 13     stdio_init_all(); 14 15     gpio_init(LED_PIN); 16     gpio_set_dir(LED_PIN, GPIO_OUT);

Manually Create your own Project

**37**

Getting started with Raspberry Pi Pico-series

17     while (1) { 18         gpio_put(LED_PIN, 0); 19         sleep_ms(250); 20         gpio_put(LED_PIN, 1); 21         puts("Hello World\n"); 22         sleep_ms(1000); 23     } 24 }

- ① The onboard LED is connected to GP25 on Pico and Pico 2, if you’re building for Pico W the LED is connected to CYW43_WL_GPIO_LED_PIN. For more information see the Pico W blink example in the Pico Examples Github repository.

- ② These lines will add strings to the binary visible using picotool, see Appendix B.

along with a CMakeLists.txt file,

cmake_minimum_required(VERSION 3.13) include(pico_sdk_import.cmake) project(test_project C CXX ASM) set(CMAKE_C_STANDARD 11) set(CMAKE_CXX_STANDARD 17) pico_sdk_init() add_executable(test test.c ) pico_enable_stdio_usb(test 1)① pico_enable_stdio_uart(test 1)② pico_add_extra_outputs(test) target_link_libraries(test pico_stdlib)

1. Enables serial output via USB.

2. Enables serial output via UART.

Then copy the pico_sdk_import.cmake file from the external folder in your pico-sdk installation to your test project folder.

$ cp ../pico-sdk/external/pico_sdk_import.cmake .

You should now have something that looks like this,

$ ls -la total 24 drwxr-xr-x  5 aa  staff   160  6 Apr 10:46 ./ drwxr-xr-x  7 aa  staff   224  6 Apr 10:41 ../ -rw-r--r--@ 1 aa  staff   394  6 Apr 10:37 CMakeLists.txt -rw-r--r--  1 aa  staff  2744  6 Apr 10:40 pico_sdk_import.cmake -rw-r--r--  1 aa  staff   383  6 Apr 10:37 test.c

Manually Create your own Project

**38**

Getting started with Raspberry Pi Pico-series

and can build it as we did before with our "Hello World" example.

$ mkdir build $ cd build $ export PICO_SDK_PATH=../../pico-sdk $ cmake .. $ make

##  **IMPORTANT**

The SDK builds binaries for the Raspberry Pi Pico by default. To build a binary for a different board, pass the -DPICO_BOARD=<board> option to CMake, replacing the <board> placeholder with the name of the board you’d like to target. To build a binary for Pico 2, pass -DPICO_BOARD=pico2. To build a binary for Pico W, pass -DPICO_BOARD=pico_w. To specify a Wi-Fi network and password that your Pico W should connect to, pass -DWIFI_SSID="Your Network" -DWIFI_PASSWORD="Your Password".

The make process will produce a number of different files. The important ones are shown in the following table.

|**File extension**|**Description**|
|---|---|
|.bin|Raw binary dump of the program code and data|
|.elf|The full program output, possibly including debug information|
|.uf2|The program code and data in a UF2 form that you can drag-and-drop on to the device<br>when it is mounted as a USB drive|
|.dis|A disassembly of the compiled binary|
|.hex|Hexdump of the compiled binary|
|.map|A map file to accompany the .elf file describing where the linker has arranged segments<br>in memory|



##  **NOTE**

UF2 (USB Flashing Format) is a Microsoft-developed file format used for flashing Raspberry Pi microcontrollers over USB. For more information, see the Microsoft UF2 Specification Repo.

##  **NOTE**

To build a binary to run in SRAM, rather than Flash memory you can either setup your cmake build with -DPICO_NO_FLASH=1 or you can add pico_set_binary_type(TARGET_NAME no_flash) to control it on a per binary basis in your CMakeLists.txt file. You can download the RAM binary to Raspberry Pi microcontrollers via UF2. For example, if there is no flash chip on your board, you can download a binary that runs on the on-chip RAM using UF2 as it simply specifies the addresses of where data goes. Note you can only download in to RAM or FLASH, not both.

## **Debugging your project**

Debugging your own project from the command line follows the same processes as we used for the "Hello World" example back in Debug with SWD.

## **Need more detail?**

There should be enough here to show you _how_ to get started, but you may find yourself wondering _why_ some of these files and incantations are needed. The **Raspberry Pi Pico-series C/C++ SDK** book dives

Manually Create your own Project

**39**

Getting started with Raspberry Pi Pico-series

deeper into how your project is actually built, and how the lines in our CMakeLists.txt files here relate to the structure of the SDK, if you find yourself wanting to know more at some future point.

Manually Create your own Project

**40**
