// Author: Wayne Wall
// Description: Counts number of LOW to HIGH transitions in a pulse train.

#include <CSCI_LED.h>     // Library routines
#include <CSCI_Timers.h>

// Array which maps bit positions (0 = LSB, 7 = MSB) to LED GPIO pin numbers.
// Note: It's conincidental the bits are connected to consecutive pin numbers.

const int BitToPin[8] = { 49, 48, 47, 46, 45, 44, 43, 42 };

const int LedOnState = LOW;   // State which turns LED on.
const int LedOffState = HIGH; // State which turns LED off.

// Holds the number of pulses counted.

unsigned long int pulseCount = 0;

// This routine called once at program start.

void setup()
{
  // Setup each LED GPIO pin.

  SetupLEDByte(BitToPin);

  // Set all LEDs off.

  SetLEDByteState(0xFF, BitToPin, LedOffState);  
}

// This routine called repeatedly until a "reset" is performed.

void loop ()
{
  // For now, simulate waiting for a LOW to HIGH transition by simply
  // waiting a fixed amount of time.  Must replace this code with logic
  // to sense a LOW to HIGH transition at a GPIO input pin.

  WaitMillis(1000);

  // Count the LOW to HIGH transition.

  pulseCount++;  
  
  // Display the low-order 8-bits of number of pulses counter in
  // the bank of 8 LEDs on the utility breadboard.
  //
  // Note: Explicit static_cast is actually unnecessary as the
  //       function prototype specifies first parameter as uint8_t.
  
  SetLEDByteState(static_cast<uint8_t>(pulseCount), BitToPin, LedOnState);
}
