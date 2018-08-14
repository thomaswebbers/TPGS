#include "nes.h"
#include "mmc1.h"

void init_mmc1(struct NES *nes)
{
    nes->cpu.memory.prg_low = nes->mmc.rom.prg;
    nes->cpu.memory.prg_high = nes->mmc.rom.prg
        + (nes->mmc.rom.num_prg - 1) * PRG_ROM_BANK_SIZE;
}

void step_mmc1(struct NES *nes)
{
    if((nes->mmc.buffer.data & 0x80) == 0x80)
        nes->mmc.shift_register = 0x80;
    else
    {
        nes->mmc.shift_register >> 1;
        nes->mmc.shift_register |= (nes->mmc.buffer.data & 0x01) << 7;
        if((nes->mmc.buffer.data & 0x40) == 0x40)
        {
            nes->mmc.shift_register = 0x80;
        }
    }
}
