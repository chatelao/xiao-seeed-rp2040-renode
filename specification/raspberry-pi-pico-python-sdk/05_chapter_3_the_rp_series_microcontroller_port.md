Raspberry Pi Pico-series Python SDK

## **Chapter 3. The RP-series microcontroller Port**

Currently supported features include:

- [REPL over USB and UART (on GP0/GP1).]

- [1600 kB filesystem using ][littlefs2][ on the on-board flash. (Default size for Raspberry Pi Pico)]

- [utime][ module with sleep and ticks functions.]

- [ubinascii][ module.]

- [machine][ module with some basic functions.]

   - [machine.Pin][ class.]

   - [machine.Timer][ class.]

   - [machine.ADC][ class.]

   - [machine.I2C][ and ][machine.SoftI2C][ classes.]

   - [machine.SPI][ and ][machine.SoftSPI][ classes.]

   - [machine.WDT][ class.]

   - [machine.PWM][ class.]

   - [machine.UART][ class.]

- [rp2][ platform-specific module.]

   - [PIO hardware access library]

   - [PIO program assembler]

   - [Raw flash read/write access]

- [Multicore support exposed via the standard ][_thread][ module]

- [Accelerated floating point arithmetic using the RP-series microcontroller ROM library and hardware divider (used] automatically)

Documentation around MicroPython is available from https://docs.micropython.org. For example the machine module, which can be used to access a lot of on-chip hardware, is standard, and you will find a lot of the information you need in the online documentation for that module.

This chapter will give a very brief tour of some of the hardware APIs, with code examples you can either type into the REPL (Chapter 2) or load onto the board using a development environment installed on your computer (Chapter 4).

## **3.1. Blinking an LED Forever (Timer)**

In Chapter 2 we saw how the machine.Pin class could be used to turn an LED on and off, by driving a GPIO high and low.

>>> from machine import Pin >>> led = Pin("LED", Pin.OUT) >>> led.value(1) >>> led.value(0)

3.1. Blinking an LED Forever (Timer)

**13**

Raspberry Pi Pico-series Python SDK

This is, to put it mildy, quite a convoluted way of turning a light on and off. A light switch would work better. The machine.Timer class, which uses RP-series microcontrollers' hardware timer to trigger callbacks at regular intervals, saves a lot of typing if we want the light to turn itself on and off repeatedly, thus bringing our level of automation from "mechanical switch" to "555 timer".

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/blink/blink.py_

1 from machine import Pin, Timer 2 3 led = Pin("LED", Pin.OUT) 4 tim = Timer() 5 def tick(timer): 6     global led 7     led.toggle() 8 9 tim.init(freq=2.5, mode=Timer.PERIODIC, callback=tick)

Typing this program into the REPL will cause the LED to start blinking, but the prompt will appear again:

>>>

The Timer we created will run in the background, at the interval we specified, blinking the LED. The MicroPython prompt is still running in the foreground, and we can enter more code, or start more timers.

## **3.2. UART**

##  **NOTE**

REPL over UART is disabled by default. See Section 2.2 for details of how to enable REPL over UART.

Example usage looping UART0 to UART1.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/uart/loopback/uart.py_

1 from machine import UART, Pin 2 import time 3 4 uart1 = UART(1, baudrate=9600, tx=Pin(8), rx=Pin(9)) 5 6 uart0 = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1)) 7 8 txData = b'hello world\n\r' 9 uart1.write(txData) 10 time.sleep(0.1) 11 rxData = bytes() 12 while uart0.any() > 0: 13     rxData += uart0.read(1) 14 15 print(rxData.decode('utf-8'))

For more detail, including a wiring diagram, see Appendix A.

3.2. UART

**14**

Raspberry Pi Pico-series Python SDK

## **3.3. ADC**

An analogue-to-digital converter (ADC) measures some analogue signal and encodes it as a digital number. The ADC on RP-series microcontrollers measures voltages.

