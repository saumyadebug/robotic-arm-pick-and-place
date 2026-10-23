# Faculty Presentation Guide

**Project:** Robotic Arm Pick-and-Place with Color Sorting  
**Board:** Arduino Uno on **COM8**  
**Baud rate:** **115200**  
**Arduino CLI:** `C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe`

This guide is the short script for demonstrating the project to faculty: what hardware is shown, what software is run, what the safe ranges are, and what output proves the system works.

---

## 1. What the project does

The arm is a **4-DOF heavy metal arm + gripper**.  
It safely picks an object and places it into a color bin:

- Red object -> base angle **335**
- Green object -> base angle **375**
- Blue object -> base angle **415**

Motion is intentionally conservative:

- Shoulder stays vertical.
- Elbow does the reach down.
- Base rotates only while the arm is folded upright.
- Wrist pitch stays fixed.
- No wrist roll / 6th DOF.
- Only Red / Green / Blue sorting. No Yellow.

---

## 2. Hardware to show

### Main controller and servo driver

- Arduino Uno
- PCA9685 16-channel PWM servo driver, I2C at `A4/A5`
- Servo power: external **5–6 V** supply, **not** Arduino 5V
- Common ground between Arduino, PCA9685, sensors, and power supply
- 2200 µF capacitor across PCA9685 `V+ / GND`

### Sensors

- HC-SR04 ultrasonic: `TRIG=D9`, `ECHO=D10`, `VCC=5V`, `GND=GND`
- TCS3200 color sensor: `S0=D4 S1=D5 S2=D6 S3=D7 OUT=D8`, `VCC=5V`, `GND=GND`, `OE=GND`

### Servo channel map

| Channel | Joint | Safe operational range | Notes |
|---:|---|---|---|
| 0 | Base / waist | **335–415**, center **375** | rotates only while upright |
| 1 | Shoulder | **250–275**, center **260** | 260 = vertical |
| 2 | Elbow | **205–310**, center **300** | reach down to **218** |
| 3 | — | **DO NOT USE** | faulty PCA9685 output |
| 4 | Wrist rotation | **DEAD SERVO** | not used |
| 6 | Wrist pitch | **370–380**, fixed **375** | held neutral |
| 7 | Gripper | **120 open / 200 closed** | verified travel only |

Full physical calibration ranges are wider, but the assembled heavy arm uses only the narrower operational clamps above.

### Known hardware faults to mention honestly

- PCA9685 channel 3 is faulty.
- Wrist-rotation servo on channel 4 is dead and unused.
- Gripper useful travel is only `120–200`.
- Shoulder deep bend is avoided; elbow does the reach.

---

## 3. Safe motion sequence

| Step | Base | Shoulder | Elbow | Gripper | Purpose |
|---|---|---|---|---|---|
| Home | 375 | 260 | 300 | 120 | compact and balanced |
| Reach down | 375 | 258 | 300 → 278 → 248 → 218 | 120 | elbow-only reach |
| Grip | 375 | 258 | 218 | 200 | grasp object |
| Lift | 375 | 260 | 218 → 240 → 262 → 284 → 300 | 200 | light forearm lifts first |
| Rotate | 375 → bin | 260 | 300 | 200 | upright only |
| Lower | bin | 258 | 300 → 278 → 248 → 218 | 200 | lower over bin |
| Release | bin | 258 | 218 | 120 | drop object |
| Return home | 375 | 260 | 300 | 120 | ready for next cycle |

Bin angles:

- **RED = 335**
- **GREEN = 375**
- **BLUE = 415**

---

## 4. Safety rules before running

1. Clamp the base to the table.
2. Support the arm during first run and during the 6-second startup recovery.
3. Power off before changing wiring.
4. Use external servo power with common ground.
5. Keep payload at or below about **100 g** for the demo.
6. Do not command servos outside the ranges above.
7. Never use PCA9685 channel 3 or channel 4.

---

## 5. Software and sketches

All sketches compile for `arduino:avr:uno`.

| Sketch | Folder | Purpose | When to run |
|---|---|---|---|
| `pick_and_place.ino` | `code/pick_and_place/` | Stable elbow-reach pick-and-place | Motion-only demo |
| `integrated_sorting.ino` | `code/integrated_sorting/` | Full demo: ultrasonic + color + pick/place | Main faculty demo |
| `color_sensor_test.ino` | `code/color_sensor_test/` | TCS3200 white-balance color test | Sensor-only demo |
| `ultrasonic_diagnostic.ino` | `code/ultrasonic_diagnostic/` | HC-SR04 distance test | Sensor-only demo |

### Compile

```powershell
$cli = "C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
& $cli compile --fqbn arduino:avr:uno "C:\Users\saumya\robotic-arm-pick-and-place\code\integrated_sorting"
```

### Upload

