// Implementation of routines to test TTL ICs containing OR gates.

#include "Globals.h"
#include "Test_OR_Gates.h"
#include <CSCI_Console.h>         // Console utility routines
#include "Utilities.h"

// ******************** Test SN7402 IC ****************

// The SN7402 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains four 2-input NOR gates at these pins:
//
// Input 1A at pin 2, input 1B at pin 3, output 1Y at pin 1.
// Input 2A at pin 5, input 2B at pin 6, output 2Y at pin 4.
// Input 3A at pin 8, input 3B at pin 9, output 3Y at pin 10.
// Input 4A at pin 11, input 4B at pin 12, output 4Y at pin 13.

void TestSN7402()
{
  DisplayNewline();
  DisplayString("Testing SN7402 (Quad 2-input NOR gates)...");
  DisplayNewline();

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW, Input B = LOW");
    
  // Set both gate inputs to LOW.

  const int icInputsLL[NumICPins] =
  {
    SKIP, LOW, LOW, SKIP, LOW, LOW, SKIP, LOW, LOW, SKIP, LOW, LOW, SKIP, SKIP
  };

  SetICInputs(icInputsLL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    HIGH, SKIP, SKIP, HIGH, SKIP, SKIP, SKIP, SKIP, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = LOW");

  // Set gate A input HIGH and gate B input LOW.

  const int icInputsHL[NumICPins] =
  {
    SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP, SKIP
  };

  SetICInputs(icInputsHL, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    LOW, SKIP, SKIP, LOW, SKIP, SKIP, SKIP, SKIP, SKIP, LOW, SKIP, SKIP, LOW, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = LOW, Input B = HIGH");  

  // Set gate A input LOW and gate B input HIGH.

  const int icInputsLH[NumICPins] =
  {
    SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP, SKIP
  };

  SetICInputs(icInputsLH, NumICPins);

  // Check if all gate outputs are LOW.

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = HIGH");    

  // Set both gate inputs to HIGH

  const int icInputsHH[NumICPins] =
  {
    SKIP, HIGH, HIGH, SKIP, HIGH, HIGH, SKIP, HIGH, HIGH, SKIP, HIGH, HIGH, SKIP, SKIP
  };

  SetICInputs(icInputsHH, NumICPins);

  // Check if all gate outputs are LOW.

  CheckICOutputs(icOutputsL, NumICPins);
}

// ******************** Test SN7432 IC ****************

// The SN7432 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains four 2-input OR gates at these pins:
//
// Input 1A at pin 1, input 1B at pin 2, output 1Y at pin 3.
// Input 2A at pin 4, input 2B at pin 5, output 2Y at pin 6.
// Input 3A at pin 9, input 3B at pin 10, output 3Y at pin 8.
// Input 4A at pin 12, input 4B at pin 13, output 4Y at pin 11.

void TestSN7432()
{
  DisplayNewline();
  DisplayString("Testing SN7432 (Quad 2-input OR gates)...");
  DisplayNewline();

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW, Input B = LOW");
    
  // Set both gate inputs to LOW.

  const int icInputsLL[NumICPins] =
  {
    LOW, LOW, SKIP, LOW, LOW, SKIP, SKIP, SKIP, LOW, LOW, SKIP, LOW, LOW, SKIP
  };

  SetICInputs(icInputsLL, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    SKIP, SKIP, LOW, SKIP, SKIP, LOW, SKIP, LOW, SKIP, SKIP, LOW, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = LOW");

  // Set gate A input HIGH and gate B input LOW.

  const int icInputsHL[NumICPins] =
  {
    HIGH, LOW, SKIP, HIGH, LOW, SKIP, SKIP, SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP
  };

  SetICInputs(icInputsHL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = LOW, Input B = HIGH");  

  // Set gate A input LOW and gate B input HIGH.

  const int icInputsLH[NumICPins] =
  {
    LOW, HIGH, SKIP, LOW, HIGH, SKIP, SKIP, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP
  };

  SetICInputs(icInputsLH, NumICPins);

  // Check if all gate outputs are HIGH.

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = HIGH");    

  // Set both gate inputs to HIGH

  const int icInputsHH[NumICPins] =
  {
    HIGH, HIGH, SKIP, HIGH, HIGH, SKIP, SKIP, SKIP, HIGH, HIGH, SKIP, HIGH, HIGH, SKIP
  };

  SetICInputs(icInputsHH, NumICPins);

  // Check if all gate outputs are HIGH.

  CheckICOutputs(icOutputsH, NumICPins);
}

//******************** Test SN7486 IC ****************

// The SN7486 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains four 2-input XOR gates at these pins:
//
// Input 1A at pin 1, input 1B at pin 2, output 1Y at pin 3.
// Input 2A at pin 4, input 2B at pin 5, output 2Y at pin 6.
// Input 3A at pin 9, input 3B at pin 10, output 3Y at pin 8.
// Input 4A at pin 12, input 4B at pin 13, output 4Y at pin 11.

void TestSN7486()
{
  DisplayNewline();
  DisplayString("Testing SN7486 (Quad 2-input XOR gates)...");
  DisplayNewline();

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW, Input B = LOW");
    
  // Set both gate inputs to LOW.

  const int icInputsLL[NumICPins] =
  {
    LOW, LOW, SKIP, LOW, LOW, SKIP, SKIP, SKIP, LOW, LOW, SKIP, LOW, LOW, SKIP
  };

  SetICInputs(icInputsLL, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    SKIP, SKIP, LOW, SKIP, SKIP, LOW, SKIP, LOW, SKIP, SKIP, LOW, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = LOW");

  // Set gate A input HIGH and gate B input LOW.

  const int icInputsHL[NumICPins] =
  {
    HIGH, LOW, SKIP, HIGH, LOW, SKIP, SKIP, SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP
  };

  SetICInputs(icInputsHL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = LOW, Input B = HIGH");  

  // Set gate A input LOW and gate B input HIGH.

  const int icInputsLH[NumICPins] =
  {
    LOW, HIGH, SKIP, LOW, HIGH, SKIP, SKIP, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP
  };

  SetICInputs(icInputsLH, NumICPins);

  // Check if all gate outputs are HIGH.

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = HIGH");    

  // Set both gate inputs to HIGH

  const int icInputsHH[NumICPins] =
  {
    HIGH, HIGH, SKIP, HIGH, HIGH, SKIP, SKIP, SKIP, HIGH, HIGH, SKIP, HIGH, HIGH, SKIP
  };

  SetICInputs(icInputsHH, NumICPins);

  // Check if all gate outputs are LOW.

  CheckICOutputs(icOutputsL, NumICPins);
}
