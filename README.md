# Robotic Arm Pick and Place System

An intelligent, multi-degree-of-freedom (DOF) robotic arm designed for automated object detection, color identification, and pick-and-place sorting. Controlled via Arduino and PCA9685 16-channel PWM servo driver, this system integrates ultrasonic distance sensing and color recognition (TCS3200) for autonomous material handling.

---

## Table of Contents
- [Project Overview](#project-overview)
- [Repository Structure](#repository-structure)
- [Hardware Architecture](#hardware-architecture)
- [Software Architecture](#software-architecture)
- [Getting Started](#getting-started)
- [Calibration & Testing](#calibration--testing)
- [Contributing](#contributing)
- [License](#license)

---

## Project Overview

The objective of this project is to automate the pick-and-place process:
1. **Object Detection**: Ultrasonic sensor detects when an object arrives in the pick zone.
2. **Color Recognition**: TCS3200 color sensor determines object color (Red, Green, Blue, etc.).
3. **Trajectory Execution**: Robotic arm moves the end-effector to the pick coordinates.
4. **Gripping**: End-effector grips the object securely.
5. **Sorting & Placement**: Arm deposits the object into the corresponding color-coded bin.
6. **Homing**: Arm returns to default standby position for the next cycle.

---

## Repository Structure

```
robotic-arm-pick-and-place/
├── README.md
├── LICENSE
├── .gitignore
├── CONTRIBUTING.md
├── code/                   # Arduino sketches for testing & integration
│   ├── servo_testing/
│   ├── joint_calibration/
│   ├── pick_and_place/
│   ├── ultrasonic_test/
│   ├── color_sensor_test/
│   ├── end_effector_test/
│   └── integrated_sorting/
├── hardware/               # BOM, pinouts, wiring diagrams, assembly photos
│   ├── circuit_diagrams/
│   └── assembly/
├── docs/                   # Documentation, theory, kinematics, troubleshooting
├── media/                  # Photos, video demonstrations, and GIFs
│   ├── photos/
│   ├── videos/
│   └── gifs/
└── tests/                  # Test procedures, logs, and benchmark results
```

---

## Hardware Architecture

- **Microcontroller**: Arduino Uno / Mega 2560
- **Servo Controller**: PCA9685 16-Channel 12-Bit I2C PWM Driver
- **Actuators**: High-torque MG996R / SG90 Servos (Base, Shoulder, Elbow, Wrist Pitch, Wrist Roll, Gripper)
- **Object Detection**: HC-SR04 Ultrasonic Sensor
- **Color Detection**: TCS3200 / TCS230 Color Sensor Module
- **Power Supply**: Dedicated 5V/6V 10A DC Power Supply (common ground with Arduino)

See [hardware/components.md](hardware/components.md) and [hardware/pin_connections.md](hardware/pin_connections.md) for full hardware specifications and wiring details.

---

## Software Architecture

Required Arduino Libraries:
- `Wire.h` (Built-in I2C)
- `Adafruit_PWMServoDriver.h` (Adafruit PWM Servo Driver Library)

Install via Arduino IDE Library Manager (`Tools` -> `Manage Libraries...`).

---

## Getting Started

1. **Hardware Setup**: Assemble the robotic arm mechanical frame and connect servos to the PCA9685 board according to [hardware/pin_connections.md](hardware/pin_connections.md).
2. **Power Supply**: Connect external 5V/6V power to the PCA9685 V+ screw terminal. **Do NOT power servos directly from Arduino 5V pin!**
3. **Calibrate Servos**: Upload `code/joint_calibration/joint_calibration.ino` to find safe min and max pulse ticks for each joint.
4. **Sensor Verification**: Run `code/ultrasonic_test/` and `code/color_sensor_test/` to calibrate detection thresholds.
5. **Run Integrated Sorting**: Upload `code/integrated_sorting/integrated_sorting.ino` for the full autonomous pipeline.

---

## Contributing
Contributions and suggestions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on code standards and pull requests.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
