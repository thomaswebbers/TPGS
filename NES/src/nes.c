#include "nes.h"
#include "stdlib.h"

void run_nes(struct NES *nes_handle)
{
    while(step_nes(nes_handle))
    {
        ;
    }
}

int init_nes(struct NES **nes_handle, char *binary_file)
{
    *nes_handle = malloc(sizeof(NES));
}

void destroy_nes(struct NES *nes_handle)
{
    free(nes_handle);
}

bool step_nes(struct NES *nes_handle)
{
    switch()
    {
        /***********************************************
        **************Immediate Addressing**************
        ***********************************************/
        case IMM_ADC:
        case IMM_AND:
        case IMM_CMP:
        case IMM_CPX:
        case IMM_CPY:
        case IMM_EOR:
        case IMM_LDA:
        case IMM_LDX:
        case IMM_LDY:
        case IMM_ORA:
        case IMM_SBC:

        /***********************************************
        **************Zero-Page Addressing**************
        ***********************************************/
        case ZP_ADC:
        case ZP_AND:
        case ZP_ASL:
        case ZP_BIT:
        case ZP_CMP:
        case ZP_CPX:
        case ZP_CPY:
        case ZP_DEC:
        case ZP_EOR:
        case ZP_INC:
        case ZP_LDA:
        case ZP_LDX:
        case ZP_LDY:
        case ZP_LSR:
        case ZP_ORA:
        case ZP_ROL:
        case ZP_ROR:
        case ZP_SBC:
        case ZP_STA:
        case ZP_STX:
        case ZP_STY:

        /***********************************************
        **************Absolute Addressing***************
        ***********************************************/
        case ABS_ADC:
        case ABS_AND:
        case ABS_ASL:
        case ABS_BIT:
        case ABS_CMP:
        case ABS_CPX:
        case ABS_CPY:
        case ABS_DEC:
        case ABS_EOR:
        case ABS_INC:
        case ABS_LDA:
        case ABS_LDX:
        case ABS_LDY:
        case ABS_LSR:
        case ABS_ORA:
        case ABS_ROL:
        case ABS_ROR:
        case ABS_SBC:
        case ABS_STA:
        case ABS_STX:
        case ABS_STY:

        /***********************************************
        **************Implied Addresssing***************
        ***********************************************/
        case IMP_BRK:
        case IMP_CLC:
        case IMP_CLD:
        case IMP_CLI:
        case IMP_CLV:
        case IMP_DEX:
        case IMP_DEY:
        case IMP_INX:
        case IMP_INY:
        case IMP_NOP:
        case IMP_PHA:
        case IMP_PHP:
        case IMP_PLA:
        case IMP_PLP:
        case IMP_RTI:
        case IMP_RTS:
        case IMP_TAX:
        case IMP_TAY:
        case IMP_TSX:
        case IMP_TXA:
        case IMP_TXS:
        case IMP_TYA:

        /***********************************************
        **************Accumulator Addressing************
        ***********************************************/
        case ACC_ASL:
        case ACC_LSR:
        case ACC_ROL:
        case ACC_ROR:

        /***********************************************
        ******Indexed Addressing (index register X)*****
        ***********************************************/
        case IXX_ADC:
        case IXX_AND:
        case IXX_ASL:
        case IXX_CMP:
        case IXX_DEC:
        case IXX_EOR:
        case IXX_INC:
        case IXX_LDA:
        case IXX_LDY:
        case IXX_LSR:
        case IXX_ORA:
        case IXX_ROL:
        case IXX_ROR:
        case IXX_SBC:
        case IXX_STA:

        /***********************************************
        ******Indexed Addressing (index register Y)*****
        ***********************************************/
        case IXY_ADC:
        case IXY_AND:
        case IXY_CMP:
        case IXY_EOR:
        case IXY_LDA:
        case IXY_LDX:
        case IXY_ORA:
        case IXY_SBC:
        case IXY_STA:

        /***********************************************
        *Zero-Page Indexed Addressing(index register X)*
        ***********************************************/
        case ZPIXX_ADC:
        case ZPIXX_AND:
        case ZPIXX_ASL:
        case ZPIXX_CMP:
        case ZPIXX_DEC:
        case ZPIXX_EOR:
        case ZPIXX_INC:
        case ZPIXX_LDA:
        case ZPIXX_LDY:
        case ZPIXX_LSR:
        case ZPIXX_ORA:
        case ZPIXX_ROL:
        case ZPIXX_ROR:
        case ZPIXX_SBC:
        case ZPIXX_STA:
        case ZPIXX_STY:

        /***********************************************
        *Zero-Page Indexed Addressing(index register Y)*
        ***********************************************/
        case ZPIXY_LDX:
        case ZPIXY_STX:

        /***********************************************
        **************Indirect Addressing***************
        ***********************************************/
        case IDR_JMP:

        /***********************************************
        ********Pre-Indexed Indirect Addressing*********
        ***********************************************/
        case PREII_ADC:
        case PREII_AND:
        case PREII_CMP:
        case PREII_EOR:
        case PREII_LDA:
        case PREII_ORA:
        case PREII_SBC:
        case PREII_STA:

        /***********************************************
        ********Post-Indexed Indirect Addressing********
        ***********************************************/
        case POSII_ADC:
        case POSII_AND:
        case POSII_CMP:
        case POSII_EOR:
        case POSII_LDA:
        case POSII_ORA:
        case POSII_SBC:
        case POSII_STA:

        /***********************************************
        **************Relative Addressing***************
        ***********************************************/
        case REL_BCC:
        case REL_BCS:
        case REL_BEQ:
        case REL_BMI:
        case REL_BNE:
        case REL_BPL:
        case REL_BVC:
        case REL_BVS:

        default:

    }
}
