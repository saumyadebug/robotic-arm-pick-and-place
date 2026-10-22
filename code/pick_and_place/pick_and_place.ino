/*
 * Basic Pick and Place Sequence
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Executes a deterministic pick-and-place routine between two fixed
 * coordinates (Pick Station and Drop Station) with smooth motion transitions.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo channels (6-DOF)
#define CH_BASE        0
#define CH_SHOULDER    1
#define CH_ELBOW       2
#define CH_WRIST_PITCH 3
#define CH_GRIPPER     4
#define CH_WRIST_ROLL  5

// Gripper states (Pulse counts)
#define GRIPPER_OPEN   220
#define GRIPPER_CLOSED 420

void setJointPulse(uint8_t channel, int pulse) {
  pwm.setPWM(channel, 0, pulse);
}

void moveArmSmooth(int b, int s, int e, int w, int wr, int g, int stepDelay = 20) {
  // Direct position update (expandable to interpolated trajectory)
  setJointPulse(CH_BASE, b);
  setJointPulse(CH_SHOULDER, s);
  setJointPulse(CH_ELBOW, e);
  setJointPulse(CH_WRIST_PITCH, w);
  setJointPulse(CH_WRIST_ROLL, wr);
  setJointPulse(CH_GRIPPER, g);
  delay(stepDelay * 20);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println(F("Robotic Arm Initializing to Home Position..."));
  // Home position: Base center, arm elevated, gripper open
  moveArmSmooth(375, 300, 350, 300, 375, GRIPPER_OPEN);
  delay(2000);
}

void loop() {
  Serial.println(F("1. Approaching Pick Location..."));
  moveArmSmooth(250, 420, 280, 250, 375, GRIPPER_OPEN);
  delay(1000);

  Serial.println(F("2. Closing Gripper (Grasping Object)..."));
  setJointPulse(CH_GRIPPER, GRIPPER_CLOSED);
  delay(800);

  Serial.println(F("3. Lifting Object..."));
  moveArmSmooth(250, 320, 350, 300, 375, GRIPPER_CLOSED);
  delay(1000);

  Serial.println(F("4. Moving to Drop Location..."));
  moveArmSmooth(500, 320, 350, 300, 375, GRIPPER_CLOSED);
  delay(1000);

  Serial.println(F("5. Lowering to Drop Box..."));
  moveArmSmooth(500, 420, 280, 250, 375, GRIPPER_CLOSED);
  delay(800);

  Serial.println(F("6. Opening Gripper (Release)..."));
  setJointPulse(CH_GRIPPER, GRIPPER_OPEN);
  delay(800);

  Serial.println(F("7. Returning to Standby Position..."));
  moveArmSmooth(375, 300, 350, 300, 375, GRIPPER_OPEN);
  delay(3000);
}