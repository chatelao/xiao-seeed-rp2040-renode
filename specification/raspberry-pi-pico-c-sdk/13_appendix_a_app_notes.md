Raspberry Pi Pico-series C/C++ SDK

## **Appendix A: App Notes**

## **Attaching a 7 segment LED via GPIO**

This example code shows how to interface the Raspberry Pi Pico to a generic 7 segment LED device. It uses the LED to count from 0 to 9 and then repeat. If the button is pressed, then the numbers will count down instead of up.

## **Wiring information**

Our 7 Segment display has pins as follows.

**==> picture [425 x 77] intentionally omitted <==**

**----- Start of picture text -----**<br>
  --A--<br>  F   B<br>  --G--<br>  E   C<br>  --D--<br>**----- End of picture text -----**<br>


By default we are allocating GPIO 2 to segment A, 3 to B etc. So, connect GPIO 2 to pin A on the 7 segment LED display and so on. You will need the appropriate resistors (68 ohm should be fine) for each segment. The LED device used here is common anode, so the anode pin is connected to the 3.3v supply, and the GPIOs need to pull low (to ground) to complete the circuit. The pull direction of the GPIOs is specified in the code itself.

Connect the switch to connect on pressing. One side should be connected to ground, the other to GPIO 9.

_Figure 9. Wiring Diagram for 7 segment LED._

**==> picture [319 x 133] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/gpio/hello_7segment/CMakeLists.txt_

1 add_executable(hello_7segment 2         hello_7segment.c 3         ) 4 5 # pull in common dependencies 6 target_link_libraries(hello_7segment pico_stdlib) 7

Attaching a 7 segment LED via GPIO

**592**

Raspberry Pi Pico-series C/C++ SDK

8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(hello_7segment) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(hello_7segment)

## **hello_7segment.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/gpio/hello_7segment/hello_7segment.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include "pico/stdlib.h"_ 9 _#include "hardware/gpio.h"_ 10 11 _/*_ 12 _Our 7 Segment display has pins as follows:_ 13 14 _--A--_ 15 _F   B_ 16 _--G--_ 17 _E   C_ 18 _--D--_ 19 20 _By default we are allocating GPIO 2 to segment A, 3 to B etc._ 21 _So, connect GPIO 2 to pin A on the 7 segment LED display etc. Don't forget_ 22 _the appropriate resistors, best to use one for each segment!_ 23 24 _Connect button so that pressing the switch connects the GPIO 9 (default) to_ 25 _ground (pull down)_ 26 _*/_ 27 28 _#define FIRST_GPIO 2_ 29 _#define BUTTON_GPIO (FIRST_GPIO+7)_ 30 31 _// This array converts a number 0-9 to a bit pattern to send to the GPIOs_ 32 int bits[10] = { 33         0x3f, _// 0_ 34         0x06, _// 1_ 35         0x5b, _// 2_ 36         0x4f, _// 3_ 37         0x66, _// 4_ 38         0x6d, _// 5_ 39         0x7d, _// 6_ 40         0x07, _// 7_ 41         0x7f, _// 8_ 42         0x67 _// 9_ 43 }; 44 45 _/// \tag::hello_gpio[]_ 46 int main() { 47     stdio_init_all(); 48     printf("Hello, 7segment - press button to count down!\n"); 49 50 _// We could use gpio_set_dir_out_masked() here_ 51     for (int gpio = FIRST_GPIO; gpio < FIRST_GPIO + 7; gpio++) {

Attaching a 7 segment LED via GPIO

**593**

Raspberry Pi Pico-series C/C++ SDK

52         gpio_init(gpio); 53         gpio_set_dir(gpio, GPIO_OUT); 54 _// Our bitmap above has a bit set where we need an LED on, BUT, we are pulling low to light_ 55 _// so invert our output_ 56         gpio_set_outover(gpio, GPIO_OVERRIDE_INVERT); 57     } 58 59     gpio_init(BUTTON_GPIO); 60     gpio_set_dir(BUTTON_GPIO, GPIO_IN); 61 _// We are using the button to pull down to 0v when pressed, so ensure that when_ 62 _// unpressed, it uses internal pull ups. Otherwise when unpressed, the input will_ 63 _// be floating._ 64     gpio_pull_up(BUTTON_GPIO); 65 66     int val = 0; 67     while (true) { 68 _// Count upwards or downwards depending on button input_ 69 _// We are pulling down on switch active, so invert the get to make_ 70 _// a press count downwards_ 71         if (!gpio_get(BUTTON_GPIO)) { 72             if (val == 9) { 73                 val = 0; 74             } else { 75                 val++; 76             } 77         } else if (val == 0) { 78             val = 9; 79         } else { 80             val--; 81         } 82 83 _// We are starting with GPIO 2, our bitmap starts at bit 0 so shift to start at 2._ 84         int32_t mask = bits[val] << FIRST_GPIO; 85 86 _// Set all our GPIOs in one go!_ 87 _// If something else is using GPIO, we might want to use gpio_put_masked()_ 88         gpio_set_mask(mask); 89         sleep_ms(250); 90         gpio_clr_mask(mask); 91     } 92 } 93 _/// \end::hello_gpio[]_

## **Bill of Materials**

|_Table 38. A list of_<br>_materials required for_<br>_the example_|**Item**|**Quantity**|Details|
|---|---|---|---|
||Breadboard|1|generic part|
||Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
||7 segment LED module|1|generic part|
||68 ohm resistor|7|generic part|
||DIL push to make switch|1|generic switch|
||M/M Jumper wires|10|generic part|



Attaching a 7 segment LED via GPIO

**594**

Raspberry Pi Pico-series C/C++ SDK

## **DHT-11, DHT-22, and AM2302 Sensors**

The DHT sensors are fairly well known hobbyist sensors for measuring relative humidity and temperature using a capacitive humidity sensor, and a thermistor. While they are slow, one reading every ~2 seconds, they are reliable and good for basic data logging. Communication is based on a custom protocol which uses a single wire for data.

##  **NOTE**

The DHT-11 and DHT-22 sensors are the most common. They use the same protocol but have different characteristics, the DHT-22 has better accuracy, and has a larger sensor range than the DHT-11. The sensor is available from a number of retailers.

## **Wiring information**

See Figure 10 for wiring instructions.

_Figure 10. Wiring the DHT-22 temperature sensor to Raspberry Pi Pico, and connecting Pico’s UART0 to the Raspberry Pi 4._

**==> picture [319 x 220] intentionally omitted <==**

##  **NOTE**

One of the pins (pin 3) on the DHT sensor will not be connected, it is not used.

You will want to place a 10 kΩ resistor between VCC and the data pin, to act as a medium-strength pull up on the data line.

Connecting UART0 of Pico to Raspberry Pi as in Figure 10 and you should see something similar to Figure 11 in minicom when connected to /dev/serial0 on the Raspberry Pi.

DHT-11, DHT-22, and AM2302 Sensors

**595**

Raspberry Pi Pico-series C/C++ SDK

_Figure 11. Serial output over Pico’s UART0 in a terminal window._

**==> picture [319 x 240] intentionally omitted <==**

Connect to /dev/serial0 by typing,

$ minicom -b 115200 -o -D /dev/serial0

at the command line.

## **List of Files**

A list of files with descriptions of their function;

## **CMakeLists.txt**

Make file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/gpio/dht_sensor/CMakeLists.txt_

1 add_executable(dht 2         dht.c 3         ) 4 5 target_link_libraries(dht pico_stdlib) 6 7 pico_add_extra_outputs(dht) 8 9 # add url via pico_set_program_url 10 example_auto_set_url(dht)

## **dht.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/gpio/dht_sensor/dht.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_

DHT-11, DHT-22, and AM2302 Sensors

**596**

Raspberry Pi Pico-series C/C++ SDK

4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _**/_ 6 7 _#include <stdio.h>_ 8 _#include <math.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "hardware/gpio.h"_ 11 12 _#ifdef PICO_DEFAULT_LED_PIN_ 13 _#define LED_PIN PICO_DEFAULT_LED_PIN_ 14 _#endif_ 15 16 const uint DHT_PIN = 15; 17 const uint MAX_TIMINGS = 85; 18 19 typedef struct { 20     float humidity; 21     float temp_celsius; 22 } dht_reading; 23 24 void read_from_dht(dht_reading *result); 25 26 int main() { 27     stdio_init_all(); 28     gpio_init(DHT_PIN); 29 _#ifdef LED_PIN_ 30     gpio_init(LED_PIN); 31     gpio_set_dir(LED_PIN, GPIO_OUT); 32 _#endif_ 33     while (1) { 34         dht_reading reading; 35         read_from_dht(&reading); 36         float fahrenheit = (reading.temp_celsius * 9 / 5) + 32; 37         printf("Humidity = %.1f%%, Temperature = %.1fC (%.1fF)\n", 38                reading.humidity, reading.temp_celsius, fahrenheit); 39 40         sleep_ms(2000); 41     } 42 } 43 44 void read_from_dht(dht_reading *result) { 45     int data[5] = {0, 0, 0, 0, 0}; 46     uint last = 1; 47     uint j = 0; 48 49     gpio_set_dir(DHT_PIN, GPIO_OUT); 50     gpio_put(DHT_PIN, 0); 51     sleep_ms(20); 52     gpio_set_dir(DHT_PIN, GPIO_IN); 53 54 _#ifdef LED_PIN_ 55     gpio_put(LED_PIN, 1); 56 _#endif_ 57     for (uint i = 0; i < MAX_TIMINGS; i++) { 58         uint count = 0; 59         while (gpio_get(DHT_PIN) == last) { 60             count++; 61             sleep_us(1); 62             if (count == 255) break; 63         } 64         last = gpio_get(DHT_PIN); 65         if (count == 255) break; 66 67         if ((i >= 4) && (i % 2 == 0)) {

DHT-11, DHT-22, and AM2302 Sensors

**597**

Raspberry Pi Pico-series C/C++ SDK

68             data[j / 8] <<= 1; 69             if (count > 16) data[j / 8] |= 1; 70             j++; 71         } 72     } 73 _#ifdef LED_PIN_ 74     gpio_put(LED_PIN, 0); 75 _#endif_ 76 77     if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) { 78         result->humidity = (float) ((data[0] << 8) + data[1]) / 10; 79         if (result->humidity > 100) { 80             result->humidity = data[0]; 81         } 82         result->temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10; 83         if (result->temp_celsius > 125) { 84             result->temp_celsius = data[2]; 85         } 86         if (data[2] & 0x80) { 87             result->temp_celsius = -result->temp_celsius; 88         } 89     } else { 90         printf("Bad data\n"); 91     } 92 }

## **Bill of Materials**

_Table 39. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|10 kΩ resistor|1|generic part|
|M/M Jumper wires|4|generic part|
|DHT-22 sensor|1|generic part|



## **Attaching a 16x2 LCD via TTL**

This example code shows how to interface the Raspberry Pi Pico to one of the very common 16x2 LCD character displays. Due to the large number of pins these displays use, they are commonly used with extra drivers or backpacks. In this example, we will use an Adafruit LCD display backpack, which supports communication over USB or TTL. A monochrome display with an RGB backlight is also used, but the backpack is compatible with monochrome backlight displays too. There is another example that uses I2C to control a 16x2 display.

The backpack processes a set of commands that are documented here and preceded by the "special" byte 0xFE. The backpack does the ASCII character conversion and even supports custom character creation. In this example, we use the Pico’s primary UART (uart0) to read characters from our computer and send them via the other UART (uart1) to print them onto the LCD. We also define a special startup sequence and vary the display’s backlight color.

Attaching a 16x2 LCD via TTL

**598**

Raspberry Pi Pico-series C/C++ SDK

##  **NOTE**

You can change where stdio output goes (Pico’s USB, uart0 or both) with CMake directives. The CMakeLists.txt file shows how to enable both.

## **Wiring information**

Wiring up the backpack to the Pico requires 3 jumpers, to connect VCC (3.3v), GND, TX. The example here uses both of the Pico’s UARTs, one (uart0) for stdio and the other (uart1) for communication with the backpack. Pin 8 is used as the TX pin. Power is supplied from the 3.3V pin. To connect the backpack to the display, it is common practice to solder it onto the back of the display, or during the prototyping stage to use the same parallel lanes on a breadboard.

##  **NOTE**

While this display will work at 3.3V, it will be quite dim. Using a 5V source will make it brighter.

_Figure 12. Wiring Diagram for LCD with TTL backpack._

**==> picture [319 x 181] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/uart/lcd_uart/CMakeLists.txt_

1 add_executable(lcd_uart 2         lcd_uart.c 3         ) 4 5 # pull in common dependencies and additional uart hardware support 6 target_link_libraries(lcd_uart pico_stdlib hardware_uart) 7 8 # enable usb output and uart output 9 # modify here as required 10 pico_enable_stdio_usb(lcd_uart 1) 11 pico_enable_stdio_uart(lcd_uart 1) 12 13 # create map/bin/hex file etc. 14 pico_add_extra_outputs(lcd_uart) 15 16 # add url via pico_set_program_url

Attaching a 16x2 LCD via TTL

**599**

Raspberry Pi Pico-series C/C++ SDK

## 17 example_auto_set_url(lcd_uart)

## **lcd_uart.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/uart/lcd_uart/lcd_uart.c_

1 _/**_ 2 _* Copyright (c) 2021 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _/* Example code to drive a 16x2 LCD panel via an Adafruit TTL LCD "backpack"_ 8 9 _Optionally, the backpack can be connected the VBUS (pin 40) at 5V if_ 10 _the Pico in question is powered by USB for greater brightness._ 11 12 _If this is done, then no other connections should be made to the backpack apart_ 13 _from those listed below as the backpack's logic levels will change._ 14 15 _Connections on Raspberry Pi Pico board, other boards may vary._ 16 17 _GPIO 8 (pin 11)-> RX on backpack_ 18 _3.3v (pin 36) -> 3.3v on backpack_ 19 _GND (pin 38)  -> GND on backpack_ 20 _*/_ 21 22 _#include <stdio.h>_ 23 _#include <math.h>_ 24 _#include "pico/stdlib.h"_ 25 _#include "pico/binary_info.h"_ 26 _#include "hardware/uart.h"_ 27 28 _// leave uart0 free for stdio_ 29 _#define UART_ID uart1_ 30 _#define BAUD_RATE 9600_ 31 _#define UART_TX_PIN 8_ 32 _#define LCD_WIDTH 16_ 33 _#define LCD_HEIGHT 2_ 34 35 _// basic commands_ 36 _#define LCD_DISPLAY_ON 0x42_ 37 _#define LCD_DISPLAY_OFF 0x46_ 38 _#define LCD_SET_BRIGHTNESS 0x99_ 39 _#define LCD_SET_CONTRAST 0x50_ 40 _#define LCD_AUTOSCROLL_ON 0x51_ 41 _#define LCD_AUTOSCROLL_OFF 0x52_ 42 _#define LCD_CLEAR_SCREEN 0x58_ 43 _#define LCD_SET_SPLASH 0x40_ 44 45 _// cursor commands_ 46 _#define LCD_SET_CURSOR_POS 0x47_ 47 _#define LCD_CURSOR_HOME 0x48_ 48 _#define LCD_CURSOR_BACK 0x4C_ 49 _#define LCD_CURSOR_FORWARD 0x4D_ 50 _#define LCD_UNDERLINE_CURSOR_ON 0x4A_ 51 _#define LCD_UNDERLINE_CURSOR_OFF 0x4B_ 52 _#define LCD_BLOCK_CURSOR_ON 0x53_ 53 _#define LCD_BLOCK_CURSOR_OFF 0x54_ 54 55 _// rgb commands_

Attaching a 16x2 LCD via TTL

**600**

Raspberry Pi Pico-series C/C++ SDK

56 _#define LCD_SET_BACKLIGHT_COLOR 0xD0_ 57 _#define LCD_SET_DISPLAY_SIZE 0xD1_ 58 59 _// change to 0 if display is not RGB capable_ 60 _#define LCD_IS_RGB 1_ 61 62 void lcd_write(uint8_t cmd, uint8_t* buf, uint8_t buflen) { 63 _// all commands are prefixed with 0xFE_ 64     const uint8_t pre = 0xFE; 65     uart_write_blocking(UART_ID, &pre, 1); 66     uart_write_blocking(UART_ID, &cmd, 1); 67     uart_write_blocking(UART_ID, buf, buflen); 68     sleep_ms(10); _// give the display some time_ 69 } 70 71 void lcd_set_size(uint8_t w, uint8_t h) { 72 _// sets the dimensions of the display_ 73     uint8_t buf[] = { w, h }; 74     lcd_write(LCD_SET_DISPLAY_SIZE, buf, 2); 75 } 76 77 void lcd_set_contrast(uint8_t contrast) { 78 _// sets the display contrast_ 79     lcd_write(LCD_SET_CONTRAST, &contrast, 1); 80 } 81 82 void lcd_set_brightness(uint8_t brightness) { 83 _// sets the backlight brightness_ 84     lcd_write(LCD_SET_BRIGHTNESS, &brightness, 1); 85 } 86 87 void lcd_set_cursor(bool is_on) { 88 _// set is_on to true if we want the blinking block and underline cursor to show_ 89     if (is_on) { 90         lcd_write(LCD_BLOCK_CURSOR_ON, NULL, 0); 91         lcd_write(LCD_UNDERLINE_CURSOR_ON, NULL, 0); 92     } else { 93         lcd_write(LCD_BLOCK_CURSOR_OFF, NULL, 0); 94         lcd_write(LCD_UNDERLINE_CURSOR_OFF, NULL, 0); 95     } 96 } 97 98 void lcd_set_backlight(bool is_on) { 99 _// turn the backlight on (true) or off (false)_ 100     if (is_on) { 101         lcd_write(LCD_DISPLAY_ON, (uint8_t *) 0, 1); 102     } else { 103         lcd_write(LCD_DISPLAY_OFF, NULL, 0); 104     } 105 } 106 107 void lcd_clear() { 108 _// clear the contents of the display_ 109     lcd_write(LCD_CLEAR_SCREEN, NULL, 0); 110 } 111 112 void lcd_cursor_reset() { 113 _// reset the cursor to (1, 1)_ 114     lcd_write(LCD_CURSOR_HOME, NULL, 0); 115 } 116 117 _#if LCD_IS_RGB_ 118 void lcd_set_backlight_color(uint8_t r, uint8_t g, uint8_t b) { 119 _// only supported on RGB displays!_

Attaching a 16x2 LCD via TTL

**601**

Raspberry Pi Pico-series C/C++ SDK

120     uint8_t buf[] = { r, g, b }; 121     lcd_write(LCD_SET_BACKLIGHT_COLOR, buf, 3); 122 } 123 _#endif_ 124 125 void lcd_init() { 126     lcd_set_backlight(true); 127     lcd_set_size(LCD_WIDTH, LCD_HEIGHT); 128     lcd_set_contrast(155); 129     lcd_set_brightness(255); 130     lcd_set_cursor(false); 131 } 132 133 int main() { 134     stdio_init_all(); 135     uart_init(UART_ID, BAUD_RATE); 136     uart_set_translate_crlf(UART_ID, false); 137     gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN)); 138 139     bi_decl(bi_1pin_with_func(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN))); 140 141     lcd_init(); 142 143 _// define startup sequence and save to EEPROM_ 144 _// no more or less than 32 chars, if not enough, fill remaining ones with spaces_ 145     uint8_t splash_buf[] = "Hello LCD, from Pi Towers!      "; 146     lcd_write(LCD_SET_SPLASH, splash_buf, LCD_WIDTH * LCD_HEIGHT); 147 148     lcd_cursor_reset(); 149     lcd_clear(); 150 151 _#if LCD_IS_RGB_ 152     uint8_t i = 0; _// it's ok if this overflows and wraps, we're using sin_ 153     const float frequency = 0.1f; 154     uint8_t red, green, blue; 155 _#endif_ 156 157     while (1) { 158 _// send any chars from stdio straight to the backpack_ 159         char c = getchar(); 160 _// any bytes not followed by 0xFE (the special command) are interpreted_ 161 _// as text to be displayed on the backpack, so we just send the char_ 162 _// down the UART byte pipe!_ 163         if (c < 128) uart_putc_raw(UART_ID, c); _// skip extra non-ASCII chars_ 164 _#if LCD_IS_RGB_ 165 _// change the display color on keypress, rainbow style!_ 166         red = (uint8_t)(sin(frequency * i + 0) * 127 + 128); 167         green = (uint8_t)(sin(frequency * i + 2) * 127 + 128); 168         blue = (uint8_t)(sin(frequency * i + 4) * 127 + 128); 169         lcd_set_backlight_color(red, green, blue); 170         i++; 171 _#endif_ 172     } 173 }

## **Bill of Materials**

_Table 40. A list of_ **Item Quantity** Details _materials required for the example_ Breadboard 1 generic part

Attaching a 16x2 LCD via TTL

**602**

Raspberry Pi Pico-series C/C++ SDK

|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|---|---|---|
|16x2 RGB LCD panel 3.3v|1|generic part,available on Adafruit|
|16x2 LCD backpack|1|from Adafruit|
|M/M Jumper wires|3|generic part|



## **Attaching a microphone using the ADC**

This example code shows how to interface the Raspberry Pi Pico with a standard analog microphone via the onboard analog to digital converter (ADC). In this example, we use an ICS-40180 breakout board by SparkFun but any analog microphone should be compatible with this tutorial. SparkFun have written a guide for this board that goes into more detail about the board and how it works.

##  **TIP**

An analog to digital converter (ADC) is responsible for reading continually varying input signals that may range from 0 to a specified reference voltage (in the Pico’s case this reference voltage is set by the supply voltage and can be measured on pin 35, ADC_VREF) and converting them into binary, i.e. a number that can be digitally stored.

The Pico has a 12-bit ADC (ENOB of 8.7-bit, see RP2040 datasheet section 4.9.3 for more details), meaning that a read operation will return a number ranging from 0 to 4095 (2^12 - 1) for a total of 4096 possible values. Therefore, the resolution of the ADC is 3.3/4096, so roughly steps of 0.8 millivolts. The SparkFun breakout uses an OPA344 operational amplifier to boost the signal coming from the microphone to voltage levels that can be easily read by the ADC. An important side effect is that a bias of 0.5*Vcc is added to the signal, even when the microphone is not picking up any sound.

The ADC provides us with a raw voltage value but when dealing with sound, we’re more interested in the amplitude of the audio signal. This is defined as one half the peak-to-peak amplitude. Included with this example is a very simple Python script that will plot the voltage values it receives via the serial port. By tweaking the sampling rates, and various other parameters, the data from the microphone can be analysed in various ways, such as in a Fast Fourier Transform to see what frequencies make up the signal.

_Figure 13. Example output from included Python script_

**==> picture [319 x 170] intentionally omitted <==**

## **Wiring information**

Wiring up the device requires 3 jumpers, to connect VCC (3.3v), GND, and AOUT. The example here uses ADC0, which is GP26. Power is supplied from the 3.3V pin.

Attaching a microphone using the ADC

**603**

Raspberry Pi Pico-series C/C++ SDK

##  **WARNING**

