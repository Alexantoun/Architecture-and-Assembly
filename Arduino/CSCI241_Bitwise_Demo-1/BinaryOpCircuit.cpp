// Implementation related to external binary operation circuitry.

#include <Arduino.h>

const int inputAPin = 9;        // GPIO output pin to set input "A".  
const int inputBPin = 8;        // GPIO output pin to set input "B".
const int inputCarryPin = 7;    // GPIO output pin to set input carry.
const int outputResultPin = 6;  // GPIO input pin to read output result.  
const int outputCarryPin = 5;   // GPIO input pin to read output carry.

// ---------------------------------------------------------
bool BinaryCircuitFunc(bool valueA, bool valueB, bool& valueCarry)
{
  // Set the external circuity inputs.

  int stateA = valueA ? HIGH : LOW;
  int stateB = valueB ? HIGH : LOW;
  int stateCarry = valueCarry ? HIGH: LOW;

  digitalWrite(inputAPin, stateA);
  digitalWrite(inputBPin, stateB);
  digitalWrite(inputCarryPin, stateCarry);

  delay(1);   // Delay a short time so circuit can settle down.

  // Read circut outputs and return them.

  valueCarry = false;
  
  if ( digitalRead(outputCarryPin) == HIGH )
  {
    valueCarry = true;
  }

  bool valueResult = false;

  if ( digitalRead(outputResultPin) == HIGH )
  {
    valueResult = true;
  }

  return valueResult;
}

// ---------------------------------------------------------
void SetupBinaryCircuit()
{
  pinMode(inputAPin, OUTPUT);
  pinMode(inputBPin, OUTPUT);
  pinMode(inputCarryPin, OUTPUT);
  pinMode(outputResultPin, INPUT_PULLUP);
  pinMode(outputCarryPin, INPUT_PULLUP);

  // Set external circuitry input pins LOW.

  digitalWrite(inputAPin, LOW);
  digitalWrite(inputBPin, LOW);
  digitalWrite(inputCarryPin, LOW);
}
