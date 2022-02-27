// Programmer: Wayne Wall
// Description: Blink an LED

const int LedPin = LED_BUILTIN; // GPIO pin LED connected to.

const int LedOnState = HIGH;    // GPIO state which turns LED on.
const int LedOffState = LOW;    // GPIO state which turns LED off.

//-------------------------------------------------------------
// "setup" is called first after power up or reset.

void setup()
{
  SetupLed(LedPin);
}

//-------------------------------------------------------------
// "loop" is called repeatedly until power off or reset.

void loop()
{
  // LED on/off times (in milliseconds)
  
  static const int ON_TIME = 2000;
  static const int OFF_TIME = 4000;
  
  SetLedOn(LedPin, ON_TIME);
  SetLedOff(LedPin, OFF_TIME); 
}

//-------------------------------------------------------------
// Routine to turn the LED on (i.e., emit light)
// then wait a specified number of milliseconds.

void SetLedOn(int ledPin, size_t waitTime)
{
  SetOutputState(ledPin, LedOnState);
  WaitMillis(waitTime); 
}

//-------------------------------------------------------------
// Routine to turn the LED off (i.e., no light emitted)
// then wait a specified number of milliseconds.

void SetLedOff(int ledPin, size_t waitTime)
{
  SetOutputState(ledPin, LedOffState);
  WaitMillis(waitTime);
}

//-------------------------------------------------------------
// Set the state (HIGH/LOW) of a digital output pin.

void SetOutputState(int pin, int state)
{
  digitalWrite(pin, state);
}

//-------------------------------------------------------------
// Initialize the LED GPIO pin.

void SetupLed(int ledPin)
{
  pinMode(ledPin, OUTPUT);    // LED is an output device.
  SetLedOff(ledPin, 0);       // Make sure LED is in the "off" state.
}

//-------------------------------------------------------------
// Wait a specified number of milliseconds.

void WaitMillis(int waitTime)
{
  // Don't pass 0 to "delay" function, as there's no guarantee
  // it won't do something weird.
  
  if ( waitTime > 0 )
  {
    delay(waitTime);
  }
}