Most boards will take a range of VCC voltages from the Pico’s default 3.3V to the 5 volts commonly seen on other microcontrollers. Ensure your board doesn’t output an analogue signal greater than 3.3V as this may result in permanent damage to the Pico’s ADC.

_Figure 14. Wiring Diagram for ICS-40180 microphone breakout board._

**==> picture [319 x 221] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/adc/microphone_adc/CMakeLists.txt_

1 add_executable(microphone_adc 2         microphone_adc.c 3         ) 4 5 # pull in common dependencies and adc hardware support 6 target_link_libraries(microphone_adc pico_stdlib hardware_adc) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(microphone_adc) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(microphone_adc)

## **microphone_adc.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/adc/microphone_adc/microphone_adc.c_

1 _/**_ 2 _* Copyright (c) 2021 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6

Attaching a microphone using the ADC

**604**

Raspberry Pi Pico-series C/C++ SDK

7 _#include <stdio.h>_ 8 _#include "pico/stdlib.h"_ 9 _#include "hardware/gpio.h"_ 10 _#include "hardware/adc.h"_ 11 _#include "hardware/uart.h"_ 12 _#include "pico/binary_info.h"_ 13 14 _/* Example code to extract analog values from a microphone using the ADC_ 15 _with accompanying Python file to plot these values_ 16 17 _Connections on Raspberry Pi Pico board, other boards may vary._ 18 19 _GPIO 26/ADC0 (pin 31)-> AOUT or AUD on microphone board_ 20 _3.3v (pin 36) -> VCC on microphone board_ 21 _GND (pin 38)  -> GND on microphone board_ 22 _*/_ 23 24 _#define ADC_NUM 0_ 25 _#define ADC_PIN (26 + ADC_NUM)_ 26 _#define ADC_VREF 3.3_ 27 _#define ADC_RANGE (1 << 12)_ 28 _#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))_ 29 30 int main() { 31     stdio_init_all(); 32     printf("Beep boop, listening...\n"); 33 34     bi_decl(bi_program_description("Analog microphone example for Raspberry Pi Pico")); _// for picotool_ 35     bi_decl(bi_1pin_with_name(ADC_PIN, "ADC input pin")); 36 37     adc_init(); 38     adc_gpio_init( ADC_PIN); 39     adc_select_input( ADC_NUM); 40 41     uint adc_raw; 42     while (1) { 43         adc_raw = adc_read(); _// raw voltage from ADC_ 44         printf("%.2f\n", adc_raw * ADC_CONVERT); 45         sleep_ms(10); 46     } 47 }

## **Bill of Materials**

||**Bill of Materials**|||
|---|---|---|---|
|_Table 41. A list of_<br>_materials required for_<br>_the example_|**Item**|**Quantity**|Details|
||Breadboard|1|generic part|
||Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
||ICS-40180 microphone breakout<br>board or similar|1|From SparkFun|
||M/M Jumper wires|3|generic part|



Attaching a microphone using the ADC

**605**

Raspberry Pi Pico-series C/C++ SDK

## **Attaching a BME280 temperature/humidity/pressure sensor via SPI**

This example code shows how to interface the Raspberry Pi Pico to a BME280 temperature/humidity/pressure. The particular device used can be interfaced via I2C or SPI, we are using SPI, and interfacing at 3.3v.

This examples reads the data from the sensor, and runs it through the appropriate compensation routines (see the chip datasheet for details https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280ds002.pdf). At startup the compensation parameters required by the compensation routines are read from the chip. )

## **Wiring information**

Wiring up the device requires 6 jumpers as follows:

- [GPIO 16 (pin 21) MISO/spi0_rx][→][ SDO/SDO on bme280 board]

- [GPIO 17 (pin 22) Chip select ][→][ CSB/!CS on bme280 board]

- [GPIO 18 (pin 24) SCK/spi0_sclk ][→][ SCL/SCK on bme280 board]

- [GPIO 19 (pin 25) MOSI/spi0_tx ][→][ SDA/SDI on bme280 board]

- [3.3v (pin 3;6) ][→][ VCC on bme280 board]

- [GND (pin 38) ][→][ GND on bme280 board]

The example here uses SPI port 0. Power is supplied from the 3.3V pin.

##  **NOTE**

There are many different manufacturers who sell boards with the BME280. Whilst they all appear slightly different, they all have, at least, the same 6 pins required to power and communicate. When wiring up a board that is different to the one in the diagram, ensure you connect up as described in the previous paragraph.

_Figure 15. Wiring Diagram for bme280._

**==> picture [319 x 117] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/bme280_spi/CMakeLists.txt_

1 add_executable(bme280_spi 2         bme280_spi.c 3         ) 4 5 # pull in common dependencies and additional spi hardware support 6 target_link_libraries(bme280_spi pico_stdlib hardware_spi)

Attaching a BME280 temperature/humidity/pressure sensor via SPI

**606**

Raspberry Pi Pico-series C/C++ SDK

7

8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(bme280_spi) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(bme280_spi)

## **bme280_spi.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/bme280_spi/bme280_spi.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/spi.h"_ 12

13 _/* Example code to talk to a bme280 humidity/temperature/pressure sensor._ 14

- 15 _NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico_ 16 _GPIO (and therefore SPI) cannot be used at 5v._ 17

18 _You will need to use a level shifter on the SPI lines if you want to run the_ 19 _board at 5v._ 20

21 _Connections on Raspberry Pi Pico board and a generic bme280 board, other_ 22 _boards may vary._ 23

24 _GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO on bme280 board_ 25 _GPIO 17 (pin 22) Chip select -> CSB/!CS on bme280 board_ 26 _GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK on bme280 board_ 27 _GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI on bme280 board_ 28 _3.3v (pin 36) -> VCC on bme280 board_ 29 _GND (pin 38)  -> GND on bme280 board_ 30

31 _Note: SPI devices can have a number of different naming schemes for pins. See_

32 _the Wikipedia page at https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_ 33 _for variations._ 34

35 _This code uses a bunch of register definitions, and some compensation code derived_ 36 _from the Bosch datasheet which can be found here._

37 _https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280ds002.pdf_ 38 _*/_ 39 40 _#define READ_BIT 0x80_ 41 42 int32_t t_fine; 43 44 uint16_t dig_T1; 45 int16_t dig_T2, dig_T3; 46 uint16_t dig_P1; 47 int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9; 48 uint8_t dig_H1, dig_H3; 49 int8_t dig_H6;

Attaching a BME280 temperature/humidity/pressure sensor via SPI

**607**

Raspberry Pi Pico-series C/C++ SDK

50 int16_t dig_H2, dig_H4, dig_H5; 51 52 _/* The following compensation functions are required to convert from the raw ADC_ 53 _data from the chip to something usable. Each chip has a different set of_ 54 _compensation parameters stored on the chip at point of manufacture, which are_ 55 _read from the chip at startup and used in these routines._ 56 _*/_ 57 int32_t compensate_temp(int32_t adc_T) { 58     int32_t var1, var2, T; 59     var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11; 60     var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) 61             >> 14; 62 63     t_fine = var1 + var2; 64     T = (t_fine * 5 + 128) >> 8; 65     return T; 66 } 67 68 uint32_t compensate_pressure(int32_t adc_P) { 69     int32_t var1, var2; 70     uint32_t p; 71     var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000; 72     var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) dig_P6); 73     var2 = var2 + ((var1 * ((int32_t) dig_P5)) << 1); 74     var2 = (var2 >> 2) + (((int32_t) dig_P4) << 16); 75     var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t) dig_P2) * var1) >> 1)) >> 18; 76     var1 = ((((32768 + var1)) * ((int32_t) dig_P1)) >> 15); 77     if (var1 == 0) 78         return 0; 79 80     p = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12))) * 3125; 81     if (p < 0x80000000) 82         p = (p << 1) / ((uint32_t) var1); 83     else 84         p = (p / (uint32_t) var1) * 2; 85 86     var1 = (((int32_t) dig_P9) * ((int32_t) (((p >> 3) * (p >> 3)) >> 13))) >> 12; 87     var2 = (((int32_t) (p >> 2)) * ((int32_t) dig_P8)) >> 13; 88     p = (uint32_t) ((int32_t) p + ((var1 + var2 + dig_P7) >> 4)); 89 90     return p; 91 } 92 93 uint32_t compensate_humidity(int32_t adc_H) { 94     int32_t v_x1_u32r; 95     v_x1_u32r = (t_fine - ((int32_t) 76800)); 96     v_x1_u32r = (((((adc_H << 14) - (((int32_t) dig_H4) << 20) - (((int32_t) dig_H5) * v_x1_u32r)) + 97                    ((int32_t) 16384)) >> 15) * (((((((v_x1_u32r * ((int32_t) dig_H6)) >> 10) * (((v_x1_u32r * 98 ((int32_t) dig_H3)) 99             >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) * 100                                                  ((int32_t) dig_H2) + 8192) >> 14)); 101     v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) dig_H1)) >> 4)); 102     v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r); 103     v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r); 104 105     return (uint32_t) (v_x1_u32r >> 12); 106 } 107

Attaching a BME280 temperature/humidity/pressure sensor via SPI

**608**

Raspberry Pi Pico-series C/C++ SDK

108 _#ifdef PICO_DEFAULT_SPI_CSN_PIN_ 109 static inline void cs_select() { 110     asm volatile("nop \n nop \n nop"); 111     gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0); _// Active low_ 112     asm volatile("nop \n nop \n nop"); 113 } 114 115 static inline void cs_deselect() { 116     asm volatile("nop \n nop \n nop"); 117     gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); 118     asm volatile("nop \n nop \n nop"); 119 } 120 _#endif_ 121 122 _#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)_ 123 static void write_register(uint8_t reg, uint8_t data) { 124     uint8_t buf[2]; 125     buf[0] = reg & 0x7f; _// remove read bit as this is a write_ 126     buf[1] = data; 127     cs_select(); 128     spi_write_blocking(spi_default, buf, 2); 129     cs_deselect(); 130     sleep_ms(10); 131 } 132 133 static void read_registers(uint8_t reg, uint8_t *buf, uint16_t len) { 134 _// For this particular device, we send the device the register we want to read_ 135 _// first, then subsequently read from the device. The register is auto incrementing_ 136 _// so we don't need to keep sending the register we want, just the first._ 137     reg |= READ_BIT; 138     cs_select(); 139     spi_write_blocking(spi_default, &reg, 1); 140     sleep_ms(10); 141     spi_read_blocking(spi_default, 0, buf, len); 142     cs_deselect(); 143     sleep_ms(10); 144 } 145 146 _/* This function reads the manufacturing assigned compensation parameters from the device */_ 147 void read_compensation_parameters() { 148     uint8_t buffer[26]; 149 150     read_registers(0x88, buffer, 26); 151 152     dig_T1 = buffer[0] | (buffer[1] << 8); 153     dig_T2 = buffer[2] | (buffer[3] << 8); 154     dig_T3 = buffer[4] | (buffer[5] << 8); 155 156     dig_P1 = buffer[6] | (buffer[7] << 8); 157     dig_P2 = buffer[8] | (buffer[9] << 8); 158     dig_P3 = buffer[10] | (buffer[11] << 8); 159     dig_P4 = buffer[12] | (buffer[13] << 8); 160     dig_P5 = buffer[14] | (buffer[15] << 8); 161     dig_P6 = buffer[16] | (buffer[17] << 8); 162     dig_P7 = buffer[18] | (buffer[19] << 8); 163     dig_P8 = buffer[20] | (buffer[21] << 8); 164     dig_P9 = buffer[22] | (buffer[23] << 8); 165 166     dig_H1 = buffer[25]; _// 0xA1_ 167 168     read_registers(0xE1, buffer, 8); 169 170     dig_H2 = buffer[0] | (buffer[1] << 8); _// 0xE1 | 0xE2_ 171     dig_H3 = (int8_t) buffer[2]; _// 0xE3_

Attaching a BME280 temperature/humidity/pressure sensor via SPI

**609**

Raspberry Pi Pico-series C/C++ SDK

172     dig_H4 = buffer[3] << 4 | (buffer[4] & 0xf); _// 0xE4 | 0xE5[3:0]_ 173     dig_H5 = (buffer[4] >> 4) | (buffer[5] << 4); _// 0xE5[7:4] | 0xE6_ 174     dig_H6 = (int8_t) buffer[6]; _// 0xE7_ 175 } 176 177 static void bme280_read_raw(int32_t *humidity, int32_t *pressure, int32_t *temperature) { 178     uint8_t buffer[8]; 179 180     read_registers(0xF7, buffer, 8); 181     *pressure = ((uint32_t) buffer[0] << 12) | ((uint32_t) buffer[1] << 4) | (buffer[2] >> 4); 182     *temperature = ((uint32_t) buffer[3] << 12) | ((uint32_t) buffer[4] << 4) | (buffer[5] >> 4); 183     *humidity = (uint32_t) buffer[6] << 8 | buffer[7]; 184 } 185 _#endif_ 186 187 int main() { 188     stdio_init_all(); 189 _#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)_ 190 _#warning spi/bme280_spi example requires a board with SPI pins_ 191     puts("Default SPI pins were not defined"); 192 _#else_ 193 194     printf("Hello, bme280! Reading raw data from registers via SPI...\n"); 195 196 _// This example will use SPI0 at 0.5MHz._ 197     spi_init(spi_default, 500 * 1000); 198     gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI); 199     gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI); 200     gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI); 201 _// Make the SPI pins available to picotool_ 202     bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI)); 203 204 _// Chip select is active-low, so we'll initialise it to a driven-high state_ 205     gpio_init(PICO_DEFAULT_SPI_CSN_PIN); 206     gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT); 207     gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); 208 _// Make the CS pin available to picotool_ 209     bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS")); 210 211 _// See if SPI is working - interrograte the device for its I2C ID number, should be 0x60_ 212     uint8_t id; 213     read_registers(0xD0, &id, 1); 214     printf("Chip ID is 0x%x\n", id); 215 216     read_compensation_parameters(); 217 218     write_register(0xF2, 0x1); _// Humidity oversampling register - going for x1_ 219     write_register(0xF4, 0x27); _// Set rest of oversampling modes and run mode to normal_ 220 221     int32_t humidity, pressure, temperature; 222 223     while (1) { 224         bme280_read_raw(&humidity, &pressure, &temperature); 225 226 _// These are the raw numbers from the chip, so we need to run through the_ 227 _// compensations to get human understandable numbers_ 228         temperature = compensate_temp(temperature); 229         pressure = compensate_pressure(pressure); 230         humidity = compensate_humidity(humidity);

Attaching a BME280 temperature/humidity/pressure sensor via SPI

**610**

Raspberry Pi Pico-series C/C++ SDK

231 232         printf("Humidity = %.2f%%\n", humidity / 1024.0); 233         printf("Pressure = %dPa\n", pressure); 234         printf("Temp. = %.2fC\n", temperature / 100.0); 235 236         sleep_ms(1000); 237     } 238 _#endif_ 239 }

## **Bill of Materials**

_Table 42. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|BME280 board|1|generic part|
|M/M Jumper wires|6|generic part|



## **Attaching a MPU9250 accelerometer/gyroscope via SPI**

This example code shows how to interface the Raspberry Pi Pico to the MPU9250 accelerometer/gyroscope board. The particular device used can be interfaced via I2C or SPI, we are using SPI, and interfacing at 3.3v.

##  **NOTE**

This is a very basic example, and only recovers raw data from the sensor. There are various calibration options available that should be used to ensure that the final results are accurate. It is also possible to wire up the interrupt pin to a GPIO and read data only when it is ready, rather than using the polling approach in the example.

## **Wiring information**

Wiring up the device requires 6 jumpers as follows:

- [GPIO 4 (pin 6) MISO/spi0_rx][→][ ADO on MPU9250 board]

- [GPIO 5 (pin 7) Chip select ][→][ NCS on MPU9250 board]

- [GPIO 6 (pin 9) SCK/spi0_sclk ][→][ SCL on MPU9250 board]

- [GPIO 7 (pin 10) MOSI/spi0_tx ][→][ SDA on MPU9250 board]

- [3.3v (pin 36) ][→][ VCC on MPU9250 board]

- [GND (pin 38) ][→][ GND on MPU9250 board]

The example here uses SPI port 0. Power is supplied from the 3.3V pin.

Attaching a MPU9250 accelerometer/gyroscope via SPI

**611**

Raspberry Pi Pico-series C/C++ SDK

##  **NOTE**

There are many different manufacturers who sell boards with the MPU9250. Whilst they all appear slightly different, they all have, at least, the same 6 pins required to power and communicate. When wiring up a board that is different to the one in the diagram, ensure you connect up as described in the previous paragraph.

_Figure 16. Wiring Diagram for MPU9250._

**==> picture [319 x 116] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/mpu9250_spi/CMakeLists.txt_

- 1 add_executable(mpu9250_spi 2         mpu9250_spi.c 3         ) 4 5 # pull in common dependencies and additional spi hardware support 6 target_link_libraries(mpu9250_spi pico_stdlib hardware_spi) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(mpu9250_spi) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(mpu9250_spi)

## **mpu9250_spi.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/mpu9250_spi/mpu9250_spi.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/spi.h"_ 12 13 _/* Example code to talk to a MPU9250 MEMS accelerometer and gyroscope._ 14 _Ignores the magnetometer, that is left as a exercise for the reader._ 15

Attaching a MPU9250 accelerometer/gyroscope via SPI

**612**

Raspberry Pi Pico-series C/C++ SDK

16 _This is taking to simple approach of simply reading registers. It's perfectly_ 17 _possible to link up an interrupt line and set things up to read from the_ 18 _inbuilt FIFO to make it more useful._ 19 20 _NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico_ 21 _GPIO (and therefore SPI) cannot be used at 5v._ 22 23 _You will need to use a level shifter on the I2C lines if you want to run the_ 24 _board at 5v._ 25 26 _Connections on Raspberry Pi Pico board and a generic MPU9250 board, other_ 27 _boards may vary._ 28 29 _GPIO 4 (pin 6) MISO/spi0_rx-> ADO on MPU9250 board_ 30 _GPIO 5 (pin 7) Chip select -> NCS on MPU9250 board_ 31 _GPIO 6 (pin 9) SCK/spi0_sclk -> SCL on MPU9250 board_ 32 _GPIO 7 (pin 10) MOSI/spi0_tx -> SDA on MPU9250 board_ 33 _3.3v (pin 36) -> VCC on MPU9250 board_ 34 _GND (pin 38)  -> GND on MPU9250 board_ 35 36 _Note: SPI devices can have a number of different naming schemes for pins. See_ 37 _the Wikipedia page at https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_ 38 _for variations._ 39 _The particular device used here uses the same pins for I2C and SPI, hence the_ 40 _using of I2C names_ 41 _*/_ 42 43 _#define PIN_MISO 4_ 44 _#define PIN_CS   5_ 45 _#define PIN_SCK  6_ 46 _#define PIN_MOSI 7_ 47 48 _#define SPI_PORT spi0_ 49 _#define READ_BIT 0x80_ 50 51 static inline void cs_select() { 52     asm volatile("nop \n nop \n nop"); 53     gpio_put(PIN_CS, 0); _// Active low_ 54     asm volatile("nop \n nop \n nop"); 55 } 56 57 static inline void cs_deselect() { 58     asm volatile("nop \n nop \n nop"); 59     gpio_put(PIN_CS, 1); 60     asm volatile("nop \n nop \n nop"); 61 } 62 63 static void mpu9250_reset() { 64 _// Two byte reset. First byte register, second byte data_ 65 _// There are a load more options to set up the device in different ways that could be added here_ 66     uint8_t buf[] = {0x6B, 0x00}; 67     cs_select(); 68     spi_write_blocking(SPI_PORT, buf, 2); 69     cs_deselect(); 70 } 71 72 73 static void read_registers(uint8_t reg, uint8_t *buf, uint16_t len) { 74 _// For this particular device, we send the device the register we want to read_ 75 _// first, then subsequently read from the device. The register is auto incrementing_ 76 _// so we don't need to keep sending the register we want, just the first._ 77 78     reg |= READ_BIT;

Attaching a MPU9250 accelerometer/gyroscope via SPI

**613**

Raspberry Pi Pico-series C/C++ SDK