An ADC has two key features: its resolution, measured in digital bits, and its channels, or how many analogue signals it can accept and convert at once. The ADC on RP2350 and RP2040 has a resolution of 12-bits, meaning that it can transform an analogue signal into a digital signal as a number ranging from 0 to 4095 – though this is handled in MicroPython transformed to a 16-bit number ranging from 0 to 65,535, so that it behaves the same as the ADC on other MicroPython microcontrollers.

RP2350 and RP2040 have five ADC channels total, four of which are brought out to chip GPIOs: GP26, GP27, GP28 and GP29. On Pico W and Pico, the first three of these are brought out to GPIO pins, and the fourth can be used to measure the VSYS voltage on the board.

The ADC’s fifth input channel is connected to a temperature sensor built into RP2350 and RP2040.

You can specify which ADC channel you’re using by pin number:

adc = machine.ADC(26) _# Connect to GP26, which is channel 0_

or by channel:

adc = machine.ADC(4) _# Connect to the internal temperature sensor_ adc = machine.ADC(0) _# Connect to channel 0 (GP26)_

An example reading the fourth analogue-to-digital (ADC) converter channel, connected to the internal temperature sensor:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/adc/temperature.py_

1 import machine 2 import utime 3 4 sensor_temp = machine.ADC(4) 5 conversion_factor = 3.3 / (65535) 6 7 while True: 8     reading = sensor_temp.read_u16() * conversion_factor 9 10 _# The temperature sensor measures the Vbe voltage of a biased bipolar diode, connected to the fifth ADC channel_ 11 _# Typically, Vbe = 0.706V at 27 degrees C, with a slope of -1.721mV (0.001721) per degree._ 12     temperature = 27 - (reading - 0.706)/0.001721 13     print(temperature) 14     utime.sleep(2)

## **3.4. Interrupts**

You can set an IRQ like this:

3.3. ADC

**15**

Raspberry Pi Pico-series Python SDK

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/irq/irq.py_

1 from machine import Pin 2 3 p2 = Pin(2, Pin.IN, Pin.PULL_UP) 4 p2.irq(lambda pin: print("IRQ with flags:", pin.irq().flags()), Pin.IRQ_FALLING)

It should print out something when GP2 has a falling edge.

## **3.5. Multicore Support**

Example usage:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/multicore/multicore.py_

1 import time, _thread, machine 2 3 def task(n, delay): 4     led = machine.Pin("LED", machine.Pin.OUT) 5     for i in range(n): 6         led.high() 7         time.sleep(delay) 8         led.low() 9         time.sleep(delay) 10     print('done') 11 12 _thread.start_new_thread(task, (10, 0.5))

Only one thread can be started/running at any one time, because there is no RTOS just a second core. The GIL is not enabled so both core0 and core1 can run Python code concurrently, with care to use locks for shared data.

## **3.6. I2C**

Example usage:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/i2c.py_

1 from machine import Pin, I2C 2 3 i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=100000) 4 i2c.scan() 5 i2c.writeto(76, b'123') 6 i2c.readfrom(76, 4) 7 8 i2c = I2C(1, scl=Pin(7), sda=Pin(6), freq=100000) 9 i2c.scan() 10 i2c.writeto_mem(76, 6, b'456') 11 i2c.readfrom_mem(76, 6, 4)

I2C can be constructed without specifying the frequency, if you just want all the defaults.

3.5. Multicore Support

**16**

Raspberry Pi Pico-series Python SDK

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/i2c_without_freq.py_

1 from machine import I2C 2

3 i2c = I2C(0) _# defaults to SCL=Pin(9), SDA=Pin(8), freq=400000_

##  **WARNING**

There may be some bugs reading/writing to device addresses that do not respond, the hardware seems to lock up in some cases.

_Table 2. Default I2C pins_

|**Function**|**Default**|
|---|---|
|I2C Frequency|400,000|
|I2C0 SCL|Pin 9|
|I2C0 SDA|Pin 8|
|I2C1 SCL|Pin 7|
|I2C1 SDA|Pin 6|



## **3.7. SPI**

Example usage:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/spi/spi.py_

