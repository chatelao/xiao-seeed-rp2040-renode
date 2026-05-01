Raspberry Pi Pico-series Python SDK

## **Appendix A: App Notes**

## **Using a SSD1306-based OLED graphics display**

Display an image and text on I2C driven SSD1306-based OLED graphics display.

## **Wiring information**

See Figure 9 for wiring instructions.

_Figure 9. Wiring the OLED to Pico using I2C_

**==> picture [319 x 234] intentionally omitted <==**

## **List of Files**

A list of files with descriptions of their function;

## **i2c_1306oled_using_defaults.py**

The example code.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/1306oled/i2c_1306oled_using_defaults.py_

1 _# Display Image & text on I2C driven ssd1306 OLED display_ 2 from machine import Pin, I2C 3 from ssd1306 import SSD1306_I2C 4 import framebuf 5 6 WIDTH  = 128 _# oled display width_ 7 HEIGHT = 32 _# oled display height_ 8 9 i2c = I2C(0) _# Init I2C using I2C0 defaults, SCL=Pin(GP9), SDA=Pin(GP8), freq=400000_ 10 print("I2C Address      : "+hex(i2c.scan()[0]).upper()) _# Display device address_ 11 print("I2C Configuration: "+str(i2c)) _# Display I2C config_ 12

Using a SSD1306-based OLED graphics display

**37**

Raspberry Pi Pico-series Python SDK

13 14 oled = SSD1306_I2C(WIDTH, HEIGHT, i2c) _# Init oled display_ 15 16 _# Raspberry Pi logo as 32x32 bytearray_ 17 buffer = bytearray(b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00|?\x00\x01\x86 @\x80\x01\x01\x80\x80\x01\x11\x88\x80\x01\x05\xa0\x80\x00\x83\xc1\x00\x00C\xe3\x00\x00 ~\xfc\x00\x00L'\x00\x00\x9c\x11\x00\x00\xbf\xfd\x00\x00\xe1\x87\x00\x01\xc1\x83\x80\x02A\x82 @\x02A\x82@\x02\xc1\xc2@\x02\xf6>\xc0\x01\xfc=\x80\x01\x18\x18\x80\x01\x88\x10\x80\x00\x8c !\x00\x00\x87\xf1\x00\x00\x7f\xf6\x00\x008\x1c\x00\x00\x0c \x00\x00\x03\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00") 18 19 _# Load the raspberry pi logo into the framebuffer (the image is 32x32)_ 20 fb = framebuf.FrameBuffer(buffer, 32, 32, framebuf.MONO_HLSB) 21 22 _# Clear the oled display in case it has junk on it._ 23 oled.fill(0) 24 25 _# Blit the image from the framebuffer to the oled display_ 26 oled.blit(fb, 96, 0) 27 28 _# Add some text_ 29 oled.text("Raspberry Pi",5,5) 30 oled.text("Pico",5,15) 31 32 _# Finally update the oled display so the image & text is displayed_ 33 oled.show()

## **i2c_1306oled_with_freq.py**

The example code, explicitly sets a frequency.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/1306oled/i2c_1306oled_with_freq.py_

1 _# Display Image & text on I2C driven ssd1306 OLED display_ 2 from machine import Pin, I2C 3 from ssd1306 import SSD1306_I2C 4 import framebuf 5 6 WIDTH  = 128 _# oled display width_ 7 HEIGHT = 32 _# oled display height_ 8 9 i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=200000) _# Init I2C using pins GP8 & GP9 (default I2C0 pins)_ 10 print("I2C Address      : "+hex(i2c.scan()[0]).upper()) _# Display device address_ 11 print("I2C Configuration: "+str(i2c)) _# Display I2C config_ 12 13 14 oled = SSD1306_I2C(WIDTH, HEIGHT, i2c) _# Init oled display_ 15 16 _# Raspberry Pi logo as 32x32 bytearray_ 17 buffer = bytearray(b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00|?\x00\x01\x86 @\x80\x01\x01\x80\x80\x01\x11\x88\x80\x01\x05\xa0\x80\x00\x83\xc1\x00\x00C\xe3\x00\x00 ~\xfc\x00\x00L'\x00\x00\x9c\x11\x00\x00\xbf\xfd\x00\x00\xe1\x87\x00\x01\xc1\x83\x80\x02A\x82 @\x02A\x82@\x02\xc1\xc2@\x02\xf6>\xc0\x01\xfc=\x80\x01\x18\x18\x80\x01\x88\x10\x80\x00\x8c !\x00\x00\x87\xf1\x00\x00\x7f\xf6\x00\x008\x1c\x00\x00\x0c \x00\x00\x03\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00") 18 19 _# Load the raspberry pi logo into the framebuffer (the image is 32x32)_ 20 fb = framebuf.FrameBuffer(buffer, 32, 32, framebuf.MONO_HLSB) 21 22 _# Clear the oled display in case it has junk on it._ 23 oled.fill(0) 24

