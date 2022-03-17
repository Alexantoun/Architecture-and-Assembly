#ifndef INCLUDE_UTILITIES_H
#define INCLUDE_UTILITIES_H

#include <Arduino.h>

// *********** ZIF Socket Manipulation Routines *******

// Set IC power pin voltages.

void SetICPowerPins(int pinGND, int pinVCC);

// Set IC input pin states according to the passed input state array.
// State array's first element (subscript 0) contains the state (HIGH/LOW)
// to set IC pin 1, etc.  A state value of SKIP means leave the pin alone.

void SetICInputs(const int icInputs[], int numICPins);  // Set IC pin input states.

// Check the specified IC output pins to see if they are in the expected
// state.  If not, report the error.  A state of SKIP means skip the IC pin.

void CheckICOutputs(const int icOutputs[], int numICPins);

// Set IC pin as an INPUT pin, which means the GPIO pin
// should be set as an OUTPUT pin.

void SetICPinAsInput(int pin);

// Set IC pin as an OUTPUT pin, which means the GPIO pin
// should be set as an INPUT pin.

void SetICPinAsOutput(int pin);

// Set IC input pin HIGH

void SetICInputHigh(int pin);

// Set IC input pin LOW

void SetICInputLow(int pin);

// Read state of IC output pin

int ReadICOutput(int pin);

// Map IC pin to corresponding GPIO pin.

int MapICPinToGPIO(int icPin);

// *************** Misc Utility Routines **************

// Prompt for the IC name and return the IC number (7400, 7404, 7432, etc).

int InputICNumber();

// Initialize all GPIO pins associated with ZIF socket pins
// to be input pins so no current is flowing throug the IC
// (if there is one inserted).  The inputMode parameter is
// either INPUT or INPUT_PULLUP.

void ZIFPinSetup(int inputMode);

// Display test information on the console.

void DisplayTestInfo(const String& msg);

#endif INCLUDE_UTILITIES_H
