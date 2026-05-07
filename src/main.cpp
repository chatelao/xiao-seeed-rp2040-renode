#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/watchdog.h"
#include "hardware/rtc.h"
#include "pico/time.h"
#include "pico/util/datetime.h"
#include "blink.pio.h"

// On XIAO RP2040:
// RED LED is on GPIO 17
// GREEN LED is on GPIO 16
// BLUE LED is on GPIO 25
// (from seeed_xiao_rp2040.repl)
// NOTE: LEDs on XIAO RP2040 are active-low.

const int LED_PIN = 17; // RED LED
const int GREEN_LED_PIN = 16;
const int INTERRUPT_PIN = 21; // XIAO D6 (GPIO 21) - Moved from D8 (GPIO 2) to avoid SPI0 SCK conflict
bool ledState = false;
bool pioRunning = false;
PIO pio = pio0;
uint sm = 0;
volatile bool interruptOccurred = false;
volatile bool periodicTimerActive = false;
volatile int alarmCount = 0;
volatile bool pwmInterruptOccurred = false;
volatile bool rtcInterruptOccurred = false;
volatile bool dmaInterruptOccurred = false;
volatile bool syncAdcEnabled = false;
volatile uint16_t lastSyncAdcValue = 0;
int alarmId = -1;

void handleInterrupt() {
  interruptOccurred = true;
}

void on_pwm_interrupt() {
    pwm_clear_irq(pwm_gpio_to_slice_num(LED_PIN));
    pwmInterruptOccurred = true;
    if (syncAdcEnabled) {
        // We assume AINSEL is already set to 1 by the 'H' command
        lastSyncAdcValue = adc_read();
    }
}

void on_rtc_interrupt() {
    rtcInterruptOccurred = true;
}

void on_dma_interrupt() {
    // Clear the interrupt on the channel that triggered it
    // The bits in INTS0 are cleared by writing to the corresponding bits in INTR
    uint32_t ints = dma_hw->ints0;
    dma_hw->intr = ints;
    dmaInterruptOccurred = true;
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

  if (watchdog_caused_reboot()) {
    Serial1.println("Watchdog Reboot Detected");
  }

  // Initialize I2C
  Wire.begin();

  // Initialize RTC
  rtc_init();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Start with LED OFF

  // Configure Interrupt Pin
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);

  // Configure ADC
  adc_init();
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

  // Initialize PIO
  uint offset = pio_add_program(pio, &blink_program);
  blink_program_init(pio, sm, offset, GREEN_LED_PIN);
}

