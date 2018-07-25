#include <stdio.h>

#include "mmc.h"
#include "cpu.h"
#include "ppu.h"

#define NO_MAPPER       ((byte_t) 0x00)

void init_mmc(struct MMC *mmc_handle, struct CPU *cpu_handle, struct PPU *ppu_handle, char *binary_file)
{
    mmc_handle->SRAM_CPU = &cpu_handle->memory.SRAM;
    mmc_handle->PRG_low_CPU = &cpu_handle->memory.PRG_low;
    mmc_handle->PRG_high_CPU = &cpu_handle->memory.PRG_high;
    mmc_handle->CHR_PPU = &ppu_handle->memory.pattern_table;
    init_rom(&mmc_handle->rom, binary_file);
    switch(mmc_handle->rom.mapper)
    {
        case NO_MAPPER:
            if(mmc_handle->rom.num_prg == 2)
            {
                *mmc_handle->PRG_low_CPU = mmc_handle->rom.prg[0];
                *mmc_handle->PRG_high_CPU = mmc_handle->rom.prg[1];
            }
            else
            {
                *mmc_handle->PRG_low_CPU = mmc_handle->rom.prg[0];
                *mmc_handle->PRG_high_CPU = mmc_handle->rom.prg[0];
            }
            break;
        default:
            printf("Mapper not yet implemented: %x\n", mmc_handle->rom.mapper);
    }
}

void destroy_mmc(struct MMC *mmc_handle)
{
    destroy_rom(&mmc_handle->rom);
}
