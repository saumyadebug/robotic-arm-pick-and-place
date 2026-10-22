# Working Principle

## 1. Object Detection (Ultrasonic Triggering)
The HC-SR04 sensor transmits an 8-cycle ultrasonic burst at 40 kHz. The echo pulse duration $t$ corresponds to distance $D$:
$$D = \frac{t \times v_{\text{sound}}}{2} = \frac{t \times 0.0343}{2} \text{ cm}$$
When $D$ is between **2 cm and 10 cm** for three consecutive readings, the controller flags an object present in the pick staging area.

## 2. Color Sensing (TCS3200)
The sensor contains an array of photodiodes with red, green, blue, and clear filters. The internal current-to-frequency converter outputs a square wave whose frequency is proportional to light intensity.
- Filter mode selected via pins S2 and S3.
- Output frequency scaling set to 20% via S0=HIGH, S1=LOW.
- Pulse duration is inversely proportional to color component intensity.

## 3. Motion & Trajectory
The arm moves through predefined waypoints with per-joint clamps and sequential motion:
- **Reach waypoint**: the shoulder stays vertical; the elbow folds down (300 → 278 → 248 → 218).
- **Grasp waypoint**: the gripper closes to 200 ticks.
- **Lift waypoint**: the elbow raises first (218 → 240 → 262 → 284 → 300), then the shoulder returns to 260.
- **Sort waypoint**: the base rotates only while the arm is folded upright.
- **Release waypoint**: the gripper opens to 120 ticks, the arm lifts, and the base returns home.

The arm does not use inverse kinematics yet; this iteration is waypoint-based for safety.
