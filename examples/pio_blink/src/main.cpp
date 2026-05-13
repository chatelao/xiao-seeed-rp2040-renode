#include <Arduino.h>
#include "hardware/pio.h"
#include "blink.pio.h"

// RED LED on XIAO RP2040
const int LED_PIN = 17;

void setup() {
    Serial1.begin(115200);
    while (!Serial1);
    Serial1.println("PIO Blink Example Started");

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &blink_program);

    Serial1.printf("Loaded PIO program at offset %u\n", offset);

    blink_program_init(pio, sm, offset, LED_PIN);

    // Set blink delay (cycles at 125MHz, 0.5s = 62,500,000)
    // We'll use a smaller value for faster simulation verification
    pio_sm_put_blocking(pio, sm, 1000000);
    pio_sm_set_enabled(pio, sm, true);

    Serial1.println("PIO Blinking Enabled");
}

void loop() {
    static uint32_t last_msg = 0;
    if (millis() - last_msg > 2000) {
        Serial1.println("PIO Blink Example Running");
        last_msg = millis();
    }
}
