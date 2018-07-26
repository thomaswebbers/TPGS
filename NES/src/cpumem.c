#include "cpumem.h"
#include <stdio.h>

byte_t cpumem_readb(struct CPUmem *cpumem_handle, uint16_t address)
{
    if(address < 0x2000)
        return cpumem_handle->RAM[address & 0x07FF];
    else if(address < 0x4000)
        return cpumem_handle->PPU_IO[(address - 0x2000) & 0x07];
    else if(address < 0x4020)
        return cpumem_handle->APU_IO[address - 0x4000];
    else if(address < 0x6000)
        return cpumem_handle->EROM[address - 0x4020];
    else if(address < 0x8000)
        return cpumem_handle->SRAM[address - 0x6000];
    else if(address < 0xC000)
        return cpumem_handle->PRG_low[address - 0x8000];

    return cpumem_handle->PRG_high[address - 0xC000];

}

void cpumem_writeb(struct CPUmem *cpumem_handle, uint16_t address, byte_t data)
{
    if(address < 0x2000)
        cpumem_handle->RAM[address & 0x07FF] = data;
    else if(address < 0x4000)
        cpumem_handle->PPU_IO[(address - 0x2000) & 0x07] = data;
    else if(address < 0x4020)
        cpumem_handle->APU_IO[address - 0x4000] = data;
    else if(address < 0x6000)
        cpumem_handle->EROM[address - 0x4020] = data;
    else if(address < 0x8000)
        cpumem_handle->SRAM[address - 0x6000] = data;
    else if(address < 0xC000)
        cpumem_handle->PRG_low[address - 0x8000] = data;
    else
        cpumem_handle->PRG_high[address - 0xC000] = data;
}

uint16_t cpumem_reads(struct CPUmem *cpumem_handle, uint16_t address)
{
    return  (cpumem_readb(cpumem_handle, address + 1) << 8)
            | cpumem_readb(cpumem_handle, address);
}
void cpumem_writes(struct CPUmem *cpumem_handle, uint16_t address, uint16_t data)
{
    cpumem_writeb(cpumem_handle, address, (byte_t) data);
    cpumem_writeb(cpumem_handle, address + 1, (byte_t) (data >> 8));
}
