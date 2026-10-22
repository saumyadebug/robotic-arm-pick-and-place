/*
 * HC-SR04 Diagnostic
 *
 * Use this when ultrasonic_test.ino prints "Sensor timeout / out of range".
 *
 * Wiring:
 *   HC-SR04 VCC  -> Arduino 5V
 *   HC-SR04 GND  -> Arduino GND
 *   HC-SR04 TRIG -> D9
 *   HC-SR04 ECHO -> D10
 *
 * Serial Monitor: 115200 baud.
 *
 * What the output means:
 *   ECHO idle = 0, pulse_us > 0  -> sensor is working
 *   ECHO idle = 0, pulse_us = 0  -> no echo: wiring, power, trigger, or sensor
 *   ECHO idle = 1, pulse_us = 0  -> ECHO line stuck high / shorted / wrong pin
 */

#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.println(F("HC-SR04 diagnostic"));
  Serial.println(F("TRIG=D9  ECHO=D10  VCC=5V  GND=GND"));
  Serial.println(F("Point the sensor at a flat box/wall 10-30 cm away."));
  Serial.println(F("--------------------------------------------------"));
  delay(1000);
}

void loop() {
  int echoIdle = digitalRead(ECHO_PIN);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  Serial.print(F("ECHO idle="));
  Serial.print(echoIdle);
  Serial.print(F("  pulse_us="));
  Serial.print(duration);

  if (duration == 0) {
    Serial.println(F("  -> NO ECHO"));
  } else {
    float cm = (duration * 0.0343) / 2.0;
    Serial.print(F("  -> "));
    Serial.print(cm);
    Serial.println(F(" cm"));
  }

  delay(500);
}
