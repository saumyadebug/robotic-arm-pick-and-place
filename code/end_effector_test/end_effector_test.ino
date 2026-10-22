/*
 * End Effector (Gripper) Test
 * Robotic Arm Pick and Place System
 *
 * Calibrated gripper:
 *   PCA9685 channel 7
 *   open   = 120 ticks
 *   closed = 200 ticks
 *
 * Only the verified 120-200 travel is used. Commanding beyond this range
 * makes the gripper servo stall and buzz.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define GRIPPER_CHANNEL   7
#define GRIPPER_OPEN      120
#define GRIPPER_CLOSED    200

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Serial.println(F("Gripper test on channel 7 (open 120 / closed 200)"));
  pwm.setPWM(GRIPPER_CHANNEL, 0, GRIPPER_OPEN);
  delay(1000);
}

void loop() {
  Serial.println(F("Closing gripper..."));
  for (int p = GRIPPER_OPEN; p <= GRIPPER_CLOSED; p += 5) {
    pwm.setPWM(GRIPPER_CHANNEL, 0, p);
    delay(30);
  }
  delay(1200);

  Serial.println(F("Opening gripper..."));
  for (int p = GRIPPER_CLOSED; p >= GRIPPER_OPEN; p -= 5) {
    pwm.setPWM(GRIPPER_CHANNEL, 0, p);
    delay(30);
  }
  delay(1200);
}
