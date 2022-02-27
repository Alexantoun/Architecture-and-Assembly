#ifndef INCLUDE_BINARY_OP_CIRCUIT_H
#define INCLUDE_BINARY_OP_CIRCUIT_H

// Definitions related to external binary operation circuitry.

// Callback function to perform a binary operation on two
// boolean values (plus an optional "carry" value) using
// circuitry connected to Arduino GPIO pins.

bool BinaryCircuitFunc(bool valueA, bool valueB, bool& valueCarry);

// Routine to initialize external binary operation ciruitry.

void SetupBinaryCircuit();

#endif  //INCLUDE_BINARY_OP_CIRCUIT_H
