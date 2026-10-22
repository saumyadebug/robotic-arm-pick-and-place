/*
 * TCS3200 / TCS230 Color Sensor Test
 * Robotic Arm Pick and Place System
 *
 * Description:
 * Reads raw photodiode frequencies for Red, Green, and Blue filter modes
 * and categorizes the scanned object into dominant color categories.
 */

// Pin definitions
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define SENSOR_OUT 8

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.println(F("TCS3200 Color Sensor Test Initialized."));
}

int readFrequency(bool s2Val, bool s3Val) {
  digitalWrite(S2, s2Val);
  digitalWrite(S3, s3Val);
  delay(10);
  return pulseIn(SENSOR_OUT, LOW, 50000);
}

void loop() {
  // Read Red
  int redFreq = readFrequency(LOW, LOW);
  // Read Blue
  int blueFreq = readFrequency(LOW, HIGH);
  // Read Clear (No filter)
  int clearFreq = readFrequency(HIGH, LOW);
  // Read Green
  int greenFreq = readFrequency(HIGH, HIGH);

  Serial.print(F("R: ")); Serial.print(redFreq);
  Serial.print(F(" | G: ")); Serial.print(greenFreq);
  Serial.print(F(" | B: ")); Serial.print(blueFreq);
  Serial.print(F(" | C: ")); Serial.print(clearFreq);

  // Note: For TCS3200, LOWER pulse duration means HIGHER intensity
  bool rStrong = (redFreq < 200);
  bool gStrong = (greenFreq < 200);
  bool bStrong = (blueFreq < 200);

  if (rStrong && gStrong && !bStrong) {
    Serial.println(F(" => Detected: YELLOW"));
  } else if (rStrong && !gStrong && !bStrong) {
    Serial.println(F(" => Detected: RED"));
  } else if (gStrong && !rStrong && !bStrong) {
    Serial.println(F(" => Detected: GREEN"));
  } else if (bStrong && !rStrong && !gStrong) {
    Serial.println(F(" => Detected: BLUE"));
  } else {
    Serial.println(F(" => Detected: UNKNOWN / NONE"));
  }

  delay(500);
}
