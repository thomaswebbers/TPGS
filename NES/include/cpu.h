#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include "opcodes.h"
#include "cpumem.h"
#include "cpupsr.h"

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
 * Sets all registers in the CPU to 0.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return void
 */
void init_cpu(struct CPU *cpu_handle);

/*
 * Adds an amount of cycles to the clock of the cpu. The amount added should
 * differs depending on the opcode executed.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 * @param cycles the amount of cycles that should be added to the cpu clock
 *
 * @return void
 */
void cpu_add_cycles(struct CPU *cpu_handle, uint32_t cycles);

/*
 * Executes the next opcode of the rom.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return false if proces should end
 */
bool cpu_step(struct CPU *cpu_handle);

/*
 * Cleans up the resources used by the cpu.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return void
 */
void destroy_cpu(struct CPU *cpu_handle);

/*
 * Pushes the top byte on the stack in the CPUmemory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 * @param data the byte to be pushed on the stack
 *
 * @return void
 */
void cpu_push(struct CPU *cpu_handle, byte_t data);

/*
 * Pops the top item of the stack in the CPUmemory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return top byte of the stack
 */
byte_t cpu_pop(struct CPU *cpu_handle);

/*
 * Peeks at the top item on the stack in the CPUmemory.
 *
 * @param cpu_handle a handle to the nes virtual machine cpu
 *
 * @return top byte of the stack
 */
byte_t cpu_peek(struct CPU *cpu_handle);

void cpu_imm(struct CPU *cpu_handle);

void cpu_zp(struct CPU *cpu_handle);

void cpu_abs(struct CPU *cpu_handle);

void cpu_imp(struct CPU *cpu_handle);

void cpu_acc(struct CPU *cpu_handle);

void cpu_ixx(struct CPU *cpu_handle);

void cpu_ixy(struct CPU *cpu_handle);

void cpu_zpixx(struct CPU *cpu_handle);

void cpu_zpixy(struct CPU *cpu_handle);

void cpu_idr(struct CPU *cpu_handle);

void cpu_preii(struct CPU *cpu_handle);

void cpu_posii(struct CPU *cpu_handle);

void cpu_rel(struct CPU *cpu_handle);

void cpu_adc(struct CPU *cpu_handle, byte_t operand);

void cpu_and(struct CPU *cpu_handle, byte_t operand);

void cpu_asl(struct CPU *cpu_handle, byte_t *operand);

void cpu_bcc(struct CPU *cpu_handle, int8_t offset);

void cpu_bcs(struct CPU *cpu_handle, int8_t offset);

void cpu_beq(struct CPU *cpu_handle, int8_t offset);

void cpu_bit(struct CPU *cpu_handle, byte_t operand);

void cpu_bmi(struct CPU *cpu_handle, int8_t offset);

void cpu_bne(struct CPU *cpu_handle, int8_t offset);

void cpu_bpl(struct CPU *cpu_handle, int8_t offset);

void cpu_brk(struct CPU *cpu_handle);

void cpu_bvc(struct CPU *cpu_handle);

void cpu_bvs(struct CPU *cpu_handle);

void cpu_clc(struct CPU *cpu_handle);

void cpu_cld(struct CPU *cpu_handle);

void cpu_cli(struct CPU *cpu_handle);

void cpu_clv(struct CPU *cpu_handle);

void cpu_cmp(struct CPU *cpu_handle);

void cpu_cpx(struct CPU *cpu_handle);

void cpu_cpy(struct CPU *cpu_handle);

void cpu_dec(struct CPU *cpu_handle);

void cpu_dex(struct CPU *cpu_handle);

void cpu_dey(struct CPU *cpu_handle);

void cpu_eor(struct CPU *cpu_handle);

void cpu_inc(struct CPU *cpu_handle);

void cpu_inx(struct CPU *cpu_handle);

void cpu_iny(struct CPU *cpu_handle);

void cpu_jmp(struct CPU *cpu_handle, uint16_t address);

void cpu_jsr(struct CPU *cpu_handle, uint16_t address);

void cpu_lda(struct CPU *cpu_handle, byte_t operand);

void cpu_ldx(struct CPU *cpu_handle);

void cpu_ldy(struct CPU *cpu_handle);

void cpu_lsr(struct CPU *cpu_handle);

void cpu_nop(struct CPU *cpu_handle);

void cpu_ora(struct CPU *cpu_handle);

void cpu_pha(struct CPU *cpu_handle);

void cpu_php(struct CPU *cpu_handle);

void cpu_pla(struct CPU *cpu_handle);

void cpu_plp(struct CPU *cpu_handle);

void cpu_rol(struct CPU *cpu_handle);

void cpu_ror(struct CPU *cpu_handle);

void cpu_rti(struct CPU *cpu_handle);

void cpu_rts(struct CPU *cpu_handle);

void cpu_sbc(struct CPU *cpu_handle);

void cpu_sec(struct CPU *cpu_handle);

void cpu_sed(struct CPU *cpu_handle);

void cpu_sei(struct CPU *cpu_handle);

void cpu_sta(struct CPU *cpu_handle, uint16_t address);

void cpu_stx(struct CPU *cpu_handle);

void cpu_sty(struct CPU *cpu_handle);

void cpu_tax(struct CPU *cpu_handle);

void cpu_tay(struct CPU *cpu_handle);

void cpu_tsx(struct CPU *cpu_handle);

void cpu_txa(struct CPU *cpu_handle);

void cpu_txs(struct CPU *cpu_handle);

void cpu_tya(struct CPU *cpu_handle);

#endif
