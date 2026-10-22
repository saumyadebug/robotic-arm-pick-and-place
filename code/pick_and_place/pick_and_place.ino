/*
 * Pick and Place  -  ELBOW-REACH SAFE MODE (heavy metal arm)
 *
 * ROOT-CAUSE FIX:
 * The shoulder was doing the deep bend, so the whole arm's weight hung in
 * front of the base -> servo could not hold it, arm collapsed, and lifting
 * it back up needed more torque than the servo has.
 *
 * New strategy: the SHOULDER STAYS NEAR VERTICAL (260). The reach down is
 * done with the ELBOW only (300 -> 245). That lowers the gripper while the
 * heavy upper arm stays balanced over the base, and lifting back up only
 * moves the light forearm + gripper.
 *
 * Channel map:
 *   CH_BASE      0   clamped 335-415
 *   CH_SHOULDER  1   clamped 250-275 (stays vertical)
 *   CH_ELBOW     2   clamped 240-310 (does the reach)
 *   CH_WRIST     6   fixed 375
 *   CH_GRIPPER   7   open 120 / closed 200
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      6
#define CH_GRIPPER    7

// Clamps
#define BASE_MIN      335
#define BASE_MAX      415
#define SHOULDER_MIN  250
#define SHOULDER_MAX  275
#define ELBOW_MIN     205
#define ELBOW_MAX     310
#define WRIST_MIN     370
#define WRIST_MAX     380

#define GRIPPER_OPEN   120
#define GRIPPER_CLOSED 200

// Poses
#define HOME_BASE      375
#define HOME_SHOULDER  260
#define HOME_ELBOW     300
#define HOME_WRIST     375

// Reach: elbow does the work, shoulder stays vertical.
// Elbow folds much deeper now, so the gripper drops without the heavy
// upper arm leaning forward (that was what made the arm collapse).
#define REACH_SHOULDER 258
#define REACH_ELBOW    218

// Bins
#define BIN_RED        335
#define BIN_GREEN      375
#define BIN_BLUE       415

int clampPulse(uint8_t ch, int p) {
  if (ch == CH_BASE)     return constrain(p, BASE_MIN, BASE_MAX);
  if (ch == CH_SHOULDER) return constrain(p, SHOULDER_MIN, SHOULDER_MAX);
  if (ch == CH_ELBOW)    return constrain(p, ELBOW_MIN, ELBOW_MAX);
  if (ch == CH_WRIST)    return constrain(p, WRIST_MIN, WRIST_MAX);
  return p;
}

void setP(uint8_t ch, int p) { pwm.setPWM(ch, 0, clampPulse(ch, p)); }

// Lift the forearm in small steps so it never stalls.
// Elbow first (only the light forearm moves), then shoulder back to vertical.
void liftUpright(int grip) {
  setP(CH_ELBOW, 240);  delay(550);
  setP(CH_ELBOW, 262);  delay(550);
  setP(CH_ELBOW, 284);  delay(550);
  setP(CH_SHOULDER, HOME_SHOULDER); delay(550);
  setP(CH_ELBOW, HOME_ELBOW);       delay(650);
  setP(CH_WRIST, HOME_WRIST);       delay(200);
  pwm.setPWM(CH_GRIPPER, 0, grip);  delay(500);
}

// Lower the forearm in small steps. The shoulder stays vertical, so the
// heavy upper arm never hangs out in front of the base.
void reachDown() {
  setP(CH_SHOULDER, REACH_SHOULDER); delay(400);
  setP(CH_ELBOW, 278);               delay(500);
  setP(CH_ELBOW, 248);               delay(500);
  setP(CH_ELBOW, REACH_ELBOW);       delay(800);
}

// Upright first, then rotate. Small, slow base move.
void rotateBaseSlow(int target) {
  liftUpright(GRIPPER_CLOSED);
  setP(CH_BASE, target);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println(F("SAFE MODE - recovering to upright, 6s"));
  setP(CH_ELBOW, 280);  delay(600);
  setP(CH_ELBOW, HOME_ELBOW);      delay(600);
  setP(CH_SHOULDER, HOME_SHOULDER); delay(600);
  setP(CH_WRIST, HOME_WRIST);       delay(300);
  setP(CH_BASE, HOME_BASE);         delay(600);
  pwm.setPWM(CH_GRIPPER, 0, GRIPPER_OPEN); delay(6000);
}

void loop() {
  Serial.println(F("1. Reach down (elbow only)"));
  reachDown();

  Serial.println(F("2. Grip"));
  pwm.setPWM(CH_GRIPPER, 0, GRIPPER_CLOSED); delay(700);

  Serial.println(F("3. Lift upright"));
  liftUpright(GRIPPER_CLOSED);

  Serial.println(F("4. Rotate base to RED bin (upright)"));
  rotateBaseSlow(BIN_RED);

  Serial.println(F("5. Reach down over bin"));
  reachDown();

  Serial.println(F("6. Release"));
  pwm.setPWM(CH_GRIPPER, 0, GRIPPER_OPEN); delay(700);

  Serial.println(F("7. Lift upright"));
  liftUpright(GRIPPER_OPEN);

  Serial.println(F("8. Rotate home"));
  rotateBaseSlow(HOME_BASE);

  Serial.println(F("Cycle done - pause 3s"));
  delay(3000);
}
