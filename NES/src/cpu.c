#include <stdio.h>
#include <stdint.h>
#include "nes.h"
#include "opcodes.h"
#include "cpu.h"
#include "cpumem.h"
#include "psr.h"
#include "mmcbuf.h"

const byte_t opcode_cycles[256] =
{
	7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0,
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
	6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0,
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
	6, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 3, 4, 6, 0,
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
	6, 6, 0, 0, 0, 3, 5, 0, 4, 2, 2, 0, 5, 4, 6, 0,
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
	0, 6, 0, 0, 3, 3, 3, 0, 2, 0, 2, 0, 4, 4, 4, 0,
	2, 6, 0, 0, 4, 4, 4, 0, 2, 5, 2, 0, 0, 5, 0, 0,
	2, 6, 2, 0, 3, 3, 3, 0, 2, 2, 2, 0, 0, 0, 0, 4,
	2, 5, 0, 0, 4, 4, 4, 0, 2, 4, 2, 0, 4, 4, 4, 0,
	2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0,
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
	2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0,
	2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
};

char op_names[256][4] = {
	"BRK", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
	"PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
	"BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
	"CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
	"JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA",
	"PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
	"BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA",
	"SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
	"RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
	"PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
	"BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
	"CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
	"RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
	"PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
	"BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
	"SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
	"NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX",
	"DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
	"BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX",
	"TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
	"LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX",
	"TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
	"BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX",
	"CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
	"CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP",
	"INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
	"BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP",
	"CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
	"CPX", "SBC", "NOP", "ISC", "CPX", "SBC", "INC", "ISC",
	"INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISC",
	"BEQ", "SBC", "KIL", "ISC", "NOP", "SBC", "INC", "ISC",
	"SED", "SBC", "NOP", "ISC", "NOP", "SBC", "INC", "ISC",
};

