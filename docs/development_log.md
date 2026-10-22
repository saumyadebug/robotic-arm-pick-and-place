# Development Log

### Phase 1: Mechanical Assembly & Electrical Bring-up
- Assembled 4-DOF robotic arm mechanical frame with MG996R and SG90 servos.
- Fabricated power distribution bus; wired PCA9685 I2C driver board to Arduino Uno.

### Phase 2: Sensor Interfacing & Calibration
- Mounted HC-SR04 ultrasonic distance sensor to conveyor pick station.
- Integrated TCS3200 color sensor with custom 3D-printed ambient light shield.
- Calibrated RGB pulse frequency thresholds.

### Phase 3: Software & FSM Implementation
- Built modular Arduino test scripts for individual subsystems.
- Developed integrated finite-state machine coordinating sensor trigger, pick kinematics, color sorting bins, and homing.

### Phase 4: Verification & Benchmarking
- Bench-tested individual subsystems and documented the procedure.
- Full 10-cycle integration and endurance validation still pending after the stability rework.

### Phase 5: Stability Rework & Sensor Integration (2026-10-23)
- Calibrated working channels: base 0, shoulder 1, elbow 2, wrist 6, gripper 7.
- Identified PCA9685 channel 3 as faulty and the wrist-rotation servo on channel 4 as dead.
- Replaced the dangerous shoulder-reach motion with an elbow-only reach; the shoulder stays vertical.
- Added staged lifting (elbow first) so the arm can recover from the reach position.
- Added operational clamps: base 335–415, shoulder 250–275, elbow 205–310, gripper 120/200.
- Rewrote `integrated_sorting.ino` with HC-SR04 debounce, TCS3200 R/G/B/C scanning, retries, and the stable motion sequence.
- Added sensor wiring guide (`docs/sensor_integration.md`) and updated the testing procedure.
