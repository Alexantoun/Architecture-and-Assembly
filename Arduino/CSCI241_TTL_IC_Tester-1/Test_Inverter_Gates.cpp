// Implementation of routines to test TTL ICs containing inverter gates.

#include "Globals.h"
#include "Test_Inverter_Gates.h"
#include <CSCI_Console.h>         // Console utility routines
#include "Utilities.h"

// ******************** Test SN7404 IC ****************
//
// The 7404 is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains six inverters.
//
// Input A1 at pin 1, output Y1 at pin 2.
// Input A2 at pin 3, output Y2 at pin 4.
// Input A3 at pin 5, output Y3 at pin 6.
// Input A4 at pin 9, output Y4 at pin 8.
// Input A5 at pin 11, output Y5 at pin 10.
// Input A6 at pin 13, output Y6 at pin 12.

void TestSN7404()
{
  DisplayNewline();
  DisplayString("Testing SN7404 (Hex inverter)...");
  DisplayNewline();

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW");
    
  // Set gate input to LOW.

  const int icInputsL[NumICPins] =
  {
    LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP   
  };

  SetICInputs(icInputsL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH");
    
  // Set gate input to HIGH.

  const int icInputsH[NumICPins] =
  {
    HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP   
  };

  SetICInputs(icInputsH, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);    
}

// ******************** Test SN7405 IC ****************
//
// The 7405 is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains six inverters with open collector outputs.
//
// Input A1 at pin 1, output Y1 at pin 2.
// Input A2 at pin 3, output Y2 at pin 4.
// Input A3 at pin 5, output Y3 at pin 6.
// Input A4 at pin 9, output Y4 at pin 8.
// Input A5 at pin 11, output Y5 at pin 10.
// Input A6 at pin 13, output Y6 at pin 12.

void TestSN7405()
{
  DisplayNewline();
  DisplayString("Testing SN7405 (Hex inverter, open collector)...");
  DisplayNewline();

  // Open collector IC outputs require GPIO input pins to
  // have internal pullup resistors.

  ZIFPinSetup(INPUT_PULLUP);

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW");
    
  // Set gate input to LOW.

  const int icInputsL[NumICPins] =
  {
    LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP   
  };

  SetICInputs(icInputsL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH");
    
  // Set gate input to HIGH.

  const int icInputsH[NumICPins] =
  {
    HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP   
  };

  SetICInputs(icInputsH, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);      
}

// ******************** Test SN7406 IC ****************
//
// The 7406 is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains six inverters with open collector outputs that have
// high voltage (up to 30 volt) capability.
//
// Input A1 at pin 1, output Y1 at pin 2.
// Input A2 at pin 3, output Y2 at pin 4.
// Input A3 at pin 5, output Y3 at pin 6.
// Input A4 at pin 9, output Y4 at pin 8.
// Input A5 at pin 11, output Y5 at pin 10.
// Input A6 at pin 13, output Y6 at pin 12.

void TestSN7406()
{
  DisplayNewline();
  DisplayString("Testing SN7406 (Hex inverter, open collector, high voltage)...");
  DisplayNewline();

  // Open collector IC outputs require GPIO input pins to
  // have internal pullup resistors.

  ZIFPinSetup(INPUT_PULLUP);

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW");
    
  // Set gate input to LOW.

  const int icInputsL[NumICPins] =
  {
    LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP   
  };

  SetICInputs(icInputsL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH");
    
  // Set gate input to HIGH.

  const int icInputsH[NumICPins] =
  {
    HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP   
  };

  SetICInputs(icInputsH, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);      
}

// ******************** Test SN7407 IC ****************
//
// The 7407 is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains six non-inverting gates with open collector high-voltage outputs.
//
// Input A1 at pin 1, output Y1 at pin 2.
// Input A2 at pin 3, output Y2 at pin 4.
// Input A3 at pin 5, output Y3 at pin 6.
// Input A4 at pin 9, output Y4 at pin 8.
// Input A5 at pin 11, output Y5 at pin 10.
// Input A6 at pin 13, output Y6 at pin 12.

void TestSN7407()
{
  DisplayNewline();
  DisplayString("Testing SN7407 (Hex gates, open collector, high voltage)...");
  DisplayNewline();

  // Open collector IC outputs require GPIO input pins to
  // have internal pullup resistors.

  ZIFPinSetup(INPUT_PULLUP);

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW");
    
  // Set gate input to LOW.

  const int icInputsL[NumICPins] =
  {
    LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP   
  };

  SetICInputs(icInputsL, NumICPins);

  // Check if all gate outputs are LOW.

  const int icOutputsL[NumICPins] =
  {
    SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, LOW, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH");
    
  // Set gate input to HIGH.

  const int icInputsH[NumICPins] =
  {
    HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP   
  };

  SetICInputs(icInputsH, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);      
}
