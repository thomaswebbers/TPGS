#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "opcodes.h"
#include "ppumem.h"

struct PPU
{
    byte_t workinprogress;
    struct PPUmem memory;
};

void init_ppu(struct PPU *ppu_handle);

void destroy_ppu(struct PPU *ppu_handle);

#endif
