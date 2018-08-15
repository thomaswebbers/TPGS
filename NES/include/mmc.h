#ifndef MMC_H
#define MMC_H
#include "opcodes.h"
#include "rom.h"
#include "mmcbuf.h"
#include "cpumem.h"
#include "ppumem.h"

struct NES;

struct MMC
{
    struct ROM rom;
    struct MMCbuf buffer;
    byte_t shift_register;
};

void init_mmc(struct NES *nes, char *binary_file);

void step_mmc(struct NES *nes);

void destroy_mmc(struct NES *nes);

#endif