1 from machine import SPI 2 3 spi = SPI(0) 4 spi = SPI(0, 100_000) 5 spi = SPI(0, 100_000, polarity=1, phase=1) 6 7 spi.write('test') 8 spi.read(5) 9 10 buf = bytearray(3) 11 spi.write_readinto('out', buf)

##  **NOTE**

The chip select must be managed separately using a machine.Pin.

|_Table 3. Default SPI_<br>_pins_|**Function**|**Default**|
|---|---|---|
||SPI_BAUDRATE|1,000,000|
||SPI_POLARITY|0|
||SPI_PHASE|0|
||SPI_BITS|8|
||SPI_FIRSTBIT|MSB|
||SPI0_SCK|Pin 6|



3.7. SPI

**17**

Raspberry Pi Pico-series Python SDK

|SPI0_MOSI|Pin 7|
|---|---|
|SPI0_MISO|Pin 4|
|SPI1_SCK|Pin 10|
|SPI1_MOSI|Pin 11|
|SPI1_MISO|Pin 8|



## **3.8. PWM**

Example of using PWM to fade an LED:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pwm/pwm_fade.py_

1 _# Example using PWM to fade an LED._ 2 3 import time 4 from machine import Pin, PWM 5 6 7 _# Construct PWM object, with LED on Pin(25)._ 8 pwm = PWM(Pin(25)) 9 10 _# Set the PWM frequency._ 11 pwm.freq(1000) 12 13 _# Fade the LED in and out a few times._ 14 duty = 0 15 direction = 1 16 for _ in range(8 * 256): 17     duty += direction 18     if duty > 255: 19         duty = 255 20         direction = -1 21     elif duty < 0: 22         duty = 0 23         direction = 1 24     pwm.duty_u16(duty * duty) 25     time.sleep(0.001)

This example does not work with Raspberry Pi Pico W as the on-board LED is connected via the 43439 wireless chip rather than directly to the RP2040 itself. The example will work with an off-board LED, e.g. one wired to GP15 as shown below.

3.8. PWM

**18**

Raspberry Pi Pico-series Python SDK

_Figure 3. Connecting your Raspberry Pi Pico W to an off-board LED._

**==> picture [425 x 291] intentionally omitted <==**

## **3.9. PIO Support**

Current support allows you to define Programmable IO (PIO) Assembler blocks and using them in the PIO peripheral, more documentation around PIO can be found in Chapter 3 of the **RP2040 Datasheet** and Chapter 4 of the **Raspberry Pi Pico-series C/C++ SDK** book.

The Pico-series MicroPython introduces a new @rp2.asm_pio decorator, along with a rp2.PIO class. The definition of a PIO program, and the configuration of the state machine, into 2 logical parts:

- [The program definition, including how many pins are used and if they are in/out pins. This goes in the ][@rp2.asm_pio] definition. This is close to what the pioasm tool from the SDK would generate from a .pio file (but here it’s all defined in Python).

- [The program instantiation, which sets the frequency of the state machine and which pins to bind to. These get set] when setting a SM to run a particular program.

The aim was to allow a program to be defined once and then easily instantiated multiple times (if needed) with different GPIO. Another aim was to make it easy to do basic things without getting weighed down in too much PIO/SM configuration.

##  **NOTE**

The following examples will not work with the on-board LED on Raspberry Pi Pico W, as PIO is unable to access the wireless chip.

Example usage, to blink the on-board LED connected to GPIO 25,

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_blink.py_

1 import time 2 import rp2 3 from machine import Pin

3.9. PIO Support

**19**

Raspberry Pi Pico-series Python SDK

4 5 _# Define the blink program.  It has one GPIO to bind to on the set instruction, which is an output pin._ 6 _# Use lots of delays to make the blinking visible by eye._ 7 @rp2.asm_pio(set_init=rp2.PIO.OUT_LOW) 8 def blink(): 9     wrap_target() 10     set(pins, 1)   [31] 11     nop()          [31] 12     nop()          [31] 13     nop()          [31] 14     nop()          [31] 15     set(pins, 0)   [31] 16     nop()          [31] 17     nop()          [31] 18     nop()          [31] 19     nop()          [31] 20     wrap() 21 22 _# Instantiate a state machine with the blink program, at 2000Hz, with set bound to Pin(25) (LED on the Pico board)_ 23 sm = rp2.StateMachine(0, blink, freq=2000, set_base=Pin(25)) 24 25 _# Run the state machine for 3 seconds.  The LED should blink._ 26 sm.active(1) 27 time.sleep(3) 28 sm.active(0)

