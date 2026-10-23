# Troubleshooting Guide

| Issue | Probable Cause | Corrective Action |
|---|---|---|
| Servos twitch or jitter violently | unshared ground or inadequate supply current | connect external PSU GND to Arduino GND; ensure PSU can deliver ≥6 A |
| Arduino resets when arm moves | voltage brownout on logic rail | power servos strictly from external supply; add 2200 µF across PCA9685 V+ |
| PCA9685 not detected on I2C | loose SDA/SCL or wrong address | run I2C scanner; check A4/A5; address should be 0x40 |
| Arm collapses when bending down | shoulder is doing the deep reach | use the elbow-reach firmware; shoulder stays at ~258, elbow folds to 218 |
| Arm cannot lift after bending | shoulder lifting a fully extended arm; servo torque/supply limit | lift elbow first in stages; verify 5–6 V supply and common ground |
| Arm tips during base rotation | base rotated while arm was extended | fold upright first; base clamp is 335–415 |
| Shoulder or base buzzes continuously | mechanical bind or command outside safe range | keep pulses inside the operational clamps; check horn/link screws |
| PCA9685 channel 3 has no output | faulty driver output | never use channel 3 |
| Wrist rotation never moves | dead servo (confirmed on bare shaft) | replace the servo; do not chase it in software |
| Gripper hums continuously | over-gripping / command beyond 200 | use open 120 / closed 200 only |
| Ultrasonic returns 999 cm | wiring or target out of range | check TRIG=D9, ECHO=D10, 5V, GND; object must be 2–10 cm away |
| Ultrasonic always prints "Sensor timeout / out of range" | no echo pulse reached D10 | verify sensor pin labels (VCC/TRIG/ECHO/GND), 5V at the sensor, common ground, and jumper continuity; run `code/ultrasonic_diagnostic/` |
| Ultrasonic triggers randomly | reflections, loose wire, soft target | require 3 consecutive readings (firmware does this); secure wiring; use a flat target |
| Color sensor reads all colors as UNKNOWN | object too far, too dark, or OE not grounded | place object 1.5–3.0 cm away; ground OE; raise `DARK_LIMIT` |
| Color sensor mixes red/green/blue | ambient light or wrong filter sequence | shade the sensor; verify S2/S3 truth table; lower `MARGIN_PCT` |
| Only RED is detected; green/blue give UNKNOWN, and R≈C with G≈B | sensor S2 input is stuck / not connected, so filters never switch | check TCS3200 **S2 → Arduino D6** (not D5/D7/5V/GND); reseat or replace the S2 jumper; verify continuity |
| Color sensor output stuck | S0/S1 not set | firmware sets S0=HIGH, S1=LOW (20% scaling) |
| Sensors work alone but not together | pin conflict or shared 5V overload | verify D4–D10 are not used by anything else; power sensors from Arduino 5V, servos from buck |
