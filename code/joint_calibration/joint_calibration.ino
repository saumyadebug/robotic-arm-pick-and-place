/*
 * Joint Calibration Utility
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Interactive Serial monitor tool to calibrate individual servo joints.
 * Allows entering target channel and pulse width or angle to identify exact
 * physical mechanical limits (0 deg, 90 deg, 180 deg) without stalling.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVO_FREQ 50

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  Serial.println(F("========================================"));
  Serial.println(F("      Robotic Arm Joint Calibration      "));
  Serial.println(F("========================================"));
  Serial.println(F("Format: <channel> <pulse_ticks>"));
  Serial.println(F("Example: '0 375' (moves channel 0 to tick 375)"));
  Serial.println(F("Typical pulse tick range: 150 to 600"));
}

void loop() {
  if (Serial.available() > 0) {
    int channel = Serial.parseInt();
    int pulse = Serial.parseInt();

    if (channel >= 0 && channel <= 15 && pulse >= 100 && pulse <= 650) {
      Serial.print(F("Moving Channel "));
      Serial.print(channel);
      Serial.print(F(" -> Pulse: "));
      Serial.println(pulse);

      pwm.setPWM(channel, 0, pulse);
    } else {
      Serial.println(F("Invalid input! Ensure channel 0-15 and pulse 100-650."));
    }

    // Flush any remaining characters in the buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
