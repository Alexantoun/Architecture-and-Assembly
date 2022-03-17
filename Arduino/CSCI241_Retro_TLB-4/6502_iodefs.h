#ifndef _6502_IODEFS_H_
#define _6502_IODEFS_H_

////////////////////////////////////////////////////////////////////
// 6502 Processor Control Definitions
////////////////////////////////////////////////////////////////////

// Arduino GPIO Pin Assignments

#define DATA_OUT PORTL
#define DATA_IN  PINL
#define ADDR_H   PINC
#define ADDR_L   PINA

#define uP_RESET_N  38
#define uP_RW_N     40
#define uP_RDY      39
#define uP_SO_N     41
#define uP_IRQ_N    50
#define uP_NMI_N    51
#define uP_E        52
#define uP_GPIO     53

// Fast routines to drive 6502 clock signals high/low

#define CLK_ENABLE      PORTB   ;Arduino port controlling clock enable
#define CLK_E_BIT       1       ;Bit position (0,1,2,...,7) of clock enable
#define STATE_RW_N      PING    ;Arduino port R/W line is connected to
#define STATE_RW_N_BIT  1       ;Bit position (0,1,2,...,7) of 6502 R/W line

#define DIR_IN  0x00
#define DIR_OUT 0xFF
#define DATA_DIR   DDRL
#define ADDR_H_DIR DDRC
#define ADDR_L_DIR DDRA

#endif  // 6502_IODEFS_H
