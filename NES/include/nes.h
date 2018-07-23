#ifndef NES_H
#define NES_H
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "ppu.h"
#include "cartrige.h"



#define INIT_SUCCES         ((uint32_t) 0x00)
#define INIT_FAILURE        ((uint32_t) 0xFF)

/*
#define SET_NEGATIVE_FLAG(nes_handle, operand)
    ((operand) > 0x7F
        ? nes_handle->cpu.P = nes_handle->cpu.P | 0x80
        : nes_handle->cpu.P = nes_handle->cpu.P & 0x7F)

#define SET_ZERO_FLAG(nes_handle, operand)
    (operand == 0
        ? nes_handle->cpu.P = nes_handle->cpu.P | 0x02
        : nes_handle->cpu.P = nes_handle->cpu.P & 0xFD)
*/
struct NES
{
    uint32_t master_clock;

    struct CPU cpu;

    struct PPU ppu;

    struct Cartrige cartrige;
};

/*
 * Allocates memory for the nes, loading the rom from the file path into the
 * memory of the NES virtual machine.
 *
 * @param nes_handle a handle to the nes virtual machine
 * @param binary_path a path to the rom to load into memory

 * @return flag for succes status
 */
int init_nes(struct NES **nes_handle, char *binary_path);

/*
 * Steps through the rom to execute the game.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return void
 */
void run_nes(struct NES *nes_handle);

/*
 * Handles the next opcode.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return false if end of program is reached
 */
bool step_nes(struct NES *nes_handle);

/*
 * Deallocates the memory used by the nes.
 *
 * @param nes_handle a handle to the nes virtual machine
 *
 * @return void
 */
void destroy_nes(struct NES *nes_handle);

#endif
