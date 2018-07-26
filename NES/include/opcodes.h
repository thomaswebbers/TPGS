#ifndef OPCODES_H
#define OPCODES_H
#include <stdint.h>

//basic unit of the NES
typedef uint8_t byte_t;

/*
 * OPCODE FORMAT:
 * ==============
 * ADDRESSINGMODE_OPCODE
 *
 * ADDRESSING MODES:
 * =================
 * IMM      - Immediate
 * ZP       - Zero-Page
 * ABS      - Absolute
 * IMP      - Implied
 * ACC      - Accumulator
 * IXX      - Indexed (index register X)
 * IXY      - Indexed (index register Y)
 * ZPIXX    - Zero-Page Indexed (index register X)
 * ZPIXY    - Zero-Page Indexed (index register Y)
 * IDR      - Indirect
 * PREII    - Pre-Indexed Indirect
 * POSII    - Post-Indexed Indirect
 * REL      - Relative
 *
 * OPCODES:
 * ========
 * ADC      - Add M to A with Carry
 * AND      - "AND" M with A
 * ASL      - Shift Left One Bit (M or A)
 * BCC      - Branch on Carry Clear
 * BCS      - Branch on Carry Set
 * BEQ      - Branch on Result Zero
 * BIT      - Test Bits in M with A
 * BMI      - Branch on Result Minus
 * BNE      - Branch on Result not Zero
 * BPL      - Branch on Result Plus
 * BRK      - Force Break
 * BVC      - Branch on Overflow Clear
 * BVS      - Branch on Overflow Set
 * CLC      - Clear Carry Flag
 * CLD      - Clear Decimal Mode
 * CLI      - Clear interrupt Disable Bit
 * CLV      - Clear Overflow Flag
 * CMP      - Compare M and A
 * CPX      - Compare M and X
 * CPY      - Compare M and Y
 * DEC      - Decrement M by One
 * DEX      - Decrement X by One
 * DEY      - Decrement Y by One
 * EOR      - "Exclusive     -Or" M with A
 * INC      - Increment M by One
 * INX      - Increment X by One
 * INY      - Increment Y by One
 * JMP      - Jump to Location
 * JSR      - Jump to Location Save Return Address
 * LDA      - Load A with M
 * LDX      - Load X with M
 * LDY      - Load Y with M
 * LSR      - Shift Right One Bit (M or A)
 * NOP      - No Operation
 * ORA      - "OR" M with A
 * PHA      - Push A on Stack
 * PHP      - Push Processor Status on Stack
 * PLA      - Pull A from Stack
 * PLP      - Pull Processor Status from Stack
 * ROL      - Rotate One Bit Left (M or A)
 * ROR      - Rotate One Bit Right (M or A)
 * RTI      - Return from Interrupt
 * RTS      - Return from Subroutine
 * SBC      - Subtract M from A with Borrow
 * SEC      - Set Carry Flag
 * SED      - Set Decimal Mode
 * SEI      - Set Interrupt Disable Status
 * STA      - Store A in M
 * STX      - Store X in M
 * STY      - Store Y in M
 * TAX      - Transfer A to X
 * TAY      - Transfer A to Y
 * TSX      - Transfer Stack Pointer to X
 * TXA      - Transfer X to A
 * TXS      - Transfer X to Stack Pointer
 * TYA      - Transfer Y to A
 */

#define IMM_ADC         ((byte_t) 0x69)
#define IMM_AND         ((byte_t) 0x29)
#define IMM_CMP         ((byte_t) 0xC9)
#define IMM_CPX         ((byte_t) 0xE0)
#define IMM_CPY         ((byte_t) 0xC0)
#define IMM_EOR         ((byte_t) 0x49)
#define IMM_LDA         ((byte_t) 0xA9)
#define IMM_LDX         ((byte_t) 0xA2)
#define IMM_LDY         ((byte_t) 0xA0)
#define IMM_ORA         ((byte_t) 0x09)
#define IMM_SBC         ((byte_t) 0xE9)

