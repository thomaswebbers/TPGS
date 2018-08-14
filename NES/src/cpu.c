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

void init_cpu(struct NES *nes)
{
    nes->cpu.clock = 0;
    nes->cpu.pc = cpumem_reads(nes, RESET_VEC);
    nes->cpu.sp = 0;
    nes->cpu.A = 0;
    nes->cpu.X = 0;
    nes->cpu.Y = 0;
    init_psr(nes);
	init_cpumem(nes);
}

void destroy_cpu(struct NES *nes)
{

}

void cpu_add_cycles(struct NES *nes, uint32_t cycles)
{
    nes->cpu.clock += cycles;
}

bool cpu_step(struct NES *nes)
{
    byte_t opcode = *cpumem_readbp(nes, nes->cpu.pc);
    printf("opcode[%x]: %x ", nes->cpu.pc, opcode);
    byte_t *arg;
    nes->cpu.pc++;
    nes->cpu.clock += opcode_cycles[opcode];
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
            cpu_imm(nes, &arg);
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
            cpu_zp(nes, &arg);
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
            cpu_abs(nes, &arg);
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
            cpu_acc(nes, &arg);
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
            cpu_ixx(nes, &arg);
            break;

        /***********************************************
        ******Indexed Addressing (index register Y)*****
        ***********************************************/
        case IXY_ADC: case IXY_AND: case IXY_CMP:
        case IXY_EOR: case IXY_LDA: case IXY_LDX:
        case IXY_ORA: case IXY_SBC: case IXY_STA:
			printf("IXY_");
            cpu_ixy(nes, &arg);
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
            cpu_zpixx(nes, &arg);
            break;

        /***********************************************
        *Zero-Page Indexed Addressing(index register Y)*
        ***********************************************/
        case ZPIXY_LDX: case ZPIXY_STX:
			printf("ZPIXY_");
            cpu_zpixy(nes, &arg);
            break;

        /***********************************************
        **************Indirect Addressing***************
        ***********************************************/
        case IDR_JMP:
			printf("IDR_");
            cpu_idr(nes, &arg);
            break;

        /***********************************************
        ********Pre-Indexed Indirect Addressing*********
        ***********************************************/
        case PREII_ADC: case PREII_AND: case PREII_CMP:
        case PREII_EOR: case PREII_LDA: case PREII_ORA:
        case PREII_SBC: case PREII_STA:
			printf("PREII_");
            cpu_preii(nes, &arg);
            break;

        /***********************************************
        ********Post-Indexed Indirect Addressing********
        ***********************************************/
        case POSII_ADC: case POSII_AND: case POSII_CMP:
        case POSII_EOR: case POSII_LDA: case POSII_ORA:
        case POSII_SBC: case POSII_STA:
			printf("POSII_");
            cpu_posii(nes, &arg);
            break;

        /***********************************************
        **************Relative Addressing***************
        ***********************************************/
        case REL_BCC: case REL_BCS: case REL_BEQ:
        case REL_BMI: case REL_BNE: case REL_BPL:
        case REL_BVC: case REL_BVS:
			printf("REL_");
            cpu_rel(nes, &arg);
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
            cpu_adc(nes, *arg);
            break;

        /***********************************************
        *****************"AND" M with A*****************
        ***********************************************/
        case IMM_AND:   case ZP_AND:    case ABS_AND:
        case IXX_AND:   case IXY_AND:   case ZPIXX_AND:
        case PREII_AND: case POSII_AND:
            cpu_and(nes, *arg);
            break;

        /***********************************************
        **********Shift Left One Bit (M or A)***********
        ***********************************************/
        case ZP_ASL:    case ABS_ASL:   case ACC_ASL:
        case IXX_ASL:
            cpu_asl(nes, arg);
            break;

        /***********************************************
        *************Branch on Carry Clear**************
        ***********************************************/
        case REL_BCC:
            cpu_bcc(nes, *arg);
            break;

        /***********************************************
        **************Branch on Carry Set***************
        ***********************************************/
        case REL_BCS:
            cpu_bcs(nes, *arg);
            break;

        /***********************************************
        *************Branch on Result Zero**************
        ***********************************************/
        case REL_BEQ:
            cpu_beq(nes, *arg);
            break;

        /***********************************************
        *************Test Bits in M with A**************
        ***********************************************/
        case ZP_BIT:    case ABS_BIT:
            cpu_bit(nes, *arg);
            break;

        /***********************************************
        *************Branch on Result Minus*************
        ***********************************************/
        case REL_BMI:
            cpu_bmi(nes, *arg);
            break;

        /***********************************************
        ***********Branch on Result not Zero************
        ***********************************************/
        case REL_BNE:
            cpu_bne(nes, *arg);
            break;

        /***********************************************
        *************Branch on Result Plus**************
        ***********************************************/
        case REL_BPL:
            cpu_bpl(nes, *arg);
            break;

        /***********************************************
        ******************Force Break*******************
        ***********************************************/
        case IMP_BRK:
            cpu_brk(nes);
            break;

        /***********************************************
        ************Branch on Overflow Clear************
        ***********************************************/
        case REL_BVC:
            cpu_bvc(nes, *arg);
            break;

        /***********************************************
        *************Branch on Overflow Set*************
        ***********************************************/
        case REL_BVS:
            cpu_bvs(nes, *arg);
            break;

        /***********************************************
        ****************Clear Carry Flag****************
        ***********************************************/
        case IMP_CLC:
            cpu_clc(nes);
            break;

        /***********************************************
        ***************Clear Decimal Mode***************
        ***********************************************/
        case IMP_CLD:
            cpu_cld(nes);
            break;

        /***********************************************
        **********Clear interrupt Disable Bit***********
        ***********************************************/
        case IMP_CLI:
            cpu_cli(nes);
            break;

        /***********************************************
        **************Clear Overflow Flag***************
        ***********************************************/
        case IMP_CLV:
            cpu_clv(nes);
            break;

        /***********************************************
        ****************Compare M and A*****************
        ***********************************************/
        case IMM_CMP:   case ZP_CMP:    case ABS_CMP:
        case IXX_CMP:   case IXY_CMP:   case ZPIXX_CMP:
        case PREII_CMP: case POSII_CMP:
            cpu_cmp(nes, *arg);
            break;

        /***********************************************
        ****************Compare M and X*****************
        ***********************************************/
        case IMM_CPX:   case ZP_CPX:    case ABS_CPX:
            cpu_cpx(nes, *arg);
            break;

        /***********************************************
        ****************Compare M and Y*****************
        ***********************************************/
        case IMM_CPY:   case ZP_CPY:    case ABS_CPY:
            cpu_cpy(nes, *arg);
            break;

        /***********************************************
        ***************Decrement M by One***************
        ***********************************************/
        case ZP_DEC:    case ABS_DEC:   case IXX_DEC:
        case ZPIXX_DEC:
            cpu_dec(nes, arg);
            break;

        /***********************************************
        ***************Decrement X by One***************
        ***********************************************/
        case IMP_DEX:
            cpu_dex(nes);
            break;

        /***********************************************
        ***************Decrement Y by One***************
        ***********************************************/
        case IMP_DEY:
            cpu_dey(nes);
            break;

        /***********************************************
        ************"Exclusive-Or" M with A*************
        ***********************************************/
        case IMM_EOR:   case ZP_EOR:    case ABS_EOR:
        case IXX_EOR:   case IXY_EOR:   case ZPIXX_EOR:
        case PREII_EOR: case POSII_EOR:
            cpu_eor(nes, *arg);
            break;

        /***********************************************
        ***************Increment M by One***************
        ***********************************************/
        case ZP_INC:    case ABS_INC:   case IXX_INC:
        case ZPIXX_INC:
            cpu_inc(nes, arg);
            break;

        /***********************************************
        ***************Increment X by One***************
        ***********************************************/
        case IMP_INX:
            cpu_inx(nes);
            break;

        /***********************************************
        ***************Increment Y by One***************
        ***********************************************/
        case IMP_INY:
            cpu_iny(nes);
            break;

        /***********************************************
        ****************Jump to Location****************
        ***********************************************/
        case ABS_JMP:  case IDR_JMP:
            cpu_jmp(nes, arg);
            break;

        /***********************************************
        ******Jump to Location Save Return Address******
        ***********************************************/
        case ABS_JSR:
            cpu_jsr(nes, arg);
            break;

        /***********************************************
        *****************Load A with M******************
        ***********************************************/
        case IMM_LDA:   case ZP_LDA:    case ABS_LDA:
        case IXX_LDA:   case IXY_LDA:   case ZPIXX_LDA:
        case PREII_LDA: case POSII_LDA:
			cpu_lda(nes, *arg);
            break;

        /***********************************************
        *****************Load X with M******************
        ***********************************************/
        case IMM_LDX:   case ZP_LDX:    case ABS_LDX:
        case IXY_LDX:   case ZPIXY_LDX:
			cpu_ldx(nes, *arg);
            break;

        /***********************************************
        *****************Load Y with M******************
        ***********************************************/
        case IMM_LDY:   case ZP_LDY:    case ABS_LDY:
        case IXX_LDY:   case ZPIXX_LDY:
			cpu_ldy(nes, *arg);
            break;

        /***********************************************
        **********Shift Right One Bit (M or A)**********
        ***********************************************/
        case ZP_LSR:    case ABS_LSR:   case ACC_LSR:
        case IXX_LSR:   case ZPIXX_LSR:
			cpu_lsr(nes, arg);
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
			cpu_ora(nes, *arg);
            break;

        /***********************************************
        ****************Push A on Stack*****************
        ***********************************************/
        case IMP_PHA:
			cpu_pha(nes);
            break;

        /***********************************************
        *********Push Processor Status on Stack*********
        ***********************************************/
        case IMP_PHP:
			cpu_php(nes);
            break;

        /***********************************************
        ***************Pull A from Stack****************
        ***********************************************/
        case IMP_PLA:
			cpu_pla(nes);
            break;

        /***********************************************
        ********Pull Processor Status from Stack********
        ***********************************************/
        case IMP_PLP:
			cpu_plp(nes);
            break;

        /***********************************************
        **********Rotate One Bit Left (M or A)**********
        ***********************************************/
        case ZP_ROL:    case ABS_ROL:   case ACC_ROL:
        case IXX_ROL:   case ZPIXX_ROL:
			cpu_rol(nes, arg);
            break;

        /***********************************************
        *********Rotate One Bit Right (M or A)**********
        ***********************************************/
        case ZP_ROR:    case ABS_ROR:   case ACC_ROR:
        case IXX_ROR:   case ZPIXX_ROR:
			cpu_ror(nes, arg);
            break;

        /***********************************************
        *************Return from Interrupt**************
        ***********************************************/
        case IMP_RTI:
			cpu_rti(nes);
            break;

        /***********************************************
        *************Return from Subroutine*************
        ***********************************************/
        case IMP_RTS:
			cpu_rts(nes);
            break;

        /***********************************************
        *********Subtract M from A with Borrow**********
        ***********************************************/
        case IMM_SBC:   case ZP_SBC:    case ABS_SBC:
        case IXX_SBC:   case IXY_SBC:   case ZPIXX_SBC:
        case PREII_SBC: case POSII_SBC:
			cpu_sbc(nes, *arg);
            break;

        /***********************************************
        *****************Set Carry Flag*****************
        ***********************************************/
        case IMP_SEC:
			cpu_sec(nes);
            break;

        /***********************************************
        ****************Set Decimal Mode****************
        ***********************************************/
        case IMP_SED:
			cpu_sed(nes);
            break;

        /***********************************************
        **********Set Interrupt Disable Status**********
        ***********************************************/
        case IMP_SEI:
			cpu_sei(nes);
            break;

        /***********************************************
        ******************Store A in M******************
        ***********************************************/
        case ZP_STA:    case ABS_STA:   case IXX_STA:
        case IXY_STA:   case ZPIXX_STA: case PREII_STA:
        case POSII_STA:
			cpu_sta(nes, arg);
            break;

        /***********************************************
        ******************Store X in M******************
        ***********************************************/
        case ZP_STX:    case ABS_STX:   case ZPIXY_STX:
			cpu_stx(nes, arg);
            break;

        /***********************************************
        ******************Store Y in M******************
        ***********************************************/
        case ZP_STY:    case ABS_STY:   case ZPIXX_STY:
			cpu_sty(nes, arg);
            break;

        /***********************************************
        *****************Transfer A to X****************
        ***********************************************/
        case IMP_TAX:
			cpu_tax(nes);
            break;

        /***********************************************
        **********Transfer Stack Pointer to X***********
        ***********************************************/
        case IMP_TSX:
			cpu_tsx(nes);
            break;

        /***********************************************
        *****************Transfer X to A****************
        ***********************************************/
        case IMP_TXA:
			cpu_txa(nes);
            break;

        /***********************************************
        **********Transfer X to Stack Pointer***********
        ***********************************************/
        case IMP_TXS:
			cpu_txs(nes);
            break;

        /***********************************************
        *****************Transfer Y to A****************
        ***********************************************/
        case IMP_TYA:
			cpu_tya(nes);
            break;
    }
    return true;
}

