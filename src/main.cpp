#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "hardware/timer.h"
#include "pico/time.h"

// On XIAO RP2040:
// RED LED is on GPIO 17
// GREEN LED is on GPIO 16
// BLUE LED is on GPIO 25
// (from seeed_xiao_rp2040.repl)
// NOTE: LEDs on XIAO RP2040 are active-low.

const int LED_PIN = 17; // RED LED
const int INTERRUPT_PIN = 21; // XIAO D6 (GPIO 21) - Moved from D8 (GPIO 2) to avoid SPI0 SCK conflict
bool ledState = false;
volatile bool interruptOccurred = false;
volatile bool periodicTimerActive = false;
volatile int alarmCount = 0;
int alarmId = -1;

void handleInterrupt() {
  interruptOccurred = true;
}

void on_timer_alarm(uint alarm_num) {
  alarmCount++;
  if (periodicTimerActive) {
      hardware_alarm_set_target(alarm_num, make_timeout_time_ms(100));
  }
}

void setup() {
  // Use Serial1 for hardware UART output (mapped to sysbus.uart0 in Renode)
  Serial1.begin(115200);

  // Initialize I2C
  Wire.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Start with LED OFF

  // Configure Interrupt Pin
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);

  // Configure ADC
  analogReadResolution(12);

  // Configure Timer Alarm
  alarmId = hardware_alarm_claim_unused(true);
  if (alarmId >= 0) {
      hardware_alarm_set_callback(alarmId, on_timer_alarm);
      Serial1.print("Claimed Alarm ID: ");
      Serial1.println(alarmId);
  } else {
      Serial1.println("Failed to claim a hardware alarm!");
  }

  Serial1.println("UART Bidirectional Communication Ready");
  Serial1.flush();
}

void loop() {
  if (interruptOccurred) {
    interruptOccurred = false;
    Serial1.println("GPIO Interrupt Handled");
    Serial1.flush();
  }

  while (alarmCount > 0) {
    alarmCount--;
    Serial1.println("Timer Alarm Handled");
    Serial1.flush();
  }

  if (Serial1.available() > 0) {
    char incomingByte = Serial1.read();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? LOW : HIGH);

    if (incomingByte == 'A') {
      int adcValue = analogRead(A0);
      Serial1.print("ADC0: ");
      Serial1.println(adcValue);
      Serial1.flush();
    } else if (incomingByte == 'I') {
      Wire.beginTransmission(0x76);
      Wire.write(0xD0); // REG_ID
      Wire.endTransmission(false);
      Wire.requestFrom(0x76, 1);
      if (Wire.available()) {
        byte id = Wire.read();
        Serial1.print("BMP280 ID: 0x");
        Serial1.println(id, HEX);
      } else {
        Serial1.println("BMP280 not found");
      }
      Serial1.flush();
    } else if (incomingByte == 'P') {
      static int pwmValue = 0;
      pwmValue = (pwmValue + 64) % 256;
      analogWrite(LED_PIN, pwmValue);
      Serial1.print("PWM set to: ");
      Serial1.println(pwmValue);
      Serial1.flush();
    } else if (incomingByte == 'T') {
      if (alarmId >= 0) {
          periodicTimerActive = false;
          hardware_alarm_set_target(alarmId, make_timeout_time_ms(100));
          Serial1.println("One-shot Timer Alarm Set for 100ms");
      } else {
          Serial1.println("Timer Error: No alarm claimed");
      }
      Serial1.flush();
    } else if (incomingByte == 'U') {
      if (alarmId >= 0) {
          periodicTimerActive = !periodicTimerActive;
          if (periodicTimerActive) {
              hardware_alarm_set_target(alarmId, make_timeout_time_ms(100));
              Serial1.println("Periodic Timer Started (100ms)");
          } else {
              hardware_alarm_cancel(alarmId);
              Serial1.println("Periodic Timer Stopped");
          }
      } else {
          Serial1.println("Timer Error: No alarm claimed");
      }
      Serial1.flush();
    } else if (incomingByte == 'S') {
      // SPI Loopback Test
      SPI.begin();

      // Manually enable loopback mode in the SPI0 peripheral (PL022)
      // SSPCR1 (0x4) bit 0 is LBM (Loop Back Mode)
      // On RP2040, SPI0 base is 0x4003c000
      volatile uint32_t *spi0_cr1 = (volatile uint32_t *)(0x4003c000 + 0x4);
      *spi0_cr1 |= 0x1;

      uint8_t testByte = 0xBC;
      uint8_t receivedByte = SPI.transfer(testByte);

      if (receivedByte == testByte) {
        Serial1.print("SPI Loopback Success: 0x");
        Serial1.println(receivedByte, HEX);
      } else {
        Serial1.print("SPI Loopback Failed: Sent 0x");
        Serial1.print(testByte, HEX);
        Serial1.print(", Got 0x");
        Serial1.println(receivedByte, HEX);
      }
      SPI.end();
      Serial1.flush();
    } else {
      Serial1.print("Echo: ");
      Serial1.println(incomingByte);
      Serial1.flush();
    }
  }

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 1000) {
    Serial1.println("Hello from XIAO RP2040!");
    Serial1.flush();
    lastMsg = millis();
  }
}
