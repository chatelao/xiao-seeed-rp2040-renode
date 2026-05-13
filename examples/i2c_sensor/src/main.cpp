#include <Arduino.h>
#include <Wire.h>

void setup() {
    Serial1.begin(115200);
    while (!Serial1);
    Serial1.println("I2C Sensor Example Started");

    // Initialize I2C on default pins (SDA=GP6, SCL=GP7 for XIAO)
    // Note: Renode wiring must match these pins or the controller used.
    // By default, XIAO RP2040 uses i2c1 for the default Wire instance.
    Wire.begin();

    Serial1.println("Scanning I2C for BMP280 (0x76)...");

    // BMP280 Chip ID register is 0xD0. Expected value is 0x58.
    Wire.beginTransmission(0x76);
    Wire.write(0xD0);
    byte error = Wire.endTransmission();

    if (error == 0) {
        Wire.requestFrom(0x76, 1);
        if (Wire.available()) {
            byte id = Wire.read();
            Serial1.print("Found BMP280. Chip ID: 0x");
            Serial1.println(id, HEX);

            if (id == 0x58) {
                Serial1.println("I2C Sensor Success!");
            } else {
                Serial1.println("Unexpected Chip ID!");
            }
        }
    } else {
        Serial1.print("BMP280 not found at 0x76. Error: ");
        Serial1.println(error);
    }
}

void loop() {
    static uint32_t last_msg = 0;
    if (millis() - last_msg > 5000) {
        Serial1.println("I2C Sensor Example Idle");
        last_msg = millis();
    }
}