void cpu_push(struct NES *nes, byte_t data)
{
    nes->cpu.memory.ram[0x100 + nes->cpu.sp++];
}

byte_t cpu_pop(struct NES *nes)
{
    return nes->cpu.memory.ram[0x100 + --nes->cpu.sp];
}

byte_t cpu_peek(struct NES *nes)
{
    return nes->cpu.memory.ram[0x100 + nes->cpu.sp - 1];
}

void cpu_imm(struct NES *nes, byte_t **arg)
{
    *arg = cpumem_readbp(nes, nes->cpu.pc++);
}

void cpu_zp(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes,
            *cpumem_readbp(nes, nes->cpu.pc++));
}

void cpu_abs(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes,
            cpumem_reads(nes, nes->cpu.pc));
    nes->cpu.pc += 2;
}

void cpu_acc(struct NES *nes, byte_t **arg)
{
    *arg = &nes->cpu.A;
    nes->cpu.pc;
}

void cpu_ixx(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes, nes->cpu.X +
            cpumem_reads(nes, nes->cpu.pc));
    nes->cpu.pc += 2;
}

void cpu_ixy(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes, nes->cpu.Y +
            cpumem_reads(nes, nes->cpu.pc));
    nes->cpu.pc += 2;
}

void cpu_zpixx(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes, nes->cpu.X +
            *cpumem_readbp(nes, nes->cpu.pc++));
}