Using a SSD1306-based OLED graphics display

**38**

Raspberry Pi Pico-series Python SDK

25 _# Blit the image from the framebuffer to the oled display_ 26 oled.blit(fb, 96, 0) 27 28 _# Add some text_ 29 oled.text("Raspberry Pi",5,5) 30 oled.text("Pico",5,15) 31 32 _# Finally update the oled display so the image & text is displayed_ 33 oled.show()

## **Bill of Materials**

_Table 4. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|Monochrome 128x32 I2C OLED<br>Display|1|https://www.adafruit.com/product/<br>931|



## **Using a SH1106-based OLED graphics display**

Display an image and text on I2C driven SH1106-based OLED graphics display such as the Pimoroni Breakout Garden 1.12" Mono OLED https://shop.pimoroni.com/products/1-12-oled-breakout?variant=29421050757203 .

## **Wiring information**

See Figure 10 for wiring instructions.

_Figure 10. Wiring the OLED to Pico using I2C_

**==> picture [319 x 234] intentionally omitted <==**

Using a SH1106-based OLED graphics display

**39**

Raspberry Pi Pico-series Python SDK

## **List of Files**

A list of files with descriptions of their function;

## **i2c_1106oled_using_defaults.py**

The example code.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/1106oled/i2c_1106oled_using_defaults.py_

1 _# Display Image & text on I2C driven SH1106 OLED display_ 2 from machine import I2C, ADC 3 from sh1106 import SH1106_I2C 4 import framebuf 5 6 WIDTH  = 128 _# oled display width_ 7 HEIGHT = 128 _# oled display height_ 8 9 i2c = I2C(0) _# Init I2C using I2C0 defaults, SCL=Pin(GP9), SDA=Pin(GP8), freq=400000_ 10 print("I2C Address      : "+hex(i2c.scan()[0]).upper()) _# Display device address_ 11 print("I2C Configuration: "+str(i2c)) _# Display I2C config_ 12 13 14 oled = SH1106_I2C(WIDTH, HEIGHT, i2c) _# Init oled display_ 15 16 _# Raspberry Pi logo as 32x32 bytearray_ 17 buffer = bytearray(b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00|?\x00\x01\x86 @\x80\x01\x01\x80\x80\x01\x11\x88\x80\x01\x05\xa0\x80\x00\x83\xc1\x00\x00C\xe3\x00\x00 ~\xfc\x00\x00L'\x00\x00\x9c\x11\x00\x00\xbf\xfd\x00\x00\xe1\x87\x00\x01\xc1\x83\x80\x02A\x82 @\x02A\x82@\x02\xc1\xc2@\x02\xf6>\xc0\x01\xfc=\x80\x01\x18\x18\x80\x01\x88\x10\x80\x00\x8c !\x00\x00\x87\xf1\x00\x00\x7f\xf6\x00\x008\x1c\x00\x00\x0c \x00\x00\x03\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00") 18 19 _# Load the raspberry pi logo into the framebuffer (the image is 32x32)_ 20 fb = framebuf.FrameBuffer(buffer, 32, 32, framebuf.MONO_HLSB) 21 22 _# Clear the oled display in case it has junk on it._ 23 oled.fill(0) 24 25 _# Blit the image from the framebuffer to the oled display_ 26 oled.blit(fb, 96, 0) 27 28 _# Add some text_ 29 oled.text("Raspberry Pi",5,5) 30 oled.text("Pico",5,15) 31 32 _# Finally update the oled display so the image & text is displayed_ 33 oled.show()

## **i2c_1106oled_with_freq.py**

The example code, explicitly sets a frequency.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/1106oled/i2c_1106oled_with_freq.py_

