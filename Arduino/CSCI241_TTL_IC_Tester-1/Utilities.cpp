// Various TTL IC Tester utility routines.

#include "Globals.h"
#include "Utilities.h"
#include <CSCI_Console.h>     // Console utility routines
#include <CSCI_SysUtils.h>    // System-wide utility routines
#include <CSCI_Timers.h>      // Timer utility routines
#include <Arduino.h>

// *********** ZIF Socket Manipulation Routines *******

// ---------------------------------------------------------
void SetICPowerPins(int pinGND, int pinVCC)
{
  // As an internal consistency check, make sure the programmer has
  // passed the correct ground pin for the type of IC (14, 16 or 20
  // pin) specified.

  switch ( NumICPins )
  {
    case Num_14PinIC_Pins:
    if ( pinGND != IC14GNDPin )
    {
      InternalError(__FILE__, __LINE__);
    }
    break;

    case Num_16PinIC_Pins:
    if ( pinGND != IC16GNDPin )
    {
      InternalError(__FILE__, __LINE__);
    }    
    break;

    case Num_20PinIC_Pins:
    if ( pinGND != IC20GNDPin )
    {
      InternalError(__FILE__, __LINE__);
    }    
    break;

    default:
      InternalError(__FILE__, __LINE__);
  }

  if ( pinVCC != 14 )
  {
    InternalError(__FILE__, __LINE__);
  }
  
  // Set VCC pin as an IC "INPUT" pin.

  SetICPinAsInput(pinVCC);
  
  // Apply Vcc via relay

  SetICInputHigh(pinVCC);

  // Wait for relay to close and debounce.

  WaitMillis(500);
}

// ---------------------------------------------------------
void SetICInputs(const int icInputs[], int numICPins)
{
  for ( int index = 0; index < numICPins; index++ )
  {
    int icPin = index + 1;        // IC pins are numbered (1,2,3,...)
    int state = icInputs[index];

    if ( state != SKIP )
    {
      SetICPinAsInput(icPin);

      if ( state == HIGH )
      {
        SetICInputHigh(icPin);
      }

      if ( state == LOW )
      {
        SetICInputLow(icPin); 
      }
    }
  }
}

// ---------------------------------------------------------
void CheckICOutputs(const int icOutputs[], int numICPins)
{
  for ( int index = 0; index < numICPins; index++ )
  {
    int icPin = index + 1;        // IC pins are numbered (1,2,3,...)
    int expectedState = icOutputs[index];

    if ( expectedState != SKIP )
    {
      // If the actual state is NOT the expected state...

      int icPinState = ReadICOutput(icPin);

      if ( icPinState != expectedState )
      {
        // Report failure information...

        String msg("*** Failure: IC pin ");
        msg += String(icPin) + " was " + GetStateName(icPinState) +
               ", expected " + GetStateName(expectedState);

        DisplayNewline();
        DisplayString(msg);
        DisplayNewline();

//      DisplayPause();   // Uncomment to pause after each error detected.
      }
    }
  }
}

// ---------------------------------------------------------
void SetICPinAsInput(int pin)
{
  pinMode(MapICPinToGPIO(pin), OUTPUT);
}

// ---------------------------------------------------------
void SetICPinAsOutput(int pin)
{
  pinMode(MapICPinToGPIO(pin), INPUT);
}

// ---------------------------------------------------------
void SetICInputHigh(int pin)
{
  digitalWrite(MapICPinToGPIO(pin), HIGH);
}

// ---------------------------------------------------------
void SetICInputLow(int pin)
{
  digitalWrite(MapICPinToGPIO(pin), LOW);
}

// ---------------------------------------------------------
int ReadICOutput(int pin)
{
  return digitalRead(MapICPinToGPIO(pin));
}

// ---------------------------------------------------------
int MapICPinToGPIO(int icPin)
{
  // Assume no associated GPIO pin...
  
  int gpioPin = -1;
  
  // If the IC has 14-pins...

  if ( NumICPins == Num_14PinIC_Pins )
  {
    // If the icPin number is NOT within valid range...

    if ( ( icPin < 1 ) || ( icPin > Num_14PinIC_Pins ) )
    {
      InternalError(__FILE__, __LINE__); 
    }

    gpioPin = ZIFPinToGPIO[ IC14PinToZIFPin[icPin] ];
  }
  else
  {
    // If the IC has 16-pins...

    if ( NumICPins == Num_16PinIC_Pins )
    {
      // If the icPin number is NOT within valid range...
  
      if ( ( icPin < 1 ) || ( icPin > Num_16PinIC_Pins ) )
      {
        InternalError(__FILE__, __LINE__); 
      }

      gpioPin = ZIFPinToGPIO[ IC16PinToZIFPin[icPin] ];      
    }
    else
    {
      if ( NumICPins == Num_20PinIC_Pins )
      {
        // If the icPin number is NOT withing valid range...

        if ( ( icPin < 1 ) || ( icPin > Num_20PinIC_Pins ) )
        {
          InternalError(__FILE__, __LINE__);
        }

        gpioPin = ZIFPinToGPIO[ IC20PinToZIFPin[icPin] ];
      }
      else
      {
        InternalError(__FILE__, __LINE__);
      }
    }
  }

  // If there's no GPIO pin associated with this ZIF pin...

  if ( gpioPin == SKIP )
  {
    InternalError(__FILE__, __LINE__);
  }

  return gpioPin;
}


// *************** Misc Utility Routines **************

// ---------------------------------------------------------
int InputICNumber()
{
  // Prompt for a valid IC number we know how to test.

  bool icNumberValid = false;   // Assume we can't handle it.
  int icNumber = 0;

  while ( !icNumberValid )
  {
    DisplayNewline();
    DisplayString("Enter IC to test (7400, 7404, etc.): ");
    String ICName = KeyboardGetline();
  
    icNumber = ICName.toInt();
  
    switch ( icNumber )
    {
      case 7400:
      case 7402:
      case 7403:
      case 7404:
      case 7405:
      case 7406:
      case 7407:
      case 7408:
      case 7410:
      case 7411:
      case 7432:
      case 7486:
      icNumberValid = true;   // Yes, we can handle.
      break;
  
      default:
      {
        String errMsg("Don't know how to test a ");
        errMsg += ( "'" + ICName + "' IC!" );
        DisplayError(errMsg);
        DisplayNewline();
      }
    }
  }

  return icNumber;
}

// ---------------------------------------------------------
void ZIFPinSetup(int inputMode)
{
  for ( int zifPin = 1; zifPin <= Num_ZIFPins; zifPin++ )
  {
    // If there's a GPIO pin associated with this ZIF pin...

    int gpioPin = ZIFPinToGPIO[zifPin];

    if ( gpioPin != SKIP )
    {
      pinMode(gpioPin, inputMode);
    }
  }

  NumICPins = 0;    // Don't (yet) know how many pins IC has.
}

// ---------------------------------------------------------
void DisplayTestInfo(const String& msg)
{
  DisplayNewline();
  DisplayString(msg);
  DisplayNewline();
}