79     cs_select(); 80     spi_write_blocking(SPI_PORT, &reg, 1); 81     sleep_ms(10); 82     spi_read_blocking(SPI_PORT, 0, buf, len); 83     cs_deselect(); 84     sleep_ms(10); 85 } 86 87 88 static void mpu9250_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) { 89     uint8_t buffer[6]; 90 91 _// Start reading acceleration registers from register 0x3B for 6 bytes_ 92     read_registers(0x3B, buffer, 6); 93 94     for (int i = 0; i < 3; i++) { 95         accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]); 96     } 97 98 _// Now gyro data from reg 0x43 for 6 bytes_ 99     read_registers(0x43, buffer, 6); 100 101     for (int i = 0; i < 3; i++) { 102         gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);; 103     } 104 105 _// Now temperature from reg 0x41 for 2 bytes_ 106     read_registers(0x41, buffer, 2); 107 108     *temp = buffer[0] << 8 | buffer[1]; 109 } 110

111 int main() { 112     stdio_init_all(); 113

114     printf("Hello, MPU9250! Reading raw data from registers via SPI...\n"); 115

116 _// This example will use SPI0 at 0.5MHz._ 117     spi_init(SPI_PORT, 500 * 1000); 118     gpio_set_function(PIN_MISO, GPIO_FUNC_SPI); 119     gpio_set_function(PIN_SCK, GPIO_FUNC_SPI); 120     gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI); 121 _// Make the SPI pins available to picotool_ 122     bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, GPIO_FUNC_SPI)); 123 124 _// Chip select is active-low, so we'll initialise it to a driven-high state_ 125     gpio_init(PIN_CS); 126     gpio_set_dir(PIN_CS, GPIO_OUT); 127     gpio_put(PIN_CS, 1); 128 _// Make the CS pin available to picotool_ 129     bi_decl(bi_1pin_with_name(PIN_CS, "SPI CS")); 130 131     mpu9250_reset(); 132 133 _// See if SPI is working - interrograte the device for its I2C ID number, should be 0x71_ 134     uint8_t id; 135     read_registers(0x75, &id, 1); 136     printf("I2C address is 0x%x\n", id); 137 138     int16_t acceleration[3], gyro[3], temp; 139 140     while (1) { 141         mpu9250_read_raw(acceleration, gyro, &temp); 142

Attaching a MPU9250 accelerometer/gyroscope via SPI

**614**

Raspberry Pi Pico-series C/C++ SDK

143 _// These are the raw numbers from the chip, so will need tweaking to be really useful._ 144 _// See the datasheet for more information_ 145         printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]); 146         printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]); 147 _// Temperature is simple so use the datasheet calculation to get deg C._ 148 _// Note this is chip temperature._ 149         printf("Temp. = %f\n", (temp / 340.0) + 36.53); 150 151         sleep_ms(100); 152     } 153 }

## **Bill of Materials**

_Table 43. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|MPU9250 board|1|generic part|
|M/M Jumper wires|6|generic part|



## **Attaching a MPU6050 accelerometer/gyroscope via I2C**

This example code shows how to interface the Raspberry Pi Pico to the MPU6050 accelerometer/gyroscope board. This device uses I2C for communications, and most MPU6050 parts are happy running at either 3.3 or 5v. The Raspberry Pi RP2040 GPIO’s work at 3.3v so that is what the example uses.

##  **NOTE**

This is a very basic example, and only recovers raw data from the sensor. There are various calibration options available that should be used to ensure that the final results are accurate. It is also possible to wire up the interrupt pin to a GPIO and read data only when it is ready, rather than using the polling approach in the example.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VCC (3.3v), GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 3.3V pin.

##  **NOTE**

There are many different manufacturers who sell boards with the MPU6050. Whilst they all appear slightly different, they all have, at least, the same 4 pins required to power and communicate. When wiring up a board that is different to the one in the diagram, ensure you connect up as described in the previous paragraph.

Attaching a MPU6050 accelerometer/gyroscope via I2C

**615**

Raspberry Pi Pico-series C/C++ SDK

_Figure 17. Wiring Diagram for MPU6050._

**==> picture [319 x 116] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mpu6050_i2c/CMakeLists.txt_

1 add_executable(mpu6050_i2c 2         mpu6050_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(mpu6050_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(mpu6050_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(mpu6050_i2c)

## **mpu6050_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mpu6050_i2c/mpu6050_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/i2c.h"_ 12 13 _/* Example code to talk to a MPU6050 MEMS accelerometer and gyroscope_ 14 15 _This is taking to simple approach of simply reading registers. It's perfectly_ 16 _possible to link up an interrupt line and set things up to read from the_ 17 _inbuilt FIFO to make it more useful._ 18 19 _NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico_ 20 _GPIO (and therefore I2C) cannot be used at 5v._ 21 22 _You will need to use a level shifter on the I2C lines if you want to run the_

Attaching a MPU6050 accelerometer/gyroscope via I2C

**616**

Raspberry Pi Pico-series C/C++ SDK

23 _board at 5v._ 24 25 _Connections on Raspberry Pi Pico board, other boards may vary._ 26 27 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is GP4 (pin 6)) -> SDA on MPU6050 board_ 28 _GPIO PICO_DEFAULT_I2C_SCL_PIN (On Pico this is GP5 (pin 7)) -> SCL on MPU6050 board_ 29 _3.3v (pin 36) -> VCC on MPU6050 board_ 30 _GND (pin 38)  -> GND on MPU6050 board_ 31 _*/_ 32 33 _// By default these devices  are on bus address 0x68_ 34 static int addr = 0x68; 35 36 _#ifdef i2c_default_ 37 static void mpu6050_reset() { 38 _// Two byte reset. First byte register, second byte data_ 39 _// There are a load more options to set up the device in different ways that could be added here_ 40     uint8_t buf[] = {0x6B, 0x80}; 41     i2c_write_blocking(i2c_default, addr, buf, 2, false); 42     sleep_ms(100); _// Allow device to reset and stabilize_ 43 44 _// Clear sleep mode (0x6B register, 0x00 value)_ 45     buf[1] = 0x00; _// Clear sleep mode by writing 0x00 to the 0x6B register_ 46     i2c_write_blocking(i2c_default, addr, buf, 2, false); 47     sleep_ms(10); _// Allow stabilization after waking up_ 48 } 49 50 static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) { 51 _// For this particular device, we send the device the register we want to read_ 52 _// first, then subsequently read from the device. The register is auto incrementing_ 53 _// so we don't need to keep sending the register we want, just the first._ 54 55     uint8_t buffer[6]; 56 57 _// Start reading acceleration registers from register 0x3B for 6 bytes_ 58     uint8_t val = 0x3B; 59     i2c_write_blocking(i2c_default, addr, &val, 1, true); _// true to keep master control of bus_ 60     i2c_read_blocking(i2c_default, addr, buffer, 6, false); 61 62     for (int i = 0; i < 3; i++) { 63         accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]); 64     } 65 66 _// Now gyro data from reg 0x43 for 6 bytes_ 67 _// The register is auto incrementing on each read_ 68     val = 0x43; 69     i2c_write_blocking(i2c_default, addr, &val, 1, true); 70     i2c_read_blocking(i2c_default, addr, buffer, 6, false); _// False - finished with bus_ 71 72     for (int i = 0; i < 3; i++) { 73         gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);; 74     } 75 76 _// Now temperature from reg 0x41 for 2 bytes_ 77 _// The register is auto incrementing on each read_ 78     val = 0x41; 79     i2c_write_blocking(i2c_default, addr, &val, 1, true); 80     i2c_read_blocking(i2c_default, addr, buffer, 2, false); _// False - finished with bus_ 81 82     *temp = buffer[0] << 8 | buffer[1]; 83 } 84 _#endif_

Attaching a MPU6050 accelerometer/gyroscope via I2C

**617**

Raspberry Pi Pico-series C/C++ SDK

85 86 int main() { 87     stdio_init_all(); 88 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 89 _#warning i2c/mpu6050_i2c example requires a board with I2C pins_ 90     puts("Default I2C pins were not defined"); 91     return 0; 92 _#else_ 93     printf("Hello, MPU6050! Reading raw data from registers...\n"); 94 95 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 96     i2c_init(i2c_default, 400 * 1000); 97     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 98     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 99     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 100     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 101 _// Make the I2C pins available to picotool_ 102     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 103 104     mpu6050_reset(); 105 106     int16_t acceleration[3], gyro[3], temp; 107 108     while (1) { 109         mpu6050_read_raw(acceleration, gyro, &temp); 110 111 _// These are the raw numbers from the chip, so will need tweaking to be really useful._ 112 _// See the datasheet for more information_ 113         printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]); 114         printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]); 115 _// Temperature is simple so use the datasheet calculation to get deg C._ 116 _// Note this is chip temperature._ 117         printf("Temp. = %f\n", (temp / 340.0) + 36.53); 118 119         sleep_ms(100); 120     } 121 _#endif_ 122 }

## **Bill of Materials**

||**Bill of Materials**|||
|---|---|---|---|
|_Table 44. A list of_<br>_materials required for_<br>_the example_|**Item**|**Quantity**|Details|
||Breadboard|1|generic part|
||Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
||MPU6050 board|1|generic part|
||M/M Jumper wires|4|generic part|



## **Attaching a 16x2 LCD via I2C**

This example code shows how to interface the Raspberry Pi Pico to one of the very common 16x2 LCD character

Attaching a 16x2 LCD via I2C

**618**

Raspberry Pi Pico-series C/C++ SDK

displays. The display will need a 3.3V I2C adapter board as this example uses I2C for communications.

##  **NOTE**

These LCD displays can also be driven directly using GPIO without the use of an adapter board. That is beyond the scope of this example.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VCC (3.3v), GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 3.3V pin.

##  **WARNING**

Many displays of this type are 5v. If you wish to use a 5v display you will need to use level shifters on the SDA and SCL lines to convert from the 3.3V used by the RP2040. Whilst a 5v display will just about work at 3.3v, the display will be dim.

_Figure 18. Wiring Diagram for LCD1602A LCD with I2C bridge._

**==> picture [319 x 179] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/lcd_1602_i2c/CMakeLists.txt_

1 add_executable(lcd_1602_i2c 2         lcd_1602_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(lcd_1602_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(lcd_1602_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(lcd_1602_i2c)

Attaching a 16x2 LCD via I2C

**619**

Raspberry Pi Pico-series C/C++ SDK

## **lcd_1602_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/lcd_1602_i2c/lcd_1602_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "hardware/i2c.h"_ 11 _#include "pico/binary_info.h"_ 12 13 _/* Example code to drive a 16x2 LCD panel via a I2C bridge chip (e.g. PCF8574)_ 14 15 _NOTE: The panel must be capable of being driven at 3.3v NOT 5v. The Pico_ 16 _GPIO (and therefore I2C) cannot be used at 5v._ 17 18 _You will need to use a level shifter on the I2C lines if you want to run the_ 19 _board at 5v._ 20 21 _Connections on Raspberry Pi Pico board, other boards may vary._ 22 23 _GPIO 4 (pin 6)-> SDA on LCD bridge board_ 24 _GPIO 5 (pin 7)-> SCL on LCD bridge board_ 25 _3.3v (pin 36) -> VCC on LCD bridge board_ 26 _GND (pin 38)  -> GND on LCD bridge board_ 27 _*/_ 28 _// commands_ 29 const int LCD_CLEARDISPLAY = 0x01; 30 const int LCD_RETURNHOME = 0x02; 31 const int LCD_ENTRYMODESET = 0x04; 32 const int LCD_DISPLAYCONTROL = 0x08; 33 const int LCD_CURSORSHIFT = 0x10; 34 const int LCD_FUNCTIONSET = 0x20; 35 const int LCD_SETCGRAMADDR = 0x40; 36 const int LCD_SETDDRAMADDR = 0x80; 37 38 _// flags for display entry mode_ 39 const int LCD_ENTRYSHIFTINCREMENT = 0x01; 40 const int LCD_ENTRYLEFT = 0x02; 41 42 _// flags for display and cursor control_ 43 const int LCD_BLINKON = 0x01; 44 const int LCD_CURSORON = 0x02; 45 const int LCD_DISPLAYON = 0x04; 46 47 _// flags for display and cursor shift_ 48 const int LCD_MOVERIGHT = 0x04; 49 const int LCD_DISPLAYMOVE = 0x08; 50 51 _// flags for function set_ 52 const int LCD_5x10DOTS = 0x04; 53 const int LCD_2LINE = 0x08; 54 const int LCD_8BITMODE = 0x10; 55 56 _// flag for backlight control_ 57 const int LCD_BACKLIGHT = 0x08; 58

Attaching a 16x2 LCD via I2C

**620**

Raspberry Pi Pico-series C/C++ SDK

59 const int LCD_ENABLE_BIT = 0x04; 60 61 _// By default these LCD display drivers are on bus address 0x27_ 62 static int addr = 0x27; 63 64 _// Modes for lcd_send_byte_ 65 _#define LCD_CHARACTER  1_ 66 _#define LCD_COMMAND    0_ 67 68 _#define MAX_LINES      2_ 69 _#define MAX_CHARS      16_ 70 71 _/* Quick helper function for single byte transfers */_ 72 void i2c_write_byte(uint8_t val) { 73 _#ifdef i2c_default_ 74     i2c_write_blocking(i2c_default, addr, &val, 1, false); 75 _#endif_ 76 } 77 78 void lcd_toggle_enable(uint8_t val) { 79 _// Toggle enable pin on LCD display_ 80 _// We cannot do this too quickly or things don't work_ 81 _#define DELAY_US 600_ 82     sleep_us(DELAY_US); 83     i2c_write_byte(val | LCD_ENABLE_BIT); 84     sleep_us(DELAY_US); 85     i2c_write_byte(val & ~LCD_ENABLE_BIT); 86     sleep_us(DELAY_US); 87 } 88 89 _// The display is sent a byte as two separate nibble transfers_ 90 void lcd_send_byte(uint8_t val, int mode) { 91     uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT; 92     uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT; 93 94     i2c_write_byte(high); 95     lcd_toggle_enable(high); 96     i2c_write_byte(low); 97     lcd_toggle_enable(low); 98 } 99 100 void lcd_clear(void) { 101     lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND); 102 } 103 104 _// go to location on LCD_ 105 void lcd_set_cursor(int line, int position) { 106     int val = (line == 0) ? 0x80 + position : 0xC0 + position; 107     lcd_send_byte(val, LCD_COMMAND); 108 } 109 110 static inline void lcd_char(char val) { 111     lcd_send_byte(val, LCD_CHARACTER); 112 } 113 114 void lcd_string(const char *s) { 115     while (*s) { 116         lcd_char(*s++); 117     } 118 } 119 120 void lcd_init() { 121     lcd_send_byte(0x03, LCD_COMMAND); 122     lcd_send_byte(0x03, LCD_COMMAND);

Attaching a 16x2 LCD via I2C

**621**

Raspberry Pi Pico-series C/C++ SDK

123     lcd_send_byte(0x03, LCD_COMMAND); 124     lcd_send_byte(0x02, LCD_COMMAND); 125 126     lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND); 127     lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND); 128     lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND); 129     lcd_clear(); 130 } 131 132 int main() { 133 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 134 _#warning i2c/lcd_1602_i2c example requires a board with I2C pins_ 135 _#else_ 136 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 137     i2c_init(i2c_default, 100 * 1000); 138     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 139     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 140     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 141     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 142 _// Make the I2C pins available to picotool_ 143     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 144 145     lcd_init(); 146 147     static char *message[] = 148             { 149                     "RP2040 by", "Raspberry Pi", 150                     "A brand new", "microcontroller", 151                     "Twin core M0", "Full C SDK", 152                     "More power in", "your product", 153                     "More beans", "than Heinz!" 154             }; 155 156     while (1) { 157         for (uint m = 0; m < sizeof(message) / sizeof(message[0]); m += MAX_LINES) { 158             for (int line = 0; line < MAX_LINES; line++) { 159                 lcd_set_cursor(line, (MAX_CHARS / 2) - strlen(message[m + line]) / 2); 160                 lcd_string(message[m + line]); 161             } 162             sleep_ms(2000); 163             lcd_clear(); 164         } 165     } 166 _#endif_ 167 }

## **Bill of Materials**

||**Bill of Materials**|||
|---|---|---|---|
|_Table 45. A list of_<br>_materials required for_<br>_the example_|**Item**|**Quantity**|Details|
||Breadboard|1|generic part|
||Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
||1602A based LCD panel 3.3v|1|generic part|
||1602A to I2C bridge device 3.3v|1|generic part|



Attaching a 16x2 LCD via I2C

**622**

Raspberry Pi Pico-series C/C++ SDK

M/M Jumper wires 4 generic part

## **Attaching a BMP280 temp/pressure sensor via I2C**

This example code shows how to interface the Raspberry Pi Pico with the popular BMP280 temperature and air pressure sensor manufactured by Bosch. A similar variant, the BME280, exists that can also measure humidity. There is another example that uses the BME280 device but talks to it via SPI as opposed to I2C.

The code reads data from the sensor’s registers every 500 milliseconds and prints it via the onboard UART. This example operates the BMP280 in _normal_ mode, meaning that the device continuously cycles between a measurement period and a standby period at a regular interval we can set. This has the advantage that subsequent reads do not require configuration register writes and is the recommended mode of operation to filter out short-term disturbances.

##  **TIP**

The BMP280 is highly configurable with 3 modes of operation, various oversampling levels, and 5 filter settings. Find the datasheet online (https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bstbmp280-ds001.pdf) to explore all of its capabilities beyond the simple example given here.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VCC (3.3v), GND, SDA and SCL. The example here uses the default I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 3.3V pin from the Pico.

##  **WARNING**

The BMP280 has a maximum supply voltage rating of 3.6V. Most breakout boards have voltage regulators that will allow a range of input voltages of 2-6V, but make sure to check beforehand.

_Figure 19. Wiring Diagram for BMP280 sensor via I2C._

**==> picture [319 x 221] intentionally omitted <==**

## **List of Files**

Attaching a BMP280 temp/pressure sensor via I2C

**623**

Raspberry Pi Pico-series C/C++ SDK

## **CMakeLists.txt**

CMake file to incorporate the example into the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/bmp280_i2c/CMakeLists.txt_

1 add_executable(bmp280_i2c 2         bmp280_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(bmp280_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(bmp280_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(bmp280_i2c)

## **bmp280_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/bmp280_i2c/bmp280_i2c.c_

1 _/**_ 2 _* Copyright (c) 2021 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _**/_ 6 7 _#include <stdio.h>_ 8 9 _#include "hardware/i2c.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "pico/stdlib.h"_ 12 13 _/* Example code to talk to a BMP280 temperature and pressure sensor_ 14 15 _NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico_ 16 _GPIO (and therefore I2C) cannot be used at 5v._ 17 18 _You will need to use a level shifter on the I2C lines if you want to run the_ 19 _board at 5v._ 20 21 _Connections on Raspberry Pi Pico board, other boards may vary._ 22 23 _GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on BMP280_ 24 _board_ 25 _GPIO PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7)) -> SCL on_ 26 _BMP280 board_ 27 _3.3v (pin 36) -> VCC on BMP280 board_ 28 _GND (pin 38)  -> GND on BMP280 board_ 29 _*/_ 30 31 _// device has default bus address of 0x76_ 32 _#define ADDR _u(0x76)_ 33 34 _// hardware registers_ 35 _#define REG_CONFIG _u(0xF5)_ 36 _#define REG_CTRL_MEAS _u(0xF4)_ 37 _#define REG_RESET _u(0xE0)_ 38

Attaching a BMP280 temp/pressure sensor via I2C

**624**

Raspberry Pi Pico-series C/C++ SDK

39 _#define REG_TEMP_XLSB _u(0xFC)_ 40 _#define REG_TEMP_LSB _u(0xFB)_ 41 _#define REG_TEMP_MSB _u(0xFA)_ 42 43 _#define REG_PRESSURE_XLSB _u(0xF9)_ 44 _#define REG_PRESSURE_LSB _u(0xF8)_ 45 _#define REG_PRESSURE_MSB _u(0xF7)_ 46 47 _// calibration registers_ 48 _#define REG_DIG_T1_LSB _u(0x88)_ 49 _#define REG_DIG_T1_MSB _u(0x89)_ 50 _#define REG_DIG_T2_LSB _u(0x8A)_ 51 _#define REG_DIG_T2_MSB _u(0x8B)_ 52 _#define REG_DIG_T3_LSB _u(0x8C)_ 53 _#define REG_DIG_T3_MSB _u(0x8D)_ 54 _#define REG_DIG_P1_LSB _u(0x8E)_ 55 _#define REG_DIG_P1_MSB _u(0x8F)_ 56 _#define REG_DIG_P2_LSB _u(0x90)_ 57 _#define REG_DIG_P2_MSB _u(0x91)_ 58 _#define REG_DIG_P3_LSB _u(0x92)_ 59 _#define REG_DIG_P3_MSB _u(0x93)_ 60 _#define REG_DIG_P4_LSB _u(0x94)_ 61 _#define REG_DIG_P4_MSB _u(0x95)_ 62 _#define REG_DIG_P5_LSB _u(0x96)_ 63 _#define REG_DIG_P5_MSB _u(0x97)_ 64 _#define REG_DIG_P6_LSB _u(0x98)_ 65 _#define REG_DIG_P6_MSB _u(0x99)_ 66 _#define REG_DIG_P7_LSB _u(0x9A)_ 67 _#define REG_DIG_P7_MSB _u(0x9B)_ 68 _#define REG_DIG_P8_LSB _u(0x9C)_ 69 _#define REG_DIG_P8_MSB _u(0x9D)_ 70 _#define REG_DIG_P9_LSB _u(0x9E)_ 71 _#define REG_DIG_P9_MSB _u(0x9F)_ 72 73 _// number of calibration registers to be read_ 74 _#define NUM_CALIB_PARAMS 24_ 75 76 struct bmp280_calib_param { 77 _// temperature params_ 78     uint16_t dig_t1; 79     int16_t dig_t2; 80     int16_t dig_t3; 81 82 _// pressure params_ 83     uint16_t dig_p1; 84     int16_t dig_p2; 85     int16_t dig_p3; 86     int16_t dig_p4; 87     int16_t dig_p5; 88     int16_t dig_p6; 89     int16_t dig_p7; 90     int16_t dig_p8; 91     int16_t dig_p9; 92 }; 93 94 _#ifdef i2c_default_ 95 void bmp280_init() { 96 _// use the "handheld device dynamic" optimal setting (see datasheet)_ 97     uint8_t buf[2]; 98 99 _// 500ms sampling time, x16 filter_ 100     const uint8_t reg_config_val = ((0x04 << 5) | (0x05 << 2)) & 0xFC; 101 102 _// send register number followed by its corresponding value_

Attaching a BMP280 temp/pressure sensor via I2C

**625**

Raspberry Pi Pico-series C/C++ SDK

103     buf[0] = REG_CONFIG; 104     buf[1] = reg_config_val; 105     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 106 107 _// osrs_t x1, osrs_p x4, normal mode operation_ 108     const uint8_t reg_ctrl_meas_val = (0x01 << 5) | (0x03 << 2) | (0x03); 109     buf[0] = REG_CTRL_MEAS; 110     buf[1] = reg_ctrl_meas_val; 111     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 112 } 113 114 void bmp280_read_raw(int32_t* temp, int32_t* pressure) { 115 _// BMP280 data registers are auto-incrementing and we have 3 temperature and_ 116 _// pressure registers each, so we start at 0xF7 and read 6 bytes to 0xFC_ 117 _// note: normal mode does not require further ctrl_meas and config register writes_ 118 119     uint8_t buf[6]; 120     uint8_t reg = REG_PRESSURE_MSB; 121     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); _// true to keep master control of bus_ 122     i2c_read_blocking(i2c_default, ADDR, buf, 6, false); _// false - finished with bus_ 123 124 _// store the 20 bit read in a 32 bit signed integer for conversion_ 125     *pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4); 126     *temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4); 127 } 128 129 void bmp280_reset() { 130 _// reset the device with the power-on-reset procedure_ 131     uint8_t buf[2] = { REG_RESET, 0xB6 }; 132     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 133 } 134 135 _// intermediate function that calculates the fine resolution temperature_ 136 _// used for both pressure and temperature conversions_ 137 int32_t bmp280_convert(int32_t temp, struct bmp280_calib_param* params) { 138 _// use the 32-bit fixed point compensation implementation given in the_ 139 _// datasheet_ 140 141     int32_t var1, var2; 142     var1 = ((((temp >> 3) - ((int32_t)params->dig_t1 << 1))) * ((int32_t)params->dig_t2)) >> 11; 143     var2 = (((((temp >> 4) - ((int32_t)params->dig_t1)) * ((temp >> 4) - ((int32_t)params>dig_t1))) >> 12) * ((int32_t)params->dig_t3)) >> 14; 144     return var1 + var2; 145 } 146 147 int32_t bmp280_convert_temp(int32_t temp, struct bmp280_calib_param* params) { 148 _// uses the BMP280 calibration parameters to compensate the temperature value read from its registers_ 149     int32_t t_fine = bmp280_convert(temp, params); 150     return (t_fine * 5 + 128) >> 8; 151 } 152 153 int32_t bmp280_convert_pressure(int32_t pressure, int32_t temp, struct bmp280_calib_param* params) { 154 _// uses the BMP280 calibration parameters to compensate the pressure value read from its registers_ 155 156     int32_t t_fine = bmp280_convert(temp, params); 157 158     int32_t var1, var2; 159     uint32_t converted = 0.0; 160     var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;

Attaching a BMP280 temp/pressure sensor via I2C

**626**

Raspberry Pi Pico-series C/C++ SDK

161     var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)params->dig_p6); 162     var2 += ((var1 * ((int32_t)params->dig_p5)) << 1); 163     var2 = (var2 >> 2) + (((int32_t)params->dig_p4) << 16); 164     var1 = (((params->dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t )params->dig_p2) * var1) >> 1)) >> 18; 165     var1 = ((((32768 + var1)) * ((int32_t)params->dig_p1)) >> 15); 166     if (var1 == 0) { 167         return 0; _// avoid exception caused by division by zero_ 168     } 169     converted = (((uint32_t)(((int32_t)1048576) - pressure) - (var2 >> 12))) * 3125; 170     if (converted < 0x80000000) { 171         converted = (converted << 1) / ((uint32_t)var1); 172     } else { 173         converted = (converted / (uint32_t)var1) * 2; 174     } 175     var1 = (((int32_t)params->dig_p9) * ((int32_t)(((converted >> 3) * (converted >> 3)) >> 13))) >> 12; 176     var2 = (((int32_t)(converted >> 2)) * ((int32_t)params->dig_p8)) >> 13; 177     converted = (uint32_t)((int32_t)converted + ((var1 + var2 + params->dig_p7) >> 4)); 178     return converted; 179 } 180 181 void bmp280_get_calib_params(struct bmp280_calib_param* params) { 182 _// raw temp and pressure values need to be calibrated according to_ 183 _// parameters generated during the manufacturing of the sensor_ 184 _// there are 3 temperature params, and 9 pressure params, each with a LSB_ 185 _// and MSB register, so we read from 24 registers_ 186 187     uint8_t buf[NUM_CALIB_PARAMS] = { 0 }; 188     uint8_t reg = REG_DIG_T1_LSB; 189     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); _// true to keep master control of bus_ 190 _// read in one go as register addresses auto-increment_ 191     i2c_read_blocking(i2c_default, ADDR, buf, NUM_CALIB_PARAMS, false); _// false, we're done reading_ 192 193 _// store these in a struct for later use_ 194     params->dig_t1 = (uint16_t)(buf[1] << 8) | buf[0]; 195     params->dig_t2 = (int16_t)(buf[3] << 8) | buf[2]; 196     params->dig_t3 = (int16_t)(buf[5] << 8) | buf[4]; 197 198     params->dig_p1 = (uint16_t)(buf[7] << 8) | buf[6]; 199     params->dig_p2 = (int16_t)(buf[9] << 8) | buf[8]; 200     params->dig_p3 = (int16_t)(buf[11] << 8) | buf[10]; 201     params->dig_p4 = (int16_t)(buf[13] << 8) | buf[12]; 202     params->dig_p5 = (int16_t)(buf[15] << 8) | buf[14]; 203     params->dig_p6 = (int16_t)(buf[17] << 8) | buf[16]; 204     params->dig_p7 = (int16_t)(buf[19] << 8) | buf[18]; 205     params->dig_p8 = (int16_t)(buf[21] << 8) | buf[20]; 206     params->dig_p9 = (int16_t)(buf[23] << 8) | buf[22]; 207 } 208 209 _#endif_ 210 211 int main() { 212     stdio_init_all(); 213 214 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 215 _#warning i2c / bmp280_i2c example requires a board with I2C pins_ 216         puts("Default I2C pins were not defined"); 217     return 0; 218 _#else_ 219 _// useful information for picotool_

Attaching a BMP280 temp/pressure sensor via I2C

**627**

Raspberry Pi Pico-series C/C++ SDK

220     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 221     bi_decl(bi_program_description("BMP280 I2C example for the Raspberry Pi Pico")); 222 223     printf("Hello, BMP280! Reading temperaure and pressure values from sensor...\n"); 224 225 _// I2C is "open drain", pull ups to keep signal high when no data is being sent_ 226     i2c_init(i2c_default, 100 * 1000); 227     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 228     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 229     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 230     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 231 232 _// configure BMP280_ 233     bmp280_init(); 234 235 _// retrieve fixed compensation params_ 236     struct bmp280_calib_param params; 237     bmp280_get_calib_params(&params); 238 239     int32_t raw_temperature; 240     int32_t raw_pressure; 241 242     sleep_ms(250); _// sleep so that data polling and register update don't collide_ 243     while (1) { 244         bmp280_read_raw(&raw_temperature, &raw_pressure); 245         int32_t temperature = bmp280_convert_temp(raw_temperature, &params); 246         int32_t pressure = bmp280_convert_pressure(raw_pressure, raw_temperature, &params); 247         printf("Pressure = %.3f kPa\n", pressure / 1000.f); 248         printf("Temp. = %.2f C\n", temperature / 100.f); 249 _// poll every 500ms_ 250         sleep_ms(500); 251     } 252 _#endif_ 253 }

## **Bill of Materials**

||**Bill of Materials**|||
|---|---|---|---|
|_Table 46. A list of_<br>_materials required for_<br>_the example_|**Item**|**Quantity**|Details|
||Breadboard|1|generic part|
||Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
||BMP280-based breakout board|1|from Pimoroni|
||M/M Jumper wires|4|generic part|



## **Attaching a LIS3DH Nano Accelerometer via i2c.**

This example shows you how to interface the Raspberry Pi Pico to the LIS3DH accelerometer and temperature sensor.

The code reads and displays the acceleration values of the board in the 3 axes and the ambient temperature value. The datasheet for the sensor can be found at https://www.st.com/resource/en/datasheet/cd00274221.pdf. The device is being operated on 'normal mode' and at a frequency of 1.344 kHz (this can be changed by editing the ODR bits of CTRL_REG4). The range of the data is controlled by the FS bit in CTRL_REG4 and is equal to ±2g in this example. The sensitivity depends on the operating mode and data range; exact values can be found on page 10 of the datasheet. In

Attaching a LIS3DH Nano Accelerometer via i2c.

**628**

Raspberry Pi Pico-series C/C++ SDK

this case, the sensitivity value is 4mg (where g is the value of gravitational acceleration on the surface of Earth). In order to use the auxiliary ADC to read temperature, the we must set the BDU bit to 1 in CTRL_REG4 and the ADC_EN bit to 1 in TEMP_CFG_REG. Temperature is communicated through ADC 3.

##  **NOTE**

The sensor doesn’t have features to eliminate offsets in the data and these will need to be taken into account in the code.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VIN, GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 3V pin.

_Figure 20. Wiring Diagram for LIS3DH._

**==> picture [319 x 221] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/lis3dh_i2c/CMakeLists.txt_

1 add_executable(lis3dh_i2c 2         lis3dh_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(lis3dh_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(lis3dh_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(lis3dh_i2c)

Attaching a LIS3DH Nano Accelerometer via i2c.

**629**

Raspberry Pi Pico-series C/C++ SDK

## **lis3dh_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/lis3dh_i2c/lis3dh_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/i2c.h"_ 12 13 _/* Example code to talk to a LIS3DH Triple Axis Accelerometer_ 14 15 _This example reads data from all 3 axes of the accelerometer and uses an auxiliary ADC to output temperature values._ 16 17 _Connections on Raspberry Pi Pico board, other boards may vary._ 18 19 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is 4 (physical pin 6)) -> SDA on LIS3DH board_ 20 _GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is 5 (physical pin 7)) -> SCL on LIS3DH board_ 21 _3.3v (physical pin 36) -> VIN on LIS3DH board_ 22 _GND (physical pin 38)  -> GND on LIS3DH board_ 23 _*/_ 24 25 _// By default this device is on bus address 0x18. If this doesn't work, try 0x19._ 26 27 const int ADDRESS = 0x18; 28 const uint8_t CTRL_REG_1 = 0x20; 29 const uint8_t CTRL_REG_4 = 0x23; 30 const uint8_t TEMP_CFG_REG = 0x1F; 31 32 _#ifdef i2c_default_ 33 34 void lis3dh_init() { 35     uint8_t buf[2]; 36 37 _// Turn normal mode and 1.344kHz data rate on_ 38     buf[0] = CTRL_REG_1; 39     buf[1] = 0x97; 40     i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false); 41 42 _// Turn block data update on (for temperature sensing)_ 43     buf[0] = CTRL_REG_4; 44     buf[1] = 0x80; 45     i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false); 46 47 _// Turn auxiliary ADC on_ 48     buf[0] = TEMP_CFG_REG; 49     buf[1] = 0xC0; 50     i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false); 51 } 52 53 void lis3dh_calc_value(uint16_t raw_value, float *final_value, bool isAccel) { 54 _// Convert with respect to the value being temperature or acceleration reading_ 55     float scaling; 56     float senstivity = 0.004f; _// g per unit_ 57

Attaching a LIS3DH Nano Accelerometer via i2c.

**630**

Raspberry Pi Pico-series C/C++ SDK

58     if (isAccel == true) { 59         scaling = 64 / senstivity; 60     } else { 61         scaling = 64; 62     } 63 64 _// raw_value is signed_ 65     *final_value = (float) ((int16_t) raw_value) / scaling; 66 } 67 68 void lis3dh_read_data(uint8_t reg, float *final_value, bool IsAccel) { 69 _// Read two bytes of data and store in a 16 bit data structure_ 70     uint8_t lsb; 71     uint8_t msb; 72     uint16_t raw_accel; 73     i2c_write_blocking(i2c_default, ADDRESS, &reg, 1, true); 74     i2c_read_blocking(i2c_default, ADDRESS, &lsb, 1, false); 75 76     reg |= 0x01; 77     i2c_write_blocking(i2c_default, ADDRESS, &reg, 1, true); 78     i2c_read_blocking(i2c_default, ADDRESS, &msb, 1, false); 79 80     raw_accel = (msb << 8) | lsb; 81 82     lis3dh_calc_value(raw_accel, final_value, IsAccel); 83 } 84 85 _#endif_ 86 87 int main() { 88     stdio_init_all(); 89 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 90 _#warning i2c/lis3dh_i2c example requires a board with I2C pins_ 91     puts("Default I2C pins were not defined"); 92 _#else_ 93     printf("Hello, LIS3DH! Reading raw data from registers...\n"); 94 95 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 96     i2c_init(i2c_default, 400 * 1000); 97     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 98     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 99     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 100     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 101 _// Make the I2C pins available to picotool_ 102     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 103 104     float x_accel, y_accel, z_accel, temp; 105 106     lis3dh_init(); 107 108     while (1) { 109         lis3dh_read_data(0x28, &x_accel, true); 110         lis3dh_read_data(0x2A, &y_accel, true); 111         lis3dh_read_data(0x2C, &z_accel, true); 112         lis3dh_read_data(0x0C, &temp, false); 113 114 _// Display data_ 115         printf("TEMPERATURE: %.3f%cC\n", temp, 176); 116 _// Acceleration is read as a multiple of g (gravitational acceleration on the Earth's surface)_ 117         printf("ACCELERATION VALUES: \n"); 118         printf("X acceleration: %.3fg\n", x_accel);

Attaching a LIS3DH Nano Accelerometer via i2c.

**631**

Raspberry Pi Pico-series C/C++ SDK

119         printf("Y acceleration: %.3fg\n", y_accel); 120         printf("Z acceleration: %.3fg\n", z_accel); 121 122         sleep_ms(500); 123 124 _// Clear terminal_ 125         printf("\033[1;1H\033[2J"); 126     } 127 _#endif_ 128 }

## **Bill of Materials**

_Table 47. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|LIS3DH board|1|https://www.adafruit.com/product/<br>2809|
|M/M Jumper wires|4|generic part|



## **Attaching a MCP9808 digital temperature sensor via I2C**

This example code shows how to interface the Raspberry Pi Pico to the MCP9808 digital temperature sensor board.

This example reads the ambient temperature value each second from the sensor and sets upper, lower and critical limits for the temperature and checks if alerts need to be raised. The CONFIG register can also be used to check for an alert if the critical temperature is surpassed.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VDD, GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the VSYS pin.

Attaching a MCP9808 digital temperature sensor via I2C

**632**

Raspberry Pi Pico-series C/C++ SDK

_Figure 21. Wiring Diagram for MCP9808._

**==> picture [319 x 221] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mcp9808_i2c/CMakeLists.txt_

1 add_executable(mcp9808_i2c 2         mcp9808_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(mcp9808_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(mcp9808_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(mcp9808_i2c)

## **mcp9808_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mcp9808_i2c/mcp9808_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/i2c.h"_ 12 13 _/* Example code to talk to a MCP9808 ±0.5°C Digital temperature Sensor_

Attaching a MCP9808 digital temperature sensor via I2C

**633**

Raspberry Pi Pico-series C/C++ SDK

14 15 _This reads and writes to registers on the board._ 16 17 _Connections on Raspberry Pi Pico board, other boards may vary._ 18 19 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is GP4 (physical pin 6)) -> SDA on MCP9808 board_ 20 _GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is GP5 (physical pin 7)) -> SCL on MCP9808 board_ 21 _Vsys (physical pin 39) -> VDD on MCP9808 board_ 22 _GND (physical pin 38)  -> GND on MCP9808 board_ 23 24 _*/_ 25 _//The bus address is determined by the state of pins A0, A1 and A2 on the MCP9808 board_ 26 static uint8_t ADDRESS = 0x18; 27 28 _//hardware registers_ 29 30 const uint8_t REG_POINTER = 0x00; 31 const uint8_t REG_CONFIG = 0x01; 32 const uint8_t REG_TEMP_UPPER = 0x02; 33 const uint8_t REG_TEMP_LOWER = 0x03; 34 const uint8_t REG_TEMP_CRIT = 0x04; 35 const uint8_t REG_TEMP_AMB = 0x05; 36 const uint8_t REG_RESOLUTION = 0x08; 37 38 39 void mcp9808_check_limits(uint8_t upper_byte) { 40 41 _// Check flags and raise alerts accordingly_ 42     if ((upper_byte & 0x40) == 0x40) { _//TA > TUPPER_ 43         printf("Temperature is above the upper temperature limit.\n"); 44     } 45     if ((upper_byte & 0x20) == 0x20) { _//TA < TLOWER_ 46         printf("Temperature is below the lower temperature limit.\n"); 47     } 48     if ((upper_byte & 0x80) == 0x80) { _//TA > TCRIT_ 49         printf("Temperature is above the critical temperature limit.\n"); 50     } 51 } 52 53 float mcp9808_convert_temp(uint8_t upper_byte, uint8_t lower_byte) { 54 55     float temperature; 56 57 58 _//Check if TA <= 0°C and convert to denary accordingly_ 59     if ((upper_byte & 0x10) == 0x10) { 60         upper_byte = upper_byte & 0x0F; 61         temperature = 256 - (((float) upper_byte * 16) + ((float) lower_byte / 16)); 62     } else { 63         temperature = (((float) upper_byte * 16) + ((float) lower_byte / 16)); 64 65     } 66     return temperature; 67 } 68 69 _#ifdef i2c_default_ 70 void mcp9808_set_limits() { 71 72 _//Set an upper limit of 30°C for the temperature_ 73     uint8_t upper_temp_msb = 0x01; 74     uint8_t upper_temp_lsb = 0xE0; 75

Attaching a MCP9808 digital temperature sensor via I2C

**634**

Raspberry Pi Pico-series C/C++ SDK

76 _//Set a lower limit of 20°C for the temperature_ 77     uint8_t lower_temp_msb = 0x01; 78     uint8_t lower_temp_lsb = 0x40; 79 80 _//Set a critical limit of 40°C for the temperature_ 81     uint8_t crit_temp_msb = 0x02; 82     uint8_t crit_temp_lsb = 0x80; 83 84     uint8_t buf[3]; 85     buf[0] = REG_TEMP_UPPER; 86     buf[1] = upper_temp_msb; 87     buf[2] = upper_temp_lsb; 88     i2c_write_blocking(i2c_default, ADDRESS, buf, 3, false); 89 90     buf[0] = REG_TEMP_LOWER; 91     buf[1] = lower_temp_msb; 92     buf[2] = lower_temp_lsb; 93     i2c_write_blocking(i2c_default, ADDRESS, buf, 3, false); 94 95     buf[0] = REG_TEMP_CRIT; 96     buf[1] = crit_temp_msb; 97     buf[2] = crit_temp_lsb;; 98     i2c_write_blocking(i2c_default, ADDRESS, buf, 3, false); 99 } 100 _#endif_ 101 102 int main() { 103 104     stdio_init_all(); 105 106 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 107 _#warning i2c/mcp9808_i2c example requires a board with I2C pins_ 108     puts("Default I2C pins were not defined"); 109 _#else_ 110     printf("Hello, MCP9808! Reading raw data from registers...\n"); 111 112 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 113     i2c_init(i2c_default, 400 * 1000); 114     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 115     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 116     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 117     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 118 _// Make the I2C pins available to picotool_ 119     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 120 121     mcp9808_set_limits(); 122 123     uint8_t buf[2]; 124     uint16_t upper_byte; 125     uint16_t lower_byte; 126 127     float temperature; 128 129     while (1) { 130 _// Start reading ambient temperature register for 2 bytes_ 131         i2c_write_blocking(i2c_default, ADDRESS, &REG_TEMP_AMB, 1, true); 132         i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false); 133 134         upper_byte = buf[0]; 135         lower_byte = buf[1]; 136 137 _//isolates limit flags in upper byte_

Attaching a MCP9808 digital temperature sensor via I2C

**635**

Raspberry Pi Pico-series C/C++ SDK

138         mcp9808_check_limits(upper_byte & 0xE0); 139 140 _//clears flag bits in upper byte_ 141         temperature = mcp9808_convert_temp(upper_byte & 0x1F, lower_byte); 142         printf("Ambient temperature: %.4f°C\n", temperature); 143 144         sleep_ms(1000); 145     } 146 _#endif_ 147 }

## **Bill of Materials**

_Table 48. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|MCP9808 board|1|https://www.adafruit.com/product/<br>1782|
|M/M Jumper wires|4|generic part|



## **Attaching a MMA8451 3-axis digital accelerometer via I2C**

This example code shows how to interface the Raspberry Pi Pico to the MMA8451 digital accelerometer sensor board.

This example reads and displays the acceleration values of the board in the 3 axis. It also allows the user to set the trade-off between the range and precision based on the values they require. Values often have an offset which can be accounted for by writing to the offset correction registers. The datasheet for the sensor can be found at https://cdnshop.adafruit.com/datasheets/MMA8451Q-1.pdf for additional information.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VIN, GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the VSYS pin.

Attaching a MMA8451 3-axis digital accelerometer via I2C

**636**

Raspberry Pi Pico-series C/C++ SDK

_Figure 22. Wiring Diagram for MMA8451._

**==> picture [319 x 221] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mma8451_i2c/CMakeLists.txt_

1 add_executable(mma8451_i2c 2         mma8451_i2c.c 3         ) 4 # pull in common dependencies and additional i2c hardware support 5 target_link_libraries(mma8451_i2c pico_stdlib hardware_i2c) 6 7 # create map/bin/hex file etc. 8 pico_add_extra_outputs(mma8451_i2c) 9 10 # add url via pico_set_program_url 11 example_auto_set_url(mma8451_i2c)

## **mma8451_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mma8451_i2c/mma8451_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/i2c.h"_ 12 13 _/* Example code to talk to a MMA8451 triple-axis accelerometer._ 14

Attaching a MMA8451 3-axis digital accelerometer via I2C

**637**

Raspberry Pi Pico-series C/C++ SDK

15 _This reads and writes to registers on the board._ 16 17 _Connections on Raspberry Pi Pico board, other boards may vary._ 18 19 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is GP4 (physical pin 6)) -> SDA on MMA8451 board_ 20 _GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is GP5 (physical pin 7)) -> SCL on MMA8451 board_ 21 _VSYS (physical pin 39) -> VDD on MMA8451 board_ 22 _GND (physical pin 38)  -> GND on MMA8451 board_ 23 24 _*/_ 25 26 const uint8_t ADDRESS = 0x1D; 27 28 _//hardware registers_ 29 30 const uint8_t REG_X_MSB = 0x01; 31 const uint8_t REG_X_LSB = 0x02; 32 const uint8_t REG_Y_MSB = 0x03; 33 const uint8_t REG_Y_LSB = 0x04; 34 const uint8_t REG_Z_MSB = 0x05; 35 const uint8_t REG_Z_LSB = 0x06; 36 const uint8_t REG_DATA_CFG = 0x0E; 37 const uint8_t REG_CTRL_REG1 = 0x2A; 38 39 _// Set the range and precision for the data_ 40 const uint8_t range_config = 0x01; _// 0x00 for ±2g, 0x01 for ±4g, 0x02 for ±8g_ 41 const float count = 2048; _// 4096 for ±2g, 2048 for ±4g, 1024 for ±8g_ 42 43 uint8_t buf[2]; 44 45 float mma8451_convert_accel(uint16_t raw_accel) { 46     float acceleration; 47 _// Acceleration is read as a multiple of g (gravitational acceleration on the Earth's surface)_ 48 _// Check if acceleration < 0 and convert to decimal accordingly_ 49     if ((raw_accel & 0x2000) == 0x2000) { 50         raw_accel &= 0x1FFF; 51         acceleration = (-8192 + (float) raw_accel) / count; 52     } else { 53         acceleration = (float) raw_accel / count; 54     } 55     acceleration *= 9.81f; 56     return acceleration; 57 } 58 59 _#ifdef i2c_default_ 60 void mma8451_set_state(uint8_t state) { 61     buf[0] = REG_CTRL_REG1; 62     buf[1] = state; _// Set RST bit to 1_ 63     i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false); 64 } 65 _#endif_ 66 67 int main() { 68     stdio_init_all(); 69 70 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 71 _#warning i2c/mma8451_i2c example requires a board with I2C pins_ 72     puts("Default I2C pins were not defined"); 73 _#else_ 74     printf("Hello, MMA8451! Reading raw data from registers...\n");

Attaching a MMA8451 3-axis digital accelerometer via I2C

**638**

Raspberry Pi Pico-series C/C++ SDK

75 76 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 77     i2c_init(i2c_default, 400 * 1000); 78     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 79     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 80     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 81     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 82 _// Make the I2C pins available to picotool_ 83     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 84 85     float x_acceleration; 86     float y_acceleration; 87     float z_acceleration; 88 89 _// Enable standby mode_ 90     mma8451_set_state(0x00); 91 92 _// Edit configuration while in standby mode_ 93     buf[0] = REG_DATA_CFG; 94     buf[1] = range_config; 95     i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false); 96 97 _// Enable active mode_ 98     mma8451_set_state(0x01); 99 100     while (1) { 101 102 _// Start reading acceleration registers for 2 bytes_ 103         i2c_write_blocking(i2c_default, ADDRESS, &REG_X_MSB, 1, true); 104         i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false); 105         x_acceleration = mma8451_convert_accel(buf[0] << 6 | buf[1] >> 2); 106 107         i2c_write_blocking(i2c_default, ADDRESS, &REG_Y_MSB, 1, true); 108         i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false); 109         y_acceleration = mma8451_convert_accel(buf[0] << 6 | buf[1] >> 2); 110 111         i2c_write_blocking(i2c_default, ADDRESS, &REG_Z_MSB, 1, true); 112         i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false); 113         z_acceleration = mma8451_convert_accel(buf[0] << 6 | buf[1] >> 2); 114 115 _// Display acceleration values_ 116         printf("ACCELERATION VALUES: \n"); 117         printf("X acceleration: %.6fms^-2\n", x_acceleration); 118         printf("Y acceleration: %.6fms^-2\n", y_acceleration); 119         printf("Z acceleration: %.6fms^-2\n", z_acceleration); 120 121         sleep_ms(500); 122 123 _// Clear terminal_ 124         printf("\033[1;1H\033[2J"); 125     } 126 127 _#endif_ 128 }

## **Bill of Materials**

Attaching a MMA8451 3-axis digital accelerometer via I2C

**639**

Raspberry Pi Pico-series C/C++ SDK

_Table 49. A list of materials required for the example_

|**Item**|**Quantity**|Details|
|---|---|---|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|MMA8451 board|1|https://www.adafruit.com/product/<br>2019|
|M/M Jumper wires|4|generic part|



## **Attaching an MPL3115A2 altimeter via I2C**

This example code shows how to interface the Raspberry Pi Pico to an MPL3115A2 altimeter via I2C. The MPL3115A2 has onboard pressure and temperature sensors which are used to estimate the altitude. In comparison to the BMPfamily of pressure and temperature sensors, the MPL3115A2 has two interrupt pins for ultra low power operation and takes care of the sensor reading compensation on the board! It also has multiple modes of operation and impressive operating conditions.

The board used in this example comes from Adafruit, but any MPL3115A2 breakouts should work similarly.

The MPL3115A2 makes available two ways of reading its temperature and pressure data. The first is known as polling, where the Pico will continuously read data out of a set of auto-incrementing registers which are refreshed with new data every so often. The second, which this example will demonstrate, uses a 160-byte first-in-first-out (FIFO) queue and configurable interrupts to tell the Pico when to read data. More information regarding when the interrupts can be triggered is available at in the datasheet. This example waits for the 32 sample FIFO to overflow, detects this via an interrupt pin, and then averages the 32 samples taken. The sensor is configured to take a sample every second.

Bit math is used to convert the temperature and altitude data from the raw bits collected in the registers. Take the temperature calculation as an example: it is a 12-bit signed number with 8 integer bits and 4 fractional bits. First, we read the 2 8-bit registers and store them in a buffer. Then, we concatenate them into one unsigned 16-bit integer starting with the OUT_T_MSB register, thus making sure that the last bit of this register is aligned with the MSB in our 16 bit unsigned integer so it is correctly interpreted as the signed bit when we later cast this to a signed 16-bit integer. Finally, the entire number is converted to a float implicitly when we multiply it by 1/2^8 to shift it 8 bits to the right of the decimal point. Though only the last 4 bits of the OUT_T_LSB register hold data, this does not matter as the remaining 4 are held at zero and "disappear" when we shift the decimal point left by 8. Similar logic is applied to the altitude calculation.

##  **TIP**

Choosing the right sensor for your project among so many choices can be hard! There are multiple factors you may have to consider in addition to any constraints imposed on you. Cost, operating temperature, sensor resolution, power consumption, ease of use, communication protocols and supply voltage are all but a few factors that can play a role in sensor choice. For most hobbyist purposes though, the majority of sensors out there will do just fine!

## **Wiring information**

Wiring up the device requires 5 jumpers, to connect VCC (3.3v), GND, INT1, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and GPIO 5 (SCL) by default. Power is supplied from the 3.3V pin.

Attaching an MPL3115A2 altimeter via I2C

**640**

Raspberry Pi Pico-series C/C++ SDK

##  **NOTE**

The MPL3115A2 has a 1.6-3.6V voltage supply range. This means it can work with the Pico’s 3.3v pins out of the box but our Adafruit breakout has an onboard voltage regulator for good measure. This may not always be true of other sensors, though.

_Figure 23. Wiring Diagram for MPL3115A2 altimeter._

**==> picture [319 x 230] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mpl3115a2_i2c/CMakeLists.txt_

1 add_executable(mpl3115a2_i2c 2         mpl3115a2_i2c.c 3         ) 4 5 target_include_directories(mpl3115a2_i2c PUBLIC 6     ${CMAKE_CURRENT_SOURCE_DIR} 7 ) 8 9 target_link_libraries(mpl3115a2_i2c 10     pico_stdlib 11     hardware_i2c 12 ) 13 14 # pull in common dependencies and additional i2c hardware support 15 target_link_libraries(mpl3115a2_i2c pico_stdlib hardware_i2c) 16 17 # create map/bin/hex file etc. 18 pico_add_extra_outputs(mpl3115a2_i2c) 19 20 # add url via pico_set_program_url 21 example_auto_set_url(mpl3115a2_i2c)

Attaching an MPL3115A2 altimeter via I2C

**641**

Raspberry Pi Pico-series C/C++ SDK

## **mpl3115a2_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/mpl3115a2_i2c/mpl3115a2_i2c.c_

1 _/**_ 2 _* Copyright (c) 2021 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include "pico/stdlib.h"_ 9 _#include "pico/binary_info.h"_ 10 _#include "hardware/gpio.h"_ 11 _#include "hardware/i2c.h"_ 12 _#include "mpl3115a2_i2c.h"_ 13 14 _/* Example code to talk to an MPL3115A2 altimeter sensor via I2C_ 15 16 _See accompanying documentation in README.adoc or the C++ SDK booklet._ 17 18 _Connections on Raspberry Pi Pico board, other boards may vary._ 19 20 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is 4 (pin 6)) -> SDA on MPL3115A2 board_ 21 _GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is 5 (pin 7)) -> SCL on MPL3115A2 board_ 22 _GPIO 16 -> INT1 on MPL3115A2 board_ 23 _3.3v (pin 36) -> VCC on MPL3115A2 board_ 24 _GND (pin 38)  -> GND on MPL3115A2 board_ 25 _*/_ 26 27 _// 7-bit address_ 28 _#define ADDR 0x60_ 29 _#define INT1_PIN _u(16)_ 30 31 _// following definitions only valid for F_MODE > 0 (ie. if FIFO enabled)_ 32 _#define MPL3115A2_F_DATA _u(0x01)_ 33 _#define MPL3115A2_F_STATUS _u(0x00)_ 34 _#define MPL3115A2_F_SETUP _u(0x0F)_ 35 _#define MPL3115A2_INT_SOURCE _u(0x12)_ 36 _#define MPL3115A2_CTRLREG1 _u(0x26)_ 37 _#define MPL3115A2_CTRLREG2 _u(0x27)_ 38 _#define MPL3115A2_CTRLREG3 _u(0x28)_ 39 _#define MPL3115A2_CTRLREG4 _u(0x29)_ 40 _#define MPL3115A2_CTRLREG5 _u(0x2A)_ 41 _#define MPL3115A2_PT_DATA_CFG _u(0x13)_ 42 _#define MPL3115A2_OFF_P _u(0x2B)_ 43 _#define MPL3115A2_OFF_T _u(0x2C)_ 44 _#define MPL3115A2_OFF_H _u(0x2D)_ 45 46 _/*** Sea-level pressure registers ***/_ 47 _#define MPL3115A2_BAR_IN_MSB _u(0x14)_ 48 _#define MPL3115A2_BAR_IN_LSB _u(0x15)_ 49 50 51 _#define MPL3115A2_FIFO_DISABLED _u(0x00)_ 52 _#define MPL3115A2_FIFO_STOP_ON_OVERFLOW _u(0x80)_ 53 _#define MPL3115A2_FIFO_SIZE 32_ 54 _#define MPL3115A2_DATA_BATCH_SIZE 5_ 55 _#define MPL3115A2_ALTITUDE_NUM_REGS 3_ 56 _#define MPL3115A2_ALTITUDE_INT_SIZE 20_ 57 _#define MPL3115A2_TEMPERATURE_INT_SIZE 12_ 58 _#define MPL3115A2_NUM_FRAC_BITS 4_

Attaching an MPL3115A2 altimeter via I2C

**642**

Raspberry Pi Pico-series C/C++ SDK

59 60 _#define PARAM_ASSERTIONS_ENABLE_I2C 1_ 61 62 volatile uint8_t fifo_data[MPL3115A2_FIFO_SIZE * MPL3115A2_DATA_BATCH_SIZE]; 63 volatile bool has_new_data = false; 64 65 66 _/*** Sea-level pressure functions ***/_ 67 _// Set sea-level pressure in hectopascals (hPa)_ 68 void mpl3115a2_set_sealevel_pressure(float hPa) { 69     uint16_t bars = (uint16_t)(hPa * 50); _// Convert hPa to BAR_IN value (2 Pa/LSB)_ 70     uint8_t buf[] = {MPL3115A2_BAR_IN_MSB, (bars >> 8) & 0xFF, bars & 0xFF}; 71     i2c_write_blocking(i2c_default, ADDR, buf, 3, false); 72 } 73 74 _// Get current sea-level pressure setting in hPa_ 75 float mpl3115a2_get_sealevel_pressure() { 76     uint8_t reg = MPL3115A2_BAR_IN_MSB; 77     uint8_t buf[2]; 78     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); 79     i2c_read_blocking(i2c_default, ADDR, buf, 2, false); 80     uint16_t bars = (buf[0] << 8) | buf[1]; 81     return (float)bars / 50.0f; _// Convert back to hPa_ 82 } 83 84 void copy_to_vbuf(uint8_t buf1[], volatile uint8_t buf2[], uint buflen) { 85     for (size_t i = 0; i < buflen; i++) { 86         buf2[i] = buf1[i]; 87     } 88 } 89 90 _#ifdef i2c_default_ 91 92 void mpl3115a2_read_fifo(volatile uint8_t fifo_buf[]) { 93 _// drains the 160 byte FIFO_ 94     uint8_t reg = MPL3115A2_F_DATA; 95     uint8_t buf[MPL3115A2_FIFO_SIZE * MPL3115A2_DATA_BATCH_SIZE]; 96     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); 97 _// burst read 160 bytes from fifo_ 98     i2c_read_blocking(i2c_default, ADDR, buf, MPL3115A2_FIFO_SIZE * MPL3115A2_DATA_BATCH_SIZE, false); 99     copy_to_vbuf(buf, fifo_buf, MPL3115A2_FIFO_SIZE * MPL3115A2_DATA_BATCH_SIZE); 100 } 101 102 uint8_t mpl3115a2_read_reg(uint8_t reg) { 103     uint8_t read; 104     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); _// keep control of bus_ 105     i2c_read_blocking(i2c_default, ADDR, &read, 1, false); 106     return read; 107 } 108 109 void mpl3115a2_init() { 110 _// set as altimeter with oversampling ratio of 128_ 111     uint8_t buf[] = {MPL3115A2_CTRLREG1, 0xB8}; 112     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 113 114 _// set data refresh every 2 seconds, 0 next bits as we're not using those interrupts_ 115     buf[0] = MPL3115A2_CTRLREG2, buf[1] = 0x00; 116     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 117 118 _// set both interrupts pins to active low and enable internal pullups_ 119     buf[0] = MPL3115A2_CTRLREG3, buf[1] = 0x01; 120     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 121

Attaching an MPL3115A2 altimeter via I2C

**643**

Raspberry Pi Pico-series C/C++ SDK

122 _// enable FIFO interrupt_ 123     buf[0] = MPL3115A2_CTRLREG4, buf[1] = 0x40; 124     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 125 126 _// tie FIFO interrupt to pin INT1_ 127     buf[0] = MPL3115A2_CTRLREG5, buf[1] = 0x40; 128     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 129 130 _/*** Default sea-level pressure (1013.25 hPa) ***/_ 131     mpl3115a2_set_sealevel_pressure(1013.25f); 132 133 _// set p, t and h offsets here if needed_ 134 _// eg. 2's complement number: 0xFF subtracts 1 meter_ 135 _//buf[0] = MPL3115A2_OFF_H, buf[1] = 0xFF;_ 136 _//i2c_write_blocking(i2c_default, ADDR, buf, 2, false);_ 137 138 _// do not accept more data on FIFO overflow_ 139     buf[0] = MPL3115A2_F_SETUP, buf[1] = MPL3115A2_FIFO_STOP_ON_OVERFLOW; 140     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 141 142 _// set device active_ 143     buf[0] = MPL3115A2_CTRLREG1, buf[1] = 0xB9; 144     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 145 } 146 147 void gpio_callback(uint gpio, __unused uint32_t events) { 148 _// if we had enabled more than 2 interrupts on same pin, then we should read_ 149 _// INT_SOURCE reg to find out which interrupt triggered_ 150 151 _// we can filter by which GPIO was triggered_ 152     if (gpio == INT1_PIN) { 153 _// FIFO overflow interrupt_ 154 _// watermark bits set to 0 in F_SETUP reg, so only possible event is an overflow_ 155 _// otherwise, we would read F_STATUS to confirm it was an overflow_ 156 _// drain the fifo_ 157         mpl3115a2_read_fifo(fifo_data); 158 _// read status register to clear interrupt bit_ 159         mpl3115a2_read_reg(MPL3115A2_F_STATUS); 160         has_new_data = true; 161     } 162 } 163 164 _#endif_ 165 166 void mpl3115a2_convert_fifo_batch(uint8_t start, volatile uint8_t buf[], struct mpl3115a2_data_t *data) { 167 _// convert a batch of fifo data into temperature and altitude data_ 168 169 _// 3 altitude registers: MSB (8 bits), CSB (8 bits) and LSB (4 bits, starting from MSB)_ 170 _// first two are integer bits (2's complement) and LSB is fractional bits -> makes 20 bit signed integer_ 171     int32_t h = (int32_t) buf[start] << 24; 172     h |= (int32_t) buf[start + 1] << 16; 173     h |= (int32_t) buf[start + 2] << 8; 174     data->altitude = ((float)h) / 65536.f; 175 176 _// 2 temperature registers: MSB (8 bits) and LSB (4 bits, starting from MSB)_ 177 _// first 8 are integer bits with sign and LSB is fractional bits -> 12 bit signed integer_ 178     int16_t t = (int16_t) buf[start + 3] << 8; 179     t |= (int16_t) buf[start + 4]; 180     data->temperature = ((float)t) / 256.f; 181 } 182 183 int main() {

Attaching an MPL3115A2 altimeter via I2C

**644**

Raspberry Pi Pico-series C/C++ SDK

184     stdio_init_all(); 185 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 186 _#warning i2c / mpl3115a2_i2c example requires a board with I2C pins_ 187     puts("Default I2C pins were not defined"); 188     return 0; 189 _#else_ 190     printf("Hello, MPL3115A2. Waiting for something to interrupt me!...\n"); 191 192 _// use default I2C0 at 400kHz, I2C is active low_ 193     i2c_init(i2c_default, 400 * 1000); 194     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 195     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 196     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 197     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 198 199     gpio_init(INT1_PIN); 200     gpio_pull_up(INT1_PIN); _// pull it up even more!_ 201 202 _// add program information for picotool_ 203     bi_decl(bi_program_name("Example in the pico-examples library for the MPL3115A2 altimeter")); 204     bi_decl(bi_1pin_with_name(16, "Interrupt pin 1")); 205     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 206 207     mpl3115a2_init(); 208 209 _// Uncomment to overwrite default sea-level pressure:_ 210 _// mpl3115a2_set_sealevel_pressure(1020.0f); // Local weather pressure_ 211 212     gpio_set_irq_enabled_with_callback(INT1_PIN, GPIO_IRQ_LEVEL_LOW, true, &gpio_callback); 213 214     while (1) { 215 _// as interrupt data comes in, let's print the 32 sample average_ 216         if (has_new_data) { 217             float tsum = 0, hsum = 0; 218             struct mpl3115a2_data_t data; 219             for (int i = 0; i < MPL3115A2_FIFO_SIZE; i++) { 220                 mpl3115a2_convert_fifo_batch(i * MPL3115A2_DATA_BATCH_SIZE, fifo_data, & data); 221                 tsum += data.temperature; 222                 hsum += data.altitude; 223             } 224             printf("%d sample average -> t: %.4f C, h: %.4f m\n", MPL3115A2_FIFO_SIZE, tsum / MPL3115A2_FIFO_SIZE, 225                    hsum / MPL3115A2_FIFO_SIZE); 226             mpl3115a2_get_sealevel_pressure(); _// Show current setting_ 227             has_new_data = false; 228         } 229         sleep_ms(10); 230     }; 231 232 _#endif_ 233 }

## **Bill of Materials**

Attaching an MPL3115A2 altimeter via I2C

**645**

Raspberry Pi Pico-series C/C++ SDK

_Table 50. A list of materials required for the example_

|**Item**|**Quantity**|Details|
|---|---|---|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|MPL3115A2 altimeter|1|Adafruit|
|M/M Jumper wires|5|generic part|



## **Attaching an OLED display via I2C**

This example code shows how to interface the Raspberry Pi Pico with an 128x32 OLED display board based on the SSD1306 display driver, datasheet here.

The code displays a series of small demo graphics; tiny raspberries that scroll horizontally, some text, and some line drawing, in the process showing you how to initialize the display, write to the entire display, write to only a portion of the display, configure scrolling, invert the display etc.

The SSD1306 is operated via a list of versatile commands (see datasheet) that allows the user to access all the capabilities of the driver. After sending a slave address, the data that follows can be either a command, flags to follow up a command or data to be written directly into the display’s RAM. A control byte is required for each write after the slave address so that the driver knows what type of data is being sent.

The example code supports displays of 32 pixel or 64 pixels high by 128 pixels wide by changing a define at the top of the code.

In the 32 vertical pixels case, the display is partitioned into 4 pages, each 8 pixels in height. In RAM, this looks roughly like:

| COL0 | COL1 | COL2 | COL3 |  ...  | COL126 | COL127 | PAGE 0 |      |      |      |      |       |        |        | PAGE 1 |      |      |      |      |       |        |        | PAGE 2 |      |      |      |      |       |        |        | PAGE 3 |      |      |      |      |       |        |        | --------------------------------------------------------------

Within each page, we have:

| COL0 | COL1 | COL2 | COL3 |  ...  | COL126 | COL127 | COM 0 |      |      |      |      |       |        |        | COM 1 |      |      |      |      |       |        |        | :  |      |      |      |      |       |        |        | COM 7 |      |      |      |      |       |        |        | -------------------------------------------------------------

Attaching an OLED display via I2C

**646**

Raspberry Pi Pico-series C/C++ SDK

##  **NOTE**

There is a difference between columns in RAM and the actual segment pads that connect the driver to the display. The RAM addresses COL0 - COL127 are mapped to these segment pins SEG0 - SEG127 by default. The distinction between these two is important as we can for example, easily mirror contents of RAM without rewriting a buffer.

The driver has 3 modes of transferring the pixels in RAM to the display (provided that the driver is set to use its RAM content to drive the display, ie. command 0xA4 is sent). We choose horizontal addressing mode which, after setting the column address and page address registers to our desired start positions, will increment the column address register until the OLED display width is reached (127 in our case) after which the column address register will reset to its starting value and the page address is incremented. Once the page register reaches the end, it will wrap around as well. Effectively, this scans across the display from top to bottom, left to right in blocks that are 8 pixels high. When a byte is sent to be written into RAM, it sets all the rows for the current position of the column address register. So, if we send 10101010, and we are on PAGE 0 and COL1, COM0 is set to 1, COM1 is set to 0, COM2 is set to 1, and so on. Effectively, the byte is "transposed" to fill a single page’s column. The datasheet has further information on this and the two other modes.

Horizontal addressing mode has the key advantage that we can keep one single 512 byte buffer (128 columns x 4 pages and each byte fills a page’s rows) and write this in one go to the RAM (column address auto increments on writes as well as reads) instead of working with 2D matrices of pixels and adding more overhead.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VCC (3.3v), GND, SDA and SCL and optionally a 5th jumper for the driver RESET pin. The example here uses the default I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 3.3V pin from the Pico.

_Figure 24. Wiring Diagram for oled display via I2C._

**==> picture [319 x 247] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example into the examples build tree.

Attaching an OLED display via I2C

**647**

Raspberry Pi Pico-series C/C++ SDK

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/ssd1306_i2c/CMakeLists.txt_

1 add_executable(ssd1306_i2c 2         ssd1306_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(ssd1306_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(ssd1306_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(ssd1306_i2c)

## **ssd1306_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/ssd1306_i2c/ssd1306_i2c.c_

1 _/**_ 2 _* Copyright (c) 2021 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include <stdlib.h>_ 10 _#include <ctype.h>_ 11 _#include "pico/stdlib.h"_ 12 _#include "pico/binary_info.h"_ 13 _#include "hardware/i2c.h"_ 14 _#include "raspberry26x32.h"_ 15 _#include "ssd1306_font.h"_ 16 17 _/* Example code to talk to an SSD1306-based OLED display_ 18 19 _The SSD1306 is an OLED/PLED driver chip, capable of driving displays up to_ 20 _128x64 pixels._ 21 22 _NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico_ 23 _GPIO (and therefore I2C) cannot be used at 5v._ 24 25 _You will need to use a level shifter on the I2C lines if you want to run the_ 26 _board at 5v._ 27 28 _Connections on Raspberry Pi Pico board, other boards may vary._ 29 30 _GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on display_ 31 _board_ 32 _GPIO PICO_DEFAULT_I2C_SCL_PIN (on Pico this is GP5 (pin 7)) -> SCL on_ 33 _display board_ 34 _3.3v (pin 36) -> VCC on display board_ 35 _GND (pin 38)  -> GND on display board_ 36 _*/_ 37 38 _// Define the size of the display we have attached. This can vary, make sure you_ 39 _// have the right size defined or the output will look rather odd!_ 40 _// Code has been tested on 128x32 and 128x64 OLED displays_ 41 _#define SSD1306_HEIGHT              32_

Attaching an OLED display via I2C

**648**

Raspberry Pi Pico-series C/C++ SDK

42 _#define SSD1306_WIDTH               128_ 43 44 _#define SSD1306_I2C_ADDR            _u(0x3C)_ 45 46 _// 400 is usual, but often these can be overclocked to improve display response._ 47 _// Tested at 1000 on both 32 and 84 pixel height devices and it worked._ 48 _#define SSD1306_I2C_CLK             400_ 49 _//#define SSD1306_I2C_CLK             1000_ 50 51 52 _// commands (see datasheet)_ 53 _#define SSD1306_SET_MEM_MODE        _u(0x20)_ 54 _#define SSD1306_SET_COL_ADDR        _u(0x21)_ 55 _#define SSD1306_SET_PAGE_ADDR       _u(0x22)_ 56 _#define SSD1306_SET_HORIZ_SCROLL    _u(0x26)_ 57 _#define SSD1306_SET_SCROLL          _u(0x2E)_ 58 59 _#define SSD1306_SET_DISP_START_LINE _u(0x40)_ 60 61 _#define SSD1306_SET_CONTRAST        _u(0x81)_ 62 _#define SSD1306_SET_CHARGE_PUMP     _u(0x8D)_ 63 64 _#define SSD1306_SET_SEG_REMAP       _u(0xA0)_ 65 _#define SSD1306_SET_ENTIRE_ON       _u(0xA4)_ 66 _#define SSD1306_SET_ALL_ON          _u(0xA5)_ 67 _#define SSD1306_SET_NORM_DISP       _u(0xA6)_ 68 _#define SSD1306_SET_INV_DISP        _u(0xA7)_ 69 _#define SSD1306_SET_MUX_RATIO       _u(0xA8)_ 70 _#define SSD1306_SET_DISP            _u(0xAE)_ 71 _#define SSD1306_SET_COM_OUT_DIR     _u(0xC0)_ 72 _#define SSD1306_SET_COM_OUT_DIR_FLIP _u(0xC0)_ 73 74 _#define SSD1306_SET_DISP_OFFSET     _u(0xD3)_ 75 _#define SSD1306_SET_DISP_CLK_DIV    _u(0xD5)_ 76 _#define SSD1306_SET_PRECHARGE       _u(0xD9)_ 77 _#define SSD1306_SET_COM_PIN_CFG     _u(0xDA)_ 78 _#define SSD1306_SET_VCOM_DESEL      _u(0xDB)_ 79 80 _#define SSD1306_PAGE_HEIGHT         _u(8)_ 81 _#define SSD1306_NUM_PAGES           (SSD1306_HEIGHT / SSD1306_PAGE_HEIGHT)_ 82 _#define SSD1306_BUF_LEN             (SSD1306_NUM_PAGES * SSD1306_WIDTH)_ 83 84 _#define SSD1306_WRITE_MODE         _u(0xFE)_ 85 _#define SSD1306_READ_MODE          _u(0xFF)_ 86 87 88 struct render_area { 89     uint8_t start_col; 90     uint8_t end_col; 91     uint8_t start_page; 92     uint8_t end_page; 93 94     int buflen; 95 }; 96 97 void calc_render_area_buflen(struct render_area *area) { 98 _// calculate how long the flattened buffer will be for a render area_ 99     area->buflen = (area->end_col - area->start_col + 1) * (area->end_page - area>start_page + 1); 100 } 101 102 _#ifdef i2c_default_ 103 104 void SSD1306_send_cmd(uint8_t cmd) {

Attaching an OLED display via I2C

**649**

Raspberry Pi Pico-series C/C++ SDK

105 _// I2C write process expects a control byte followed by data_ 106 _// this "data" can be a command or data to follow up a command_ 107 _// Co = 1, D/C = 0 => the driver expects a command_ 108     uint8_t buf[2] = {0x80, cmd}; 109     i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, buf, 2, false); 110 } 111 112 void SSD1306_send_cmd_list(uint8_t *buf, int num) { 113     for (int i=0;i<num;i++) 114         SSD1306_send_cmd(buf[i]); 115 } 116 117 void SSD1306_send_buf(uint8_t buf[], int buflen) { 118 _// in horizontal addressing mode, the column address pointer auto-increments_ 119 _// and then wraps around to the next page, so we can send the entire frame_ 120 _// buffer in one gooooooo!_ 121 122 _// copy our frame buffer into a new buffer because we need to add the control byte_ 123 _// to the beginning_ 124 125     uint8_t *temp_buf = malloc(buflen + 1); 126 127     temp_buf[0] = 0x40; 128     memcpy(temp_buf+1, buf, buflen); 129 130     i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, temp_buf, buflen + 1, false); 131 132     free(temp_buf); 133 } 134 135 void SSD1306_init() { 136 _// Some of these commands are not strictly necessary as the reset_ 137 _// process defaults to some of these but they are shown here_ 138 _// to demonstrate what the initialization sequence looks like_ 139 _// Some configuration values are recommended by the board manufacturer_ 140 141     uint8_t cmds[] = { 142         SSD1306_SET_DISP, _// set display off_ 143 _/* memory mapping */_ 144         SSD1306_SET_MEM_MODE, _// set memory address mode 0 = horizontal, 1 = vertical, 2 = page_ 145         0x00, _// horizontal addressing mode_ 146 _/* resolution and layout */_ 147         SSD1306_SET_DISP_START_LINE, _// set display start line to 0_ 148         SSD1306_SET_SEG_REMAP | 0x01, _// set segment re-map, column address 127 is mapped to SEG0_ 149         SSD1306_SET_MUX_RATIO, _// set multiplex ratio_ 150         SSD1306_HEIGHT - 1, _// Display height - 1_ 151         SSD1306_SET_COM_OUT_DIR | 0x08, _// set COM (common) output scan direction. Scan from bottom up, COM[N-1] to COM0_ 152         SSD1306_SET_DISP_OFFSET, _// set display offset_ 153         0x00, _// no offset_ 154         SSD1306_SET_COM_PIN_CFG, _// set COM (common) pins hardware configuration. Board specific magic number._ 155 _// 0x02 Works for 128x32, 0x12 Possibly works for 128x64. Other options 0x22, 0x32_ 156 _#if ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 32))_ 157         0x02, 158 _#elif ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 64))_ 159         0x12, 160 _#else_ 161         0x02, 162 _#endif_ 163 _/* timing and driving scheme */_

Attaching an OLED display via I2C

**650**

Raspberry Pi Pico-series C/C++ SDK

164         SSD1306_SET_DISP_CLK_DIV, _// set display clock divide ratio_ 165         0x80, _// div ratio of 1, standard freq_ 166         SSD1306_SET_PRECHARGE, _// set pre-charge period_ 167         0xF1, _// Vcc internally generated on our board_ 168         SSD1306_SET_VCOM_DESEL, _// set VCOMH deselect level_ 169         0x30, _// 0.83xVcc_ 170 _/* display */_ 171         SSD1306_SET_CONTRAST, _// set contrast control_ 172         0xFF, 173         SSD1306_SET_ENTIRE_ON, _// set entire display on to follow RAM content_ 174         SSD1306_SET_NORM_DISP, _// set normal (not inverted) display_ 175         SSD1306_SET_CHARGE_PUMP, _// set charge pump_ 176         0x14, _// Vcc internally generated on our board_ 177         SSD1306_SET_SCROLL | 0x00, _// deactivate horizontal scrolling if set. This is necessary as memory writes will corrupt if scrolling was enabled_ 178         SSD1306_SET_DISP | 0x01, _// turn display on_ 179     }; 180 181     SSD1306_send_cmd_list(cmds, count_of(cmds)); 182 } 183 184 void SSD1306_scroll(bool on) { 185 _// configure horizontal scrolling_ 186     uint8_t cmds[] = { 187         SSD1306_SET_HORIZ_SCROLL | 0x00, 188         0x00, _// dummy byte_ 189         0x00, _// start page 0_ 190         0x00, _// time interval_ 191         SSD1306_NUM_PAGES - 1, _// end page_ 192         0x00, _// dummy byte_ 193         0xFF, _// dummy byte_ 194         SSD1306_SET_SCROLL | (on ? 0x01 : 0) _// Start/stop scrolling_ 195     }; 196 197     SSD1306_send_cmd_list(cmds, count_of(cmds)); 198 } 199 200 void render(uint8_t *buf, struct render_area *area) { 201 _// update a portion of the display with a render area_ 202     uint8_t cmds[] = { 203         SSD1306_SET_COL_ADDR, 204         area->start_col, 205         area->end_col, 206         SSD1306_SET_PAGE_ADDR, 207         area->start_page, 208         area->end_page 209     }; 210 211     SSD1306_send_cmd_list(cmds, count_of(cmds)); 212     SSD1306_send_buf(buf, area->buflen); 213 } 214 215 static void SetPixel(uint8_t *buf, int x,int y, bool on) { 216     assert(x >= 0 && x < SSD1306_WIDTH && y >=0 && y < SSD1306_HEIGHT); 217 218 _// The calculation to determine the correct bit to set depends on which address_ 219 _// mode we are in. This code assumes horizontal_ 220 221 _// The video ram on the SSD1306 is split up in to 8 rows, one bit per pixel._ 222 _// Each row is 128 long by 8 pixels high, each byte vertically arranged, so byte 0 is x=0, y=0->7,_ 223 _// byte 1 is x = 1, y=0->7 etc_ 224 225 _// This code could be optimised, but is like this for clarity. The compiler_

Attaching an OLED display via I2C

**651**

Raspberry Pi Pico-series C/C++ SDK

226 _// should do a half decent job optimising it anyway._ 227 228     const int BytesPerRow = SSD1306_WIDTH ; _// x pixels, 1bpp, but each row is 8 pixel high, so (x / 8) * 8_ 229 230     int byte_idx = (y / 8) * BytesPerRow + x; 231     uint8_t byte = buf[byte_idx]; 232 233     if (on) 234         byte |=  1 << (y % 8); 235     else 236         byte &= ~(1 << (y % 8)); 237 238     buf[byte_idx] = byte; 239 } 240 _// Basic Bresenhams._ 241 static void DrawLine(uint8_t *buf, int x0, int y0, int x1, int y1, bool on) { 242 243     int dx =  abs(x1-x0); 244     int sx = x0<x1 ? 1 : -1; 245     int dy = -abs(y1-y0); 246     int sy = y0<y1 ? 1 : -1; 247     int err = dx+dy; 248     int e2; 249 250     while (true) { 251         SetPixel(buf, x0, y0, on); 252         if (x0 == x1 && y0 == y1) 253             break; 254         e2 = 2*err; 255 256         if (e2 >= dy) { 257             err += dy; 258             x0 += sx; 259         } 260         if (e2 <= dx) { 261             err += dx; 262             y0 += sy; 263         } 264     } 265 } 266 267 static inline int GetFontIndex(uint8_t ch) { 268     if (ch >= 'A' && ch <='Z') { 269         return  ch - 'A' + 1; 270     } 271     else if (ch >= '0' && ch <='9') { 272         return  ch - '0' + 27; 273     } 274     else return 0; _// Not got that char so space._ 275 } 276 277 static void WriteChar(uint8_t *buf, int16_t x, int16_t y, uint8_t ch) { 278     if (x > SSD1306_WIDTH - 8 || y > SSD1306_HEIGHT - 8) 279         return; 280 281 _// For the moment, only write on Y row boundaries (every 8 vertical pixels)_ 282     y = y/8; 283 284     ch = toupper(ch); 285     int idx = GetFontIndex(ch); 286     int fb_idx = y * 128 + x; 287 288     for (int i=0;i<8;i++) {

Attaching an OLED display via I2C

**652**

Raspberry Pi Pico-series C/C++ SDK

289         buf[fb_idx++] = font[idx * 8 + i]; 290     } 291 } 292 293 static void WriteString(uint8_t *buf, int16_t x, int16_t y, char *str) { 294 _// Cull out any string off the screen_ 295     if (x > SSD1306_WIDTH - 8 || y > SSD1306_HEIGHT - 8) 296         return; 297 298     while (*str) { 299         WriteChar(buf, x, y, *str++); 300         x+=8; 301     } 302 } 303 304 305 306 _#endif_ 307 308 int main() { 309     stdio_init_all(); 310 311 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 312 _#warning i2c / SSD1306_i2d example requires a board with I2C pins_ 313     puts("Default I2C pins were not defined"); 314 _#else_ 315 _// useful information for picotool_ 316     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 317     bi_decl(bi_program_description("SSD1306 OLED driver I2C example for the Raspberry Pi Pico")); 318 319     printf("Hello, SSD1306 OLED display! Look at my raspberries..\n"); 320 321 _// I2C is "open drain", pull ups to keep signal high when no data is being_ 322 _// sent_ 323     i2c_init(i2c_default, SSD1306_I2C_CLK * 1000); 324     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 325     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 326     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 327     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 328 329 _// run through the complete initialization process_ 330     SSD1306_init(); 331 332 _// Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)_ 333     struct render_area frame_area = { 334         start_col: 0, 335         end_col : SSD1306_WIDTH - 1, 336         start_page : 0, 337         end_page : SSD1306_NUM_PAGES - 1 338         }; 339 340     calc_render_area_buflen(&frame_area); 341 342 _// zero the entire display_ 343     uint8_t buf[SSD1306_BUF_LEN]; 344     memset(buf, 0, SSD1306_BUF_LEN); 345     render(buf, &frame_area); 346 347 _// intro sequence: flash the screen 3 times_ 348     for (int i = 0; i < 3; i++) {

Attaching an OLED display via I2C

**653**

Raspberry Pi Pico-series C/C++ SDK

349         SSD1306_send_cmd(SSD1306_SET_ALL_ON); _// Set all pixels on_ 350         sleep_ms(500); 351         SSD1306_send_cmd(SSD1306_SET_ENTIRE_ON); _// go back to following RAM for pixel state_ 352         sleep_ms(500); 353     } 354 355 _// render 3 cute little raspberries_ 356     struct render_area area = { 357         start_page : 0, 358         end_page : (IMG_HEIGHT / SSD1306_PAGE_HEIGHT)  - 1 359     }; 360 361 restart: 362 363     area.start_col = 0; 364     area.end_col = IMG_WIDTH - 1; 365 366     calc_render_area_buflen(&area); 367 368     uint8_t offset = 5 + IMG_WIDTH; _// 5px padding_ 369 370     for (int i = 0; i < 3; i++) { 371         render(raspberry26x32, &area); 372         area.start_col += offset; 373         area.end_col += offset; 374     } 375 376     SSD1306_scroll(true); 377     sleep_ms(5000); 378     SSD1306_scroll(false); 379 380     char *text[] = { 381         "A long time ago", 382         "  on an OLED ", 383         "   display", 384         " far far away", 385         "Lived a small", 386         "red raspberry", 387         "by the name of", 388         "    PICO" 389     }; 390 391     int y = 0; 392     for (uint i = 0 ;i < count_of(text); i++) { 393         WriteString(buf, 5, y, text[i]); 394         y+=8; 395     } 396     render(buf, &frame_area); 397 398 _// Test the display invert function_ 399     sleep_ms(3000); 400     SSD1306_send_cmd(SSD1306_SET_INV_DISP); 401     sleep_ms(3000); 402     SSD1306_send_cmd(SSD1306_SET_NORM_DISP); 403 404     bool pix = true; 405     for (int i = 0; i < 2;i++) { 406         for (int x = 0;x < SSD1306_WIDTH;x++) { 407             DrawLine(buf, x, 0,  SSD1306_WIDTH - 1 - x, SSD1306_HEIGHT - 1, pix); 408             render(buf, &frame_area); 409         } 410 411         for (int y = SSD1306_HEIGHT-1; y >= 0 ;y--) { 412             DrawLine(buf, 0, y, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1 - y, pix);

Attaching an OLED display via I2C

**654**

Raspberry Pi Pico-series C/C++ SDK

413             render(buf, &frame_area); 414         } 415         pix = false; 416     } 417 418     goto restart; 419 420 _#endif_ 421     return 0; 422 }

## **ssd1306_font.h**

A simple font used in the example.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/ssd1306_i2c/ssd1306_font.h_

1 _/**_ 2 _* Copyright (c) 2022 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _// Vertical bitmaps, A-Z, 0-9. Each is 8 pixels high and wide_ 8 _// These are defined vertically to make them quick to copy to FB_ 9 10 static uint8_t font[] = { 11 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, _// Nothing_ 12 0x78, 0x14, 0x12, 0x11, 0x12, 0x14, 0x78, 0x00, _//A_ 13 0x7f, 0x49, 0x49, 0x49, 0x49, 0x49, 0x7f, 0x00, _//B_ 14 0x7e, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x00, _//C_ 15 0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7e, 0x00, _//D_ 16 0x7f, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x00, _//E_ 17 0x7f, 0x09, 0x09, 0x09, 0x09, 0x01, 0x01, 0x00, _//F_ 18 0x7f, 0x41, 0x41, 0x41, 0x51, 0x51, 0x73, 0x00, _//G_ 19 0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x7f, 0x00, _//H_ 20 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, _//I_ 21 0x21, 0x41, 0x41, 0x3f, 0x01, 0x01, 0x01, 0x00, _//J_ 22 0x00, 0x7f, 0x08, 0x08, 0x14, 0x22, 0x41, 0x00, _//K_ 23 0x7f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, _//L_ 24 0x7f, 0x02, 0x04, 0x08, 0x04, 0x02, 0x7f, 0x00, _//M_ 25 0x7f, 0x02, 0x04, 0x08, 0x10, 0x20, 0x7f, 0x00, _//N_ 26 0x3e, 0x41, 0x41, 0x41, 0x41, 0x41, 0x3e, 0x00, _//O_ 27 0x7f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00, _//P_ 28 0x3e, 0x41, 0x41, 0x49, 0x51, 0x61, 0x7e, 0x00, _//Q_ 29 0x7f, 0x11, 0x11, 0x11, 0x31, 0x51, 0x0e, 0x00, _//R_ 30 0x46, 0x49, 0x49, 0x49, 0x49, 0x30, 0x00, 0x00, _//S_ 31 0x01, 0x01, 0x01, 0x7f, 0x01, 0x01, 0x01, 0x00, _//T_ 32 0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3f, 0x00, _//U_ 33 0x0f, 0x10, 0x20, 0x40, 0x20, 0x10, 0x0f, 0x00, _//V_ 34 0x7f, 0x20, 0x10, 0x08, 0x10, 0x20, 0x7f, 0x00, _//W_ 35 0x00, 0x41, 0x22, 0x14, 0x14, 0x22, 0x41, 0x00, _//X_ 36 0x01, 0x02, 0x04, 0x78, 0x04, 0x02, 0x01, 0x00, _//Y_ 37 0x41, 0x61, 0x59, 0x45, 0x43, 0x41, 0x00, 0x00, _//Z_ 38 0x3e, 0x41, 0x41, 0x49, 0x41, 0x41, 0x3e, 0x00, _//0_ 39 0x00, 0x00, 0x42, 0x7f, 0x40, 0x00, 0x00, 0x00, _//1_ 40 0x30, 0x49, 0x49, 0x49, 0x49, 0x46, 0x00, 0x00, _//2_ 41 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, _//3_ 42 0x3f, 0x20, 0x20, 0x78, 0x20, 0x20, 0x00, 0x00, _//4_ 43 0x4f, 0x49, 0x49, 0x49, 0x49, 0x30, 0x00, 0x00, _//5_ 44 0x3f, 0x48, 0x48, 0x48, 0x48, 0x48, 0x30, 0x00, _//6_ 45 0x01, 0x01, 0x01, 0x61, 0x31, 0x0d, 0x03, 0x00, _//7_ 46 0x36, 0x49, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, _//8_

Attaching an OLED display via I2C

**655**

Raspberry Pi Pico-series C/C++ SDK

47 0x06, 0x09, 0x09, 0x09, 0x09, 0x09, 0x7f, 0x00, _//9_ 48 };

## **img_to_array.py**

A helper to convert an image file to an array that can be used in the example.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/ssd1306_i2c/img_to_array.py_

1 _#!/usr/bin/env python3_ 2 3 _# Converts a grayscale image into a format able to be_ 4 _# displayed by the SSD1306 driver in horizontal addressing mode_ 5 6 _# usage: python3 img_to_array.py <logo.bmp>_ 7 8 _# depends on the Pillow library_ 9 _# `python3 -m pip install --upgrade Pillow`_ 10 11 from PIL import Image 12 import sys 13 from pathlib import Path 14 15 OLED_HEIGHT = 32 16 OLED_WIDTH = 128 17 OLED_PAGE_HEIGHT = 8 18 19 if len(sys.argv) < 2: 20     print("No image path provided.") 21     sys.exit() 22 23 img_path = sys.argv[1] 24 25 try: 26     im = Image.open(img_path) 27 except OSError: 28     raise Exception("Oops! The image could not be opened.") 29 30 img_width = im.size[0] 31 img_height = im.size[1] 32 33 if img_width > OLED_WIDTH or img_height > OLED_HEIGHT: 34     print(f'Your image is f{img_width} pixels wide and {img_height} pixels high, but...') 35     raise Exception(f"OLED display only {OLED_WIDTH} pixels wide and {OLED_HEIGHT} pixels high!") 36 37 if not (im.mode == "1" or im.mode == "L"): 38     raise Exception("Image must be grayscale only") 39 40 _# black or white_ 41 out = im.convert("1") 42 43 img_name = Path(im.filename).stem 44 45 _# `pixels` is a flattened array with the top left pixel at index 0_ 46 _# and bottom right pixel at the width*height-1_ 47 pixels = list(out.getdata()) 48 49 _# swap white for black and swap (255, 0) for (1, 0)_ 50 pixels = [0 if x == 255 else 1 for x in pixels] 51 52 _# our goal is to divide the image into 8-pixel high pages_ 53 _# and turn a pixel column into one byte, eg for one page:_

Attaching an OLED display via I2C

**656**

Raspberry Pi Pico-series C/C++ SDK

54 _# 0 1 0 ...._ 55 _# 1 0 0_ 56 _# 1 1 1_ 57 _# 0 0 1_ 58 _# 1 1 0_ 59 _# 0 1 0_ 60 _# 1 1 1_ 61 _# 0 0 1 ...._ 62 63 _# we get 0x6A, 0xAE, 0x33 ... and so on_ 64 _# as `pixels` is flattened, each bit in a column is IMG_WIDTH apart from the next_ 65 66 buffer = [] 67 for i in range(img_height // OLED_PAGE_HEIGHT): 68     start_index = i*img_width*OLED_PAGE_HEIGHT 69     for j in range(img_width): 70         out_byte = 0 71         for k in range(OLED_PAGE_HEIGHT): 72             out_byte |= pixels[k*img_width + start_index + j] << k 73         buffer.append(f'{out_byte: _#04x_ }') 74 75 buffer = ", ".join(buffer) 76 buffer_hex = f'static uint8_t {img_name}[] = {{{buffer}}};\n' 77 78 with open(f'{img_name}.h', 'wt') as file: 79     file.write(f'#define IMG_WIDTH {img_width}\n') 80     file.write(f'#define IMG_HEIGHT {img_height}\n\n') 81     file.write(buffer_hex)

## **raspberry26x32.bmp**

Example image file of a Raspberry.

## **raspberry26x32.h**

The example image file converted to an C array.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/ssd1306_i2c/raspberry26x32.h_

1 _#define IMG_WIDTH 26_ 2 _#define IMG_HEIGHT 32_ 3 4 static uint8_t raspberry26x32[] = { 0x0, 0x0, 0xe, 0x7e, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7e, 0x1e, 0x0, 0x0, 0x0, 0x80, 0xe0, 0xf8, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xf8, 0xe0, 0x80, 0x0, 0x0, 0x1e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x1e, 0x0, 0x0, 0x0, 0x3, 0x7, 0xf, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0xf, 0x7, 0x3, 0x0, 0x0};

## **Bill of Materials**

_Table 51. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|SSD1306-based OLED display|1|Adafruit part|



Attaching an OLED display via I2C

**657**

Raspberry Pi Pico-series C/C++ SDK

M/M Jumper wires 4

generic part

## **Attaching a PA1010D Mini GPS module via I2C**

This example code shows how to interface the Raspberry Pi Pico to the PA1010D Mini GPS module

This allows you to read basic location and time data from the Recommended Minimum Specific GNSS Sentence (GNRMC protocol) and displays it in a user-friendly format. The datasheet for the module can be found on https://cdnlearn.adafruit.com/assets/assets/000/084/295/original/CD_PA1010D_Datasheet_v.03.pdf?1573833002. The output sentence is read and parsed to split the data fields into a 2D character array, which are then individually printed out. The commands to use different protocols and change settings are found on https://www.sparkfun.com/datasheets/GPS/ Modules/PMTK_Protocol.pdf. Additional protocols can be used by editing the init_command array.

##  **NOTE**

Each command requires a checksum after the asterisk. The checksum can be calculated for your command using the following website: https://nmeachecksum.eqth.net/.

The GPS needs to be used outdoors in open skies and requires about 15 seconds to acquire a satellite signal in order to display valid data. When the signal is detected, the device will blink a green LED at 1 Hz.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VDD, GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 3V pin.

_Figure 25. Wiring Diagram for PA1010D._

**==> picture [319 x 213] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

Attaching a PA1010D Mini GPS module via I2C

**658**

Raspberry Pi Pico-series C/C++ SDK

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/pa1010d_i2c/CMakeLists.txt_

1 add_executable(pa1010d_i2c 2         pa1010d_i2c.c 3         ) 4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(pa1010d_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(pa1010d_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(pa1010d_i2c)

## **pa1010d_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/pa1010d_i2c/pa1010d_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/i2c.h"_ 12 _#include "string.h"_ 13 14 _/* Example code to talk to a PA1010D Mini GPS module._ 15 16 _This example reads the Recommended Minimum Specific GNSS Sentence, which includes basic location and time data, each second, formats and displays it._ 17 18 _Connections on Raspberry Pi Pico board, other boards may vary._ 19 20 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is 4 (physical pin 6)) -> SDA on PA1010D board_ 21 _GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is 5 (physical pin 7)) -> SCL on PA1010D board_ 22 _3.3v (physical pin 36) -> VCC on PA1010D board_ 23 _GND (physical pin 38)  -> GND on PA1010D board_ 24 _*/_ 25 26 const int addr = 0x10; 27 _#define MAX_READ 250_ 28 29 _#ifdef i2c_default_ 30 31 void pa1010d_write_command(const char command[], int com_length) { 32 _// Convert character array to bytes for writing_ 33     uint8_t int_command[com_length]; 34 35     for (int i = 0; i < com_length; ++i) { 36         int_command[i] = command[i]; 37         i2c_write_blocking(i2c_default, addr, &int_command[i], 1, true); 38     } 39 } 40

Attaching a PA1010D Mini GPS module via I2C

**659**

Raspberry Pi Pico-series C/C++ SDK

41 void pa1010d_parse_string(char output[], char protocol[]) { 42 _// Finds location of protocol message in output_ 43     char *com_index = strstr(output, protocol); 44     int p = com_index - output; 45 46 _// Splits components of output sentence into array_ 47 _#define NO_OF_FIELDS 14_ 48 _#define MAX_LEN 15_ 49 50     int n = 0; 51     int m = 0; 52 53     char gps_data[NO_OF_FIELDS][MAX_LEN]; 54     memset(gps_data, 0, sizeof(gps_data)); 55 56     bool complete = false; 57     while (output[p] != '$' && n < MAX_LEN && complete == false) { 58         if (output[p] == ',' || output[p] == '*') { 59             n += 1; 60             m = 0; 61         } else { 62             gps_data[n][m] = output[p]; 63 _// Checks if sentence is complete_ 64             if (m < NO_OF_FIELDS) { 65                 m++; 66             } else { 67                 complete = true; 68             } 69         } 70         p++; 71     } 72 73 _// Displays GNRMC data_ 74 _// Similarly, additional if statements can be used to add more protocols_ 75     if (strcmp(protocol, "GNRMC") == 0) { 76         printf("Protocol:%s\n", gps_data[0]); 77         printf("UTC Time: %s\n", gps_data[1]); 78         printf("Status: %s\n", gps_data[2][0] == 'V' ? "Data invalid. GPS fix not found." : "Data Valid"); 79         printf("Latitude: %s\n", gps_data[3]); 80         printf("N/S indicator: %s\n", gps_data[4]); 81         printf("Longitude: %s\n", gps_data[5]); 82         printf("E/W indicator: %s\n", gps_data[6]); 83         printf("Speed over ground: %s\n", gps_data[7]); 84         printf("Course over ground: %s\n", gps_data[8]); 85         printf("Date: %c%c/%c%c/%c%c\n", gps_data[9][0], gps_data[9][1], gps_data[9][2], gps_data[9][3], gps_data[9][4], 86                gps_data[9][5]); 87         printf("Magnetic Variation: %s\n", gps_data[10]); 88         printf("E/W degree indicator: %s\n", gps_data[11]); 89         printf("Mode: %s\n", gps_data[12]); 90         printf("Checksum: %c%c\n", gps_data[13][0], gps_data[13][1]); 91     } 92 } 93 94 void pa1010d_read_raw(char numcommand[]) { 95     uint8_t buffer[MAX_READ]; 96 97     int i = 0; 98     bool complete = false; 99 100     i2c_read_blocking(i2c_default, addr, buffer, MAX_READ, false); 101 102 _// Convert bytes to characters_

Attaching a PA1010D Mini GPS module via I2C

**660**

Raspberry Pi Pico-series C/C++ SDK

103     while (i < MAX_READ && complete == false) { 104         numcommand[i] = buffer[i]; 105 _// Stop converting at end of message_ 106         if (buffer[i] == 10 && buffer[i + 1] == 10) { 107             complete = true; 108         } 109         i++; 110     } 111 } 112 113 _#endif_ 114 115 int main() { 116     stdio_init_all(); 117 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 118 _#warning i2c/mpu6050_i2c example requires a board with I2C pins_ 119     puts("Default I2C pins were not defined"); 120 _#else_ 121 122     char numcommand[MAX_READ]; 123 124 _// Decide which protocols you would like to retrieve data from_ 125     char init_command[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"; 126 127 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 128     i2c_init(i2c_default, 400 * 1000); 129     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 130     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 131     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 132     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 133 134 _// Make the I2C pins available to picotool_ 135     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 136 137     printf("Hello, PA1010D! Reading raw data from module...\n"); 138 139     pa1010d_write_command(init_command, sizeof(init_command)); 140 141     while (1) { 142 _// Clear array_ 143         memset(numcommand, 0, MAX_READ); 144 _// Read and re-format_ 145         pa1010d_read_raw(numcommand); 146         pa1010d_parse_string(numcommand, "GNRMC"); 147 148 _// Wait for data to refresh_ 149         sleep_ms(1000); 150 151 _// Clear terminal_ 152         printf("\033[1;1H\033[2J"); 153     } 154 _#endif_ 155 }

## **Bill of Materials**

Attaching a PA1010D Mini GPS module via I2C

**661**

Raspberry Pi Pico-series C/C++ SDK

_Table 52. A list of materials required for the example_

|**Item**|**Quantity**|Details|
|---|---|---|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|PA1010D board|1|https://shop.pimoroni.com/products/<br>pa1010d-gps-breakout|
|M/M Jumper wires|4|generic part|



## **Attaching a PCF8523 Real Time Clock via I2C**

This example code shows how to interface the Raspberry Pi Pico to the PCF8523 Real Time Clock

This example allows you to initialise the current time and date and then displays it every half-second. Additionally it lets you set an alarm for a particular time and date and raises an alert accordingly. More information about the module is available at https://learn.adafruit.com/adafruit-pcf8523-real-time-clock.

## **Wiring information**

Wiring up the device requires 4 jumpers, to connect VDD, GND, SDA and SCL. The example here uses I2C port 0, which is assigned to GPIO 4 (SDA) and 5 (SCL) in software. Power is supplied from the 5V pin.

_Figure 26. Wiring Diagram for PCF8523._

**==> picture [319 x 221] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/pcf8523_i2c/CMakeLists.txt_

- 1 add_executable(pcf8523_i2c 2         pcf8523_i2c.c 3         )

Attaching a PCF8523 Real Time Clock via I2C

**662**

Raspberry Pi Pico-series C/C++ SDK

4 5 # pull in common dependencies and additional i2c hardware support 6 target_link_libraries(pcf8523_i2c pico_stdlib hardware_i2c) 7 8 # create map/bin/hex file etc. 9 pico_add_extra_outputs(pcf8523_i2c) 10 11 # add url via pico_set_program_url 12 example_auto_set_url(pcf8523_i2c)

## **pcf8523_i2c.c**

The example code.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/i2c/pcf8523_i2c/pcf8523_i2c.c_

1 _/**_ 2 _* Copyright (c) 2020 Raspberry Pi (Trading) Ltd._ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _*/_ 6 7 _#include <stdio.h>_ 8 _#include <string.h>_ 9 _#include "pico/stdlib.h"_ 10 _#include "pico/binary_info.h"_ 11 _#include "hardware/i2c.h"_ 12 13 _/* Example code to talk to a PCF8520 Real Time Clock module_ 14 15 _Connections on Raspberry Pi Pico board, other boards may vary._ 16 17 _GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is 4 (physical pin 6)) -> SDA on PCF8520 board_ 18 _GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is 5 (physical pin 7)) -> SCL on PCF8520 board_ 19 _5V (physical pin 40) -> VCC on PCF8520 board_ 20 _GND (physical pin 38)  -> GND on PCF8520 board_ 21 _*/_ 22 23 _#ifdef i2c_default_ 24 25 _// By default these devices  are on bus address 0x68_ 26 static int addr = 0x68; 27 28 static void pcf8520_reset() { 29 _// Two byte reset. First byte register, second byte data_ 30 _// There are a load more options to set up the device in different ways that could be added here_ 31     uint8_t buf[] = {0x00, 0x58}; 32     i2c_write_blocking(i2c_default, addr, buf, 2, false); 33 } 34 35 static void pcf820_write_current_time() { 36 _// buf[0] is the register to write to_ 37 _// buf[1] is the value that will be written to the register_ 38     uint8_t buf[2]; 39 40 _//Write values for the current time in the array_ 41 _//index 0 -> second: bits 4-6 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 42 _//index 1 -> minute: bits 4-6 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 43 _//index 2 -> hour: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit_

Attaching a PCF8523 Real Time Clock via I2C

**663**

Raspberry Pi Pico-series C/C++ SDK

44 _//index 3 -> day of the month: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 45 _//index 4 -> day of the week: where Sunday = 0x00, Monday = 0x01, Tuesday... ...Saturday = 0x06_ 46 _//index 5 -> month: bit 4 is responsible for the ten's digit and bits 0-3 for the unit's digit_ 47 _//index 6 -> year: bits 4-7 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 48 49 _//NOTE: if the value in the year register is a multiple for 4, it will be considered a leap year and hence will include the 29th of February_ 50 51     uint8_t current_val[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 52 53     for (int i = 3; i < 10; ++i) { 54         buf[0] = i; 55         buf[1] = current_val[i - 3]; 56         i2c_write_blocking(i2c_default, addr, buf, 2, false); 57     } 58 } 59 60 static void pcf8520_read_raw(uint8_t *buffer) { 61 _// For this particular device, we send the device the register we want to read_ 62 _// first, then subsequently read from the device. The register is auto incrementing_ 63 _// so we don't need to keep sending the register we want, just the first._ 64 65 _// Start reading acceleration registers from register 0x3B for 6 bytes_ 66     uint8_t val = 0x03; 67     i2c_write_blocking(i2c_default, addr, &val, 1, true); _// true to keep master control of bus_ 68     i2c_read_blocking(i2c_default, addr, buffer, 7, false); 69 } 70 71 72 void pcf8520_set_alarm() { 73 _// buf[0] is the register to write to_ 74 _// buf[1] is the value that will be written to the register_ 75     uint8_t buf[2]; 76 77 _// Default value of alarm register is 0x80_ 78 _// Set bit 8 of values to 0 to activate that particular alarm_ 79 _// Index 0 -> minute: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 80 _// Index 1 -> hour: bits 4-6 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 81 _// Index 2 -> day of the month: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit_ 82 _// Index 3 -> day of the week: where Sunday = 0x00, Monday = 0x01, Tuesday... ...Saturday = 0x06_ 83 84     uint8_t alarm_val[4] = {0x01, 0x80, 0x80, 0x80}; 85 _// Write alarm values to registers_ 86     for (int i = 10; i < 14; ++i) { 87         buf[0] = (uint8_t) i; 88         buf[1] = alarm_val[i - 10]; 89         i2c_write_blocking(i2c_default, addr, buf, 2, false); 90     } 91 } 92 93 void pcf8520_check_alarm() { 94 _// Check bit 3 of control register 2 for alarm flags_ 95     uint8_t status[1]; 96     uint8_t val = 0x01; 97     i2c_write_blocking(i2c_default, addr, &val, 1, true); _// true to keep master control of_

Attaching a PCF8523 Real Time Clock via I2C

**664**

Raspberry Pi Pico-series C/C++ SDK

_bus_ 98     i2c_read_blocking(i2c_default, addr, status, 1, false); 99 100     if ((status[0] & 0x08) == 0x08) { 101         printf("ALARM RINGING"); 102     } else { 103         printf("Alarm not triggered yet"); 104     } 105 } 106 107 108 void pcf8520_convert_time(int conv_time[7], const uint8_t raw_time[7]) { 109 _// Convert raw data into time_ 110     conv_time[0] = (10 * (int) ((raw_time[0] & 0x70) >> 4)) + ((int) (raw_time[0] & 0x0F)); 111     conv_time[1] = (10 * (int) ((raw_time[1] & 0x70) >> 4)) + ((int) (raw_time[1] & 0x0F)); 112     conv_time[2] = (10 * (int) ((raw_time[2] & 0x30) >> 4)) + ((int) (raw_time[2] & 0x0F)); 113     conv_time[3] = (10 * (int) ((raw_time[3] & 0x30) >> 4)) + ((int) (raw_time[3] & 0x0F)); 114     conv_time[4] = (int) (raw_time[4] & 0x07); 115     conv_time[5] = (10 * (int) ((raw_time[5] & 0x10) >> 4)) + ((int) (raw_time[5] & 0x0F)); 116     conv_time[6] = (10 * (int) ((raw_time[6] & 0xF0) >> 4)) + ((int) (raw_time[6] & 0x0F)); 117 } 118 _#endif_ 119 120 int main() { 121     stdio_init_all(); 122 _#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)_ 123 _#warning i2c/pcf8520_i2c example requires a board with I2C pins_ 124     puts("Default I2C pins were not defined"); 125 _#else_ 126     printf("Hello, PCF8520! Reading raw data from registers...\n"); 127 128 _// This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)_ 129     i2c_init(i2c_default, 400 * 1000); 130     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); 131     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); 132     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); 133     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); 134 _// Make the I2C pins available to picotool_ 135     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C)); 136 137     pcf8520_reset(); 138 139     pcf820_write_current_time(); 140     pcf8520_set_alarm(); 141     pcf8520_check_alarm(); 142 143     uint8_t raw_time[7]; 144     int real_time[7]; 145     char days_of_week[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 146 147     while (1) { 148 149         pcf8520_read_raw(raw_time); 150         pcf8520_convert_time(real_time, raw_time); 151 152         printf("Time: %02d : %02d : %02d\n", real_time[2], real_time[1], real_time[0]); 153         printf("Date: %s %02d / %02d / %02d\n", days_of_week[real_time[4]], real_time[3], real_time[5], real_time[6]); 154         pcf8520_check_alarm(); 155 156         sleep_ms(500);

Attaching a PCF8523 Real Time Clock via I2C

**665**

Raspberry Pi Pico-series C/C++ SDK

157 158 _// Clear terminal_ 159         printf("\033[1;1H\033[2J"); 160     } 161 _#endif_ 162 }

## **Bill of Materials**

_Table 53. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|PCF8523 board|1|https://www.adafruit.com/product/<br>3295|
|M/M Jumper wires|4|generic part|



## **Interfacing 1-Wire devices to the Pico**

This example demonstrates how to use 1-Wire devices with the Raspberry Pi Pico (RP2040). 1-Wire is an interface that enables a master to control several slave devices over a simple shared serial bus.

The example provides a 1-Wire library that is used to take readings from a set of connected DS18B20 1-Wire temperature sensors. The results are sent to the default serial terminal connected via USB or UART as configured in the SDK.

The library uses a driver based on the RP2040 PIO state machine to generate accurate bus timings and control the 1- Wire bus via a GPIO pin.

_1-Wire® is a registered trademark of Maxim Integrated Products, Inc._

## **Wiring information**

Connect one or more DS18B20 sensors to the Pico as shown in the diagram and table below.

Connect GPIO 15 to 3V3(OUT) with a pull-up resistor of about 4k ohms.

Interfacing 1-Wire devices to the Pico

**666**

Raspberry Pi Pico-series C/C++ SDK

_Figure 27. Wiring diagram_

_Table 54. Connections table_

**==> picture [319 x 222] intentionally omitted <==**

|**Pico**|**pin**|**DS18B20**|**pin / sensor wire**|
|---|---|---|---|
|GND|38 or equivalent|GND|1 / Black|
|GPIO 15|20|DQ|2 / Yellow|
|3V3(OUT)|36|VDD|3 / Red|



## **Bill of materials**

_Table 55. A list of materials for the example circuit_

|**Bill of materials**|||
|---|---|---|
|**Item**|**Quantity**|**Details**|
|Breadboard|1|generic part|
|Raspberry Pi Pico|1|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|DS18B20|3|chip or wired sensor|
|3900 ohm resistor|1|generic part|
|M/M jumper wire|13|generic part|



## **List of files**

## **CMakeLists.txt**

CMake file to incorporate the example in the build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/CMakeLists.txt_

- 1 add_executable(pio_onewire)

- 2

- 3 target_sources(pio_onewire PRIVATE onewire.c)

- 4

- 5 add_subdirectory(onewire_library)

- 6

- 7 target_link_libraries(pio_onewire PRIVATE 8     pico_stdlib

Interfacing 1-Wire devices to the Pico

**667**

Raspberry Pi Pico-series C/C++ SDK

9     hardware_pio 10     onewire_library) 11 12 pico_add_extra_outputs(pio_onewire) 13 14 # add url via pico_set_program_url 15 example_auto_set_url(pio_onewire)

## **onewire.c**

Source code for the example program.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/onewire.c_

1 _/**_ 2 _* Copyright (c) 2023 mjcross_ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _**/_ 6 7 _#include <stdio.h>_ 8 _#include "pico/stdlib.h"_ 9 _#include "pico/binary_info.h"_ 10 11 _#include "onewire_library.h"    // onewire library functions_ 12 _#include "ow_rom.h"             // onewire ROM command codes_ 13 _#include "ds18b20.h"            // ds18b20 function codes_ 14 15 _// Demonstrates the PIO onewire driver by taking readings from a set of_ 16 _// ds18b20 1-wire temperature sensors._ 17 18 int main() { 19     stdio_init_all(); 20 21     PIO pio = pio0; 22     uint gpio = 15; 23 24     OW ow; 25     uint offset; 26 _// add the program to the PIO shared address space_ 27     if (pio_can_add_program (pio, &onewire_program)) { 28         offset = pio_add_program (pio, &onewire_program); 29 30 _// claim a state machine and initialise a driver instance_ 31         if (ow_init (&ow, pio, offset, gpio)) { 32 33 _// find and display 64-bit device addresses_ 34             int maxdevs = 10; 35             uint64_t romcode[maxdevs]; 36             int num_devs = ow_romsearch (&ow, romcode, maxdevs, OW_SEARCH_ROM); 37 38             printf("Found %d devices\n", num_devs); 39             for (int i = 0; i < num_devs; i += 1) { 40                 printf("\t%d: 0x%llx\n", i, romcode[i]); 41             } 42             putchar ('\n'); 43 44             while (num_devs > 0) { 45 _// start temperature conversion in parallel on all devices_ 46 _// (see ds18b20 datasheet)_ 47                 ow_reset (&ow); 48                 ow_send (&ow, OW_SKIP_ROM); 49                 ow_send (&ow, DS18B20_CONVERT_T);

Interfacing 1-Wire devices to the Pico

**668**

Raspberry Pi Pico-series C/C++ SDK

50 51 _// wait for the conversions to finish_ 52                 while (ow_read(&ow) == 0); 53 54 _// read the result from each device_ 55                 for (int i = 0; i < num_devs; i += 1) { 56                     ow_reset (&ow); 57                     ow_send (&ow, OW_MATCH_ROM); 58                     for (int b = 0; b < 64; b += 8) { 59                         ow_send (&ow, romcode[i] >> b); 60                     } 61                     ow_send (&ow, DS18B20_READ_SCRATCHPAD); 62                     int16_t temp = 0; 63                     temp = ow_read (&ow) | (ow_read (&ow) << 8); 64                     printf ("\t%d: %f", i, temp / 16.0); 65                 } 66                 putchar ('\n'); 67             } 68 69         } else { 70             puts ("could not initialise the driver"); 71         } 72     } else { 73         puts ("could not add the program"); 74     } 75 76     while(true); 77 }

## **ow_rom.h**

Header file with generic ROM command codes for 1-Wire devices.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/ow_rom.h_

1 _// Generic ROM commands for 1-Wire devices_ 2 _// https://www.analog.com/en/technical-articles/guide-to-1wire-communication.html_ 3 _//_ 4 _#define OW_READ_ROM         0x33_ 5 _#define OW_MATCH_ROM        0x55_ 6 _#define OW_SKIP_ROM         0xCC_ 7 _#define OW_ALARM_SEARCH     0xEC_ 8 _#define OW_SEARCH_ROM       0xF0_

## **ds18b20.h**

Header file with function command codes for the DS18B20 device.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/ds18b20.h_

1 _// Function commands for d218b20 1-Wire temperature sensor_ 2 _// https://www.analog.com/en/products/ds18b20.html_ 3 _//_ 4 _#define DS18B20_CONVERT_T           0x44_ 5 _#define DS18B20_WRITE_SCRATCHPAD    0x4e_ 6 _#define DS18B20_READ_SCRATCHPAD     0xbe_ 7 _#define DS18B20_COPY_SCRATCHPAD     0x48_ 8 _#define DS18B20_RECALL_EE           0xb8_ 9 _#define DS18B20_READ_POWER_SUPPLY   0xb4_

Interfacing 1-Wire devices to the Pico

**669**

Raspberry Pi Pico-series C/C++ SDK

## **onewire_library/**

Subdirectory containing the 1-Wire library and driver.

## **onewire_library/CMakeLists.txt**

CMake file to build the 1-Wire library and driver.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/onewire_library/CMakeLists.txt_

1 add_library(onewire_library INTERFACE) 2 target_sources(onewire_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/onewire_library.c) 3 4 # invoke pio_asm to assemble the state machine programs 5 # 6 pico_generate_pio_header(onewire_library ${CMAKE_CURRENT_LIST_DIR}/onewire_library.pio) 7 8 target_link_libraries(onewire_library INTERFACE 9         pico_stdlib 10         hardware_pio 11         ) 12 13 # add the `binary` directory so that the generated headers are included in the project 14 # 15 target_include_directories(onewire_library INTERFACE 16     ${CMAKE_CURRENT_SOURCE_DIR} 17     ${CMAKE_CURRENT_BINARY_DIR} 18     )

## **onewire_library/onewire_library.c**

Source code for the 1-Wire user functions.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/onewire_library/onewire_library.c_

1 _/**_ 2 _* Copyright (c) 2023 mjcross_ 3 _*_ 4 _* SPDX-License-Identifier: BSD-3-Clause_ 5 _**/_ 6 7 _#include "pico/stdlib.h"_ 8 _#include "hardware/gpio.h"_ 9 _#include "hardware/pio.h"_ 10 11 _#include "onewire_library.h"_ 12 13 14 _// Create a driver instance and populate the provided OW structure._ 15 _// Returns: True on success._ 16 _// ow: A pointer to a blank OW structure to hold the driver parameters._ 17 _// pio: The PIO hardware instance to use._ 18 _// offset: The location of the onewire program in the PIO shared address space._ 19 _// gpio: The pin to use for the bus (NB: see the README)._ 20 bool ow_init (OW *ow, PIO pio, uint offset, uint gpio) { 21     int sm = pio_claim_unused_sm (pio, false); 22     if (sm == -1) { 23         return false; 24     } 25     gpio_init (gpio); _// enable the gpio and clear any output value_ 26     pio_gpio_init (pio, gpio); _// set the function to PIO output_ 27     ow->gpio = gpio; 28     ow->pio = pio; 29     ow->offset = offset;

Interfacing 1-Wire devices to the Pico

**670**

Raspberry Pi Pico-series C/C++ SDK

30     ow->sm = (uint)sm; 31     ow->jmp_reset = onewire_reset_instr (ow->offset); _// assemble the bus reset instruction_ 32     onewire_sm_init (ow->pio, ow->sm, ow->offset, ow->gpio, 8); _// set 8 bits per word_ 33     return true; 34 } 35 36 37 _// Send a binary word on the bus (LSB first)._ 38 _// ow: A pointer to an OW driver struct._ 39 _// data: The word to be sent._ 40 void ow_send (OW *ow, uint data) { 41     pio_sm_put_blocking (ow->pio, ow->sm, (uint32_t)data); 42     pio_sm_get_blocking (ow->pio, ow->sm); _// discard the response_ 43 } 44 45 46 _// Read a binary word from the bus._ 47 _// Returns: the word read (LSB first)._ 48 _// ow: pointer to an OW driver struct_ 49 uint8_t ow_read (OW *ow) { 50     pio_sm_put_blocking (ow->pio, ow->sm, 0xff); _// generate read slots_ 51     return (uint8_t)(pio_sm_get_blocking (ow->pio, ow->sm) >> 24); _// shift response into bits 0..7_ 52 } 53 54 55 _// Reset the bus and detect any connected slaves._ 56 _// Returns: true if any slaves responded._ 57 _// ow: pointer to an OW driver struct_ 58 bool ow_reset (OW *ow) { 59     pio_sm_exec_wait_blocking (ow->pio, ow->sm, ow->jmp_reset); 60     if ((pio_sm_get_blocking (ow->pio, ow->sm) & 1) == 0) { _// apply pin mask (see pio program)_ 61         return true; _// a slave pulled the bus low_ 62     } 63     return false; 64 } 65 66 67 _// Find ROM codes (64-bit hardware addresses) of all connected devices._ 68 _// See https://www.analog.com/en/app-notes/1wire-search-algorithm.html_ 69 _// Returns: the number of devices found (up to maxdevs) or -1 if an error occurrred._ 70 _// ow: pointer to an OW driver struct_ 71 _// romcodes: location at which store the addresses (NULL means don't store)_ 72 _// maxdevs: maximum number of devices to find (0 means no limit)_ 73 _// command: 1-Wire search command (e.g. OW_SEARCHROM or OW_ALARM_SEARCH)_ 74 int ow_romsearch (OW *ow, uint64_t *romcodes, int maxdevs, uint command) { 75     int index; 76     uint64_t romcode = 0ull; 77     int branch_point; 78     int next_branch_point = -1; 79     int num_found = 0; 80     bool finished = false; 81 82     onewire_sm_init (ow->pio, ow->sm, ow->offset, ow->gpio, 1); _// set driver to 1-bit mode_ 83 84     while (finished == false && (maxdevs == 0 || num_found < maxdevs )) { 85         finished = true; 86         branch_point = next_branch_point; 87         if (ow_reset (ow) == false) { 88             num_found = 0; _// no slaves present_ 89             finished = true; 90             break; 91         }

Interfacing 1-Wire devices to the Pico

**671**

Raspberry Pi Pico-series C/C++ SDK

92         for (int i = 0; i < 8; i += 1) { _// send search command as single bits_ 93             ow_send (ow, command >> i); 94         } 95         for (index = 0; index < 64; index += 1) { _// determine romcode bits 0..63 (see ref)_ 96             uint a = ow_read (ow); 97             uint b = ow_read (ow); 98             if (a == 0 && b == 0) { _// (a, b) = (0, 0)_ 99                 if (index == branch_point) { 100                     ow_send (ow, 1); 101                     romcode |= (1ull << index); 102                 } else { 103                     if (index > branch_point || (romcode & (1ull << index)) == 0) { 104                         ow_send(ow, 0); 105                         finished = false; 106                         romcode &= ~(1ull << index); 107                         next_branch_point = index; 108                     } else { _// index < branch_point or romcode[index] = 1_ 109                         ow_send (ow, 1); 110                     } 111                 } 112             } else if (a != 0 && b != 0) { _// (a, b) = (1, 1) error (e.g. device disconnected)_ 113                 num_found = -2; _// function will return -1_ 114                 finished = true; 115                 break; _// terminate for loop_ 116             } else { 117                 if (a == 0) { _// (a, b) = (0, 1) or (1, 0)_ 118                     ow_send (ow, 0); 119                     romcode &= ~(1ull << index); 120                 } else { 121                     ow_send (ow, 1); 122                     romcode |= (1ull << index); 123                 } 124             } 125         } _// end of for loop_ 126 127         if (romcodes != NULL) { 128             romcodes[num_found] = romcode; _// store the romcode_ 129         } 130         num_found += 1; 131     } _// end of while loop_ 132 133     onewire_sm_init (ow->pio, ow->sm, ow->offset, ow->gpio, 8); _// restore 8-bit mode_ 134     return num_found; 135 }

## **onewire_library/onewire_library.h**

Header file for the 1-Wire user functions and types.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/onewire_library/onewire_library.h_

1 _#ifndef _ONEWIRE_LIBRARY_H_ 2 _#define _ONEWIRE_LIBRARY_H_ 3 4 _#include "hardware/pio.h"_ 5 _#include "hardware/clocks.h"            // for clock_get_hz() in generated header_ 6 _#include "onewire_library.pio.h"        // generated by pioasm_ 7 8 typedef struct { 9     PIO pio; 10     uint sm; 11     uint jmp_reset;

Interfacing 1-Wire devices to the Pico

**672**

Raspberry Pi Pico-series C/C++ SDK

12     int offset; 13     int gpio; 14 } OW; 15 16 bool ow_init (OW *ow, PIO pio, uint offset, uint gpio); 17 void ow_send (OW *ow, uint data); 18 uint8_t ow_read (OW *ow); 19 bool ow_reset (OW *ow); 20 int ow_romsearch (OW *ow, uint64_t *romcodes, int maxdevs, uint command); 21 22 _#endif_

## **onewire_library/onewire_library.pio**

PIO assembly code for the 1-Wire driver.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/pio/onewire/onewire_library/onewire_library.pio_

1 ; 2 ; Copyright (c) 2023 mjcross 3 ; 4 ; SPDX-License-Identifier: BSD-3-Clause 5 ; 6 7 ; Implements a Maxim 1-Wire bus with a GPIO pin. 8 ; 9 ; Place data words to be transmitted in the TX FIFO and read the results from the 10 ; RX FIFO. To reset the bus execute a jump to 'reset_bus' using the opcode from 11 ; the provided function. 12 ; 13 ; At 1us per cycle as initialised below the timings are those recommended by: 14 ; https://www.analog.com/en/technical-articles/1wire-communication-through-software.html 15 ; 16 ; Notes: 17 ;   (1) The code will stall with the bus in a safe state if the FIFOs are empty/full. 18 ;   (2) The bus must be pulled up with an external pull-up resistor of about 4k. 19 ;       The internal GPIO resistors are too high (~50k) to work reliably for this. 20 ;   (3) Do not connect the GPIO pin directly to a bus powered at more than 3.3V. 21 22 .program onewire 23 .side_set 1 pindirs 24 25 PUBLIC reset_bus: 26         set x, 28       side 1  [15]    ; pull bus low                          16 27 loop_a: jmp x-- loop_a  side 1  [15]    ;                                  29 x 16 28         set x, 8        side 0  [6]     ; release bus                            7 29 loop_b: jmp x-- loop_b  side 0  [6]     ;                                    9 x 7 30 31         mov isr, pins   side 0          ; read all pins to ISR (avoids autopush) 1 32         push            side 0          ; push result manually                   1 33         set x, 24       side 0  [7]     ;                                        8 34 loop_c: jmp x-- loop_c  side 0  [15]    ;                                  25 x 16 35 36 .wrap_target 37 PUBLIC fetch_bit: 38         out x, 1        side 0          ; shift next bit from OSR (autopull)     1 39         jmp !x  send_0  side 1  [5]     ; pull bus low, branch if sending '0'    6 40 41 send_1: ; send a '1' bit 42         set x, 2        side 0  [8]     ; release bus, wait for slave response   9 43         in pins, 1      side 0  [4]     ; read bus, shift bit to ISR (autopush)  5 44 loop_e: jmp x-- loop_e  side 0  [15]    ;                                   3 x 16 45         jmp fetch_bit   side 0          ;                                        1

Interfacing 1-Wire devices to the Pico

**673**

Raspberry Pi Pico-series C/C++ SDK

46 47 send_0: ; send a '0' bit 48         set x, 2        side 1  [5]     ; continue pulling bus low               6 49 loop_d: jmp x-- loop_d  side 1  [15]    ;                                   3 x 16 50         in null, 1      side 0  [8]     ; release bus, shift 0 to ISR (autopush) 9 51 .wrap 52 ;; (17 instructions) 53 54 55 % c-sdk { 56 static inline void onewire_sm_init (PIO pio, uint sm, uint offset, uint pin_num, uint bits_per_word) { 57 58     // create a new state machine configuration 59     pio_sm_config c = onewire_program_get_default_config (offset); 60 61     // Input Shift Register configuration settings 62     sm_config_set_in_shift ( 63         &c, 64         true,           // shift direction: right 65         true,           // autopush: enabled 66         bits_per_word   // autopush threshold 67     ); 68 69     // Output Shift Register configuration settings 70     sm_config_set_out_shift ( 71         &c, 72         true,           // shift direction: right 73         true,           // autopull: enabled 74         bits_per_word   // autopull threshold 75     ); 76 77     // configure the input and sideset pin groups to start at `pin_num` 78     sm_config_set_in_pins (&c, pin_num); 79     sm_config_set_sideset_pins (&c, pin_num); 80 81     // configure the clock divider for 1 usec per instruction 82     float div = clock_get_hz (clk_sys) * 1e-6; 83     sm_config_set_clkdiv (&c, div); 84 85     // apply the configuration and initialise the program counter 86     pio_sm_init (pio, sm, offset + onewire_offset_fetch_bit, &c); 87 88     // enable the state machine 89     pio_sm_set_enabled (pio, sm, true); 90 } 91 92 static inline uint onewire_reset_instr (uint offset) { 93     // encode a "jmp reset_bus side 0" instruction for the state machine 94     return pio_encode_jmp (offset + onewire_offset_reset_bus) | pio_encode_sideset (1, 0); 95 } 96 %}

## **Communicating as master and slave via SPI**

This example code shows how to interface two RP2040 microcontrollers to each other using SPI.

Communicating as master and slave via SPI

**674**

Raspberry Pi Pico-series C/C++ SDK

## **Wiring information**

|**Function**|**Master (RP2040)**|**Slave (RP2040)**|**Master (Pico)**|**Slave (Pico)**|
|---|---|---|---|---|
|MOSI|DO0|DI0|25|21|
|SCLK|SCK0|SCK0|24|24|
|GND|GND|GND|23|23|
|CS|CS0|CS0|22|22|
|MISO|DI0|DO0|21|25|



_Figure 28. Wiring Diagram for SPI Master and Slave._

**==> picture [319 x 117] intentionally omitted <==**

At least one of the boards should be powered, and will share power to the other.

If the master is not connected properly to a slave, the master will report reading all zeroes.

If the slave is not connected properly to a master, it will initialize but never transmit nor receive, because it’s waiting for clock signal from the master.

## **Outputs**

Both master and slave boards will give output to stdio.

With master and slave properly connected, the master should output something like this:

SPI master example SPI master says: The following buffer will be written to MOSI endlessly: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff SPI master says: read page 0 from the MISO line: ff fe fd fc fb fa f9 f8 f7 f6 f5 f4 f3 f2 f1 f0 ef ee ed ec eb ea e9 e8 e7 e6 e5 e4 e3 e2 e1 e0 df de dd dc db da d9 d8 d7 d6 d5 d4 d3 d2 d1 d0 cf ce cd cc cb ca c9 c8 c7 c6 c5 c4 c3 c2 c1 c0 bf be bd bc bb ba b9 b8 b7 b6 b5 b4 b3 b2 b1 b0

Communicating as master and slave via SPI

**675**

Raspberry Pi Pico-series C/C++ SDK

af ae ad ac ab aa a9 a8 a7 a6 a5 a4 a3 a2 a1 a0 9f 9e 9d 9c 9b 9a 99 98 97 96 95 94 93 92 91 90 8f 8e 8d 8c 8b 8a 89 88 87 86 85 84 83 82 81 80 7f 7e 7d 7c 7b 7a 79 78 77 76 75 74 73 72 71 70 6f 6e 6d 6c 6b 6a 69 68 67 66 65 64 63 62 61 60 5f 5e 5d 5c 5b 5a 59 58 57 56 55 54 53 52 51 50 4f 4e 4d 4c 4b 4a 49 48 47 46 45 44 43 42 41 40 3f 3e 3d 3c 3b 3a 39 38 37 36 35 34 33 32 31 30 2f 2e 2d 2c 2b 2a 29 28 27 26 25 24 23 22 21 20 1f 1e 1d 1c 1b 1a 19 18 17 16 15 14 13 12 11 10 0f 0e 0d 0c 0b 0a 09 08 07 06 05 04 03 02 01 00

The slave should output something like this:

SPI slave example

SPI slave says: When reading from MOSI, the following buffer will be written to MISO: ff fe fd fc fb fa f9 f8 f7 f6 f5 f4 f3 f2 f1 f0 ef ee ed ec eb ea e9 e8 e7 e6 e5 e4 e3 e2 e1 e0 df de dd dc db da d9 d8 d7 d6 d5 d4 d3 d2 d1 d0 cf ce cd cc cb ca c9 c8 c7 c6 c5 c4 c3 c2 c1 c0 bf be bd bc bb ba b9 b8 b7 b6 b5 b4 b3 b2 b1 b0 af ae ad ac ab aa a9 a8 a7 a6 a5 a4 a3 a2 a1 a0 9f 9e 9d 9c 9b 9a 99 98 97 96 95 94 93 92 91 90 8f 8e 8d 8c 8b 8a 89 88 87 86 85 84 83 82 81 80 7f 7e 7d 7c 7b 7a 79 78 77 76 75 74 73 72 71 70 6f 6e 6d 6c 6b 6a 69 68 67 66 65 64 63 62 61 60 5f 5e 5d 5c 5b 5a 59 58 57 56 55 54 53 52 51 50 4f 4e 4d 4c 4b 4a 49 48 47 46 45 44 43 42 41 40 3f 3e 3d 3c 3b 3a 39 38 37 36 35 34 33 32 31 30 2f 2e 2d 2c 2b 2a 29 28 27 26 25 24 23 22 21 20 1f 1e 1d 1c 1b 1a 19 18 17 16 15 14 13 12 11 10 0f 0e 0d 0c 0b 0a 09 08 07 06 05 04 03 02 01 00 SPI slave says: read page 0 from the MOSI line: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff

If you look at the communication with a logic analyzer, you should see this:

Communicating as master and slave via SPI

**676**

Raspberry Pi Pico-series C/C++ SDK

_Figure 29. Data capture as seen in Saleae Logic._

**==> picture [319 x 138] intentionally omitted <==**

## **List of Files**

## **CMakeLists.txt**

CMake file to incorporate the example in to the examples build tree.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/spi_master_slave/CMakeLists.txt_

- 1 add_subdirectory_exclude_platforms(spi_master)

- 2 add_subdirectory_exclude_platforms(spi_slave)

## **spi_master/spi_master.c**

The example code for SPI master.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/spi_master_slave/spi_master/spi_master.c_

- 1 _// Copyright (c) 2021 Michael Stoops. All rights reserved._

- 2 _// Portions copyright (c) 2021 Raspberry Pi (Trading) Ltd._

- 3 _//_

- 4 _// Redistribution and use in source and binary forms, with or without modification, are_

   - _permitted provided that the_

- 5 _// following conditions are met:_

- 6 _//_

- 7 _// 1. Redistributions of source code must retain the above copyright notice, this list of_

   - _conditions and the following_

- 8 _//    disclaimer._

- 9 _// 2. Redistributions in binary form must reproduce the above copyright notice, this list of_

   - _conditions and the_

- 10 _//    following disclaimer in the documentation and/or other materials provided with the distribution._

- 11 _// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote_

- 12 _//    products derived from this software without specific prior written permission._ 13 _//_

- 14 _// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,_

- 15 _// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE_

- 16 _// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,_

- 17 _// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR_

- 18 _// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,_

- 19 _// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE_

Communicating as master and slave via SPI

**677**

Raspberry Pi Pico-series C/C++ SDK

20 _// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE._ 21 _//_ 22 _// SPDX-License-Identifier: BSD-3-Clause_ 23 _//_ 24 _// Example of an SPI bus master using the PL022 SPI interface_ 25 26 _#include <stdio.h>_ 27 _#include "pico/stdlib.h"_ 28 _#include "pico/binary_info.h"_ 29 _#include "hardware/spi.h"_ 30 31 _#define BUF_LEN         0x100_ 32 33 void printbuf(uint8_t buf[], size_t len) { 34     size_t i; 35     for (i = 0; i < len; ++i) { 36         if (i % 16 == 15) 37             printf("%02x\n", buf[i]); 38         else 39             printf("%02x ", buf[i]); 40     } 41 42 _// append trailing newline if there isn't one_ 43     if (i % 16) { 44         putchar('\n'); 45     } 46 } 47 48 int main() { 49 _// Enable UART so we can print_ 50     stdio_init_all(); 51 _#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)_ 52 _#warning spi/spi_master example requires a board with SPI pins_ 53     puts("Default SPI pins were not defined"); 54 _#else_ 55 56     printf("SPI master example\n"); 57 58 _// Enable SPI 0 at 1 MHz and connect to GPIOs_ 59     spi_init(spi_default, 1000 * 1000); 60     gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI); 61     gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI); 62     gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI); 63     gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI); 64 _// Make the SPI pins available to picotool_ 65     bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI)); 66 67     uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN]; 68 69 _// Initialize output buffer_ 70     for (size_t i = 0; i < BUF_LEN; ++i) { 71         out_buf[i] = i; 72     } 73 74     printf("SPI master says: The following buffer will be written to MOSI endlessly:\n"); 75     printbuf(out_buf, BUF_LEN); 76 77     for (size_t i = 0; ; ++i) { 78 _// Write the output buffer to MOSI, and at the same time read from MISO._ 79         spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN); 80

Communicating as master and slave via SPI

**678**

Raspberry Pi Pico-series C/C++ SDK

81 _// Write to stdio whatever came in on the MISO line._ 82         printf("SPI master says: read page %d from the MISO line:\n", i); 83         printbuf(in_buf, BUF_LEN); 84

85 _// Sleep for ten seconds so you get a chance to read the output._ 86         sleep_ms(10 * 1000); 87     } 88 _#endif_ 89 }

## **spi_slave/spi_slave.c**

The example code for SPI slave.

_Pico Examples: https://github.com/raspberrypi/pico-examples/blob/master/spi/spi_master_slave/spi_slave/spi_slave.c_

1 _// Copyright (c) 2021 Michael Stoops. All rights reserved._ 2 _// Portions copyright (c) 2021 Raspberry Pi (Trading) Ltd._ 3 _//_ 4 _// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the_ 5 _// following conditions are met:_ 6 _//_ 7 _// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following_ 8 _//    disclaimer._

- 9 _// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the_

- 10 _//    following disclaimer in the documentation and/or other materials provided with the distribution._

- 11 _// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote_

- 12 _//    products derived from this software without specific prior written permission._ 13 _//_

- 14 _// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,_

- 15 _// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE_

- 16 _// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,_

- 17 _// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR_

- 18 _// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,_

- 19 _// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE_

20 _// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE._ 21 _//_ 22 _// SPDX-License-Identifier: BSD-3-Clause_ 23 _//_ 24 _// Example of an SPI bus slave using the PL022 SPI interface_ 25 26 _#include <stdio.h>_ 27 _#include <string.h>_ 28 _#include "pico/stdlib.h"_ 29 _#include "pico/binary_info.h"_ 30 _#include "hardware/spi.h"_ 31 32 _#define BUF_LEN         0x100_ 33 34 void printbuf(uint8_t buf[], size_t len) { 35     size_t i; 36     for (i = 0; i < len; ++i) {

Communicating as master and slave via SPI

**679**

Raspberry Pi Pico-series C/C++ SDK

37         if (i % 16 == 15) 38             printf("%02x\n", buf[i]); 39         else 40             printf("%02x ", buf[i]); 41     } 42 43 _// append trailing newline if there isn't one_ 44     if (i % 16) { 45         putchar('\n'); 46     } 47 } 48 49 50 int main() { 51 _// Enable UART so we can print_ 52     stdio_init_all(); 53 _#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)_ 54 _#warning spi/spi_slave example requires a board with SPI pins_ 55     puts("Default SPI pins were not defined"); 56 _#else_ 57 58     printf("SPI slave example\n"); 59 60 _// Enable SPI 0 at 1 MHz and connect to GPIOs_ 61     spi_init(spi_default, 1000 * 1000); 62     spi_set_slave(spi_default, true); 63     gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI); 64     gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI); 65     gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI); 66     gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI); 67 _// Make the SPI pins available to picotool_ 68     bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI)); 69 70     uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN]; 71 72 _// Initialize output buffer_ 73     for (size_t i = 0; i < BUF_LEN; ++i) { 74 _// bit-inverted from i. The values should be: {0xff, 0xfe, 0xfd...}_ 75         out_buf[i] = ~i; 76     } 77 78     printf("SPI slave says: When reading from MOSI, the following buffer will be written to MISO:\n"); 79     printbuf(out_buf, BUF_LEN); 80 81     for (size_t i = 0; ; ++i) { 82 _// Write the output buffer to MISO, and at the same time read from MOSI._ 83         spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN); 84 85 _// Write to stdio whatever came in on the MOSI line._ 86         printf("SPI slave says: read page %d from the MOSI line:\n", i); 87         printbuf(in_buf, BUF_LEN); 88     } 89 _#endif_ 90 }

Communicating as master and slave via SPI

**680**

Raspberry Pi Pico-series C/C++ SDK

## **Bill of Materials**

_Table 56. A list of materials required for the example_

|**Bill of Materials**|||
|---|---|---|
|**Item**|**Quantity**|Details|
|Breadboard|1|generic part|
|Raspberry Pi Pico|2|https://www.raspberrypi.com/<br>products/raspberry-pi-pico/|
|M/M Jumper wires|8|generic part|



Communicating as master and slave via SPI

**681**
