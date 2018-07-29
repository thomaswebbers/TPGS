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

    mmc_handle->cpu_mem->sram = mmc_handle->rom.sram;
    switch(mmc_handle->rom.mapper)
    {
        case NROM:
            if(mmc_handle->rom.num_prg == 2)
            {
                mmc_handle->cpu_mem->prg_low = mmc_handle->rom.prg;
                mmc_handle->cpu_mem->prg_high = mmc_handle->rom.prg + PRG_ROM_BANK_SIZE;
            }
            else
            {
                mmc_handle->cpu_mem->prg_low = mmc_handle->rom.prg;
                mmc_handle->cpu_mem->prg_high = mmc_handle->rom.prg;
            }
            break;
        case MMC1:
            printf("MMC1 support on its way! :D\n");
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
            printf("MMC1 support on its way! :D\n");


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
