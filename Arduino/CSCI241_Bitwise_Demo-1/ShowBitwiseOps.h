#ifndef INCLUDE_CSCI_SHOW_BITWISE_OPS_H
#define INCLUDE_CSCI_SHOW_BITWISE_OPS_H

#include <Arduino.h>

// Definitions related to showing bitwise operations.

// Routine to show the result of a logical NOT operation

void ShowLogicalNOT(uint8_t operand);

// Routine to show the result of a logical AND operation.

void ShowLogicalAND(uint8_t operandA, uint8_t operandB);

// Routine to show the result of a logical OR operation.

void ShowLogicalOR(uint8_t operandA, uint8_t operandB);

// Routine to show the result of a logical XOR operation.

void ShowLogicalXOR(uint8_t operandA, uint8_t operandB);

// Routine to show the result of a two's complement NEGATION.

void ShowNegation(uint8_t operand);

// Routine to show an operation bit-by-bit (from LSB to MSB)

// Note: The third argument is a pointer to a function that takes
//       two boolean values and a boolean "carry" value (false = 0,
//       true = 1).  The function returns the resultant boolean
//       value and sets the carry value to any resultant carry out.
   
void ShowBitByBit(uint8_t operandA, uint8_t operandB,
                  bool (*opfunc) (bool opA, bool opB, bool& carry) );

// Routine to show two 8-bit binary operands on LED matrix.
// Operand "A" is displayed first, then operand "B" is
// displayed below it.

void ShowOperands(uint8_t operandA, uint8_t operandB);

// Routine to show an 8-bit result on LED matrix.

void ShowResult(uint8_t result);

// Routine to show carry bits.

void ShowCarryBits(uint8_t carryBits);

// Routine to show bit markers.

void ShowMarkerBits(uint8_t markerBits);

// Routine to initialize ShowBitmapOps utilities.

void SetupShowBitwiseOps();

#endif    // INCLUDE_CSCI_SHOW_BITWISE_OPS_H
