//
//#include <Stepper.h>
//
////Set how many steps it takes to make a full revolution
////Divide the degrees per step by 360 to get the steps
//const int stepsPerRevolution = 2048;
//
////Use pin 8-11 to IN1-IN4
//
//Stepper stepperName = Stepper(stepsPerRevolution, 9, 11, 10, 12);
//
//void setup() {
//
//  //Set the RPM of the stepper motor
//  stepperName.setSpeed(15);
//
//}
//
//void loop() {
//
//    //This should make the stepper do a full 360 degrees
//    stepperName.step(-stepsPerRevolution);
//    
//}



//#include <Stepper.h>
//
////Define an input pin
//int buttonPressPin = 7;
//
////Define a Boolean to track a button press
//bool buttonPressed;
//
////Set how many steps it takes to make a full revolution. Divide the degrees per step by 360 to get the steps
//const int stepsPerRevolution = 2048;
//
////Use pin 8-11 on the arduino to IN1-IN4 on the stepper board
////Setting up the stepper
//Stepper stepperName = Stepper(stepsPerRevolution, 8, 10, 9, 11);
//
//void setup() {
//
//  //Set the RPM of the stepper motor
//  stepperName.setSpeed(15);
//
//  //Set the pinMode of our button pin
//  pinMode(buttonPressPin, INPUT_PULLUP);
//
//  //Set our button press Boolean to a known value
//  buttonPressed = true;
//}
//
//void loop() {
//    //Checking the state of the button pin and saving it in our Boolean
//    buttonPressed = digitalRead(buttonPressPin);
//    //If the button is pressed (grounded), step to 90* and reset the button pressing state
//    if (buttonPressed == false) {
//      stepperName.step(-stepsPerRevolution / 4);
//      delay(50);
//      buttonPressed = true;
//    }
//    
//}



/* This program has been written (poorly) by Dan of the Simple Electronics Youtube channel
 * (www.SimpleElectronics.ca) and is available for you to do whatever you want with it as
 * long as you don't hold me responsible. But if you want to send me money for my terrible
 * coding, feel free! (www.patreon.com/SimpleElectronics)
 */




//Include the Arduino stepper library
//#include <Stepper.h>
//
////Define some limit pins
//int firstLimit = 7;
//int secondLimit = 6;
//
////Define some Booleans to track the limit switch statuses
//bool e1State = true;
//bool e2State = true;
////Define a variable to flip around the rotation
//int revolutionVal = 100;
//
////Use pin 8-11 on the arduino to IN1-IN4 on the stepper board
////Setting up the stepper
//Stepper stepperName = Stepper(2048, 9, 10, 11, 12);
//
//void setup() {
//
//  //Set the RPM of the stepper motor
//  stepperName.setSpeed(15);
//
//  //Set the pinMode of our limit switches
//  pinMode(firstLimit, INPUT_PULLUP);
//  pinMode(secondLimit, INPUT_PULLUP);
//
//}
//
//void loop() {
//    //Checking the state of the button pin and saving it in our Booleans
//    e1State = digitalRead(firstLimit);
//    e2State = digitalRead(secondLimit);
//    //Make the stepper spin in one direction
//    stepperName.step(revolutionVal);
//    //If any limit switch is pressed (grounded), reverse the direction of the motor
//    if (e1State == false) {
//      revolutionVal = revolutionVal * -1;
//      stepperName.step(revolutionVal);
//      e1State = true;
//      delay(200);
//      
//    }
//    if (e2State == false) {
//      revolutionVal = revolutionVal * -1;
//      stepperName.step(revolutionVal);
//      e2State = true;
//      delay(200);
//    }
//}



 

#include <Stepper.h>

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevolution = 2048;

// Initialize the stepper motor with the appropriate pins
Stepper stepperName(stepsPerRevolution, 9, 11, 10, 12);

// Define the analog pins for the buttons
const int buttonForwardPin = A0;
const int buttonBackwardPin = A1;

void setup() {
  // Set the speed of the stepper motor in RPM
  stepperName.setSpeed(15);


}

void loop() {
  // Read the state of the buttons


  // Define a threshold to determine if the button is pressed
  const int threshold = 512; // You can adjust this if necessary

  // If the forward button is pressed

    // Move the motor in the forward direction (clockwise)
    stepperName.step(10); // Small incremental step
               // Add a short delay to stabilize motor movement
    }
