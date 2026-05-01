Getting started with Raspberry Pi Pico-series

## **Appendix B: Picotool**

It is possible to embed information into a Pico-series binary, which can be retrieved using a command line utility called picotool.

## **Getting picotool**

The picotool utility is available in its own repository. You will need to clone and build it if you haven’t ran the pico-setup script.

$ git clone https://github.com/raspberrypi/picotool.git $ cd picotool

You will also need to install libusb if it is not already installed,

$ sudo apt install libusb-1.0-0-dev

##  **NOTE**

If you are building picotool on macOS you can install libusb using Homebrew,

$ brew install libusb pkg-config

While if you are building on Microsoft Windows you can download and install a Windows binary of libusb directly from the libusb.info site.

## **Building picotool**

Building picotool can be done as follows,

$ mkdir build $ cd build $ export PICO_SDK_PATH=~/pico/pico-sdk $ cmake ../ $ make

this will generate a picotool command-line binary in the build/picotool directory.

Getting picotool

**24**

Getting started with Raspberry Pi Pico-series

##  **NOTE**

If you are building on Microsoft Windows you should invoke CMake as follows,

C:\Users\pico\picotool> mkdir build

C:\Users\pico\picotool> cd build

C:\Users\pico\picotool\build> cmake .. -G "NMake Makefiles"

C:\Users\pico\picotool\build> nmake

## **Using picotool**

The picotool binary includes a command-line help function,

$ picotool help PICOTOOL:

Tool for interacting with RP2040/RP2350 device(s) in BOOTSEL mode, or with an RP2040/RP2350 binary

SYNOPSIS: picotool info [-b] [-p] [-d] [--debug] [-l] [-a] [device-selection] picotool info [-b] [-p] [-d] [--debug] [-l] [-a] <filename> [-t <type>] picotool config [-s <key> <value>] [-g <group>] [device-selection] picotool config [-s <key> <value>] [-g <group>] <filename> [-t <type>] picotool load [-p] [--family <family_id>] [-n] [-N] [-u] [-v] [-x] <filename> [-t <type>] [-o <offset>] [device-selection] picotool encrypt [--quiet] [--verbose] [--hash] [--sign] <infile> [-t <type>] [-o <offset>] <outfile> [-t <type>] <aes_key> [-t <type>] [<signing_key>] [-t <type>] picotool seal [--quiet] [--verbose] [--hash] [--sign] [--clear] <infile> [-t <type>] [-o <offset>] <outfile> [-t <type>] [<key>] [-t <type>] [<otp>] [-t <type>] [--major <major>] [--minor <minor>] [--rollback <rollback> [<rows>..]] picotool link [--quiet] [--verbose] <outfile> [-t <type>] <infile1> [-t <type>] <infile2> [-t <type>] [<infile3>] [-t <type>] [-p] <pad> picotool save [-p] [device-selection] picotool save -a [device-selection] picotool save -r <from> <to> [device-selection] picotool verify [device-selection] picotool reboot [-a] [-u] [-g <partition>] [-c <cpu>] [device-selection] picotool otp list|get|set|load|dump|permissions|white-label picotool partition info|create picotool uf2 info|convert picotool version [-s] [<version>] picotool coprodis [--quiet] [--verbose] <infile> [-t <type>] <outfile> [-t <type>] picotool help [<cmd>]

COMMANDS: info        Display information from the target device(s) or file. Without any arguments, this will display basic information for all connected RP2040 devices in BOOTSEL mode config      Display or change program configuration settings from the target device(s) or file. load        Load the program / memory range stored in a file onto the device. encrypt     Encrypt the program. seal        Add final metadata to a binary, optionally including a hash and/or signature. link        Link multiple binaries into one block loop. save        Save the program / memory stored in flash on the device to a file. verify      Check that the device contents match those in the file. reboot      Reboot the device otp         Commands related to the RP2350 OTP (One-Time-Programmable) Memory

Using picotool

**25**

Getting started with Raspberry Pi Pico-series

