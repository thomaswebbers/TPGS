#include "cpu.h"

void cpu_add_cycles(struct NES *nes_handle, uint32_t cycles)
{
    nes_handle->cpu.clock += cycles;
}

byte_t cpu_current_byte(struct NES *nes_handle)
{
    return nes_handle->cpu.memory[nes_handle->cpu.pc];
}

bool cpu_step(struct NES *nes_handle)
{
    if(nes_handle->master_clock < nes_handle->cpu.clock)
    {
        return true;
    }

    switch(cpu_current_byte(nes_handle))
    {
        /***********************************************
        **************Immediate Addressing**************
        ***********************************************/
        case IMM_ADC:
            break;
        case IMM_AND:
            break;
        case IMM_CMP:
            break;
        case IMM_CPX:
            break;
        case IMM_CPY:
            break;
        case IMM_EOR:
            break;
        case IMM_LDA:
            break;
        case IMM_LDX:
            break;
        case IMM_LDY:
            break;
        case IMM_ORA:
            break;
        case IMM_SBC:
            break;

        /***********************************************
        **************Zero-Page Addressing**************
        ***********************************************/
        case ZP_ADC:
            break;
        case ZP_AND:
            break;
        case ZP_ASL:
            break;
        case ZP_BIT:
            break;
        case ZP_CMP:
            break;
        case ZP_CPX:
            break;
        case ZP_CPY:
            break;
        case ZP_DEC:
            break;
        case ZP_EOR:
            break;
        case ZP_INC:
            break;
        case ZP_LDA:
            break;
        case ZP_LDX:
            break;
        case ZP_LDY:
            break;
        case ZP_LSR:
            break;
        case ZP_ORA:
            break;
        case ZP_ROL:
            break;
        case ZP_ROR:
            break;
        case ZP_SBC:
            break;
        case ZP_STA:
            break;
        case ZP_STX:
            break;
        case ZP_STY:
            break;

        /***********************************************
        **************Absolute Addressing***************
        ***********************************************/
        case ABS_ADC:
            break;
        case ABS_AND:
            break;
        case ABS_ASL:
            break;
        case ABS_BIT:
            break;
        case ABS_CMP:
            break;
        case ABS_CPX:
            break;
        case ABS_CPY:
            break;
        case ABS_DEC:
            break;
        case ABS_EOR:
            break;
        case ABS_INC:
            break;
        case ABS_LDA:
            break;
        case ABS_LDX:
            break;
        case ABS_LDY:
            break;
        case ABS_LSR:
            break;
        case ABS_ORA:
            break;
        case ABS_ROL:
            break;
        case ABS_ROR:
            break;
        case ABS_SBC:
            break;
        case ABS_STA:
            break;
        case ABS_STX:
            break;
        case ABS_STY:
            break;

        /***********************************************
        **************Implied Addresssing***************
        ***********************************************/
        case IMP_BRK:
            break;
        case IMP_CLC:
            break;
        case IMP_CLD:
            break;
        case IMP_CLI:
            break;
        case IMP_CLV:
            break;
        case IMP_DEX:
            break;
        case IMP_DEY:
            break;
        case IMP_INX:
            break;
        case IMP_INY:
            break;
        case IMP_NOP:
            break;
        case IMP_PHA:
            break;
        case IMP_PHP:
            break;
        case IMP_PLA:
            break;
        case IMP_PLP:
            break;
        case IMP_RTI:
            break;
        case IMP_RTS:
            break;
        case IMP_TAX:
            break;
        case IMP_TAY:
            break;
        case IMP_TSX:
            break;
        case IMP_TXA:
            break;
        case IMP_TXS:
            break;
        case IMP_TYA:
            break;

        /***********************************************
        **************Accumulator Addressing************
        ***********************************************/
        case ACC_ASL:
            break;
        case ACC_LSR:
            break;
        case ACC_ROL:
            break;
        case ACC_ROR:
            break;

        /***********************************************
        ******Indexed Addressing (index register X)*****
        ***********************************************/
        case IXX_ADC:
            break;
        case IXX_AND:
            break;
        case IXX_ASL:
            break;
        case IXX_CMP:
            break;
        case IXX_DEC:
            break;
        case IXX_EOR:
            break;
        case IXX_INC:
            break;
        case IXX_LDA:
            break;
        case IXX_LDY:
            break;
        case IXX_LSR:
            break;
        case IXX_ORA:
            break;
        case IXX_ROL:
            break;
        case IXX_ROR:
            break;
        case IXX_SBC:
            break;
        case IXX_STA:
            break;

        /***********************************************
        ******Indexed Addressing (index register Y)*****
        ***********************************************/
        case IXY_ADC:
            break;
        case IXY_AND:
            break;
        case IXY_CMP:
            break;
        case IXY_EOR:
            break;
        case IXY_LDA:
            break;
        case IXY_LDX:
            break;
        case IXY_ORA:
            break;
        case IXY_SBC:
            break;
        case IXY_STA:
            break;

        /***********************************************
        *Zero-Page Indexed Addressing(index register X)*
        ***********************************************/
        case ZPIXX_ADC:
            break;
        case ZPIXX_AND:
            break;
        case ZPIXX_ASL:
            break;
        case ZPIXX_CMP:
            break;
        case ZPIXX_DEC:
            break;
        case ZPIXX_EOR:
            break;
        case ZPIXX_INC:
            break;
        case ZPIXX_LDA:
            break;
        case ZPIXX_LDY:
            break;
        case ZPIXX_LSR:
            break;
        case ZPIXX_ORA:
            break;
        case ZPIXX_ROL:
            break;
        case ZPIXX_ROR:
            break;
        case ZPIXX_SBC:
            break;
        case ZPIXX_STA:
            break;
        case ZPIXX_STY:
            break;

        /***********************************************
        *Zero-Page Indexed Addressing(index register Y)*
        ***********************************************/
        case ZPIXY_LDX:
            break;
        case ZPIXY_STX:
            break;

        /***********************************************
        **************Indirect Addressing***************
        ***********************************************/
        case IDR_JMP:
            break;

        /***********************************************
        ********Pre-Indexed Indirect Addressing*********
        ***********************************************/
        case PREII_ADC:
            break;
        case PREII_AND:
            break;
        case PREII_CMP:
            break;
        case PREII_EOR:
            break;
        case PREII_LDA:
            break;
        case PREII_ORA:
            break;
        case PREII_SBC:
            break;
        case PREII_STA:
            break;

        /***********************************************
        ********Post-Indexed Indirect Addressing********
        ***********************************************/
        case POSII_ADC:
            break;
        case POSII_AND:
            break;
        case POSII_CMP:
            break;
        case POSII_EOR:
            break;
        case POSII_LDA:
            break;
        case POSII_ORA:
            break;
        case POSII_SBC:
            break;
        case POSII_STA:
            break;

        /***********************************************
        **************Relative Addressing***************
        ***********************************************/
        case REL_BCC:
            break;
        case REL_BCS:
            break;
        case REL_BEQ:
            break;
        case REL_BMI:
            break;
        case REL_BNE:
            break;
        case REL_BPL:
            break;
        case REL_BVC:
            break;
        case REL_BVS:
            break;

        default:
            break;
    }
    return true;
}