void init_cpu(struct CPU *cpu_handle, struct MMCbuf *buffer)
{
    cpu_handle->clock = 0;
    cpu_handle->pc = cpumem_reads(&cpu_handle->memory, RESET_VEC);
    cpu_handle->sp = 0;
    cpu_handle->A = 0;
    cpu_handle->X = 0;
    cpu_handle->Y = 0;
    init_psr(&cpu_handle->P);
	init_cpumem(&cpu_handle->memory, buffer);
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
    byte_t opcode = *cpumem_readbp(&cpu_handle->memory, cpu_handle->pc);
    printf("opcode[%x]: %x ", cpu_handle->pc, opcode);
    byte_t *arg;
    cpu_handle->pc++;
    cpu_handle->clock += opcode_cycles[opcode];
    switch(opcode)
    {
        /***********************************************
        **************Immediate Addressing**************
        ***********************************************/
        case IMM_ADC: case IMM_AND: case IMM_CMP:
        case IMM_CPX: case IMM_CPY: case IMM_EOR:
        case IMM_LDA: case IMM_LDX: case IMM_LDY:
        case IMM_ORA: case IMM_SBC:
			printf("IMM_");
            cpu_imm(cpu_handle, &arg);
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
			printf("ZP_");
            cpu_zp(cpu_handle, &arg);
            break;

        /***********************************************
        **************Absolute Addressing***************
        ***********************************************/
        case ABS_ADC: case ABS_AND: case ABS_ASL:
        case ABS_BIT: case ABS_CMP: case ABS_CPX:
        case ABS_CPY: case ABS_DEC: case ABS_EOR:
        case ABS_INC: case ABS_JMP: case ABS_JSR:
		case ABS_LDA: case ABS_LDX: case ABS_LDY:
		case ABS_LSR: case ABS_ORA: case ABS_ROL:
		case ABS_ROR: case ABS_SBC: case ABS_STA:
		case ABS_STX: case ABS_STY:
			printf("ABS_");
            cpu_abs(cpu_handle, &arg);
            break;

        /***********************************************
        **************Implied Addresssing***************
        ***********************************************/
        case IMP_BRK: case IMP_CLC: case IMP_CLD:
        case IMP_CLI: case IMP_CLV: case IMP_DEX:
        case IMP_DEY: case IMP_INX: case IMP_INY:
        case IMP_NOP: case IMP_PHA: case IMP_PHP:
        case IMP_PLA: case IMP_PLP: case IMP_RTI:
        case IMP_RTS: case IMP_SEC: case IMP_SED:
        case IMP_SEI: case IMP_TAX: case IMP_TAY:
        case IMP_TSX: case IMP_TXA: case IMP_TXS:
        case IMP_TYA:
			printf("IMP_");
            break;

        /***********************************************
        **************Accumulator Addressing************
        ***********************************************/
        case ACC_ASL: case ACC_LSR: case ACC_ROL:
        case ACC_ROR:
			printf("ACC_");
            cpu_acc(cpu_handle, &arg);
            break;

        /***********************************************
        ******Indexed Addressing (index register X)*****
        ***********************************************/
        case IXX_ADC: case IXX_AND: case IXX_ASL:
        case IXX_CMP: case IXX_DEC: case IXX_EOR:
        case IXX_INC: case IXX_LDA: case IXX_LDY:
        case IXX_LSR: case IXX_ORA: case IXX_ROL:
        case IXX_ROR: case IXX_SBC: case IXX_STA:
			printf("IXX_");
            cpu_ixx(cpu_handle, &arg);
            break;

        /***********************************************
        ******Indexed Addressing (index register Y)*****
        ***********************************************/
        case IXY_ADC: case IXY_AND: case IXY_CMP:
        case IXY_EOR: case IXY_LDA: case IXY_LDX:
        case IXY_ORA: case IXY_SBC: case IXY_STA:
			printf("IXY_");
            cpu_ixy(cpu_handle, &arg);
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
			printf("ZPIXX_");
            cpu_zpixx(cpu_handle, &arg);
            break;

        /***********************************************
        *Zero-Page Indexed Addressing(index register Y)*
        ***********************************************/
        case ZPIXY_LDX: case ZPIXY_STX:
			printf("ZPIXY_");
            cpu_zpixy(cpu_handle, &arg);
            break;

        /***********************************************
        **************Indirect Addressing***************
        ***********************************************/
        case IDR_JMP:
			printf("IDR_");
            cpu_idr(cpu_handle, &arg);
            break;

        /***********************************************
        ********Pre-Indexed Indirect Addressing*********
        ***********************************************/
        case PREII_ADC: case PREII_AND: case PREII_CMP:
        case PREII_EOR: case PREII_LDA: case PREII_ORA:
        case PREII_SBC: case PREII_STA:
			printf("PREII_");
            cpu_preii(cpu_handle, &arg);
            break;

        /***********************************************
        ********Post-Indexed Indirect Addressing********
        ***********************************************/
        case POSII_ADC: case POSII_AND: case POSII_CMP:
        case POSII_EOR: case POSII_LDA: case POSII_ORA:
        case POSII_SBC: case POSII_STA:
			printf("POSII_");
            cpu_posii(cpu_handle, &arg);
            break;

        /***********************************************
        **************Relative Addressing***************
        ***********************************************/
        case REL_BCC: case REL_BCS: case REL_BEQ:
        case REL_BMI: case REL_BNE: case REL_BPL:
        case REL_BVC: case REL_BVS:
			printf("REL_");
            cpu_rel(cpu_handle, &arg);
            break;

        default:
            fprintf(stderr, "DEFAULT: %x\n", opcode);
            return false;
    }
	printf("%c%c%c\n", op_names[opcode][0], op_names[opcode][1], op_names[opcode][2]);
    switch(opcode)
    {
        /***********************************************
        *************Add M to A with Carry**************
        ***********************************************/
        case IMM_ADC:   case ZP_ADC:    case ABS_ADC:
        case IXX_ADC:   case IXY_ADC:   case ZPIXX_ADC:
        case PREII_ADC: case POSII_ADC:
            cpu_adc(cpu_handle, *arg);
            break;

        /***********************************************
        *****************"AND" M with A*****************
        ***********************************************/
        case IMM_AND:   case ZP_AND:    case ABS_AND:
        case IXX_AND:   case IXY_AND:   case ZPIXX_AND:
        case PREII_AND: case POSII_AND:
            cpu_and(cpu_handle, *arg);
            break;

        /***********************************************
        **********Shift Left One Bit (M or A)***********
        ***********************************************/
        case ZP_ASL:    case ABS_ASL:   case ACC_ASL:
        case IXX_ASL:
            cpu_asl(cpu_handle, arg);
            break;

        /***********************************************
        *************Branch on Carry Clear**************
        ***********************************************/
        case REL_BCC:
            cpu_bcc(cpu_handle, *arg);
            break;

        /***********************************************
        **************Branch on Carry Set***************
        ***********************************************/
        case REL_BCS:
            cpu_bcs(cpu_handle, *arg);
            break;

        /***********************************************
        *************Branch on Result Zero**************
        ***********************************************/
        case REL_BEQ:
            cpu_beq(cpu_handle, *arg);
            break;

        /***********************************************
        *************Test Bits in M with A**************
        ***********************************************/
        case ZP_BIT:    case ABS_BIT:
            cpu_bit(cpu_handle, *arg);
            break;

        /***********************************************
        *************Branch on Result Minus*************
        ***********************************************/
        case REL_BMI:
            cpu_bmi(cpu_handle, *arg);
            break;

        /***********************************************
        ***********Branch on Result not Zero************
        ***********************************************/
        case REL_BNE:
            cpu_bne(cpu_handle, *arg);
            break;

        /***********************************************
        *************Branch on Result Plus**************
        ***********************************************/
        case REL_BPL:
            cpu_bpl(cpu_handle, *arg);
            break;

        /***********************************************
        ******************Force Break*******************
        ***********************************************/
        case IMP_BRK:
            cpu_brk(cpu_handle);
            break;

        /***********************************************
        ************Branch on Overflow Clear************
        ***********************************************/
        case REL_BVC:
            cpu_bvc(cpu_handle, *arg);
            break;

        /***********************************************
        *************Branch on Overflow Set*************
        ***********************************************/
        case REL_BVS:
            cpu_bvs(cpu_handle, *arg);
            break;

        /***********************************************
        ****************Clear Carry Flag****************
        ***********************************************/
        case IMP_CLC:
            cpu_clc(cpu_handle);
            break;

        /***********************************************
        ***************Clear Decimal Mode***************
        ***********************************************/
        case IMP_CLD:
            cpu_cld(cpu_handle);
            break;

        /***********************************************
        **********Clear interrupt Disable Bit***********
        ***********************************************/
        case IMP_CLI:
            cpu_cli(cpu_handle);
            break;

        /***********************************************
        **************Clear Overflow Flag***************
        ***********************************************/
        case IMP_CLV:
            cpu_clv(cpu_handle);
            break;

        /***********************************************
        ****************Compare M and A*****************
        ***********************************************/
        case IMM_CMP:   case ZP_CMP:    case ABS_CMP:
        case IXX_CMP:   case IXY_CMP:   case ZPIXX_CMP:
        case PREII_CMP: case POSII_CMP:
            cpu_cmp(cpu_handle, *arg);
            break;

        /***********************************************
        ****************Compare M and X*****************
        ***********************************************/
        case IMM_CPX:   case ZP_CPX:    case ABS_CPX:
            cpu_cpx(cpu_handle, *arg);
            break;

        /***********************************************
        ****************Compare M and Y*****************
        ***********************************************/
        case IMM_CPY:   case ZP_CPY:    case ABS_CPY:
            cpu_cpy(cpu_handle, *arg);
            break;

        /***********************************************
        ***************Decrement M by One***************
        ***********************************************/
        case ZP_DEC:    case ABS_DEC:   case IXX_DEC:
        case ZPIXX_DEC:
            cpu_dec(cpu_handle, arg);
            break;

        /***********************************************
        ***************Decrement X by One***************
        ***********************************************/
        case IMP_DEX:
            cpu_dex(cpu_handle);
            break;

        /***********************************************
        ***************Decrement Y by One***************
        ***********************************************/
        case IMP_DEY:
            cpu_dey(cpu_handle);
            break;

        /***********************************************
        ************"Exclusive-Or" M with A*************
        ***********************************************/
        case IMM_EOR:   case ZP_EOR:    case ABS_EOR:
        case IXX_EOR:   case IXY_EOR:   case ZPIXX_EOR:
        case PREII_EOR: case POSII_EOR:
            cpu_eor(cpu_handle, *arg);
            break;

        /***********************************************
        ***************Increment M by One***************
        ***********************************************/
        case ZP_INC:    case ABS_INC:   case IXX_INC:
        case ZPIXX_INC:
            cpu_inc(cpu_handle, arg);
            break;

        /***********************************************
        ***************Increment X by One***************
        ***********************************************/
        case IMP_INX:
            cpu_inx(cpu_handle);
            break;

        /***********************************************
        ***************Increment Y by One***************
        ***********************************************/
        case IMP_INY:
            cpu_iny(cpu_handle);
            break;

        /***********************************************
        ****************Jump to Location****************
        ***********************************************/
        case ABS_JMP:  case IDR_JMP:
            cpu_jmp(cpu_handle, arg);
            break;

        /***********************************************
        ******Jump to Location Save Return Address******
        ***********************************************/
        case ABS_JSR:
            cpu_jsr(cpu_handle, arg);
            break;

        /***********************************************
        *****************Load A with M******************
        ***********************************************/
        case IMM_LDA:   case ZP_LDA:    case ABS_LDA:
        case IXX_LDA:   case IXY_LDA:   case ZPIXX_LDA:
        case PREII_LDA: case POSII_LDA:
			cpu_lda(cpu_handle, *arg);
            break;

        /***********************************************
        *****************Load X with M******************
        ***********************************************/
        case IMM_LDX:   case ZP_LDX:    case ABS_LDX:
        case IXY_LDX:   case ZPIXY_LDX:
			cpu_ldx(cpu_handle, *arg);
            break;

        /***********************************************
        *****************Load Y with M******************
        ***********************************************/
        case IMM_LDY:   case ZP_LDY:    case ABS_LDY:
        case IXX_LDY:   case ZPIXX_LDY:
			cpu_ldy(cpu_handle, *arg);
            break;

        /***********************************************
        **********Shift Right One Bit (M or A)**********
        ***********************************************/
        case ZP_LSR:    case ABS_LSR:   case ACC_LSR:
        case IXX_LSR:   case ZPIXX_LSR:
			cpu_lsr(cpu_handle, arg);
            break;

        /***********************************************
        ******************No Operation******************
        ***********************************************/
        case IMP_NOP:
            break;

        /***********************************************
        *****************"OR" M with A******************
        ***********************************************/
        case IMM_ORA:   case ZP_ORA:    case ABS_ORA:
        case IXX_ORA:   case IXY_ORA:   case ZPIXX_ORA:
        case PREII_ORA: case POSII_ORA:
			cpu_ora(cpu_handle, *arg);
            break;

        /***********************************************
        ****************Push A on Stack*****************
        ***********************************************/
        case IMP_PHA:
			cpu_pha(cpu_handle);
            break;

        /***********************************************
        *********Push Processor Status on Stack*********
        ***********************************************/
        case IMP_PHP:
			cpu_php(cpu_handle);
            break;

        /***********************************************
        ***************Pull A from Stack****************
        ***********************************************/
        case IMP_PLA:
			cpu_pla(cpu_handle);
            break;

        /***********************************************
        ********Pull Processor Status from Stack********
        ***********************************************/
        case IMP_PLP:
			cpu_plp(cpu_handle);
            break;

        /***********************************************
        **********Rotate One Bit Left (M or A)**********
        ***********************************************/
        case ZP_ROL:    case ABS_ROL:   case ACC_ROL:
        case IXX_ROL:   case ZPIXX_ROL:
			cpu_rol(cpu_handle, arg);
            break;

        /***********************************************
        *********Rotate One Bit Right (M or A)**********
        ***********************************************/
        case ZP_ROR:    case ABS_ROR:   case ACC_ROR:
        case IXX_ROR:   case ZPIXX_ROR:
			cpu_ror(cpu_handle, arg);
            break;

        /***********************************************
        *************Return from Interrupt**************
        ***********************************************/
        case IMP_RTI:
			cpu_rti(cpu_handle);
            break;

        /***********************************************
        *************Return from Subroutine*************
        ***********************************************/
        case IMP_RTS:
			cpu_rts(cpu_handle);
            break;

        /***********************************************
        *********Subtract M from A with Borrow**********
        ***********************************************/
        case IMM_SBC:   case ZP_SBC:    case ABS_SBC:
        case IXX_SBC:   case IXY_SBC:   case ZPIXX_SBC:
        case PREII_SBC: case POSII_SBC:
			cpu_sbc(cpu_handle, *arg);
            break;

        /***********************************************
        *****************Set Carry Flag*****************
        ***********************************************/
        case IMP_SEC:
			cpu_sec(cpu_handle);
            break;

        /***********************************************
        ****************Set Decimal Mode****************
        ***********************************************/
        case IMP_SED:
			cpu_sed(cpu_handle);
            break;

        /***********************************************
        **********Set Interrupt Disable Status**********
        ***********************************************/
        case IMP_SEI:
			cpu_sei(cpu_handle);
            break;

        /***********************************************
        ******************Store A in M******************
        ***********************************************/
        case ZP_STA:    case ABS_STA:   case IXX_STA:
        case IXY_STA:   case ZPIXX_STA: case PREII_STA:
        case POSII_STA:
			cpu_sta(cpu_handle, arg);
            break;

        /***********************************************
        ******************Store X in M******************
        ***********************************************/
        case ZP_STX:    case ABS_STX:   case ZPIXY_STX:
			cpu_stx(cpu_handle, arg);
            break;

        /***********************************************
        ******************Store Y in M******************
        ***********************************************/
        case ZP_STY:    case ABS_STY:   case ZPIXX_STY:
			cpu_sty(cpu_handle, arg);
            break;

        /***********************************************
        *****************Transfer A to X****************
        ***********************************************/
        case IMP_TAX:
			cpu_tax(cpu_handle);
            break;

        /***********************************************
        **********Transfer Stack Pointer to X***********
        ***********************************************/
        case IMP_TSX:
			cpu_tsx(cpu_handle);
            break;

        /***********************************************
        *****************Transfer X to A****************
        ***********************************************/
        case IMP_TXA:
			cpu_txa(cpu_handle);
            break;

        /***********************************************
        **********Transfer X to Stack Pointer***********
        ***********************************************/
        case IMP_TXS:
			cpu_txs(cpu_handle);
            break;

        /***********************************************
        *****************Transfer Y to A****************
        ***********************************************/
        case IMP_TYA:
			cpu_tya(cpu_handle);
            break;
    }
    return true;
}

