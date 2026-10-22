/*
 * Safe Servo Test Routine
 * Robotic Arm Pick and Place System
 *
 * Tests only the working channels with SMALL, verified-safe ranges.
 * This sketch deliberately does NOT sweep 0-180 degrees on the assembled
 * arm; that can tip the base or overload the shoulder.
 *
 * Calibrated channel map:
 *   0 Base       operational 335-415, center 375
 *   1 Shoulder   operational 250-275, center 260
 *   2 Elbow      operational 205-310, center 300
 *   6 Wrist      fixed around 375
 *   7 Gripper    open 120 / closed 200
 *   3            FAULTY - never use
 *   4            wrist-rotation servo DEAD - not used
 *   5            spare
 *
 * For full mechanical range testing, detach the linkage and use
 * joint_calibration.ino instead.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

struct JointTest {
  uint8_t channel;
  const char* name;
  int minPulse;
  int maxPulse;
  int centerPulse;
};

JointTest joints[] = {
  {0, "Base",     355, 395, 375},
  {1, "Shoulder", 255, 265, 260},
  {2, "Elbow",    285, 305, 300},
  {6, "Wrist",    370, 380, 375},
  {7, "Gripper",  120, 200, 160}
};

const uint8_t JOINT_COUNT = sizeof(joints) / sizeof(joints[0]);

void moveJoint(uint8_t ch, int from, int to, int stepDelay = 25) {
  int step = (to >= from) ? 5 : -5;
  for (int p = from; (step > 0) ? (p <= to) : (p >= to); p += step) {
    pwm.setPWM(ch, 0, p);
    delay(stepDelay);
  }
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  delay(1000);

  Serial.println(F("--- Safe joint test (small ranges) ---"));
  for (uint8_t i = 0; i < JOINT_COUNT; i++) {
    pwm.setPWM(joints[i].channel, 0, joints[i].centerPulse);
    delay(400);
  }
}

void loop() {
  for (uint8_t i = 0; i < JOINT_COUNT; i++) {
    Serial.print(F("Testing "));
    Serial.print(joints[i].name);
    Serial.print(F(" (ch "));
    Serial.print(joints[i].channel);
    Serial.println(F(")"));

    moveJoint(joints[i].channel, joints[i].centerPulse, joints[i].maxPulse);
    delay(500);
    moveJoint(joints[i].channel, joints[i].maxPulse, joints[i].minPulse);
    delay(500);
    moveJoint(joints[i].channel, joints[i].minPulse, joints[i].centerPulse);
    delay(800);
  }

  Serial.println(F("Cycle complete. Pausing 3 s."));
  delay(3000);
}
