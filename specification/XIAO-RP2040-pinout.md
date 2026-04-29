# XIAO RP2040 Pinout

Based on [Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAO-RP2040/):

## Pin Mapping
| XIAO Pin | Function | Chip Pin | Description |
|----------|----------|----------|-------------|
| D0 | Analog | GPIO26 | GPIO, ADC |
| D1 | Analog | GPIO27 | GPIO, ADC |
| D2 | Analog | GPIO28 | GPIO, ADC |
| D3 | Analog | GPIO29 | GPIO, ADC |
| D4 | SDA | GPIO6 | GPIO, I2C Data |
| D5 | SCL | GPIO7 | GPIO, I2C Clock |
| D6 | TX | GPIO0 | GPIO, UART Transmit |
| D7 | RX | GPIO1 | GPIO, UART Receive |
| D8 | SCK | GPIO2 | GPIO, SPI Clock |
| D9 | MISO | GPIO4 | GPIO, SPI Data |
| D10 | MOSI | GPIO3 | GPIO, SPI Data |

## Onboard LEDs
| LED | Chip Pin | Polarity |
|-----|----------|----------|
| USER_LED_R (Red) | GPIO17 | Active Low |
| USER_LED_G (Green) | GPIO16 | Active Low |
| USER_LED_B (Blue) | GPIO25 | Active Low |
| RGB LED (WS2812) | GPIO12 | - |
