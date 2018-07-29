#ifndef MMCBUF_H
#define MMCBUF_H
#include <stdint.h>
#include "opcodes.h"

struct MMCbuf
{
    byte_t data;
    uint16_t address;
};

#endif