partition   Commands related to RP2350 Partition Tables uf2         Commands related to UF2 creation and status version     Display picotool version coprodis    Post-process coprocessor instructions in dissassembly files. help        Show general help or help for a specific command Use "picotool help <cmd>" for more info

##  **NOTE**

The majority of commands require a Raspberry Pi microcontroller device in BOOTSEL mode to be connected.

##  **IMPORTANT**

If you get an error message No accessible RP2040/RP2350 devices in BOOTSEL mode were found. accompanied with a note similar to Device at bus 1, address 7 appears to be a RP2040 device in BOOTSEL mode, but picotool was unable to connect indicating that there was a Pico-series device connected then you can run picotool using sudo, e.g.

$ sudo picotool info -a

If you get this message on Windows you will need to install a driver.

Download and run Zadig, select RP2 Boot (Interface 1) from the dropdown box and select WinUSB as the driver, and click on the "Install Driver" button. Wait for the installation to complete - this may take a few minutes.

As of version 1.1 of picotool it is also possible to interact with Raspberry Pi microcontrollers that are not in BOOTSEL mode, but are using USB stdio support from the SDK by using the -f argument of picotool.

## **Display information**

So there is now _Binary Information_ support in the SDK which allows for easily storing compact information that picotool can find (See Binary Information below). The info command is for reading this information.

The information can be either read from one or more connected Raspberry Pi microcontrollers in BOOTSEL mode, or from a file. This file can be an ELF, a UF2 or a BIN file.

$ picotool help info INFO: Display information from the target device(s) or file. Without any arguments, this will display basic information for all connected RP2040 devices in BOOTSEL mode SYNOPSIS: picotool info [-b] [-p] [-d] [-l] [-a] [--bus <bus>] [--address <addr>] [-f] [-F] picotool info [-b] [-p] [-d] [-l] [-a] <filename> [-t <type>] OPTIONS: Information to display -b, --basic Include basic information. This is the default -p, --pins Include pin information -d, --device Include device information -l, --build Include build attributes -a, --all

Using picotool

**26**

Getting started with Raspberry Pi Pico-series

Include all information

TARGET SELECTION: To target one or more connected RP2040 device(s) in BOOTSEL mode (the default) --bus <bus> Filter devices by USB bus number --address <addr> Filter devices by USB device address -f, --force Force a device not in BOOTSEL mode but running compatible code to reset so the command can be executed. After executing the command (unless the command itself is a 'reboot') the device will be rebooted back to application mode -F, --force-no-reboot Force a device not in BOOTSEL mode but running compatible code to reset so the command can be executed. After executing the command (unless the command itself is a 'reboot') the device will be left connected and accessible to picotool, but without the RPI-RP2 drive mounted To target a file <filename> The file name -t <type> Specify file type (uf2 | elf | bin) explicitly, ignoring file extension

For example, connect your Pico-series device to your computer as mass storage mode, by pressing and holding the BOOTSEL button before plugging it into the USB. Then open up a Terminal window and type,

$ sudo picotool info Program Information name:      hello_world features:  stdout to UART

or,

$ sudo picotool info -a Program Information name:          hello_world features:      stdout to UART binary start:  0x10000000 binary end:    0x1000606c Fixed Pin Information 20:  UART1 TX 21:  UART1 RX Build Information build date:        Dec 31 2020 build attributes:  Debug build Device Information flash size:   2048K ROM version:  2

for more information. Alternatively you can just get information on the pins used as follows,

Using picotool

**27**

Getting started with Raspberry Pi Pico-series

$ sudo picotool info -bp Program Information name:      hello_world features:  stdout to UART Fixed Pin Information 20:  UART1 TX 21:  UART1 RX

The tool can also be used on binaries still on your local filesystem,

$ picotool info -a lcd_1602_i2c.uf2 File lcd_1602_i2c.uf2: Program Information name:          lcd_1602_i2c web site:      https://github.com/raspberrypi/pico-examples/tree/HEAD/i2c/lcd_1602_i2c binary start:  0x10000000 binary end:    0x10003c1c Fixed Pin Information 4:  I2C0 SDA 5:  I2C0 SCL Build Information build date:  Dec 31 2020

