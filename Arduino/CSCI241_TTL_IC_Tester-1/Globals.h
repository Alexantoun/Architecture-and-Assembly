#ifndef INCLUDE_GLOBALS_H
#define INCLUDE_GLOBALS_H

#include <Arduino.h>

// Global definitions for TTL IC Tester program

// The 20-pin ZIF (Zero Insertion Force) socket is oriented so pin 1 is
// at the top right, pin 10 is at the top left, pin 11 is at the bottom
// left and pin 20 is at the bottom right.

// Define constant used in pin mapping arrays that means to "skip" the value.
// In general, this constant needs to be negative (since the other array
// members are positive integers.

 const int SKIP = -1;

// This array maps the ZIF pin numbers (1,2,3,...,20) to their corresponding
// Arduino GPIO pin numbers.  ZIF pins not connected have negative GPIO pin values,
//
// Note: ZIF pin 20 is assumed to be the +5 volt Vcc power pin on all ICs.
//       The corresponding GPIO pin controls a relay that switches the Arduino
//       Vcc power rail to ZIF pin 20 so the IC can draw more power than the
//       40 ma per GPIO pin limit.
//
//       There is a pair of rocker switches on the breadboard
//       which allow connecting ZIF socket pin 7 to ground
//       (for 14 pin ICs) or to a GPIO pin (for 16 and 20 pin ICs).
//
//       There is another pair of rocker switches on the
//       breadboard which allow connecting ZIF socket pin 8 to
//       ground (for 16 pin ICs) or to a GPIO pin (for 20 pin ICs).
//
//       Pin 10 is always connected to ground (for testing
//       20 pin ICs) and, obviously, isn't needed for 14 and
//       16 pin ICs).

const int ZIFVccPin = 20;

// ZIF socket pin to Arduino GPIO mapping array.  Index by ZIF pin number
// (1,2,3,...,20).

const int ZIFPinToGPIO[] =
{
  SKIP,                         // No ZIF pin 0
  40, 39, 38, 37, 36, 35, 34,   // ZIF pins 1 thru 7.
  33, 32, SKIP,                 // ZIF pins 8, 9, 10
  23, 24, 25,                   // ZIF pins 11, 12, 13
  26, 27, 28, 29, 30, 31, 22    // ZIF pins 13 thru 20 (Pin 20 is Vcc relay control.)
};

const int Num_ZIFPins = ( sizeof(ZIFPinToGPIO) / sizeof(ZIFPinToGPIO[0]) ) - 1;

// This array maps 14-pin (1,2,3,...14) IC numbers to ZIF socket numbers.

const int IC14PinToZIFPin[] =
{
  SKIP,                         // No IC pin 0
  1, 2, 3, 4, 5, 6, 7,          // "Pin 1" side of 14-pin IC
  14, 15, 16, 17, 18, 19, 20    // "Pin 14" side of 14 pin IC
};

const int Num_14PinIC_Pins = sizeof(IC14PinToZIFPin) / sizeof(IC14PinToZIFPin[0]) - 1;

const int IC14GNDPin = 7;

// This array maps 16-pin (1,2,3,...16) IC numbers to ZIF socket numbers.

const int IC16PinToZIFPin[] =
{
  SKIP,                             // No IC pin 0
  1, 2, 3, 4, 5, 6, 7, 8,           // "Pin 1" side of 16-pin IC
  13, 14, 15, 16, 17, 18, 19, 20    // "Pin 16" side of 16 pin IC
};

const int Num_16PinIC_Pins = sizeof(IC16PinToZIFPin) / sizeof(IC16PinToZIFPin[0]) - 1;

const int IC16GNDPin = 8;

// This array maps 20-pin (1,2,3,...20) IC numbers to ZIF socket numbers.

const int IC20PinToZIFPin[] =
{
  SKIP,                                   // No IC pin 0
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,          // "Pin 1" side of 20-pin IC
  11, 12, 13, 14, 15, 16, 17, 18, 19, 20  // "Pin 20" side of 20-pin IC
};

const int Num_20PinIC_Pins = sizeof(IC20PinToZIFPin) / sizeof(IC20PinToZIFPin[0]) - 1;

const int IC20GNDPin = 10;

// Each IC test routine should set this global to the number of pins]
// (14, 16 or 20) on the IC being tested.

extern int NumICPins;

#endif INCLUDE_GLOBALS_H
