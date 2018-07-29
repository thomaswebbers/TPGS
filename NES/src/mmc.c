#include <stdio.h>

#include "mmc.h"
#include "mmcbuf.h"

#define NROM            ((byte_t) 0x00)
#define MMC1            ((byte_t) 0x01)

void init_mmc(struct MMC *mmc_handle, struct CPUmem *cpumem_handle, struct PPUmem *ppumem_handle, char *binary_file)
{
    mmc_handle->cpu_mem = cpumem_handle;
    mmc_handle->ppu_mem = ppumem_handle;
    init_rom(&mmc_handle->rom, binary_file);
    mmc_handle->shift_register = 0;

    mmc_handle->cpu_mem->sram = mmc_handle->rom.sram;
    switch(mmc_handle->rom.mapper)
    {
        case NROM:
            init_nrom(mmc_handle);
            break;
        case MMC1:
            init_mmc1(mmc_handle);
            break;
        default:
            printf("Mapper not yet implemented: %x\n", mmc_handle->rom.mapper);
    }
}

void step_mmc(struct MMC *mmc_handle)
{
    if(mmc_handle->buffer.address == 0x00)
        return;

    switch(mmc_handle->rom.mapper)
    {
        case NROM:
            fprintf(stderr, "Shouldn't write to rom if mapper == NROM\n");
            break;
        case MMC1:
            step_mmc1(mmc_handle);
            break;
        default:
            fprintf(stderr, "Mapper not yet implemented: %x\n", mmc_handle->rom.mapper);
    }
    mmc_handle->buffer.address = 0x00;
}

void destroy_mmc(struct MMC *mmc_handle)
{
    destroy_rom(&mmc_handle->rom);
}
