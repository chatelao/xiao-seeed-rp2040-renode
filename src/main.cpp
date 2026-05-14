/**
 * @file main.cpp
 * @brief RP2040 Feature Verification and Motor Control Firmware.
 *
 * This firmware is designed for testing and verifying various RP2040 peripherals
 * in the Renode simulation environment. It includes tests for:
 * - GPIO and external interrupts
 * - ADC (including error detection and synchronized reads)
 * - PWM (including interrupts, double buffering, DIVMODE, and phase adjustment)
 * - DMA (memory-to-memory, pacing, ring buffers, sniffer, and debug registers)
 * - PIO (simple blinking program)
 * - RTC (date/time set/get and alarms)
 * - Watchdog (enable, update, and reboot detection)
 * - I2C (BMP280 identification)
 * - SPI (loopback mode)
 * - PID control for motor speed simulation using bEMF feedback.
 */

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

// Hardware Pin Definitions (XIAO RP2040)
const int LED_PIN = 17;        ///< RED LED (Active-Low)
const int GREEN_LED_PIN = 16;  ///< GREEN LED (Active-Low)
const int INTERRUPT_PIN = 21;  ///< GPIO for external interrupt testing (XIAO D6)

// Global State Variables
bool ledState = false;         ///< Current state of the RED LED
bool pioRunning = false;       ///< Whether the PIO blinking program is active
PIO pio = pio0;                ///< PIO instance used
uint sm = 0;                   ///< State machine index for PIO

// Interrupt and Event Flags (volatile for ISR safety)
volatile bool interruptOccurred = false;    ///< Flag for GPIO interrupt
volatile bool periodicTimerActive = false;  ///< Flag for periodic timer status
volatile int alarmCount = 0;                ///< Counter for timer alarms
volatile bool pwmInterruptOccurred = false; ///< Flag for PWM wrap interrupt
volatile bool rtcInterruptOccurred = false; ///< Flag for RTC alarm interrupt
volatile bool dmaInterruptOccurred = false; ///< Flag for DMA transfer completion
volatile bool syncAdcEnabled = false;       ///< Flag for PWM-ADC synchronization
volatile uint16_t lastSyncAdcValue = 0;     ///< Last captured synchronized ADC value
int alarmId = -1;                           ///< ID of the claimed hardware alarm

/**
 * @brief PID Controller structure for motor speed regulation.
 */
struct PidController {
    volatile float kp = 0.5f;         ///< Proportional gain
    volatile float ki = 0.05f;        ///< Integral gain
    volatile float kd = 0.01f;        ///< Derivative gain
    volatile uint16_t target = 1000;  ///< Target ADC value (speed)
    volatile float integral = 0;      ///< Accumulated integral error
    volatile float prev_error = 0;    ///< Previous error for derivative calculation
    volatile bool enabled = false;    ///< Whether PID control is active
    volatile bool telemetry = false;  ///< Whether to output PID telemetry
    volatile uint16_t last_output = 0;///< Last calculated PWM output
} pid;

/**
 * @brief Updates the PID controller with a new measurement.
 *
 * @param actual The current ADC measurement.
 * @return uint16_t The calculated PWM output (0-255).
 */
uint16_t update_pid(uint16_t actual) {
    if (!pid.enabled) return 0;

    float error = (float)pid.target - (float)actual;
    pid.integral += error;

    // Simple anti-windup: limit integral term
    if (pid.integral > 1000) pid.integral = 1000;
    if (pid.integral < -1000) pid.integral = -1000;

    float derivative = error - pid.prev_error;
    float output = (pid.kp * error) + (pid.ki * pid.integral) + (pid.kd * derivative);
    pid.prev_error = error;

    // Constrain output to 8-bit PWM range
    int pwm_out = (int)output;
    if (pwm_out > 255) pwm_out = 255;
    if (pwm_out < 0) pwm_out = 0;

    pid.last_output = (uint16_t)pwm_out;
    return pid.last_output;
}

/**
 * @brief ISR for GPIO external interrupt.
 */
void handleInterrupt() {
  interruptOccurred = true;
}

/**
 * @brief ISR for PWM wrap interrupt.
 *
 * Clears the interrupt and optionally triggers a synchronized ADC read
 * and PID update for motor control simulation.
 */