void cpu_push(struct CPU *cpu_handle, byte_t data)
{
    cpu_handle->memory.ram[0x100 + cpu_handle->sp++];
}

byte_t cpu_pop(struct CPU *cpu_handle)
{
    return cpu_handle->memory.ram[0x100 + --cpu_handle->sp];
}

byte_t cpu_peek(struct CPU *cpu_handle)
{
    return cpu_handle->memory.ram[0x100 + cpu_handle->sp - 1];
}

void cpu_imm(struct CPU *cpu_handle, byte_t **arg)
{
    *arg = cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++);
}

void cpu_zp(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory,
            *cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++));
}

void cpu_abs(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory,
            cpumem_reads(&cpu_handle->memory, cpu_handle->pc));
    cpu_handle->pc += 2;
}

void cpu_acc(struct CPU *cpu_handle, byte_t **arg)
{
    *arg = &cpu_handle->A;
    cpu_handle->pc;
}

void cpu_ixx(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory, cpu_handle->X +
            cpumem_reads(&cpu_handle->memory, cpu_handle->pc));
    cpu_handle->pc += 2;
}

void cpu_ixy(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory, cpu_handle->Y +
            cpumem_reads(&cpu_handle->memory, cpu_handle->pc));
    cpu_handle += 2;
}

