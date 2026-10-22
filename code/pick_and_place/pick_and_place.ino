/*
 * Basic Pick and Place Sequence
 * Robotic Arm Pick and Place System
 *
 * Channel mapping (calibrated):
 *   CH_BASE      0   range 200-500, center 375
 *   CH_SHOULDER  1   range 200-340, center 260 (vertical)
 *   CH_ELBOW     2   range 200-420, center 300 (vertical)
 *   CH_WRIST     6   held at 375 (neutral)
 *   CH_GRIPPER   7   open 120, closed 200
 *
 * Safety: the arm is folded UPRIGHT (compact) before every base rotation
 * so the center of mass stays over the base - prevents tipping.
 * Joints move sequentially (20 ms apart) to limit peak current.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      6
#define CH_GRIPPER    7

#define BASE_MIN      200
#define BASE_MAX      500
#define SHOULDER_MIN  200
#define SHOULDER_MAX  340
#define ELBOW_MIN     200
#define ELBOW_MAX     420
#define WRIST_MIN     300
#define WRIST_MAX     450

#define GRIPPER_OPEN   120
#define GRIPPER_CLOSED 200

// --- Poses (ticks) ---
// Compact upright pose: arm folded straight up -> balanced over base
#define P_HOME_BASE      375
#define P_HOME_SHOULDER  260
#define P_HOME_ELBOW     300
#define P_HOME_WRIST     375

// Reach pose: lean forward and lower forearm to the object (small, controlled)
#define P_REACH_BASE     300
#define P_REACH_SHOULDER 310
#define P_REACH_ELBOW    250

// Bin angles (base only; arm stays compact while rotating)
#define BIN_RED          250
#define BIN_GREEN        375
#define BIN_BLUE         475

int clampPulse(uint8_t channel, int pulse) {
  if (channel == CH_BASE)     return constrain(pulse, BASE_MIN, BASE_MAX);
  if (channel == CH_SHOULDER) return constrain(pulse, SHOULDER_MIN, SHOULDER_MAX);
  if (channel == CH_ELBOW)    return constrain(pulse, ELBOW_MIN, ELBOW_MAX);
  if (channel == CH_WRIST)    return constrain(pulse, WRIST_MIN, WRIST_MAX);
  return pulse;
}

void setJointPulse(uint8_t channel, int pulse) {
  pwm.setPWM(channel, 0, clampPulse(channel, pulse));
}

// Sequential joint motion: base -> shoulder -> elbow -> wrist -> gripper
void moveArmSafe(int b, int s, int e, int w, int g, int stepDelay = 25) {
  setJointPulse(CH_BASE, b);     delay(stepDelay);
  setJointPulse(CH_SHOULDER, s); delay(stepDelay);
  setJointPulse(CH_ELBOW, e);    delay(stepDelay);
  setJointPulse(CH_WRIST, w);    delay(stepDelay);
  setJointPulse(CH_GRIPPER, g);  delay(stepDelay * 10);
}

// Go upright first (mass centered), then rotate base - avoids tipping
void rotateBaseSafe(int targetBase) {
  moveArmSafe(P_HOME_BASE, P_HOME_SHOULDER, P_HOME_ELBOW, P_HOME_WRIST, GRIPPER_CLOSED, 25);
  delay(600);                                   // let it settle upright
  setJointPulse(CH_BASE, targetBase);           // rotate only while compact
  delay(900);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println(F("Initializing to compact home pose..."));
  moveArmSafe(P_HOME_BASE, P_HOME_SHOULDER, P_HOME_ELBOW, P_HOME_WRIST, GRIPPER_OPEN);
  delay(2000);
}

void loop() {
  Serial.println(F("1. Reach to pick zone (compact lean)..."));
  moveArmSafe(P_REACH_BASE, P_REACH_SHOULDER, P_REACH_ELBOW, P_HOME_WRIST, GRIPPER_OPEN);
  delay(800);

  Serial.println(F("2. Close gripper..."));
  setJointPulse(CH_GRIPPER, GRIPPER_CLOSED);
  delay(700);

  Serial.println(F("3. Lift upright (balance)..."));
  moveArmSafe(P_REACH_BASE, P_HOME_SHOULDER, P_HOME_ELBOW, P_HOME_WRIST, GRIPPER_CLOSED);
  delay(800);

  Serial.println(F("4. Rotate base to RED bin (arm compact)..."));
  rotateBaseSafe(BIN_RED);

  Serial.println(F("5. Lower to bin..."));
  moveArmSafe(BIN_RED, P_REACH_SHOULDER, P_REACH_ELBOW, P_HOME_WRIST, GRIPPER_CLOSED);
  delay(700);

  Serial.println(F("6. Release..."));
  setJointPulse(CH_GRIPPER, GRIPPER_OPEN);
  delay(700);

  Serial.println(F("7. Lift upright..."));
  moveArmSafe(BIN_RED, P_HOME_SHOULDER, P_HOME_ELBOW, P_HOME_WRIST, GRIPPER_OPEN);
  delay(600);

  Serial.println(F("8. Return to home..."));
  rotateBaseSafe(P_HOME_BASE);
  delay(2000);
}
