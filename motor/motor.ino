#define ENA 22   // PWM-capable pin to enable motor
#define IN1 21   // Direction control pin 1
#define IN2 19   // Direction control pin 2

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    // Start with the motor off
    digitalWrite(ENA, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    Serial.begin(115200);
    Serial.println("L298N Motor Control Initialized");
}

void loop() {
    Serial.println("Running motor forward");
    moveMotor(true);  // Run motor forward
    delay(5000);      // Run for 3 seconds

    Serial.println("Running motor backward");
    moveMotor(false); // Run motor backward
    delay(5000);      // Run for 3 seconds
}

// Function to move motor
void moveMotor(bool forward) {
    digitalWrite(ENA, HIGH); // Enable motor
    if (forward) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
}