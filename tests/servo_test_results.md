# Servo Calibration & Test Results

Calibrated on hardware **2026-10-23**. Current draw has not been measured with a meter; leave it blank until recorded.

| Channel | Joint | Servo model | Full calibrated range | Operational clamp | Center | Status |
|---|---|---|---|---|---|---|
| 0 | Base | MG996R | 200–500 | 335–415 | 375 | PASS |
| 1 | Shoulder | MG996R | 200–340 | 250–275 | 260 | PASS |
| 2 | Elbow | MG996R | 200–420 | 205–310 | 300 | PASS |
| 3 | — | — | — | — | — | **FAULTY OUTPUT — DO NOT USE** |
| 4 | Wrist rotation | — | — | — | — | **DEAD SERVO — replace** |
| 5 | Spare | — | — | — | — | unused |
| 6 | Wrist pitch | MG90S/SG90 | 300–450 | 370–380 | 375 | PASS (held neutral) |
| 7 | Gripper | MG90S/SG90 | 120–200 | 120 open / 200 closed | — | PASS |

## Notes

- The **operational clamp** is narrower than the full calibrated range because the assembled heavy arm is not stable at the extremes.
- The shoulder must stay near **260** (vertical). Deep shoulder bends cause the arm to fall and prevent it from lifting back up.
- The elbow does the reach: **300 → 278 → 248 → 218**.
- Wrist rotation (ch4) was tested on several channels and with a bare shaft; it holds but never rotates. This is a hardware failure, not a firmware issue.
