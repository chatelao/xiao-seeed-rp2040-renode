#include <Arduino.h>

void setup() {
  // Serial1 is mapped to the hardware UART0 on XIAO RP2040 (GPIO 0/1)
  Serial1.begin(115200);
  // RP2040 ADC is 12-bit
  analogReadResolution(12);
}

void loop() {
  // A0 is GPIO26 on XIAO RP2040
  int raw = analogRead(A0);
  float voltage = raw * 3.3 / 4095.0;

  Serial1.print("ADC Raw: ");
  Serial1.print(raw);
  Serial1.print(", Voltage: ");
  Serial1.println(voltage, 2);

  delay(1000);
}
