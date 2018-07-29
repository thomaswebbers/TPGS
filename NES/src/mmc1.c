#include "mmc.h"
#include "mmc1.h"

void init_mmc1(struct MMC *mmc_handle)
{
    mmc_handle->cpu_mem->prg_low = mmc_handle->rom.prg;
    mmc_handle->cpu_mem->prg_high = mmc_handle->rom.prg
        + (mmc_handle->rom.num_prg - 1) * PRG_ROM_BANK_SIZE;
}

void step_mmc1(struct MMC *mmc_handle)
{
    if((mmc_handle->buffer.data & 0x80) == 0x80)
        mmc_handle->shift_register = 0x80;
    else
    {
        mmc_handle->shift_register >> 1;
        mmc_handle->shift_register |= (mmc_handle->buffer.data & 0x01) << 7;
        if((mmc_handle->buffer.data & 0x40) == 0x40)
        {

            mmc_handle->shift_register = 0x80;
        }
    }
}
