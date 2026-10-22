# Sensor Integration

## TCS3200 Color Sensor Integration
- **Mounting**: Sensor is gripper-mounted (per project report) to scan objects at a fixed 1.5-3.0 cm distance during pick.
- **Shielding**: Ambient room lighting variations can corrupt RGB frequency readings. Enclosing the sensor and target staging area in an opaque shroud improves classification accuracy.
- **White Balance**: Calibrate readings using a pure white card and a pure black surface prior to running sorting routines.

## HC-SR04 Ultrasonic Integration
- **Acoustic Reflections**: Ensure sound beam hits flat surfaces perpendicularly. Curved or soft sponge objects may scatter sound waves.
- **Software Debouncing**: Require 3 consecutive consistent readings before transitioning states to avoid spurious noise triggers.
