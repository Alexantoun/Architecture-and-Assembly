// Blink an 8-bit port's worth of LEDs.

#include <CSCI_LED.h>     // Library routines
#include <CSCI_Timers.h>

// Port-related definitions.

const uint8_t *PortDirPtr = &DDRL;    // Ptr to port direction control.
const uint8_t *PortDataPtr = &PORTL;  // Ptr to port data.

// State-related definitions

const int LedOnState = LOW;   // State which turns LED on.
const int LedOffState = HIGH; // State which turns LED off.

const int LedOnTime = 400;    // LED on time (in milliseconds).
const int LedOffTime = 600;   // LED off time (in milliseconds).

// This routine called once at program start.

void setup()
{
  SetupLEDPort(PortDirPtr); // Set port direction bits
}

// This routine called repeatedly until a "reset" is performed.

void loop ()
{
  uint8_t ledByte = 0b00000001;

  SweepLSBToMSB(ledByte, PortDataPtr, LedOnState, LedOnTime, LedOffTime);

  ledByte = 0b10000000;
  
  SweepMSBToLSB(ledByte, PortDataPtr, LedOnState, LedOnTime, LedOffTime);
  
  ledByte = 0b11110000;

  for ( int times = 1; times <= 8; times++ )
  {
    CompPattern(ledByte, PortDataPtr, LedOnState, LedOnTime, LedOffTime);
  }
  
  ledByte = 0b10101010;

  for ( int times = 1; times <= 8; times++ )
  {
    CompPattern(ledByte, PortDataPtr, LedOnState, LedOnTime, LedOffTime);
  }  
}

// Routine to display bit pattern passed in "ledByte", hold it for
// "patTime", then display the complement of the bit pattern and hold
// it for "compTime".

void CompPattern(uint8_t ledByte, uint8_t *portDataPtr,
                 int onState, int patTime, int compTime)
{
  SetLEDPortBits(portDataPtr, ledByte, onState);
  WaitMillis(patTime);
  
  ledByte = ~ledByte;     // Complement bits

  SetLEDPortBits(portDataPtr, ledByte, onState);
  WaitMillis(compTime);
}

// Routine to sweep bit pattern passed in "ledByte" from LSB to MSB,
// holding each LED on/off for the specified times (in milliseconds).

void SweepLSBToMSB(uint8_t ledByte, uint8_t *portDataPtr,
                   int onState, int onTime, int offTime)
{
  for ( uint8_t bit = 0; bit < 8; bit++ )
  {
    SetLEDPortBits(portDataPtr, ledByte, onState);
    WaitMillis(onTime);
    SetLEDPortBits(portDataPtr, 0x00, onState);
    WaitMillis(offTime);

    ledByte = ledByte << 1;
  }  
}

// Routine to sweep bit pattern passed in "ledByte" from MSB to LSB,
// holding each LED on/off for the specified times (in milliseconds).

void SweepMSBToLSB(uint8_t ledByte, uint8_t *portDataPtr,
                   int onState, int onTime, int offTime)
{
  for ( uint8_t bit = 0; bit < 8; bit++ )
  {
    SetLEDPortBits(portDataPtr, ledByte, onState);
    WaitMillis(onTime);
    SetLEDPortBits(portDataPtr, ledByte, onState);
    WaitMillis(offTime);

    ledByte = ledByte >> 1;
  }  
}
