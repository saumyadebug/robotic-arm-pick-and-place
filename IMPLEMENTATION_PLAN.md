# Implementation Plan — Robotic Arm Pick-and-Place + Sensor Integration

**Project:** 4-DOF + gripper robotic arm (6-DOF frame, wrist roll deferred)
**Repository:** https://github.com/saumyadebug/robotic-arm-pick-and-place
**Version:** 1.3 (stable elbow-reach + sensor bring-up)
**Date:** 2026-10-23

---

## 1. Current scope

The arm must complete a stable pick-and-place cycle and sort Red/Green/Blue objects using:
- **HC-SR04** ultrasonic sensor for pick-zone detection.
- **TCS3200** color sensor for RGB classification.

The shoulder no longer performs the deep reach. The shoulder stays vertical and the elbow does the reach, so the heavy upper arm remains balanced over the base. The base rotates only while the arm is folded upright.

Deferred: Yellow sorting, wrist-roll (6th DOF), inverse kinematics, S-curve trajectories.

---

## 2. Calibrated hardware state

| Joint | Channel | Operational range |
|---|---|---|
| Base | 0 | 335–415 (center 375) |
| Shoulder | 1 | 250–275 (center 260, vertical) |
| Elbow | 2 | 205–310 (center 300, vertical) |
| Wrist pitch | 6 | fixed 375 |
| Gripper | 7 | open 120 / closed 200 |
| PCA9685 ch3 | 3 | **FAULTY — do not use** |
| Wrist rotation | 4 | **DEAD SERVO — not used** |

Sensor pins:
- HC-SR04: TRIG = D9, ECHO = D10, VCC = 5V, GND = GND.
- TCS3200: S0 = D4, S1 = D5, S2 = D6, S3 = D7, OUT = D8, OE = GND, VCC = 5V, GND = GND.

---

## 3. Implementation phases

### Phase 1 — Motion stabilization (complete)
- [x] Add per-joint operational clamps.
- [x] Replace simultaneous joint updates with sequential movement.
- [x] Keep the shoulder vertical; elbow-only reach.
- [x] Staged lift: elbow first, then shoulder.
- [x] Rotate base only while upright.
- [x] Fix gripper state when returning home.

### Phase 2 — Sensor bring-up (in progress)
- [x] Integrate HC-SR04 debounce (3 consecutive readings) in firmware.
- [x] Integrate TCS3200 R/G/B/C scanning in firmware.
- [x] Add standalone test sketches for both sensors.
- [ ] Wire both sensors on the hardware.
- [ ] Verify ultrasonic 2–10 cm window.
- [ ] Verify red/green/blue readings and adjust `DARK_LIMIT` / `MARGIN_PCT`.
- [ ] Verify sensor-to-arm integration with the base clamped.

### Phase 3 — Validation
- [ ] 3 clean dry-run sorting cycles (no payload).
- [ ] 10 dry-run cycles.
- [ ] Loaded cycles (≤180 g).
- [ ] Record results in `tests/`.

### Phase 4 — Deferred backlog
- [ ] Yellow sorting and 4th bin.
- [ ] Replace wrist-rotation servo / 6th DOF.
- [ ] Trapezoidal or S-curve interpolation.
- [ ] Inverse kinematics.

---

## 4. Change summary (this iteration)

### Added
- Integrated FSM in `integrated_sorting.ino` using the stable elbow-reach motion.
- TCS3200 clear-channel reading and relative color classification.
- Sensor wiring and step-by-step bring-up guide (`docs/sensor_integration.md`).
- Safe-range servo test and corrected gripper test.
- Updated pin map, safe motion chart, troubleshooting, and testing procedure.

### Changed
- `pick_and_place.ino`: elbow-only reach; staged lift; gripper remains open when rotating home.
- `integrated_sorting.ino`: latest channel map and safety clamps; sensor retry logic.
- `end_effector_test.ino`: channel 7, open 120 / closed 200.
- `servo_testing.ino`: only working channels, small safe ranges, skip ch3/ch4.
- `joint_calibration.ino`: calibrated range table and faulty-channel warnings.
- `color_sensor_test.ino`: relative RGB classification, clear channel, noise averaging.

### Removed / deferred
- Yellow sorting and 4th bin.
- Wrist-roll (6th DOF) integration.

---

## 5. Hardware actions required

1. **Power:** XL4016 output 5–6 V; ≥6 A peak; buck GND tied to Arduino GND; 2200 µF across PCA9685 V+/GND.
2. **Base:** clamp it to the table before any powered test.
3. **Sensors:** wire per `hardware/pin_connections.md`; mount HC-SR04 facing the pick zone and TCS3200 1.5–3.0 cm above the object, shaded from room light.
4. **Bring-up order:** servo test → gripper test → ultrasonic test → color test → pick_and_place dry run → integrated sorting.
5. **Support the arm** during the first 6-second startup recovery.

---

## 6. Risks and mitigation

| Risk | Mitigation |
|---|---|
| Shoulder collapse | shoulder stays vertical; elbow-only reach; staged lift |
| Base tipping | base rotates only while upright; base clamped to table |
| Servo brownout | external 5–6 V supply; common ground; 2200 µF capacitor; sequential motion |
| Color misclassification | shade sensor; 1.5–3.0 cm distance; tune `DARK_LIMIT` and `MARGIN_PCT` |
| Ultrasonic false trigger | 3-reading debounce; flat target; secure wiring |

---

## 7. Acceptance criteria

- [ ] Arm completes reach → grip → lift → rotate → lower → release → home without collapsing.
- [ ] Ultrasonic detects an object at 2–10 cm and ignores the empty zone.
- [ ] TCS3200 correctly identifies red, green, and blue test objects.
- [ ] Integrated FSM sorts one object to the matching bin.
- [ ] Three consecutive sorting cycles complete without manual intervention.
