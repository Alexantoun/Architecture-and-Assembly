// Illustrate bitwise operations using an 8 x 8 LED matrix.

// Demonstrate various bitwise operations using an 8 x 8 LED matrix.

#include <CSCI_LedMatrix.h>   // LED matrix utilities
#include "ShowBitwiseOps.h"   // Show bitwise operations utilities
#include "BinaryOpCircuit.h"  // Binary operation circuitry utilities 

/************* CALLED ONCE AT START UP *******************/
void setup()
{
  SetupShowBitwiseOps();
  SetupBinaryCircuit();
}

/******************* MAIN LOOP ***************************/

void loop() 
{
  // Demonstrate basic boolean logic operations
  
  ShowLogicalNOT(0b11110000);
  ClearLedMatrix();

  ShowLogicalNOT(0b10101010);
  ClearLedMatrix();
  
  ShowLogicalOR(0b11110000, 0b10101010);  
  ClearLedMatrix();

  ShowLogicalAND(0b11110000, 0b10101010);
  ClearLedMatrix();

  ShowLogicalXOR(0b11110000, 0b10101010);
  ClearLedMatrix();

  // Demonstrate twos complement negation.

  ShowNegation(0b00011101);
  ClearLedMatrix();

  ShowNegation(0b11100011);
  ClearLedMatrix();
  
  // Perform basic boolean logic operations bit by bit.

  ShowBitByBit(0b11110000, 0, LogicalNOTFunc);
  ClearLedMatrix();

  ShowBitByBit(0b10101010, 0, LogicalNOTFunc);
  ClearLedMatrix();

  ShowBitByBit(0b11110000, 0b10101010, LogicalORFunc);
  ClearLedMatrix();

  ShowBitByBit(0b11110000, 0b10101010, LogicalANDFunc);
  ClearLedMatrix();

  ShowBitByBit(0b11110000, 0b10101010, LogicalXORFunc);
  ClearLedMatrix();
 
  // Perform some ADC (Add with Carry) operations

  ShowBitByBit(0b00011101, 0b00001010, ADCFunc);
  ClearLedMatrix();

  ShowBitByBit(0b00011101, 0b00001110, ADCFunc);
  ClearLedMatrix();

  ShowBitByBit(0b00001101, 0b11111111, ADCFunc);
  ClearLedMatrix();

  ShowBitByBit(0b00001101, 0b11110011, ADCFunc);
  ClearLedMatrix();

  /*
  // Perform a logical operation using external circuitry.
  // (SN7400 NAND gate)

  ShowBitByBit(0b11110000, 0b10101010, BinaryCircuitFunc);
  ClearLedMatrix();

  // Peform some additions using external circuitry.
  // (SN7486 XOR, SN7408 AND, SN7432 OR gates wired as a full adder.)

  ShowBitByBit(0b00011101, 0b00001010, BinaryCircuitFunc);
  ClearLedMatrix();

  ShowBitByBit(0b00011101, 0b00001110, BinaryCircuitFunc);
  ClearLedMatrix();

  ShowBitByBit(0b00001101, 0b11111111, BinaryCircuitFunc);
  ClearLedMatrix();

  ShowBitByBit(0b00001101, 0b11110011, BinaryCircuitFunc);
  ClearLedMatrix();
  */
}

// ---------------------------------------------------------
// Callback function to perform a logical NOT on the first
// of two boolean values.

bool LogicalNOTFunc(bool valueA, bool valueB, bool& valueCarry)
{
  // Carry in is ignored.  Set carry out to false (0).

   valueCarry = 0;

   // Second operand is ignored.

   return !valueA;
}

// ---------------------------------------------------------
// Callback function to perform a logical OR on two boolean values.

bool LogicalORFunc(bool valueA, bool valueB, bool& valueCarry)
{
  // Carry in is ignored.  Set carry out to false (0).

   valueCarry = 0;

   return valueA | valueB;
}

// ---------------------------------------------------------
// Callback function to perform a logical AND on two boolean values.

bool LogicalANDFunc(bool valueA, bool valueB, bool& valueCarry)
{
  // Carry in is ignored.  Set carry out to false (0).

   valueCarry = 0;

   return valueA & valueB;
}

// ---------------------------------------------------------
// Callback function to perform a logical XOR on two boolean values.

bool LogicalXORFunc(bool valueA, bool valueB, bool& valueCarry)
{
  // Carry in is ignored.  Set carry out to false (0).

   valueCarry = 0;

   return valueA ^ valueB;
}

// ---------------------------------------------------------
// Callback function to perform addition on two boolean values.

bool ADCFunc(bool valueA, bool valueB, bool& valueCarry)
{
  // Convert boolean values to integers.

  int intA = valueA ? 1 : 0;
  int intB = valueB ? 1 : 0;
  int intCarry = valueCarry ? 1: 0;

  // Perform addition with carry in,

  int intResult = intA + intB + intCarry;

  // Return the carry out state.

  valueCarry = ( intResult & 0b00000010 ) ? true : false;

  // Return the result of the addition.

  return ( intResult & 0b00000001 ) ? true : false;
}
