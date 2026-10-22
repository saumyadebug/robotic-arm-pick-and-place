# System Architecture

```mermaid
graph TD
    A[Power Supply 5V/6V 10A] -->|V+ High Current| B[PCA9685 PWM Driver]
    C[Arduino Uno] -->|I2C SDA / SCL| B
    C -->|Trigger / Echo| D[HC-SR04 Ultrasonic Sensor]
    C -->|S0-S3 / Frequency| E[TCS3200 Color Sensor]
    B -->|PWM Ch 0| F[Base Servo - MG995]
    B -->|PWM Ch 1| G[Shoulder Servo - MG995]
    B -->|PWM Ch 2| H[Elbow Servo - MG995]
    B -->|PWM Ch 3| I[Wrist Pitch - MG995]
    B -->|PWM Ch 4| J[Gripper - MG995]
    B -->|PWM Ch 5| K[Wrist Roll - MG995]
    A -->|Common GND| C
```

---

## Architectural Layers
1. **Perception Layer**:
   - Ultrasonic distance sensing detects object arrival in the workspace.
   - TCS3200 color sensor determines object classification.
2. **Decision & Control Layer**:
   - Finite State Machine (FSM) on Arduino orchestrates transitions between scanning, grasping, transport, and release.
3. **Actuation Layer**:
   - PCA9685 converts 12-bit PWM values into pulse durations sent to high-torque servos.
