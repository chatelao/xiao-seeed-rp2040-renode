Getting started with Raspberry Pi Pico-series

## **Appendix A: Debugprobe**

Raspberry Pi provides two ways to debug Pico-series devices:

- [the ][Raspberry Pi Debug Probe]

- [debugprobe][ firmware running on a second Pico or Pico 2]

Both methods provide a way to debug Pico-series devices on platforms that lack GPIOs to connect directly to UART or SWD, such as Windows, macOS, and Linux. The debugging device connects to your usual computer using USB, and to the Pico using SWD and UART.

## **Building OpenOCD**

Shortly after RP2350 launch you will likely need to build openocd from source if not using the VS Code extension. You can get a binary release from https://github.com/raspberrypi/pico-sdk-tools.

$ git clone https://github.com/raspberrypi/openocd.git $ cd openocd $ ./bootstrap $ ./configure --disable-werror $ make -j4

To start openocd from the build directory, you can use:

For RP2350:

sudo src/openocd -s tcl -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 5000"

For RP2040:

sudo src/openocd -s tcl -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"

## **Install OpenOCD**

To get started, you’ll need OpenOCD.

To install OpenOCD, run the following command in a terminal:

$ sudo apt install openocd

To install OpenOCD on macOS, run the following command:

$ brew install openocd

Building OpenOCD

**15**

Getting started with Raspberry Pi Pico-series

To run OpenOCD, use the openocd command in your terminal.

## **Debug Probe**

The simplest way to debug a Pico-series device is the Raspberry Pi Debug Probe. The Raspberry Pi Debug Probe provides Serial Wire Debug (SWD), and a generic USB-to-Serial bridge.

##  **NOTE**

For more information about the Debug Probe, see the documentation site.

## **Debug Probe wiring**

_Figure 7. Wires included with the Debug Probe._

**==> picture [319 x 183] intentionally omitted <==**

Debug Probe

**16**

Getting started with Raspberry Pi Pico-series

_Figure 8. Wiring between the Debug Probe (left) and Pico (right)._

**==> picture [319 x 311] intentionally omitted <==**

To connect Debug Probe to Pico H, connect the following:

- [Debug Probe "D" port to Pico H "DEBUG" SWD JST-SH connector]

- [Debug Probe "U" port, with the three-pin JST-SH connector to 0.1-inch header (male):]

   - [Debug Probe ][RX][ connected to Pico H ][TX][ pin]

   - [Debug Probe ][TX][ connected to Pico H ][RX][ pin]

   - [Debug Probe ][GND][ connected to Pico H ][GND][ pin]

Then, connect two USB cables: one from your computer to the microUSB port on Debug Probe and another from your computer to the microUSB port on Pico.

##  **NOTE**

If you have a non-H Pico, Pico 2 or Pico W (without a JST-SH connector) you can still connect it to a Debug Probe. Solder a male connector to the SWCLK, GND, and SWDIO header pins on the board. Using the alternate 3-pin JST-SH connector to 0.1-inch header (female) cable included with the Debug Probe, connect to the Debug Probe "D" port. Connect SWCLK, GND, and SWDIO on the Pico or Pico W to the SC, GND, and SD pins on the Debug Probe, respectively.

The wiring loom between Pico and the Debug Probe is shown in Figure 8.

## **Debug with a second Pico or Pico 2**

One Pico or Pico 2 can reprogram and debug another using the debugprobe firmware, which transforms the Pico or Pico 2 into a USB → SWD and UART bridge.

Debug with a second Pico or Pico 2

**17**

Getting started with Raspberry Pi Pico-series

_Figure 9. Wiring between Pico A (left) and Pico B (right) with Pico A acting as a debugger and Pico B as a system under test. You must connect at least the ground and the two SWD wires. Connect the UART serial port to provide access to the UART serial output of Pico B. You can also bridge the power supply to power both boards with one USB cable. For more information, see_ debugprobe _wiring._

**==> picture [224 x 168] intentionally omitted <==**

## **Install** debugprobe

You can download a UF2 binary of debugprobe from the Pico-series documentation.

Boot the debugger Pico or Pico 2 with the BOOTSEL button pressed. Copy debugprobe_on_pico.uf2 onto the device to begin debugging.

##  **NOTE**

Use debugprobe_on_pico.uf2 to use a Pico for debugging. Use debugprobe.uf2 for the Debug Probe accessory hardware.

## **Build** debugprobe

Alternatively, you can build debugprobe using the following instructions:

These build instructions assume you are running on Linux, and have installed the SDK.

##  **NOTE**

These instructions are for Pico; replace the -DPICO_BOARD=pico with -DPICO_BOARD=pico2 for Pico 2