void loop() {
  if (interruptOccurred) {
    interruptOccurred = false;
    Serial1.println("GPIO Interrupt Handled");
    Serial1.flush();
  }

  if (pwmInterruptOccurred) {
    pwmInterruptOccurred = false;
    if (!syncAdcEnabled) {
      Serial1.println("PWM Interrupt Handled");
      Serial1.flush();
    }
  }

  if (rtcInterruptOccurred) {
    rtcInterruptOccurred = false;
    Serial1.println("RTC Alarm Handled");
    Serial1.flush();
  }

  if (dmaInterruptOccurred) {
    dmaInterruptOccurred = false;
    Serial1.println("DMA Interrupt Handled");
    Serial1.flush();
  }

  while (alarmCount > 0) {
    alarmCount--;
    Serial1.println("Timer Alarm Handled");
    Serial1.flush();
  }

  if (Serial1.available() > 0) {
    char incomingByte = Serial1.read();

    if (incomingByte == 'A') {
      adc_select_input(0);
      int adcValue = adc_read();
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
      // Using SET alias (base + 0x2000) for safety
      volatile uint32_t *spi0_cr1_set = (volatile uint32_t *)(0x4003c000 + 0x2000 + 0x4);
      *spi0_cr1_set = 0x1;

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
      // Disable loopback using CLEAR alias (base + 0x3000)
      volatile uint32_t *spi0_cr1_clr = (volatile uint32_t *)(0x4003c000 + 0x3000 + 0x4);
      *spi0_cr1_clr = 0x1;
      SPI.end();
      Serial1.flush();
    } else if (incomingByte == 'E') {
      // Test ADC Error Detection
      adc_hw->cs = (adc_hw->cs & ~ADC_CS_AINSEL_BITS) | (0 << ADC_CS_AINSEL_LSB);
      adc_hw->cs |= ADC_CS_START_ONCE_BITS;
      adc_hw->cs = (adc_hw->cs & ~ADC_CS_AINSEL_BITS) | (1 << ADC_CS_AINSEL_LSB); // Trigger error

      while (!(adc_hw->cs & ADC_CS_READY_BITS));

      bool err = (adc_hw->cs & ADC_CS_ERR_BITS) != 0;
      bool err_sticky = (adc_hw->cs & ADC_CS_ERR_STICKY_BITS) != 0;
      Serial1.printf("ADC Error Test: ERR=%d ERR_STICKY=%d\n", err ? 1 : 0, err_sticky ? 1 : 0);

      adc_hw->cs |= ADC_CS_ERR_STICKY_BITS;
      Serial1.flush();
    } else if (incomingByte == 'F') {
      // Test ADC FIFO Error bit
      adc_fifo_setup(true, false, 1, true, false);
      adc_hw->cs = (adc_hw->cs & ~ADC_CS_AINSEL_BITS) | (0 << ADC_CS_AINSEL_LSB);
      adc_hw->cs |= ADC_CS_START_ONCE_BITS;
      adc_hw->cs = (adc_hw->cs & ~ADC_CS_AINSEL_BITS) | (1 << ADC_CS_AINSEL_LSB); // Trigger error

      while (adc_fifo_get_level() == 0);
      uint16_t val = adc_fifo_get();
      Serial1.printf("ADC FIFO Error Test: VAL=0x%04X\n", val);

      adc_fifo_setup(false, false, 0, false, false);
      adc_hw->cs |= ADC_CS_ERR_STICKY_BITS;
      Serial1.flush();
    } else if (incomingByte == 'B') {
      pioRunning = !pioRunning;
      if (pioRunning) {
        // Set blink delay (number of cycles)
        // At 125MHz, 62,500,000 cycles = 0.5 seconds
        pio_sm_put_blocking(pio, sm, 62500000);
        pio_sm_set_enabled(pio, sm, true);
        Serial1.println("PIO Blinking Started");
      } else {
        pio_sm_set_enabled(pio, sm, false);
        // Important: Re-initialize GPIO to take it back from PIO
        gpio_init(GREEN_LED_PIN);
        pinMode(GREEN_LED_PIN, OUTPUT);
        digitalWrite(GREEN_LED_PIN, HIGH); // Off
        Serial1.println("PIO Blinking Stopped");
      }
      Serial1.flush();
    } else if (incomingByte == 'W') {
      watchdog_enable(500, 1);
      Serial1.println("Watchdog Enabled (500ms)");
      Serial1.flush();
    } else if (incomingByte == 'K') {
      watchdog_update();
      Serial1.println("Watchdog Kicked");
      Serial1.flush();
    } else if (incomingByte == 'M') {
      // PWM Interrupt Test
      uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
      pwm_clear_irq(slice_num);
      pwm_set_irq_enabled(slice_num, true);
      irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_interrupt);
      irq_set_enabled(PWM_IRQ_WRAP, true);

      pwm_config config = pwm_get_default_config();
      pwm_config_set_clkdiv(&config, 100.0f);
      pwm_config_set_wrap(&config, 5000);
      pwm_init(slice_num, &config, true);

      Serial1.print("PWM Interrupt Enabled for Slice ");
      Serial1.println(slice_num);
      Serial1.flush();
    } else if (incomingByte == 'G') {
      adc_select_input(1);
      int motorAdc = adc_read();
      Serial1.print("Motor ADC: ");
      Serial1.println(motorAdc);
      Serial1.flush();
    } else if (incomingByte == 'H') {
      syncAdcEnabled = !syncAdcEnabled;
      if (syncAdcEnabled) {
          adc_select_input(1);
      }
      Serial1.print("Sync ADC ");
      Serial1.println(syncAdcEnabled ? "Enabled" : "Disabled");
      Serial1.flush();
    } else if (incomingByte == 'V') {
      Serial1.print("Last Sync ADC: ");
      Serial1.println(lastSyncAdcValue);
      Serial1.flush();
    } else if (incomingByte == 'R') {
      // RTC Test: Set and Get time
      datetime_t t = {
          .year  = 2024,
          .month = 05,
          .day   = 06,
          .dotw  = 1, // Monday
          .hour  = 12,
          .min   = 00,
          .sec   = 00
      };
      rtc_set_datetime(&t);
      sleep_us(64); // Wait for clock domain crossing

      rtc_get_datetime(&t);
      char datetime_buf[64];
      datetime_to_str(datetime_buf, sizeof(datetime_buf), &t);
      Serial1.print("RTC Time Set: ");
      Serial1.println(datetime_buf);

      // Simple wait to see it increment (in a real test we'd poll or use alarms)
      delay(1100);
      rtc_get_datetime(&t);
      datetime_to_str(datetime_buf, sizeof(datetime_buf), &t);
      Serial1.print("RTC Time After 1s: ");
      Serial1.println(datetime_buf);
      Serial1.flush();
    } else if (incomingByte == 'Q') {
      // RTC Alarm Test
      datetime_t t = {
          .year  = 2024,
          .month = 05,
          .day   = 06,
          .dotw  = 1,
          .hour  = 12,
          .min   = 00,
          .sec   = 00
      };
      rtc_set_datetime(&t);
      sleep_us(64);

      datetime_t alarm = {
          .year  = -1,
          .month = -1,
          .day   = -1,
          .dotw  = -1,
          .hour  = -1,
          .min   = -1,
          .sec   = 02
      };

      Serial1.println("Setting RTC Alarm...");
      Serial1.flush();
      rtc_set_alarm(&alarm, on_rtc_interrupt);
      Serial1.println("RTC Alarm Set for +2s");
      Serial1.flush();
    } else if (incomingByte == 'J') {
      // External SPI Flash Test (JEDEC ID)
      SPI.setSCK(2);
      SPI.setTX(3);
      SPI.setRX(4);
      SPI.setCS(1);
      SPI.begin();

      // Ensure loopback is OFF using CLEAR alias
      volatile uint32_t *spi0_cr1_clr = (volatile uint32_t *)(0x4003c000 + 0x3000 + 0x4);
      *spi0_cr1_clr = 0x1;

      // JEDEC ID command is 0x9F
      // We expect 3 bytes: Manufacturer ID (0xEF), Memory Type, Capacity

      SPI.transfer(0x9F);
      uint8_t m_id = SPI.transfer(0x00);
      uint8_t type = SPI.transfer(0x00);
      uint8_t cap = SPI.transfer(0x00);

      Serial1.print("Flash JEDEC ID: ");
      if (m_id < 0x10) Serial1.print("0");
      Serial1.print(m_id, HEX);
      Serial1.print(" ");
      if (type < 0x10) Serial1.print("0");
      Serial1.print(type, HEX);
      Serial1.print(" ");
      if (cap < 0x10) Serial1.print("0");
      Serial1.println(cap, HEX);

      SPI.end();
      Serial1.flush();
    } else if (incomingByte == 'D') {
      // DMA Memory-to-Memory Test
      const char *src_str = "DMA TRANSFER TEST";
      char dst_str[32] = {0};

      int dma_chan = dma_claim_unused_channel(true);
      dma_channel_config c = dma_channel_get_default_config(dma_chan);
      channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
      channel_config_set_read_increment(&c, true);
      channel_config_set_write_increment(&c, true);

      dma_channel_configure(
          dma_chan,
          &c,
          dst_str,
          src_str,
          strlen(src_str),
          false
      );

      // Enable interrupt on DMA channel completion
      dma_channel_set_irq0_enabled(dma_chan, true);
      irq_set_exclusive_handler(DMA_IRQ_0, on_dma_interrupt);
      irq_set_enabled(DMA_IRQ_0, true);

      dma_channel_start(dma_chan);

      // Wait for completion (in a real app we'd use the interrupt, here we poll and check interrupt flag)
      dma_channel_wait_for_finish_blocking(dma_chan);

      if (strcmp(src_str, dst_str) == 0) {
          Serial1.print("DMA Transfer Success: ");
          Serial1.println(dst_str);
      } else {
          Serial1.print("DMA Transfer Failed: Got '");
          Serial1.print(dst_str);
          Serial1.println("'");
      }
      dma_channel_unclaim(dma_chan);
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
