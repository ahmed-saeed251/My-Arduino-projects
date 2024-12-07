// Define motor driver pins
const int IN1 = 7;        // Connected to IN1 of L298
const int IN2 = 8;        // Connected to IN2 of L298
const int ENA = 6;        // Connected to ENA (Enable A, for PWM speed control)
const int limitSwitch1 = 5; // Limit switch 1 connected to pin 5
const int limitSwitch2 = 4; // Limit switch 2 connected to pin 4
const int button1 = 2;      // Push button 1 connected to pin 2
const int button2 = 10;      // Push button 2 connected to pin 3

// Motor speed (constant speed for the entire period)
const int motorSpeed = 50;

void setup() {
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Set limit switch and button pins as inputs with pull-up resistors
  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Motor control with limit switches and push buttons");
}

void loop() {
  // Check button 1 state
  if (digitalRead(button1) == LOW) { // Button 1 pressed
    Serial.println("Button 1 pressed: Moving forward...");
    moveForwardUntilLimitSwitch1();
  }

  // Check button 2 state
  if (digitalRead(button2) == LOW) { // Button 2 pressed
    Serial.println("Button 2 pressed: Moving backward...");
    moveBackwardUntilLimitSwitch2();
  }
}

// Function to move forward until limit switch 1 is triggered
void moveForwardUntilLimitSwitch1() {
  while (digitalRead(limitSwitch1) == HIGH) { // Keep moving forward until limit switch 1 is triggered
    moveMotorForward();
  }
  stopMotor();
  Serial.println("Limit switch 1 triggered! Stopping motor.");
}

// Function to move backward until limit switch 2 is triggered
void moveBackwardUntilLimitSwitch2() {
  while (digitalRead(limitSwitch2) == HIGH) { // Keep moving backward until limit switch 2 is triggered
    moveMotorBackward();
  }
  stopMotor();
  Serial.println("Limit switch 2 triggered! Stopping motor.");
}

// Function to move the motor forward
void moveMotorForward() {
  digitalWrite(IN1, HIGH);        // Set IN1 HIGH
  digitalWrite(IN2, LOW);         // Set IN2 LOW
  analogWrite(ENA, motorSpeed);   // Set constant speed via PWM on ENA
}

// Function to move the motor backward
void moveMotorBackward() {
  digitalWrite(IN1, LOW);         // Set IN1 LOW
  digitalWrite(IN2, HIGH);        // Set IN2 HIGH
  analogWrite(ENA, motorSpeed);   // Set constant speed via PWM on ENA
}

// Function to stop the motor
void stopMotor() {
  digitalWrite(IN1, LOW);  // Set IN1 LOW
  digitalWrite(IN2, LOW);  // Set IN2 LOW
  analogWrite(ENA, 0);     // Set speed to 0
}