#define ZP_ADC          ((byte_t) 0x65)
#define ZP_AND          ((byte_t) 0x25)
#define ZP_ASL          ((byte_t) 0x06)
#define ZP_BIT          ((byte_t) 0x24)
#define ZP_CMP          ((byte_t) 0xC5)
#define ZP_CPX          ((byte_t) 0xE4)
#define ZP_CPY          ((byte_t) 0xC4)
#define ZP_DEC          ((byte_t) 0xC6)
#define ZP_EOR          ((byte_t) 0x45)
#define ZP_INC          ((byte_t) 0xE6)
#define ZP_LDA          ((byte_t) 0xA5)
#define ZP_LDX          ((byte_t) 0xA6)
#define ZP_LDY          ((byte_t) 0xA4)
#define ZP_LSR          ((byte_t) 0x46)
#define ZP_ORA          ((byte_t) 0x05)
#define ZP_ROL          ((byte_t) 0x26)
#define ZP_ROR          ((byte_t) 0x66)
#define ZP_SBC          ((byte_t) 0xE5)
#define ZP_STA          ((byte_t) 0x85)
#define ZP_STX          ((byte_t) 0x86)
#define ZP_STY          ((byte_t) 0x84)

#define ABS_ADC         ((byte_t) 0x6D)
#define ABS_AND         ((byte_t) 0x2D)
#define ABS_ASL         ((byte_t) 0x0E)
#define ABS_BIT         ((byte_t) 0x2C)
#define ABS_CMP         ((byte_t) 0xCD)
#define ABS_CPX         ((byte_t) 0xEC)
#define ABS_CPY         ((byte_t) 0xCC)
#define ABS_DEC         ((byte_t) 0xCE)
#define ABS_EOR         ((byte_t) 0x4D)
#define ABS_INC         ((byte_t) 0xEE)
#define ABS_JMP         ((byte_t) 0x4C)
#define ABS_JSR         ((byte_t) 0x20)
#define ABS_LDA         ((byte_t) 0xAD)
#define ABS_LDX         ((byte_t) 0xAE)
#define ABS_LDY         ((byte_t) 0xAC)
#define ABS_LSR         ((byte_t) 0x4E)
#define ABS_ORA         ((byte_t) 0x0D)
#define ABS_ROL         ((byte_t) 0x2E)
#define ABS_ROR         ((byte_t) 0x6E)
#define ABS_SBC         ((byte_t) 0xED)
#define ABS_STA         ((byte_t) 0x8D)
#define ABS_STX         ((byte_t) 0x8E)
#define ABS_STY         ((byte_t) 0x8C)

#define IMP_BRK         ((byte_t) 0x00)
#define IMP_CLC         ((byte_t) 0x18)
#define IMP_CLD         ((byte_t) 0xD8)
#define IMP_CLI         ((byte_t) 0x58)
#define IMP_CLV         ((byte_t) 0xB8)
#define IMP_DEX         ((byte_t) 0xCA)
#define IMP_DEY         ((byte_t) 0x88)
#define IMP_INX         ((byte_t) 0xE8)
#define IMP_INY         ((byte_t) 0xC8)
#define IMP_NOP         ((byte_t) 0xEA)
#define IMP_PHA         ((byte_t) 0x48)
#define IMP_PHP         ((byte_t) 0x08)
#define IMP_PLA         ((byte_t) 0x68)
#define IMP_PLP         ((byte_t) 0x28)
#define IMP_RTI         ((byte_t) 0x40)
#define IMP_RTS         ((byte_t) 0x60)
#define IMP_SEC         ((byte_t) 0x38)
#define IMP_SED         ((byte_t) 0xF8)
#define IMP_SEI         ((byte_t) 0x78)
#define IMP_TAX         ((byte_t) 0xAA)
#define IMP_TAY         ((byte_t) 0xA8)
#define IMP_TSX         ((byte_t) 0xBA)
#define IMP_TXA         ((byte_t) 0x8A)
#define IMP_TXS         ((byte_t) 0x9A)
#define IMP_TYA         ((byte_t) 0x98)

#define ACC_ASL         ((byte_t) 0x0A)
#define ACC_LSR         ((byte_t) 0x4A)
#define ACC_ROL         ((byte_t) 0x2A)
#define ACC_ROR         ((byte_t) 0x6A)

#define IXX_ADC         ((byte_t) 0x7D)
#define IXX_AND         ((byte_t) 0x3D)
#define IXX_ASL         ((byte_t) 0x1E)
#define IXX_CMP         ((byte_t) 0xDD)
#define IXX_DEC         ((byte_t) 0xDE)
#define IXX_EOR         ((byte_t) 0x5D)
#define IXX_INC         ((byte_t) 0xFE)
#define IXX_LDA         ((byte_t) 0xBD)
#define IXX_LDY         ((byte_t) 0xBC)
#define IXX_LSR         ((byte_t) 0x5E)
#define IXX_ORA         ((byte_t) 0x1D)
#define IXX_ROL         ((byte_t) 0x3E)
#define IXX_ROR         ((byte_t) 0x7E)
#define IXX_SBC         ((byte_t) 0xFD)
#define IXX_STA         ((byte_t) 0x9D)

