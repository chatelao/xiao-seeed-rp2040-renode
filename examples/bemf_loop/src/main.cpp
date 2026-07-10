#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

// Pin configuration based on Seeed XIAO RP2040 mapping
const int MOTOR_PWM_A_PIN = 1;  // D7 (GP1) -> InA
const int MOTOR_PWM_B_PIN = 2;  // D8 (GP2) -> InB
const int BEMF_A_ADC_PIN = 26;  // A0 (GP26) -> bEMF A
const int BEMF_B_ADC_PIN = 27;  // A1 (GP27) -> bEMF B
const int SHUT_ADC_PIN = 28;    // A2 (GP28) -> Shut
const int LED_A_PIN = 17;       // D15 (GP17) -> LED Red
const int LED_B_PIN = 16;       // D16 (GP16) -> LED Green

volatile uint16_t last_bemf_a = 0;
volatile uint16_t last_bemf_b = 0;
volatile uint16_t last_shut = 0;
volatile bool new_sample = false;

uint slice_a, slice_b;
bool direction_forward = true;

// PWM wrap interrupt handler - synchronized with PWM cycle
void on_pwm_wrap() {
    // Clear interrupt for both slices
    pwm_clear_irq(slice_a);
    pwm_clear_irq(slice_b);

    // Sample bEMF A
    adc_select_input(0);
    last_bemf_a = adc_read();

    // Sample bEMF B
    adc_select_input(1);
    last_bemf_b = adc_read();

    // Sample Shut
    adc_select_input(2);
    last_shut = adc_read();

    new_sample = true;
}

void setup() {
    Serial1.begin(115200);
    delay(100);
    Serial1.println("Bidirectional bEMF Loop Example Started");

    // Initialize LEDs
    pinMode(LED_A_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    digitalWrite(LED_A_PIN, HIGH); // Off (active-low)
    digitalWrite(LED_B_PIN, HIGH); // Off (active-low)

    // Initialize ADC
    adc_init();
    adc_gpio_init(BEMF_A_ADC_PIN);
    adc_gpio_init(BEMF_B_ADC_PIN);
    adc_gpio_init(SHUT_ADC_PIN);

    // Initialize PWM
    gpio_set_function(MOTOR_PWM_A_PIN, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR_PWM_B_PIN, GPIO_FUNC_PWM);

    slice_a = pwm_gpio_to_slice_num(MOTOR_PWM_A_PIN);
    slice_b = pwm_gpio_to_slice_num(MOTOR_PWM_B_PIN);

    // Configure interrupts on slice_a (they can be synchronized)
    pwm_clear_irq(slice_a);
    pwm_set_irq_enabled(slice_a, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.0f); // Slow down for simulation visibility
    pwm_config_set_wrap(&config, 1000);

    pwm_init(slice_a, &config, true);
    pwm_init(slice_b, &config, true);

    // Initial state: Forward, duty 100
    pwm_set_gpio_level(MOTOR_PWM_A_PIN, 100);
    pwm_set_gpio_level(MOTOR_PWM_B_PIN, 0);
    digitalWrite(LED_A_PIN, LOW); // LED A on

    Serial1.println("DIR:F DUTY:100 bEMF_A:0 bEMF_B:0 SHUT:0");
    Serial1.flush();
}

void loop() {
    static uint32_t last_log = 0;
    static int target_duty = 100;
    static int ramp_step = 10;

    if (new_sample) {
        new_sample = false;
        // In a real application, you'd run a PID loop here
    }

    if (millis() - last_log > 100) {
        // Ramp duty cycle
        target_duty += ramp_step;

        if (target_duty >= 1000) {
            target_duty = 1000;
            ramp_step = -10;
        } else if (target_duty <= 0) {
            target_duty = 0;
            ramp_step = 10;

            // Switch direction at zero speed
            direction_forward = !direction_forward;

            if (direction_forward) {
                digitalWrite(LED_A_PIN, LOW);
                digitalWrite(LED_B_PIN, HIGH);
                Serial1.println("Direction: FORWARD");
            } else {
                digitalWrite(LED_A_PIN, HIGH);
                digitalWrite(LED_B_PIN, LOW);
                Serial1.println("Direction: REVERSE");
            }
        }

        if (direction_forward) {
            pwm_set_gpio_level(MOTOR_PWM_A_PIN, target_duty);
            pwm_set_gpio_level(MOTOR_PWM_B_PIN, 0);
        } else {
            pwm_set_gpio_level(MOTOR_PWM_A_PIN, 0);
            pwm_set_gpio_level(MOTOR_PWM_B_PIN, target_duty);
        }

        Serial1.print("DIR:");
        Serial1.print(direction_forward ? "F" : "R");
        Serial1.print(" DUTY:");
        Serial1.print(target_duty);
        Serial1.print(" bEMF_A:");
        Serial1.print(last_bemf_a);
        Serial1.print(" bEMF_B:");
        Serial1.print(last_bemf_b);
        Serial1.print(" SHUT:");
        Serial1.println(last_shut);
        Serial1.flush();

        last_log = millis();
    }
}
