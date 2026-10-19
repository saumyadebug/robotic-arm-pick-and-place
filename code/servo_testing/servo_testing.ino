/*
 * Servo Testing Routine
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Sequentially tests and sweeps each connected servo motor to verify
 * smooth operation, correct rotational direction, and mechanical limits.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)
#define USMIN     600 // Microsecond minimum
#define USMAX     2400 // Microsecond maximum
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz

const uint8_t TOTAL_SERVOS = 6;

void setup() {
  Serial.begin(115200);
  Serial.println(F("--- Robotic Arm Servo Sweep Test ---"));

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  delay(1000);
}

void loop() {
  for (uint8_t ch = 0; ch < TOTAL_SERVOS; ch++) {
    Serial.print(F("Testing Servo Channel: "));
    Serial.println(ch);

    // Sweep from Min to Max
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen += 5) {
      pwm.setPWM(ch, 0, pulselen);
      delay(15);
    }
    delay(500);

    // Sweep from Max to Min
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen -= 5) {
      pwm.setPWM(ch, 0, pulselen);
      delay(15);
    }
    delay(500);
  }

  Serial.println(F("Cycle complete. Waiting 3 seconds..."));
  delay(3000);
}
