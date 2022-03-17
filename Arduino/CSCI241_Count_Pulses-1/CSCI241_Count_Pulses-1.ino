//Authors: Alexander Antoun, Prof. Wayne Wall
#include "CSCI_GPIO.h"
#include "CSCI_LED.h"
#include "CSCI_PushButton.h"
#include "CSCI_Console.h"
#include "CSCI_SysUtils.h"

//Variables pertaining to LED's
const int LEDArray[8] = {49, 48, 47, 46, 45, 44, 43, 42};
const int LedOnState = LOW;
const int LedOffState = HIGH;

//Variables pertaining to auxiliaries
const int inPin = 2;
const int buttonA = 52;

//Variables for intermediate arithmetic
unsigned long int pulseCount = 0;
bool lowToHigh = false;

void setup() {
  SetupGPIOInputPin(inPin);
  SetupLEDByte(LEDArray);
  SetLEDByteState(0xFF, LEDArray, LedOffState);
  SetupButton(buttonA);
  ConsoleSetup();
}

void loop() {
    if(GetGPIOInputState(inPin)!= lowToHigh){
      if(lowToHigh == LOW)
        pulseCount++;
      lowToHigh = !lowToHigh;
    }
    SetLEDByteState(static_cast<uint8_t>(pulseCount), LEDArray, LedOnState);  

    if(IsButtonPressed(buttonA))
    {
      DisplayString("Total pulse count = ");
      DisplayInteger(pulseCount);
      HaltProgram();
    }
}
