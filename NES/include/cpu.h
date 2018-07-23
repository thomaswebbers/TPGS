#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "nes.h"

#define NES_MEMORY_SIZE ((uint32_t) 0x10000)

struct CPU
{
    /*
     * Every processing unit in the nes gets its own clock to match against The
     * master clock. (i.e. to maintain same clock speed as original NES)
     */
    uint32_t clock;

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

     /*
      * MEMORY LAYOUT:
      * ==============
      * 0x100   - Zero Page
      * 0x200   - Stack
      * 0x800   - RAM
      * 0x2000  - Mirrors (0-0x7FF)
      * 0x2008  - I/O Registers
      * 0x4000  - Mirrors (0x2000-0x2007)
      * 0x4020  - I/O Registers
      * 0x6000  - Expansion ROM
      * 0x8000  - SRAM
      * 0xC000  - PRG-ROM (Lower Bank)
      * 0x10000 - PRG-ROM (Upper Bank)
      */
     byte_t memory[NES_MEMORY_SIZE];
};

/*
 * Adds an amount of cycles to the clock of the cpu. The amount added should
 * differs depending on the opcode executed.
 *
 * @param nes_handle a handle to the nes virtual machine
 * @param cycles the amount of cycles that should be added to the cpu clock
 *
 * @return void
 */
void cpu_add_cycles(struct NES *nes_handle, uint32_t cycles);

/*
 * @param nes_handle a handle to the nes virtual machine
 */
bool cpu_step(struct NES *nes_handle);

/*
 * Fetches the byte the pc is currenty pointing to.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return byte pc is pointing to
 */
byte_t cpu_current_byte(struct NES *nes_handle);

#endif