1 _# Display Image & text on I2C driven ssd1306 OLED display_ 2 from machine import Pin, I2C 3 from sh1106 import SH1106_I2C 4 import framebuf 5 6 WIDTH  = 128 _# oled display width_ 7 HEIGHT = 32 _# oled display height_

Using a SH1106-based OLED graphics display

**40**

Raspberry Pi Pico-series Python SDK

8 9 i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=200000) _# Init I2C using pins GP8 & GP9 (default I2C0 pins)_ 10 print("I2C Address      : "+hex(i2c.scan()[0]).upper()) _# Display device address_ 11 print("I2C Configuration: "+str(i2c)) _# Display I2C config_ 12 13 14 oled = SH1106_I2C(WIDTH, HEIGHT, i2c) _# Init oled display_ 15 16 _# Raspberry Pi logo as 32x32 bytearray_ 17 buffer = bytearray(b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00|?\x00\x01\x86 @\x80\x01\x01\x80\x80\x01\x11\x88\x80\x01\x05\xa0\x80\x00\x83\xc1\x00\x00C\xe3\x00\x00 ~\xfc\x00\x00L'\x00\x00\x9c\x11\x00\x00\xbf\xfd\x00\x00\xe1\x87\x00\x01\xc1\x83\x80\x02A\x82 @\x02A\x82@\x02\xc1\xc2@\x02\xf6>\xc0\x01\xfc=\x80\x01\x18\x18\x80\x01\x88\x10\x80\x00\x8c !\x00\x00\x87\xf1\x00\x00\x7f\xf6\x00\x008\x1c\x00\x00\x0c \x00\x00\x03\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00") 18 19 _# Load the raspberry pi logo into the framebuffer (the image is 32x32)_ 20 fb = framebuf.FrameBuffer(buffer, 32, 32, framebuf.MONO_HLSB) 21 22 _# Clear the oled display in case it has junk on it._ 23 oled.fill(0) 24 25 _# Blit the image from the framebuffer to the oled display_ 26 oled.blit(fb, 96, 0) 27 28 _# Add some text_ 29 oled.text("Raspberry Pi",5,5) 30 oled.text("Pico",5,15) 31 32 _# Finally update the oled display so the image & text is displayed_ 33 oled.show()

## **sh1106.py**

SH1106 Driver Obtained from https://github.com/robert-hh/SH1106

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/i2c/1106oled/sh1106.py_

1 _#_ 2 _# MicroPython SH1106 OLED driver, I2C and SPI interfaces_ 3 _#_ 4 _# The MIT License (MIT)_ 5 _#_ 6 _# Copyright (c) 2016 Radomir Dopieralski (@deshipu),_ 7 _#               2017 Robert Hammelrath (@robert-hh)_ 8 _#_ 9 _# Permission is hereby granted, free of charge, to any person obtaining a copy_ 10 _# of this software and associated documentation files (the "Software"), to deal_ 11 _# in the Software without restriction, including without limitation the rights_ 12 _# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell_ 13 _# copies of the Software, and to permit persons to whom the Software is_ 14 _# furnished to do so, subject to the following conditions:_ 15 _#_ 16 _# The above copyright notice and this permission notice shall be included in_ 17 _# all copies or substantial portions of the Software._ 18 _#_ 19 _# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR_ 20 _# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,_ 21 _# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE_ 22 _# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER_ 23 _# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,_ 24 _# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN_

Using a SH1106-based OLED graphics display

**41**

Raspberry Pi Pico-series Python SDK

