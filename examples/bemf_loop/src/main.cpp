#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

// Pin configuration
const int MOTOR_PWM_PIN = 16; // PWM0_A
const int BEMF_ADC_PIN = 26;  // ADC0

volatile uint16_t last_bemf_raw = 0;
volatile bool new_sample = false;
uint slice_num;

// PWM wrap interrupt handler - synchronized with PWM cycle
void on_pwm_wrap() {
    pwm_clear_irq(slice_num);

    // At the wrap point, PWM output is typically transition or center
    // We want to sample when PWM is LOW (off) to see back-EMF.
    // In our MotorModel, when PWM is false, ADC shows Vbemf.

    // Trigger ADC conversion
    adc_select_input(0);
    last_bemf_raw = adc_read();
    new_sample = true;
}

void setup() {
    Serial1.begin(115200);
    while (!Serial1);
    Serial1.println("bEMF Feedback Loop Example Started");

    // Initialize ADC
    adc_init();
    adc_gpio_init(BEMF_ADC_PIN);
    adc_select_input(0);

    // Initialize PWM
    gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.0f); // Slow down for simulation visibility
    pwm_config_set_wrap(&config, 1000);
    pwm_init(slice_num, &config, true);

    // Initial duty cycle (10%)
    pwm_set_gpio_level(MOTOR_PWM_PIN, 100);
}

void loop() {
    static uint32_t last_log = 0;
    static int target_duty = 100;
    static int direction = 5;

    if (new_sample) {
        new_sample = false;
        // Basic feedback: if BEMF is high, we might be overspeeding or just reporting
    }

    if (millis() - last_log > 100) {
        Serial1.print("DUTY:");
        Serial1.print(target_duty);
        Serial1.print(" BEMF:");
        Serial1.println(last_bemf_raw);

        // Ramp duty cycle to see BEMF change
        target_duty += direction;
        if (target_duty >= 900 || target_duty <= 100) {
            direction = -direction;
        }
        pwm_set_gpio_level(MOTOR_PWM_PIN, target_duty);

        last_log = millis();
    }
}
