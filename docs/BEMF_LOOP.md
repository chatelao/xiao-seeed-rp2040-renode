# BEMF Loop Calculation Concept

This document describes the conceptual design for calculating Back Electromotive Force (bEMF) signals during ADC readout gaps, based on the previous cycles of PWM.

## Mathematical Model

The bEMF voltage ($V_{bemf}$) of a brushless DC (BLDC) motor is proportional to the motor's angular velocity ($\omega$):

$V_{bemf} = K_e \cdot \omega$

Where $K_e$ is the back-EMF constant (V/(rad/s)).

In a PWM-driven system, the effective voltage applied to the motor phases depends on the duty cycle ($D$):

$V_{effective} = V_{bus} \cdot D$

During the "OFF" period of the PWM cycle (the readout gap), the motor is coasting, and the voltage measured on an undriven phase is the sum of the bEMF and the center-tap voltage.

### bEMF Estimation from PWM

By tracking the PWM duty cycle and the resulting motor speed from previous cycles, we can estimate the expected bEMF during the current cycle's gap.

$\omega_{est} = f(D_{prev}, Load, MotorParams)$

$V_{bemf, predicted} = K_e \cdot \omega_{est}$

## Motor Configuration

To support various real-world motors, the simulation allows configuring motor-specific parameters:

| Parameter | Description | Units |
| --- | --- | --- |
| `MotorType` | Identifier for the motor model (e.g., `Drone_2207`, `Industrial_BLDC`) | - |
| `Kv` | Motor velocity constant | RPM/V |
| `Poles` | Number of permanent magnet poles | - |
| `Resistance` | Phase-to-phase resistance | Ohms |
| `Inductance` | Phase-to-phase inductance | Henrys |
| `Inertia` | Rotor moment of inertia | kg·m² |

### Config Example
```yaml
motor_config:
  type: "Drone_HighKV"
  kv: 2400
  poles: 14
  resistance: 0.045
  inductance: 0.00002
```

## ADC Synchronization and Readout Gaps

To accurately measure bEMF, the ADC sampling must be synchronized with the PWM "OFF" time to avoid switching noise.

1. **PWM Trigger:** The PWM peripheral generates a trigger signal at a specific point in its cycle (usually center-aligned or at the end of the ON pulse).
2. **ADC Delay:** A configurable delay ensures the MOSFETs have fully switched and ringing has subsided.
3. **Sampling Window:** The ADC captures the voltage on the floating phase.
4. **Integration:** The bEMF loop controller uses these samples to update the commutation timing or speed estimate.

## Logging and Graphical Analysis

For comparison with real-world motor data, the system logs PWM and bEMF values in a CSV-compatible format via UART:

`TIMESTAMP, PWM_DUTY, PHASE_A_V, PHASE_B_V, PHASE_C_V, BEMF_EST`

### Data Flow
- **Simulation:** Renode calculates the "virtual" bEMF based on the motor model and PWM inputs.
- **Firmware:** Reads simulated ADC values and applies the BEMF loop logic.
- **Host:** Captures UART logs and uses tools like `matplotlib` or `Teleplot` for real-time graphical analysis.
