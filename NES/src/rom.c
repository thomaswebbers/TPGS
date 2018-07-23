#include "rom.h"
#include "stdio.h"
#include "string.h"

bool load_rom(struct NES *nes_handle, char *binary_file)
{
    FILE *fp;
    byte_t nes_signature[4];

    //open file
    fp = fopen(binary_file, "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open filepath\n");
        return false;
    }

    //read nes_signature to check if file is nes rom
    fread(nes_signature, sizeof(byte_t), 4, fp);
    if(memcmp(nes_signature, NES_SIGNATURE, strlen(NES_SIGNATURE)) != 0)
    {
        fprintf(stderr, "File is not a NES rom\n");
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}
