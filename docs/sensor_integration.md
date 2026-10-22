# Sensor Integration — HC-SR04 + TCS3200

This guide takes the working arm and adds:
1. **HC-SR04 ultrasonic** — detects that an object is in the pick zone.
2. **TCS3200 color sensor** — classifies the object as Red, Green, or Blue.

The integrated sketch is `code/integrated_sorting/integrated_sorting.ino`.

---

## 0. Before you wire anything

- **Power off** the Arduino and unplug the battery/buck converter.
- Keep the servo power wiring as it is: external 5–6 V to PCA9685 **V+**, grounds common.
- The sensors are low-current and can be powered from the Arduino **5V** pin.
- Do **not** move the arm while wiring.

---

## 1. HC-SR04 wiring (4 wires)

| HC-SR04 | Arduino Uno |
|---|---|
| VCC | **5V** |
| GND | **GND** |
| TRIG | **D9** |
| ECHO | **D10** |

**Mount it:** fix the sensor at the pick station, facing the object. It should see the object at **2–10 cm**. Keep the beam roughly perpendicular to the object; soft or curved objects reflect poorly.

---

## 2. TCS3200 wiring (7 or 8 wires)

| TCS3200 | Arduino Uno |
|---|---|
| VCC | **5V** |
| GND | **GND** |
| S0 | **D4** |
| S1 | **D5** |
| S2 | **D6** |
| S3 | **D7** |
| OUT | **D8** |
| OE | **GND** (only if your module has this pin) |

Some modules label `OUT` as `SIG`, `OUT`, or `FREQ` — they are the same signal. Some modules have no `OE` pin; that is fine.

**Mount it:** point the sensor down at the object from **1.5–3.0 cm**. Shield it from direct room light with a short cardboard/3D-printed shroud. The color sensor must see the object **before** the arm grips it.

> Recommended simple setup: mount both sensors **fixed at the pick station**, not on the moving gripper. The arm scans at home, then reaches down to pick. This avoids running sensor wires through moving joints.

---

## 3. Verify the ultrasonic sensor first

1. Upload `code/ultrasonic_test/ultrasonic_test.ino`.
2. Open **Serial Monitor** at **115200 baud**.
3. Place an object 3–8 cm in front of the sensor.
4. You should see:

```
Distance: 5.12 cm -> [OBJECT IN PICK ZONE]
Distance: 15.40 cm -> [zone clear]
```

If it shows `Sensor timeout`, check VCC/GND, TRIG=D9, ECHO=D10. If the distance is wrong, check that the sensor is not tilted and the object is flat.

---

## 4. Verify the color sensor

1. Upload `code/color_sensor_test/color_sensor_test.ino`.
2. Open Serial Monitor at **115200 baud**.
3. Hold a **red** object 2 cm below the sensor. You should see something like:

```
R: 120 | G: 380 | B: 340 | C: 180 => RED
```

4. Repeat with **green** and **blue** objects:

```
R: 340 | G: 150 | B: 310 | C: 190 => GREEN
R: 390 | G: 310 | B: 140 | C: 200 => BLUE
```

5. A white/black background should print `UNKNOWN / NONE`.

### If the color is wrong

The absolute numbers depend on your lighting. Use the readings from the test to adjust these two constants in both `color_sensor_test.ino` and `integrated_sorting.ino`:

```cpp
const unsigned int DARK_LIMIT = 400;  // raise if real colors read as UNKNOWN
const unsigned int MARGIN_PCT = 85;   // lower to 75 if colors are confused
```

- If every object says `UNKNOWN`, **raise** `DARK_LIMIT`.
- If red/green/blue are mixed up, **lower** `MARGIN_PCT` (for example 75) or shield the sensor from room light.
- The dominant color is the channel with the **lowest** pulse width.

---

## 5. Test the two sensors together

Keep both sensors wired and upload the integrated sketch only after both individual tests pass.

1. Upload `code/integrated_sorting/integrated_sorting.ino`.
2. Open Serial Monitor at **115200**.
3. Support the arm during the **6-second startup recovery**.
4. Wait for:

```
System Ready. Place an object 2-10 cm from the ultrasonic sensor.
```

5. Place a red object in the pick zone. Expected sequence:

```
Object detected at 5.0 cm. Scanning colour...
Scan -> R:120 G:380 B:340 C:180
Colour recognised: RED
PICK: reaching down...
PICK: closing gripper...
PICK: lifting upright...
PLACE: rotating upright to base 335
PLACE: reaching down over bin...
PLACE: releasing...
PLACE: lifting upright...
RETURN: rotating home...
Cycle complete. Waiting for next object.
```

6. Then test green (base 375) and blue (base 415).

---

## 6. How the integrated logic behaves

- **IDLE:** reads the ultrasonic every 60 ms. Requires **3 consecutive** readings inside 2–10 cm before scanning.
- **COLOR_SCAN:** reads R/G/B/C. If the color is not recognized, it retries up to **3 times**, then returns to IDLE.
- **PICK:** elbow-only reach, close gripper, staged lift.
- **PLACE:** fold upright, rotate base to the matching bin, elbow-reach down, release, lift.
- **RETURN_HOME:** fold upright, rotate back to 375.

The base **never** rotates while the arm is leaning. That is what keeps the heavy arm from tipping.

---

## 7. Troubleshooting

| Symptom | Check |
|---|---|
| Ultrasonic always 999 cm | TRIG/ECHO swapped, no 5V, or object out of 2–10 cm |
| Ultrasonic triggers randomly | reduce reflections, require 3 readings (already in code), check loose wires |
| Color always UNKNOWN | object too far (>3 cm), too dark, `DARK_LIMIT` too low, OE not grounded |
| Red/green/blue mixed up | room light, sensor distance, `MARGIN_PCT`; recalibrate with the test sketch |
| Arm starts moving while wiring | power off before wiring; never hot-plug servo/sensor leads |
| Arduino resets during motion | servo power brownout — check buck converter, common ground, 2200 µF capacitor |

---

## 8. Wiring diagram (text form)

```
Arduino Uno
  5V  ──────────┬─────────────── HC-SR04 VCC
                └─────────────── TCS3200 VCC
  GND ──────────┬─────────────── HC-SR04 GND
                ├─────────────── TCS3200 GND
                ├─────────────── TCS3200 OE
                └─────────────── PCA9685 GND / PSU GND

  D4  ─────────────────────────── TCS3200 S0
  D5  ─────────────────────────── TCS3200 S1
  D6  ─────────────────────────── TCS3200 S2
  D7  ─────────────────────────── TCS3200 S3
  D8  ─────────────────────────── TCS3200 OUT
  D9  ─────────────────────────── HC-SR04 TRIG
  D10 ─────────────────────────── HC-SR04 ECHO

  A4  ─────────────────────────── PCA9685 SDA
  A5  ─────────────────────────── PCA9685 SCL

External 5–6 V PSU +  ────────── PCA9685 V+
External PSU −        ────────── PCA9685 GND + Arduino GND
```