void cpu_zpixx(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory, cpu_handle->X +
            *cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++));
}

void cpu_zpixy(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory, cpu_handle->Y +
            *cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++));
}

void cpu_idr(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory,
            cpumem_reads(&cpu_handle->memory, cpu_handle->pc));
    cpu_handle->sp += 2;
}

void cpu_preii(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory,
            cpumem_reads(&cpu_handle->memory, cpu_handle->X +
            *cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++)));
}

void cpu_posii(struct CPU *cpu_handle, byte_t **arg)
{
    *arg =  cpumem_readbp(&cpu_handle->memory, cpu_handle->Y +
            cpumem_reads(&cpu_handle->memory,
            *cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++)));
}

void cpu_rel(struct CPU *cpu_handle, byte_t **arg)
{
    *arg = cpumem_readbp(&cpu_handle->memory, cpu_handle->pc++);
}

void cpu_adc(struct CPU *cpu_handle, byte_t arg)
{
    uint16_t temp = arg + cpu_handle->A + cpu_handle->P.C;
    cpu_handle->P.V =   !((cpu_handle->A ^ arg) & 0x80)
                        && ((cpu_handle->A ^ temp) & 0x80);
    cpu_handle->A = (byte_t) temp;
    cpu_handle->P.Z = (cpu_handle->A == 0);
    cpu_handle->P.S = (cpu_handle->A > 0x7F);
    cpu_handle->P.C = (temp > 0xff);
}

