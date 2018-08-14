#include <stdio.h>

#include "nes.h"
#include "nrom.h"
#include "mmc1.h"

#define NROM            ((byte_t) 0x00)
#define MMC1            ((byte_t) 0x01)

void init_mmc(struct NES *nes, char *binary_file)
{
    init_rom(nes, binary_file);
    nes->mmc.shift_register = 0;
    switch(nes->mmc.rom.mapper)
    {
        case NROM:
            init_nrom(nes);
            break;
        case MMC1:
            init_mmc1(nes);
            break;
        default:
            printf("Mapper not yet implemented: %x\n", nes->mmc.rom.mapper);
    }
}

void step_mmc(struct NES *nes)
{
    if(nes->mmc.buffer.address == 0x00)
        return;

    switch(nes->mmc.rom.mapper)
    {
        case NROM:
            fprintf(stderr, "Shouldn't write to rom if mapper == NROM\n");
            break;
        case MMC1:
            step_mmc1(nes);
            break;
        default:
            fprintf(stderr, "Mapper not yet implemented: %x\n", nes->mmc.rom.mapper);
    }
    nes->mmc.buffer.address = 0x00;
}

void destroy_mmc(struct NES *nes)
{
    destroy_rom(nes);
}