or via explicit exec.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_exec.py_

1 _# Example using PIO to turn on an LED via an explicit exec._ 2 _#_ 3 _# Demonstrates:_ 4 _#   - using set_init and set_base_ 5 _#   - using StateMachine.exec_ 6 7 import time 8 from machine import Pin 9 import rp2 10 11 _# Define an empty program that uses a single set pin._ 12 @rp2.asm_pio(set_init=rp2.PIO.OUT_LOW) 13 def prog(): 14     pass 15 16 17 _# Construct the StateMachine, binding Pin 25 to the set pin._ 18 sm = rp2.StateMachine(0, prog, set_base=Pin(25)) 19 20 _# Turn on the set pin via an exec instruction._ 21 sm.exec("set(pins, 1)") 22 23 _# Sleep for 500ms._ 24 time.sleep(0.5) 25 26 _# Turn off the set pin via an exec instruction._ 27 sm.exec("set(pins, 0)")

Some points to note,

3.9. PIO Support

**20**

Raspberry Pi Pico-series Python SDK

- [All program configuration (eg autopull) is done in the ][@asm_pio][ decorator. Only the frequency and base pins are set] in the StateMachine constructor.

- [[n]][ is used for delay, ][.set(n)][ used for sideset]

- [The assembler will automatically detect if sideset is used everywhere or only on a few instructions, and set the] SIDE_EN bit automatically

The idea is that for the 4 sets of pins (in, out, set, sideset, excluding jmp) that can be connected to a state machine, there’s the following that need configuring for each set:

1. base GPIO

2. number of consecutive GPIO

3. initial GPIO direction (in or out pin)

4. initial GPIO value (high or low)

In the design of the Python API for PIO these 4 items are split into "declaration" (items 2-4) and "instantiation" (item 1). In other words, a program is written with items 2-4 fixed for that program (eg a WS2812 driver would have 1 output pin) and item 1 is free to change without changing the program (eg which pin the WS2812 is connected to).

So in the @asm_pio decorator you declare items 2-4, and in the StateMachine constructor you say which base pin to use (item 1). That makes it easy to define a single program and instantiate it multiple times on different pins (you can’t really change items 2-4 for a different instantiation of the same program, it doesn’t really make sense to do that).

To declare multiple pins in the decorator (e.g. the count: item 2 above), use a tuple. Each item in the tuple specifies items 3 and 4. For example:

1 @asm_pio(set_init=(PIO.OUT_LOW, PIO.OUT_HIGH, PIO.IN_LOW), sideset_init=PIO.OUT_LOW, in_init =PIO.IN_HIGH) 2 def foo(): 3     .... 4 5 sm = StateMachine(0, foo, freq=10000, set_base=Pin(15), sideset_base=Pin(22))

In this example:

- [there are 3 set pins connected to the SM, and their initial state (set when the StateMachine is created) is: output] low, output high, input low (used for open-drain)

- [there is 1 sideset pin, initial state is output low]

- [the 3 set pins start at ][Pin(15)]

- [the 1 sideset pin starts at ][Pin(22)]

The reason to have the constants OUT_LOW, OUT_HIGH, IN_LOW and IN_HIGH is so that the pin value and dir are automatically set before the start of the PIO program (instead of wasting instruction words to do set(pindirs, 1) etc at the start).

## **3.9.1. IRQ**

There is support for PIO IRQs, e.g.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_irq.py_

1 import time 2 import rp2 3 4 @rp2.asm_pio() 5 def irq_test():

3.9. PIO Support

**21**

Raspberry Pi Pico-series Python SDK

