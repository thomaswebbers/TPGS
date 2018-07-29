#include "mmc.h"
#include "nrom.h"

void init_nrom(struct MMC *mmc_handle)
{
    mmc_handle->cpu_mem->prg_low = mmc_handle->rom.prg;
    mmc_handle->cpu_mem->prg_high = mmc_handle->rom.prg
        + (mmc_handle->rom.num_prg - 1) * PRG_ROM_BANK_SIZE;
}
