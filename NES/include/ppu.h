#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include "opcodes.h"
#include "ppumem.h"

struct NES;

struct PPU
{
    byte_t workinprogress;
    struct PPUmem memory;
};

void init_ppu(struct NES *nes);

void destroy_ppu(struct NES *nes);

#endif
