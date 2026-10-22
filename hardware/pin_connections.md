# Pin Connection Mapping

Comprehensive pin connection table for Arduino Uno, PCA9685 PWM driver, and external sensors.

---

## 1. Arduino to PCA9685 (I2C Interface)

| Arduino Uno Pin | PCA9685 Pin | Description |
|-----------------|-------------|-------------|
| A4 (SDA) | SDA | I2C Data line (with 4.7k pull-up recommended) |
| A5 (SCL) | SCL | I2C Clock line |
| 5V | VCC | Logic Power (Arduino 5V rail) |
| GND | GND | Common Logic Ground |
| N/A | OE | Output Enable (leave floating or pulled to GND) |
| External 5V-6V PSU (+) | V+ (Screw Terminal) | Servo motor power |
| External PSU (-) | GND (Screw Terminal)| Servo power ground (**Must tie to Arduino GND!**) |

---

## 2. PCA9685 Servo Channels

| Channel | Joint Name | Servo Type | Min Pulse (0°) | Max Pulse (180°) | Center (90°) |
|---------|------------|------------|----------------|------------------|--------------|
| Channel 0 | Base (Waist) | MG996R | 150 | 600 | 375 |
| Channel 1 | Shoulder | MG996R | 180 | 580 | 380 |
| Channel 2 | Elbow | MG996R | 160 | 590 | 375 |
| Channel 3 | Wrist Pitch | SG90 / MG90S | 150 | 600 | 375 |
| Channel 4 | End Effector (Gripper)| SG90 / MG90S | 200 (Open) | 450 (Grip) | N/A |
| Channel 5 | Wrist Roll | MG995 | 150 | 600 | 375 |

---

## 3. Sensors to Arduino Uno

### HC-SR04 Ultrasonic Sensor
| Sensor Pin | Arduino Pin | Function |
|------------|-------------|----------|
| VCC | 5V | Sensor 5V Supply |
| GND | GND | Ground |
| TRIG | D9 | Trigger output pulse |
| ECHO | D10 | Echo return pulse input |

### TCS3200 Color Sensor
| Sensor Pin | Arduino Pin | Function |
|------------|-------------|----------|
| VCC | 5V | Sensor 5V Supply |
| GND | GND | Common Ground |
| S0 | D4 | Frequency scaling selector S0 |
| S1 | D5 | Frequency scaling selector S1 |
| S2 | D6 | Photodiode filter selector S2 |
| S3 | D7 | Photodiode filter selector S3 |
| OUT | D8 | Frequency output square wave |
| OE | GND | Active-low output enable |
