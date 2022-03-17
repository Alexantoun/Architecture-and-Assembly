// Author: Wayne Wall
// Description: Counts number of LOW to HIGH transitions in a pulse train.

#include <CSCI_LED.h>     // Library routines
#include <CSCI_Timers.h>


// Port-related definitions.

const uint8_t *PortDirPtr = &DDRL;    // Ptr to port direction control.
const uint8_t *PortDataPtr = &PORTL;  // Ptr to port data.

// State-related definitions

const int LedOnState = LOW;   // State which turns LED on.
const int LedOffState = HIGH; // State which turns LED off.

// Holds the number of pulses counted.

unsigned long int pulseCount = 0;

// This routine called once at program start.

void setup()
{
  SetupLEDPort(PortDirPtr); // Set port direction bits

  // Set all LEDs off.
  
  SetLEDPortBits(PortDataPtr, 0, LedOnState);  
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
  //       function prototype specifies second parameter as uint8_t.

  SetLEDPortBits(PortDataPtr, static_cast<uint8_t>(pulseCount), LedOnState);  
}
