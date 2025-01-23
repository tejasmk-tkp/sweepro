#include <ESP32Servo.h> // ESP32-compatible servo library

Servo myServo;     // Create a servo object

int servoPin = 12; // Change this to your GPIO pin

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin, 500, 2500); // Attach servo with min/max pulse widths
  Serial.println("Servo test starting...");
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 5) { // Sweep from 0 to 180 degrees
    myServo.write(pos);
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos -= 5) { // Sweep back to 0 degrees
    myServo.write(pos);
    delay(15);
  }
}