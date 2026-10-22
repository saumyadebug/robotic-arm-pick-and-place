/*
 * HC-SR04 Ultrasonic Distance Sensor Test
 * Robotic Arm Pick and Place System
 *
 * WIRING (Arduino Uno):
 *   HC-SR04 VCC  -> 5V
 *   HC-SR04 GND  -> GND
 *   HC-SR04 TRIG -> D9
 *   HC-SR04 ECHO -> D10
 *
 * Mount the sensor so it faces the pick zone. The integrated sorting
 * sketch triggers when an object is between 2 cm and 10 cm away.
 */

#define TRIG_PIN 9
#define ECHO_PIN 10

const float OBJECT_MIN_CM = 2.0;
const float OBJECT_MAX_CM = 10.0;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println(F("HC-SR04 test ready. Object trigger window: 2-10 cm."));
}

float measureDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1.0;

  return (duration * 0.0343) / 2.0;
}

void loop() {
  float distance = measureDistanceCM();

  if (distance > 0) {
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.print(F(" cm"));

    if (distance >= OBJECT_MIN_CM && distance <= OBJECT_MAX_CM) {
      Serial.println(F(" -> [OBJECT IN PICK ZONE]"));
    } else {
      Serial.println(F(" -> [zone clear]"));
    }
  } else {
    Serial.println(F("Sensor timeout / out of range."));
  }

  delay(250);
}
