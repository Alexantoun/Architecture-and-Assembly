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

const char PROGVER[] = "V0.3";

#include <DIO2.h>

////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// 65C02 DEFINITIONS
////////////////////////////////////////////////////////////////////

// 65C02 HW CONSTRAINTS
// 1- RESET_N must be asserted at least 2 clock cycles.
// 2- CLK can not be low more than 5 microseconds.  Can be high indefinitely.
//

// MEMORY LAYOUT

// 6K RAM

const uint16_t RAM_START = 0x0000;
const uint16_t RAM_END   = 0x17FF;

byte  RAM[RAM_END - RAM_START + 1];

// ROM 

const uint16_t ROM_START   = 0xFF00;
const uint16_t ROM_END     = 0xFFFF;

////////////////////////////////////////////////////////////////////
// Woz Monitor Code
////////////////////////////////////////////////////////////////////
// static const unsigned char 
PROGMEM const unsigned char rom_bin[] = {
// static const unsigned char rom_bin[] = {
    0xd8, 0x58, 0xa0, 0x7f, 0x8c, 0x12, 0xd0, 0xa9, 0xa7, 0x8d, 0x11, 0xd0, 
    0x8d, 0x13, 0xd0, 0xc9, 0xdf, 0xf0, 0x13, 0xc9, 0x9b, 0xf0, 0x03, 0xc8, 
    0x10, 0x0f, 0xa9, 0xdc, 0x20, 0xef, 0xff, 0xa9, 0x8d, 0x20, 0xef, 0xff, 
    0xa0, 0x01, 0x88, 0x30, 0xf6, 0xad, 0x11, 0xd0, 0x10, 0xfb, 0xad, 0x10, 
    0xd0, 0x99, 0x00, 0x02, 0x20, 0xef, 0xff, 0xc9, 0x8d, 0xd0, 0xd4, 0xa0, 
    0xff, 0xa9, 0x00, 0xaa, 0x0a, 0x85, 0x2b, 0xc8, 0xb9, 0x00, 0x02, 0xc9, 
    0x8d, 0xf0, 0xd4, 0xc9, 0xae, 0x90, 0xf4, 0xf0, 0xf0, 0xc9, 0xba, 0xf0, 
    0xeb, 0xc9, 0xd2, 0xf0, 0x3b, 0x86, 0x28, 0x86, 0x29, 0x84, 0x2a, 0xb9, 
    0x00, 0x02, 0x49, 0xb0, 0xc9, 0x0a, 0x90, 0x06, 0x69, 0x88, 0xc9, 0xfa, 
    0x90, 0x11, 0x0a, 0x0a, 0x0a, 0x0a, 0xa2, 0x04, 0x0a, 0x26, 0x28, 0x26, 
    0x29, 0xca, 0xd0, 0xf8, 0xc8, 0xd0, 0xe0, 0xc4, 0x2a, 0xf0, 0x97, 0x24, 
    0x2b, 0x50, 0x10, 0xa5, 0x28, 0x81, 0x26, 0xe6, 0x26, 0xd0, 0xb5, 0xe6, 
    0x27, 0x4c, 0x44, 0xff, 0x6c, 0x24, 0x00, 0x30, 0x2b, 0xa2, 0x02, 0xb5, 
    0x27, 0x95, 0x25, 0x95, 0x23, 0xca, 0xd0, 0xf7, 0xd0, 0x14, 0xa9, 0x8d, 
    0x20, 0xef, 0xff, 0xa5, 0x25, 0x20, 0xdc, 0xff, 0xa5, 0x24, 0x20, 0xdc, 
    0xff, 0xa9, 0xba, 0x20, 0xef, 0xff, 0xa9, 0xa0, 0x20, 0xef, 0xff, 0xa1, 
    0x24, 0x20, 0xdc, 0xff, 0x86, 0x2b, 0xa5, 0x24, 0xc5, 0x28, 0xa5, 0x25, 
    0xe5, 0x29, 0xb0, 0xc1, 0xe6, 0x24, 0xd0, 0x02, 0xe6, 0x25, 0xa5, 0x24, 
    0x29, 0x07, 0x10, 0xc8, 0x48, 0x4a, 0x4a, 0x4a, 0x4a, 0x20, 0xe5, 0xff, 
    0x68, 0x29, 0x0f, 0x09, 0xb0, 0xc9, 0xba, 0x90, 0x02, 0x69, 0x06, 0x2c, 
    0x12, 0xd0, 0x30, 0xfb, 0x8d, 0x12, 0xd0, 0x60, 0x00, 0x00, 0x00, 0x0f, 
    0x00, 0xff, 0x00, 0x00
};

////////////////////////////////////////////////////////////////////
// 6821 Peripheral
// emulate just enough so keyboard/display works thru serial port.
////////////////////////////////////////////////////////////////////
//

const uint16_t KBD   = 0xd010;
const uint16_t KBDCR = 0xd011;
const uint16_t DSP   = 0xd012;
const uint16_t DSPCR = 0xd013;

byte regKBD;
byte regKBDDIR;    // Dir register when KBDCR.bit2 == 0
byte regKBDCR;

byte regDSP;
byte regDSPDIR;    // Dir register when DSPCR.bit2 == 0
byte regDSPCR;

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

unsigned long uP_start_millis;
unsigned long uP_stop_millis;
unsigned long uP_millis_last;
unsigned int  uP_ADDR;

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
  
  uP_start_millis = millis();
}

void m6821_init()
{
  regKBD    = 0x00;
  regKBDDIR = 0x00;
  regKBDCR  = 0x00;
  regDSP    = 0x00;
  regDSPDIR = 0x00;
  regDSPCR  = 0x00;
}