void cpu_and(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->A &= arg;
    cpu_handle->P.Z = (cpu_handle->A == 0);
    cpu_handle->P.S = (cpu_handle->A > 0x7F);
}

void cpu_asl(struct CPU *cpu_handle, byte_t *arg)
{
    cpu_handle->P.C = (*arg > 0x7F);
    *arg <<= 1;
    cpu_handle->P.Z = (*arg == 0);
    cpu_handle->P.S = (*arg > 0x7F);
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

void cpu_bit(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->P.S = (arg > 0x7F);
    cpu_handle->P.V = ((arg & 0x40) > 0x3F);
    cpu_handle->P.Z = ((arg & cpu_handle->A) == 0);
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
    cpu_push(cpu_handle, cpu_handle->pc >> 8);
    cpu_push(cpu_handle, cpu_handle->pc);
    cpu_handle->P.B = true;
    cpu_push(cpu_handle, psr_as_byte(&cpu_handle->P));
    cpu_handle->P.I = true;
    cpu_handle->pc = cpumem_reads(&cpu_handle->memory, IRQ_VEC);
}

void cpu_bvc(struct CPU *cpu_handle, int8_t offset)
{
    if(!cpu_handle->P.V)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_bvs(struct CPU *cpu_handle, int8_t offset)
{
    if(cpu_handle->P.V)
    {
        cpu_handle->clock +=    ((cpu_handle->pc & 0xFF00)
                                != ((cpu_handle->pc + offset) & 0xFF00)
                                ? 2 : 1);
        cpu_handle->pc += offset;
    }
}

void cpu_clc(struct CPU *cpu_handle)
{
    cpu_handle->P.C = false;
}

void cpu_cld(struct CPU *cpu_handle)
{
    cpu_handle->P.D = false;
}

void cpu_cli(struct CPU *cpu_handle)
{
    cpu_handle->P.I = false;
}

void cpu_clv(struct CPU *cpu_handle)
{
    cpu_handle->P.V = false;
}

void cpu_cmp(struct CPU *cpu_handle, byte_t arg)
{
    uint16_t temp = cpu_handle->A - arg;
    cpu_handle->P.C = (temp > 0xFF);
    cpu_handle->P.S = ((temp & 0xFF) > 0x7F);
    cpu_handle->P.Z = (temp == 0);
}

void cpu_cpx(struct CPU *cpu_handle, byte_t arg)
{
    uint16_t temp = cpu_handle->X - arg;
    cpu_handle->P.C = (temp > 0xFF);
    cpu_handle->P.S = ((temp & 0xFF) > 0x7F);
    cpu_handle->P.Z = (temp == 0);
}

void cpu_cpy(struct CPU *cpu_handle, byte_t arg)
{
    uint16_t temp = cpu_handle->Y - arg;
    cpu_handle->P.C = (temp > 0xFF);
    cpu_handle->P.S = ((temp & 0xFF) > 0x7F);
    cpu_handle->P.Z = (temp == 0);
}

void cpu_dec(struct CPU *cpu_handle, byte_t *arg)
{
    *arg--;
    cpu_handle->P.S = (*arg > 0x7F);
    cpu_handle->P.Z = (*arg == 0);
}

void cpu_dex(struct CPU *cpu_handle)
{
    cpu_handle->X--;
    cpu_handle->P.S = (cpu_handle->X > 0x7F);
    cpu_handle->P.Z = (cpu_handle->X == 0);
}

void cpu_dey(struct CPU *cpu_handle)
{
    cpu_handle->Y--;
    cpu_handle->P.S = (cpu_handle->Y > 0x7F);
    cpu_handle->P.Z = (cpu_handle->Y == 0);
}

void cpu_eor(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->A ^= arg;
    cpu_handle->P.S = (cpu_handle->Y > 0x7F);
    cpu_handle->P.Z = (cpu_handle->Y == 0);
}

void cpu_inc(struct CPU *cpu_handle, byte_t *arg)
{
    *arg++;
    cpu_handle->P.S = (*arg > 0x7F);
    cpu_handle->P.Z = (*arg == 0);
}

void cpu_inx(struct CPU *cpu_handle)
{
    cpu_handle->X++;
    cpu_handle->P.S = (cpu_handle->X > 0x7F);
    cpu_handle->P.Z = (cpu_handle->X == 0);
}

void cpu_iny(struct CPU *cpu_handle)
{
    cpu_handle->Y++;
    cpu_handle->P.S = (cpu_handle->Y > 0x7F);
    cpu_handle->P.Z = (cpu_handle->Y == 0);
}

void cpu_jmp(struct CPU *cpu_handle, byte_t *arg)
{
    if(arg >= cpu_handle->memory.prg_low && arg < cpu_handle->memory.prg_low + 0x4000)
        cpu_handle->pc = 0x8000 + arg - cpu_handle->memory.prg_low;
    else if(arg >= cpu_handle->memory.prg_high && arg < cpu_handle->memory.prg_high + 0x4000)
        cpu_handle->pc = 0xC000 + arg - cpu_handle->memory.prg_high;
    else if(arg >= cpu_handle->memory.sram && arg < cpu_handle->memory.sram + 0x2000)
        cpu_handle->pc = 0x6000 + arg - cpu_handle->memory.sram;
    else if(arg >= cpu_handle->memory.erom && arg < cpu_handle->memory.erom + 0x1FE0)
        cpu_handle->pc = 0x4020 + arg - cpu_handle->memory.erom;
    else if(arg >= cpu_handle->memory.ram && arg < cpu_handle->memory.ram + 0x2000)
        cpu_handle->pc = arg - cpu_handle->memory.ram;
    else if(arg >= cpu_handle->memory.ppu_io && arg < cpu_handle->memory.ppu_io + 0x08)
        cpu_handle->pc = 0x2000 + arg - cpu_handle->memory.ppu_io;
    else if(arg >= cpu_handle->memory.apu_io && arg < cpu_handle->memory.apu_io + 0x20)
        cpu_handle->pc = 0x4000 + arg - cpu_handle->memory.apu_io;
}

void cpu_jsr(struct CPU *cpu_handle, byte_t *arg)
{
    cpu_push(cpu_handle, (byte_t) (cpu_handle->pc >> 8));
    cpu_push(cpu_handle, (byte_t) cpu_handle->pc);
    cpu_jmp(cpu_handle, arg);
}

void cpu_lda(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->P.S = (arg > 0x7F);
    cpu_handle->P.Z = (arg == 0);
    cpu_handle->A = arg;
}

void cpu_ldx(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->P.S = (arg > 0x7F);
    cpu_handle->P.Z = (arg == 0);
    cpu_handle->X = arg;
}

void cpu_ldy(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->P.S = (arg > 0x7F);
    cpu_handle->P.Z = (arg == 0);
    cpu_handle->Y = arg;
}

void cpu_lsr(struct CPU *cpu_handle, byte_t *arg)
{
    cpu_handle->P.C = ((*arg & 0x01) == 0x01);
    *arg >= 1;
    cpu_handle->P.S = false;
    cpu_handle->P.Z = (arg == 0);
}

void cpu_ora(struct CPU *cpu_handle, byte_t arg)
{
    cpu_handle->A |= arg;
    cpu_handle->P.S = (cpu_handle->A > 0x7F);
    cpu_handle->P.Z = (cpu_handle->A == 0);
}

void cpu_pha(struct CPU *cpu_handle)
{
    cpu_push(cpu_handle, cpu_handle->A);
}

void cpu_php(struct CPU *cpu_handle)
{
    cpu_push(cpu_handle, psr_as_byte(&cpu_handle->P));
}

void cpu_pla(struct CPU *cpu_handle)
{
    cpu_handle->A = cpu_pop(cpu_handle);
}

void cpu_plp(struct CPU *cpu_handle)
{
    byte_as_psr(&cpu_handle->P, cpu_pop(cpu_handle));
}

void cpu_rol(struct CPU *cpu_handle, byte_t *arg)
{
	bool carry = ((*arg & 0x80) == 0x80);
	*arg <<= 1;
	*arg |= cpu_handle->P.C;
	cpu_handle->P.C = carry;
	cpu_handle->P.S = (*arg > 0x7F);
    cpu_handle->P.Z = (*arg == 0);
}

void cpu_ror(struct CPU *cpu_handle, byte_t *arg)
{
	bool carry = ((*arg & 0x01) == 0x01);
	*arg >>= 1;
	*arg |= (cpu_handle->P.C << 7);
	cpu_handle->P.C = carry;
	cpu_handle->P.S = (*arg > 0x7F);
    cpu_handle->P.Z = (*arg == 0);
}

void cpu_rti(struct CPU *cpu_handle)
{
	byte_as_psr(&cpu_handle->P, cpu_pop(cpu_handle));
	byte_t low = cpu_pop(cpu_handle);
	cpu_handle->pc = (cpu_pop(cpu_handle) << 8) | low;
}

void cpu_rts(struct CPU *cpu_handle)
{
	byte_t low = cpu_pop(cpu_handle);
	cpu_handle->pc = (cpu_pop(cpu_handle) << 8) | low;
}

void cpu_sbc(struct CPU *cpu_handle, byte_t arg)
{
	uint16_t temp = cpu_handle->A - arg - cpu_handle->P.C;
	cpu_handle->P.V =   !((cpu_handle->A ^ arg) & 0x80)
						&& ((cpu_handle->A ^ temp) & 0x80);
    cpu_handle->A = (byte_t) temp;
    cpu_handle->P.Z = (cpu_handle->A == 0);
    cpu_handle->P.S = (cpu_handle->A > 0x7F);
    cpu_handle->P.C = (temp > 0xff);

}

void cpu_sec(struct CPU *cpu_handle)
{
	cpu_handle->P.C = true;
}

void cpu_sed(struct CPU *cpu_handle)
{
	cpu_handle->P.D = true;
}

void cpu_sei(struct CPU *cpu_handle)
{
    cpu_handle->P.I = true;
}

void cpu_sta(struct CPU *cpu_handle, byte_t *arg)
{
	*arg = cpu_handle->A;
}

void cpu_stx(struct CPU *cpu_handle, byte_t *arg)
{
	*arg = cpu_handle->X;
}

void cpu_sty(struct CPU *cpu_handle, byte_t *arg)
{
	*arg = cpu_handle->Y;
}

void cpu_tax(struct CPU *cpu_handle)
{
	cpu_handle->X = cpu_handle->A;
	cpu_handle->P.Z = (cpu_handle->X == 0);
    cpu_handle->P.S = (cpu_handle->X > 0x7F);
}

void cpu_tay(struct CPU *cpu_handle)
{
	cpu_handle->Y = cpu_handle->A;
	cpu_handle->P.Z = (cpu_handle->Y == 0);
    cpu_handle->P.S = (cpu_handle->Y > 0x7F);
}

void cpu_tsx(struct CPU *cpu_handle)
{
	cpu_handle->X = cpu_handle->sp;
	cpu_handle->P.Z = (cpu_handle->X == 0);
    cpu_handle->P.S = (cpu_handle->X > 0x7F);
}

void cpu_txa(struct CPU *cpu_handle)
{
	cpu_handle->A = cpu_handle->X;
	cpu_handle->P.Z = (cpu_handle->A == 0);
	cpu_handle->P.S = (cpu_handle->A > 0x7F);
}

void cpu_txs(struct CPU *cpu_handle)
{
	cpu_handle->sp = cpu_handle->X;
}

void cpu_tya(struct CPU *cpu_handle)
{
	cpu_handle->A = cpu_handle->Y;
	cpu_handle->P.Z = (cpu_handle->A == 0);
	cpu_handle->P.S = (cpu_handle->A > 0x7F);
}
