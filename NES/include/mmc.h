#ifndef MMC_H
#define MMC_H
#include "opcodes.h"
#include "rom.h"
#include "mmcbuf.h"
#include "cpumem.h"
#include "ppumem.h"

struct MMC
{
    struct ROM rom;
    struct CPUmem *cpu_mem;
    struct PPUmem *ppu_mem;
    struct MMCbuf buffer;
    byte_t shift_register;
};

void init_mmc(struct MMC *mmc_handle, struct CPUmem *cpumem_handle, struct PPUmem *ppumem_handle, char *binary_file);

void step_mmc(struct MMC *mmc_handle);

void destroy_mmc(struct MMC *mmc_handle);

#endif