void cpu_zpixy(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes, nes->cpu.Y +
            *cpumem_readbp(nes, nes->cpu.pc++));
}

void cpu_idr(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes,
            cpumem_reads(nes, nes->cpu.pc));
    nes->cpu.sp += 2;
}

void cpu_preii(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes,
            cpumem_reads(nes, nes->cpu.X +
            *cpumem_readbp(nes, nes->cpu.pc++)));
}

void cpu_posii(struct NES *nes, byte_t **arg)
{
    *arg =  cpumem_readbp(nes, nes->cpu.Y +
            cpumem_reads(nes,
            *cpumem_readbp(nes, nes->cpu.pc++)));
}

void cpu_rel(struct NES *nes, byte_t **arg)
{
    *arg = cpumem_readbp(nes, nes->cpu.pc++);
}

void cpu_adc(struct NES *nes, byte_t arg)
{
    uint16_t temp = arg + nes->cpu.A + nes->cpu.P.C;
    nes->cpu.P.V =   !((nes->cpu.A ^ arg) & 0x80)
                        && ((nes->cpu.A ^ temp) & 0x80);
    nes->cpu.A = (byte_t) temp;
    nes->cpu.P.Z = (nes->cpu.A == 0);
    nes->cpu.P.S = (nes->cpu.A > 0x7F);
    nes->cpu.P.C = (temp > 0xff);
}

