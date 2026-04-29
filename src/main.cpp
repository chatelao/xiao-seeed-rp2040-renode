#include <Arduino.h>

void setup() {
    Serial1.begin(115200);
    pinMode(25, OUTPUT); // Blue LED
}

void loop() {
    Serial1.println("Hello, XIAO RP2040!");
    digitalWrite(25, LOW); // Active Low: LOW is ON
    delay(500);
    digitalWrite(25, HIGH); // Active Low: HIGH is OFF
    delay(500);
}