6     wrap_target() 7     nop()          [31] 8     nop()          [31] 9     nop()          [31] 10     nop()          [31] 11     irq(0) 12     nop()          [31] 13     nop()          [31] 14     nop()          [31] 15     nop()          [31] 16     irq(1) 17     wrap() 18 19 20 rp2.PIO(0).irq(lambda pio: print(pio.irq().flags())) 21 22 sm = rp2.StateMachine(0, irq_test, freq=2000) 23 sm.active(1) 24 time.sleep(1) 25 sm.active(0)

An example program that blinks at 1Hz and raises an IRQ at 1Hz to print the current millisecond timestamp,

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_1hz.py_

1 _# Example using PIO to blink an LED and raise an IRQ at 1Hz._ 2 3 import time 4 from machine import Pin 5 import rp2 6 7 8 @rp2.asm_pio(set_init=rp2.PIO.OUT_LOW) 9 def blink_1hz(): 10 _# Cycles: 1 + 1 + 6 + 32 * (30 + 1) = 1000_ 11     irq(rel(0)) 12     set(pins, 1) 13     set(x, 31)                  [5] 14     label("delay_high") 15     nop()                       [29] 16     jmp(x_dec, "delay_high") 17 18 _# Cycles: 1 + 7 + 32 * (30 + 1) = 1000_ 19     set(pins, 0) 20     set(x, 31)                  [6] 21     label("delay_low") 22     nop()                       [29] 23     jmp(x_dec, "delay_low") 24 25 26 _# Create the StateMachine with the blink_1hz program, outputting on Pin(25)._ 27 sm = rp2.StateMachine(0, blink_1hz, freq=2000, set_base=Pin(25)) 28 29 _# Set the IRQ handler to print the millisecond timestamp._ 30 sm.irq(lambda p: print(time.ticks_ms())) 31 32 _# Start the StateMachine._ 33 sm.active(1)

or to wait for a pin change and raise an IRQ.

3.9. PIO Support

**22**

Raspberry Pi Pico-series Python SDK

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_pinchange.py_

1 _# Example using PIO to wait for a pin change and raise an IRQ._ 2 _#_ 3 _# Demonstrates:_ 4 _#   - PIO wrapping_ 5 _#   - PIO wait instruction, waiting on an input pin_ 6 _#   - PIO irq instruction, in blocking mode with relative IRQ number_ 7 _#   - setting the in_base pin for a StateMachine_ 8 _#   - setting an irq handler for a StateMachine_ 9 _#   - instantiating 2x StateMachine's with the same program and different pins_ 10 11 import time 12 from machine import Pin 13 import rp2 14 15 16 @rp2.asm_pio() 17 def wait_pin_low(): 18     wrap_target() 19 20     wait(0, pin, 0) 21     irq(block, rel(0)) 22     wait(1, pin, 0) 23 24     wrap() 25 26 27 def handler(sm): 28 _# Print a (wrapping) timestamp, and the state machine object._ 29     print(time.ticks_ms(), sm) 30 31 32 _# Instantiate StateMachine(0) with wait_pin_low program on Pin(16)._ 33 pin16 = Pin(16, Pin.IN, Pin.PULL_UP) 34 sm0 = rp2.StateMachine(0, wait_pin_low, in_base=pin16) 35 sm0.irq(handler) 36 37 _# Instantiate StateMachine(1) with wait_pin_low program on Pin(17)._ 38 pin17 = Pin(17, Pin.IN, Pin.PULL_UP) 39 sm1 = rp2.StateMachine(1, wait_pin_low, in_base=pin17) 40 sm1.irq(handler) 41 42 _# Start the StateMachine's running._ 43 sm0.active(1) 44 sm1.active(1) 45 46 _# Now, when Pin(16) or Pin(17) is pulled low a message will be printed to the REPL._

## **3.9.2. WS2812 LED (NeoPixel)**

While a WS2812 LED (NeoPixel) can be driven via the following program,

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_ws2812.py_

1 _# Example using PIO to drive a set of WS2812 LEDs._ 2 3 import array, time 4 from machine import Pin 5 import rp2

