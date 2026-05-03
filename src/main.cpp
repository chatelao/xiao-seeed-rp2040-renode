#include <Arduino.h>
#include "hardware/timer.h"
#include "pico/time.h"

// On XIAO RP2040:
// RED LED is on GPIO 17
// GREEN LED is on GPIO 16
// BLUE LED is on GPIO 25
// (from seeed_xiao_rp2040.repl)
// NOTE: LEDs on XIAO RP2040 are active-low.

const int LED_PIN = 17; // RED LED
const int INTERRUPT_PIN = 2; // XIAO D8
bool ledState = false;
volatile bool interruptOccurred = false;
volatile bool periodicTimerActive = false;
int alarmId = -1;

void handleInterrupt() {
  interruptOccurred = true;
}

void on_timer_alarm(uint alarm_num) {
  Serial1.println("Timer Alarm Handled");
  if (periodicTimerActive) {
      hardware_alarm_set_target(alarm_num, make_timeout_time_ms(100));
  }
}

void setup() {
  // Use Serial1 for hardware UART output (mapped to sysbus.uart0 in Renode)
  Serial1.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Start with LED OFF

  // Configure Interrupt Pin
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);

  // Configure ADC
  analogReadResolution(12);

  // Configure Timer Alarm
  alarmId = hardware_alarm_claim_unused(true);
  hardware_alarm_set_callback(alarmId, on_timer_alarm);

  Serial1.println("UART Bidirectional Communication Ready");
}

void loop() {
  if (interruptOccurred) {
    interruptOccurred = false;
    Serial1.println("GPIO Interrupt Handled");
  }

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
    } else if (incomingByte == 'T') {
      periodicTimerActive = false;
      hardware_alarm_set_target(alarmId, make_timeout_time_ms(100));
      Serial1.println("One-shot Timer Alarm Set for 100ms");
    } else if (incomingByte == 'U') {
      periodicTimerActive = !periodicTimerActive;
      if (periodicTimerActive) {
          hardware_alarm_set_target(alarmId, make_timeout_time_ms(100));
          Serial1.println("Periodic Timer Started (100ms)");
      } else {
          hardware_alarm_cancel(alarmId);
          Serial1.println("Periodic Timer Stopped");
      }
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
