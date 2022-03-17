;RetroShield 6502 TLB Utility routines (in assembly language)
;
;Optimized for speed by keeping as much as possible in registers


#define __SFR_OFFSET 0
#include "avr/io.h"
#include "6502_iodefs.h"

;External symbols referenced from internal assembly code.

.global sram_bytes      ;Array of bytes representing 6502 static RAM
.global ptr_rom1_bytes  ;Pointer to array of bytes in first 6502 ROM array
.global ptr_rom2_bytes  ;Pointer to array of bytes in second 6502 ROM array
.global SerialRead      ;Function "uint8_t SerialRead()"
.global SerialWrite     ;Function "void SerialWrite(uint8_t chr)"
.global DisplayAddress  ;Function "void DisplayAddress(uint16_t addr)"
.global DisplayData     ;Function "void DisplayData(uint8_t data)"

;Internal symbols referenced from external code.

.global reset_tick
.global cpu_tick_loop

  ;reset_tick - Perform one simple 6502 clock tick during "reset".

reset_tick:
  sbi   PORTB,1     ;Set 6502 master clock HIGH (CLK_ENABLE | 0x02)
  nop               ;Hold it HIGH for a while.
  nop
  nop
  nop
  cbi   PORTB,1     ;Hold LOW for a while.
  nop
  nop
  nop
  nop    
  sbi   PORTB,1     ;Leave master clock in HIGH state.
  ret

  ;cpu_tick_loop - Repeat 6502 clock ticks forever

  ;Define specialized local register names

#define r_ddir_out     r9
#define r_tlbio_0_L    r10
#define r_tlbio_0_H    r11
#define r_tlbio_1_L    r12
#define r_tlbio_1_H    r13
#define r_sram_bytes_L r14
#define r_sram_bytes_H r15
#define r_rom1_bytes_L r16
#define r_rom1_bytes_H r17
#define r_rom2_bytes_L r18
#define r_rom2_bytes_H r19

// *** IMPORTANT: This register pair MUST be the Z register! ***

#define r_uP_addr_L    r30
#define r_uP_addr_H    r31

cpu_tick_loop:        ;Perform one-time register initialization
  clr r_ddir_out      ;Bit pattern to set all 6502 data bus pins as output pins
  com r_ddir_out
  ldi r31,0xEC        ;TLBIO_0 = $EC00
  mov r_tlbio_0_H,r31
  ldi r31,0x00
  mov r_tlbio_0_L,r31
  ldi r31,0xEC        ;TLBIO_1 = $EC01
  mov r_tlbio_1_H,r31
  ldi r31,0x01
  mov r_tlbio_1_L,r31
  ldi r31,hi8(sram_bytes) ;16-bit address of sram_bytes array.
  mov r_sram_bytes_H,r31
  ldi r31,lo8(sram_bytes)
  mov r_sram_bytes_L,r31
  
  lds r_rom1_bytes_H,ptr_rom1_bytes+1   ;16-bit address of rom1_bytes array.
  lds r_rom1_bytes_L,ptr_rom1_bytes

  mov   r30,r_rom1_bytes_L    ;Subtract $8000 address offset
  mov   r31,r_rom1_bytes_H
  subi  r30,0x00
  sbci  r31,0x80
  mov   r_rom1_bytes_L,r30
  mov   r_rom1_bytes_H,r31

  lds r_rom2_bytes_H,ptr_rom2_bytes+1   ;16-bit address of rom2_bytes array.
  lds r_rom2_bytes_L,ptr_rom2_bytes

  mov   r30,r_rom2_bytes_L    ;Subtract $C000 address offset
  mov   r31,r_rom2_bytes_H
  subi  r30,0x00
  sbci  r31,0xC0
  mov   r_rom2_bytes_L,r30
  mov   r_rom2_bytes_H,r31 
  
  rjmp  begin_cpu_tick  ;Enter the infinite clock tick loop   
  
end_cpu_tick:
  cbi   PORTB,1     ;Set 6502 master clock LOW (CLK_ENABLE & 0xFD)
  sts   DDRL,r1     ;Set DATA_DIR (DDRL) direction to input from 6502 data bus.
                    ;   (By convention r1 always contains zero.)