3.9. PIO Support

**23**

Raspberry Pi Pico-series Python SDK

6 7 _# Configure the number of WS2812 LEDs._ 8 NUM_LEDS = 8 9 10 11 @rp2.asm_pio(sideset_init=rp2.PIO.OUT_LOW, out_shiftdir=rp2.PIO.SHIFT_LEFT, autopull=True, pull_thresh=24) 12 def ws2812(): 13     T1 = 2 14     T2 = 5 15     T3 = 3 16     wrap_target() 17     label("bitloop") 18     out(x, 1)               .side(0)    [T3 - 1] 19     jmp(not_x, "do_zero")   .side(1)    [T1 - 1] 20     jmp("bitloop")          .side(1)    [T2 - 1] 21     label("do_zero") 22     nop()                   .side(0)    [T2 - 1] 23     wrap() 24 25 26 _# Create the StateMachine with the ws2812 program, outputting on Pin(22)._ 27 sm = rp2.StateMachine(0, ws2812, freq=8_000_000, sideset_base=Pin(22)) 28 29 _# Start the StateMachine, it will wait for data on its FIFO._ 30 sm.active(1) 31 32 _# Display a pattern on the LEDs via an array of LED RGB values._ 33 ar = array.array("I", [0 for _ in range(NUM_LEDS)]) 34 35 _# Cycle colours._ 36 for i in range(4 * NUM_LEDS): 37     for j in range(NUM_LEDS): 38         r = j * 100 // (NUM_LEDS - 1) 39         b = 100 - j * 100 // (NUM_LEDS - 1) 40         if j != i % NUM_LEDS: 41             r >>= 3 42             b >>= 3 43         ar[j] = r << 16 | b 44     sm.put(ar, 8) 45     time.sleep_ms(50) 46 47 _# Fade out._ 48 for i in range(24): 49     for j in range(NUM_LEDS): 50         ar[j] >>= 1 51     sm.put(ar, 8) 52     time.sleep_ms(50)

## **3.9.3. UART TX**

A UART TX example,

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_uart_tx.py_

1 _# Example using PIO to create a UART TX interface_ 2 3 from machine import Pin 4 from rp2 import PIO, StateMachine, asm_pio 5

3.9. PIO Support

**24**

Raspberry Pi Pico-series Python SDK

6 UART_BAUD = 115200 7 PIN_BASE = 10 8 NUM_UARTS = 8 9 10 11 @asm_pio(sideset_init=PIO.OUT_HIGH, out_init=PIO.OUT_HIGH, out_shiftdir=PIO.SHIFT_RIGHT) 12 def uart_tx(): 13 _# Block with TX deasserted until data available_ 14     pull() 15 _# Initialise bit counter, assert start bit for 8 cycles_ 16     set(x, 7)  .side(0)       [7] 17 _# Shift out 8 data bits, 8 execution cycles per bit_ 18     label("bitloop") 19     out(pins, 1)              [6] 20     jmp(x_dec, "bitloop") 21 _# Assert stop bit for 8 cycles total (incl 1 for pull())_ 22     nop()      .side(1)       [6] 23 24 25 _# Now we add 8 UART TXs, on pins 10 to 17. Use the same baud rate for all of them._ 26 uarts = [] 27 for i in range(NUM_UARTS): 28     sm = StateMachine( 29         i, uart_tx, freq=8 * UART_BAUD, sideset_base=Pin(PIN_BASE + i), out_base=Pin(PIN_BASE + i) 30     ) 31     sm.active(1) 32     uarts.append(sm) 33 34 _# We can print characters from each UART by pushing them to the TX FIFO_ 35 def pio_uart_print(sm, s): 36     for c in s: 37         sm.put(ord(c)) 38 39 40 _# Print a different message from each UART_ 41 for i, u in enumerate(uarts): 42     pio_uart_print(u, "Hello from UART {}!\n".format(i))

##  **NOTE**

You need to specify an initial OUT pin state in your program in order to be able to pass OUT mapping to your SM instantiation, even though in this program it is redundant because the mappings overlap.

