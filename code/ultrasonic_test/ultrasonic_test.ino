/*
 * HC-SR04 Ultrasonic Distance Sensor Test
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Measures object distance in front of the conveyor or pick zone.
 * Triggers object presence flag when an item is within pick threshold.
 */

#define TRIG_PIN 9
#define ECHO_PIN 10

const float OBJECT_DETECT_THRESHOLD_CM = 10.0; // Trigger threshold

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println(F("HC-SR04 Ultrasonic Sensor Test Initialized."));
}

float measureDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1.0; // Out of range or timeout

  return (duration * 0.0343) / 2.0;
}

void loop() {
  float distance = measureDistanceCM();

  if (distance > 0) {
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.print(F(" cm"));

    if (distance <= OBJECT_DETECT_THRESHOLD_CM) {
      Serial.println(F(" -> [OBJECT DETECTED IN PICK ZONE!]"));
    } else {
      Serial.println(F(" -> [Zone Clear]"));
    }
  } else {
    Serial.println(F("Sensor timeout / Object out of range."));
  }

  delay(200);
}
