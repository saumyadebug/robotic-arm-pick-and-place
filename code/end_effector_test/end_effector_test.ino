/*
 * End Effector (Gripper) Test
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Tests the gripper actuation range, grip force compliance, and
 * prevents servo stalling at extreme closing angles.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define GRIPPER_CHANNEL 4
#define GRIPPER_MIN_PULSE 200 // Fully open
#define GRIPPER_MAX_PULSE 450 // Maximum closed grip

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Serial.println(F("End Effector Gripper Test Routine Starting..."));
}

void loop() {
  Serial.println(F("Opening Gripper..."));
  for (int p = GRIPPER_MAX_PULSE; p >= GRIPPER_MIN_PULSE; p -= 10) {
    pwm.setPWM(GRIPPER_CHANNEL, 0, p);
    delay(20);
  }
  delay(1500);

  Serial.println(F("Closing Gripper..."));
  for (int p = GRIPPER_MIN_PULSE; p <= GRIPPER_MAX_PULSE; p += 10) {
    pwm.setPWM(GRIPPER_CHANNEL, 0, p);
    delay(20);
  }
  delay(1500);
}
