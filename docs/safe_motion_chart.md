# Safe Motion Chart — Joint Ranges for Stable Pick-and-Place

**Arm type:** Heavy metal 4-DOF + gripper (MG995/MG996R joints, SG90 gripper)
**Controller:** Arduino Uno + PCA9685 (I2C, 50 Hz)
**Purpose:** Keep the arm stable, avoid servo overload and mechanical collision during pick-and-place.
**Status:** Starting safe ranges — must be calibrated per your physical arm (see Calibration).

---

## 1. Joint Range Table (starting safe ranges)

Pulse values are PCA9685 ticks at 50 Hz (1 tick ≈ 4.88 µs). Lower/higher limits must be
calibrated with `joint_calibration` before use.

| Joint | Channel | Servo | Safe Min (tick) | Safe Max (tick) | Neutral (tick) | Notes |
|---|---|---|---|---|---|---|
| Base (waist) | 0 | MG995 | 200 | 500 | 375 | Keep sweep ≤ ±60° from center to avoid cable twist |
| Shoulder | 1 | MG995 | 230 | 470 | 300 | Heavy arm: avoid full horizontal extension under load |
| Elbow | 2 | MG995 | 220 | 480 | 350 | Avoid fully straight (over-center) positions |
| Wrist pitch | 3 | MG90S/SG90 | 180 | 520 | 300 | Keep gripper level during transport |
| Gripper | 4 | SG90 | 220 (open) | 420 (grip) | — | Add 10–20 tick margin; do not stall |

> **Rule:** Never command a joint to its absolute mechanical limit during motion. Keep a 10–20 tick margin.

---

## 2. Pick-and-Place Waypoint Chart

All values in ticks. Move joints **sequentially** (base → shoulder → elbow → wrist → gripper) with
20 ms between joints to reduce peak current and collision risk.

| Step | Base | Shoulder | Elbow | Wrist | Gripper | Delay (ms) | Purpose |
|---|---|---|---|---|---|---|---|
| 0. Home (standby) | 375 | 300 | 350 | 300 | 220 (open) | 2000 | Safe idle pose, arm elevated |
| 1. Approach pick | 250 | 420 | 280 | 250 | 220 (open) | 1000 | Hover above object |
| 2. Grasp | 250 | 420 | 280 | 250 | 420 (grip) | 800 | Close gripper |
| 3. Lift | 250 | 320 | 350 | 300 | 420 (grip) | 1000 | Clear pick zone |
| 4. Transport to drop | 500 | 320 | 350 | 300 | 420 (grip) | 1000 | Rotate to bin |
| 5. Lower to bin | 500 | 420 | 280 | 250 | 420 (grip) | 800 | Descend |
| 6. Release | 500 | 420 | 280 | 250 | 220 (open) | 800 | Open gripper |
| 7. Return home | 375 | 300 | 350 | 300 | 220 (open) | 3000 | Reset |

---

## 3. Safety Rules (heavy metal arm)

1. **Power:** XL4016 set to 5.0–6.0 V, ≥6 A peak, common ground, 2200 µF cap on PCA9685.
2. **Never** power servos from Arduino 5 V pin.
3. **Calibrate first:** run `joint_calibration` per joint; update limits in code.
4. **Sequential motion:** move one joint at a time with 20 ms gaps (already in `moveArmSafe`).
5. **Payload limit:** ≤180 g at full extension; reduce if arm droops.
6. **Gripper margin:** keep 10–20 tick gap from stall; test on soft objects first.
7. **No full-range sweeps** on the assembled metal arm — use calibrated safe ranges only.
8. **Mechanical stops:** add hard stops if a joint can over-rotate into the frame.
9. **Base clamping:** clamp the base to the table to prevent tipping.
10. **Dry run first:** 10 cycles with no payload before loaded test.

---

## 4. Calibration Procedure (per joint)

1. Detach linkages; run `joint_calibration/joint_calibration.ino`.
2. Command tick 375 (neutral); attach horn at 90°.
3. Increment by 10 ticks until mechanical limit or hum/buzz; record as MAX.
4. Decrement to minimum; record as MIN.
5. Update `*_MIN/*_MAX` in `pick_and_place.ino` and `integrated_sorting.ino`.
6. Document in `tests/servo_test_results.md`.

---

## 5. How to Use This Chart During Presentation

- Show Serial Monitor output (115200 baud) — each step prints its name.
- Run `pick_and_place` with no payload first; verify each joint stays in safe range.
- If any joint buzzes or stalls, stop immediately and reduce that joint's range.
- Keep the arm clamped to the table; stand clear during motion.

---

## 6. Note on the Uploaded Arm Photo

Image received: `WhatsApp Image 2026-10-22 at 6.11.43 PM.jpeg` (899×1599 px).
I cannot visually inspect images, so please confirm:
- Which joint binds or collides first?
- Does the arm droop under its own weight?
- Does the gripper stall or fail to grip?
- Are any servo horns loose or slipping?

Your answers will let me tighten the safe ranges and waypoints further.