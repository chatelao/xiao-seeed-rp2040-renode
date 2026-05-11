# H0 Locomotive Motor Models

This document provides 10 pre-defined motor models for H0 scale locomotives, suitable for use in Renode simulations. These models simulate different electrical and mechanical characteristics, allowing for testing PID tuning and bEMF control logic across a variety of locomotive types.

## Summary Table

| Model ID | Description | Kv (RPM/V) | Resistance (Ω) | Inertia (kg·m²) | Friction (N·m·s) | Typical Behavior |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **MOD_CAN** | Modern 5-Pole CAN | 1200 | 12.0 | 0.0001 | 0.0001 | Balanced, efficient, standard modern loco. |
| **VIN_OPEN** | Vintage Open-Frame | 1500 | 5.0 | 0.0002 | 0.0005 | High torque, high stall current, noisy. |
| **COR_PREC** | Coreless Precision | 2000 | 25.0 | 0.00001 | 0.00005 | Low inertia, very fast response, fragile. |
| **FLY_FRGT** | Flywheel Freight | 1000 | 10.0 | 0.001 | 0.0001 | Smooth starting, long coasting (momentum). |
| **SWT_LOW** | Low-Speed Switcher | 800 | 15.0 | 0.0001 | 0.0002 | High torque at low speeds, low top speed. |
| **EXP_HI** | High-Speed Express | 1800 | 14.0 | 0.00015 | 0.0001 | Optimized for high-speed passenger runs. |
| **WORN_VIN** | Worn-out Vintage | 1300 | 18.0 | 0.0002 | 0.002 | High internal friction, sluggish response. |
| **SML_N** | Small (N-Style) | 2500 | 35.0 | 0.00005 | 0.0001 | Low torque, high RPM, used in small shays. |
| **HVY_DFLY** | Heavy Dual-Flywheel | 1100 | 8.0 | 0.005 | 0.0002 | Massive momentum, very slow to stop. |
| **TOY_STD** | Low-Cost Toy Motor | 2200 | 20.0 | 0.00008 | 0.0003 | Lower efficiency, erratic at low voltages. |

---

## Renode Configuration Snippets (.repl)

### 1. Modern 5-Pole CAN (MOD_CAN)
The workhorse of modern H0 locomotives. Smooth and reliable.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 1200
    Resistance: 12.0
    Inertia: 0.0001
    Friction: 0.0001
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 2. Vintage Open-Frame (VIN_OPEN)
Found in older Athearn or Hornby models. High current draw.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 1500
    Resistance: 5.0
    Inertia: 0.0002
    Friction: 0.0005
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 3. Coreless Precision (COR_PREC)
Used in high-end brass or precision models. Requires very fine PID tuning.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 2000
    Resistance: 25.0
    Inertia: 0.00001
    Friction: 0.00005
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 4. Flywheel-Equipped Freight (FLY_FRGT)
Simulates a motor with a standard brass flywheel for improved coasting.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 1000
    Resistance: 10.0
    Inertia: 0.001
    Friction: 0.0001
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 5. Low-Speed Switcher (SWT_LOW)
Geared down for yard work. Excellent low-speed control.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 800
    Resistance: 15.0
    Inertia: 0.0001
    Friction: 0.0002
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 6. High-Speed Express (EXP_HI)
Designed for TGV, ICE, or Shinkansen models where top speed is priority.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 1800
    Resistance: 14.0
    Inertia: 0.00015
    Friction: 0.0001
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 7. Worn-out Vintage (WORN_VIN)
Simulates a motor with dry bearings or dirty commutator.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 1300
    Resistance: 18.0
    Inertia: 0.0002
    Friction: 0.002
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 8. Small (N-Style) in H0 (SML_N)
Small motor used in narrow-gauge or very small H0 switchers.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 2500
    Resistance: 35.0
    Inertia: 0.00005
    Friction: 0.0001
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 9. Heavy Dual-Flywheel (HVY_DFLY)
Extreme momentum simulation for heavy mainline diesel locomotives.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 1100
    Resistance: 8.0
    Inertia: 0.005
    Friction: 0.0002
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```

### 10. Low-Cost Toy Motor (TOY_STD)
Standard motor found in "train set" starter locomotives.
```renode
motor: Peripherals.Motor.MotorModel @ sysbus
    Kv: 2200
    Resistance: 20.0
    Inertia: 0.00008
    Friction: 0.0003
    Vbus: 12.0
    Adc: adc
    AdcChannel: 1
```
