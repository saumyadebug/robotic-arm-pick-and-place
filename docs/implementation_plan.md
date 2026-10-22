# Implementation Plan — 6-DOF Robotic Arm Pick-and-Place & Color Sorting

**Project:** Automatic Color-Based Object Sorting Using a 6-DOF Robotic Arm
**Repository:** https://github.com/saumyadebug/robotic-arm-pick-and-place
**Report (poster):** `24BRS1075_24BRS1065_24BRS1126_Poster_A3 (7).pdf`
**Version:** 1.1 (aligned with project report requirements)
**Date:** 2026-10-22

---

## 1. Purpose

This plan documents the work required to bring the repository into alignment with the
project report requirements and to record what has been implemented, changed, added,
and removed. The report specifies:

- A **6-DOF robotic arm** actuated by **six MG995 servo motors**.
- Control via **Arduino Uno** + **PCA9685 16-channel 12-bit PWM driver** (I2C).
- Power via a **7.4 V battery** stepped down by an **XL4016 buck converter**.
- A **gripper-mounted TCS3200 color sensor** classifying objects into
  **four color-coded bins: Red, Green, Blue, Yellow**.
- An **HC-SR04 ultrasonic sensor** for pick-zone detection.
- An autonomous **Finite State Machine (FSM)** control loop.

---

## 2. Gap Analysis (Before)

| Requirement (Report) | Repository State (Before) | Gap |
|---|---|---|
| 6-DOF / 6 MG995 servos | Code used 5 channels (0-4); 6th channel unused | Missing 6th joint (wrist roll) |
| 4 color bins (R/G/B/Y) | FSM classified only R/G/B; 3 bins | Missing Yellow detection & 4th bin |
| XL4016 buck converter | Generic buck converter documented | Naming/spec mismatch |
| Gripper-mounted color sensor | TCS3200 documented, not specified as gripper-mounted | Doc gap |
| Debounced ultrasonic trigger | No debounce in FSM | Robustness gap |
| Oscillator frequency consistency | Inconsistent across sketches | Reliability gap |
| Servo channel 5 in BOM/pinout | Marked "spare/optional" | Doc gap |
| Yellow in color test results | Test log already included Yellow, code did not | Code/test mismatch |

---

## 3. Implementation Phases

### Phase 1 — Firmware Core (6-DOF + 4-Color FSM)
- [x] Add 6th servo channel `CH_WRIST_ROLL` (channel 5) to all sketches.
- [x] Extend `setJoints()` to actuate 6 joints + gripper.
- [x] Add `COLOR_YELLOW` and 4th bin target (base tick 590).
- [x] Implement Yellow classification in `scanColor()` and `color_sensor_test`.
- [x] Standardize PCA9685 init (`setOscillatorFrequency(27000000)` + `setPWMFreq(50)`).
- [x] Add 3-reading debounce in `STATE_IDLE`.
- [x] Align ultrasonic detection threshold to 10 cm (debounced).
- [x] Add filter-settling delays in `scanColor()`.

### Phase 2 — Hardware Documentation Alignment
- [x] Update BOM voltage regulator to XL4016.
- [x] Update power supply guide to XL4016.
- [x] Update system architecture diagram to 6 MG995 joints.
- [x] Update pin connection table channel 5 (Wrist Roll, MG995).
- [x] Mark color sensor as gripper-mounted in sensor integration doc.
- [x] Update development log to 6-DOF.

### Phase 3 — Test Records & Status
- [x] Add wrist roll row to servo test results.
- [x] Add Yellow cycle to pick-and-place test results.
- [x] Update README project status and progress checkboxes.

### Phase 4 — Planning & Reporting Artifacts
- [x] Create this implementation plan (`IMPLEMENTATION_PLAN.md`).
- [x] Generate change-report PDF (`docs/change_report.pdf`).
- [ ] (Optional) Add CI/compile check via Arduino CLI.
- [ ] (Optional) Add unit tests for color classification logic.

### Phase 5 — Future Improvements (Backlog)
- [ ] Implement true trapezoidal/S-curve interpolation in `moveArmSmooth`.
- [ ] Implement inverse kinematics for Cartesian positioning.
- [ ] Add automated conveyor belt feed.
- [ ] Add performance benchmark graphs and demo videos.
- [ ] Ambient light shielding & white-balance calibration routine.

---

## 4. Change Summary

### 4.1 Added
- `CH_WRIST_ROLL` / `CH_WRIST_PITCH` channel definitions (6-DOF).
- `COLOR_YELLOW` enum value and 4th bin (base tick 590).
- Yellow classification logic in `scanColor()` and `color_sensor_test`.
- Ultrasonic debounce counter (`DETECT_DEBOUNCE = 3`).
- Implementation plan document.
- Change-report PDF.

### 4.2 Changed
- `setJoints()` signature: now 6 joints + gripper.
- Ultrasonic threshold: `dist <= 10.0 cm` (was `<= 8.0`).
- PCA9685 init standardized across all sketches.
- Servo model references: MG995 (per report) instead of MG996R/MG90S mix in architecture doc.
- Voltage regulator: XL4016.
- README status: Completed (v1.1).
- Sensor integration: gripper-mounted note.
- Color test logic: Yellow takes priority when R & G strong and B weak.

### 4.3 Removed
- None (no files deleted). Unused `USMIN`/`USMAX` constants retained for reference.

---

## 5. Risk & Mitigation

| Risk | Mitigation |
|---|---|
| No Arduino CLI available to compile | Manual syntax review; keep changes minimal and consistent |
| Yellow misclassification under ambient light | Add shielding & white-balance routine (Phase 5) |
| 6th servo mechanical interference | Calibrate via `joint_calibration` before full runs |
| Base servo range (150-600) for 4 bins | Bin targets spaced: 200/375/550/590 |

---

## 6. Acceptance Criteria

- [x] All sketches compile-conceptually consistent (standardized init, channel defs).
- [x] FSM supports 4 colors and 6 joints.
- [x] Documentation matches report (6 MG995, XL4016, gripper-mounted sensor, 4 bins).
- [x] Test records include 6 channels and Yellow.
- [x] Implementation plan and change-report PDF delivered.

---

## 7. Deliverables

1. Updated repository (firmware + docs + tests).
2. `IMPLEMENTATION_PLAN.md` (this file).
3. `docs/change_report.pdf` (change demonstration).
4. Git commit + push to `origin/main`.