25 _# THE SOFTWARE._ 26 _#_ 27 _# Sample code sections_ 28 _# ------------ SPI ------------------_ 29 _# Pin Map SPI_ 30 _#   - 3V3      - Vcc_ 31 _#   - GND      - Gnd_ 32 _#   - GPIO 11  - DIN / MOSI fixed_ 33 _#   - GPIO 10  - CLK / Sck fixed_ 34 _#   - GPIO 4   - CS (optional, if the only connected device, connect to GND)_ 35 _#   - GPIO 5   - D/C_ 36 _#   - GPIO 2   - Res_ 37 _#_ 38 _# for CS, D/C and Res other ports may be chosen._ 39 _#_ 40 _# from machine import Pin, SPI_ 41 _# import sh1106_ 42 43 _# spi = SPI(1, baudrate=1000000)_ 44 _# display = sh1106.SH1106_SPI(128, 64, spi, Pin(5), Pin(2), Pin(4))_ 45 _# display.sleep(False)_ 46 _# display.fill(0)_ 47 _# display.text('Testing 1', 0, 0, 1)_ 48 _# display.show()_ 49 _#_ 50 _# --------------- I2C ------------------_ 51 _#_ 52 _# Pin Map I2C_ 53 _#   - 3V3      - Vcc_ 54 _#   - GND      - Gnd_ 55 _#   - GPIO 5   - CLK / SCL_ 56 _#   - GPIO 4   - DIN / SDA_ 57 _#   - GPIO 2   - Res_ 58 _#   - GND      - CS_ 59 _#   - GND      - D/C_ 60 _#_ 61 _#_ 62 _# from machine import Pin, I2C_ 63 _# import sh1106_ 64 _#_ 65 _# i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)_ 66 _# display = sh1106.SH1106_I2C(128, 64, i2c, Pin(2), 0x3c)_ 67 _# display.sleep(False)_ 68 _# display.fill(0)_ 69 _# display.text('Testing 1', 0, 0, 1)_ 70 _# display.show()_ 71 72 from micropython import const 73 import utime as time 74 import framebuf 75 76 77 _# a few register definitions_ 78 _SET_CONTRAST        = const(0x81) 79 _SET_NORM_INV        = const(0xa6) 80 _SET_DISP            = const(0xae) 81 _SET_SCAN_DIR        = const(0xc0) 82 _SET_SEG_REMAP       = const(0xa0) 83 _LOW_COLUMN_ADDRESS  = const(0x00) 84 _HIGH_COLUMN_ADDRESS = const(0x10) 85 _SET_PAGE_ADDRESS    = const(0xB0) 86 87 88 class SH1106:

Using a SH1106-based OLED graphics display

**42**

Raspberry Pi Pico-series Python SDK

89     def __init__(self, width, height, external_vcc): 90         self.width = width 91         self.height = height 92         self.external_vcc = external_vcc 93         self.pages = self.height // 8 94         self.buffer = bytearray(self.pages * self.width) 95         fb = framebuf.FrameBuffer(self.buffer, self.width, self.height, 96                                   framebuf.MVLSB) 97         self.framebuf = fb 98 _# set shortcuts for the methods of framebuf_ 99         self.fill = fb.fill 100         self.fill_rect = fb.fill_rect 101         self.hline = fb.hline 102         self.vline = fb.vline 103         self.line = fb.line 104         self.rect = fb.rect 105         self.pixel = fb.pixel 106         self.scroll = fb.scroll 107         self.text = fb.text 108         self.blit = fb.blit 109 110         self.init_display() 111 112     def init_display(self): 113         self.reset() 114         self.fill(0) 115         self.poweron() 116         self.show() 117 118     def poweroff(self): 119         self.write_cmd(_SET_DISP | 0x00) 120 121     def poweron(self): 122         self.write_cmd(_SET_DISP | 0x01) 123 124     def rotate(self, flag, update=True): 125         if flag: 126             self.write_cmd(_SET_SEG_REMAP | 0x01) _# mirror display vertically_ 127             self.write_cmd(_SET_SCAN_DIR | 0x08) _# mirror display hor._ 128         else: 129             self.write_cmd(_SET_SEG_REMAP | 0x00) 130             self.write_cmd(_SET_SCAN_DIR | 0x00) 131         if update: 132             self.show() 133 134     def sleep(self, value): 135         self.write_cmd(_SET_DISP | (not value)) 136 137     def contrast(self, contrast): 138         self.write_cmd(_SET_CONTRAST) 139         self.write_cmd(contrast) 140 141     def invert(self, invert): 142         self.write_cmd(_SET_NORM_INV | (invert & 1)) 143 144     def show(self): 145         for page in range(self.height // 8): 146             self.write_cmd(_SET_PAGE_ADDRESS | page) 147             self.write_cmd(_LOW_COLUMN_ADDRESS | 2) 148             self.write_cmd(_HIGH_COLUMN_ADDRESS | 0) 149             self.write_data(self.buffer[ 150                 self.width * page:self.width * page + self.width 151             ]) 152

Using a SH1106-based OLED graphics display

**43**

Raspberry Pi Pico-series Python SDK

