#ifndef MMC_H
#define MMC_H
#include "opcodes.h"
#include "rom.h"
#include "cpu.h"
#include "ppu.h"

struct MMC
{
    byte_t reg0;
    byte_t reg1;
    byte_t reg2;
    byte_t reg3;
    struct ROM rom;
    byte_t **SRAM_CPU;
    byte_t **PRG_low_CPU;
    byte_t **PRG_high_CPU;
    byte_t **CHR_PPU;
};

void init_mmc(struct MMC *mmc_handle, struct CPU *cpu_handle, struct PPU *ppu_handle, char *binary_file);

void destroy_mmc(struct MMC *mmc_handle);

#endif
