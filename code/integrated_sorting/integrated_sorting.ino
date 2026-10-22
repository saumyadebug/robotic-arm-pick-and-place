/*
 * Integrated Color Sorting Pick and Place System
 * Robotic Arm Pick and Place Project
 *
 * Description:
 * Complete autonomous finite-state machine (FSM):
 * 1. STATE_IDLE: Wait for object detection via ultrasonic sensor (debounced).
 * 2. STATE_COLOR_SCAN: Read TCS3200 color sensor and classify item (R/G/B/Y).
 * 3. STATE_PICK: Navigate arm to pick zone, grip object, and lift.
 * 4. STATE_PLACE: Route arm to assigned color bin (Red, Green, Blue, Yellow) and drop.
 * 5. STATE_RETURN_HOME: Reset arm to standby position.
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

// Servo Channels (6-DOF: 6 joints + gripper on channel 4)
#define CH_BASE        0
#define CH_SHOULDER    1
#define CH_ELBOW       2
#define CH_WRIST_PITCH 3
#define CH_GRIPPER     4
#define CH_WRIST_ROLL  5

#define GRIPPER_OPEN   220
#define GRIPPER_GRIP   410

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
  COLOR_BLUE,
  COLOR_YELLOW
};

SystemState currentState = STATE_IDLE;
ObjectColor detectedColor = COLOR_UNKNOWN;

// Debounce counter for ultrasonic detection
uint8_t detectCount = 0;
const uint8_t DETECT_DEBOUNCE = 3;

void setJoints(int b, int s, int e, int w, int wr, int g) {
  pwm.setPWM(CH_BASE, 0, b);
  pwm.setPWM(CH_SHOULDER, 0, s);
  pwm.setPWM(CH_ELBOW, 0, e);
  pwm.setPWM(CH_WRIST_PITCH, 0, w);
  pwm.setPWM(CH_WRIST_ROLL, 0, wr);
  pwm.setPWM(CH_GRIPPER, 0, g);
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
  // Red filter
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  delay(10);
  int r = pulseIn(COLOR_OUT, LOW, 40000);

  // Green filter
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  delay(10);
  int g = pulseIn(COLOR_OUT, LOW, 40000);

  // Blue filter
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  delay(10);
  int b = pulseIn(COLOR_OUT, LOW, 40000);

  Serial.print(F("Scan -> R:")); Serial.print(r);
  Serial.print(F(" G:")); Serial.print(g);
  Serial.print(F(" B:")); Serial.println(b);

  // Lower pulse duration = higher intensity
  bool redStrong   = (r < 200);
  bool greenStrong = (g < 200);
  bool blueStrong  = (b < 200);

  if (redStrong && !greenStrong && !blueStrong) return COLOR_RED;
  if (greenStrong && !redStrong && !blueStrong) return COLOR_GREEN;
  if (blueStrong && !redStrong && !greenStrong) return COLOR_BLUE;
  if (redStrong && greenStrong && !blueStrong) return COLOR_YELLOW;
  // Fallback: dominant component
  if (r < g && r < b) return COLOR_RED;
  if (g < r && g < b) return COLOR_GREEN;
  if (b < r && b < g) return COLOR_BLUE;
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

  // Home position (6 joints + gripper)
  setJoints(375, 300, 350, 300, 375, GRIPPER_OPEN);
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
          delay(500); // Settle time
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
        if (detectedColor == COLOR_RED)    name = "RED";
        if (detectedColor == COLOR_GREEN)  name = "GREEN";
        if (detectedColor == COLOR_BLUE)   name = "BLUE";
        if (detectedColor == COLOR_YELLOW) name = "YELLOW";
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
      setJoints(300, 420, 280, 250, 375, GRIPPER_OPEN);
      delay(1000);
      setJoints(300, 420, 280, 250, 375, GRIPPER_GRIP);
      delay(800);
      setJoints(300, 320, 350, 300, 375, GRIPPER_GRIP); // Lift
      delay(800);
      currentState = STATE_PLACE;
      break;
    }

    case STATE_PLACE: {
      Serial.println(F("Executing Place to Target Bin..."));
      int targetBase = 375;
      if (detectedColor == COLOR_RED)     targetBase = 200; // Bin 1
      if (detectedColor == COLOR_GREEN)   targetBase = 375; // Bin 2
      if (detectedColor == COLOR_BLUE)    targetBase = 550; // Bin 3
      if (detectedColor == COLOR_YELLOW)  targetBase = 590; // Bin 4

      setJoints(targetBase, 320, 350, 300, 375, GRIPPER_GRIP);
      delay(1000);
      setJoints(targetBase, 420, 280, 250, 375, GRIPPER_GRIP);
      delay(800);
      setJoints(targetBase, 420, 280, 250, 375, GRIPPER_OPEN); // Release
      delay(800);
      currentState = STATE_RETURN_HOME;
      break;
    }

    case STATE_RETURN_HOME: {
      Serial.println(F("Returning to Home Position..."));
      setJoints(375, 300, 350, 300, 375, GRIPPER_OPEN);
      delay(1500);
      currentState = STATE_IDLE;
      break;
    }
  }
}