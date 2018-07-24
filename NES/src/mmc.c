#include "mmc.h"
#include "cpu.h"
#include "ppu.h"

void init_mmc(struct MMC *mmc_handle, struct CPU *cpu_handle, struct PPU *ppu_handle, char *binary_file)
{
    mmc_handle->SRAM_CPU = &cpu_handle->memory.SRAM;
    mmc_handle->PRG_low_CPU = &cpu_handle->memory.PRG_low;
    mmc_handle->PRG_high_CPU = &cpu_handle->memory.PRG_high;
    mmc_handle->CHR_PPU = &ppu_handle->memory.pattern_table;
    init_rom(&mmc_handle->rom, binary_file);
}

void destroy_mmc(struct MMC *mmc_handle)
{
    destroy_rom(&mmc_handle->rom);
}
