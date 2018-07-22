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
      * 0       - Carry (if last instruction resulted in under/overflow)
      * 1       - Zero (if last instruction's result was 0)
      * 2       - Interrupt Disable (Enable to prevent system from responding to interrupts)
      * 3       - Decimal mode (unsupported on this chip variant)
      * 4       - Break Command (if BRK (break) instruction has been executed)
      * 5       - Empty
      * 6       - Overflow (if previous instruction resulted in an invalid two's complement)
      * 7       - Negative
      */
     byte_t P;
};

#endif
