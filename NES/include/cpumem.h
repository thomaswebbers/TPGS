#ifndef CPUMEM_H
#define CPUMEM_H
#include "opcodes.h"

struct CPUmem
{
    /*
     * MEMORY LAYOUT:
     * ==============
     * 0x100   - Zero Page (part of RAM)
     * 0x200   - Stack (part of RAM)
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
    byte_t RAM[0x800];
    byte_t PPU_IO[0x08];
    byte_t APU_IO[0x20];
    byte_t EROM[0x1FE0];

    /*
     * the mmc handles which part of the rom is currently loaded. The banks
     * change at runtime, so the prg and sram arrays change as well.
     */

    byte_t *SRAM;
    byte_t *PRG_low;
    byte_t *PRG_high;
};

/*
 * Reads from the cpu memory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu memory
 * @param address the address to read from
 *
 * @return the byte at the specified address
 */
byte_t cpumem_read(struct CPUmem *cpumem_handle, uint16_t address);

/*
 * Writes to the cpu memory.
 *
 * @param cpumem_handle a handle to the nes virtual machine cpu memory
 * @param address the address to write to
 * @param data the data which should be written at the specified address
 *
 * @return void
 */
void cpumem_write(struct CPUmem *cpumem_handle, uint16_t address, byte_t data);

#endif
