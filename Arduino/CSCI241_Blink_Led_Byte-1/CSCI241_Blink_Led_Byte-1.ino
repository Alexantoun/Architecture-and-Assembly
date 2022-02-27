// Blink a byte's worth of LEDs.

#include <CSCI_LED.h>     // Library routines
#include <CSCI_Timers.h>

// Array which maps bit positions (0 = LSB, 7 = MSB) to LED GPIO pin numbers.
// Note: It's conincidental the bits are connected to consecutive pin numbers.

const int BitToPin[8] = { 49, 48, 47, 46, 45, 44, 43, 42 };

const int LedOnState = LOW;   // State which turns LED on.
const int LedOffState = HIGH; // State which turns LED off.

const int LedOnTime = 400;   // LED on time (in milliseconds).
const int LedOffTime = 600;  // LED off time (in milliseconds).

// This routine called once at program start.

void setup()
{
  // Setup each LED GPIO pin.

  SetupLEDByte(BitToPin);
}

// This routine called repeatedly until a "reset" is performed.

void loop ()
{
  uint8_t ledByte = 0b00000001;

  SweepLSBToMSB(ledByte, BitToPin, LedOnState, LedOnTime, LedOffTime);

  ledByte = 0b10000000;
  
  SweepMSBToLSB(ledByte, BitToPin, LedOnState, LedOnTime, LedOffTime);
  
  ledByte = 0b11110000;

  for ( int times = 1; times <= 8; times++ )
  {
    CompPattern(ledByte, BitToPin, LedOnState, LedOnTime, LedOffTime);
  }
  
  ledByte = 0b10101010;

  for ( int times = 1; times <= 8; times++ )
  {
    CompPattern(ledByte, BitToPin, LedOnState, LedOnTime, LedOffTime);
  }  
}

// Routine to display bit pattern passed in the "ledByte", hold it for
// "patTime", then display the complement of the bit pattern and hold
// it for "compTime".

void CompPattern(uint8_t ledByte, const int bitToPin[8],
                 int onState, int patTime, int compTime)
{
  SetLEDByteState(ledByte, bitToPin, onState);
  WaitMillis(patTime);
  
  ledByte = ~ledByte;     // Complement bits

  SetLEDByteState(ledByte, bitToPin, onState);
  WaitMillis(compTime);
}

// Routine to sweep bit pattern passed in "ledByte" from LSB to MSB,
// holding each LED on/off for the specified times (in milliseconds).

void SweepLSBToMSB(uint8_t ledByte, const int bitToPin[8],
                   int onState, int onTime, int offTime)
{
  for ( uint8_t bit = 0; bit < 8; bit++ )
  {
    SetLEDByteState(ledByte, bitToPin, onState);
    WaitMillis(onTime);
    SetLEDByteState(0x00, bitToPin, onState);
    WaitMillis(offTime);

    ledByte = ledByte << 1;
  }  
}

// Routine to sweep bit pattern passed in "ledByte" from MSB to LSB,
// holding each LED on/off for the specified times (in milliseconds).

void SweepMSBToLSB(uint8_t ledByte, const int bitToPin[8],
                   int onState, int onTime, int offTime)
{
  for ( uint8_t bit = 0; bit < 8; bit++ )
  {
    SetLEDByteState(ledByte, bitToPin, onState);
    WaitMillis(onTime);
    SetLEDByteState(0x00, bitToPin, onState);
    WaitMillis(offTime);

    ledByte = ledByte >> 1;
  }  
}