////////////////////////////////////////////////////////////////////
// Processor Control Loop
////////////////////////////////////////////////////////////////////

// This is where the action is.
// it reads processor control signals and acts accordingly.

inline __attribute__((always_inline))
void cpu_tick()
{ 
  int ch;
  
  CLK_E_HIGH;    // E goes high   // digitalWrite(uP_E, HIGH);

  uP_ADDR = ADDR;
    
  if (STATE_RW_N)	  
  //////////////////////////////////////////////////////////////////
  // HIGH = READ
  {
    // change DATA port to output to uP:
    DATA_DIR = DIR_OUT;
    
    // ROM?
    if ( (ROM_START <= uP_ADDR) && (uP_ADDR <= ROM_END) )
      DATA_OUT = pgm_read_byte_near(rom_bin + (uP_ADDR - ROM_START));
    else
    // RAM?
    if ( (uP_ADDR <= RAM_END) && (RAM_START <= uP_ADDR) )
      // Use Arduino RAM for stack/important stuff
      DATA_OUT = RAM[uP_ADDR - RAM_START];
    else
    // 6821?
    if ( KBD <=uP_ADDR && uP_ADDR <= DSPCR )   
    {      
      // KBD?
      if (uP_ADDR == KBD)
      {
        if (regKBDCR & 0x02)
          // KBD register  
          {
            DATA_OUT = regKBD;
            regKBDCR = regKBDCR & 0x7F;    // clear IRQA bit upon read
          }
        else
          DATA_OUT = regKBDDIR;
      }
      else
      // KBDCR?
      if (uP_ADDR == KBDCR)
      {
        // KBDCR register
        DATA_OUT = regKBDCR;  
      }
      else
      // DSP?
      if (uP_ADDR == DSP)
      {
        if (regDSPCR & 0x02) 
          // DSP register  
          {
            DATA_OUT = regDSP;
            regDSPCR = regDSPCR & 0x7F;    // clear IRQA bit upon read
          }
        else
          DATA_OUT = regDSPDIR;
      }
      else
      // DSPCR?
      if (uP_ADDR == DSPCR)
      {
        // DSPCR register
        DATA_OUT = regDSPCR;  
      }   
      
    }
  } 
  else 
  //////////////////////////////////////////////////////////////////
  // R/W = LOW = WRITE
  {
    // RAM?
    if ( (uP_ADDR <= RAM_END) && (RAM_START <= uP_ADDR) )
      // Use Arduino RAM for stack/important stuff
      RAM[uP_ADDR - RAM_START] = DATA_IN;
    else
    // 6821?
    if ( KBD <=uP_ADDR && uP_ADDR <= DSPCR )
    {
      // KBD?
      if (uP_ADDR == KBD)
      {
        if (regKBDCR & 0x02)
          // KBD register
          {  
            regKBD = DATA_IN;
          }
        else
          regKBDDIR = DATA_IN;
      }
      else
      // KBDCR?
      if (uP_ADDR == KBDCR)
      {
        // KBDCR register
        regKBDCR = DATA_IN & 0X7F;  
      }
      else
      // DSP?
      if (uP_ADDR == DSP)
      {
        if (regDSPCR & 0x02)
          // DSP register
          if (DATA_IN == 0x8D)
            Serial.write("\r\n");    // send CR / LF
          else
            Serial.write(regDSP = DATA_IN & 0x7F);
        else
          regDSPDIR = DATA_IN;  
      }
      else
      // DSPCR?
      if (uP_ADDR == DSPCR)
      {
        // DSPCR register
        regDSPCR = DATA_IN;  
      }
    }
  }

  //////////////////////////////////////////////////////////////////

  // start next cycle
  CLK_E_LOW;    // E goes low

  // natural delay for DATA Hold time (t_HR)
  DATA_DIR = DIR_IN;
}

////////////////////////////////////////////////////////////////////
// Serial Event
////////////////////////////////////////////////////////////////////

/*
 SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
 
inline __attribute__((always_inline))
void serialEvent0() 
{
  if (Serial.available())
    if ((regKBDCR & 0x80) == 0x00)      // read serial byte only if we can set 6821 interrupt
    {
      cli();                            // stop interrupts while changing 6821 guts.
      // 6821 portA is available      
      int ch = toupper( Serial.read() );    // apple1 expects upper case
      regKBD = ch | 0x80;               // apple1 expects bit 7 set for incoming characters.
      regKBDCR = regKBDCR | 0x80;       // set 6821 interrupt
      sei();
    }
  return;
}



////////////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////////////

void setup() 
{
  Serial.begin(115200);

  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");
  Serial.println("\n");
  Serial.print("Configuration: ");
  Serial.println(PROGVER);
  Serial.println("-------------------");
  Serial.print("SRAM Size:  "); Serial.print(RAM_END - RAM_START + 1, DEC); Serial.println(" Bytes");
  Serial.print("SRAM_START: 0x"); Serial.println(RAM_START, HEX); 
  Serial.print("SRAM_END:   0x"); Serial.println(RAM_END, HEX); 
  Serial.println("");
  Serial.println("----------------------------------------------------");
  Serial.println("> WOZ Monitor");
  Serial.println("> by Steve Wozniak");
  Serial.println("----------------------------------------------------");
   
    // Initialize processor GPIO's
    
  uP_init();
  m6821_init();
  
  // Reset processor for 25 cycles
  
  uP_assert_reset();
  
  for(int i=0; i<25; i++) cpu_tick();
  
  uP_release_reset();

  Serial.println("\n");
}


////////////////////////////////////////////////////////////////////
// Loop()
////////////////////////////////////////////////////////////////////

void loop()
{
  // Loop forever
 
  while (true)
  {
    serialEvent0();
    cpu_tick();
  }
}
