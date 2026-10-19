# Power Supply & Electrical Guidelines

Proper power distribution is the single most critical factor for reliable robotic arm operation. Servos draw substantial instantaneous current, especially when starting motion or carrying payloads.

---

## Current Consumption Estimates

| Actuator / Component | Idle Current | Operating Current | Stall Current |
|----------------------|--------------|-------------------|---------------|
| MG996R (x3) | ~15 mA each | 500 - 800 mA each | 2.5 A each |
| SG90 / MG90S (x2) | ~10 mA each | 200 - 300 mA each | 800 mA each |
| Arduino Uno | ~50 mA | ~50 mA | ~50 mA |
| Sensors (TCS3200 + HC-SR04) | ~30 mA | ~40 mA | ~40 mA |
| **Total Peak Estimate** | **~100 mA** | **~3.0 A - 4.0 A** | **~8.5 A - 9.0 A** |

---

## Power Distribution Rules

1. **Never Power Servos from Arduino 5V Pin**:
   The Arduino onboard 5V regulator can only supply ~500-800 mA max. Powering servos directly from it will cause voltage brownouts, resetting the microcontroller and damaging the regulator.

2. **Common Ground is Mandatory**:
   The ground (`GND`) of the external 5V/6V power supply MUST be connected to the Arduino `GND`. Without a common ground reference, the PWM control signals will be noisy and erratic.

3. **Capacitor Decoupling**:
   Install a large capacitor (1000 µF to 2200 µF, 16V electrolytic) across the V+ and GND terminals of the PCA9685 board to buffer voltage dips during sudden acceleration.

4. **Battery & Buck Converter Setup**:
   - **7.4V 2500mAh 2S Battery**: Provides sufficient discharge capacity for servo transient loads.
   - **DC-DC Step-Down Buck Converter**: Essential to step down 7.4V to 5V–6V regulated output for the PCA9685 V+ rail. Do not feed 7.4V directly to standard 5V-6V rated servos as it will burn their internal control circuits.
   - **Common Ground**: Ensure the Buck Converter negative output is tied to Arduino GND.
