Hardware design with RP2040

## **Appendix A: Using the rescue debug port**

## **Overview**

The rescue debug port (DP) on RP2040 can be used to reset the chip into a known state if the user has programmed some bad code into the flash. For example some code that turned off the system clock would stop the processor debug ports being accessed, but the rescue DP would still work because it is clocked from the SWCLK of the SWD interface.

On the Raspberry Pi Pico, the BOOTSEL button can be used to force the chip into BOOTSEL mode instead of executing the code in flash. The rescue DP is intended for use in designs that use an RP2040 but don’t have a BOOTSEL button.

##  **NOTE**

For further information on how to configure SWD see the **Getting started with Raspberry Pi Pico-series** book.

## **Activating the rescue DP from OpenOCD**

The RP2040 port of OpenOCD provides two targets:

- [rp2040.cfg]

- [rp2040-rescue.cfg]

rp2040-rescue.cfg connects to the rescue debug port with id 0xf.

To use the rescue DP, start OpenOCD with the rp2040-rescue configuration.

$ openocd -f interface/raspberrypi-swd.cfg -f target/rp2040-rescue.cfg ... Warn : gdb services need one or more targets defined Now attach a debugger to your RP2040 and load some code Info : Listening on port 6666 for tcl connections Info : Listening on port 4444 for telnet connections Ctrl + C

Now start OpenOCD with the normal rp2040 configuration.

$ openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg

To verify the rescue DP restarted the chip, you can check the VREG_AND_POR.CHIP_RESET register: 0x40064008. Bit 20 of this register is the HAD_PSM_RESTART bit.

In another terminal connect to the OpenOCD telnet port and use mdw (memory display word) to read the CHIP_RESET register. If the rescue DP restarted the chip, then the value will be 0x00100000, aka bit 20 will be set.

Overview

**31**

Hardware design with RP2040

$ telnet 127.0.0.1 4444 Trying 127.0.0.1... Connected to 127.0.0.1. Escape character is '^]'. Open On-Chip Debugger > mdw  0x40064008 0x40064008: 00100000

You can now load code as described in Use GDB and OpenOCD to debug Hello World in **Getting started with Raspberry Pi Pico-series** book.

Activating the rescue DP from OpenOCD

**32**
