void setup() {
  Serial.begin(115200); // Match this with Python's baud rate
  Serial.println("ESP Ready to receive joystick data!");
}

void loop() {
  if (Serial.available() > 0) {
    // Read incoming data
    String data = Serial.readStringUntil('\n');

    // Print the received data
    Serial.println("Received: " + data);

    // (Optional) Parse the data if needed
    float axis_0, axis_1;
    int button_0, button_1;
    sscanf(data.c_str(), "%f,%f,%d,%d", &axis_0, &axis_1, &button_0, &button_1);

    // Print parsed values
    Serial.print("Axis 0: ");
    Serial.print(axis_0);
    Serial.print(", Axis 1: ");
    Serial.print(axis_1);
    Serial.print(", Button 0: ");
    Serial.print(button_0);
    Serial.print(", Button 1: ");
    Serial.println(button_1);
  }
}