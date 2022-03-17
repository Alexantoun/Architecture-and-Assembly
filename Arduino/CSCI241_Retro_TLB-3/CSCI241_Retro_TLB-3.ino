////////////////////////////////////////////////////////////////////
// RetroShield 6502 for Arduino Mega
// TLB Homebrew Computer
//
// Hardware is the RetroShield 6502 for Arduino Mega,
// Copyright (c) 2019 Erturk Kocalar, 8Bitforce.com
// 
// Original software released in 2019 by Erturk Kocalar under MIT license.
// Software massively rewritten by Wayne Wall to simulate the TLB computer,
// a homebrew computer circa 1977.

// Version History
// 
// V0.1   06/09/2021  // Removed SPI_RAM, LCD keyboard and output debug code.
// V0.2   06/09/2021  // Removed Apple audio cassette code.
// V0.3   06/09/2021  // Removed Integer Basic code.
// V0.4   06/09/2021  // Removed 6821 peripheral chip code.
// V0.5   06/09/2021  // Replced Woz Monitor with DDT.
// V0.6   06/10/2021  // First attempt to make cpu_tick() routine faster.
// V0.7   06/10/2021  // Preserve microprocessor RAM contents across reset.
// V0.8   06/11/2021  // Supports "memmap.h" containing 6502 memory contents.
// V0.9   06/13/2021  // Second attempt to make cpu_tick() routine faster.
// V1.0   06/14/2021  // Rewrite of cpu_tick() in assembly language.

const char PROGVER[] = "V1.0";

// Include Arduino GPIO definitions related to 6502 cpu control.

#include "6502_iodefs.h"

// External assembly language globals.

extern "C"
{
  // Assembly language subroutines called from C code.

  void reset_tick();    // Perform one 6502 clock tick during "reset"
  void cpu_tick_loop(); // Enter infinite 6502 clock tick loop.
                        //   (This routine does NOT return.)

  // C code subroutines called from assembly language code.

  uint8_t SerialRead();
  void SerialWrite(uint8_t chr);
  
  void DisplayAddress(uint16_t addr);
  void DisplayData(uint8_t data);
};

////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////

// TLB I/O addresses

const uint16_t TLBIO_0 = 0xEC00;  // Input = Keyboard
const uint16_t TLBIO_1 = 0xEC01;  // Output = Console display
const uint16_t TLBIO_2 = 0xEC02;
const uint16_t TLBIO_3 = 0xEC03;

////////////////////////////////////////////////////////////////////
// 6502 DEFINITIONS
////////////////////////////////////////////////////////////////////

// 6502 HW CONSTRAINTS
// 1- RESET_N must be asserted at least 2 clock cycles.
// 2- CLK can not be low more than 5 microseconds.  Can be high indefinitely.
//

// TLB 6502 MEMORY LAYOUT

// 32K RAM
//
// IMPORTANT: Other code assumes RAM begins at 0x0000 and spans
//            32768 addresses.

const uint16_t RAM_START = 0x0000;
const uint16_t RAM_END   = 0x7FFF;

// 32K ROM 
//
// IMPORTANT: Other code assumes ROM begins at 0x8000 and spans
//            32788 addresses.

const uint16_t ROM_START = 0x8000;
const uint16_t ROM_END   = 0xFFFF;

// Include file which contains the following four arrays that will be
// stored in Arduino flash memory:
//
//   ram1_bytes  ;A 16384 byte array with initial first half of 6502 ram data
//   ram2_bytes  ;A 16384 byte array with initial second half of 6502 ram data
//
//   rom1_bytes  ;A 16384 byte array with first half of 6502 rom data
//   rom2_bytes  ;A 16384 byte array with second half of 6502 rom data
//
// We have to break the RAM and ROM data stored in flash memory into two
// 16K arrays, because Arduino C/C++ doesn't allow an array (or any object)
// to be larger than 32767 bytes, which is one byte too small!

#include "memmap.h"

// Store pointers to 6502 ROM sections so they are accessible by the
// assembly code and also so the C compiler won't optimize them away.

const unsigned char *ptr_rom1_bytes = rom1_bytes;
const unsigned char *ptr_rom2_bytes = rom2_bytes;

// This sram_bytes array contains as much 6502 RAM data as allowed in the
// Arduino.  Use approximately 6.5K of the Arduino Mega's 8K bytes of SRAM.
// By default, sram_bytes is not ininitalized on an Arduino reset.  On reset,
// the user may optionally copy the initial ram contents from flash into SRAM.

const uint16_t SRAM_START = 0x0000;
const uint16_t SRAM_END   = 0x19FF;

byte  sram_bytes[SRAM_END - SRAM_START + 1]
        __attribute__((section(".noinit")));

