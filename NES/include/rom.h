#ifndef ROM_H
#define ROM_H
#include <stdbool.h>
#include <stdint.h>

#include "opcodes.h"

struct NES;


#define PRG_ROM_BANK_SIZE       ((uint16_t) 0x4000) //16KB
#define CHR_ROM_BANK_SIZE       ((uint16_t) 0x2000) //8KB
#define TRAINER_SIZE            ((uint16_t) 0x0200) //512B

struct ROM
{
    byte_t num_prg;     //amount of PRG-ROM banks
    byte_t *prg;       //PRG-ROM banks
    byte_t num_chr;     //amount of CHR-ROM banks
    byte_t *chr;       //CHR-ROM banks
    byte_t *sram;       //Save RAM
    byte_t mapper;      //mapper type
    byte_t mirror;      //mirroring mode
    byte_t battery;     //battery-backed ROM
};

bool init_rom(struct NES *nes, char *binary_path);

void destroy_rom(struct NES *nes);

#endif
