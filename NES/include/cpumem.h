#ifndef CPUMEM_H
#define CPUMEM_H
#include "opcodes.h"

#define NMI_VEC         ((uint16_t) 0xFFFA)
#define RESET_VEC       ((uint16_t) 0xFFFC)
#define IRQ_VEC         ((uint16_t) 0xFFFE)

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

    byte_t ram[0x800];
    byte_t ppu_io[0x08];
    byte_t apu_io[0x20];
    byte_t erom[0x1FE0];

    /*
     * the mmc handles which part of the rom is currently loaded. The banks
     * change at runtime, so the prg and sram arrays change as well.
     */

    byte_t *sram;
    byte_t *prg_low;
    byte_t *prg_high;

    /*
     * To keep bank switching seperate of the cpu, the calls to the mmc are
     * buffered inside mmc memory. The cpu has a pointer to this buffer.
     */

     struct MMCbuf *buffer_mmc;
};

void init_cpumem(struct CPUmem *cpumem_handle, struct MMCbuf *buffer);

/*
 * Reads a byte from the cpu memory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu memory
 * @param address the address to read from
 *
 * @return the byte at the specified address
 */
byte_t *cpumem_readbp(struct CPUmem *cpumem_handle, uint16_t address);

/*
 * Writes a byte to the cpu memory.
 *
 * @param cpumem_handle a handle to the nes virtual machine cpu memory
 * @param address the address to write to
 * @param data the data which should be written at the specified address
 *
 * @return void
 */
void cpumem_writeb(struct CPUmem *cpumem_handle, uint16_t address, byte_t data);

/*
 * Reads a byte from the cpu memory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu memory
 * @param address the address to read from
 *
 * @return the short at the specified address
 */
uint16_t cpumem_reads(struct CPUmem *cpumem_handle, uint16_t address);

/*
 * Writes a short to the cpu memory.
 *
 * @param cpumem_handle a handle to the nes virtual machine cpu memory
 * @param address the address to write to
 * @param data the data which should be written at the specified address
 *
 * @return void
 */
void cpumem_writes(struct CPUmem *cpumem_handle, uint16_t address, uint16_t data);

#endif
