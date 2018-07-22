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
        case ADC_IMM:
        case ADC_ZP:
        case ADC_ZP_X:
        case ADC_AB:
        case ADC_AB_X:
        case ADC_AB_Y:
        case ADC_IR_X:
        case ADC_IR_Y:

        case AND_IMM:
        case AND_ZP:
        case AND_ZP_X:
        case AND_AB:
        case AND_AB_X:
        case AND_AB_Y:
        case AND_IR_X:
        case AND_IR_Y:

        case ASL_AC:
        case ASL_ZP:
        case ASL_ZP_X:
        case ASL_AB:
        case ASL_AB_X:

        case BCC:

        case BCS:

        case BEQ:

        case BIT_ZP:
        case BIT_AB:

        case BMI:

        case BNE:

        case BPL:

        case BRK:

        case BVC:

        case BVS:

        case CLC:

        case CLD:

        case CLI:

        case CLV:

        case CMP_IMM:
        case CMP_ZP:
        case CMP_ZP_X:
        case CMP_AB:
        case CMP_AB_X:
        case CMP_AB_Y:
        case CMP_IR_X:
        case CMP_IR_Y:

        case CPX_IMM:
        case CPX_ZP:
        case CPX_AB:

        case CPY_IMM:
        case CPY_ZP:
        case CPY_AB:

        case DEC_ZP:
        case DEC_ZP_X:
        case DEC_AB:
        case DEC_AB_X:

        case DEX:

        case DEY:

        case EOR_IMM:
        case EOR_ZP:
        case EOR_ZP_X:
        case EOR_AB:
        case EOR_AB_X:
        case EOR_AB_Y:
        case EOR_IR_X:
        case EOR_IR_Y:

        case INC_ZP:
        case INC_ZP_X:
        case INC_AB:
        case INC_AB_X:

        case INX:

        case INY:

        case JMP_IR:
        case JMP_AB:

        case JSR:

        case LDA_IMM:
        case LDA_ZP:
        case LDA_ZP_X:
        case LDA_AB:
        case LDA_AB_X:
        case LDA_AB_Y:
        case LDA_IR_X:
        case LDA_IR_Y:

        case LDX_IMM:
        case LDX_ZP:
        case LDX_ZP_Y:
        case LDX_AB:
        case LDX_AB_Y:

        case LDY_IMM:
        case LDY_ZP:
        case LDY_ZP_X:
        case LDY_AB:
        case LDY_AB_X:

        case LSR_AC:
        case LSR_ZP:
        case LSR_ZP_X:
        case LSR_AB:
        case LSR_AB_X:

        case NOP:

        case ORA_IMM:
        case ORA_ZP:
        case ORA_ZP_X:
        case ORA_AB:
        case ORA_AB_X:
        case ORA_AB_Y:
        case ORA_IR_X:
        case ORA_IR_Y:

        case PHA:

        case PHP:

        case PLA:

        case PLP:

        case ROL_AC:
        case ROL_ZP:
        case ROL_ZP_X:
        case ROL_AB:
        case ROL_AB_X:

        case ROR_AC:
        case ROR_ZP:
        case ROR_ZP_X:
        case ROR_AB:
        case ROR_AB_X:

        case RTI:

        case RTS:

        case SBC_IMM:
        case SBC_ZP:
        case SBC_ZP_X:
        case SBC_AB:
        case SBC_AB_X:
        case SBC_AB_Y:
        case SBC_IR_X:
        case SBC_IR_Y:

        case SEC:

        case SED:

        case SEI:

        case STA_ZP:
        case STA_ZP_X:
        case STA_AB:
        case STA_AB_X:
        case STA_AB_Y:
        case STA_IR_X:
        case STA_IR_Y:

        case STX_ZP:
        case STX_ZP_X:
        case STX_AB:

        case STY_ZP:
        case STY_ZP_X:
        case STY_AB:

        case TAX:

        case TAY:

        case TSX:

        case TXA:

        case TXS:

        case TYA:

        default:

    }
}
