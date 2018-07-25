#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nes.h"
#include "cpu.h"
#include "ppu.h"
#include "rom.h"

int init_nes(struct NES **nes_handle, char *binary_file)
{
    *nes_handle = malloc(sizeof(struct NES));
    (**nes_handle).master_clock = 0;
    init_mmc(&(**nes_handle).mmc, &(**nes_handle).cpu, &(**nes_handle).ppu, binary_file);
    init_cpu(&(**nes_handle).cpu);
    init_ppu(&(**nes_handle).ppu);
    /*
    if(!load_cartrige(&(**nes_handle).rom, binary_file))
    {
        free(*nes_handle);
        return INIT_FAILURE;
    }
    */
    return INIT_SUCCES;
}

void run_nes(struct NES *nes_handle)
{
    while(step_nes(nes_handle))
    {

    }
}

bool step_nes(struct NES *nes_handle)
{
    nes_handle->master_clock++;
    return cpu_step(&(nes_handle->cpu));
}

void destroy_nes(struct NES *nes_handle)
{
    destroy_cpu(&nes_handle->cpu);
    destroy_ppu(&nes_handle->ppu);
    destroy_mmc(&nes_handle->mmc);
    free(nes_handle);
}