153     def reset(self, res): 154         if res is not None: 155             res(1) 156             time.sleep_ms(1) 157             res(0) 158             time.sleep_ms(20) 159             res(1) 160             time.sleep_ms(20) 161 162 163 class SH1106_I2C(SH1106): 164     def __init__(self, width, height, i2c, res=None, addr=0x3c, 165                  external_vcc=False): 166         self.i2c = i2c 167         self.addr = addr 168         self.res = res 169         self.temp = bytearray(2) 170         if res is not None: 171             res.init(res.OUT, value=1) 172         super().__init__(width, height, external_vcc) 173 174     def write_cmd(self, cmd): 175         self.temp[0] = 0x80 _# Co=1, D/C#=0_ 176         self.temp[1] = cmd 177         self.i2c.writeto(self.addr, self.temp) 178 179     def write_data(self, buf): 180         self.i2c.writeto(self.addr, b'\x40'+buf) 181 182     def reset(self): 183         super().reset(self.res) 184 185 186 class SH1106_SPI(SH1106): 187     def __init__(self, width, height, spi, dc, res=None, cs=None, 188                  external_vcc=False): 189         self.rate = 10 * 1000 * 1000 190         dc.init(dc.OUT, value=0) 191         if res is not None: 192             res.init(res.OUT, value=0) 193         if cs is not None: 194             cs.init(cs.OUT, value=1) 195         self.spi = spi 196         self.dc = dc 197         self.res = res 198         self.cs = cs 199         super().__init__(width, height, external_vcc) 200 201     def write_cmd(self, cmd): 202         self.spi.init(baudrate=self.rate, polarity=0, phase=0) 203         if self.cs is not None: 204             self.cs(1) 205             self.dc(0) 206             self.cs(0) 207             self.spi.write(bytearray([cmd])) 208             self.cs(1) 209         else: 210             self.dc(0) 211             self.spi.write(bytearray([cmd])) 212 213     def write_data(self, buf): 214         self.spi.init(baudrate=self.rate, polarity=0, phase=0) 215         if self.cs is not None: 216             self.cs(1)

Using a SH1106-based OLED graphics display

**44**

Raspberry Pi Pico-series Python SDK

217             self.dc(1) 218             self.cs(0) 219             self.spi.write(buf) 220             self.cs(1) 221         else: 222             self.dc(1) 223             self.spi.write(buf) 224 225     def reset(self): 226         super().reset(self.res)

## **Bill of Materials**

_Table 5. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|Monochrome 128x128 I2C OLED<br>Display|1|https://shop.pimoroni.com/products/<br>1-12-oled-breakout?<br>variant=29421050757203|



## **Using PIO to drive a set of NeoPixel Ring (WS2812 LEDs)**

Combination of the PIO WS2812 demo with the Adafruit 'essential' NeoPixel example code to show off color fills, chases and of course a rainbow swirl on a 16-LED ring.

## **Wiring information**

See Figure 11 for wiring instructions.

_Figure 11. Wiring the 16-LED NeoPixel Ring to Pico_

**==> picture [319 x 166] intentionally omitted <==**

## **List of Files**

A list of files with descriptions of their function;

Using PIO to drive a set of NeoPixel Ring (WS2812 LEDs)

**45**

Raspberry Pi Pico-series Python SDK

## **neopixel_ring.py**

The example code.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/pio/neopixel_ring/neopixel_ring.py_

1 _# Example using PIO to drive a set of WS2812 LEDs._ 2 3 import array, time 4 from machine import Pin 5 import rp2 6 7 _# Configure the number of WS2812 LEDs._ 8 NUM_LEDS = 16 9 PIN_NUM = 6 10 brightness = 0.2 11 12 @rp2.asm_pio(sideset_init=rp2.PIO.OUT_LOW, out_shiftdir=rp2.PIO.SHIFT_LEFT, autopull=True, pull_thresh=24) 13 def ws2812(): 14     T1 = 2 15     T2 = 5 16     T3 = 3 17     wrap_target() 18     label("bitloop") 19     out(x, 1)               .side(0)    [T3 - 1] 20     jmp(not_x, "do_zero")   .side(1)    [T1 - 1] 21     jmp("bitloop")          .side(1)    [T2 - 1] 22     label("do_zero") 23     nop()                   .side(0)    [T2 - 1] 24     wrap() 25 26 27 _# Create the StateMachine with the ws2812 program, outputting on pin_ 28 sm = rp2.StateMachine(0, ws2812, freq=8_000_000, sideset_base=Pin(PIN_NUM)) 29 30 _# Start the StateMachine, it will wait for data on its FIFO._ 31 sm.active(1) 32 33 _# Display a pattern on the LEDs via an array of LED RGB values._ 34 ar = array.array("I", [0 for _ in range(NUM_LEDS)]) 35 36 _##########################################################################_ 37 def pixels_show(): 38     dimmer_ar = array.array("I", [0 for _ in range(NUM_LEDS)]) 39     for i,c in enumerate(ar): 40         r = int(((c >> 8) & 0xFF) * brightness) 41         g = int(((c >> 16) & 0xFF) * brightness) 42         b = int((c & 0xFF) * brightness) 43         dimmer_ar[i] = (g<<16) + (r<<8) + b 44     sm.put(dimmer_ar, 8) 45     time.sleep_ms(10) 46 47 def pixels_set(i, color): 48     ar[i] = (color[1]<<16) + (color[0]<<8) + color[2] 49 50 def pixels_fill(color): 51     for i in range(len(ar)): 52         pixels_set(i, color) 53 54 def color_chase(color, wait): 55     for i in range(NUM_LEDS): 56         pixels_set(i, color) 57         time.sleep(wait)

