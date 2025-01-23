#include <ESP32Servo.h>

// Pin definitions
int RPWM = 22;
int LPWM = 23;
int L_EN = 21;
int R_EN = 19;
int M1 = 18;
int M2 = 5;
int M3 = 4;
int servoPin = 12; // Servo signal pin (change as needed)

// Servo parameters
Servo myServo; // Servo object
int servoNeutral = 90; // Neutral position (0 degrees = 90, 180 = fully clockwise)
int servoRight = 180; // Full right position
int servoLeft = 0;    // Full left position

// Braking parameters
int currentSpeed = 0; // Tracks current motor speed
const int brakeStep = 30; // Speed decrement step for braking
const int brakeDelay = 5; // Delay between steps in milliseconds

void setup() {
  Serial.begin(115200);

  // Configure motor control pins
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);

  // Initialize motors and components OFF
  digitalWrite(RPWM, LOW);
  digitalWrite(LPWM, LOW);
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  digitalWrite(M3, HIGH);

  // Attach the servo
  myServo.attach(servoPin, 500, 2500); // Attach the servo with default min/max pulse width (500µs to 2500µs)
  myServo.write(servoNeutral); // Start at neutral position
}

void loop() {
  // Check if data is available on Serial
  if (Serial.available() > 0) {
    // Read the joystick data
    String data = Serial.readStringUntil('\n'); // Read till newline character
    data.trim(); // Remove any extra whitespace

    // Parse the joystick values
    float axis_1 = 0; // Now using axis_0 for servo
    int button_0 = 0, button_1 = 0;
    int right = 0, left = 0, neutral = 0;
    sscanf(data.c_str(), "%f,%d,%d,%d,%d,%d", &axis_1, &button_0, &button_1, &right, &left, &neutral);

    // Control the servo based on button presses
    if (right == 1) {
      // Get current servo position
      int currentPos = myServo.read();
      
      // Sweep from current position to full right
      for (int pos = currentPos; pos <= servoRight; pos++) { 
        myServo.write(pos);  // Move to position
        delay(15); // Short delay for smooth movement
      }
    }
    else if (left == 1) {
      // Get current servo position
      int currentPos = myServo.read();
      
      // Sweep from current position to full left
      for (int pos = currentPos; pos >= servoLeft; pos--) { 
        myServo.write(pos);  // Move to position
        delay(15); // Short delay for smooth movement
      }
    }
    else if (neutral == 1) {
      myServo.write(servoNeutral); // Return servo to neutral position
    }

    // Motor control with axis_1 and braking
    if (button_1 == 1) {
      gradualBrake();
    } else {
      int targetSpeed = map(abs(axis_1) * 100, -100, 100, 0, 255);

      if (axis_1 > -1) { // Forward
        digitalWrite(R_EN, HIGH);
        digitalWrite(L_EN, HIGH);
        analogWrite(RPWM, targetSpeed);
        currentSpeed = targetSpeed;
      } else { // At -1, turn off the motor
        digitalWrite(R_EN, LOW);
        digitalWrite(L_EN, LOW);
        analogWrite(RPWM, 0);
        currentSpeed = 0;
      }
    }

    // Handle button_0 to toggle M1, M2, and M3
    static bool mState = HIGH;
    mState = (button_0 == 1) ? LOW : HIGH;
    digitalWrite(M1, mState);
    digitalWrite(M2, mState);
    digitalWrite(M3, mState);

    // Debugging: Print received values
    Serial.print("Right: ");
    Serial.print(right);
    Serial.print(", Left: ");
    Serial.print(left);
    Serial.print(", Neutral: ");
    Serial.println(neutral);
  }
}

// Function to gradually decrease speed in reverse direction
void gradualBrake() {
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  while (currentSpeed > 0) {
    currentSpeed -= brakeStep;
    if (currentSpeed < 0) currentSpeed = 0;
    analogWrite(RPWM, currentSpeed);
    delay(brakeDelay);
  }

  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  analogWrite(RPWM, 0);
}