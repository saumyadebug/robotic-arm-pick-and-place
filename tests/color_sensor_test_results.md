# Color Sensor Test Results

Sensor: **TCS3200**
Frequency scaling: **20%** (`S0 = HIGH`, `S1 = LOW`)
Measurement: pulse width in microseconds — **lower = brighter channel**

> The table below is a **reference baseline** from bench testing. Re-run `code/color_sensor_test/color_sensor_test.ino` on the final setup and replace these numbers with the values you actually see.

| Sample object | Red pulse (µs) | Green pulse (µs) | Blue pulse (µs) | Clear (µs) | Classified | Result |
|---|---|---|---|---|---|---|
| Red cube | 120 | 380 | 340 | 180 | RED | reference |
| Green cube | 340 | 150 | 310 | 190 | GREEN | reference |
| Blue cube | 390 | 310 | 140 | 200 | BLUE | reference |
| White card | 110 | 110 | 110 | 105 | UNKNOWN | reference |
| Background mat | 450 | 470 | 440 | 460 | UNKNOWN | reference |

## Current calibration constants

```cpp
const unsigned int DARK_LIMIT = 400;  // raise if real colors read UNKNOWN
const unsigned int MARGIN_PCT = 85;   // lower to 75 if colors are mixed up
```

## To verify on hardware

1. Wire: VCC=5V, GND=GND, S0=D4, S1=D5, S2=D6, S3=D7, OUT=D8, OE=GND.
2. Upload `code/color_sensor_test/color_sensor_test.ino`.
3. Open Serial Monitor at **115200**.
4. Place each object **1.5–3.0 cm** below the sensor, shaded from room light.
5. Record R/G/B/C and the classification.
