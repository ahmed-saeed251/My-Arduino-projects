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

void setup() {
  stepperName.setSpeed(15);
  Serial.begin(9600);
  Serial.println("Elevator System Ready");

  // Set button pins as INPUT_PULLUP
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Check keypad for floor selection
  char key = keypad.getKey(); // Check if a key is pressed
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    // Convert the pressed key to the corresponding floor number
    if (key >= '0' && key <= '3') { // Check if the key is a valid floor number (0–3)
      int targetFloor = key - '0'; // Converts '0'-'3' to integers 0–3
      moveToFloor(targetFloor);
    } else {
      Serial.println("Invalid Floor!");
    }
  }

  // Check external push buttons for calling the elevator
  for (int i = 0; i < 6; i++) {
    int buttonState = digitalRead(buttonPins[i]);

    if (buttonState == LOW && !buttonPressed[i]) { // Button pressed and not already registered
      Serial.println(buttonActions[i]);  // Print action
      buttonPressed[i] = true;           // Set flag to true

      // Determine the target floor based on the button press
      int targetFloor = -1;  // Default to an invalid floor
      
      if (i == 0) targetFloor = 0;  // First floor up
      else if (i == 1) targetFloor = 1;  // Second floor up
      else if (i == 2) targetFloor = 1;  // Second floor down
      else if (i == 3) targetFloor = 2;  // Third floor up
      else if (i == 4) targetFloor = 2;  // Third floor down
      else if (i == 5) targetFloor = 3;  // Fourth floor down

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
    stepperName.step(-stepsToMove); // Move the stepper motor
    currentFloor = targetFloor;    // Update the current floor
    Serial.print("Moved to Floor: ");
    Serial.println(targetFloor);
  } else {
    Serial.println("Already on the selected floor.");
  }
}
