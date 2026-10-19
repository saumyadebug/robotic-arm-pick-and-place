# Working Principle

## 1. Object Detection (Ultrasonic Triggering)
The HC-SR04 sensor transmits an 8-cycle ultrasonic burst at 40 kHz. The echo pulse duration $t$ corresponds to distance $D$:
$$D = \frac{t \times v_{\text{sound}}}{2} = \frac{t \times 0.0343}{2} \text{ cm}$$
When $D \le 10\text{ cm}$, the controller flags an object present in the pick staging area.

## 2. Color Sensing (TCS3200)
The sensor contains an array of photodiodes with red, green, blue, and clear filters. The internal current-to-frequency converter outputs a square wave whose frequency is proportional to light intensity.
- Filter mode selected via pins S2 and S3.
- Output frequency scaling set to 20% via S0=HIGH, S1=LOW.
- Pulse duration is inversely proportional to color component intensity.

## 3. Motion & Trajectory
The arm moves through predefined geometric waypoints using interpolation to minimize jerk and mechanical vibration:
- **Approach Waypoint**: Arm hovers directly above the object.
- **Grasp Waypoint**: Arm descends vertically; gripper closes to calibrated grip pulse.
- **Retract Waypoint**: Arm elevates to clear obstacles.
- **Sort Waypoint**: Arm rotates base to corresponding color bin.
- **Release Waypoint**: Gripper opens; arm returns to home position.