$ cd ~/pico $ git clone https://github.com/raspberrypi/debugprobe.git $ cd debugprobe $ git submodule update --init $ mkdir build $ cd build $ export PICO_SDK_PATH=../../pico-sdk $ cmake -DDEBUG_ON_PICO=ON -DPICO_BOARD=pico .. $ make -j4

Boot the debugger Pico or Pico 2 with the BOOTSEL button pressed. Copy debugprobe.uf2 onto the device to begin debugging.

## debugprobe **wiring**

debugprobe wiring

**18**

Getting started with Raspberry Pi Pico-series

_Figure 10. Wiring between Pico A (left) and Pico B (right), configuring Pico A as a debugger._

**==> picture [319 x 240] intentionally omitted <==**

The wiring loom between the two Pico boards is shown in Figure 10.

Pico A GND -> Pico B GND Pico A GP2 -> Pico B SWCLK Pico A GP3 -> Pico B SWDIO Pico A GP4/UART1 TX -> Pico B GP1/UART0 RX Pico A GP5/UART1 RX -> Pico B GP0/UART0 TX

The minimum set of connections required to load and run code via OpenOCD is GND, SWCLK and SWDIO. Connect the UART wires to communicate with Pico B’s UART serial port through Pico A’s USB connection. You can also use the UART wires to talk to any other UART serial device, such as the boot console on a Raspberry Pi.

To power Pico A with Pico B, connect the following pins:

- [When using USB in device mode, or not at all, connect ][VSYS][ to ][VSYS]

- [When acting as a USB Host, connect ][VBUS][ to ][VBUS][ to provide 5V on the USB connector.]

## **Debug Probe interfaces**

Both the Debug Probe and any Pico-serires device running debugprobe are composite devices with two USB interfaces:

1. A class-compliant CDC UART (serial port), so it works on Windows out of the box.

2. A vendor-specific interface for SWD probe data conforming to CMSIS-DAP v2.

## **Use the UART**

## **Linux**

To use the UART connection on Linux, run the following command:

Debug Probe interfaces

**19**

Getting started with Raspberry Pi Pico-series

$ sudo minicom -D /dev/ttyACM0 -b 115200

## **Windows**

Download and install PuTTY.

Open Device Manager and locate the COM port number of the device running debugprobe. In this example it is COM7.

**==> picture [319 x 234] intentionally omitted <==**

Open PuTTY. Select Serial under connection type. Then type the name of your COM port along with 115200 as the speed.

Use the UART

**20**

Getting started with Raspberry Pi Pico-series

**==> picture [319 x 308] intentionally omitted <==**

Select Open to start the serial console. You are now ready to run your application.

**==> picture [319 x 202] intentionally omitted <==**

## **macOS**

First, install minicom using Homebrew:

$ brew install minicom

Then, run the following command to use the UART connection:

Use the UART

**21**

Getting started with Raspberry Pi Pico-series

$ minicom -D /dev/tty.usbmodem1234561 -b 115200

## **Debug with OpenOCD**

With Debug Probe, you can load binaries via the SWD port and OpenOCD.

First, build a binary. Then, run the following command to upload the binary to the Pico, replacing blink.elf with the name of the ELF file you just built:

$ sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program blink.elf verify reset exit"

If you are using a RP2350 based board, then use

$ sudo openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 5000" -c "program blink.elf verify reset exit"

## **Debug with SWD**

You can also use openocd in server mode and connect a debugger that provides break points and more.

##  **IMPORTANT**

To allow debugging, build your binaries with the Debug build type using the DCMAKE_BUILD_TYPE option: $ cd ~/pico/pico-examples/ $ rm -rf build $ mkdir build $ cd build $ export PICO_SDK_PATH=../../pico-sdk $ cmake -DCMAKE_BUILD_TYPE=Debug -DPICO_BOARD=pico .. $ cd blink $ make -j4

Note: you should use -DPICO_BOARD=pico2 for a Raspberry Pi Pico 2. The debug build provides more information when you run it under the debugger.

First, run an OpenOCD server:

$ sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"

For a RP2350-based device use -f target/rp2350.cfg instead.

Then, open a second terminal window. Start your debugger, passing your binary as an argument:

Debug with OpenOCD

**22**

Getting started with Raspberry Pi Pico-series

$ gdb blink.elf > target remote localhost:3333 > monitor reset init > continue

GDB doesn’t work on all platforms. Use one of the following alternatives instead of gdb, depending on your operating system and device:

- [On Linux devices that are ] _[not]_[ Raspberry Pis, use ][gdb-multiarch][.]

- [On Arm-based macOS devices, use ][lldb][.]

Debug with OpenOCD

**23**
