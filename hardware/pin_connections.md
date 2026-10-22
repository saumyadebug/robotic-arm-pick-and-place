# Pin Connection Map

Arduino Uno + PCA9685 + HC-SR04 + TCS3200 + heavy metal arm.

> **Power off / unplug the battery before changing any wiring.**
> Servo power must come from the external buck converter, **not** the Arduino 5V pin.

---

## 1. Arduino to PCA9685 (I2C)

| Arduino Uno | PCA9685 | Notes |
|---|---|---|
| A4 (SDA) | SDA | I2C data |
| A5 (SCL) | SCL | I2C clock |
| 5V | VCC | logic power only |
| GND | GND | common logic ground |
| — | OE | leave floating or tie to GND |
| External 5–6 V PSU + | V+ screw terminal | servo power |
| External PSU − | GND screw terminal | **must be tied to Arduino GND** |

---

## 2. PCA9685 servo channels (working configuration)

| Ch | Joint | Servo | Safe/operational range | Notes |
|---|---|---|---|---|
| **0** | Base / waist | MG996R | **335–415** (center 375) | rotate only while upright |
| **1** | Shoulder | MG996R | **250–275** (center 260) | 260 = vertical |
| **2** | Elbow | MG996R | **205–310** (center 300) | does the reach down |
| **3** | — | — | **DO NOT USE** | faulty PCA9685 output |
| **4** | Wrist rotation | — | **DEAD SERVO** | replace; not used |
| **5** | Spare | — | — | unused |
| **6** | Wrist pitch | MG90S/SG90 | **370–380** (fixed 375) | held neutral |
| **7** | Gripper | MG90S/SG90 | **120 open / 200 closed** | verified travel only |

Full physical calibration (before operational narrowing): base 200–500, shoulder 200–340, elbow 200–420, wrist 300–450.

---

## 3. HC-SR04 ultrasonic sensor

| HC-SR04 pin | Arduino Uno pin | Notes |
|---|---|---|
| VCC | **5V** | sensor logic supply |
| GND | **GND** | common ground |
| TRIG | **D9** | trigger output |
| ECHO | **D10** | echo input |

**Mounting:** face the pick zone. The integrated sketch triggers when an object is **2–10 cm** in front of the sensor. Keep the beam perpendicular to the object and avoid soft/curved targets.

---

## 4. TCS3200 / TCS230 color sensor

| TCS3200 pin | Arduino Uno pin | Notes |
|---|---|---|
| VCC | **5V** | module supply |
| GND | **GND** | common ground |
| S0 | **D4** | frequency scaling |
| S1 | **D5** | frequency scaling |
| S2 | **D6** | filter select |
| S3 | **D7** | filter select |
| OUT | **D8** | frequency output |
| OE | **GND** | output enable (active low), if exposed |

**Settings used in firmware:** `S0 = HIGH`, `S1 = LOW` → **20% frequency scaling**.

**Filter truth table:**

| S2 | S3 | Channel |
|---|---|---|
| LOW | LOW | Red |
| HIGH | HIGH | Green |
| LOW | HIGH | Blue |
| HIGH | LOW | Clear / no filter |

**Mounting:** place the sensor **1.5–3.0 cm** from the object, shaded from direct room light. Lower pulse width = brighter channel.

---

## 5. Grounding checklist

- [ ] Buck converter negative output tied to Arduino GND.
- [ ] PCA9685 GND tied to Arduino GND.
- [ ] HC-SR04 GND tied to Arduino GND.
- [ ] TCS3200 GND tied to Arduino GND.
- [ ] 2200 µF capacitor across PCA9685 V+ / GND (observe polarity).
- [ ] Servo power **not** taken from Arduino 5V.

---

## 6. Quick pin summary

```
PCA9685: SDA=A4  SCL=A5  VCC=5V  GND=GND
HC-SR04: TRIG=D9  ECHO=D10  VCC=5V  GND=GND
TCS3200: S0=D4  S1=D5  S2=D6  S3=D7  OUT=D8  VCC=5V  GND=GND  OE=GND
Servos:  base=ch0  shoulder=ch1  elbow=ch2  wrist=ch6  gripper=ch7
Never:   ch3 (faulty)  ch4 (dead wrist rotation)
```
