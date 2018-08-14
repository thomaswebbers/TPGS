#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include "opcodes.h"
#include "cpumem.h"
#include "psr.h"
#include "mmcbuf.h"

struct NES;

struct CPU
{
    /*
     * Every processing unit in the nes gets its own clock to match against the
     * master clock. (i.e. to maintain same clock speed as original NES)
     */
    uint32_t clock;

    /*
     * Registers
     * ==========
     * Program Counter  (16bit)
     * Stack pointer    (8bit)
     * Accumulator      (8bit)
     * Index Register X (8bit)
     * Index Register Y (8bit)
     */
    uint16_t pc;
    byte_t sp;
    byte_t A;
    byte_t X;
    byte_t Y;

    /*
     * Processor Status
     * =================
     * 0x01    - Carry (if last instruction resulted in under/overflow)
     * 0x02    - Zero (if last instruction's result was 0)
     * 0x04    - Interrupt Disable (Enable to prevent system from responding to interrupts)
     * 0x08    - Decimal mode (unsupported on this chip variant)
     * 0x10    - Break Command (if BRK (break) instruction has been executed)
     * 0x20    - Empty
     * 0x40    - Overflow (if previous instruction resulted in an invalid two's complement)
     * 0x80    - Negative
     */
    struct PSR P;

    /*
     * MEMORY LAYOUT:
     * ==============
     * 0x100   - Zero Page
     * 0x200   - Stack
     * 0x800   - RAM
     * 0x2000  - Mirrors (0-0x7FF)
     * 0x2008  - I/O Registers
     * 0x4000  - Mirrors (0x2000-0x2007)
     * 0x4020  - I/O Registers
     * 0x6000  - Expansion ROM
     * 0x8000  - SRAM
     * 0xC000  - PRG-ROM (Lower Bank)
     * 0x10000 - PRG-ROM (Upper Bank)
     */
    struct CPUmem memory;
};

/*
 * Sets all registers in the CPU to 0. Initiates the pc with the reset vector.
 * The addresses for the mmc buffers are also set.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return void
 */
void init_cpu(struct NES *nes);

/*
 * Adds an amount of cycles to the clock of the cpu. The amount added should
 * differs depending on the opcode executed.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 * @param cycles the amount of cycles that should be added to the cpu clock
 *
 * @return void
 */
void cpu_add_cycles(struct NES *nes, uint32_t cycles);

/*
 * Executes the next opcode of the rom.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return false if proces should end
 */
bool cpu_step(struct NES *nes);

/*
 * Cleans up the resources used by the cpu.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return void
 */
void destroy_cpu(struct NES *nes);

/*
 * Pushes the top byte on the stack in the CPUmemory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 * @param data the byte to be pushed on the stack
 *
 * @return void
 */
void cpu_push(struct NES *nes, byte_t data);

/*
 * Pops the top item of the stack in the CPUmemory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return top byte of the stack
 */
byte_t cpu_pop(struct NES *nes);

/*
 * Peeks at the top item on the stack in the CPUmemory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return top byte of the stack
 */
byte_t cpu_peek(struct NES *nes);

void cpu_imm(struct NES *nes, byte_t **arg);

void cpu_zp(struct NES *nes, byte_t **arg);

void cpu_abs(struct NES *nes, byte_t **arg);

void cpu_acc(struct NES *nes, byte_t **arg);

void cpu_ixx(struct NES *nes, byte_t **arg);

void cpu_ixy(struct NES *nes, byte_t **arg);

void cpu_zpixx(struct NES *nes, byte_t **arg);

void cpu_zpixy(struct NES *nes, byte_t **arg);

void cpu_idr(struct NES *nes, byte_t **arg);

void cpu_preii(struct NES *nes, byte_t **arg);

void cpu_posii(struct NES *nes, byte_t **arg);

void cpu_rel(struct NES *nes, byte_t **arg);

void cpu_adc(struct NES *nes, byte_t arg);

void cpu_and(struct NES *nes, byte_t arg);

void cpu_asl(struct NES *nes, byte_t *arg);

void cpu_bcc(struct NES *nes, int8_t offset);

void cpu_bcs(struct NES *nes, int8_t offset);

void cpu_beq(struct NES *nes, int8_t offset);

void cpu_bit(struct NES *nes, byte_t arg);

void cpu_bmi(struct NES *nes, int8_t offset);

void cpu_bne(struct NES *nes, int8_t offset);

void cpu_bpl(struct NES *nes, int8_t offset);

void cpu_brk(struct NES *nes);

void cpu_bvc(struct NES *nes, int8_t offset);

void cpu_bvs(struct NES *nes, int8_t offset);

void cpu_clc(struct NES *nes);

void cpu_cld(struct NES *nes);

void cpu_cli(struct NES *nes);

void cpu_clv(struct NES *nes);

void cpu_cmp(struct NES *nes, byte_t arg);

void cpu_cpx(struct NES *nes, byte_t arg);

void cpu_cpy(struct NES *nes, byte_t arg);

void cpu_dec(struct NES *nes, byte_t *arg);

void cpu_dex(struct NES *nes);

void cpu_dey(struct NES *nes);

void cpu_eor(struct NES *nes, byte_t arg);

void cpu_inc(struct NES *nes, byte_t *arg);

void cpu_inx(struct NES *nes);

void cpu_iny(struct NES *nes);

void cpu_jmp(struct NES *nes, byte_t *arg);

void cpu_jsr(struct NES *nes, byte_t *arg);

void cpu_lda(struct NES *nes, byte_t arg);

void cpu_ldx(struct NES *nes, byte_t arg);

void cpu_ldy(struct NES *nes, byte_t arg);

void cpu_lsr(struct NES *nes, byte_t *arg);

void cpu_ora(struct NES *nes, byte_t arg);

void cpu_pha(struct NES *nes);

void cpu_php(struct NES *nes);

void cpu_pla(struct NES *nes);

void cpu_plp(struct NES *nes);

void cpu_rol(struct NES *nes, byte_t *arg);

void cpu_ror(struct NES *nes, byte_t *arg);

void cpu_rti(struct NES *nes);

void cpu_rts(struct NES *nes);

void cpu_sbc(struct NES *nes, byte_t arg);

void cpu_sec(struct NES *nes);

void cpu_sed(struct NES *nes);

void cpu_sei(struct NES *nes);

void cpu_sta(struct NES *nes, byte_t *arg);

void cpu_stx(struct NES *nes, byte_t *arg);

void cpu_sty(struct NES *nes, byte_t *arg);

void cpu_tax(struct NES *nes);

void cpu_tay(struct NES *nes);

void cpu_tsx(struct NES *nes);

void cpu_txa(struct NES *nes);

void cpu_txs(struct NES *nes);

void cpu_tya(struct NES *nes);

#endif
