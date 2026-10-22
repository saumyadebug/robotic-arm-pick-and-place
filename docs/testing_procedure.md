# Testing Procedure

Run these steps in order. Do not skip the sensor tests and do not run the arm with the base unclamped.

## 1. Electrical safety

- [ ] Battery unplugged while wiring.
- [ ] Buck converter set to **5–6 V**.
- [ ] Buck converter negative tied to **Arduino GND**.
- [ ] PCA9685 V+ fed from the buck converter, **not** Arduino 5V.
- [ ] 2200 µF capacitor across PCA9685 V+ / GND, polarity correct.
- [ ] All sensor grounds common with Arduino GND.
- [ ] No short between 5V and GND.

## 2. I2C and servo driver

- [ ] Run an I2C scanner; PCA9685 responds at **0x40**.
- [ ] Upload `code/servo_testing/servo_testing.ino`.
- [ ] Confirm base, shoulder, elbow, wrist and gripper move smoothly in their **small safe ranges**.
- [ ] Confirm channel 3 is not used and channel 4 (dead wrist rotation) is not used.

## 3. Gripper

- [ ] Upload `code/end_effector_test/end_effector_test.ino`.
- [ ] Confirm open = **120 ticks**, closed = **200 ticks**.
- [ ] Grip a light object; the servo must not buzz continuously at the closed position.

## 4. Ultrasonic sensor

- [ ] Wire HC-SR04: VCC=5V, GND=GND, TRIG=D9, ECHO=D10.
- [ ] Upload `code/ultrasonic_test/ultrasonic_test.ino`.
- [ ] Open Serial Monitor at **115200**.
- [ ] At 5 cm the serial shows `[OBJECT IN PICK ZONE]`.
- [ ] At 15 cm it shows `[zone clear]`.
- [ ] No random triggers with nothing in front of the sensor.

## 5. Color sensor

- [ ] Wire TCS3200: VCC=5V, GND=GND, S0=D4, S1=D5, S2=D6, S3=D7, OUT=D8, OE=GND.
- [ ] Upload `code/color_sensor_test/color_sensor_test.ino`.
- [ ] Red object → `=> RED`.
- [ ] Green object → `=> GREEN`.
- [ ] Blue object → `=> BLUE`.
- [ ] White/background → `UNKNOWN / NONE`.
- [ ] Record the actual R/G/B pulse values in `tests/color_sensor_test_results.md`.

## 6. Arm dry run (no payload)

- [ ] Clamp the base to the table.
- [ ] Upload `code/pick_and_place/pick_and_place.ino`.
- [ ] Support the arm during the **6-second startup recovery**.
- [ ] Confirm the elbow reaches down, the arm lifts back up, the base rotates only while upright, and the arm returns home.
- [ ] Stop immediately if the shoulder collapses, a servo buzzes, or the base lifts.

## 7. Full sensor integration

- [ ] Mount both sensors at the pick station.
- [ ] Upload `code/integrated_sorting/integrated_sorting.ino`.
- [ ] Place a red object in the 2–10 cm ultrasonic window.
- [ ] Confirm: detect → color scan → reach → grip → lift → rotate to RED bin → lower → release → home.
- [ ] Repeat for green and blue.
- [ ] Run 3 clean cycles before attempting 10.

## 8. Record results

Update:

- `tests/ultrasonic_test_results.md`
- `tests/color_sensor_test_results.md`
- `tests/pick_and_place_test_results.md`
- `tests/servo_test_results.md`
