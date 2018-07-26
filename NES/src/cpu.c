#include <stdio.h>
#include <stdint.h>
#include "nes.h"
#include "opcodes.h"
#include "cpu.h"
#include "cpumem.h"
#include "cpupsr.h"

void init_cpu(struct CPU *cpu_handle)
{
    cpu_handle->clock = 0;
    cpu_handle->pc = cpumem_reads(&cpu_handle->memory, RESET_VEC);
    cpu_handle->sp = 0;
    cpu_handle->A = 0;
    cpu_handle->X = 0;
    cpu_handle->Y = 0;
    init_psr(&cpu_handle->P);
}

void destroy_cpu(struct CPU *cpu_handle)
{

}

void cpu_add_cycles(struct CPU *cpu_handle, uint32_t cycles)
{
    cpu_handle->clock += cycles;
}

bool cpu_step(struct CPU *cpu_handle)
{
    printf("opcode[%x]: %x ", cpu_handle->pc, cpumem_readb(&cpu_handle->memory, cpu_handle->pc));
    unsigned int cycles = 0;
    uint16_t pc_inc = 0;
    switch(cpumem_readb(&cpu_handle->memory, cpu_handle->pc))
    {
        /***********************************************
        **************Immediate Addressing**************
        ***********************************************/
        case IMM_ADC: case IMM_AND: case IMM_CMP:
        case IMM_CPX: case IMM_CPY: case IMM_EOR:
        case IMM_LDA: case IMM_LDX: case IMM_LDY:
        case IMM_ORA: case IMM_SBC:
            printf("Immediate\n");
            break;

        /***********************************************
        **************Zero-Page Addressing**************
        ***********************************************/
        case ZP_ADC: case ZP_AND: case ZP_ASL:
        case ZP_BIT: case ZP_CMP: case ZP_CPX:
        case ZP_CPY: case ZP_DEC: case ZP_EOR:
        case ZP_INC: case ZP_LDA: case ZP_LDX:
        case ZP_LDY: case ZP_LSR: case ZP_ORA:
        case ZP_ROL: case ZP_ROR: case ZP_SBC:
        case ZP_STA: case ZP_STX: case ZP_STY:
            printf("Zero Page\n");
            break;

        /***********************************************
        **************Absolute Addressing***************
        ***********************************************/
        case ABS_ADC: case ABS_AND: case ABS_ASL:
        case ABS_BIT: case ABS_CMP: case ABS_CPX:
        case ABS_CPY: case ABS_DEC: case ABS_EOR:
        case ABS_INC: case ABS_JSR: case ABS_LDA:
        case ABS_LDX: case ABS_LDY: case ABS_LSR:
        case ABS_ORA: case ABS_ROL: case ABS_ROR:
        case ABS_SBC: case ABS_STA: case ABS_STX:
        case ABS_STY:
            printf("Absolute");
            break;

        /***********************************************
        **************Implied Addresssing***************
        ***********************************************/
        case IMP_BRK: case IMP_CLC: case IMP_CLD:
        case IMP_CLI: case IMP_CLV: case IMP_DEX:
        case IMP_DEY: case IMP_INX: case IMP_INY:
        case IMP_NOP: case IMP_PHA: case IMP_PHP:
        case IMP_PLA: case IMP_PLP: case IMP_RTI:
        case IMP_RTS: case IMP_SEI: case IMP_TAX:
        case IMP_TAY: case IMP_TSX: case IMP_TXA:
        case IMP_TXS: case IMP_TYA:
            printf("Implied\n");
            break;

        /***********************************************
        **************Accumulator Addressing************
        ***********************************************/
        case ACC_ASL: case ACC_LSR: case ACC_ROL:
        case ACC_ROR:
            printf("Accumulator\n");
            break;

        /***********************************************
        ******Indexed Addressing (index register X)*****
        ***********************************************/
        case IXX_ADC: case IXX_AND: case IXX_ASL:
        case IXX_CMP: case IXX_DEC: case IXX_EOR:
        case IXX_INC: case IXX_LDA: case IXX_LDY:
        case IXX_LSR: case IXX_ORA: case IXX_ROL:
        case IXX_ROR: case IXX_SBC: case IXX_STA:
            printf("Indexed X\n");
            break;

        /***********************************************
        ******Indexed Addressing (index register Y)*****
        ***********************************************/
        case IXY_ADC: case IXY_AND: case IXY_CMP:
        case IXY_EOR: case IXY_LDA: case IXY_LDX:
        case IXY_ORA: case IXY_SBC: case IXY_STA:
            printf("Indexed Y\n");
            break;

        /***********************************************
        *Zero-Page Indexed Addressing(index register X)*
        ***********************************************/
        case ZPIXX_ADC: case ZPIXX_AND: case ZPIXX_ASL:
        case ZPIXX_CMP: case ZPIXX_DEC: case ZPIXX_EOR:
        case ZPIXX_INC: case ZPIXX_LDA: case ZPIXX_LDY:
        case ZPIXX_LSR: case ZPIXX_ORA: case ZPIXX_ROL:
        case ZPIXX_ROR: case ZPIXX_SBC: case ZPIXX_STA:
        case ZPIXX_STY:
            printf("Zero Page Indexed X\n");
            break;

        /***********************************************
        *Zero-Page Indexed Addressing(index register Y)*
        ***********************************************/
        case ZPIXY_LDX: case ZPIXY_STX:
            printf("Zero Page Indexed Y\n");
            break;

        /***********************************************
        **************Indirect Addressing***************
        ***********************************************/
        case IDR_JMP:
            printf("Indirect\n");
            break;

        /***********************************************
        ********Pre-Indexed Indirect Addressing*********
        ***********************************************/
        case PREII_ADC: case PREII_AND: case PREII_CMP:
        case PREII_EOR: case PREII_LDA: case PREII_ORA:
        case PREII_SBC: case PREII_STA:
            printf("Pre-Indexed Indirect\n");
            break;

        /***********************************************
        ********Post-Indexed Indirect Addressing********
        ***********************************************/
        case POSII_ADC: case POSII_AND: case POSII_CMP:
        case POSII_EOR: case POSII_LDA: case POSII_ORA:
        case POSII_SBC: case POSII_STA:
            printf("Post-Indexed Indirect\n");
            break;

        /***********************************************
        **************Relative Addressing***************
        ***********************************************/
        case REL_BCC: case REL_BCS: case REL_BEQ:
        case REL_BMI: case REL_BNE: case REL_BPL:
        case REL_BVC: case REL_BVS:
            printf("Relative\n");
            break;

        default:
            fprintf(stderr, "DEFAULT: %x\n", cpumem_readb(&cpu_handle->memory, cpu_handle->pc));
            return false;
    }
    switch()
    {

    }
    cpu_handle->clock += cycles;
    cpu_handle->pc += pc_inc;
    return true;
}

