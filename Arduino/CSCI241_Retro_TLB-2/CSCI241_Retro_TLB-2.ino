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

const char PROGVER[] = "V0.9";

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
//   ram1_bytes  ;A 16384 byte array containing first half of 6502 ram data
//   ram2_bytes  ;A 16384 byte array containing second half of 6502 ram data
//
//   rom1_bytes  ;A 16384 byte array containing first half of 6502 rom data
//   rom2_bytes  ;A 16384 byte array containing second half of 6502 rom data
//
// We have to break the RAM and ROM data stored in flash memory into two
// 16K arrays, because Arduino C/C++ doesn't allow an array (or any object)
// to be larger than 32767 bytes, which is one byte too small!

#include "memmap.h"

// This sram_bytes array contains as much 6502 RAM data as allowed in the
// Arduino.  Use approximately 6.5K of the Arduino Mega's 8K bytes of SRAM.
// By default, sram_bytes is not ininitalized on an Arduino reset.  On reset,
// the user may optionally copy the initial ram contents from flash into SRAM.

const uint16_t SRAM_START = 0x0000;
const uint16_t SRAM_END   = 0x19FF;

byte  sram_bytes[SRAM_END - SRAM_START + 1]
        __attribute__((section(".noinit")));

////////////////////////////////////////////////////////////////////
// 65c02 Processor Control
////////////////////////////////////////////////////////////////////

/* Digital Pin Assignments */
#define DATA_OUT PORTL
#define DATA_IN  PINL
#define ADDR_H   PINC
#define ADDR_L   PINA
#define ADDR     ((unsigned int) (ADDR_H << 8 | ADDR_L))

#define uP_RESET_N  38
#define uP_RW_N     40
#define uP_RDY      39
#define uP_SO_N     41
#define uP_IRQ_N    50
#define uP_NMI_N    51
#define uP_E        52
#define uP_GPIO     53

// Fast routines to drive clock signals high/low; faster than digitalWrite
// required to meet >100kHz clock freq for 6809e.
// 6502 & z80 do not have this requirement.
//
#define CLK_Q_HIGH  (PORTB = PORTB | 0x01)
#define CLK_Q_LOW   (PORTB = PORTB & 0xFE)
#define CLK_E_HIGH  (PORTB = PORTB | 0x02)
#define CLK_E_LOW   (PORTB = PORTB & 0xFD)
#define STATE_RW_N  (PING & 0x02)

#define DIR_IN  0x00
#define DIR_OUT 0xFF
#define DATA_DIR   DDRL
#define ADDR_H_DIR DDRC
#define ADDR_L_DIR DDRA

byte uP_DATA;

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
// Processor Control Loop
////////////////////////////////////////////////////////////////////

// This is where the action is.
// It reads processor control signals and acts accordingly.

inline __attribute__((always_inline))
void cpu_tick()
{ 
  register int uP_ADDR;
  
  CLK_E_HIGH;    // E goes high   // digitalWrite(uP_E, HIGH);

  uP_ADDR = ADDR;

  if (STATE_RW_N)	  
  //////////////////////////////////////////////////////////////////
  // HIGH = READ
  {
    // change DATA port to output to uP:
    DATA_DIR = DIR_OUT;

    // ROM? (Any address >= 0x8000 is assumed to be in ROM.)
      
    if ( uP_ADDR & 0x8000 )
    {
      // Determine which half of ROM address space to fetch data from.

      if ( uP_ADDR & 0x4000 )
      {
        // If NOT reading input port 0...
    
        if ( TLBIO_0 != uP_ADDR )
        {        
          // Read from second half of ROM address space.
            
          DATA_OUT = __LPM( (uint16_t) (rom2_bytes + (uP_ADDR & 0x3FFF)) );
        }
        else
        {
          // Any keyboard data?
    
          if ( Serial.available() )
          {
            DATA_OUT = Serial.read() & 0x7F;  // Fetch ASCII char code
          }
          else
          {
            DATA_OUT = 0x00;  // No keyboard data available.
          }
        }
      }
      else
      {
        // Read from first half of ROM address space.
            
        DATA_OUT = __LPM( (uint16_t) (rom1_bytes + (uP_ADDR & 0x3FFF)) );        
      }
    }
    else
    {
      // SRAM - (If addr is not in ROM, assume it's in SRAM.)
      //        Assume SRAM starts at 0x0000.
    
      DATA_OUT = *(sram_bytes + uP_ADDR);
    }     
  }
  else 
  //////////////////////////////////////////////////////////////////
  // R/W = LOW = WRITE
  {
    // If NOT writing to output port 1...

    if ( TLBIO_1 != uP_ADDR )
    {
      // SRAM - Assume we're writing to SRAM.
      //        Assume SRAM starts at 0x0000.

      *(sram_bytes + uP_ADDR) = DATA_IN;

      // *** NOTE:  Comment out the previous statement and enable the
      //            code below to verify we're writing to a valid SRAM
      //            address.  Useful for debugging 6502 programs.

      /*
      if ( uP_ADDR <= SRAM_END )
      {
        *(SRAM + uP_ADDR) = DATA_IN;
      }
      else
      {
        Serial.println();
        Serial.print(F("Invalid write address = "));
        Serial.println(uP_ADDR, HEX);
        while (true)
        { }
      }
      */
    }
    else
    {
      Serial.write(DATA_IN & 0x7F); // Display ASCII char on console.      
    }
  }

  //////////////////////////////////////////////////////////////////

  // start next cycle
  CLK_E_LOW;    // E goes low

  // natural delay for DATA Hold time (t_HR)
  DATA_DIR = DIR_IN;
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
   
  // Initialize 6502 microprocessor GPIO's
    
  uP_init();
  
  // Reset processor for 25 cycles
  
  uP_assert_reset();
  
  for(int i = 1; i <= 25; i++)
  {
    cpu_tick();
  }
  
  uP_release_reset();

  Serial.println(F("\n"));
}


////////////////////////////////////////////////////////////////////
// Loop()
////////////////////////////////////////////////////////////////////

void loop()
{
  // Loop forever
 
  while (true)
  {
    cpu_tick();
  }
}
