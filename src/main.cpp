#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Hello from XIAO RP2040!");
  delay(1000);
}
