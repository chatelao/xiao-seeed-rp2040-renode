Raspberry Pi Pico-series Python SDK

## **Chapter 2. Connecting to the MicroPython REPL**

When MicroPython boots for the first time, it will sit and wait for you to connect and tell it what to do. You can load a .py file from your computer onto the board, but a more immediate way to interact with it is through what is called the _readevaluate-print loop_ , or REPL (often pronounced similarly to "ripple").

**Read** MicroPython waits for you to type in some text, followed by the enter key. **Evaluate** Whatever you typed is interpreted as Python code, and runs immediately. **Print** Any results of the last line you typed are printed out for you to read. **Loop** Go back to the start — prompt you for another line of code.

There are two ways to connect to this REPL, so you can communicate with the MicroPython firmware on your board: over USB, and over the UART serial port on Pico-series GPIOs.

## **2.1. Connecting from a Raspberry Pi over USB**

The MicroPython firmware is equipped with a virtual USB serial port which is accessed through the micro USB connector on Pico-series devices. Your computer should notice this serial port and list it as a character device, most likely /dev/ttyACM0.

##  **TIP**

You can run ls /dev/tty* to list your serial ports. There may be quite a few, but MicroPython’s USB serial will start with /dev/ttyACM. If in doubt, unplug the micro USB connector and see which one disappears. If you don’t see anything, you can try rebooting your Raspberry Pi.

You can install minicom to access the serial port:

$ sudo apt install minicom

and then open it as such:

$ minicom -o -D /dev/ttyACM0

Where the -D /dev/ttyACM0 is pointing minicom at MicroPython’s USB serial port, and the -o flag essentially means "just do it". There’s no need to worry about baud rate, since this is a virtual serial port.

Press the enter key a few times in the terminal where you opened minicom. You should see this:

>>>

This is a **prompt** . MicroPython wants you to type something in, and tell it what to do.

If you press CTRL-D on your keyboard whilst the minicom terminal is focused, you should see a message similar to this:

2.1. Connecting from a Raspberry Pi over USB

**8**

Raspberry Pi Pico-series Python SDK

MPY: soft reboot MicroPython v1.13-422-g904433073 on 2021-01-19; Raspberry Pi Pico with RP2040 Type "help()" for more information. >>>

This key combination tells MicroPython to reboot. You can do this at any time. When it reboots, MicroPython will print out a message saying exactly what firmware version it is running, and when it was built. Your version number will be different from the one shown here.

## **2.2. Connecting from a Raspberry Pi using UART**

##  **WARNING**

REPL over UART is disabled by default.

The MicroPython port for RP-series microcontrollers does not expose REPL over a UART port by default. However this default can be changed in the ports/rp2/mpconfigport.h source file. If you want to use the REPL over UART you’re going to have to build MicroPython yourself, see Section 1.3 for more details.

Go ahead and download the MicroPython source and in ports/rp2/mpconfigport.h change MICROPY_HW_ENABLE_UART_REPL to 1 to enable it.

_#define MICROPY_HW_ENABLE_UART_REPL             (1) // useful if there is no USB_

Then continue to follow the instructions in Section 1.3 to build your own MicroPython UF2 firmware.

This will allow the REPL to be accessed over a UART port, through two GPIO pins. The default settings for UARTs are taken from the C SDK.

_Table 1. Default UART settings in MicroPython_

|taken from the C SDK.||
|---|---|
|**Function**|**Default**|
|UART_BAUDRATE|115,200|
|UART_BITS|8|
|UART_STOP|1|
|UART0_TX|Pin 0|
|UART0_RX|Pin 1|
|UART1_TX|Pin 4|
|UART1_RX|Pin 5|



This alternative interface is handy if you have trouble with USB, if you don’t have any free USB ports, or if you are using some other RP-series microcontroller-based board which doesn’t have an exposed USB connector.

2.2. Connecting from a Raspberry Pi using UART

**9**

Raspberry Pi Pico-series Python SDK

##  **NOTE**