## **3.9.4. SPI**

An SPI example.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_spi.py_

1 import rp2 2 from machine import Pin 3 4 @rp2.asm_pio(out_shiftdir=0, autopull=True, pull_thresh=8, autopush=True, push_thresh=8, sideset_init=(rp2.PIO.OUT_LOW, rp2.PIO.OUT_HIGH), out_init=rp2.PIO.OUT_LOW) 5 def spi_cpha0(): 6 _# Note X must be preinitialised by setup code before first byte, we reload after sending each byte_ 7 _# Would normally do this via exec() but in this case it's in the instruction memory and is_

3.9. PIO Support

**25**

Raspberry Pi Pico-series Python SDK

_only run once_ 8     set(x, 6) 9 _# Actual program body follows_ 10     wrap_target() 11     pull(ifempty)            .side(0x2)   [1] 12     label("bitloop") 13     out(pins, 1)             .side(0x0)   [1] 14     in_(pins, 1)             .side(0x1) 15     jmp(x_dec, "bitloop")    .side(0x1) 16 17     out(pins, 1)             .side(0x0) 18     set(x, 6)                .side(0x0) _# Note this could be replaced with mov x, y for programmable frame size_ 19     in_(pins, 1)             .side(0x1) 20     jmp(not_osre, "bitloop") .side(0x1) _# Fallthru if TXF empties_ 21 22     nop()                    .side(0x0)   [1] _# CSn back porch_ 23     wrap() 24 25 26 class PIOSPI: 27 28     def __init__(self, sm_id, pin_mosi, pin_miso, pin_sck, cpha=False, cpol=False, freq =1000000): 29         assert(not(cpol or cpha)) 30         self._sm = rp2.StateMachine(sm_id, spi_cpha0, freq=4*freq, sideset_base=Pin(pin_sck), out_base=Pin(pin_mosi), in_base=Pin(pin_sck)) 31         self._sm.active(1) 32 33 _# Note this code will die spectacularly cause we're not draining the RX FIFO_ 34     def write_blocking(wdata): 35         for b in wdata: 36             self._sm.put(b << 24) 37 38     def read_blocking(n): 39         data = [] 40         for i in range(n): 41             data.append(self._sm.get() & 0xff) 42         return data 43 44     def write_read_blocking(wdata): 45         rdata = [] 46         for b in wdata: 47             self._sm.put(b << 24) 48             rdata.append(self._sm.get() & 0xff) 49         return rdata

##  **NOTE**

This SPI program supports programmable frame sizes (by holding the reload value for X counter in the Y register) but currently this can’t be used, because the autopull threshold is associated with the program, instead of the SM instantiation.

## **3.9.5. PWM**

A PWM example,

3.9. PIO Support

**26**

Raspberry Pi Pico-series Python SDK

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/pio_pwm.py_

1 _# Example of using PIO for PWM, and fading the brightness of an LED_ 2 3 from machine import Pin 4 from rp2 import PIO, StateMachine, asm_pio 5 from time import sleep 6 7 8 @asm_pio(sideset_init=PIO.OUT_LOW) 9 def pwm_prog(): 10     pull(noblock) .side(0) 11     mov(x, osr) _# Keep most recent pull data stashed in X, for recycling by noblock_ 12     mov(y, isr) _# ISR must be preloaded with PWM count max_ 13     label("pwmloop") 14     jmp(x_not_y, "skip") 15     nop()         .side(1) 16     label("skip") 17     jmp(y_dec, "pwmloop") 18 19 20 class PIOPWM: 21     def __init__(self, sm_id, pin, max_count, count_freq): 22         self._sm = StateMachine(sm_id, pwm_prog, freq=2 * count_freq, sideset_base=Pin(pin)) 23 _# Use exec() to load max count into ISR_ 24         self._sm.put(max_count) 25         self._sm.exec("pull()") 26         self._sm.exec("mov(isr, osr)") 27         self._sm.active(1) 28         self._max_count = max_count 29 30     def set(self, value): 31 _# Minimum value is -1 (completely turn off), 0 actually still produces narrow pulse_ 32         value = max(value, -1) 33         value = min(value, self._max_count) 34         self._sm.put(value) 35 36 37 _# Pin 25 on Pico boards_ 38 pwm = PIOPWM(0, 25, max_count=(1 << 16) - 1, count_freq=10_000_000) 39 40 while True: 41     for i in range(256): 42         pwm.set(i ** 2) 43         sleep(0.01)

