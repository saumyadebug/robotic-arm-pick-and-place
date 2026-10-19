# Hardware Components & Bill of Materials (BOM)

This document provides a detailed list of all electrical, mechanical, and electronic components required to assemble the Robotic Arm Pick and Place System.

---

## 1. Electronics & Control

| Item | Component | Specification | Quantity | Purpose |
|------|-----------|---------------|----------|---------|
| 1 | Microcontroller | Arduino Uno R3 / Mega 2560 | 1 | Master controller running motion & FSM logic |
| 2 | Servo Driver Board | PCA9685 16-Channel 12-Bit PWM (I2C) | 1 | Generates accurate hardware PWM pulses for all servos |
| 3 | Joint Servos | MG995 / MG996R Metal Gear (10-11 kg-cm torque) | 4-6 | High-torque joint actuators (Base, Shoulder, Elbow, Wrist) |
| 4 | Micro Servos | SG90 / MG90S Micro Servo (optional for gripper/wrist) | 1-2 | Gripper opening/closing mechanism |
| 5 | Ultrasonic Sensor | HC-SR04 | 1 | Non-contact distance measurement for pick zone |
| 6 | Color Sensor | TCS3200 / TCS230 | 1 | RGB color detection module |

---

## 2. Power Supply

| Item | Component | Specification | Quantity | Notes |
|------|-----------|---------------|----------|-------|
| 1 | Main Battery | 7.4V 2500mAh 2S Li-ion / LiPo | 1 | Primary mobile power source |
| 2 | Voltage Regulator | Step-Down Buck Converter (DC-DC) | 1 | Regulates 7.4V down to 5V/6V for PCA9685 V+ rail |
| 3 | Logic Power | Arduino USB / 5V rail | 1 | Power for Arduino & sensors |
| 4 | Electrolytic Capacitor | 1000uF - 2200uF, 16V | 1 | Buffers servo transient current spikes on V+ rail |

---

## 3. Mechanical & Structural

| Item | Description | Material / Notes | Quantity |
|------|-------------|------------------|----------|
| 1 | Robotic Arm Chassis | Acrylic / Aluminum / 3D-Printed (PLA/PETG) | 1 Kit |
| 2 | 2-Finger Parallel Gripper | Metal / Acrylic Gripper mechanism | 1 |
| 3 | Bearings & Spacers | M3 / M4 ball bearings for base and elbow | 1 Set |
| 4 | Hardware Fasteners | M3/M4 bolts, locknuts, nylon standoffs | Assorted |
