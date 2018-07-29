#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "nes.h"
#include "cpu.h"
#include "ppu.h"
#include "rom.h"

int init_nes(struct NES **nes_handle, char *binary_file)
{
    *nes_handle = malloc(sizeof(struct NES));
    (**nes_handle).master_clock = 0;
    init_mmc(   &(**nes_handle).mmc,
                &(**nes_handle).cpu.memory,
                &(**nes_handle).ppu.memory,
                binary_file);
    init_cpu(&(**nes_handle).cpu, &(**nes_handle).mmc.buffer);
    init_ppu(&(**nes_handle).ppu);
    return INIT_SUCCES;
}

void run_nes(struct NES *nes_handle)
{
    while(step_nes(nes_handle))
    {
        //sleep(1);
    }
}

bool step_nes(struct NES *nes_handle)
{
    nes_handle->master_clock++;
    bool test = cpu_step(&(nes_handle->cpu));
    step_mmc(&(nes_handle->mmc));
    return test;
}

void destroy_nes(struct NES *nes_handle)
{
    destroy_cpu(&nes_handle->cpu);
    destroy_ppu(&nes_handle->ppu);
    destroy_mmc(&nes_handle->mmc);
    free(nes_handle);
}
