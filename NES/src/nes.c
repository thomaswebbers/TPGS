#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nes.h"
#include "cpu.h"
#include "ppu.h"
#include "cartrige.h"

int init_nes(struct NES **nes_handle, char *binary_file)
{
    *nes_handle = malloc(sizeof(struct NES));
    (**nes_handle).master_clock = 0;
    init_cpu(&((**nes_handle).cpu));
    init_ppu(&((**nes_handle).ppu));

    if(!load_cartrige(&(**nes_handle).cartrige, binary_file))
    {
        free(*nes_handle);
        return INIT_FAILURE;
    }
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
    cpu_step(&(nes_handle->cpu));
    nes_handle->master_clock++;
    return true;
}

void destroy_nes(struct NES *nes_handle)
{
    free(nes_handle);
}
