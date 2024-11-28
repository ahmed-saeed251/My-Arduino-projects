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

void setup() {
  stepperName.setSpeed(15);
  Serial.begin(9600);
  Serial.println("Elevator System Ready");
}

void loop() {
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
}

// Function to move the elevator to a specific floor
void moveToFloor(int targetFloor) {
  if (targetFloor != currentFloor) {
    int stepsToMove = floorSteps[targetFloor] - floorSteps[currentFloor]; // Calculate steps needed
    stepperName.step(-stepsToMove); // Move the stepper motor
    currentFloor = targetFloor;   // Update the current floor
    Serial.print("Moved to Floor: ");
    Serial.println(targetFloor);
  } else {
    Serial.println("Already on the selected floor.");
  }
}
