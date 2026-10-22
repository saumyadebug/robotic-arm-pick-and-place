# Pick and Place Integration Test Results

**Status: in progress (2026-10-23).**

Verified so far:
- Firmware completes the 8-step sequence and returns to home without an Arduino reset in serial logs.
- The arm is stable with the shoulder vertical and elbow-only reach.
- Physical grip/release and sensor-triggered sorting still need to be tested.

Do **not** record a cycle as PASS until the object is actually picked, transported, and released in the correct bin.

## Test log

| Cycle # | Object color | Detection (cm) | Pick success | Target bin | Placement | Cycle time (s) | Notes |
|---|---|---|---|---|---|---|---|
| 1 | — | — | — | — | — | — | pending |
| 2 | — | — | — | — | — | — | pending |
| 3 | — | — | — | — | — | — | pending |

## Run conditions (fill in)

- Payload mass: ______ g
- Base clamped to table: yes / no
- Supply voltage at PCA9685 V+: ______ V
- Sensor distance: ______ cm
- Firmware commit: ______