void cpu_push(struct CPU *cpu_handle, byte_t data)
{
    cpu_handle->memory.RAM[0x100 + cpu_handle->sp++];
}

byte_t cpu_pop(struct CPU *cpu_handle)
{
    return cpu_handle->memory.RAM[0x100 + --cpu_handle->sp];
}

byte_t cpu_peek(struct CPU *cpu_handle)
{
    return cpu_handle->memory.RAM[0x100 + cpu_handle->sp - 1];
}

void cpu_adc(struct CPU *cpu_handle, byte_t operand)
{
    uint16_t temp = operand + cpu_handle->A + cpu_handle->P.C;
    cpu_handle->P.V =   !((cpu_handle->A ^ operand) & 0x80)
                        && ((cpu_handle->A ^ temp) & 0x80);
    cpu_handle->A = (byte_t) temp;
    cpu_handle->P.Z = (cpu_handle->A == 0);
    cpu_handle->P.S = (cpu_handle->A > 0x7F);
    cpu_handle->P.C = (temp > 0xff);
}

void cpu_and(struct CPU *cpu_handle, byte_t operand)
{
    cpu_handle->A &= operand;
    cpu_handle->P.Z = (cpu_handle->A == 0);
    cpu_handle->P.S = (cpu_handle->A > 0x7F);
}

void cpu_asl(struct CPU *cpu_handle, byte_t *operand)
{
    cpu_handle->P.C = (*operand > 0x7F);
    *operand <<= 1;
    cpu_handle->P.Z = (*operand == 0);
    cpu_handle->P.S = (*operand > 0x7F);
}

