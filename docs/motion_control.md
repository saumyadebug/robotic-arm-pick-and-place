# Motion Control & Kinematics

## Inverse Kinematics vs Waypoint Navigation
For 4-DOF articulated planar arms, inverse kinematics (IK) computes joint angles $(\theta_1, \theta_2, \theta_3)$ given end-effector coordinates $(X, Y, Z)$:

$$\theta_1 = \text{atan2}(Y, X)$$

Using the geometric law of cosines:
$$r = \sqrt{X^2 + Y^2} - d_{\text{wrist}}$$
$$s = Z - h_{\text{base}}$$
$$D = \frac{r^2 + s^2 - L_1^2 - L_2^2}{2 L_1 L_2}$$
$$\theta_3 = \text{atan2}\left(\pm \sqrt{1 - D^2}, D\right)$$
$$\theta_2 = \text{atan2}(s, r) - \text{atan2}(L_2 \sin\theta_3, L_1 + L_2 \cos\theta_3)$$

---

## Trajectory Smoothing
Abrupt step changes in pulse values cause mechanical jerk and overshoot. Smooth trapezoidal velocity profiles or linear interpolation (LERP) are implemented between consecutive waypoints.
