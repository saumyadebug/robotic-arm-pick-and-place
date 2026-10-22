/*
 * TCS3200 / TCS230 Color Sensor Test
 * Robotic Arm Pick and Place System
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
 * Place the object 1.5-3.0 cm below the sensor, in shade.
 * Lower pulse width = brighter channel.
 */

#define TCS_S0    4
#define TCS_S1    5
#define TCS_S2    6
#define TCS_S3    7
#define TCS_OUT   8

const unsigned int DARK_LIMIT = 400;   // above this = no useful object
const unsigned int MARGIN_PCT = 85;    // dominant channel must be 15% stronger

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

  Serial.println(F("TCS3200 test ready. Show RED / GREEN / BLUE objects."));
}

unsigned int readChannel(bool s2, bool s3) {
  digitalWrite(TCS_S2, s2);
  digitalWrite(TCS_S3, s3);
  delay(8);

  // Average 3 readings to reduce flicker/noise
  unsigned long total = 0;
  for (uint8_t i = 0; i < 3; i++) {
    unsigned long v = pulseIn(TCS_OUT, LOW, 50000UL);
    if (v == 0) v = 50000;
    total += v;
    delay(3);
  }
  return (unsigned int)(total / 3);
}

void loop() {
  unsigned int r = readChannel(LOW,  LOW);   // Red filter
  unsigned int g = readChannel(HIGH, HIGH);  // Green filter
  unsigned int b = readChannel(LOW,  HIGH);  // Blue filter
  unsigned int c = readChannel(HIGH, LOW);   // Clear / no filter

  Serial.print(F("R: ")); Serial.print(r);
  Serial.print(F(" | G: ")); Serial.print(g);
  Serial.print(F(" | B: ")); Serial.print(b);
  Serial.print(F(" | C: ")); Serial.print(c);

  unsigned int minVal = min(r, min(g, b));

  if (minVal == 0 || minVal > DARK_LIMIT) {
    Serial.println(F(" => UNKNOWN / NONE"));
  } else if (r == minVal &&
             (unsigned long)r * 100UL < (unsigned long)g * MARGIN_PCT &&
             (unsigned long)r * 100UL < (unsigned long)b * MARGIN_PCT) {
    Serial.println(F(" => RED"));
  } else if (g == minVal &&
             (unsigned long)g * 100UL < (unsigned long)r * MARGIN_PCT &&
             (unsigned long)g * 100UL < (unsigned long)b * MARGIN_PCT) {
    Serial.println(F(" => GREEN"));
  } else if (b == minVal &&
             (unsigned long)b * 100UL < (unsigned long)r * MARGIN_PCT &&
             (unsigned long)b * 100UL < (unsigned long)g * MARGIN_PCT) {
    Serial.println(F(" => BLUE"));
  } else {
    Serial.println(F(" => UNKNOWN / MIXED"));
  }

  delay(500);
}
