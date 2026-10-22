/*
 * Integrated Color Sorting Pick-and-Place
 * Robotic Arm Pick-and-Place Project
 *
 * HARDWARE (calibrated 2026-10-23)
 *   PCA9685 servo channels:
 *     CH_BASE      0   operational clamp 335-415, center 375
 *     CH_SHOULDER  1   operational clamp 250-275, center 260 (vertical)
 *     CH_ELBOW     2   operational clamp 205-310, center 300 (vertical)
 *     CH_WRIST     6   fixed at 375
 *     CH_GRIPPER   7   open 120 / closed 200
 *     CH 3 is faulty - never use it.
 *     CH 4 wrist-rotation servo is dead - not used.
 *
 *   Sensors (Arduino Uno):
 *     HC-SR04   VCC=5V  GND=GND  TRIG=D9  ECHO=D10
 *     TCS3200   VCC=5V  GND=GND  S0=D4 S1=D5 S2=D6 S3=D7 OUT=D8 OE=GND
 *
 * MOTION SAFETY
 *   The shoulder stays vertical; the elbow does the reach (300 -> 278 ->
 *   248 -> 218).  The base rotates ONLY after the arm is folded upright.
 *   Lifting is staged in the reverse order so the shoulder never has to
 *   lift the arm from a fully extended position.
 *
 * FSM
 *   IDLE -> COLOR_SCAN -> PICK -> PLACE -> RETURN_HOME -> IDLE
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// ---------------- Sensor pins ----------------
#define TRIG_PIN     9
#define ECHO_PIN     10

#define TCS_S0       4
#define TCS_S1       5
#define TCS_S2       6
#define TCS_S3       7
#define TCS_OUT      8

// ---------------- Servo channels ----------------
#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      6
#define CH_GRIPPER    7

// ---------------- Operational safety clamps ----------------
#define BASE_MIN      335
#define BASE_MAX      415
#define SHOULDER_MIN  250
#define SHOULDER_MAX  275
#define ELBOW_MIN     205
#define ELBOW_MAX     310
#define WRIST_MIN     370
#define WRIST_MAX     380

#define GRIPPER_OPEN   120
#define GRIPPER_CLOSED 200

// ---------------- Poses ----------------
#define HOME_BASE      375
#define HOME_SHOULDER  260
#define HOME_ELBOW     300
#define HOME_WRIST     375

#define REACH_SHOULDER 258
#define REACH_ELBOW    218

// Bins are deliberately close to the base centre while the arm is heavy.
#define BIN_RED        335
#define BIN_GREEN      375
#define BIN_BLUE       415

// ---------------- Object detection ----------------
const float OBJECT_MIN_CM = 2.0;
const float OBJECT_MAX_CM = 10.0;
const uint8_t DETECT_DEBOUNCE = 3;
const uint8_t COLOR_RETRIES = 3;

// TCS3200: lower pulse width = brighter channel.
const unsigned int COLOR_DARK_LIMIT = 400;  // above this = no useful object
const unsigned int COLOR_MARGIN_PCT = 85;   // dominant channel must be 15% stronger

enum SystemState {
  STATE_IDLE,
  STATE_COLOR_SCAN,
  STATE_PICK,
  STATE_PLACE,
  STATE_RETURN_HOME
};

enum ObjectColor {
  COLOR_UNKNOWN,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE
};

SystemState currentState = STATE_IDLE;
ObjectColor detectedColor = COLOR_UNKNOWN;

uint8_t detectCount = 0;
uint8_t colorRetries = 0;

// ---------------- Motion helpers ----------------
int clampPulse(uint8_t ch, int pulse) {
  if (ch == CH_BASE)     return constrain(pulse, BASE_MIN, BASE_MAX);
  if (ch == CH_SHOULDER) return constrain(pulse, SHOULDER_MIN, SHOULDER_MAX);
  if (ch == CH_ELBOW)    return constrain(pulse, ELBOW_MIN, ELBOW_MAX);
  if (ch == CH_WRIST)    return constrain(pulse, WRIST_MIN, WRIST_MAX);
  return pulse;
}

void setP(uint8_t ch, int pulse) {
  pwm.setPWM(ch, 0, clampPulse(ch, pulse));
}

// Lower the forearm in small steps; the heavy upper arm stays vertical.
void reachDown() {
  setP(CH_SHOULDER, REACH_SHOULDER); delay(400);
  setP(CH_ELBOW, 278);               delay(500);
  setP(CH_ELBOW, 248);               delay(500);
  setP(CH_ELBOW, REACH_ELBOW);       delay(800);
}

// Raise the forearm first, then bring the shoulder back to vertical.
void liftUpright(int grip) {
  setP(CH_ELBOW, 240);              delay(550);
  setP(CH_ELBOW, 262);              delay(550);
  setP(CH_ELBOW, 284);              delay(550);
  setP(CH_SHOULDER, HOME_SHOULDER); delay(550);
  setP(CH_ELBOW, HOME_ELBOW);       delay(650);
  setP(CH_WRIST, HOME_WRIST);       delay(200);
  pwm.setPWM(CH_GRIPPER, 0, grip);  delay(500);
}

// Fold upright before rotating the base.
void rotateBaseSlow(int targetBase, int grip) {
  liftUpright(grip);
  setP(CH_BASE, targetBase);
  delay(1000);
}

// ---------------- Sensor helpers ----------------
float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 999.0;
  return (duration * 0.0343) / 2.0;
}

unsigned int readColorChannel(bool s2, bool s3) {
  digitalWrite(TCS_S2, s2);
  digitalWrite(TCS_S3, s3);
  delay(8);
  return pulseIn(TCS_OUT, LOW, 50000UL);
}

ObjectColor scanColor() {
  // TCS3200 filter selection:
  //   S2=LOW,  S3=LOW  -> Red
  //   S2=HIGH, S3=HIGH -> Green
  //   S2=LOW,  S3=HIGH -> Blue
  //   S2=HIGH, S3=LOW  -> Clear (no filter)
  unsigned int r = readColorChannel(LOW,  LOW);
  unsigned int g = readColorChannel(HIGH, HIGH);
  unsigned int b = readColorChannel(LOW,  HIGH);
  unsigned int c = readColorChannel(HIGH, LOW);

  Serial.print(F("Scan -> R:"));
  Serial.print(r);
  Serial.print(F(" G:"));
  Serial.print(g);
  Serial.print(F(" B:"));
  Serial.print(b);
  Serial.print(F(" C:"));
  Serial.println(c);

  unsigned int minVal = min(r, min(g, b));
  if (minVal == 0 || minVal > COLOR_DARK_LIMIT) {
    return COLOR_UNKNOWN;  // timeout, black object, or too far away
  }

  if (r == minVal &&
      (unsigned long)r * 100UL < (unsigned long)g * COLOR_MARGIN_PCT &&
      (unsigned long)r * 100UL < (unsigned long)b * COLOR_MARGIN_PCT) {
    return COLOR_RED;
  }

  if (g == minVal &&
      (unsigned long)g * 100UL < (unsigned long)r * COLOR_MARGIN_PCT &&
      (unsigned long)g * 100UL < (unsigned long)b * COLOR_MARGIN_PCT) {
    return COLOR_GREEN;
  }

  if (b == minVal &&
      (unsigned long)b * 100UL < (unsigned long)r * COLOR_MARGIN_PCT &&
      (unsigned long)b * 100UL < (unsigned long)g * COLOR_MARGIN_PCT) {
    return COLOR_BLUE;
  }

  return COLOR_UNKNOWN;
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(TCS_S0, OUTPUT);
  pinMode(TCS_S1, OUTPUT);
  pinMode(TCS_S2, OUTPUT);
  pinMode(TCS_S3, OUTPUT);
  pinMode(TCS_OUT, INPUT);

  // TCS3200 frequency scaling: 20% (S0=HIGH, S1=LOW)
  digitalWrite(TCS_S0, HIGH);
  digitalWrite(TCS_S1, LOW);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println(F("SAFE MODE - recovering to upright, 6s"));
  setP(CH_ELBOW, 280);              delay(600);
  setP(CH_ELBOW, HOME_ELBOW);       delay(600);
  setP(CH_SHOULDER, HOME_SHOULDER); delay(600);
  setP(CH_WRIST, HOME_WRIST);       delay(300);
  setP(CH_BASE, HOME_BASE);         delay(600);
  pwm.setPWM(CH_GRIPPER, 0, GRIPPER_OPEN); delay(6000);

  Serial.println(F("System Ready. Place an object 2-10 cm from the ultrasonic sensor."));
}

// ---------------- Main FSM ----------------
void loop() {
  switch (currentState) {

    case STATE_IDLE: {
      float distance = readDistanceCM();

      if (distance > OBJECT_MIN_CM && distance <= OBJECT_MAX_CM) {
        detectCount++;
        if (detectCount >= DETECT_DEBOUNCE) {
          Serial.print(F("Object detected at "));
          Serial.print(distance);
          Serial.println(F(" cm. Scanning colour..."));
          detectCount = 0;
          colorRetries = 0;
          currentState = STATE_COLOR_SCAN;
        }
      } else {
        detectCount = 0;
      }

      delay(60);
      break;
    }

    case STATE_COLOR_SCAN: {
      detectedColor = scanColor();

      if (detectedColor != COLOR_UNKNOWN) {
        const char* name = "UNKNOWN";
        if (detectedColor == COLOR_RED)   name = "RED";
        if (detectedColor == COLOR_GREEN) name = "GREEN";
        if (detectedColor == COLOR_BLUE)  name = "BLUE";

        Serial.print(F("Colour recognised: "));
        Serial.println(name);
        currentState = STATE_PICK;
      } else {
        colorRetries++;
        Serial.print(F("Colour not recognised, attempt "));
        Serial.println(colorRetries);

        if (colorRetries >= COLOR_RETRIES) {
          Serial.println(F("Giving up - returning to IDLE."));
          currentState = STATE_IDLE;
        } else {
          delay(400);
        }
      }
      break;
    }

    case STATE_PICK: {
      Serial.println(F("PICK: reaching down (elbow only)..."));
      pwm.setPWM(CH_GRIPPER, 0, GRIPPER_OPEN);
      delay(300);
      reachDown();

      Serial.println(F("PICK: closing gripper..."));
      pwm.setPWM(CH_GRIPPER, 0, GRIPPER_CLOSED);
      delay(800);

      Serial.println(F("PICK: lifting upright..."));
      liftUpright(GRIPPER_CLOSED);

      currentState = STATE_PLACE;
      break;
    }

    case STATE_PLACE: {
      int targetBase = BIN_GREEN;
      if (detectedColor == COLOR_RED)   targetBase = BIN_RED;
      if (detectedColor == COLOR_GREEN) targetBase = BIN_GREEN;
      if (detectedColor == COLOR_BLUE)  targetBase = BIN_BLUE;

      Serial.print(F("PLACE: rotating upright to base "));
      Serial.println(targetBase);
      rotateBaseSlow(targetBase, GRIPPER_CLOSED);

      Serial.println(F("PLACE: reaching down over bin..."));
      reachDown();

      Serial.println(F("PLACE: releasing..."));
      pwm.setPWM(CH_GRIPPER, 0, GRIPPER_OPEN);
      delay(800);

      Serial.println(F("PLACE: lifting upright..."));
      liftUpright(GRIPPER_OPEN);

      currentState = STATE_RETURN_HOME;
      break;
    }

    case STATE_RETURN_HOME: {
      Serial.println(F("RETURN: rotating home..."));
      rotateBaseSlow(HOME_BASE, GRIPPER_OPEN);
      Serial.println(F("Cycle complete. Waiting for next object."));
      currentState = STATE_IDLE;
      break;
    }
  }
}