Using PIO to drive a set of NeoPixel Ring (WS2812 LEDs)

**46**

Raspberry Pi Pico-series Python SDK

58         pixels_show() 59     time.sleep(0.2) 60 61 def wheel(pos): 62 _# Input a value 0 to 255 to get a color value._ 63 _# The colours are a transition r - g - b - back to r._ 64     if pos < 0 or pos > 255: 65         return (0, 0, 0) 66     if pos < 85: 67         return (255 - pos * 3, pos * 3, 0) 68     if pos < 170: 69         pos -= 85 70         return (0, 255 - pos * 3, pos * 3) 71     pos -= 170 72     return (pos * 3, 0, 255 - pos * 3) 73 74 def rainbow_cycle(wait): 75     for j in range(255): 76         for i in range(NUM_LEDS): 77             rc_index = (i * 256 // NUM_LEDS) + j 78             pixels_set(i, wheel(rc_index & 255)) 79         pixels_show() 80         time.sleep(wait) 81 82 BLACK = (0, 0, 0) 83 RED = (255, 0, 0) 84 YELLOW = (255, 150, 0) 85 GREEN = (0, 255, 0) 86 CYAN = (0, 255, 255) 87 BLUE = (0, 0, 255) 88 PURPLE = (180, 0, 255) 89 WHITE = (255, 255, 255) 90 COLORS = (BLACK, RED, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE) 91 92 print("fills") 93 for color in COLORS: 94     pixels_fill(color) 95     pixels_show() 96     time.sleep(0.2) 97 98 print("chases") 99 for color in COLORS: 100     color_chase(color, 0.01) 101 102 print("rainbow") 103 rainbow_cycle(0)

## **Bill of Materials**

_Table 6. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|NeoPixel Ring|1|https://www.adafruit.com/product/<br>1463|



Using PIO to drive a set of NeoPixel Ring (WS2812 LEDs)

**47**

Raspberry Pi Pico-series Python SDK

## **Using UART on the Raspberry Pi Pico**

Send data from the UART1 port to the UART0 port. Other things to try;

uart0 = UART(0)

which will open a UART connection at the default baudrate of 115200, and

uart0.readline()

which will read until the CR (\r) and NL (\n) characters, then return the line.

## **Wiring information**

See Figure 12 for wiring instructions.

_Figure 12. Wiring two of the Pico’s ports together. Be sure to wire UART0 TX to UART1 RX and UART0 RX to UART1 TX._

**==> picture [319 x 222] intentionally omitted <==**

## **List of Files**

A list of files with descriptions of their function;

## **uart.py**

The example code.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/uart/loopback/uart.py_

1 from machine import UART, Pin 2 import time 3 4 uart1 = UART(1, baudrate=9600, tx=Pin(8), rx=Pin(9)) 5 6 uart0 = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1)) 7 8 txData = b'hello world\n\r'

Using UART on the Raspberry Pi Pico

**48**

Raspberry Pi Pico-series Python SDK

9 uart1.write(txData) 10 time.sleep(0.1) 11 rxData = bytes() 12 while uart0.any() > 0: 13     rxData += uart0.read(1) 14 15 print(rxData.decode('utf-8'))

## **Bill of Materials**

_Table 7. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|



Using UART on the Raspberry Pi Pico

**49**
