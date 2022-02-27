// Routines to show bitwise operations in the 8 x 8 LED matrix.

#include "ShowBitwiseOps.h"
#include <CSCI_LedMatrix.h>   // 8 x 8 LED matrix utilities
#include <CSCI_PushButton.h>  // Momentary pushbutton utilities

// Flow control pushbutton GPIO pin.

const int ctlButton = 53;

// ---------------------------------------------------------
void ShowLogicalNOT(uint8_t operand)
{
  ShowOperands(operand, 0b00000000);
  ShowResult(0);

  WaitForButtonClick(ctlButton); 

  uint8_t result = ~operand;

  ShowResult(result);

  WaitForButtonClick(ctlButton);
}

// ---------------------------------------------------------
void ShowLogicalAND(uint8_t operandA, uint8_t operandB)
{
  ShowOperands(operandA, operandB);
  ShowResult(0);

  WaitForButtonClick(ctlButton); 

  uint8_t result = operandA & operandB;

  ShowResult(result);

  WaitForButtonClick(ctlButton);
}

// ---------------------------------------------------------
void ShowLogicalOR(uint8_t operandA, uint8_t operandB)
{
  ShowOperands(operandA, operandB);
  ShowResult(0);

  WaitForButtonClick(ctlButton); 

  uint8_t result = operandA | operandB;

  ShowResult(result);

  WaitForButtonClick(ctlButton);
}

// ---------------------------------------------------------
void ShowLogicalXOR(uint8_t operandA, uint8_t operandB)
{
  ShowOperands(operandA, operandB);
  ShowResult(0);

  WaitForButtonClick(ctlButton); 

  uint8_t result = operandA ^ operandB;

  ShowResult(result);

  WaitForButtonClick(ctlButton);
}

// ---------------------------------------------------------
void ShowNegation(uint8_t operand)
{
  // Show the number to be negated.

  ShowOperands(operand, 0b00000000);
  ShowResult(0);

  WaitForButtonClick(ctlButton);

  // Show ones complement

  uint8_t complement = ~operand;

  ShowOperands(operand, complement);

  WaitForButtonClick(ctlButton);

  // Show ones complement + 1

  uint8_t neg_value = complement + 1;

  ShowResult(neg_value);

  WaitForButtonClick(ctlButton);  
}

// ---------------------------------------------------------
void ShowBitByBit(uint8_t operandA, uint8_t operandB,
                  bool (*opfunc) (bool, bool, bool&) )
{
  ShowOperands(operandA, operandB);

  uint8_t bitmask = 0b00000001;   // Set bit mask for LSB.
  uint8_t carryBits = 0;          // Set all carry bits to zero.
  uint8_t resultBits = 0;         // Set all operation result bits to zero.  

  do
  {
    // Show results prior to operating on this bit.
    
    ShowMarkerBits(bitmask);
    ShowCarryBits(carryBits);
    ShowResult(resultBits);

    WaitForButtonClick(ctlButton);

    // Extract selected bit values and represent them as booleans
    // (false = 0, true = 1).

    bool bitA = ( operandA & bitmask ) ? true : false;
    bool bitB = ( operandB & bitmask ) ? true : false;
    bool bitC = ( carryBits & bitmask ) ? true : false;
    
    // Perform operation on the selected bit.

    bool bitResult = (*opfunc)(bitA, bitB, bitC);

    // Set appropriate result bit.

    if ( bitResult )
    {
      resultBits |= bitmask;    // Set bit to 1.
    }
    else
    {
      resultBits &= ~bitmask;   // Set bit to 0.
    }

    bitmask = bitmask << 1;     // Shift bit mask one bit to the left.

    // Set appropriate carry bit.

    if ( bitC )
    {
      carryBits |= bitmask;     // Set bit to 1.
    }
    else
    {
      carryBits &= ~bitmask;    // Set bit to 0.
    }
                             
  }  while ( bitmask != 0 );    // While there are still bits to operate on.

  // Show results of final operation
  
  ShowMarkerBits(bitmask);
  ShowCarryBits(carryBits);
  ShowResult(resultBits);
  
  WaitForButtonClick(ctlButton);
}

// ---------------------------------------------------------
void ShowOperands(uint8_t operandA, uint8_t operandB)
{
  const int rowA = 1;  // Row where A is displayed.
  const int rowB = 2;  // Row where B is displayed.

  SetLedRow(rowA, operandA);
  SetLedRow(rowB, operandB); 
}

// ---------------------------------------------------------
void ShowResult(uint8_t result)
{
  const int rowResult = 4; // Row where result is displayed.

  SetLedRow(rowResult, result);
}

// ---------------------------------------------------------
void ShowCarryBits(uint8_t carryBits)
{
  const int rowCarry = 0; // Row where carry bits are displayed.

  SetLedRow(rowCarry, carryBits);
}

// ---------------------------------------------------------
void ShowMarkerBits(uint8_t markerBits)
{
  const int rowMarker = 7; // Row where marker bits are displayed.

  SetLedRow(rowMarker, markerBits);
}

// ---------------------------------------------------------
void SetupShowBitwiseOps()
{
  SetupButton(ctlButton);
  SetupLedMatrix();
}    
