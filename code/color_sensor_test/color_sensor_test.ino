/*
 * TCS3200 / TCS230 Color Sensor Test - WHITE-BALANCED
 *
 * WIRING (Arduino Uno):
 *   TCS3200 VCC  -> 5V
 *   TCS3200 GND  -> GND
 *   TCS3200 S0   -> D4
 *   TCS3200 S1   -> D5
 *   TCS3200 S2   -> D6
 *   TCS3200 S3   -> D7
 *   TCS3200 OUT  -> D8
 *   TCS3200 OE   -> GND (if the module exposes OE)
 *
 * At startup the sketch samples the empty background for 3 seconds.
 * Keep all objects away during that time. It then compares each colour
 * channel against that reference, so it works at different distances and
 * light levels without hard-coded red/green/blue thresholds.
 */

#define TCS_S0    4
#define TCS_S1    5
#define TCS_S2    6
#define TCS_S3    7
#define TCS_OUT   8

unsigned int refR = 184;
unsigned int refG = 299;
unsigned int refB = 318;

unsigned int readChannel(bool s2, bool s3) {
  digitalWrite(TCS_S2, s2);
  digitalWrite(TCS_S3, s3);
  delay(8);

  unsigned int samples[9];
  for (uint8_t i = 0; i < 9; i++) {
    unsigned long v = pulseIn(TCS_OUT, LOW, 50000UL);
    if (v == 0) return 0;       // timeout
    samples[i] = (unsigned int)v;
    delay(2);
  }

  // Median of 9 rejects specular glints and hand-motion spikes.
  for (uint8_t i = 1; i < 9; i++) {
    unsigned int key = samples[i];
    int j = i - 1;
    while (j >= 0 && samples[j] > key) {
      samples[j + 1] = samples[j];
      j--;
    }
    samples[j + 1] = key;
  }
  return samples[4];
}

void setup() {
  Serial.begin(115200);

  pinMode(TCS_S0, OUTPUT);
  pinMode(TCS_S1, OUTPUT);
  pinMode(TCS_S2, OUTPUT);
  pinMode(TCS_S3, OUTPUT);
  pinMode(TCS_OUT, INPUT);

  // Frequency scaling 20%
  digitalWrite(TCS_S0, HIGH);
  digitalWrite(TCS_S1, LOW);

  Serial.println(F("TCS3200 white-balance test"));
  Serial.println(F("Keep the sensor area EMPTY for 3 seconds..."));
  delay(3000);

  refR = readChannel(LOW,  LOW);
  refG = readChannel(HIGH, HIGH);
  refB = readChannel(LOW,  HIGH);

  Serial.print(F("Reference -> R:"));
  Serial.print(refR);
  Serial.print(F(" G:"));
  Serial.print(refG);
  Serial.print(F(" B:"));
  Serial.println(refB);
  Serial.println(F("Now show RED / GREEN / BLUE objects 1.5-3 cm below the sensor."));
}

void loop() {
  unsigned int r = readChannel(LOW,  LOW);   // Red filter
  unsigned int g = readChannel(HIGH, HIGH);  // Green filter
  unsigned int b = readChannel(LOW,  HIGH);  // Blue filter

  Serial.print(F("R: ")); Serial.print(r);
  Serial.print(F(" | G: ")); Serial.print(g);
  Serial.print(F(" | B: ")); Serial.print(b);

  if (r == 0 || g == 0 || b == 0) {
    Serial.println(F(" => UNKNOWN (sensor timeout)"));
    delay(500);
    return;
  }

  // Normalise against the empty-background reference.
  float nr = (float)refR / (float)r;
  float ng = (float)refG / (float)g;
  float nb = (float)refB / (float)b;

  Serial.print(F(" | norm R:"));
  Serial.print(nr, 2);
  Serial.print(F(" G:"));
  Serial.print(ng, 2);
  Serial.print(F(" B:"));
  Serial.print(nb, 2);

  float maxv = max(nr, max(ng, nb));
  float minv = min(nr, min(ng, nb));

  if ((maxv - minv) < 0.04) {
    Serial.println(F(" => UNKNOWN / NEUTRAL"));
  } else if (nr >= ng && nr >= nb && nr > ng * 1.01 && nr > nb * 1.01) {
    Serial.println(F(" => RED"));
  } else if (ng >= nr && ng >= nb && ng > nr * 1.01 && ng > nb * 1.01) {
    Serial.println(F(" => GREEN"));
  } else if (nb >= nr && nb >= ng && nb > nr * 1.01 && nb > ng * 1.01) {
    Serial.println(F(" => BLUE"));
  } else {
    Serial.println(F(" => UNKNOWN / MIXED"));
  }

  delay(500);
}
