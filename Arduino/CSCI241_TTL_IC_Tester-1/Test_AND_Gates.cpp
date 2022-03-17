// Implementation of routines to test TTL ICs containing AND gates.

#include "Globals.h"
#include "Test_AND_Gates.h"
#include "Utilities.h"
#include <CSCI_Console.h>     // Console utility routines
#include <CSCI_SysUtils.h>    // System-wide utility routines

// ******************** Test SN7400 IC ****************

// The SN7400 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains four 2-input NAND gates at these pins:
//
// Input 1A at pin 1, input 1B at pin 2, output 1Y at pin 3.
// Input 2A at pin 4, input 2B at pin 5, output 2Y at pin 6.
// Input 3A at pin 10, input 3B at pin 9, output 3Y at pin 8.
// Input 4A at pin 13, input 4B at pin 12, output 4Y at pin 11.

void TestSN7400()
{
  DisplayNewline();
  DisplayString("Testing SN7400 (Quad 2-input NAND gates)...");
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

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = LOW");

  // Set gate A input HIGH and gate B input LOW.

  const int icInputsHL[NumICPins] =
  {
    HIGH, LOW, SKIP, HIGH, LOW, SKIP, SKIP, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP
  };

  SetICInputs(icInputsHL, NumICPins);

  // Check if all gate outputs are HIGH.

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = LOW, Input B = HIGH");  

  // Set gate A input LOW and gate B input HIGH.

  const int icInputsLH[NumICPins] =
  {
    LOW, HIGH, SKIP, LOW, HIGH, SKIP, SKIP, SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP
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

  const int icOutputsL[NumICPins] =
  {
    SKIP, SKIP, LOW, SKIP, SKIP, LOW, SKIP, LOW, SKIP, SKIP, LOW, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);
}

// ******************** Test SN7403 IC ****************

// The SN7403 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains four 2-input NAND gates with open collectors at these pins:
//
// Input 1A at pin 1, input 1B at pin 2, output 1Y at pin 3.
// Input 2A at pin 4, input 2B at pin 5, output 2Y at pin 6.
// Input 3A at pin 10, input 3B at pin 9, output 3Y at pin 8.
// Input 4A at pin 13, input 4B at pin 12, output 4Y at pin 11.

void TestSN7403()
{
  DisplayNewline();
  DisplayString("Testing SN7403 (Quad 2-input NAND gates, open collector)...");
  DisplayNewline();

  // Open collector IC outputs require GPIO input pins to
  // have internal pullup resistors.

  ZIFPinSetup(INPUT_PULLUP);  

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);

  DisplayTestInfo("Test: Input A = LOW, Input B = LOW");
    
  // Set both gate inputs to LOW.

  const int icInputsLL[NumICPins] =
  {
    LOW, LOW, SKIP, LOW, LOW, SKIP, SKIP, SKIP, LOW, LOW, SKIP, LOW, LOW, SKIP
  };

  SetICInputs(icInputsLL, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = LOW");

  // Set gate A input HIGH and gate B input LOW.

  const int icInputsHL[NumICPins] =
  {
    HIGH, LOW, SKIP, HIGH, LOW, SKIP, SKIP, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP
  };

  SetICInputs(icInputsHL, NumICPins);

  // Check if all gate outputs are HIGH.

  CheckICOutputs(icOutputsH, NumICPins);

  DisplayTestInfo("Test: Input A = LOW, Input B = HIGH");  

  // Set gate A input LOW and gate B input HIGH.

  const int icInputsLH[NumICPins] =
  {
    LOW, HIGH, SKIP, LOW, HIGH, SKIP, SKIP, SKIP, HIGH, LOW, SKIP, HIGH, LOW, SKIP
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

  const int icOutputsL[NumICPins] =
  {
    SKIP, SKIP, LOW, SKIP, SKIP, LOW, SKIP, LOW, SKIP, SKIP, LOW, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsL, NumICPins);
}

// ******************** Test SN7408 IC ****************

// The SN7408 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains four 2-input AND gates at these pins:
//
// Input 1A at pin 1, input 1B at pin 2, output 1Y at pin 3.
// Input 2A at pin 4, input 2B at pin 5, output 2Y at pin 6.
// Input 3A at pin 9, input 3B at pin 10, output 3Y at pin 8.
// Input 4A at pin 12, input 4B at pin 13, output 4Y at pin 11.

void TestSN7408()
{
  DisplayNewline();
  DisplayString("Testing SN7408 (Quad 2-input AND gates)...");
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

  // Check if all gate outputs are HIGH.

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

  // Check if all gate outputs are LOW.

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = LOW, Input B = HIGH");  

  // Set gate A input LOW and gate B input HIGH.

  const int icInputsLH[NumICPins] =
  {
    LOW, HIGH, SKIP, LOW, HIGH, SKIP, SKIP, SKIP, LOW, HIGH, SKIP, LOW, HIGH, SKIP
  };

  SetICInputs(icInputsLH, NumICPins);

  // Check if all gate outputs are LOW.

  CheckICOutputs(icOutputsL, NumICPins);

  DisplayTestInfo("Test: Input A = HIGH, Input B = HIGH");    

  // Set both gate inputs to HIGH

  const int icInputsHH[NumICPins] =
  {
    HIGH, HIGH, SKIP, HIGH, HIGH, SKIP, SKIP, SKIP, HIGH, HIGH, SKIP, HIGH, HIGH, SKIP
  };

  SetICInputs(icInputsHH, NumICPins);

  // Check if all gate outputs are HIGH.

  const int icOutputsH[NumICPins] =
  {
    SKIP, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, HIGH, SKIP, SKIP, SKIP
  };

  CheckICOutputs(icOutputsH, NumICPins);
}

// ******************** Test SN7410 IC ****************

// The SN7410 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains three 3-input NAND gates at these pins:
//
// Input 1A at pin 1, input 1B at pin 2, input 1C at pin 13, output 1Y at pin 12.
// Input 2A at pin 3, input 2B at pin 4, input 2C at pin 5, output 2Y at pin 6.
// Input 3A at pin 9, input 3B at pin 10, input 3C at pin 11, output 3Y at pin 8.

void TestSN7410()
{
  DisplayNewline();
  DisplayString("Testing SN7410 (Triple 3-input NAND gates)...");
  DisplayNewline();

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);
    
  // Table of all input combinations where expected outputs are all HIGH.
  
  const int ExpectedHighInputs[][Num_14PinIC_Pins] =
  {
    { LOW, LOW, LOW, LOW, LOW, SKIP, SKIP, SKIP, LOW, LOW, LOW, SKIP, LOW, SKIP },
    { LOW, LOW, HIGH, LOW, LOW, SKIP, SKIP, SKIP, HIGH, LOW, LOW, SKIP, HIGH, SKIP },
    { HIGH, LOW, LOW, HIGH, LOW, SKIP, SKIP, SKIP, LOW, HIGH, LOW, SKIP, LOW, SKIP },
    { HIGH, LOW, HIGH, HIGH, LOW, SKIP, SKIP, SKIP, HIGH, HIGH, LOW, SKIP, HIGH, SKIP },
    { LOW, HIGH, LOW, LOW, HIGH, SKIP, SKIP, SKIP, LOW, LOW, HIGH, SKIP, LOW, SKIP },
    { LOW, HIGH, HIGH, LOW, HIGH, SKIP, SKIP, SKIP, HIGH, LOW, HIGH, SKIP, HIGH, SKIP },
    { HIGH, HIGH, LOW, HIGH, HIGH, SKIP, SKIP, SKIP, LOW, HIGH, HIGH, SKIP, LOW, SKIP } 
  };
  
  const int Num_ExpectedHighInputs = sizeof(ExpectedHighInputs) / sizeof(ExpectedHighInputs[0]);
  
  // Table of all input combinations where expected outputs are all LOW.
  
  const int ExpectedLowInputs[][Num_14PinIC_Pins] =
  {
    { HIGH, HIGH, HIGH, HIGH, HIGH, SKIP, SKIP, SKIP, HIGH, HIGH, HIGH, SKIP, HIGH, SKIP } 
  };
  
  const int Num_ExpectedLowInputs = sizeof(ExpectedLowInputs) / sizeof(ExpectedLowInputs[0]);
  
  // Array where all expected outputs are HIGH.
  
  const int ExpectedOutputsHigh[] =
  {
    SKIP, SKIP, SKIP, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, SKIP, HIGH, SKIP, SKIP
  };
  
  // Array where all expected outputs are LOW.
  
  const int ExpectedOutputsLow[] =
  {
    SKIP, SKIP, SKIP, SKIP, SKIP, LOW, SKIP, LOW, SKIP, SKIP, SKIP, LOW, SKIP, SKIP
  };
  
  // Do all tests where expected outputs are all HIGH.

  for ( int input_index = 0; input_index < Num_ExpectedHighInputs; input_index++ )
  {
    String msg("Test: ");
    msg += "Input A = " + GetStateName(ExpectedHighInputs[input_index][2]);
    msg += ", Input B = " + GetStateName(ExpectedHighInputs[input_index][3]);
    msg += ", Input C = " + GetStateName(ExpectedHighInputs[input_index][4]);

    DisplayTestInfo(msg);

    SetICInputs(ExpectedHighInputs[input_index], NumICPins);

    CheckICOutputs(ExpectedOutputsHigh, NumICPins);     
  }

  // Do all tests where expected outputs are LOW.

  for ( int input_index = 0; input_index < Num_ExpectedLowInputs; input_index++ )
  {
    String msg("Test: ");
    msg += "Input A = " + GetStateName(ExpectedLowInputs[input_index][2]);
    msg += ", Input B = " + GetStateName(ExpectedLowInputs[input_index][3]);
    msg += ", Input C = " + GetStateName(ExpectedLowInputs[input_index][4]);

    DisplayTestInfo(msg);

    SetICInputs(ExpectedLowInputs[input_index], NumICPins);

    CheckICOutputs(ExpectedOutputsLow, NumICPins);     
  }
}

// ******************** Test SN7411 IC ****************

// The SN7411 IC is a 14-pin DIP IC with Vcc at pin 14 and GND at pin 7.
// It contains three 3-input AND gates at these pins:
//
// Input 1A at pin 13, input 1B at pin 1, input 1C at pin 2, output 1Y at pin 12.
// Input 2A at pin 3, input 2B at pin 4, input 2C at pin 5, output 2Y at pin 6.
// Input 3A at pin 9, input 3B at pin 10, input 3C at pin 11, output 3Y at pin 8.

void TestSN7411()
{
  DisplayNewline();
  DisplayString("Testing SN7411 (Triple 3-input AND gates)...");
  DisplayNewline();

  NumICPins = Num_14PinIC_Pins;
  SetICPowerPins(7, 14);
    
  // Table of all input combinations where expected outputs are all LOW.
  
  const int ExpectedLowInputs[][Num_14PinIC_Pins] =
  {
    { LOW, LOW, LOW, LOW, LOW, SKIP, SKIP, SKIP, LOW, LOW, LOW, SKIP, LOW, SKIP },
    { LOW, LOW, HIGH, LOW, LOW, SKIP, SKIP, SKIP, HIGH, LOW, LOW, SKIP, HIGH, SKIP },
    { HIGH, LOW, LOW, HIGH, LOW, SKIP, SKIP, SKIP, LOW, HIGH, LOW, SKIP, LOW, SKIP },
    { HIGH, LOW, HIGH, HIGH, LOW, SKIP, SKIP, SKIP, HIGH, HIGH, LOW, SKIP, HIGH, SKIP },
    { LOW, HIGH, LOW, LOW, HIGH, SKIP, SKIP, SKIP, LOW, LOW, HIGH, SKIP, LOW, SKIP },
    { LOW, HIGH, HIGH, LOW, HIGH, SKIP, SKIP, SKIP, HIGH, LOW, HIGH, SKIP, HIGH, SKIP },
    { HIGH, HIGH, LOW, HIGH, HIGH, SKIP, SKIP, SKIP, LOW, HIGH, HIGH, SKIP, LOW, SKIP } 
  };
  
  const int Num_ExpectedLowInputs = sizeof(ExpectedLowInputs) / sizeof(ExpectedLowInputs[0]);
  
  // Table of all input combinations where expected outputs are all HIGH.
  
  const int ExpectedHighInputs[][Num_14PinIC_Pins] =
  {
    { HIGH, HIGH, HIGH, HIGH, HIGH, SKIP, SKIP, SKIP, HIGH, HIGH, HIGH, SKIP, HIGH, SKIP } 
  };
  
  const int Num_ExpectedHighInputs = sizeof(ExpectedHighInputs) / sizeof(ExpectedHighInputs[0]);
  
  // Array where all expected outputs are LOW.
  
  const int ExpectedOutputsLow[] =
  {
    SKIP, SKIP, SKIP, SKIP, SKIP, LOW, SKIP, LOW, SKIP, SKIP, SKIP, LOW, SKIP, SKIP
  };
  
  // Array where all expected outputs are HIGH.
  
  const int ExpectedOutputsHigh[] =
  {
    SKIP, SKIP, SKIP, SKIP, SKIP, HIGH, SKIP, HIGH, SKIP, SKIP, SKIP, HIGH, SKIP, SKIP
  };
  
  // Do all tests where expected outputs are all LOW.

  for ( int input_index = 0; input_index < Num_ExpectedLowInputs; input_index++ )
  {
    String msg("Test: ");
    msg += "Input A = " + GetStateName(ExpectedLowInputs[input_index][2]);
    msg += ", Input B = " + GetStateName(ExpectedLowInputs[input_index][3]);
    msg += ", Input C = " + GetStateName(ExpectedLowInputs[input_index][4]);

    DisplayTestInfo(msg);

    SetICInputs(ExpectedLowInputs[input_index], NumICPins);

    CheckICOutputs(ExpectedOutputsLow, NumICPins);     
  }

  // Do all tests where expected outputs are HIGH.

  for ( int input_index = 0; input_index < Num_ExpectedHighInputs; input_index++ )
  {
    String msg("Test: ");
    msg += "Input A = " + GetStateName(ExpectedHighInputs[input_index][2]);
    msg += ", Input B = " + GetStateName(ExpectedHighInputs[input_index][3]);
    msg += ", Input C = " + GetStateName(ExpectedHighInputs[input_index][4]);

    DisplayTestInfo(msg);

    SetICInputs(ExpectedHighInputs[input_index], NumICPins);

    CheckICOutputs(ExpectedOutputsHigh, NumICPins);     
  }
}