## **Save the program**

Save allows you to save a range of memory or a program or the whole of flash from the device to a BIN file or a UF2 file.

$ picotool help save SAVE: Save the program / memory stored in flash on the device to a file. SYNOPSIS: picotool save [-p] [--bus <bus>] [--address <addr>] [-f] [-F] <filename> [-t <type>] picotool save -a [--bus <bus>] [--address <addr>] [-f] [-F] <filename> [-t <type>] picotool save -r <from> <to> [--bus <bus>] [--address <addr>] [-f] [-F] <filename> [-t <type>] OPTIONS: Selection of data to save -p, --program Save the installed program only. This is the default -a, --all Save all of flash memory -r, --range Save a range of memory. Note that UF2s always store complete 256 byte-aligned blocks of 256 bytes, and the range is expanded accordingly <from> The lower address bound in hex <to> The upper address bound in hex Source device selection --bus <bus>

Using picotool

**28**

Getting started with Raspberry Pi Pico-series

Filter devices by USB bus number --address <addr> Filter devices by USB device address -f, --force Force a device not in BOOTSEL mode but running compatible code to reset so the command can be executed. After executing the command (unless the command itself is a 'reboot') the device will be rebooted back to application mode -F, --force-no-reboot Force a device not in BOOTSEL mode but running compatible code to reset so the command can be executed. After executing the command (unless the command itself is a 'reboot') the device will be left connected and accessible to picotool, but without the RPI-RP2 drive mounted File to save to <filename> The file name -t <type> Specify file type (uf2 | elf | bin) explicitly, ignoring file extension

For example,

$ sudo picotool info Program Information name:      lcd_1602_i2c web site:  https://github.com/raspberrypi/pico-examples/tree/HEAD/i2c/lcd_1602_i2c $ picotool save spoon.uf2 Saving file: [==============================]  100% Wrote 51200 bytes to spoon.uf2 $ picotool info spoon.uf2 File spoon.uf2: Program Information name:      lcd_1602_i2c web site:  https://github.com/raspberrypi/pico-examples/tree/HEAD/i2c/lcd_1602_i2c

## **Binary Information**

Binary information is machine-locatable and machine-readable information that is embedded in the binary at build time.

## **Basic information**

This information is really handy when you pick up a Pico-series device and don’t know what is on it!

Basic information includes

- [program name]

- [program description]

- [program version string]

- [program build date]

- [program url]

- [program end address]

- [program features, this is a list built from individual strings in the binary, that can be displayed (e.g. we will have one] for UART stdio and one for USB stdio) in the SDK

Using picotool

**29**

Getting started with Raspberry Pi Pico-series

- [build attributes, this is a similar list of strings, for things pertaining to the binary itself (e.g. Debug Build)]

## **Pins**

This is certainly handy when you have an executable called hello_serial.elf but you forgot what Raspberry Pi microcontroller-based board it was built for, as different boards may have different pins broken out.

Static (fixed) pin assignments can be recorded in the binary in very compact form:

$ picotool info --pins sprite_demo.elf File sprite_demo.elf: Fixed Pin Information 0-4:    Red 0-4 6-10:   Green 0-4 11-15:  Blue 0-4 16:     HSync 17:     VSync 18:     Display Enable 19:     Pixel Clock 20:     UART1 TX 21:     UART1 RX

## **Full Information**

Full information is available with the -a option:

$ picotool info -a i2c_bus_scan.elf File i2c_bus_scan.elf: Program Information name:          i2c_bus_scan web site:      https://github.com/raspberrypi/pico-examples/tree/HEAD/i2c/bus_scan features:      UART stdin / stdout binary start:  0x10000000 binary end:    0x10004c74 Fixed Pin Information 0:  UART0 TX 1:  UART0 RX 4:  I2C0 SDA 5:  I2C0 SCL Build Information sdk version:       2.0.0-develop pico_board:        pico build date:        Aug  1 2024 build attributes:  Debug

Using picotool

**30**