#define IXY_ADC         ((byte_t) 0x79)
#define IXY_AND         ((byte_t) 0x39)
#define IXY_CMP         ((byte_t) 0xD9)
#define IXY_EOR         ((byte_t) 0x59)
#define IXY_LDA         ((byte_t) 0xB9)
#define IXY_LDX         ((byte_t) 0xBE)
#define IXY_ORA         ((byte_t) 0x19)
#define IXY_SBC         ((byte_t) 0xF9)
#define IXY_STA         ((byte_t) 0x99)

#define ZPIXX_ADC       ((byte_t) 0x75)
#define ZPIXX_AND       ((byte_t) 0x35)
#define ZPIXX_ASL       ((byte_t) 0x16)
#define ZPIXX_CMP       ((byte_t) 0xD5)
#define ZPIXX_DEC       ((byte_t) 0xD6)
#define ZPIXX_EOR       ((byte_t) 0x55)
#define ZPIXX_INC       ((byte_t) 0xF6)
#define ZPIXX_LDA       ((byte_t) 0xB5)
#define ZPIXX_LDY       ((byte_t) 0xB4)
#define ZPIXX_LSR       ((byte_t) 0x56)
#define ZPIXX_ORA       ((byte_t) 0x15)
#define ZPIXX_ROL       ((byte_t) 0x36)
#define ZPIXX_ROR       ((byte_t) 0x76)
#define ZPIXX_SBC       ((byte_t) 0xF5)
#define ZPIXX_STA       ((byte_t) 0x95)
#define ZPIXX_STY       ((byte_t) 0x94)

#define ZPIXY_LDX       ((byte_t) 0xB6)
#define ZPIXY_STX       ((byte_t) 0x96)

#define IDR_JMP         ((byte_t) 0x6C)

#define PREII_ADC       ((byte_t) 0x61)
#define PREII_AND       ((byte_t) 0x21)
#define PREII_CMP       ((byte_t) 0xC1)
#define PREII_EOR       ((byte_t) 0x41)
#define PREII_LDA       ((byte_t) 0xA1)
#define PREII_ORA       ((byte_t) 0x01)
#define PREII_SBC       ((byte_t) 0xE1)
#define PREII_STA       ((byte_t) 0x81)

#define POSII_ADC       ((byte_t) 0x71)
#define POSII_AND       ((byte_t) 0x31)
#define POSII_CMP       ((byte_t) 0xD1)
#define POSII_EOR       ((byte_t) 0x51)
#define POSII_LDA       ((byte_t) 0xB1)
#define POSII_ORA       ((byte_t) 0x11)
#define POSII_SBC       ((byte_t) 0xF1)
#define POSII_STA       ((byte_t) 0x91)

#define REL_BCC         ((byte_t) 0x90)
#define REL_BCS         ((byte_t) 0xB0)
#define REL_BEQ         ((byte_t) 0xF0)
#define REL_BMI         ((byte_t) 0x30)
#define REL_BNE         ((byte_t) 0xD0)
#define REL_BPL         ((byte_t) 0x10)
#define REL_BVC         ((byte_t) 0x50)
#define REL_BVS         ((byte_t) 0x70)

/*
 * The operations on the NES have different amounts of cycles depending on the
 * operation and sometimes branches taken within one operation. To emulate the
 * NES correctly, the amount of cycles used by each operation should be taken
 * into account.
 */

#define C_ONE           ((uint32_t) 1)
#define C_TWO           ((uint32_t) 2)
#define C_THREE         ((uint32_t) 3)
#define C_FOUR          ((uint32_t) 4)
#define C_FIVE          ((uint32_t) 5)
#define C_SIX           ((uint32_t) 6)
#define C_SEVEN         ((uint32_t) 7)

//A cycle to be added if Indexing Accros Page Boundary
#define C_IAPB          ((uint32_t) 1)
//A cycle to be added if a Branch is Taken
#define C_BT            ((uint32_t) 1)

#endif
