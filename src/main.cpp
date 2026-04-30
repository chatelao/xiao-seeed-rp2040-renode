#include <Arduino.h>

// On XIAO RP2040:
// RED LED is on GPIO 17
// GREEN LED is on GPIO 16
// BLUE LED is on GPIO 25
// (from seeed_xiao_rp2040.repl)
// NOTE: LEDs on XIAO RP2040 are active-low.

const int LED_PIN = 17; // RED LED

void setup() {
  // Use Serial1 for hardware UART output (mapped to sysbus.uart0 in Renode)
  Serial1.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  static bool state = false;
  state = !state;

  // Active-low: LOW = ON, HIGH = OFF
  digitalWrite(LED_PIN, state ? LOW : HIGH);

  Serial1.print("Hello from XIAO RP2040! LED is ");
  Serial1.println(state ? "ON" : "OFF");

  delay(1000);
}
