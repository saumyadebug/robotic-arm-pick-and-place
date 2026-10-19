# Servo Calibration Procedure

Calibrating servo endpoints prevents mechanical bind, current overload, and stripped plastic or metal gears.

---

## Step-by-Step Instructions

1. **Mechanical Decoupling**:
   - Before powering up, detach all servo horn attachments and linkages.
2. **Upload Calibration Sketch**:
   - Upload `code/joint_calibration/joint_calibration.ino`.
3. **Find Midpoint (90°)**:
   - Command pulse `375` (typical midpoint for 50Hz PWM).
   - Attach horn as close to orthogonal (90 degrees) as possible.
4. **Determine Safe Limits**:
   - Increment pulse tick slowly (steps of 10) until desired minimum angle is reached without hum or buzz. Record value as `SERVOMIN`.
   - Decrement or increment to maximum angle. Record as `SERVOMAX`.
5. **Update Constants**:
   - Update joint bounds in `code/integrated_sorting/integrated_sorting.ino` and document in [tests/servo_test_results.md](../tests/servo_test_results.md).
