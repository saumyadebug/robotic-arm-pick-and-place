# Safe Motion Chart — Joint Ranges for Stable Pick-and-Place

**Arm type:** Heavy metal 4-DOF + gripper (MG995/MG996R joints)
**Controller:** Arduino Uno + PCA9685 (I2C, 50 Hz)
**Purpose:** Keep the arm stable, avoid servo overload and mechanical collision.
**Status:** **Calibrated on hardware (2026-10-23)** — ranges verified by physical testing.

---

## 1. Channel Map & Calibrated Ranges (HARDWARE VERIFIED)

| Joint | Ch | Safe Min | Center | Safe Max | Notes |
|---|---|---|---|---|---|
| Base (waist) | **0** | 200 | 375 | 500 | Verified smooth over full range |
| Shoulder | **1** | 200 | 260 | 340 | 260 = vertical; heavy arm - stay compact |
| Elbow | **2** | 200 | 300 | 420 | 300 = vertical/straight |
| Wrist | **6** | 300 | 375 | 450 | Keep neutral during transport |
| Wrist rotation | **4** | — | — | — | **SERVO DEAD** - no movement; replace |
| Gripper | **7** | 120 (open) | — | 200 (closed) | Verified working; 120-200 only travel |
| PCA9685 ch3 | 3 | — | — | — | **FAULTY OUTPUT** - do not use |

---

## 2. Why the arm tipped / collapsed (root cause)

The arm is **heavy metal** with 2 extra servos at the end effector → large center-of-mass
offset. Rotating the base while the arm was **extended** created a tipping moment, and the
shoulder could not hold the load.

**Rule: fold the arm UPRIGHT before every base rotation.**

---

## 3. Pick-and-Place Sequence (implemented in code)

| Step | Base | Shoulder | Elbow | Wrist | Gripper | Delay | Purpose |
|---|---|---|---|---|---|---|---|
| Home | 375 | 260 | 300 | 375 | 120 (open) | 2000 | Compact, balanced |
| 1. Reach | 300 | 310 | 250 | 375 | 120 | 800 | Lean to object (small) |
| 2. Grip | 300 | 310 | 250 | 375 | 200 (closed) | 700 | Grasp |
| 3. Lift | 300 | 260 | 300 | 375 | 200 | 800 | Back upright |
| 4. Rotate | →bin | 260 | 300 | 375 | 200 | 600+900 | Rotate ONLY while compact |
| 5. Lower | bin | 310 | 250 | 375 | 200 | 700 | Down to bin |
| 6. Release | bin | 310 | 250 | 375 | 120 | 700 | Open |
| 7. Lift | bin | 260 | 300 | 375 | 120 | 600 | Upright |
| 8. Home | 375 | 260 | 300 | 375 | 120 | 2000 | Reset |

**Bin angles:** RED=250, GREEN=375, BLUE=475 (all inside 200-500 safe base range)

---

## 4. Safety Rules (heavy metal arm)

1. **Clamp the base to the table** — mandatory.
2. **Fold upright before rotating** the base (arm compact = balanced).
3. **Sequential joint motion** with 15-25 ms gaps (limits peak current).
4. **Never** command outside calibrated min/max (code clamps automatically).
5. **Power:** XL4016 5-6 V, ≥6 A peak, common ground, 2200 µF cap.
6. **Payload** ≤ 180 g; start with ≤ 100 g.
7. **No full-range sweeps** on the assembled arm.
8. **Spot/support** the arm on first runs.

---

## 5. Known Hardware Faults

| Fault | Symptom | Action |
|---|---|---|
| PCA9685 channel 3 | No output on any servo | Avoid ch3 |
| Wrist rotation servo | Holds but never rotates (any channel, bare shaft too) | **Replace servo** |
| Gripper travel | Only 120-200 ticks move; past that it clamps | Use 120/200 only |

---

## 6. Calibration Procedure (per joint)

1. Power off; run `joint_calibration/joint_calibration.ino`.
2. Sweep in **10-tick steps**, observe for binding/collision.
3. Record safe min/max (keep 10-20 tick margin from hard stops).
4. Update `*_MIN/*_MAX` constants in `pick_and_place.ino` and `integrated_sorting.ino`.
5. Re-run smoothness sweep over the final range.
