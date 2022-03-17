// Test TTL ICs in a DIP (Dual Inline Package)

#include "Test_AND_Gates.h"
#include "Test_OR_Gates.h"
#include "Test_Inverter_Gates.h"
#include "Utilities.h"
#include <CSCI_Console.h>         // Console utility routines
#include <CSCI_SysUtils.h>        // System-wide utility routines

// *** IMPORTANT ***
//
// The IC to be tested is inserted into a Zero Insertion
// Force (ZIF) socket and then the IC number (7400, 7404,
// 7432, etc.) is entered into the program.
//
// Note: Only ICs where ZIF Socket pin 20 is IC Vcc can be
//       tested! This is by far the most common case.
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
//       16 pin ICs.

// *** This routine is called once at start up ***

void setup()
{
  ConsoleSetup();
  ZIFPinSetup(INPUT);
}

// *** This routine called repeatedly until RESET or power off ***

void loop()
{
  // Prompt the user for the IC number to test.

  int ICNumber = InputICNumber();

  // Perform appropriate test for this IC...

  switch ( ICNumber )
  {
    case 7400:
    TestSN7400();
    break;

    case 7402:
    TestSN7402();
    break;    

    case 7403:
    TestSN7403();
    break;
    
    case 7404:
    TestSN7404();
    break;    

    case 7405:
    TestSN7405();
    break;

    case 7406:
    TestSN7406();
    break;

    case 7407:
    TestSN7407();
    break;        

    case 7408:
    TestSN7408();
    break;

    case 7410:
    TestSN7410();
    break;    

    case 7411:
    TestSN7411();
    break;    

    case 7432:
    TestSN7432();
    break;              

    case 7486:
    TestSN7486();
    break; 

    default:      // Should never get here!
    {
      InternalError(__FILE__, __LINE__);
    }
  }

  DisplayNewline();
  DisplayString("Test completed!");
  DisplayNewline();

  // For safety, stop all current flow through the IC.

  ZIFPinSetup(INPUT);
}