void cpu_and(struct NES *nes, byte_t arg)
{
    nes->cpu.A &= arg;
    nes->cpu.P.Z = (nes->cpu.A == 0);
    nes->cpu.P.S = (nes->cpu.A > 0x7F);
}

void cpu_asl(struct NES *nes, byte_t *arg)
{
    nes->cpu.P.C = (*arg > 0x7F);
    *arg <<= 1;
    nes->cpu.P.Z = (*arg == 0);
    nes->cpu.P.S = (*arg > 0x7F);
}

void cpu_bcc(struct NES *nes, int8_t offset)
{
    if(!nes->cpu.P.C)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_bcs(struct NES *nes, int8_t offset)
{
    if(!nes->cpu.P.C)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_beq(struct NES *nes, int8_t offset)
{
    if(nes->cpu.P.Z)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_bit(struct NES *nes, byte_t arg)
{
    nes->cpu.P.S = (arg > 0x7F);
    nes->cpu.P.V = ((arg & 0x40) > 0x3F);
    nes->cpu.P.Z = ((arg & nes->cpu.A) == 0);
}

void cpu_bmi(struct NES *nes, int8_t offset)
{
    if(nes->cpu.P.S)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_bne(struct NES *nes, int8_t offset)
{
    if(!nes->cpu.P.Z)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_bpl(struct NES *nes, int8_t offset)
{
    if(!nes->cpu.P.S)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_brk(struct NES *nes)
{
    cpu_push(nes, nes->cpu.pc >> 8);
    cpu_push(nes, nes->cpu.pc);
    nes->cpu.P.B = true;
    cpu_push(nes, psr_as_byte(nes));
    nes->cpu.P.I = true;
    nes->cpu.pc = cpumem_reads(nes, IRQ_VEC);
}

void cpu_bvc(struct NES *nes, int8_t offset)
{
    if(!nes->cpu.P.V)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_bvs(struct NES *nes, int8_t offset)
{
    if(nes->cpu.P.V)
    {
        nes->cpu.clock +=    ((nes->cpu.pc & 0xFF00)
                                != ((nes->cpu.pc + offset) & 0xFF00)
                                ? 2 : 1);
        nes->cpu.pc += offset;
    }
}

void cpu_clc(struct NES *nes)
{
    nes->cpu.P.C = false;
}

void cpu_cld(struct NES *nes)
{
    nes->cpu.P.D = false;
}

void cpu_cli(struct NES *nes)
{
    nes->cpu.P.I = false;
}

void cpu_clv(struct NES *nes)
{
    nes->cpu.P.V = false;
}

void cpu_cmp(struct NES *nes, byte_t arg)
{
    uint16_t temp = nes->cpu.A - arg;
    nes->cpu.P.C = (temp > 0xFF);
    nes->cpu.P.S = ((temp & 0xFF) > 0x7F);
    nes->cpu.P.Z = (temp == 0);
}

void cpu_cpx(struct NES *nes, byte_t arg)
{
    uint16_t temp = nes->cpu.X - arg;
    nes->cpu.P.C = (temp > 0xFF);
    nes->cpu.P.S = ((temp & 0xFF) > 0x7F);
    nes->cpu.P.Z = (temp == 0);
}

void cpu_cpy(struct NES *nes, byte_t arg)
{
    uint16_t temp = nes->cpu.Y - arg;
    nes->cpu.P.C = (temp > 0xFF);
    nes->cpu.P.S = ((temp & 0xFF) > 0x7F);
    nes->cpu.P.Z = (temp == 0);
}

void cpu_dec(struct NES *nes, byte_t *arg)
{
    *arg--;
    nes->cpu.P.S = (*arg > 0x7F);
    nes->cpu.P.Z = (*arg == 0);
}

void cpu_dex(struct NES *nes)
{
    nes->cpu.X--;
    nes->cpu.P.S = (nes->cpu.X > 0x7F);
    nes->cpu.P.Z = (nes->cpu.X == 0);
}

void cpu_dey(struct NES *nes)
{
    nes->cpu.Y--;
    nes->cpu.P.S = (nes->cpu.Y > 0x7F);
    nes->cpu.P.Z = (nes->cpu.Y == 0);
}

void cpu_eor(struct NES *nes, byte_t arg)
{
    nes->cpu.A ^= arg;
    nes->cpu.P.S = (nes->cpu.Y > 0x7F);
    nes->cpu.P.Z = (nes->cpu.Y == 0);
}

void cpu_inc(struct NES *nes, byte_t *arg)
{
    *arg++;
    nes->cpu.P.S = (*arg > 0x7F);
    nes->cpu.P.Z = (*arg == 0);
}

void cpu_inx(struct NES *nes)
{
    nes->cpu.X++;
    nes->cpu.P.S = (nes->cpu.X > 0x7F);
    nes->cpu.P.Z = (nes->cpu.X == 0);
}

void cpu_iny(struct NES *nes)
{
    nes->cpu.Y++;
    nes->cpu.P.S = (nes->cpu.Y > 0x7F);
    nes->cpu.P.Z = (nes->cpu.Y == 0);
}

void cpu_jmp(struct NES *nes, byte_t *arg)
{
    if(arg >= nes->cpu.memory.prg_low && arg < nes->cpu.memory.prg_low + 0x4000)
        nes->cpu.pc = 0x8000 + arg - nes->cpu.memory.prg_low;
    else if(arg >= nes->cpu.memory.prg_high && arg < nes->cpu.memory.prg_high + 0x4000)
        nes->cpu.pc = 0xC000 + arg - nes->cpu.memory.prg_high;
    else if(arg >= nes->cpu.memory.sram && arg < nes->cpu.memory.sram + 0x2000)
        nes->cpu.pc = 0x6000 + arg - nes->cpu.memory.sram;
    else if(arg >= nes->cpu.memory.erom && arg < nes->cpu.memory.erom + 0x1FE0)
        nes->cpu.pc = 0x4020 + arg - nes->cpu.memory.erom;
    else if(arg >= nes->cpu.memory.ram && arg < nes->cpu.memory.ram + 0x2000)
        nes->cpu.pc = arg - nes->cpu.memory.ram;
    else if(arg >= nes->cpu.memory.ppu_io && arg < nes->cpu.memory.ppu_io + 0x08)
        nes->cpu.pc = 0x2000 + arg - nes->cpu.memory.ppu_io;
    else if(arg >= nes->cpu.memory.apu_io && arg < nes->cpu.memory.apu_io + 0x20)
        nes->cpu.pc = 0x4000 + arg - nes->cpu.memory.apu_io;
}

void cpu_jsr(struct NES *nes, byte_t *arg)
{
    cpu_push(nes, (byte_t) (nes->cpu.pc >> 8));
    cpu_push(nes, (byte_t) nes->cpu.pc);
    cpu_jmp(nes, arg);
}

void cpu_lda(struct NES *nes, byte_t arg)
{
    nes->cpu.P.S = (arg > 0x7F);
    nes->cpu.P.Z = (arg == 0);
    nes->cpu.A = arg;
}

void cpu_ldx(struct NES *nes, byte_t arg)
{
    nes->cpu.P.S = (arg > 0x7F);
    nes->cpu.P.Z = (arg == 0);
    nes->cpu.X = arg;
}

void cpu_ldy(struct NES *nes, byte_t arg)
{
    nes->cpu.P.S = (arg > 0x7F);
    nes->cpu.P.Z = (arg == 0);
    nes->cpu.Y = arg;
}

void cpu_lsr(struct NES *nes, byte_t *arg)
{
    nes->cpu.P.C = ((*arg & 0x01) == 0x01);
    *arg >= 1;
    nes->cpu.P.S = false;
    nes->cpu.P.Z = (arg == 0);
}

void cpu_ora(struct NES *nes, byte_t arg)
{
    nes->cpu.A |= arg;
    nes->cpu.P.S = (nes->cpu.A > 0x7F);
    nes->cpu.P.Z = (nes->cpu.A == 0);
}

void cpu_pha(struct NES *nes)
{
    cpu_push(nes, nes->cpu.A);
}

void cpu_php(struct NES *nes)
{
    cpu_push(nes, psr_as_byte(nes));
}

void cpu_pla(struct NES *nes)
{
    nes->cpu.A = cpu_pop(nes);
}

void cpu_plp(struct NES *nes)
{
    byte_as_psr(nes, cpu_pop(nes));
}

void cpu_rol(struct NES *nes, byte_t *arg)
{
	bool carry = ((*arg & 0x80) == 0x80);
	*arg <<= 1;
	*arg |= nes->cpu.P.C;
	nes->cpu.P.C = carry;
	nes->cpu.P.S = (*arg > 0x7F);
    nes->cpu.P.Z = (*arg == 0);
}

void cpu_ror(struct NES *nes, byte_t *arg)
{
	bool carry = ((*arg & 0x01) == 0x01);
	*arg >>= 1;
	*arg |= (nes->cpu.P.C << 7);
	nes->cpu.P.C = carry;
	nes->cpu.P.S = (*arg > 0x7F);
    nes->cpu.P.Z = (*arg == 0);
}

void cpu_rti(struct NES *nes)
{
	byte_as_psr(nes, cpu_pop(nes));
	byte_t low = cpu_pop(nes);
	nes->cpu.pc = (cpu_pop(nes) << 8) | low;
}

void cpu_rts(struct NES *nes)
{
	byte_t low = cpu_pop(nes);
	nes->cpu.pc = (cpu_pop(nes) << 8) | low;
}

void cpu_sbc(struct NES *nes, byte_t arg)
{
	uint16_t temp = nes->cpu.A - arg - nes->cpu.P.C;
	nes->cpu.P.V =   !((nes->cpu.A ^ arg) & 0x80)
						&& ((nes->cpu.A ^ temp) & 0x80);
    nes->cpu.A = (byte_t) temp;
    nes->cpu.P.Z = (nes->cpu.A == 0);
    nes->cpu.P.S = (nes->cpu.A > 0x7F);
    nes->cpu.P.C = (temp > 0xff);

}

void cpu_sec(struct NES *nes)
{
	nes->cpu.P.C = true;
}

void cpu_sed(struct NES *nes)
{
	nes->cpu.P.D = true;
}

void cpu_sei(struct NES *nes)
{
    nes->cpu.P.I = true;
}

void cpu_sta(struct NES *nes, byte_t *arg)
{
	*arg = nes->cpu.A;
}

void cpu_stx(struct NES *nes, byte_t *arg)
{
	*arg = nes->cpu.X;
}

void cpu_sty(struct NES *nes, byte_t *arg)
{
	*arg = nes->cpu.Y;
}

void cpu_tax(struct NES *nes)
{
	nes->cpu.X = nes->cpu.A;
	nes->cpu.P.Z = (nes->cpu.X == 0);
    nes->cpu.P.S = (nes->cpu.X > 0x7F);
}

void cpu_tay(struct NES *nes)
{
	nes->cpu.Y = nes->cpu.A;
	nes->cpu.P.Z = (nes->cpu.Y == 0);
    nes->cpu.P.S = (nes->cpu.Y > 0x7F);
}

void cpu_tsx(struct NES *nes)
{
	nes->cpu.X = nes->cpu.sp;
	nes->cpu.P.Z = (nes->cpu.X == 0);
    nes->cpu.P.S = (nes->cpu.X > 0x7F);
}

void cpu_txa(struct NES *nes)
{
	nes->cpu.A = nes->cpu.X;
	nes->cpu.P.Z = (nes->cpu.A == 0);
	nes->cpu.P.S = (nes->cpu.A > 0x7F);
}

void cpu_txs(struct NES *nes)
{
	nes->cpu.sp = nes->cpu.X;
}

void cpu_tya(struct NES *nes)
{
	nes->cpu.A = nes->cpu.Y;
	nes->cpu.P.Z = (nes->cpu.A == 0);
	nes->cpu.P.S = (nes->cpu.A > 0x7F);
}