void on_pwm_interrupt() {
    pwm_clear_irq(pwm_gpio_to_slice_num(LED_PIN));
    pwmInterruptOccurred = true;
    if (syncAdcEnabled) {
        // Perform synchronized ADC read (triggered on PWM wrap)
        lastSyncAdcValue = adc_read();
        if (pid.enabled) {
            uint16_t output = update_pid(lastSyncAdcValue);
            // Scale 8-bit PID output to the 5000 wrap value used in 'M' command
            pwm_set_gpio_level(LED_PIN, (uint32_t)output * 5000 / 255);
        }
    }
}

/**
 * @brief ISR for RTC alarm interrupt.
 */
void on_rtc_interrupt() {
    rtcInterruptOccurred = true;
}

/**
 * @brief ISR for DMA interrupt.
 *
 * Clears the interrupt on the channel that triggered it.
 */
void on_dma_interrupt() {
    uint32_t ints = dma_hw->ints0;
    dma_hw->intr = ints; // Writing to INTR clears bits in INTS0
    dmaInterruptOccurred = true;
}

/**
 * @brief Callback for hardware timer alarm.
 *
 * @param alarm_num The alarm number that triggered.
 */
void on_timer_alarm(uint alarm_num) {
  alarmCount++;
  if (periodicTimerActive) {
      // Re-schedule the alarm for periodic behavior
      hardware_alarm_set_target(alarm_num, make_timeout_time_ms(100));
  }
}

/**
 * @brief Arduino setup function.
 *
 * Initializes serial communication, I2C, RTC, GPIOs, ADC, hardware timers, and PIO.
 */
void setup() {
  // Serial1 is mapped to sysbus.uart0 in Renode
  Serial1.begin(115200);

  // Check if the recent reset was caused by the watchdog
  if (watchdog_caused_reboot()) {
    Serial1.println("Watchdog Reboot Detected");
  }

  // Initialize I2C (sysbus.i2c0)
  Wire.begin();

  // Initialize RTC (sysbus.rtc)
  rtc_init();

  // Initialize LEDs
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Start with LED OFF (active-low)

  // Configure External Interrupt Pin
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);

  // Configure ADC
  adc_init();
  analogReadResolution(12);

  // Claim and configure a hardware alarm
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

  // Initialize PIO with a simple blinking program
  uint offset = pio_add_program(pio, &blink_program);
  blink_program_init(pio, sm, offset, GREEN_LED_PIN);
}

/**
 * @brief Arduino main loop.
 *
 * Handles interrupt flags, timer events, PID telemetry, and UART commands
 * for peripheral verification.
 */
