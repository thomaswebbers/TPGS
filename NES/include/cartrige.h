#ifndef ROM_H
#define ROM_H
#include <stdbool.h>

#include "opcodes.h"


#define PRG_ROM_BANK_SIZE      ((uint16_t) 0x4000)

struct Cartrige
{
    byte_t **prg;        // PRG-ROM banks
    byte_t **chr;        // CHR-ROM banks
    byte_t *sram;       // Save RAM
    byte_t mapper;      // mapper type
    byte_t mirror;      // mirroring mode
};

bool load_cartrige(struct Cartrige *cartrige_handle, char *binary_path);

#endif
