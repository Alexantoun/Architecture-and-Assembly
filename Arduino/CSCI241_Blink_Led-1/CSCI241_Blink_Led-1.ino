// Blink an LED using CSCI utility routines.

#include <CSCI_LED.h>     // Library routines
#include <CSCI_Timers.h>

const int pinLed = LED_BUILTIN; // GPIO pin number LED is connected to.

const int LedOnState = HIGH;  // State which turns LED on.
const int LedOffState = LOW;  // State which turns LED off.

const int LedOnTime = 2000;   // LED on time (in milliseconds).
const int LedOffTime = 4000;  // LED off time (in milliseconds).

// This routine called once at program start.

void setup()
{
  SetupLED(pinLed);   // Setup GPIO pin for output.
}

// This routine called repeatedly until a "reset" is performed.

void loop ()
{
  SetLEDState(pinLed, LedOnState);
  WaitMillis(LedOnTime);
  SetLEDState(pinLed, LedOffState);
  WaitMillis(LedOffTime);
}
