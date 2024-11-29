#include <Stepper.h>
#include <Keypad.h>

// Stepper Motor Configuration
const int stepsPerRevolution = 2048;
Stepper stepperName(stepsPerRevolution, 9, 11, 10, 12);
const float stepsFactor = 1.07; // Multiplier for step adjustments

// Keypad Configuration
const byte ROWS = 4; // Number of rows
const byte COLS = 3; // Number of columns

// Define the keypad layout
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Connect keypad rows and columns to Arduino pins
byte rowPins[ROWS] = {5, 4, 3, 2}; // Adjusted row pins
byte colPins[COLS] = {7, 8, 6};    // Adjusted column pins

// Create a Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Floor positions (in steps from the ground floor)
int floorSteps[] = {0, (int)(stepsPerRevolution * 1.07), (int)(stepsPerRevolution * 1.07 * 2), (int)(stepsPerRevolution * 1.07 * 3)};
int currentFloor = 0; // Start at the ground floor

// External button pins
const int buttonPins[] = {A0, A1, A2, A3, A4, A5}; // Pins for external buttons (6 total)

// Flags for button debounce
bool buttonPressed[6] = {false, false, false, false, false, false};

// Button actions (up or down)
const char *buttonActions[] = {
    "First Floor Up",
    "Second Floor Up",
    "Second Floor Down",
    "Third Floor Up",
    "Third Floor Down",
    "Fourth Floor Down"
};

// LED pins
const int groundFloorLed = 0;   // RX
const int firstFloorLed = 1;     // TX
const int secondFloorLed = 13;   // Pin D13 for second floor LED
const int thirdFloorLed = 13;    // Pin D13 for third floor LED

void setup() {
  stepperName.setSpeed(19);

  // Set LED pins as OUTPUT
  pinMode(groundFloorLed, OUTPUT);
  pinMode(firstFloorLed, OUTPUT);
  pinMode(secondFloorLed, OUTPUT); // Set D13 for second floor LED
  pinMode(thirdFloorLed, OUTPUT);  // Set D13 for third floor LED

  // Set button pins as INPUT_PULLUP
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Check keypad for floor selection
  char key = keypad.getKey(); // Check if a key is pressed
  if (key) {
    // Convert the pressed key to the corresponding floor number
    if (key >= '0' && key <= '3') { // Check if the key is a valid floor number (0–3)
      int targetFloor = key - '0'; // Converts '0'-'3' to integers 0–3
      moveToFloor(targetFloor);
    }
  }

  // Check external push buttons for calling the elevator
  for (int i = 0; i < 6; i++) {
    int buttonState = digitalRead(buttonPins[i]);

    if (buttonState == LOW && !buttonPressed[i]) { // Button pressed and not already registered
      buttonPressed[i] = true;           // Set flag to true

      // Determine the target floor based on the button press
      int targetFloor = -1;  // Default to an invalid floor
      
      if (i == 0) targetFloor = 0;  // First floor up
      else if (i == 1) targetFloor = 1;  // Second floor up
      else if (i == 2) targetFloor = 1;  // Second floor down
      else if (i == 3) targetFloor = 2;  // Third floor up
      else if (i == 4) targetFloor = 2;  // Third floor down
      else if (i == 5) targetFloor = 3;  // Fourth floor down

      // Light the corresponding LED as soon as the button is pressed
      updateLeds(targetFloor);
      
      // Move elevator only if the target floor is different from the current floor
      if (targetFloor != currentFloor) {
        moveToFloor(targetFloor);          // Move elevator to the target floor
      }
    } 
    else if (buttonState == HIGH && buttonPressed[i]) { // Button released
      buttonPressed[i] = false;          // Reset flag
    }
  }
}

// Function to move the elevator to a specific floor 
void moveToFloor(int targetFloor) {
  if (targetFloor != currentFloor) {
    int stepsToMove = floorSteps[targetFloor] - floorSteps[currentFloor]; // Calculate steps needed
    softStep(abs(stepsToMove), (stepsToMove > 0 ? -1 : 1)); // Add soft start/stop
    currentFloor = targetFloor;    // Update the current floor
    updateLeds(currentFloor);      // Turn off the LED for the current floor
  }
}

// Function to implement soft start/stop
void softStep(int steps, int direction) {
  int maxSpeed = 19; // Maximum speed for the stepper
  int minSpeed = 5;  // Minimum speed for acceleration
  int rampSteps = steps / 4; // Ramp steps for acceleration/deceleration

  // Acceleration phase
  for (int i = 0; i < rampSteps; i++) {
    int speed = minSpeed + (maxSpeed - minSpeed) * i / rampSteps;
    stepperName.setSpeed(speed);
    stepperName.step(direction);
  }

  // Constant speed phase
  for (int i = rampSteps; i < steps - rampSteps; i++) {
    stepperName.setSpeed(maxSpeed);
    stepperName.step(direction);
  }

  // Deceleration phase
  for (int i = 0; i < rampSteps; i++) {
    int speed = maxSpeed - (maxSpeed - minSpeed) * i / rampSteps;
    stepperName.setSpeed(speed);
    stepperName.step(direction);
  }
}

// Function to update LEDs based on the target floor
void updateLeds(int targetFloor) {
  // Turn off all LEDs first
  digitalWrite(groundFloorLed, LOW);
  digitalWrite(firstFloorLed, LOW);
  digitalWrite(secondFloorLed, LOW);
  digitalWrite(thirdFloorLed, LOW);

  // Turn on the LED for the requested floor
  if (targetFloor == 0) {
    digitalWrite(groundFloorLed, HIGH);
  } else if (targetFloor == 1) {
    digitalWrite(firstFloorLed, HIGH);
  } else if (targetFloor == 2) {
    digitalWrite(secondFloorLed, HIGH); // Turn on second floor LED
  } else if (targetFloor == 3) {
    digitalWrite(thirdFloorLed, HIGH);  // Turn on third floor LED
  }
}
