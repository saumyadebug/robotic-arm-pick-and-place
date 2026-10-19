# Hardware Components & Bill of Materials (BOM)

This document provides a detailed list of all electrical, mechanical, and electronic components required to assemble the Robotic Arm Pick and Place System.

---

## 1. Electronics & Control

| Item | Component | Specification | Quantity | Purpose |
|------|-----------|---------------|----------|---------|
| 1 | Microcontroller | Arduino Uno R3 / Mega 2560 | 1 | Master controller running motion & FSM logic |
| 2 | Servo Driver Board | PCA9685 16-Channel 12-Bit PWM (I2C) | 1 | Generates accurate hardware PWM pulses for all servos |
| 3 | Base Servo | MG996R Metal Gear (11 kg-cm torque) | 1 | Azimuth / Base rotation (Waist joint) |
| 4 | Shoulder Servo | MG996R Metal Gear (11 kg-cm torque) | 1 or 2 | Primary arm elevation joint |
| 5 | Elbow Servo | MG996R Metal Gear (11 kg-cm torque) | 1 | Forearm pitch joint |
| 6 | Wrist Pitch Servo | SG90 / MG90S Micro Servo | 1 | Wrist orientation up/down |
| 7 | End Effector Servo | SG90 / MG90S Micro Servo | 1 | Gripper opening and closing mechanism |
| 8 | Ultrasonic Sensor | HC-SR04 | 1 | Non-contact distance measurement for pick zone |
| 9 | Color Sensor | TCS3200 / TCS230 | 1 | RGB color detection module |

---

## 2. Power Supply

| Item | Component | Specification | Quantity | Notes |
|------|-----------|---------------|----------|-------|
| 1 | Servo DC Power Supply | 5V / 6V DC, 10A regulated | 1 | Dedicated power for PCA9685 V+ power terminal |
| 2 | Logic Power Supply | 5V USB (from PC or 9V DC adapter) | 1 | Arduino logic rail |
| 3 | Electrolytic Capacitor | 1000uF - 2200uF, 16V | 1 | Buffers servo transient current spikes on V+ rail |

---

## 3. Mechanical & Structural

| Item | Description | Material / Notes | Quantity |
|------|-------------|------------------|----------|
| 1 | Robotic Arm Chassis | Acrylic / Aluminum / 3D-Printed (PLA/PETG) | 1 Kit |
| 2 | 2-Finger Parallel Gripper | Metal / Acrylic Gripper mechanism | 1 |
| 3 | Bearings & Spacers | M3 / M4 ball bearings for base and elbow | 1 Set |
| 4 | Hardware Fasteners | M3/M4 bolts, locknuts, nylon standoffs | Assorted |