begin_cpu_tick:
  sbi   PORTB,1           ;Set 6502 master clock HIGH (CLK_ENABLE | 0x02)
  in    r_uP_addr_L,PINA  ;Fetch 6502 ADDR_H:ADDR_L into registers
  in    r_uP_addr_H,PINC
  sbis  PING,1            ;Skip if 6502 R/W line is HIGH (i.e., we're doing a read)
  rjmp  cpu_write         ;We are doing a 6502 write operation.

  ;Here if performing a read operation

  sts   DDRL,r_ddir_out   ;Set DATA_DIR (DDRL) direction to output to 6502 data bus.
  
  ;Here if 6502 is reading from memory

cpu_r_mem:
  tst   r_uP_addr_H ;Is 6502 address in rom space ($8000 and higher?)
  brpl  cpu_r_ram   ;Branch if not. Read from 6502 ram addr

  ;Read from 6502 rom addr

  sbrc  r_uP_addr_H,6 ;Skip if 6502 address is NOT higher than $C000.
  brmi  cpu_r_rom2    ;Always branches when the address is $C000 or higher.

  ;Here if reading from 6502 first rom section ($8000-$BFFF)

  add   r_uP_addr_L,r_rom1_bytes_L  ;Add rom2_bytes base addr
  adc   r_uP_addr_H,r_rom1_bytes_H  ;Z reg pair now contains Arduino flash addr.
  lpm                               ;Fetch data byte from rom1_bytes array into r0.
  sts   PORTL,r0                    ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick

  ;Here if reading from 6502 second rom section. ($C000-$FFFF)

cpu_r_rom2:

  cp    r_uP_addr_H,r_tlbio_0_H ;Are we reading from TLBIO_0?
  brne  cpu_r_rom2_cont         ;Branch if not.
  cp    r_uP_addr_L,r_tlbio_0_L ;Maybe. Is low order addr byte same as TLBIO_0?
  breq  cpu_rio_0               ;Branch if yes. (We are reading from TLBIO_0)

cpu_r_rom2_cont:
  add   r_uP_addr_L,r_rom2_bytes_L  ;Add rom2_bytes base addr
  adc   r_uP_addr_H,r_rom2_bytes_H  ;Z reg pair now contains Arduino flash addr.
  lpm                               ;Fetch data byte from rom2_bytes array into r0.
  sts   PORTL,r0                    ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick          

  ;Read from 6502 ram

cpu_r_ram:
  add   r_uP_addr_L,r_sram_bytes_L  ;Add sram_bytes base address to 6502 ram addr
  adc   r_uP_addr_H,r_sram_bytes_H  ;Z reg pair now contains Arduino SRAM address
  ld    r0,Z                ;Fetch SRAM byte from Arduino
  sts   PORTL,r0            ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick   

  ;Perform a write operation.
  
cpu_write:
  cp    r_uP_addr_H,r_tlbio_1_H ;Are we writing to TLBIO_1?
  brne  cpu_w_ram               ;Branch if not.
  cp    r_uP_addr_L,r_tlbio_1_L ;Maybe. Is low order addr that of TLBIO_1?
  breq  cpu_wio_1               ;Branch if yes.  (We are writing to TLBIO_1)

  ;Here if writing to RAM

cpu_w_ram:
  lds   r0,PINL       ;Fetch 6502 DATA_IN to get 8-bits to store in ram.
  add   r_uP_addr_L,r_sram_bytes_L  ;Add sram_bytes base address to 6502 ram addr
  adc   r_uP_addr_H,r_sram_bytes_H  ;Z reg pair now contains Arduino SRAM address
  st    Z,r0          ;Store 6502 data byte into 6502 RAM array
  rjmp  end_cpu_tick

  ;Here if reading from TLBIO_0 (input from keyboard)

cpu_rio_0:
  call  serial_read   ;Call external function to read serial input.
  sts   PORTL,r0      ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick   

  ;Here if writing to TLBIO_1 (output to console device)

cpu_wio_1:
  lds   r0,PINL       ;Fetch 6502 DATA_IN to get 8-bits to write.
  call  serial_write  ;Call external function to send the byte.
  rjmp  end_cpu_tick

  ;Routine to call the external SerialRead function to get char from keyboard.
  ;Routine immediately returns a zero byte if no character is available.
  ;Otherwise, it returns the 7-bit ASCII code for the character typed.
  ;The keyboard input is returned in r0.

serial_read:
  push  r_ddir_out        ;Save contents of registers we use
  push  r_tlbio_0_L
  push  r_tlbio_0_H
  push  r_tlbio_1_L
  push  r_tlbio_1_H
  push  r_sram_bytes_L
  push  r_sram_bytes_H
  push  r_rom1_bytes_L
  push  r_rom1_bytes_H
  push  r_rom2_bytes_L
  push  r_rom2_bytes_H
  push  r_uP_addr_L
  push  r_uP_addr_H
  push  r24
  push  r25
  
  call  SerialRead        ;Call external C routine to do the work
  mov   r0,r24            ;C routine returns the character in r24.

  pop   r25               ;Restore our register contents
  pop   r24
  pop   r_uP_addr_H
  pop   r_uP_addr_L
  pop   r_rom2_bytes_H
  pop   r_rom2_bytes_L
  pop   r_rom1_bytes_H
  pop   r_rom1_bytes_L
  pop   r_sram_bytes_H
  pop   r_sram_bytes_L
  pop   r_tlbio_1_H
  pop   r_tlbio_1_L
  pop   r_tlbio_0_H
  pop   r_tlbio_0_L
  pop   r_ddir_out    

  ret

  ;Routine to call external SerialWrite routine to output the 7-bit ASCII
  ;character in r0 to the console.

serial_write:
  push  r_ddir_out        ;Save contents of registers we use
  push  r_tlbio_0_L
  push  r_tlbio_0_H
  push  r_tlbio_1_L
  push  r_tlbio_1_H
  push  r_sram_bytes_L
  push  r_sram_bytes_H
  push  r_rom1_bytes_L
  push  r_rom1_bytes_H
  push  r_rom2_bytes_L
  push  r_rom2_bytes_H
  push  r_uP_addr_L
  push  r_uP_addr_H
  push  r24
  push  r25
  push  r0
  
  mov   r24,r0            ;First parameter passed to external C functions
  clr   r25               ;is in r25:r24 register pair
  
  call  SerialWrite       ;Call external C routine to do the work

  pop   r0                ;Restore our register contents
  pop   r25
  pop   r24
  pop   r_uP_addr_H
  pop   r_uP_addr_L
  pop   r_rom2_bytes_H
  pop   r_rom2_bytes_L
  pop   r_rom1_bytes_H
  pop   r_rom1_bytes_L
  pop   r_sram_bytes_H
  pop   r_sram_bytes_L
  pop   r_tlbio_1_H
  pop   r_tlbio_1_L
  pop   r_tlbio_0_H
  pop   r_tlbio_0_L
  pop   r_ddir_out    

  ret

  ;Debug routine to display 16-bit microprocessor address

display_uP_addr:
  push  r_ddir_out        ;Save contents of registers we use
  push  r_tlbio_0_L
  push  r_tlbio_0_H
  push  r_tlbio_1_L
  push  r_tlbio_1_H
  push  r_sram_bytes_L
  push  r_sram_bytes_H
  push  r_rom1_bytes_L
  push  r_rom1_bytes_H
  push  r_rom2_bytes_L
  push  r_rom2_bytes_H
  push  r_uP_addr_L
  push  r_uP_addr_H
  push  r24
  push  r25
  push  r0
  
  mov   r24,r_uP_addr_L   ;First parameter passed to external C functions
  mov   r25,r_uP_addr_H   ;is in r25:r24 register pair
  
  call  DisplayAddress    ;Call external C routine to do the work

  pop   r0                ;Restore our register contents
  pop   r25
  pop   r24
  pop   r_uP_addr_H
  pop   r_uP_addr_L
  pop   r_rom2_bytes_H
  pop   r_rom2_bytes_L
  pop   r_rom1_bytes_H
  pop   r_rom1_bytes_L
  pop   r_sram_bytes_H
  pop   r_sram_bytes_L
  pop   r_tlbio_1_H
  pop   r_tlbio_1_L
  pop   r_tlbio_0_H
  pop   r_tlbio_0_L
  pop   r_ddir_out    

  ret

  ;Debug routine to display 8-bit data byte stored in r0

display_data:

  push  r_ddir_out        ;Save contents of registers we use
  push  r_tlbio_0_L
  push  r_tlbio_0_H
  push  r_tlbio_1_L
  push  r_tlbio_1_H
  push  r_sram_bytes_L
  push  r_sram_bytes_H
  push  r_rom1_bytes_L
  push  r_rom1_bytes_H
  push  r_rom2_bytes_L
  push  r_rom2_bytes_H
  push  r_uP_addr_L
  push  r_uP_addr_H
  push  r24
  push  r25
  push  r0
  
  mov   r24,r0            ;First parameter passed to external C functions
  clr   r25               ;is in r25:r24 register pair      
  
  call  DisplayData       ;Call external C routine to do the work

  pop   r0                ;Restore our register contents
  pop   r25
  pop   r24
  pop   r_uP_addr_H
  pop   r_uP_addr_L
  pop   r_rom2_bytes_H
  pop   r_rom2_bytes_L
  pop   r_rom1_bytes_H
  pop   r_rom1_bytes_L
  pop   r_sram_bytes_H
  pop   r_sram_bytes_L
  pop   r_tlbio_1_H
  pop   r_tlbio_1_L
  pop   r_tlbio_0_H
  pop   r_tlbio_0_L
  pop   r_ddir_out    

  ret
   
