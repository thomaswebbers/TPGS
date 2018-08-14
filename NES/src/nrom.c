#include "nes.h"
#include "nrom.h"

void init_nrom(struct NES *nes)
{
    nes->cpu.memory.prg_low = nes->mmc.rom.prg;
    nes->cpu.memory.prg_high = nes->mmc.rom.prg
        + (nes->mmc.rom.num_prg - 1) * PRG_ROM_BANK_SIZE;
}
