# Testing Procedure

Follow this checklist during hardware commissioning:

1. [ ] **Electrical Isolation Test**: Confirm 0V between unconnected rails; check common ground continuity.
2. [ ] **I2C Bus Scan**: Verify address `0x40` responds.
3. [ ] **Individual Joint Test**: Run `code/servo_testing/` on each channel individually.
4. [ ] **End Effector Grip Test**: Run `code/end_effector_test/` with target test blocks.
5. [ ] **Sensor Characterization**:
   - Run `code/ultrasonic_test/` with metric ruler calibration.
   - Run `code/color_sensor_test/` with Red, Green, and Blue test cubes.
6. [ ] **Dry Run (No Payload)**: Run `code/pick_and_place/` without payload.
7. [ ] **Full Integration Test**: Run `code/integrated_sorting/` for 10 continuous sorting cycles.
