# Troubleshooting Guide

| Issue | Probable Cause | Corrective Action |
|-------|----------------|-------------------|
| Servos twitch or jitter violently | Unshared ground or inadequate power supply current | Connect external PSU GND to Arduino GND; ensure PSU can deliver ≥ 6A |
| Arduino resets when arm moves | Voltage brownout on logic rail | Power servos strictly from external supply; add 2200µF capacitor across PCA9685 V+ |
| PCA9685 not detected on I2C | Loose SDA/SCL wiring or incorrect I2C address | Run I2C scanner sketch; check A4 (SDA) / A5 (SCL) connections |
| Color sensor reads all colors as UNKNOWN | Incorrect ambient light or inverted pulse logic | Verify S0-S3 logic; check distance from sensor to object (ideal: 1.5 - 3.0 cm) |
| Ultrasonic sensor returns 0 or 999 cm | Wiring mistake or target out of detection cone | Check TRIG (D9) and ECHO (D10); check for objects within 2cm - 20cm range |
| Gripper servo humming continuously | Over-gripping (stalling against rigid object) | Increase open/close pulse margin; do not clamp beyond physical contact |
