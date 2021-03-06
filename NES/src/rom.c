#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nes.h"

#define INES_HEADER_SIZE    ((byte_t) 0x10)
#define NES_SIGNATURE       "NES\x1A"

/*
 * iNES header contents:
 * =====================
 * 0x00 - 0x03      -NES signature in ascii. (NES\1A)
 * 0x04             -Number of 16 KB PRG-ROM banks.
 * 0x05             -Number of 8 KB CHR-ROM / VROM banks.
 * 0x06             -ROM Control Byte 1:
 *                  *==>Bit 0 - Indicates the type of mirroring used by the game
 *                      where 0 indicates horizontal mirroring, 1 indicates
 *                      vertical mirroring.
 *                  *==>Bit 1 - Indicates the presence of battery-backed RAM at
 *                      memory locations $6000-$7FFF.
 *                  *==>Bit 2 - Indicates the presence of a 512-byte trainer at
 *                      memory locations $7000-$71FF.
 *                  *==>Bit 3 - If this bit is set it overrides bit 0 to
 *                      indicate fourscreen mirroring should be used.
 *                  *==>Bits 4-7 - Four lower bits of the mapper number.
 * 0x07             -ROM Control Byte 2:
 *                  *==>Bits 0-3 - Reserved for future usage and should all be 0.
 *                  *==>Bits 4-7 - Four upper bits of the mapper number.
 * 0x08             -Number of 8 KB RAM banks. For compatibility with previous
 *                   versions of the iNES format, assume 1 page of RAM when
 *                   this is 0.
 * 0x09 - 0x0E      -Reserved for future usage and should all be 0.
 */

bool init_rom(struct NES *nes, char *binary_file)
{
    FILE *fp;
    byte_t header[INES_HEADER_SIZE];

    //open file
    fp = fopen(binary_file, "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open filepath\n");
        return false;
    }

    //read header
    fread(header, sizeof(byte_t), INES_HEADER_SIZE, fp);

    //check for nes signature
    if(memcmp(header, NES_SIGNATURE, strlen(NES_SIGNATURE)) != 0)
    {
        fprintf(stderr, "File is not a NES rom\n");
        fclose(fp);
        return false;
    }

    //info on ines format found above
    nes->mmc.rom.num_prg = header[0x04];
    nes->mmc.rom.num_chr = header[0x05];
    nes->mmc.rom.mirror = ((header[0x06] & 0x08) >> 0x02) | (header[0x06] & 0x01);
    nes->mmc.rom.battery = (header[0x06] & 0x02) >> 0x01;
    byte_t trainer = (header[0x06] & 0x04) >> 0x02;     //UNUSED
    nes->mmc.rom.mapper = (header[0x07] & 0xF0) | ((header[0x06] & 0xF0) >> 0x04);
    byte_t numRAM = header[0x08];
    if(numRAM == 0)
        numRAM = 1;

    //test :D
    nes->mmc.rom.sram = malloc(sizeof(byte_t) * 0x2000);

    //move past trainer data
    if(trainer == 1)
        fseek(fp, 0x200, SEEK_CUR);

    //allocate and read all banks for prg rom
    nes->mmc.rom.prg = malloc(sizeof(byte_t *) * nes->mmc.rom.num_prg * PRG_ROM_BANK_SIZE);
    fread(nes->mmc.rom.prg, sizeof(byte_t), PRG_ROM_BANK_SIZE * nes->mmc.rom.num_prg, fp);

    //allocate and read all banks for chr rom
    nes->mmc.rom.chr = malloc(sizeof(byte_t *) * nes->mmc.rom.num_chr * CHR_ROM_BANK_SIZE);
    fread(nes->mmc.rom.chr, sizeof(byte_t), CHR_ROM_BANK_SIZE * nes->mmc.rom.num_chr, fp);

    fclose(fp);
    return true;
}

void destroy_rom(struct NES *nes)
{
    free(nes->mmc.rom.prg);

    free(nes->mmc.rom.chr);

    free(nes->mmc.rom.sram);
}
