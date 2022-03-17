;RetroShield 6502 TLB Utility routines (in assembly language)


#define __SFR_OFFSET 0
#include "avr/io.h"
#include "6502_iodefs.h"

;External symbols referenced from internal assembly code.

.global sram_bytes      ;Array of bytes representing 6502 static RAM
.global ptr_rom1_bytes  ;Array of bytes in first 6502 ROM array
.global ptr_rom2_bytes  ;Array of bytes in second 6502 ROM array
.global SerialRead      ;Function "uint8_t SerialRead()"
.global SerialWrite     ;Function "void SerialWrite(uint8_t chr)"
.global DisplayAddress  ;Function "void DisplayAddress(uint16_t addr)"

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

end_cpu_tick:
  cbi   PORTB,1     ;Set 6502 master clock LOW (CLK_ENABLE & 0xFD)
  sts   DDRL,r1     ;Set DATA_DIR (DDRL) direction to input from 6502 data bus.
                    ;   (By convention r1 always contains zero.)

  ;cpu_tick_loop - Repeat 6502 clock ticks forever
                    
cpu_tick_loop:
  sbi   PORTB,1     ;Set 6502 master clock HIGH (CLK_ENABLE | 0x02)
  in    r24,PINA    ;Fetch 6502 ADDR_H:ADDR_L into r25:24
  in    r25,PINC    ;   (ADDR_L = PINA, ADDR_L = PINC)
  sbis  PING,1      ;Skip if 6502 R/W line is HIGH (i.e., we're doing a read)
  rjmp  cpu_write   ;   No, perform a write operation.

  ;Here if performing a read operation

  clr   r0          ;Form 0xFF in r0
  com   r0
  sts   DDRL,r0     ;Set DATA_DIR (DDRL) direction to output from 6502 data bus.
  cpi   r25,0xEC    ;Are we reading from TLBIO_0 (0xEC00)?
  brne  cpu_r_mem   ;   Branch if not. (High order addr byte is not 0xEC)
  cpi   r24,0x00    ;   Maybe. Is low order addr byte 0x00?
  breq  cpu_rio_0   ;     Branch if yes. (We are reading from TLBIO_0)

  ;Here if reading from normal memory

cpu_r_mem:
  mov   r31,r25     ;Get high order of 6502 address into hi8(Z)
  tst   r31
  brpl  cpu_r_ram   ;Branch if reading from 6502 ram addr

  ;Here if reading from 6502 rom addr

  lsl   r31         ;Which rom section are we reading from?
  brpl  cpu_r_rom1  ;Branch if reading from first rom section.

  ;Here if reading from 6502 second rom section.

  mov   r31,r25       ;Produce offset (6502 addr & $3FFF)
  andi  r31,0x3F
  mov   r30,r24       ;Z now contains offset into 6502 rom2_bytes array.
  lds   r24,ptr_rom2_bytes  ;Get rom2_bytes base address into r25:r24.
  lds   r25,ptr_rom2_bytes+1
  add   r30,r24       ;Add offset to form Arduino flash address.
  adc   r31,r25
  lpm                 ;Fetch data byte from rom2_bytes array into r0.
  sts   PORTL,r0      ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick  ;Do it again...   

  ;Here if reading from 6502 first rom section.

cpu_r_rom1:
  lsr   r31           ;Effectively produce offset (6502 addr & $7FFF)
  mov   r30,r24       ;Z now contains offset into 6502 rom1_bytes array.
  lds   r24,ptr_rom1_bytes  ;Get rom1_bytes base address into r25:r24.
  lds   r25,ptr_rom1_bytes+1
  add   r30,r24       ;Add offset to form Arduino flash address.
  adc   r31,r25
  lpm                 ;Fetch data byte from rom1_bytes array into r0.
  sts   PORTL,r0      ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick  ;Do it again...  
  
  ;Here if performing a read from 6502 ram

cpu_r_ram:
  ldi   r26,lo8(sram_bytes) ;Fetch base addr of sram_bytes into r27:r26 (X)
  ldi   r27,hi8(sram_bytes)
  add   r26,r24             ;Form addr of sram_bytes + 6502 address
  adc   r27,r25
  ld    r0,X                ;Fetch byte from 6502 RAM array
  sts   PORTL,r0            ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick        ;Do it again...   

  ;Here if performing a write operation.
  
cpu_write:
  cpi   r25,0xEC    ;Are we writing to TLBIO_1 (0xEC01)?
  brne  cpu_w_ram   ;   Branch if not.  (High order addr byte not 0xEC)
  cpi   r24,0x01    ;   Maybe. Is low order addr byte 0x01?
  breq  cpu_wio_1   ;     Branch if yes.  (We are writing to TLBIO_1)

  ;Here if writing to RAM

cpu_w_ram:  
  lds   r0,PINL             ;Fetch 6502 DATA_IN to get 8-bits to store in ram.
  ldi   r26,lo8(sram_bytes) ;Fetch base addr of sram_bytes into r27:r26 (X)
  ldi   r27,hi8(sram_bytes)
  add   r26,r24             ;Form addr of sram_bytes + 6502 address
  adc   r27,r25
  st    X,r0                ;Store 6502 data byte into 6502 RAM array
  rjmp  end_cpu_tick        ;Do it again...

  ;Here if reading from TLBIO_0 (input from keyboard)

cpu_rio_0:
  call  SerialRead    ;Call external function to read serial input.
  sts   PORTL,r24     ;Place data byte on 6502 data bus.
  rjmp  end_cpu_tick  ;Do it again...    

  ;Here if writing to TLBIO_1 (output to console device)

cpu_wio_1:
  lds   r24,PINL      ;Fetch 6502 DATA_IN to get 8-bits to write.
  clr   r25           ;Probably not necessary, but set high order byte to 0.
  call  SerialWrite   ;Call external function to send the byte.
  rjmp  end_cpu_tick  ;Do it again...

  ;Debug routine to display 16-bit address stored in r25:r24

display_r25_r24:
  push  r24             ;Preserve internally used registers
  push  r25
  push  r26
  push  r27
  push  r30
  push  r31
  push  r0
  call  DisplayAddress  ;Call external C routine to do the work
  pop   r0              ;Restore internally used registers
  pop   r31
  pop   r30
  pop   r27
  pop   r26
  pop   r25
  pop   r24
  ret

  ;Debug routine to display 8-bit data byte stored in r0

display_r0:
  push  r24             ;Preserve internally used registers
  push  r25
  push  r26
  push  r27
  push  r30
  push  r31
  push  r0
  mov   r24,r0          ;Place r0 value in r24
  clr   r25             ;Probably not necessary, but set r25 to zero
  call  DisplayData     ;Call external C routine to do the work
  pop   r0              ;Restore internally used registers
  pop   r31
  pop   r30
  pop   r27
  pop   r26
  pop   r25
  pop   r24
  ret
   