void cpu_bcc(struct CPU *cpu_handle, int8_t offset)
{
    if(!cpu_handle->P.C)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_bcs(struct CPU *cpu_handle, int8_t offset)
{
    if(!cpu_handle->P.C)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_beq(struct CPU *cpu_handle, int8_t offset)
{
    if(cpu_handle->P.Z)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_bit(struct CPU *cpu_handle, byte_t operand)
{
    cpu_handle->P.S = (operand > 0x7F);
    cpu_handle->P.V = ((operand & 0x40) > 0x3F);
    cpu_handle->P.Z = ((operand & cpu_handle->A) == 0);
}

void cpu_bmi(struct CPU *cpu_handle, int8_t offset)
{
    if(cpu_handle->P.S)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_bne(struct CPU *cpu_handle, int8_t offset)
{
    if(!cpu_handle->P.Z)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_bpl(struct CPU *cpu_handle, int8_t offset)
{
    if(!cpu_handle->P.S)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_brk(struct CPU *cpu_handle)
{

}

void cpu_bvc(struct CPU *cpu_handle)
{

}

void cpu_bvs(struct CPU *cpu_handle)
{

}

void cpu_clc(struct CPU *cpu_handle)
{

}

void cpu_cld(struct CPU *cpu_handle)
{

}

void cpu_cli(struct CPU *cpu_handle)
{

}

void cpu_clv(struct CPU *cpu_handle)
{

}

void cpu_cmp(struct CPU *cpu_handle)
{

}

void cpu_cpx(struct CPU *cpu_handle)
{

}

void cpu_cpy(struct CPU *cpu_handle)
{

}

void cpu_dec(struct CPU *cpu_handle)
{

}

void cpu_dex(struct CPU *cpu_handle)
{

}

void cpu_dey(struct CPU *cpu_handle)
{

}

void cpu_eor(struct CPU *cpu_handle)
{

}

void cpu_inc(struct CPU *cpu_handle)
{

}

void cpu_inx(struct CPU *cpu_handle)
{

}

void cpu_iny(struct CPU *cpu_handle)
{

}

void cpu_jmp(struct CPU *cpu_handle, uint16_t address)
{
    cpu_handle->pc = address;
}

void cpu_jsr(struct CPU *cpu_handle, uint16_t address)
{
    cpu_handle->pc += 2;
    cpu_push(cpu_handle, (byte_t) (cpu_handle->pc >> 8));
    cpu_push(cpu_handle, (byte_t) cpu_handle->pc);
    cpu_handle->pc = address;
}

void cpu_lda(struct CPU *cpu_handle, byte_t operand)
{
    cpu_handle->P.S = (operand > 0x7F);
    cpu_handle->P.Z = (operand == 0);
    cpu_handle->A = operand;
}

void cpu_ldx(struct CPU *cpu_handle)
{

}

void cpu_ldy(struct CPU *cpu_handle)
{

}

void cpu_lsr(struct CPU *cpu_handle)
{

}

void cpu_nop(struct CPU *cpu_handle)
{

}

void cpu_ora(struct CPU *cpu_handle)
{

}

void cpu_pha(struct CPU *cpu_handle)
{

}

void cpu_php(struct CPU *cpu_handle)
{

}

void cpu_pla(struct CPU *cpu_handle)
{

}

void cpu_plp(struct CPU *cpu_handle)
{

}

void cpu_rol(struct CPU *cpu_handle)
{

}

void cpu_ror(struct CPU *cpu_handle)
{

}

void cpu_rti(struct CPU *cpu_handle)
{

}

void cpu_rts(struct CPU *cpu_handle)
{

}

void cpu_sbc(struct CPU *cpu_handle)
{

}

void cpu_sec(struct CPU *cpu_handle)
{

}

void cpu_sed(struct CPU *cpu_handle)
{

}

void cpu_sei(struct CPU *cpu_handle)
{
    cpu_handle->P.I = true;
}

void cpu_sta(struct CPU *cpu_handle, uint16_t address)
{
    printf("address: %x\n", address);
    printf("ptr: %p\n", cpu_handle->memory.PPU_IO);
    cpumem_writeb(&cpu_handle->memory, address, cpu_handle->A);
    printf("test?\n");
}

void cpu_stx(struct CPU *cpu_handle)
{

}

void cpu_sty(struct CPU *cpu_handle)
{

}

void cpu_tax(struct CPU *cpu_handle)
{

}

void cpu_tay(struct CPU *cpu_handle)
{

}

void cpu_tsx(struct CPU *cpu_handle)
{

}

void cpu_txa(struct CPU *cpu_handle)
{

}

void cpu_txs(struct CPU *cpu_handle)
{

}

void cpu_tya(struct CPU *cpu_handle)
{

}
