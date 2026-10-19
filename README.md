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


# 6-DOF Robotic Arm for Pick-and-Place and Color-Based Sorting

## Overview

This project involves developing a 6-degree-of-freedom (6-DOF) robotic arm capable of performing pick-and-place operations, with the goal of sorting objects based on their color. The system uses an Arduino microcontroller, servo motors, a PCA9685 PWM servo driver, an ultrasonic sensor, and a color sensor.

The project focuses on robotic joint control, hardware integration, sensor-based object detection, and automated manipulation.

## Objectives

- Control the movement of all six robotic arm joints.
- Perform controlled pick-and-place operations.
- Integrate ultrasonic sensing for object detection.
- Use color sensing to identify objects for sorting.
- Improve joint coordination, motion stability, and positioning accuracy.
- Develop an integrated automated sorting workflow.

## Hardware Components

- 6-DOF metal robotic arm
- Arduino microcontroller
- MG995 servo motors
- PCA9685 16-channel PWM servo driver
- Ultrasonic distance sensor
- Color sensor
- 7.4V, 2500mAh battery
- Buck converter for regulated power supply
- Jumper wires and other mounting hardware

## Technologies and Tools

- Arduino IDE
- Embedded C/C++ (Arduino programming)
- PWM-based servo control
- I2C communication
- Sensor interfacing
- Hardware debugging and calibration

## Current Progress

- Completed the mechanical assembly of the robotic arm.
- Tested individual servo motors and robotic arm movements.
- Tested the end effector for gripping operations.
- Integrated the Arduino and PCA9685 servo driver.
- Configured the power supply using a buck converter.
- Tested basic pick-and-place movements.
- Connected and tested the ultrasonic sensor.
- Connected the color sensor; sensor integration and sorting logic are still in progress.

## Project Workflow

1. Initialize the robotic arm and servo controller.
2. Detect an object using the ultrasonic sensor.
3. Identify the object's color using the color sensor.
4. Move the arm toward the object.
5. Grip and lift the object.
6. Move the object to the designated sorting location.
7. Release the object and return the arm to its starting position.

*The complete sensor-driven workflow will be documented after integration and testing.*

## Repository Structure

- `code/` – Arduino sketches and control logic
- `hardware/` – Circuit diagrams and wiring information
- `docs/` – Setup instructions, calibration and troubleshooting
- `media/` – Project photographs and demonstrations

## Future Improvements

- Complete color sensor integration.
- Implement automatic color-based sorting.
- Improve servo calibration and coordinated joint movement.
- Add reliable object detection and sorting routines.
- Record performance tests and demonstrate the complete system.

## Project Status

**In Progress** — Hardware assembly and basic movement testing are complete. Sensor integration and automated color-based sorting are ongoing.

## Author

Developed as a robotics and embedded systems project, with a focus on hardware integration, robotic motion control, and sensor-based automation.
