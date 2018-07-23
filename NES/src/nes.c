#include "nes.h"
#include <stdlib.h>
#include <stdio.h>

//opcode function headers:
#include "imm.h"
#include "zp.h"
#include "abs.h"
#include "imp.h"
#include "acc.h"
#include "ixx.h"
#include "ixy.h"
#include "zpixx.h"
#include "zpixy"
#include "idr.h"
#include "preii.h"
#include "posii.h"
#include "rel.h"

void run_nes(struct NES *nes_handle)
{
    while(step_nes(nes_handle))
    {
        ;
    }
}

int init_nes(struct NES **nes_handle, char *binary_file)
{
    FILE *fp;
    byte_t nesMagic[4];

    //open file
    *fp = fopen(binary_file, "rb");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open filepath\n");
        return -1;
    }

    //read magic number to check if file is nes rom
    fread(nesMagic, sizeof(byte_t), strlen(NES_MAGIC)-1, fp);
    if(memcmp(nesMagic, NES_MAGIC, strlen(NES_MAGIC)-1) != 0)
    {
        fprintf(stderr, "File is not a NES rom\n")
        return -1;
    }

    *nes_handle = malloc(sizeof(NES));
    return INIT_SUCCES;
}

void destroy_nes(struct NES *nes_handle)
{
    free(nes_handle);
}

bool step_nes(struct NES *nes_handle)
{
    cpu_step(nes_handle);
}
