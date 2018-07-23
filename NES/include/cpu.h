#ifndef CPU_H
#define CPU_H
#include <stdint.h>

struct CPU
{
    /*
     * Registers
     * ==========
     * Program Counter  (16bit)
     * Stack pointer    (8bit)
     * Accumulator      (8bit)
     * Index Register X (8bit)
     * Index Register Y (8bit)
     */
     uint16_t pc;
     byte_t sp;
     byte_t A;
     byte_t X;
     byte_t Y;

     /*
      * Processor Status
      * =================
      * 0x01    - Carry (if last instruction resulted in under/overflow)
      * 0x02    - Zero (if last instruction's result was 0)
      * 0x04    - Interrupt Disable (Enable to prevent system from responding to interrupts)
      * 0x08    - Decimal mode (unsupported on this chip variant)
      * 0x10    - Break Command (if BRK (break) instruction has been executed)
      * 0x20    - Empty
      * 0x40    - Overflow (if previous instruction resulted in an invalid two's complement)
      * 0x80    - Negative
      */
     byte_t P;
};

#endif
