#include <Arduino.h>
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "quadrature_encoder.pio.h"

// Pin configuration
const int MOTOR_PWM_PIN = 16;
const int ENCODER_PIN_A = 2; // D8
const int ENCODER_PIN_B = 3; // D10

// PID Controller
struct PidController {
    float kp = 2.0f;
    float ki = 0.5f;
    float kd = 0.1f;
    int32_t target_velocity = 0; // counts per 100ms
    float integral = 0;
    int32_t prev_error = 0;
    bool enabled = false;
} pid;

PIO pio = pio0;
uint sm = 0;
uint slice_num;

void setup() {
    Serial1.begin(115200);
    Serial1.println("Motor PID Encoder Example Started");

    // Initialize PIO Quadrature Encoder
    // Note: quadrature_encoder.pio must be loaded at origin 0
    uint offset = pio_add_program_at_offset(pio, &quadrature_encoder_program, 0);
    // Use a non-zero max_step_rate (10kHz) to reduce simulation overhead in Renode
    quadrature_encoder_program_init(pio, sm, ENCODER_PIN_A, 10000);

    // Initialize PWM
    gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.0f); // Slow down for simulation visibility
    pwm_config_set_wrap(&config, 1000);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(MOTOR_PWM_PIN, 0);

    Serial1.println("System Initialized. Use 't <val>' to set target velocity, 'e' to enable/disable PID.");
}

void loop() {
    static uint32_t last_pid_run = 0;
    static int32_t last_count = 0;

    if (millis() - last_pid_run >= 100) {
        uint32_t now = millis();
        uint32_t dt_ms = now - last_pid_run;
        last_pid_run = now;

        int32_t current_count = quadrature_encoder_get_count(pio, sm);
        int32_t delta = current_count - last_count;
        last_count = current_count;

        if (pid.enabled) {
            int32_t error = pid.target_velocity - delta;
            pid.integral += (float)error * (dt_ms / 1000.0f);

            // Simple anti-windup
            if (pid.integral > 1000) pid.integral = 1000;
            if (pid.integral < -1000) pid.integral = -1000;

            float derivative = (float)(error - pid.prev_error) / (dt_ms / 1000.0f);
            pid.prev_error = error;

            float output = (pid.kp * error) + (pid.ki * pid.integral) + (pid.kd * derivative);

            int32_t pwm_out = (int32_t)output;
            if (pwm_out > 1000) pwm_out = 1000;
            if (pwm_out < 0) pwm_out = 0;

            pwm_set_gpio_level(MOTOR_PWM_PIN, pwm_out);

            Serial1.printf("VEL: %d TGT: %d OUT: %d\n", delta, pid.target_velocity, pwm_out);
        }
    }

    if (Serial1.available()) {
        char cmd = Serial1.read();
        if (cmd == 't') {
            pid.target_velocity = Serial1.parseInt();
            Serial1.printf("New Target: %d\n", pid.target_velocity);
        } else if (cmd == 'e') {
            pid.enabled = !pid.enabled;
            if (!pid.enabled) {
                pwm_set_gpio_level(MOTOR_PWM_PIN, 0);
                pid.integral = 0;
                pid.prev_error = 0;
            }
            Serial1.printf("PID %s\n", pid.enabled ? "Enabled" : "Disabled");
        }
    }
}
