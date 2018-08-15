#ifndef NES_H
#define NES_H
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "ppu.h"
#include "mmc.h"

#define INIT_SUCCES         ((uint32_t) 0x00)
#define INIT_FAILURE        ((uint32_t) 0xFF)

struct NES
{
    uint32_t master_clock;

    struct CPU cpu;

    struct PPU ppu;

    struct MMC mmc;
};

/*
 * Allocates memory for the nes, loading the rom from the file path into the
 * memory of the NES virtual machine.
 *
 * @param nes_handle a handle to the nes virtual machine
 * @param binary_path a path to the rom to load into memory

 * @return flag for succes status
 */
int init_nes(struct NES **nes, char *binary_file);

/*
 * Steps through the rom to execute the game.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return void
 */
void run_nes(struct NES *nes);

/*
 * Handles the next opcode.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return false if end of program is reached
 */
bool step_nes(struct NES *nes);

/*
 * Deallocates the memory used by the nes.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return void
 */
void destroy_nes(struct NES *nes);

#endif
