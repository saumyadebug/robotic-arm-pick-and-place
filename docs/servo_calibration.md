# Servo Calibration Procedure

Calibrating servo endpoints prevents mechanical bind, current overload, and stripped gears.

> **Safety:** support the arm and remove payloads before calibration. Never run a full 0–180° sweep on the assembled arm.

---

## Calibrated values (current hardware)

| Joint | Channel | Full calibrated range | Operational center |
|---|---|---|---|
| Base | 0 | 200–500 | 375 |
| Shoulder | 1 | 200–340 | 260 (vertical) |
| Elbow | 2 | 200–420 | 300 (vertical) |
| Wrist pitch | 6 | 300–450 | 375 |
| Gripper | 7 | 120–200 | 120 open / 200 closed |

**Do not use channel 3** (faulty output) or **channel 4** (dead wrist-rotation servo).

---

## Step-by-step

1. **Mechanical decoupling**
   - Power off. Detach the servo horn/linkage for the joint being calibrated.
2. **Upload the calibration sketch**
   - `code/joint_calibration/joint_calibration.ino`, Serial Monitor at **115200**.
3. **Find the mechanical center**
   - Command the joint's center value from the table above.
   - Attach the horn as close to the desired mechanical position as possible.
4. **Find safe limits**
   - Move in **10-tick steps** toward each end.
   - Stop at the first sign of buzzing, binding, or linkage collision.
   - Back off 10–20 ticks and record the value.
5. **Update firmware**
   - Update the operational constants in:
     - `code/pick_and_place/pick_and_place.ino`
     - `code/integrated_sorting/integrated_sorting.ino`
   - Keep the operational range narrower than the full range for the heavy arm.
6. **Document**
   - Record the values in `tests/servo_test_results.md`.
7. **Re-test**
   - Run `code/servo_testing/servo_testing.ino` with its small safe ranges.

---

## Notes for this arm

- The shoulder is the weakest link. Keep it near **260**; do not use it for deep reaches.
- The elbow performs the reach (300 → 278 → 248 → 218).
- The gripper only has useful travel from **120 to 200**; beyond that it stalls.
- The base should only rotate while the arm is folded upright.
