# Implementation Plan — Robotic Arm Pick-and-Place (Stabilization Focus)

**Project:** 6-DOF Robotic Arm Pick-and-Place and Color Sorting (current iteration: 4-DOF pick-and-place)
**Repository:** https://github.com/saumyadebug/robotic-arm-pick-and-place
**Report (poster):** `24BRS1075_24BRS1065_24BRS1126_Poster_A3 (7).pdf`
**Version:** 1.2 (pick-and-place stabilization)
**Date:** 2026-10-22

---

## 1. Purpose

The project report defines the full target system (6-DOF, six MG995 servos, 4 color bins).
For this iteration the priority is narrower and explicit: **make the arm move reliably across all
joints, make the gripper work properly, prevent mechanical collision/collapse, and complete a
stable pick-and-place cycle.** Yellow color sorting and the extra (6th) DOF are deferred until
base motion is stable.

---

## 2. Current Focus & Deferred Items

### In scope now
- Reliable 4-DOF motion (base, shoulder, elbow, wrist) + gripper.
- Safe, sequential joint motion with per-joint pulse clamping to avoid over-travel.
- Stable grip open/close without servo stalling.
- Complete pick-and-place cycle (approach, grasp, lift, transport, release, home).
- Debounced ultrasonic triggering and R/G/B color sorting.

### Deferred (not in this iteration)
- Yellow color detection and 4th bin.
- 6th DOF (wrist roll) servo integration.
- Inverse kinematics and smooth trajectory profiles.

---

## 3. Implementation Phases

### Phase 1 — Motion Stabilization (current)
- [x] Add per-joint safe pulse limits and clamping in `pick_and_place` and `integrated_sorting`.
- [x] Replace simultaneous joint updates with sequential updates (`moveArmSafe`) to reduce load/collision.
- [x] Standardize PCA9685 init across sketches.
- [x] Add ultrasonic debounce (3 consecutive readings).
- [x] Keep R/G/B color sorting (Yellow deferred).

### Phase 2 — Gripper Reliability
- [x] Define gripper open/close pulses (220/420) with margin to avoid stall.
- [ ] Test grip on soft/rigid objects; adjust pulses per hardware.
- [ ] Add current-limit awareness (do not command beyond stall).

### Phase 3 — Hardware Bring-up (user actions)
- [ ] Calibrate each joint with `joint_calibration` and update limits in code.
- [ ] Verify power supply (XL4016 set to 5-6 V, ≥6 A peak, common ground, 2200 µF cap).
- [ ] Secure all horns/screws; check for physical interference before powered motion.
- [ ] Run `servo_testing` on each channel individually before integrated motion.

### Phase 4 — Validation
- [ ] Dry run (no payload) x10 cycles without collision.
- [ ] Loaded run (≤180 g) x10 cycles.
- [ ] Document results in `tests/pick_and_place_test_results.md`.

### Phase 5 — Future (backlog)
- [ ] Add Yellow sorting and 4th bin.
- [ ] Add 6th DOF (wrist roll).
- [ ] Implement trapezoidal/S-curve interpolation.
- [ ] Develop inverse kinematics.

---

## 4. Change Summary (this iteration)

### 4.1 Added
- Per-joint safe pulse limits and `clampPulse()` in firmware.
- Sequential joint motion (`moveArmSafe`) to reduce simultaneous load.
- Ultrasonic debounce (3 readings).
- Hardware bring-up checklist (Section 3, Phase 3).

### 4.2 Changed
- `pick_and_place.ino`: sequential motion, clamped pulses, standardized init.
- `integrated_sorting.ino`: clamped pulses, debounce, standardized init; R/G/B only.
- `color_sensor_test.ino`: reverted to R/G/B classification.
- README status: In Progress (pick-and-place stabilization).
- Docs/tests reverted to 4-DOF R/G/B baseline where previously extended.

### 4.3 Removed (reverted from prior iteration)
- Wrist roll (6th DOF) channel support.
- Yellow color detection and 4th bin.
- Corresponding test rows (channel 5, Yellow cycle).

---

## 5. Hardware Actions Required (user)

1. **Power:** Set XL4016 output to 5.0-6.0 V; ensure ≥6 A peak capability; tie buck GND to Arduino GND; install 2200 µF cap across PCA9685 V+/GND.
2. **Mechanical:** Tighten servo horns and link screws; verify no link hits another during full range; add mechanical stops if needed.
3. **Calibration:** Run `joint_calibration` per joint; record min/max in `tests/servo_test_results.md` and update limits in code.
4. **Gripper:** Test open/close on actual objects; adjust `GRIPPER_OPEN`/`GRIPPER_CLOSED` to avoid stall.
5. **Bring-up order:** `servo_testing` (each channel) → `end_effector_test` → `pick_and_place` (no payload) → loaded test.

---

## 6. Risk & Mitigation

| Risk | Mitigation |
|---|---|
| Servo overload / brownout | Sequential motion, adequate PSU, common ground, capacitor |
| Mechanical collision | Per-joint limits, calibration, physical stops |
| Gripper stall | Pulse margin, current monitoring, adjust grip |
| Misclassification (R/G/B) | Ambient shielding, white balance |

---

## 7. Acceptance Criteria

- [ ] All 4 joints move within calibrated limits without collision.
- [ ] Gripper opens/closes reliably on test objects.
- [ ] 10 consecutive no-payload pick-and-place cycles pass.
- [ ] 10 loaded cycles (≤180 g) pass.
- [ ] Results documented.

---

## 8. Deliverables

1. Updated firmware and docs.
2. `IMPLEMENTATION_PLAN.md`.
3. `docs/change_report.pdf`.
4. Git commit and push.