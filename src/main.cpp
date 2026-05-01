#include <Arduino.h>

// On XIAO RP2040:
// RED LED is on GPIO 17
// GREEN LED is on GPIO 16
// BLUE LED is on GPIO 25
// (from seeed_xiao_rp2040.repl)
// NOTE: LEDs on XIAO RP2040 are active-low.

const int LED_PIN = 17; // RED LED
bool ledState = false;

void setup() {
  // Use Serial1 for hardware UART output (mapped to sysbus.uart0 in Renode)
  Serial1.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Start with LED OFF

  // Configure ADC
  analogReadResolution(12);

  Serial1.println("UART Bidirectional Communication Ready");
}

void loop() {
  if (Serial1.available() > 0) {
    char incomingByte = Serial1.read();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? LOW : HIGH);

    if (incomingByte == 'A') {
      int adcValue = analogRead(A0);
      Serial1.print("ADC0: ");
      Serial1.println(adcValue);
    } else if (incomingByte == 'P') {
      static int pwmValue = 0;
      pwmValue = (pwmValue + 64) % 256;
      analogWrite(LED_PIN, pwmValue);
      Serial1.print("PWM set to: ");
      Serial1.println(pwmValue);
    } else {
      Serial1.print("Echo: ");
      Serial1.println(incomingByte);
    }
  }

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 1000) {
    Serial1.println("Hello from XIAO RP2040!");
    lastMsg = millis();
  }
}