void uP_assert_reset()
{
  // Drive RESET conditions
  
  digitalWrite(uP_RESET_N, LOW);
  digitalWrite(uP_IRQ_N, HIGH);
  digitalWrite(uP_NMI_N, HIGH);
  digitalWrite(uP_RDY, HIGH);
  digitalWrite(uP_SO_N, HIGH);
}

void uP_release_reset()
{
  // Drive RESET conditions
  
  digitalWrite(uP_RESET_N, HIGH);
}

// 6502 microprocessor initialization routines.

void uP_init()
{
  // Set directions
  
  DATA_DIR = DIR_IN;
  ADDR_H_DIR = DIR_IN;
  ADDR_L_DIR = DIR_IN;
  
  pinMode(uP_RESET_N, OUTPUT);
  pinMode(uP_RW_N,    INPUT);
  pinMode(uP_RDY,     OUTPUT);
  pinMode(uP_SO_N,    OUTPUT);
  pinMode(uP_IRQ_N,   OUTPUT);
  pinMode(uP_NMI_N,   OUTPUT);
  pinMode(uP_E,       OUTPUT);
  pinMode(uP_GPIO,    OUTPUT);
  
  uP_assert_reset();
  
  digitalWrite(uP_E, LOW);
  digitalWrite(uP_GPIO, HIGH);
}

////////////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////////////

void setup() 
{
  Serial.begin(115200);     // Use fast baud rate

  Serial.write(27);         // ESC command
  Serial.print(F("[2J"));   // Clear screen command
  Serial.write(27);
  Serial.print(F("[H"));
  Serial.write(27);         // ESC command
  Serial.print(F("[?25l")); // Hide cursor command
  Serial.println(F("\n"));
  Serial.print(F("Configuration: "));
  Serial.println(PROGVER);
  Serial.println(F("-------------------"));
  Serial.print(F("SRAM Size:  ")); Serial.print(SRAM_END - SRAM_START + 1, DEC);
  Serial.println(F(" Bytes"));
  Serial.print(F("SRAM_START: 0x")); Serial.println(SRAM_START, HEX); 
  Serial.print(F("SRAM_END:   0x")); Serial.println(SRAM_END, HEX); 
  Serial.println();
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("TLB Homebrew Computer"));
  Serial.println(F("Copyright (c) 2021 by T. Wayne Wall"));
  Serial.println(F("----------------------------------------------------"));
  Serial.println();
  Serial.println(F("Hit 'r' key to initialize 6502 RAM contents."));
  Serial.println(F("Hit any other key to leave 6502 RAM contents as is."));
  Serial.println(F("A 6502 reset will then be perfomed."));

  int key_char;
  
  while ( ( key_char = Serial.read() ) < 0 )
    { }

  // If we are to initialize 6502 RAM contents...

  if ( key_char == 'r' )
  {
    // *** NOTE:  Since the max SRAM on an Arduino Mega is 8K bytes, there's
    //            no way we need to copy bytes from the second half of ram
    //            contents stored in flash memory.

    for ( uint16_t sram_addr = SRAM_START; sram_addr <= SRAM_END; sram_addr++ )
    {
      sram_bytes[sram_addr] = pgm_read_byte_near(ram1_bytes + sram_addr);
    }
  }

  // Initialize 6502 microprocessor GPIO control pin state.
    
  uP_init();
  
  // Reset processor for 25 cycles
  
  uP_assert_reset();

  for(int i = 1; i <= 25; i++)
  {
    reset_tick();
  }
  
  uP_release_reset();
   
  Serial.println(F("\n"));
}


////////////////////////////////////////////////////////////////////
// Loop()
////////////////////////////////////////////////////////////////////

void loop()
{
  // Enter infinite 6502 master clock tick loop.  Does NOT return.
  
  cpu_tick_loop();
}

// Functions called from assembly language code.

// Debugging functions.  (Only called when debugging assembly code.)

// Display 16-bit address value

void DisplayAddress(uint16_t addr)
{
  Serial.print("addr = ");
  Serial.print(addr, HEX);
  Serial.println();
  delay(10);
}

// Display 8-bit data value

void DisplayData(uint8_t data)
{
  Serial.print("data = ");
  Serial.print(data, HEX);
  Serial.println();
  delay(10);
}

// Function to read a character from serial input.
// Returns zero if no character available.
// Otherwise returns the non-zero ASCII code for the character.

uint8_t SerialRead()
{
  int chr = Serial.read();

  if ( chr < 0 )
  {
    return 0;
  }
  else
  {
    return chr;
  }
}

// Function to send the character passed in.

void SerialWrite(uint8_t chr)
{
  Serial.write(chr);
}
