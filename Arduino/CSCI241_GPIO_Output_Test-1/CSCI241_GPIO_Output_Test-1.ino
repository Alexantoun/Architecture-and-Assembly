// Test a GPIO output pin.

#include <CSCI_GPIO.h>    // Library routines
#include <CSCI_Timers.h>

const int GPIOPin = 5;  // GPIO pin number to test.
const int LedPin = 49;  // GPIO pin LED is connected to.

const int LedOnState = LOW; // State which turns LED on.
const int LedOffState = HIGH; // State which turns LED off.

// This routine called once at program start.

void setup()
{
  SetupGPIOOutputPin(GPIOPin);
  SetupGPIOOutputPin(LedPin);  
}

// This routine called repeatedly until a "reset" is performed.

void loop ()
{
  // Set GPIO pin HIGH, turn LED on. Wait a specific time.
  // Set GPIO pin LOW, turn LED off. Wait a specific time.
  
  SetGPIOOutputState(GPIOPin, HIGH);
  SetGPIOOutputState(LedPin, LedOnState);  
  WaitMillis(6000);
  SetGPIOOutputState(GPIOPin, LOW);
  SetGPIOOutputState(LedPin, LedOffState);
  WaitMillis(4000);
}