void loop() {
  // Handle GPIO Interrupt
  if (interruptOccurred) {
    interruptOccurred = false;
    Serial1.println("GPIO Interrupt Handled");
    Serial1.flush();
  }

  // Handle PWM Interrupt
  if (pwmInterruptOccurred) {
    pwmInterruptOccurred = false;
    if (!syncAdcEnabled) {
      Serial1.println("PWM Interrupt Handled");
      Serial1.flush();
    }
  }

  // Handle RTC Alarm
  if (rtcInterruptOccurred) {
    rtcInterruptOccurred = false;
    Serial1.println("RTC Alarm Handled");
    Serial1.flush();
  }

  // Handle DMA Interrupt
  if (dmaInterruptOccurred) {
    dmaInterruptOccurred = false;
    Serial1.println("DMA Interrupt Handled");
    Serial1.flush();
  }

  // Output PID telemetry if enabled
  if (pid.telemetry && pid.enabled) {
    static unsigned long lastTelemetry = 0;
    if (millis() - lastTelemetry > 100) {
      Serial1.printf("TELE: T=%u A=%u E=%0.2f O=%u\n", pid.target, lastSyncAdcValue, pid.prev_error, pid.last_output);
      Serial1.flush();
      lastTelemetry = millis();
    }
  }

  // Handle Hardware Timer Alarms
  while (alarmCount > 0) {
    alarmCount--;
    Serial1.println("Timer Alarm Handled");
    Serial1.flush();
  }

  // Handle UART Commands
  if (Serial1.available() > 0) {
    char incomingByte = Serial1.read();

    if (incomingByte == 'A') {
      // Command 'A': Read ADC Channel 0
      adc_select_input(0);
      int adcValue = adc_read();
      Serial1.print("ADC0: ");
      Serial1.println(adcValue);
      Serial1.flush();
    } else if (incomingByte == 'I') {
      // Command 'I': Identify BMP280 via I2C
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
      // Command 'P': Set PWM value on LED_PIN using analogWrite
      static int pwmValue = 0;
      pwmValue = (pwmValue + 64) % 256;
      analogWrite(LED_PIN, pwmValue);
      Serial1.print("PWM set to: ");
      Serial1.println(pwmValue);
      Serial1.flush();
    } else if (incomingByte == 'T') {
      // Command 'T': Set a one-shot 100ms timer alarm
      if (alarmId >= 0) {
          periodicTimerActive = false;
          hardware_alarm_set_target(alarmId, make_timeout_time_ms(100));
          Serial1.println("One-shot Timer Alarm Set for 100ms");
      } else {
          Serial1.println("Timer Error: No alarm claimed");
      }
      Serial1.flush();
    } else if (incomingByte == 'U') {
      // Command 'U': Toggle a periodic 100ms timer alarm
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
      // Command 'S': SPI Loopback Test
      SPI.begin();

      // Manually enable loopback mode in the SPI0 peripheral (PL022 SSPCR1 LBM bit)
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
    } else if (incomingByte == 'E') {
      // Command 'E': Test ADC Error Detection
      // Trigger an error by writing to AINSEL while conversion is in progress
      adc_hw->cs = (adc_hw->cs & ~ADC_CS_AINSEL_BITS) | (0 << ADC_CS_AINSEL_LSB);
      adc_hw->cs |= ADC_CS_START_ONCE_BITS;
      adc_hw->cs = (adc_hw->cs & ~ADC_CS_AINSEL_BITS) | (1 << ADC_CS_AINSEL_LSB); // Trigger error

      while (!(adc_hw->cs & ADC_CS_READY_BITS));

      bool err = (adc_hw->cs & ADC_CS_ERR_BITS) != 0;
      bool err_sticky = (adc_hw->cs & ADC_CS_ERR_STICKY_BITS) != 0;
      Serial1.printf("ADC Error Test: ERR=%d ERR_STICKY=%d\n", err ? 1 : 0, err_sticky ? 1 : 0);

      adc_hw->cs |= ADC_CS_ERR_STICKY_BITS; // Clear sticky error
      Serial1.flush();
    } else if (incomingByte == 'F') {
      // Command 'F': Test ADC FIFO Error bit
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
      // Command 'B': Toggle PIO Blinking
      pioRunning = !pioRunning;
      if (pioRunning) {
        // Set blink delay (cycles at 125MHz, 0.5s)
        pio_sm_put_blocking(pio, sm, 62500000);
        pio_sm_set_enabled(pio, sm, true);
        Serial1.println("PIO Blinking Started");
      } else {
        pio_sm_set_enabled(pio, sm, false);
        // Restore GPIO for normal output use
        gpio_init(GREEN_LED_PIN);
        pinMode(GREEN_LED_PIN, OUTPUT);
        digitalWrite(GREEN_LED_PIN, HIGH); // Off
        Serial1.println("PIO Blinking Stopped");
      }
      Serial1.flush();
    } else if (incomingByte == 'W') {
      // Command 'W': Enable Watchdog (500ms timeout)
      watchdog_enable(500, 1);
      Serial1.println("Watchdog Enabled (500ms)");
      Serial1.flush();
    } else if (incomingByte == 'K') {
      // Command 'K': Kick (update) the Watchdog
      watchdog_update();
      Serial1.println("Watchdog Kicked");
      Serial1.flush();
    } else if (incomingByte == 'M') {
      // Command 'M': Enable PWM Wrap Interrupt
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
      // Command 'G': Read ADC Channel 1 (Motor bEMF simulation)
      adc_select_input(1);
      int motorAdc = adc_read();
      Serial1.print("Motor ADC: ");
      Serial1.println(motorAdc);
      Serial1.flush();
    } else if (incomingByte == 'H') {
      // Command 'H': Toggle synchronized ADC read on PWM interrupt
      syncAdcEnabled = !syncAdcEnabled;
      if (syncAdcEnabled) {
          adc_select_input(1);
      }
      Serial1.print("Sync ADC ");
      Serial1.println(syncAdcEnabled ? "Enabled" : "Disabled");
      Serial1.flush();
    } else if (incomingByte == 'V') {
      // Command 'V': Print last captured synchronized ADC value
      Serial1.print("Last Sync ADC: ");
      Serial1.println(lastSyncAdcValue);
      Serial1.flush();
    } else if (incomingByte == 'R') {
      // Command 'R': RTC Set and Get Date/Time test
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
      sleep_us(64); // Wait for clock domain sync

      rtc_get_datetime(&t);
      char datetime_buf[64];
      datetime_to_str(datetime_buf, sizeof(datetime_buf), &t);
      Serial1.print("RTC Time Set: ");
      Serial1.println(datetime_buf);

      delay(1100); // Wait 1.1s to verify increment
      rtc_get_datetime(&t);
      datetime_to_str(datetime_buf, sizeof(datetime_buf), &t);
      Serial1.print("RTC Time After 1s: ");
      Serial1.println(datetime_buf);
      Serial1.flush();
    } else if (incomingByte == 'Q') {
      // Command 'Q': RTC Alarm Test (triggers in 2 seconds)
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
          .year  = -1, .month = -1, .day   = -1, .dotw  = -1,
          .hour  = -1, .min   = -1, .sec   = 02
      };

      Serial1.println("Setting RTC Alarm...");
      Serial1.flush();
      rtc_set_alarm(&alarm, on_rtc_interrupt);
      Serial1.println("RTC Alarm Set for +2s");
      Serial1.flush();
    } else if (incomingByte == 'D') {
      // Command 'D': DMA Memory-to-Memory Transfer Test
      const char *src_str = "DMA TRANSFER TEST";
      char dst_str[32] = {0};

      int dma_chan = dma_claim_unused_channel(true);
      dma_channel_config c = dma_channel_get_default_config(dma_chan);
      channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
      channel_config_set_read_increment(&c, true);
      channel_config_set_write_increment(&c, true);

      dma_channel_configure(dma_chan, &c, dst_str, src_str, strlen(src_str), false);

      // Enable interrupt for this DMA channel
      dma_channel_set_irq0_enabled(dma_chan, true);
      irq_set_exclusive_handler(DMA_IRQ_0, on_dma_interrupt);
      irq_set_enabled(DMA_IRQ_0, true);

      dma_channel_start(dma_chan);
      dma_channel_wait_for_finish_blocking(dma_chan);

      if (strcmp(src_str, dst_str) == 0) {
          Serial1.print("DMA Transfer Success: ");
          Serial1.println(dst_str);
      } else {
          Serial1.print("DMA Transfer Failed");
      }
      dma_channel_unclaim(dma_chan);
      Serial1.flush();
    } else if (incomingByte == 'Z') {
      // Command 'Z': Read N_CHANNELS register (DMA Capability)
      volatile uint32_t *n_channels_reg = (volatile uint32_t *)(0x50000000 + 0x448);
      uint32_t n_channels = *n_channels_reg;
      Serial1.print("DMA Channels: ");
      Serial1.println(n_channels);
      Serial1.flush();
    } else if (incomingByte == 'Y') {
      // Command 'Y': DMA Pacing Timer Test
      const char *src_str = "PACED DMA";
      char dst_str[16] = {0};

      // Configure TIMER0 pacing (X=1, Y=1000)
      volatile uint32_t *timer0_reg = (volatile uint32_t *)(0x50000000 + 0x420);
      *timer0_reg = (1000 << 16) | 1;

      int dma_chan = dma_claim_unused_channel(true);
      dma_channel_config c = dma_channel_get_default_config(dma_chan);
      channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
      channel_config_set_read_increment(&c, true);
      channel_config_set_write_increment(&c, true);
      channel_config_set_dreq(&c, 0x3b); // DREQ_TIMER0 = 59

      dma_channel_configure(dma_chan, &c, dst_str, src_str, strlen(src_str), true);
      dma_channel_wait_for_finish_blocking(dma_chan);

      if (strcmp(src_str, dst_str) == 0) {
          Serial1.print("DMA Pacing Success: ");
          Serial1.println(dst_str);
      } else {
          Serial1.print("DMA Pacing Failed");
      }
      dma_channel_unclaim(dma_chan);
      Serial1.flush();
    } else if (incomingByte == 'J') {
      // Command 'J': PWM DIVMODE Test (RISE mode)
      uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
      pwm_set_enabled(slice_num, false);
      pwm_config config = pwm_get_default_config();
      pwm_config_set_clkdiv(&config, 1.0f);
      pwm_config_set_wrap(&config, 100);
      pwm_config_set_clkdiv_mode(&config, PWM_DIV_B_RISING);
      pwm_init(slice_num, &config, false);
      pwm_set_counter(slice_num, 0);
      pwm_set_enabled(slice_num, true);
      Serial1.println("PWM RISE Mode Enabled");
      Serial1.flush();
    } else if (incomingByte == 'N') {
      // Command 'N': Read PWM Counter
      uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
      Serial1.printf("PWM CTR: %u\n", (unsigned int)pwm_get_counter(slice_num));
      Serial1.flush();
    } else if (incomingByte == 'L') {
      // Command 'L': PWM Phase Adjustment Test (PH_ADV)
      uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
      pwm_set_enabled(slice_num, false);
      pwm_set_counter(slice_num, 0);
      pwm_set_enabled(slice_num, true);
      pwm_hw->slice[slice_num].csr |= (1u << 7); // PH_ADV bit
      Serial1.println("PWM Phase Advanced");
      Serial1.flush();
    } else if (incomingByte == 'O') {
      // Command 'O': Test DMA Debug Registers
      uint32_t src = 0x12345678;
      uint32_t dst = 0;
      int dma_chan = dma_claim_unused_channel(true);
      dma_channel_config c = dma_channel_get_default_config(dma_chan);
      dma_channel_configure(dma_chan, &c, &dst, &src, 1, false);

      // Access TRANS_COUNT and DBG_TCR directly
      volatile uint32_t *trans_count_reg = (volatile uint32_t *)(0x50000000 + dma_chan * 0x40 + 0x08);
      volatile uint32_t *dbg_tcr_reg = (volatile uint32_t *)(0x50000000 + 0x800 + dma_chan * 0x40 + 0x04);

      uint32_t tc_before = *trans_count_reg;
      uint32_t tcr_before = *dbg_tcr_reg;

      dma_channel_start(dma_chan);
      dma_channel_wait_for_finish_blocking(dma_chan);

      uint32_t tc_after = *trans_count_reg;
      uint32_t tcr_after = *dbg_tcr_reg;

      Serial1.printf("DMA Debug: CH=%u TC_PRE=%u TCR_PRE=%u TC_POST=%u TCR_POST=%u\n",
                     (unsigned int)dma_chan, (unsigned int)tc_before, (unsigned int)tcr_before, (unsigned int)tc_after, (unsigned int)tcr_after);

      dma_channel_unclaim(dma_chan);
      Serial1.flush();
    } else if (incomingByte == 'p') {
      // Command 'p': Increment PID Kp
      pid.kp += 0.1f;
      if (pid.kp > 5.0f) pid.kp = 0.0f;
      Serial1.printf("PID Kp: %0.1f\n", pid.kp);
      Serial1.flush();
    } else if (incomingByte == 'i') {
      // Command 'i': Increment PID Ki
      pid.ki += 0.01f;
      if (pid.ki > 1.0f) pid.ki = 0.0f;
      Serial1.printf("PID Ki: %0.2f\n", pid.ki);
      Serial1.flush();
    } else if (incomingByte == 'd') {
      // Command 'd': Increment PID Kd
      pid.kd += 0.01f;
      if (pid.kd > 1.0f) pid.kd = 0.0f;
      Serial1.printf("PID Kd: %0.2f\n", pid.kd);
      Serial1.flush();
    } else if (incomingByte == 't') {
      // Command 't': Cycle PID Target
      pid.target = (pid.target + 200) % 4000;
      Serial1.printf("PID Target: %u\n", pid.target);
      Serial1.flush();
    } else if (incomingByte == 'l') {
      // Command 'l': Toggle PID Loop
      pid.enabled = !pid.enabled;
      Serial1.printf("PID Loop: %s\n", pid.enabled ? "Enabled" : "Disabled");
      Serial1.flush();
    } else if (incomingByte == 'v') {
      // Command 'v': Toggle PID Telemetry
      pid.telemetry = !pid.telemetry;
      Serial1.printf("PID Telemetry: %s\n", pid.telemetry ? "Enabled" : "Disabled");
      Serial1.flush();
    } else if (incomingByte == 'C') {
      // Command 'C': DMA Ring Buffer and Sniffer Test
      static uint8_t ring_buffer[256] __attribute__((aligned(256)));
      static uint8_t target_buffer[256];
      for (int i = 0; i < 256; i++) ring_buffer[i] = i;
      memset(target_buffer, 0, 256);

      int dma_chan = dma_claim_unused_channel(true);
      dma_channel_config c = dma_channel_get_default_config(dma_chan);
      channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
      channel_config_set_read_increment(&c, true);
      channel_config_set_write_increment(&c, true);
      // Enable Ring on read side, size 256 (2^8)
      channel_config_set_ring(&c, false, 8);
      channel_config_set_sniff_enable(&c, true);

      // Configure sniffer for checksum (SUM)
      dma_sniffer_enable(dma_chan, DMA_SNIFF_CTRL_CALC_VALUE_SUM, true);
      dma_hw->sniff_data = 0;

      // Transfer 512 bytes (should wrap twice through the 256-byte ring buffer)
      dma_channel_configure(dma_chan, &c, target_buffer, ring_buffer, 512, true);
      dma_channel_wait_for_finish_blocking(dma_chan);

      uint32_t sniff_sum = dma_hw->sniff_data;
      Serial1.printf("DMA Ring Test: SUM=%u\n", (unsigned int)sniff_sum);

      // Test sniffer global disable behavior
      dma_sniffer_set_output_reverse_enabled(false);
      dma_hw->sniff_ctrl &= ~DMA_SNIFF_CTRL_EN_BITS; // Global disable
      dma_hw->sniff_data = 0;

      dma_channel_configure(dma_chan, &c, target_buffer, ring_buffer, 10, true);
      dma_channel_wait_for_finish_blocking(dma_chan);
      Serial1.printf("DMA Sniff Disable Test: SUM=%u\n", (unsigned int)dma_hw->sniff_data);

      dma_channel_unclaim(dma_chan);
      Serial1.flush();
    } else if (incomingByte == 'r') {
      // Command 'r': Test RESETS peripheral (using 'r' for resets to avoid conflict with 'X')
      volatile uint32_t *resets_base = (volatile uint32_t *)0x4000c000;
      volatile uint32_t *reset_reg = resets_base + 0;
      volatile uint32_t *reset_done_reg = resets_base + 2;

      uint32_t initial_reset = *reset_reg;
      uint32_t initial_done = *reset_done_reg;

      // Clear reset for all (0x01ffffff are all bits that can be 1)
      *reset_reg = 0;
      uint32_t cleared_reset = *reset_reg;
      uint32_t cleared_done = *reset_done_reg;

      // Set reset for all
      *reset_reg = 0x01ffffff;
      uint32_t set_reset = *reset_reg;
      uint32_t set_done = *reset_done_reg;

      Serial1.printf("RESETS Test: INIT_R=0x%08X INIT_D=0x%08X CLR_R=0x%08X CLR_D=0x%08X SET_R=0x%08X SET_D=0x%08X\n",
                     (unsigned int)initial_reset, (unsigned int)initial_done,
                     (unsigned int)cleared_reset, (unsigned int)cleared_done,
                     (unsigned int)set_reset, (unsigned int)set_done);
      Serial1.flush();
    } else {
      // Default: Echo character back to UART
      Serial1.print("Echo: ");
      Serial1.println(incomingByte);
      Serial1.flush();
    }
  }

  // Periodic heartbeat message
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 1000) {
    Serial1.println("Hello from XIAO RP2040!");
    Serial1.flush();
    lastMsg = millis();
  }
}
