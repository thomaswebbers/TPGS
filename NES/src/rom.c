#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rom.h"

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

bool init_rom(struct ROM *rom_handle, char *binary_file)
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

    //amount of prg banks
    rom_handle->num_prg = header[0x04];
    printf("num_prg:\t%x\n", rom_handle->num_prg);

    //amount of chr banks
    rom_handle->num_chr = header[0x05];
    printf("num_chr:\t%x\n", rom_handle->num_chr);

    //type of mirror (0x01 for vertical/horizontal, 0x02 for fourscreen)
    rom_handle->mirror = ((header[0x06] & 0x08) >> 0x02) | (header[0x06] & 0x01);
    printf("mirror:\t\t%x\n", rom_handle->mirror);

    //cartrige contains battery backed ram
    rom_handle->battery = (header[0x06] & 0x02) >> 0x01;
    printf("battery:\t%x\n", rom_handle->battery);

    //cartrige contains trainer UNUSED
    byte_t trainer = (header[0x06] & 0x04) >> 0x02;
    printf("trainer:\t%x\n", trainer);

    //mapper type
    rom_handle->mapper = (header[0x07] & 0xF0) | ((header[0x06] & 0xF0) >> 0x04);
    printf("mapper:\t\t%x\n", rom_handle->mapper);

    //amount of ram banks
    byte_t numRAM = header[0x08];
    if(numRAM == 0)
        numRAM = 1;
    printf("numRAM:\t\t%x\tTODO :)\n", numRAM);

    //allocate all banks for prg rom
    rom_handle->prg = malloc(sizeof(*byte_t) * rom_handle->num_prg);
    for(int i = 0; i < rom_handle->num_prg; i++)
        rom_handle->prg[i] = malloc(sizeof(byte_t) * PRG_ROM_BANK_SIZE);

    //TODO: read prg rom

    //allocate all banks for chr rom
    rom_handle->chr = malloc(sizeof(*byte_t) * rom_handle->num_chr);
    for(int i = 0; i < rom_handle->num_chr; i++)
        rom_handle->chr[i] = malloc(sizeof(byte_t) * CHR_ROM_BANK_SIZE);

    //TODO: read chr rom

    fclose(fp);
    return true;
}

void destroy_rom(struct ROM *rom_handle)
{

}