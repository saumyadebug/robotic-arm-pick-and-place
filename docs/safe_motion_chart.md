# Safe Motion Chart — Calibrated Values (2026-10-23)

**Arm:** heavy metal 4-DOF + gripper (MG995/MG996R joints)
**Controller:** Arduino Uno + PCA9685 (I2C, 50 Hz)
**Purpose:** keep the arm stable, avoid servo overload, and complete a pick-and-place cycle without tipping.

---

## 1. Channel map and calibrated limits

| Joint | PCA9685 ch | Full calibrated range | Operational clamp used in code | Notes |
|---|---|---|---|---|
| Base / waist | **0** | 200–500 | **335–415** | center 375; rotate only while arm is upright |
| Shoulder | **1** | 200–340 | **250–275** | center **260 = vertical**; stays vertical during reach |
| Elbow | **2** | 200–420 | **205–310** | center **300 = vertical**; does the reach down |
| Wrist pitch | **6** | 300–450 | **370–380** | fixed at **375** during motion |
| Wrist rotation | **4** | — | — | **SERVO DEAD — not used** |
| Gripper | **7** | 120–200 | **120 open / 200 closed** | do not command outside this travel |
| PCA9685 ch3 | 3 | — | — | **FAULTY OUTPUT — never use** |

> The full calibrated range is wider than what the assembled heavy arm can safely use. The code uses the narrower operational clamp above.

---

## 2. Why the arm was collapsing

The shoulder was doing the deep bend. That hangs the entire arm's weight in front of the base. The shoulder servo could not hold that torque, the arm fell, and lifting it back from the extended position was impossible.

**Fix:** the shoulder stays essentially vertical (258). The **elbow** does the reach down (300 → 278 → 248 → 218), so only the light forearm moves. Lifting reverses the sequence in small steps (218 → 240 → 262 → 284 → 300). The base rotates only after the arm is folded upright.

---

## 3. Pick-and-place sequence (implemented)

| Step | Base | Shoulder | Elbow | Gripper | Purpose |
|---|---|---|---|---|---|
| Recover / home | 375 | 260 | 300 | open 120 | compact, balanced |
| 1. Reach down | 375 | 258 | 300 → 278 → 248 → **218** | open 120 | elbow-only reach |
| 2. Grip | 375 | 258 | 218 | **closed 200** | grasp object |
| 3. Lift | 375 | 260 | 218 → 240 → 262 → 284 → **300** | closed | light forearm lifts first |
| 4. Rotate | 375 → bin | 260 | 300 | closed | base moves only while upright |
| 5. Lower | bin | 258 | 300 → 278 → 248 → **218** | closed | lower over bin |
| 6. Release | bin | 258 | 218 | **open 120** | drop object |
| 7. Lift | bin | 260 | 218 → … → **300** | open | return upright |
| 8. Home | 375 | 260 | 300 | open | ready for next cycle |

**Bin angles:** RED = **335**, GREEN = **375**, BLUE = **415** (all inside the 335–415 base clamp).

---

## 4. Safety rules

1. **Clamp the base to the table** before running.
2. **Fold the arm upright before every base rotation.**
3. **Move one joint at a time** with delays between steps.
4. **Every pulse is clamped** in firmware; never command outside the limits above.
5. **Support the arm during the first run** and during the 6-second startup recovery.
6. **Power:** XL4016 5–6 V, ≥6 A peak, common ground, 2200 µF capacitor across PCA9685 V+/GND.
7. **Payload:** start with ≤100 g; maximum intended ≤180 g.
8. **No full-range sweeps** on the assembled arm.

---

## 5. Known hardware faults

| Fault | Symptom | Action |
|---|---|---|
| PCA9685 channel 3 | no output on any servo | avoid channel 3 |
| Wrist-rotation servo (ch4) | holds but never rotates, even with bare shaft | replace the servo |
| Gripper travel | only 120–200 ticks move | use open 120 / closed 200 |
| Shoulder deep bend | arm falls and cannot lift | use elbow-reach mode above |

---

## 6. Related files

- Firmware: `code/pick_and_place/pick_and_place.ino`
- Integrated sensors: `code/integrated_sorting/integrated_sorting.ino`
- Wiring: `hardware/pin_connections.md`
- Sensor guide: `docs/sensor_integration.md`
