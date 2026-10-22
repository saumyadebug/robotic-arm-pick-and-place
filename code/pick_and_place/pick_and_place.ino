/*
 * Basic Pick and Place Sequence  -  MINIMAL SAFE MODE (heavy metal arm)
 *
 * Channel map:
 *   CH_BASE      0   clamped 340-410  (center 375)  +/- 35 ticks max
 *   CH_SHOULDER  1   clamped 250-275  (center 260)  +/- 15 ticks max
 *   CH_ELBOW     2   clamped 290-310  (center 300)  +/- 10 ticks max
 *   CH_WRIST     6   fixed 375 (never moves)
 *   CH_GRIPPER   7   open 120 / closed 200
 *
 * Design rules for stability:
 *  1. Every joint is HARD-CLAMPED to a very small band -> the arm cannot
 *     swing far even if a pose value is wrong.
 *  2. The base NEVER rotates while the arm leans: it returns to the
 *     upright pose (shoulder 260 / elbow 300) first, then rotates.
 *  3. Only ONE joint moves at a time, with a pause between each.
 *  4. Slow speeds: 40 ms between joints, 700-1000 ms settle times.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      6
#define CH_GRIPPER    7

// --- HARD SAFE CLAMPS (small on purpose) ---
#define BASE_MIN      340
#define BASE_MAX      410
#define SHOULDER_MIN  250
#define SHOULDER_MAX  275
#define ELBOW_MIN     290
#define ELBOW_MAX     310
#define WRIST_MIN     370
#define WRIST_MAX     380

#define GRIPPER_OPEN   120
#define GRIPPER_CLOSED 200

// --- Poses ---
#define HOME_BASE     375
#define HOME_SHOULDER 260
#define HOME_ELBOW    300
#define HOME_WRIST    375

// Slight forward lean / slight forearm drop to reach the object
#define REACH_SHOULDER 252   // only 8 ticks from home
#define REACH_ELBOW    292   // only 8 ticks from home

// Bins: tiny base offsets from center (arm stays upright)
#define BIN_RED       345
#define BIN_GREEN     375
#define BIN_BLUE      405

int clampPulse(uint8_t ch, int p) {
  if (ch == CH_BASE)     return constrain(p, BASE_MIN, BASE_MAX);
  if (ch == CH_SHOULDER) return constrain(p, SHOULDER_MIN, SHOULDER_MAX);
  if (ch == CH_ELBOW)    return constrain(p, ELBOW_MIN, ELBOW_MAX);
  if (ch == CH_WRIST)    return constrain(p, WRIST_MIN, WRIST_MAX);
  return p;
}

// Move ONE joint at a time, base FIRST (so mass settles before leaning)
void moveArmSafe(int b, int s, int e, int w, int g) {
  pwm.setPWM(CH_BASE, 0, clampPulse(CH_BASE, b));        delay(40);
  pwm.setPWM(CH_SHOULDER, 0, clampPulse(CH_SHOULDER, s)); delay(40);
  pwm.setPWM(CH_ELBOW, 0, clampPulse(CH_ELBOW, e));       delay(40);
  pwm.setPWM(CH_WRIST, 0, clampPulse(CH_WRIST, w));       delay(40);
  pwm.setPWM(CH_GRIPPER, 0, g);                           delay(700);
}

// Upright first, THEN rotate. This is the anti-tipping sequence.
void goUpright(int grip) {
  pwm.setPWM(CH_ELBOW, 0, clampPulse(CH_ELBOW, HOME_ELBOW));      delay(400);
  pwm.setPWM(CH_SHOULDER, 0, clampPulse(CH_SHOULDER, HOME_SHOULDER)); delay(400);
  pwm.setPWM(CH_WRIST, 0, clampPulse(CH_WRIST, HOME_WRIST));      delay(400);
  pwm.setPWM(CH_GRIPPER, 0, grip);                                delay(600);
}

void rotateBaseSlow(int target) {
  pwm.setPWM(CH_BASE, 0, clampPulse(CH_BASE, target));
  delay(1000);              // slow, small rotation
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println(F("MINIMAL SAFE MODE - holding upright 5s"));
  moveArmSafe(HOME_BASE, HOME_SHOULDER, HOME_ELBOW, HOME_WRIST, GRIPPER_OPEN);
  delay(5000);              // time to support the arm
}

void loop() {
  Serial.println(F("1. Lean slightly to object (tiny)"));
  pwm.setPWM(CH_SHOULDER, 0, clampPulse(CH_SHOULDER, REACH_SHOULDER)); delay(500);
  pwm.setPWM(CH_ELBOW, 0, clampPulse(CH_ELBOW, REACH_ELBOW));           delay(700);

  Serial.println(F("2. Grip"));
  pwm.setPWM(CH_GRIPPER, 0, GRIPPER_CLOSED); delay(700);

  Serial.println(F("3. Back upright"));
  goUpright(GRIPPER_CLOSED);

  Serial.println(F("4. Rotate to RED bin (upright, tiny)"));
  rotateBaseSlow(BIN_RED);

  Serial.println(F("5. Tiny lean over bin"));
  pwm.setPWM(CH_SHOULDER, 0, clampPulse(CH_SHOULDER, REACH_SHOULDER)); delay(500);
  pwm.setPWM(CH_ELBOW, 0, clampPulse(CH_ELBOW, REACH_ELBOW));           delay(700);

  Serial.println(F("6. Release"));
  pwm.setPWM(CH_GRIPPER, 0, GRIPPER_OPEN); delay(700);

  Serial.println(F("7. Back upright"));
  goUpright(GRIPPER_OPEN);

  Serial.println(F("8. Rotate home"));
  rotateBaseSlow(HOME_BASE);

  Serial.println(F("Cycle done - pausing 3s"));
  delay(3000);
}
