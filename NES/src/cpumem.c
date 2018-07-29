#include <stdio.h>
#include "cpumem.h"
#include "mmcbuf.h"


void init_cpumem(struct CPUmem *cpumem_handle, struct MMCbuf *buffer)
{
    cpumem_handle->buffer_mmc = buffer;
}

byte_t *cpumem_readbp(struct CPUmem *cpumem_handle, uint16_t address)
{
    if(address < 0x2000)
        return &cpumem_handle->ram[address & 0x07FF];
    else if(address < 0x4000)
        return &cpumem_handle->ppu_io[(address - 0x2000) & 0x07];
    else if(address < 0x4020)
        return &cpumem_handle->apu_io[address - 0x4000];
    else if(address < 0x6000)
        return &cpumem_handle->erom[address - 0x4020];
    else if(address < 0x8000)
        return &cpumem_handle->sram[address - 0x6000];
    else if(address < 0xC000)
        return &cpumem_handle->prg_low[address - 0x8000];

    return &cpumem_handle->prg_high[address - 0xC000];

}

void cpumem_writeb(struct CPUmem *cpumem_handle, uint16_t address, byte_t data)
{
    if(address < 0x2000)
        cpumem_handle->ram[address & 0x07FF] = data;
    else if(address < 0x4000)
        cpumem_handle->ppu_io[(address - 0x2000) & 0x07] = data;
    else if(address < 0x4020)
        cpumem_handle->apu_io[address - 0x4000] = data;
    else if(address < 0x6000)
        cpumem_handle->erom[address - 0x4020] = data;
    else if(address < 0x8000)
        cpumem_handle->sram[address - 0x6000] = data;
    else
    {
        cpumem_handle->buffer_mmc->data = data;
        cpumem_handle->buffer_mmc->address = address;
    }
}

uint16_t cpumem_reads(struct CPUmem *cpumem_handle, uint16_t address)
{
    return  (*cpumem_readbp(cpumem_handle, address + 1) << 8)
            | *cpumem_readbp(cpumem_handle, address);
}
void cpumem_writes(struct CPUmem *cpumem_handle, uint16_t address, uint16_t data)
{
    cpumem_writeb(cpumem_handle, address, (byte_t) data);
    cpumem_writeb(cpumem_handle, address + 1, (byte_t) (data >> 8));
}