This initially occupies the UART0 peripheral on RP-series microcontrollers. The UART1 peripheral is free for you to use in your Python code as a second UART.

The next thing you’ll need to do is to enable UART serial on the Raspberry Pi. To do so, run raspi-config,

## $ sudo raspi-config

and go to Interfacing Options → Serial and select "No" when asked "Would you like a login shell to be accessible over serial?" and "Yes" when asked "Would you like the serial port hardware to be enabled?". You should see something like Figure 1.

_Figure 1. Enabling a serial UART using_ raspi-config _on the Raspberry Pi._

**==> picture [319 x 204] intentionally omitted <==**

Leaving raspi-config you should choose "Yes" and reboot your Raspberry Pi to enable the serial port.

You should then wire the Raspberry Pi and the Pico-series device together with the following mapping:

|**Raspberry Pi**|**Pico**|
|---|---|
|GND|GND|
|GPIO15 (UART_RX0)|GPIO0 (UART0_TX)|
|GPIO14 (UART_TX0)|GPOI1 (UART0_RX)|



##  **IMPORTANT**

RX matches to TX, and TX matches to RX. You mustn’t connect the two opposite TX pins together, or the two RX pins. This is because MicroPython needs to listen on the channel that the Raspberry Pi transmits on, and vice versa.

See Figure 2.

2.2. Connecting from a Raspberry Pi using UART

**10**

Raspberry Pi Pico-series Python SDK

_Figure 2. A Raspberry Pi 4 and the Raspberry Pi Pico with UART0 connected together._

**==> picture [319 x 227] intentionally omitted <==**

then connect to the board using minicom connected to /dev/serial0,

$ minicom -b 115200 -o -D /dev/serial0

If you press the enter key, MicroPython should respond by prompting you for more input:

>>>

## **2.3. Connecting from a Mac**

So long as you’re using a recent version of macOS like Catalina, drivers should already be loaded. Otherwise see the manufacturers' website for FTDI Chip Drivers. Then you should use a Terminal program to connect to Serial-over-USB (USB CDC). The serial port will show up as /dev/tty.usbmodem0000000000001

If you don’t already have a Terminal program installed you can install minicom using Homebrew,

$ brew install minicom

and connect to the board as below.

$ minicom -b 115200 -o -D /dev/tty.usbmodem0000000000001

2.3. Connecting from a Mac

**11**

Raspberry Pi Pico-series Python SDK

##  **NOTE**

Other Terminal applications like CoolTerm or Serial can also be used.

## **2.4. Say "Hello World"**

Once connected you can check that everything is working by typing a Python "Hello World" into the REPL,

>>> print("Hello, Pico!") Hello, Pico! >>>

## **2.5. Blink an LED**

The on-board LED on Raspberry Pi Pico and Pico is connected to GPIO pin 25, whereas on Raspberry Pi Pico W it is connected to the wireless chip. On both boards you can use the "LED" string. You can blink this on and off from the REPL. When you see the REPL prompt enter the following,

>>> from machine import Pin >>> led = Pin("LED", Pin.OUT)

The machine module is used to control on-chip hardware. This is standard on all MicroPython ports, and you can read more about it in the MicroPython documentation. Here we are using it to take control of a GPIO, so we can drive it high and low. If you type this in,

>>> led.value(1)

The LED should turn on. You can turn it off again with

>>> led.value(0)

## **2.6. What next?**

At this point you should have MicroPython installed on your board, and have tested your setup by typing short programs into the prompt to print some text back to you, and blink an LED.

You can read on to the next chapter, which goes into the specifics of MicroPython on RP-series microcontrollers, and where it differs from other platforms. Chapter 3 also has some short examples of the different APIs offered to interact with the hardware.

You can learn how to set up an _integrated development environment_ (IDE) in Chapter 4, so you don’t have to type programs in line by line.

You can dive straight into Appendix A if you are eager to start connecting wires to a breadboard.

2.4. Say "Hello World"

**12**
