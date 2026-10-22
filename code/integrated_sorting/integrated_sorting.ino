/*
 * Integrated Color Sorting Pick and Place System
 * Robotic Arm Pick and Place Project
 *
 * Channel mapping (calibrated):
 *   CH_BASE      0   range 200-500, center 375
 *   CH_SHOULDER  1   range 200-340, center 260 (vertical)
 *   CH_ELBOW     2   range 200-420, center 300 (vertical)
 *   CH_WRIST     6   held at 375 (neutral)
 *   CH_GRIPPER   7   open 120, closed 200
 *
 * FSM:
 * 1. STATE_IDLE      : debounced ultrasonic object detection
 * 2. STATE_COLOR_SCAN: TCS3200 classification (Red / Green / Blue)
 * 3. STATE_PICK      : reach, grip, lift upright
 * 4. STATE_PLACE     : fold upright -> rotate base to bin -> lower -> release
 * 5. STATE_RETURN_HOME
 *
 * Safety: base only rotates while the arm is folded upright so the heavy
 * metal arm's center of mass stays over the base (prevents tipping).
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Hardware Pins
#define TRIG_PIN 9
#define ECHO_PIN 10
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define COLOR_OUT 8

// Servo Channels
#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      6
#define CH_GRIPPER    7

// Per-joint safe pulse limits (calibrated)
#define BASE_MIN      200
#define BASE_MAX      500
#define SHOULDER_MIN  200
#define SHOULDER_MAX  340
#define ELBOW_MIN     200
#define ELBOW_MAX     420
#define WRIST_MIN     300
#define WRIST_MAX     450

#define GRIPPER_OPEN   120
#define GRIPPER_CLOSED 200

// Poses
#define P_HOME_BASE      375
#define P_HOME_SHOULDER  260
#define P_HOME_ELBOW     300
#define P_HOME_WRIST     375

#define P_REACH_BASE     300
#define P_REACH_SHOULDER 310
#define P_REACH_ELBOW    250

#define BIN_RED          250
#define BIN_GREEN        375
#define BIN_BLUE         475

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
const uint8_t DETECT_DEBOUNCE = 3;

int clampPulse(int ch, int pulse) {
  if (ch == CH_BASE)     return constrain(pulse, BASE_MIN, BASE_MAX);
  if (ch == CH_SHOULDER) return constrain(pulse, SHOULDER_MIN, SHOULDER_MAX);
  if (ch == CH_ELBOW)    return constrain(pulse, ELBOW_MIN, ELBOW_MAX);
  if (ch == CH_WRIST)    return constrain(pulse, WRIST_MIN, WRIST_MAX);
  return pulse;
}

void setJoints(int b, int s, int e, int w, int g) {
  pwm.setPWM(CH_BASE, 0, clampPulse(CH_BASE, b));     delay(15);
  pwm.setPWM(CH_SHOULDER, 0, clampPulse(CH_SHOULDER, s)); delay(15);
  pwm.setPWM(CH_ELBOW, 0, clampPulse(CH_ELBOW, e));   delay(15);
  pwm.setPWM(CH_WRIST, 0, clampPulse(CH_WRIST, w));   delay(15);
  pwm.setPWM(CH_GRIPPER, 0, g);                       delay(120);
}

void goUpright(int grip) {
  setJoints(P_HOME_BASE, P_HOME_SHOULDER, P_HOME_ELBOW, P_HOME_WRIST, grip);
  delay(600);
}

void rotateBaseSafe(int targetBase, int grip) {
  goUpright(grip);                    // fold upright first
  pwm.setPWM(CH_BASE, 0, clampPulse(CH_BASE, targetBase));
  delay(900);                         // rotate while balanced
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long d = pulseIn(ECHO_PIN, HIGH, 30000);
  return (d == 0) ? 999.0 : (d * 0.0343) / 2.0;
}

ObjectColor scanColor() {
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  delay(10);
  int r = pulseIn(COLOR_OUT, LOW, 40000);

  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  delay(10);
  int g = pulseIn(COLOR_OUT, LOW, 40000);

  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  delay(10);
  int b = pulseIn(COLOR_OUT, LOW, 40000);

  Serial.print(F("Scan -> R:")); Serial.print(r);
  Serial.print(F(" G:")); Serial.print(g);
  Serial.print(F(" B:")); Serial.println(b);

  if (r < g && r < b && r < 200) return COLOR_RED;
  if (g < r && g < b && g < 250) return COLOR_GREEN;
  if (b < r && b < g && b < 250) return COLOR_BLUE;
  return COLOR_UNKNOWN;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  goUpright(GRIPPER_OPEN);
  Serial.println(F("System Ready. Waiting for objects..."));
}

void loop() {
  switch (currentState) {
    case STATE_IDLE: {
      float dist = readDistance();
      if (dist > 2.0 && dist <= 10.0) {
        detectCount++;
        if (detectCount >= DETECT_DEBOUNCE) {
          Serial.println(F("Object detected in zone! Initiating scan."));
          delay(500);
          detectCount = 0;
          currentState = STATE_COLOR_SCAN;
        }
      } else {
        detectCount = 0;
      }
      break;
    }

    case STATE_COLOR_SCAN: {
      detectedColor = scanColor();
      if (detectedColor != COLOR_UNKNOWN) {
        const char* name = "UNKNOWN";
        if (detectedColor == COLOR_RED)   name = "RED";
        if (detectedColor == COLOR_GREEN) name = "GREEN";
        if (detectedColor == COLOR_BLUE)  name = "BLUE";
        Serial.print(F("Object Recognized. Color: "));
        Serial.println(name);
        currentState = STATE_PICK;
      } else {
        Serial.println(F("Color unrecognized, retrying..."));
        delay(300);
      }
      break;
    }

    case STATE_PICK: {
      Serial.println(F("Executing Pick..."));
      setJoints(P_REACH_BASE, P_REACH_SHOULDER, P_REACH_ELBOW, P_HOME_WRIST, GRIPPER_OPEN);
      delay(700);
      setJoints(P_REACH_BASE, P_REACH_SHOULDER, P_REACH_ELBOW, P_HOME_WRIST, GRIPPER_CLOSED);
      delay(700);
      goUpright(GRIPPER_CLOSED);           // lift upright (balanced)
      currentState = STATE_PLACE;
      break;
    }

    case STATE_PLACE: {
      Serial.println(F("Executing Place to Target Bin..."));
      int targetBase = BIN_GREEN;
      if (detectedColor == COLOR_RED)   targetBase = BIN_RED;
      if (detectedColor == COLOR_GREEN) targetBase = BIN_GREEN;
      if (detectedColor == COLOR_BLUE)  targetBase = BIN_BLUE;

      rotateBaseSafe(targetBase, GRIPPER_CLOSED);   // rotate compact only
      setJoints(targetBase, P_REACH_SHOULDER, P_REACH_ELBOW, P_HOME_WRIST, GRIPPER_CLOSED);
      delay(700);
      setJoints(targetBase, P_REACH_SHOULDER, P_REACH_ELBOW, P_HOME_WRIST, GRIPPER_OPEN);
      delay(700);
      goUpright(GRIPPER_OPEN);
      currentState = STATE_RETURN_HOME;
      break;
    }

    case STATE_RETURN_HOME: {
      Serial.println(F("Returning to Home Position..."));
      rotateBaseSafe(P_HOME_BASE, GRIPPER_OPEN);
      delay(800);
      currentState = STATE_IDLE;
      break;
    }
  }
}
