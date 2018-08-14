#include <stdio.h>
#include "nes.h"


void init_cpumem(struct NES *nes)
{

}

byte_t *cpumem_readbp(struct NES *nes, uint16_t address)
{
    if(address < 0x2000)
        return &nes->cpu.memory.ram[address & 0x07FF];
    else if(address < 0x4000)
        return &nes->cpu.memory.ppu_io[(address - 0x2000) & 0x07];
    else if(address < 0x4020)
        return &nes->cpu.memory.apu_io[address - 0x4000];
    else if(address < 0x6000)
        return &nes->cpu.memory.erom[address - 0x4020];
    else if(address < 0x8000)
        return &nes->cpu.memory.sram[address - 0x6000];
    else if(address < 0xC000)
        return &nes->cpu.memory.prg_low[address - 0x8000];

    return &nes->cpu.memory.prg_high[address - 0xC000];

}

void cpumem_writeb(struct NES *nes, uint16_t address, byte_t data)
{
    if(address < 0x2000)
        nes->cpu.memory.ram[address & 0x07FF] = data;
    else if(address < 0x4000)
        nes->cpu.memory.ppu_io[(address - 0x2000) & 0x07] = data;
    else if(address < 0x4020)
        nes->cpu.memory.apu_io[address - 0x4000] = data;
    else if(address < 0x6000)
        nes->cpu.memory.erom[address - 0x4020] = data;
    else if(address < 0x8000)
        nes->cpu.memory.sram[address - 0x6000] = data;
    else
    {
        nes->mmc.buffer.data = data;
        nes->mmc.buffer.address = address;
    }
}

uint16_t cpumem_reads(struct NES *nes, uint16_t address)
{
    return  (*cpumem_readbp(nes, address + 1) << 8)
            | *cpumem_readbp(nes, address);
}
void cpumem_writes(struct NES *nes, uint16_t address, uint16_t data)
{
    cpumem_writeb(nes, address, (byte_t) data);
    cpumem_writeb(nes, address + 1, (byte_t) (data >> 8));
}
