/*
 * Basic Pick and Place Sequence
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Executes a deterministic pick-and-place routine between two fixed
 * coordinates (Pick Station and Drop Station) with safe, sequential
 * joint motion to avoid mechanical collisions and servo overload.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo channels
#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      3
#define CH_GRIPPER    4

// Per-joint safe pulse limits (calibrate and update these)
#define BASE_MIN      150
#define BASE_MAX      600
#define SHOULDER_MIN  180
#define SHOULDER_MAX  580
#define ELBOW_MIN     160
#define ELBOW_MAX     590
#define WRIST_MIN     150
#define WRIST_MAX     600

// Gripper states (Pulse counts)
#define GRIPPER_OPEN   220
#define GRIPPER_CLOSED 420

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

// Move joints sequentially to reduce simultaneous load and collision risk
void moveArmSafe(int b, int s, int e, int w, int g, int stepDelay = 20) {
  setJointPulse(CH_BASE, b);
  delay(stepDelay);
  setJointPulse(CH_SHOULDER, s);
  delay(stepDelay);
  setJointPulse(CH_ELBOW, e);
  delay(stepDelay);
  setJointPulse(CH_WRIST, w);
  delay(stepDelay);
  setJointPulse(CH_GRIPPER, g);
  delay(stepDelay * 10);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println(F("Robotic Arm Initializing to Home Position..."));
  moveArmSafe(375, 300, 350, 300, GRIPPER_OPEN);
  delay(2000);
}

void loop() {
  Serial.println(F("1. Approaching Pick Location..."));
  moveArmSafe(250, 420, 280, 250, GRIPPER_OPEN);
  delay(1000);

  Serial.println(F("2. Closing Gripper (Grasping Object)..."));
  setJointPulse(CH_GRIPPER, GRIPPER_CLOSED);
  delay(800);

  Serial.println(F("3. Lifting Object..."));
  moveArmSafe(250, 320, 350, 300, GRIPPER_CLOSED);
  delay(1000);

  Serial.println(F("4. Moving to Drop Location..."));
  moveArmSafe(500, 320, 350, 300, GRIPPER_CLOSED);
  delay(1000);

  Serial.println(F("5. Lowering to Drop Box..."));
  moveArmSafe(500, 420, 280, 250, GRIPPER_CLOSED);
  delay(800);

  Serial.println(F("6. Opening Gripper (Release)..."));
  setJointPulse(CH_GRIPPER, GRIPPER_OPEN);
  delay(800);

  Serial.println(F("7. Returning to Standby Position..."));
  moveArmSafe(375, 300, 350, 300, GRIPPER_OPEN);
  delay(3000);
}