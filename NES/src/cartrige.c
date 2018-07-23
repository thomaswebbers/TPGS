#include <stdio.h>
#include <string.h>

#include "cartrige.h"

#define INES_HEADER_SIZE    ((byte_t) 0x10)
#define NES_SIGNATURE       "NES\x1A"

/*
 * iNES header contents:
 * =====================
 * 0x00 - 0x03      -NES signature. (NES\1A)
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
 *                  versions of the iNES format, assume 1 page of RAM when
 *                  this is 0.
 * 0x09 - 0x0E      -Reserved for future usage and should all be 0.
 */

bool load_cartrige(struct Cartrige *cartrige_handle, char *binary_file)
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

    fclose(fp);
    return true;
}
