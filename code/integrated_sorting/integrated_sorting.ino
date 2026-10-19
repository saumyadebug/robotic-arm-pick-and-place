/*
 * Integrated Color Sorting Pick and Place System
 * Robotic Arm Pick and Place Project
 *
 * Description:
 * Complete autonomous finite-state machine (FSM):
 * 1. STATE_IDLE: Wait for object detection via ultrasonic sensor.
 * 2. STATE_COLOR_SCAN: Read TCS3200 color sensor and classify item.
 * 3. STATE_PICK: Navigate arm to pick zone, grip object, and lift.
 * 4. STATE_PLACE: Route arm to assigned color bin (Red, Green, Blue) and drop.
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

// Servo Channels
#define CH_BASE       0
#define CH_SHOULDER   1
#define CH_ELBOW      2
#define CH_WRIST      3
#define CH_GRIPPER    4

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
  COLOR_BLUE
};

SystemState currentState = STATE_IDLE;
ObjectColor detectedColor = COLOR_UNKNOWN;

void setJoints(int b, int s, int e, int w, int g) {
  pwm.setPWM(CH_BASE, 0, b);
  pwm.setPWM(CH_SHOULDER, 0, s);
  pwm.setPWM(CH_ELBOW, 0, e);
  pwm.setPWM(CH_WRIST, 0, w);
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
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  int r = pulseIn(COLOR_OUT, LOW, 40000);

  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  int g = pulseIn(COLOR_OUT, LOW, 40000);

  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  int b = pulseIn(COLOR_OUT, LOW, 40000);

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
  pwm.setPWMFreq(50);

  // Home position
  setJoints(375, 300, 350, 300, GRIPPER_OPEN);
  Serial.println(F("System Ready. Waiting for objects..."));
}

void loop() {
  switch (currentState) {
    case STATE_IDLE: {
      float dist = readDistance();
      if (dist > 2.0 && dist <= 8.0) {
        Serial.println(F("Object detected in zone! Initiating scan."));
        delay(500); // Settle time
        currentState = STATE_COLOR_SCAN;
      }
      break;
    }

    case STATE_COLOR_SCAN: {
      detectedColor = scanColor();
      if (detectedColor != COLOR_UNKNOWN) {
        Serial.print(F("Object Recognized. Color: "));
        Serial.println(detectedColor == COLOR_RED ? "RED" : (detectedColor == COLOR_GREEN ? "GREEN" : "BLUE"));
        currentState = STATE_PICK;
      } else {
        Serial.println(F("Color unrecognized, retrying..."));
        delay(300);
      }
      break;
    }

    case STATE_PICK: {
      Serial.println(F("Executing Pick..."));
      setJoints(300, 420, 280, 250, GRIPPER_OPEN);
      delay(1000);
      setJoints(300, 420, 280, 250, GRIPPER_GRIP);
      delay(800);
      setJoints(300, 320, 350, 300, GRIPPER_GRIP); // Lift
      delay(800);
      currentState = STATE_PLACE;
      break;
    }

    case STATE_PLACE: {
      Serial.println(F("Executing Place to Target Bin..."));
      int targetBase = 375;
      if (detectedColor == COLOR_RED)   targetBase = 200; // Bin 1
      if (detectedColor == COLOR_GREEN) targetBase = 375; // Bin 2
      if (detectedColor == COLOR_BLUE)  targetBase = 550; // Bin 3

      setJoints(targetBase, 320, 350, 300, GRIPPER_GRIP);
      delay(1000);
      setJoints(targetBase, 420, 280, 250, GRIPPER_GRIP);
      delay(800);
      setJoints(targetBase, 420, 280, 250, GRIPPER_OPEN); // Release
      delay(800);
      currentState = STATE_RETURN_HOME;
      break;
    }

    case STATE_RETURN_HOME: {
      Serial.println(F("Returning to Home Position..."));
      setJoints(375, 300, 350, 300, GRIPPER_OPEN);
      delay(1500);
      currentState = STATE_IDLE;
      break;
    }
  }
}
