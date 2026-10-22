# Experimental Results & Performance Metrics

**Status: not yet measured on the current hardware (2026-10-23).**

The values below are **targets** from the project brief, not measured results. Replace them with real measurements after the sensor integration and 10-cycle test.

| Metric | Target | Measured |
|---|---|---|
| Sorting accuracy | ≥90% under controlled lighting | pending |
| Average cycle time | ~7 s | pending |
| Maximum payload | 180 g | pending |
| Positional repeatability | ±3.5 mm | pending |
| Standby current | — | pending |
| Dynamic current | — | pending |
| Maximum transient current | — | pending |

## Current verified behavior

- Base, shoulder, elbow, wrist pitch, and gripper respond on channels 0, 1, 2, 6, 7.
- Elbow-reach pick-and-place completes its programmed sequence without a firmware reset.
- PCA9685 channel 3 is faulty; wrist-rotation channel 4 is a dead servo.
- HC-SR04 and TCS3200 test sketches are ready; hardware verification is in progress.