```powershell
& $cli upload -p COM8 --fqbn arduino:avr:uno "C:\Users\saumya\robotic-arm-pick-and-place\code\integrated_sorting"
```

### Serial Monitor

- Open with `Ctrl + Shift + M`
- Select **Arduino Uno** and **COM8**
- Set baud to **115200**
- If it says “not connected”, close the monitor, reselect board/port, reopen, or restart Arduino IDE

---

## 6. Sensor behavior and ranges

### HC-SR04

- Object detection range used by firmware: **2–10 cm**
- Mount the sensor facing the pick zone
- Keep the beam perpendicular to the object
- Soft or curved targets may give weak echoes

### TCS3200 color sensor

- Object distance: **1.5–3.0 cm**
- Frequency scaling: **20%** (`S0=HIGH`, `S1=LOW`)
- Filter truth table:
  - `S2=LOW, S3=LOW` -> Red
  - `S2=HIGH, S3=HIGH` -> Green
  - `S2=LOW, S3=HIGH` -> Blue
  - `S2=HIGH, S3=LOW` -> Clear
- Lower pulse width means a brighter channel
- The sketch performs **3 seconds of white balance** at startup
- During startup, keep the sensor area empty
- Expected empty-background result: `UNKNOWN / NEUTRAL`
- Expected colored-object result: `RED`, `GREEN`, or `BLUE`

---

## 7. Suggested faculty demo flow

1. **Show hardware and safety**
   - Point out common ground, external servo power, capacitor, clamped base.
   - Mention channel 3 fault and dead wrist-rotation servo.

2. **Show wiring**
   - PCA9685 on `A4/A5`
   - HC-SR04 on `D9/D10`
   - TCS3200 on `D4–D8`

3. **Run color sensor test**
   - Upload `color_sensor_test.ino`
   - Keep sensor empty for 3 seconds
   - Show `UNKNOWN / NEUTRAL`
   - Hold red, green, and blue objects 1.5–2 cm away
   - Show each prints `RED`, `GREEN`, or `BLUE`

4. **Run ultrasonic diagnostic**
   - Upload `ultrasonic_diagnostic.ino`
   - Move an object 2–10 cm away
   - Show distance readings

5. **Run motion-only demo**
   - Upload `pick_and_place.ino`
   - Show elbow-reach sequence
   - Explain shoulder stays vertical and base rotates only while upright

6. **Run full integrated demo**
   - Upload `integrated_sorting.ino`
   - Keep pick area empty during the 3-second color calibration
   - Place a red/green/blue object 2–10 cm from ultrasonic sensor
   - Show serial output:
     - object detected
     - color recognized
     - pick sequence
     - place into correct bin

7. **Show final state**
   - Arm returns home
   - Gripper opens
   - System ready for next object

---

## 8. Expected serial output

Color test:

```text
Reference -> R:... G:... B:...
R: ... | G: ... | B: ... | norm R:1.00 G:1.00 B:1.00 => UNKNOWN / NEUTRAL
R: ... | G: ... | B: ... | norm R:1.45 G:1.40 B:1.39 => RED
```

Integrated sorting:

```text
Colour reference -> R:... G:... B:...
Scan -> R:... G:... B:... C:... | norm R:... G:... B:...
Colour recognised: RED
PICK: reaching down (elbow only)...
PLACE: rotating upright to base 335
```

---

## 9. Troubleshooting during the demo

| Problem | Fix |
|---|---|
| Serial monitor says “not connected” | Close monitor, reselect board/port, reopen, or restart IDE |
| Upload fails on COM8 | Close Serial Monitor, kill `serial-monitor` / `arduino-cli`, upload again |
| Color reads `MIXED/UNKNOWN` | Hold object at 1.5–2 cm, shade the sensor, recalibrate with empty area |
| Blue only works very close | Increase light, reduce distance, keep sensor shaded |
| Arm collapses | Support arm, verify elbow-reach mode, do not let shoulder deep-bend |
| Base does not rotate | Arm must be folded upright first |
| Servo on channel 3/4 does nothing | Expected; avoid those channels |

---

## 10. Files to point to during presentation

- `README.md`
- `hardware/pin_connections.md`
- `docs/safe_motion_chart.md`
- `docs/sensor_integration.md`
- `docs/troubleshooting.md`
- `tests/color_sensor_test_results.md`
- `tests/servo_test_results.md`
- `IMPLEMENTATION_PLAN.md`
- `code/pick_and_place/pick_and_place.ino`
- `code/integrated_sorting/integrated_sorting.ino`
- `code/color_sensor_test/color_sensor_test.ino`
- `code/ultrasonic_diagnostic/ultrasonic_diagnostic.ino`

---

## 11. One-line summary for faculty

> “This is a safe elbow-reach robotic arm with calibrated servo limits, common-ground 5 V sensor wiring, ultrasonic object detection, white-balanced TCS3200 color sorting, and Red/Green/Blue bin placement.”