##  **NOTE**

This example does not work with Raspberry Pi Pico W as the on-board LED is connected via the 43439 wireless chip rather than directly to the RP2040 itself. The example will work with an off-board LED connected via GPIO.

## **3.9.6. Using** pioasm

As well as writing PIO code inline in your MicroPython script you can use the pioasm tool from the C/C++ SDK to generate a Python file.

3.9. PIO Support

**27**

Raspberry Pi Pico-series Python SDK

$ pioasm -o python input (output)

For more information on pioasm see the **Raspberry Pi Pico-series C/C++ SDK** book which talks about the C/C++ SDK.

## **3.10. Wireless Support**

##  **IMPORTANT**

Wireless support is only available on Pico W and Pico 2 W, not on Pico.

Example usage:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/wireless/webserver.py_

1 import network 2 import socket 3 import time 4 5 from machine import Pin 6 7 led = Pin(15, Pin.OUT) 8 9 ssid = 'YOUR NETWORK NAME' 10 password = 'YOUR NETWORK PASSWORD' 11 12 wlan = network.WLAN(network.STA_IF) 13 wlan.active(True) 14 wlan.connect(ssid, password) 15 16 html = """<!DOCTYPE html> 17 <html> 18     <head> <title>Pico W</title> </head> 19     <body> <h1>Pico W</h1> 20         <p>%s</p> 21     </body> 22 </html> 23 """ 24 25 max_wait = 10 26 while max_wait > 0: 27     if wlan.status() < 0 or wlan.status() >= 3: 28         break 29     max_wait -= 1 30     print('waiting for connection...') 31     time.sleep(1) 32 33 if wlan.status() != 3: 34     raise RuntimeError('network connection failed') 35 else: 36     print('connected') 37     status = wlan.ifconfig() 38     print( 'ip = ' + status[0] ) 39 40 addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1] 41 42 s = socket.socket() 43 s.bind(addr)

3.10. Wireless Support

**28**

Raspberry Pi Pico-series Python SDK

44 s.listen(1) 45 46 print('listening on', addr) 47 48 _# Listen for connections_ 49 while True: 50     try: 51         cl, addr = s.accept() 52         print('client connected from', addr) 53         request = cl.recv(1024) 54         print(request) 55 56         request = str(request) 57         led_on = request.find('/light/on') 58         led_off = request.find('/light/off') 59         print( 'led on = ' + str(led_on)) 60         print( 'led off = ' + str(led_off)) 61 62         if led_on == 6: 63             print("led on") 64             led.value(1) 65             stateis = "LED is ON" 66 67         if led_off == 6: 68             print("led off") 69             led.value(0) 70             stateis = "LED is OFF" 71 72         response = html % stateis 73 74         cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n') 75         cl.send(response) 76         cl.close() 77 78     except OSError as e: 79         cl.close() 80         print('connection closed')

##  **NOTE**

Make sure to replace the ssid and password with the name and password for your own wireless network.

Here we have chosen to attach an external LED to GP15 of our Pico W, but you could just as easily use the on-board LED.

3.10. Wireless Support

**29**

Raspberry Pi Pico-series Python SDK

_Figure 4. Connecting your Raspberry Pi Pico W to a LED._

**==> picture [425 x 291] intentionally omitted <==**

After your Pico W connects to your wireless network, you should see the IP address for your board appear on the REPL shell.

To turn our LED on, you can open up a web browser and go to http://X.X.X.X/light/on to turn the LED on, and http://X.X.X.X/light/off to turn the LED off again.

##  **NOTE**

You should substitute your IP address, which for most home networks will probably be in the 192.168.1.X range.

3.10. Wireless Support

**30**
