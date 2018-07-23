#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "opcodes.h"

struct PPU
{
    byte_t workinprogress;
};

void init_ppu(struct PPU *ppu_handle);

#endif
