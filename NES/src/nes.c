#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "nes.h"
#include "cpu.h"
#include "ppu.h"
#include "rom.h"

int init_nes(struct NES **nes, char *binary_file)
{
    *nes = malloc(sizeof(struct NES));
    (**nes).master_clock = 0;
    init_mmc(*nes, binary_file);
    init_cpu(*nes);
    init_ppu(*nes);
    return INIT_SUCCES;
}

void run_nes(struct NES *nes)
{
    while(step_nes(nes))
    {
        usleep(10000);
    }
}

bool step_nes(struct NES *nes)
{
    nes->master_clock++;
    bool test = cpu_step(nes);
    step_mmc(nes);
    return test;
}

void destroy_nes(struct NES *nes)
{
    destroy_cpu(nes);
    destroy_ppu(nes);
    destroy_mmc(nes);
    free(nes);
}
