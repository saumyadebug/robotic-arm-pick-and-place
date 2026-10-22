# 6-DOF Robotic Arm Pick-and-Place and Color Sorting

An articulated 6-degree-of-freedom (6-DOF) metal robotic arm designed for automated pick-and-place operations and color-based object sorting. Controlled via an Arduino microcontroller and a PCA9685 16-channel 12-bit PWM servo driver, the system integrates an ultrasonic distance sensor for pick-zone detection and an RGB color sensor (TCS3200) for autonomous material classification.

---

## Table of Contents
- [About the Project](#about-the-project)
- [Project Status](#project-status)
- [Current Progress](#current-progress)
- [Hardware Components](#hardware-components)
- [Tools and Technologies](#tools-and-technologies)
- [Project Workflow](#project-workflow)
- [Repository Structure](#repository-structure)
- [Wiring & Pin Connections](#wiring--pin-connections)
- [Getting Started](#getting-started)
- [Future Improvements](#future-improvements)
- [Contributing](#contributing)
- [License](#license)
- [Author](#author)

---

## About the Project
This robotics and embedded systems project focuses on robotic joint control, multi-axis hardware integration, sensor-driven perception, and automated object manipulation. The arm identifies colored items placed in its work envelope, grasps them with a parallel gripper end effector, and transports them to designated sorting receptacles.

---

## Project Status
**Completed (v1.1)** — Hardware assembly, servo testing, and pick-and-place movements are completed. Autonomous color-based sorting now supports four colors (Red, Green, Blue, Yellow) across six servo joints; firmware and documentation updated to match the project report requirements.

---

## Current Progress
- [x] Mechanical assembly of the 6-DOF robotic arm completed.
- [x] Individual servo motors tested for range and motion stability.
- [x] End effector gripper mechanism tested for grasp force and compliance.
- [x] Arduino and PCA9685 16-channel PWM servo driver integrated over I2C.
- [x] Regulated power supply configured using a 7.4V battery and step-down buck converter.
- [x] Basic pick-and-place kinematic motion tested (6-DOF).
- [x] HC-SR04 ultrasonic sensor tested for pick-zone proximity triggering.
- [x] TCS3200 color sensor integrated; autonomous sorting for Red/Green/Blue/Yellow completed.

---

## Hardware Components
- **6-DOF Metal Robotic Arm Frame**: Sturdy multi-joint mechanical chassis.
- **Microcontroller**: Arduino Uno R3 / Mega 2560.
- **Servo Motors**: MG995 / MG996R high-torque metal gear servos (Joints 1 to 6).
- **Servo Driver**: PCA9685 16-Channel 12-Bit PWM I2C Driver board.
- **Ultrasonic Distance Sensor**: HC-SR04 for non-contact object detection.
- **Color Sensor**: TCS3200 / TCS230 RGB light-to-frequency converter.
- **Power Source**: 7.4V, 2500mAh Li-ion/LiPo battery pack.
- **Voltage Regulator**: XL4016 9A DC-DC Buck Converter (stepping down 7.4V to 5V/6V for servo rail).
- **End Effector**: 2-Finger parallel mechanical gripper.
- **Accessories**: 1000–2200 µF electrolytic buffer capacitor, breadboard/mounting plates, jumper wires.

---

## Tools and Technologies
- **Development Environment**: Arduino IDE
- **Languages**: Embedded C / C++
- **Communication Protocols**: I2C (Wire library), Hardware PWM (Adafruit PWM Servo Driver library), UART Serial
- **Core Concepts**: PWM Servo Control, Inverse/Forward Kinematics, Sensor Fusion, Finite State Machines (FSM)
- **Hardware Debugging**: Joint calibration routines, logic-rail voltage regulation, common ground isolation

---

## Project Workflow
```
[ Standby / Home Pose ]
         │
         ▼
[ Ultrasonic Sensor: Object Detected in Pick Zone? ] ──(No)──► [ Wait ]
         │ (Yes)
         ▼
[ Color Sensor: Scan & Classify (Red / Green / Blue) ]
         │
         ▼
[ Trajectory Planning: Move Arm to Pick Coordinates ]
         │
         ▼
[ End Effector: Grip Object Securely & Lift ]
         │
         ▼
[ Sorting Movement: Rotate Arm to Color-Assigned Bin ]
         │
         ▼
[ Release Object into Target Bin ]
         │
         ▼
[ Return to Standby Position ]
```

---

## Repository Structure

```
robotic-arm-pick-and-place/
├── README.md                           # Main project documentation
├── LICENSE                             # MIT open-source license
├── .gitignore                          # Build, IDE, and OS ignore rules
├── CONTRIBUTING.md                     # Contribution guidelines
│
├── code/                               # Arduino sketches & firmware
│   ├── servo_testing/                  # Individual joint sweep routines
│   ├── joint_calibration/              # Interactive serial calibration tool
│   ├── pick_and_place/                 # Deterministic waypoint pick-and-place
│   ├── ultrasonic_test/                # HC-SR04 distance measurement test
│   ├── color_sensor_test/              # TCS3200 RGB frequency reading test
│   ├── end_effector_test/              # Gripper open/close actuation test
│   └── integrated_sorting/             # Full autonomous FSM color sorting
│
├── hardware/                           # Hardware specifications & diagrams
│   ├── components.md                   # Complete bill of materials (BOM)
│   ├── pin_connections.md              # Pin mapping & wiring tables
│   ├── power_supply.md                 # Power distribution & buck converter guide
│   ├── wiring_diagram.png              # Full electrical schematic
│   ├── circuit_diagrams/               # Individual module hookup schematics
│   └── assembly/                       # Arm assembly reference photos
│
├── docs/                               # Detailed technical documentation
│   ├── project_overview.md             # High-level architecture and scope
│   ├── system_architecture.md          # Multi-layer system design
│   ├── working_principle.md            # Physics, sensor equations & algorithms
│   ├── servo_calibration.md            # Step-by-step joint zeroing guide
│   ├── motion_control.md               # Trajectory smoothing and kinematics
│   ├── sensor_integration.md           # Sensor placement and noise filtering
│   ├── troubleshooting.md              # Known issues and solutions
│   ├── testing_procedure.md            # Commissioning verification checklist
│   ├── results.md                      # Benchmarks and cycle times
│   └── development_log.md              # Chronological engineering log
│
├── media/                              # Visual assets & demonstrations
│   ├── photos/                         # Setup, wiring, and assembly photos
│   ├── videos/                         # Functional demo video recordings
│   └── gifs/                           # Animated previews of arm motion
│
└── tests/                              # Test procedures & recorded logs
    ├── servo_test_results.md           # Servo bounds & current draw records
    ├── ultrasonic_test_results.md      # Distance detection accuracy logs
    ├── color_sensor_test_results.md    # RGB frequency thresholds table
    └── pick_and_place_test_results.md  # Multi-cycle endurance test results
```

---

## Wiring & Pin Connections

### Arduino Uno to PCA9685 (I2C)
| Arduino Pin | PCA9685 Pin | Description |
|-------------|-------------|-------------|
| A4 (SDA) | SDA | I2C Serial Data |
| A5 (SCL) | SCL | I2C Serial Clock |
| 5V | VCC | Logic Power (from Arduino) |
| GND | GND | Common Logic Ground |
| Buck Converter (+) | V+ (Screw Terminal) | 5V/6V High-current Servo Power |
| Buck Converter (-) | GND (Screw Terminal)| Servo Power Ground (**tied to Arduino GND**) |

### Sensors to Arduino
- **HC-SR04**: `TRIG` -> D9, `ECHO` -> D10, `VCC` -> 5V, `GND` -> GND
- **TCS3200**: `S0` -> D4, `S1` -> D5, `S2` -> D6, `S3` -> D7, `OUT` -> D8, `VCC` -> 5V, `GND` -> GND

*For the complete pinout matrix, see [hardware/pin_connections.md](hardware/pin_connections.md).*

---

## Getting Started

### 1. Prerequisites
- Install the latest [Arduino IDE](https://www.arduino.cc/en/software).
- Install the required library via the Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries...`):
  - **Adafruit PWM Servo Driver Library**

### 2. Hardware Assembly & Power
1. Assemble the arm joints and mount servos for Base, Shoulder, Elbow, Wrist Pitch, Wrist Roll, and Gripper.
2. Connect the 7.4V battery to the DC-DC Buck converter input, adjust the converter output to **5V or 6V**, and connect it to the PCA9685 V+ screw terminal.
3. **Important**: Connect the buck converter ground to Arduino `GND` to establish a shared ground reference.

### 3. Step-by-Step Bring-up
1. **Calibrate Servos**: Upload `code/joint_calibration/joint_calibration.ino` and use the Serial Monitor to identify safe pulse bounds (preventing mechanical stall).
2. **Verify Sensors**:
   - Run `code/ultrasonic_test/ultrasonic_test.ino` to verify distance triggers.
   - Run `code/color_sensor_test/color_sensor_test.ino` under operational lighting to log RGB pulse durations.
3. **Run Integrated Sorting**: Upload `code/integrated_sorting/integrated_sorting.ino` to execute autonomous sorting.

---

## Future Improvements
- Complete color sensor calibration and ambient light shielding.
- Implement smooth S-curve / trapezoidal acceleration profiles to prevent arm overshoot.
- Develop an inverse kinematics (IK) model for Cartesian coordinate positioning.
- Integrate an automated conveyor belt feed mechanism.
- Add live video recordings and performance benchmark graphs.

---

## Contributing
Contributions are welcomed! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details on submitting issues, proposing features, and opening pull requests.

---

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Author
Developed by [saumyadebug](https://github.com/saumyadebug) as a robotics and embedded systems project focused on hardware integration, robotic motion control, and sensor-based automation.
