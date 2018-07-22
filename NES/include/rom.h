#ifndef ROM_H
#define ROM_H
#include "nes.h"

struct ROM
{
    uint32_t length;
    byte_t *binary;
};


#endif
