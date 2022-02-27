// Programmer: Wayne Wall
// Description: Use push buttons to control blinking groups of LEDs.

// CSCI utility library definitions

#include <CSCI_PushButton.h>
#include <CSCI_LED.h>
#include <CSCI_Buzzer.h>
#include <CSCI_Timers.h>

const int pinButtonA = 52;  // GPIO pin pushbutton "A" is connected to.
const int pinButtonB = 53;  // GPIO pin pushbutton "B" is connected to.

const int pinBuzzer = 51;   // GPIO pin active buzzer is connected to.

// Array which maps bit positions (0 = LSB, 7 = MSB) to LED GPIO pin numbers.
// Note: It's conincidental the bits are connected to consecutive pin numbers.

const int BitToPin[8] = { 49, 48, 47, 46, 45, 44, 43, 42 };

const int LedOnState = LOW;   // State which turns LED on.
const int LedOffState = HIGH; // State which turns LED off.

// This routine called once at program start.

void setup()
{
  SetupButton(pinButtonA);
  SetupButton(pinButtonB);

  SetupActiveBuzzer(pinBuzzer);

  // Setup GPIO pins corresponding to each bit in LED control byte.
     
  SetupLEDByte(BitToPin);

  // Make sure all LEDs are off.
  
  SetLEDByteState(0b11111111, BitToPin, LedOffState);
}

// This routine called repeatedly until a "reset" is performed.

void loop ()
{
  uint8_t ledBits = 0b00000000; // One bit per LED.  Assumed all LEDs are off.

  // If pushbutton "A" is pressed...

  if ( IsButtonPressed(pinButtonA) )
  {
    // Set bits which will turn on the red LEDs

    ledBits = ledBits | 0b11110000;
  }

  // If pushbutton "B" is pressed...

  if ( IsButtonPressed(pinButtonB) )
  {
    // Set bits which will turn on the green LEDs

    ledBits = ledBits | 0b00001111;
  }

  // If there are LEDs to turn on...

  if ( ledBits != 0b00000000 )
  {
    // Turn on the appropriate LEDs
  
    SetLEDByteState(ledBits, BitToPin, LedOnState);
  
    // Emit sound from active buzzer.
  
    SetActiveBuzzerOn(pinBuzzer);
  
    // Hold them on for some amount of time (in milliseconds)
  
    const uint32_t holdTime = 100;
    
    WaitMillis(holdTime);
  
    // Turn all the LEDs off.
  
    SetLEDByteState(0b11111111, BitToPin, LedOffState);
  
    // Hold them off for same amount of time.
  
    WaitMillis(holdTime);
  }
  else
  {
    // Silence active buzzer.
  
    SetActiveBuzzerOff(pinBuzzer);    
  }
}
