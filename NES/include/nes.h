#ifndef NES_H
#define NES_H
#include <stdint.h>

typedef uint8_t byte_t;

/*
 * OPCODE FORMAT:
 * ==============
 * OPCODE_ADDRESSINGMODE(_REGISTER)
 *
 * OPCODES:
 * ========
 *
 *
 * ADDRESSING MODES:
 * =================
 * IMM => Immediate
 * ZP  => Zero-Page
 * AB  => Absolute
 * IMP => Implied
 * AC  => Accumulator
 * IX  => Indexed
 * ZPI => Zero-Page Indexed
 * IR  => Indirect
 * PRE => Pre-Indexed Indirect
 * POS => Post-Indexed Indirect
 * RE  => Relative
 *
 * REGISTER:
 * =========
 * X
 * Y
 */

#define ADC_IMM         ((byte_t) 0x69)
#define ADC_ZP          ((byte_t) 0x65)
#define ADC_ZP_X        ((byte_t) 0x75)
#define ADC_AB          ((byte_t) 0x6D)
#define ADC_AB_X        ((byte_t) 0x7D)
#define ADC_AB_Y        ((byte_t) 0x79)
#define ADC_IR_X        ((byte_t) 0x61)
#define ADC_IR_Y        ((byte_t) 0x71)

#define AND_IMM         ((byte_t) 0x29)
#define AND_ZP          ((byte_t) 0x25)
#define AND_ZP_X        ((byte_t) 0x35)
#define AND_AB          ((byte_t) 0x2D)
#define AND_AB_X        ((byte_t) 0x3D)
#define AND_AB_Y        ((byte_t) 0x39)
#define AND_IR_X        ((byte_t) 0x21)
#define AND_IR_Y        ((byte_t) 0x31)

#define ASL_AC          ((byte_t) 0x0A)
#define ASL_ZP          ((byte_t) 0x06)
#define ASL_ZP_X        ((byte_t) 0x16)
#define ASL_AB          ((byte_t) 0x0E)
#define ASL_AB_X        ((byte_t) 0x1E)

#define BCC             ((byte_t) 0x90)

#define BCS             ((byte_t) 0xB0)

#define BEQ             ((byte_t) 0xF0)

#define BIT_ZP          ((byte_t) 0x24)
#define BIT_AB          ((byte_t) 0x2C)

#define BMI             ((byte_t) 0x30)

#define BNE             ((byte_t) 0xD0)

#define BPL             ((byte_t) 0x10)

#define BRK             ((byte_t) 0x00)

#define BVC             ((byte_t) 0x50)

#define BVS             ((byte_t) 0x70)

#define CLC             ((byte_t) 0x18)

#define CLD             ((byte_t) 0xD8)

#define CLI             ((byte_t) 0x58)

#define CLV             ((byte_t) 0xB8)

#define CMP_IMM         ((byte_t) 0xC9)
#define CMP_ZP          ((byte_t) 0xC5)
#define CMP_ZP_X        ((byte_t) 0xD5)
#define CMP_AB          ((byte_t) 0xCD)
#define CMP_AB_X        ((byte_t) 0xDD)
#define CMP_AB_Y        ((byte_t) 0xD9)
#define CMP_IR_X        ((byte_t) 0xC1)
#define CMP_IR_Y        ((byte_t) 0xD1)

#define CPX_IMM         ((byte_t) 0xE0)
#define CPX_ZP          ((byte_t) 0xE4)
#define CPX_AB          ((byte_t) 0xEC)

#define CPY_IMM         ((byte_t) 0xC0)
#define CPY_ZP          ((byte_t) 0xC4)
#define CPY_AB          ((byte_t) 0xCC)

#define DEC_ZP          ((byte_t) 0xC6)
#define DEC_ZP_X        ((byte_t) 0xD6)
#define DEC_AB          ((byte_t) 0xCE)
#define DEC_AB_X        ((byte_t) 0xDE)

#define DEX             ((byte_t) 0xCA)

#define DEY             ((byte_t) 0x88)

#define EOR_IMM         ((byte_t) 0x49)
#define EOR_ZP          ((byte_t) 0x45)
#define EOR_ZP_X        ((byte_t) 0x55)
#define EOR_AB          ((byte_t) 0x4D)
#define EOR_AB_X        ((byte_t) 0x5D)
#define EOR_AB_Y        ((byte_t) 0x59)
#define EOR_IR_X        ((byte_t) 0x41)
#define EOR_IR_Y        ((byte_t) 0x51)

#define INC_ZP          ((byte_t) 0xE6)
#define INC_ZP_X        ((byte_t) 0xF6)
#define INC_AB          ((byte_t) 0xEE)
#define INC_AB_X        ((byte_t) 0xFE)

#define INX             ((byte_t) 0xE8)

#define INY             ((byte_t) 0xC8)

#define JMP_IR          ((byte_t) 0x6C)
#define JMP_AB          ((byte_t) 0x4C)

#define JSR             ((byte_t) 0x20)

#define LDA_IMM         ((byte_t) 0xA9)
#define LDA_ZP          ((byte_t) 0xA5)
#define LDA_ZP_X        ((byte_t) 0xB5)
#define LDA_AB          ((byte_t) 0xAD)
#define LDA_AB_X        ((byte_t) 0xB5)
#define LDA_AB_Y        ((byte_t) 0xB9)
#define LDA_IR_X        ((byte_t) 0xA1)
#define LDA_IR_Y        ((byte_t) 0xB1)

#define LDX_IMM         ((byte_t) 0xA2)
#define LDX_ZP          ((byte_t) 0xA6)
#define LDX_ZP_Y        ((byte_t) 0xB6)
#define LDX_AB          ((byte_t) 0xAE)
#define LDX_AB_Y        ((byte_t) 0xBE)

#define LDY_IMM         ((byte_t) 0xA0)
#define LDY_ZP          ((byte_t) 0xA4)
#define LDY_ZP_X        ((byte_t) 0xB4)
#define LDY_AB          ((byte_t) 0xAC)
#define LDY_AB_X        ((byte_t) 0xBC)

#define LSR_AC          ((byte_t) 0x4A)
#define LSR_ZP          ((byte_t) 0x46)
#define LSR_ZP_X        ((byte_t) 0x56)
#define LSR_AB          ((byte_t) 0x4E)
#define LSR_AB_X        ((byte_t) 0x5E)

#define NOP             ((byte_t) 0xEA)

#define ORA_IMM         ((byte_t) 0x09)
#define ORA_ZP          ((byte_t) 0x05)
#define ORA_ZP_X        ((byte_t) 0x15)
#define ORA_AB          ((byte_t) 0x0D)
#define ORA_AB_X        ((byte_t) 0x1D)
#define ORA_AB_Y        ((byte_t) 0x19)
#define ORA_IR_X        ((byte_t) 0x01)
#define ORA_IR_Y        ((byte_t) 0x11)

#define PHA             ((byte_t) 0x48)

#define PHP             ((byte_t) 0x08)

#define PLA             ((byte_t) 0x68)

#define PLP             ((byte_t) 0x28)

#define ROL_AC          ((byte_t) 0x2A)
#define ROL_ZP          ((byte_t) 0x26)
#define ROL_ZP_X        ((byte_t) 0x36)
#define ROL_AB          ((byte_t) 0x2E)
#define ROL_AB_X        ((byte_t) 0x3E)

#define ROR_AC          ((byte_t) 0x6A)
#define ROR_ZP          ((byte_t) 0x66)
#define ROR_ZP_X        ((byte_t) 0x76)
#define ROR_AB          ((byte_t) 0x6E)
#define ROR_AB_X        ((byte_t) 0x7E)

#define RTI             ((byte_t) 0x40)

#define RTS             ((byte_t) 0x60)

#define SBC_IMM         ((byte_t) 0xE9)
#define SBC_ZP          ((byte_t) 0xE5)
#define SBC_ZP_X        ((byte_t) 0xF5)
#define SBC_AB          ((byte_t) 0xED)
#define SBC_AB_X        ((byte_t) 0xFD)
#define SBC_AB_Y        ((byte_t) 0xF9)
#define SBC_IR_X        ((byte_t) 0xE1)
#define SBC_IR_Y        ((byte_t) 0xF1)

#define SEC             ((byte_t) 0x38)

#define SED             ((byte_t) 0xF8)

#define SEI             ((byte_t) 0x78)

#define STA_ZP          ((byte_t) 0x85)
#define STA_ZP_X        ((byte_t) 0x95)
#define STA_AB          ((byte_t) 0x8D)
#define STA_AB_X        ((byte_t) 0x9D)
#define STA_AB_Y        ((byte_t) 0x99)
#define STA_IR_X        ((byte_t) 0x81)
#define STA_IR_Y        ((byte_t) 0x91)

#define STX_ZP          ((byte_t) 0x84)
#define STX_ZP_X        ((byte_t) 0x94)
#define STX_AB          ((byte_t) 0x8E)

#define STY_ZP          ((byte_t) 0x84)
#define STY_ZP_X        ((byte_t) 0x94)
#define STY_AB          ((byte_t) 0x8C)

#define TAX             ((byte_t) 0xAA)

#define TAY             ((byte_t) 0xA8)

#define TSX             ((byte_t) 0xBA)

#define TXA             ((byte_t) 0x8A)

#define TXS             ((byte_t) 0x9A)

#define TYA             ((byte_t) 0x98)

struct NES
{
    /*
     * MEMORY LAYOUT:
     * ==============
     * 0x100   => Zero Page
     * 0x200   => Stack
     * 0x800   => RAM
     * 0x2000  => Mirrors (0-0x7FF)
     * 0x2008  => I/O Registers
     * 0x4000  => Mirrors (0x2000-0x2007)
     * 0x4020  => I/O Registers
     * 0x6000  => Expansion ROM
     * 0x8000  => SRAM
     * 0xC000  => PRG-ROM (Lower Bank)
     * 0x10000 => PRG-ROM (Upper Bank)
     */
    byte_t *memory;

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
      * Processor status
      * =================
      * 0 => Carry (if last instruction resulted in under/overflow)
      * 1 => Zero (if last instruction's result was 0)
      * 2 => Interrupt Disable (Enable to prevent system from responding to interrupts)
      * 3 => Decimal mode (unsupported on this chip variant)
      * 4 => Break Command (if BRK (break) instruction has been executed)
      * 5 => Empty
      * 6 => Overflow (if previous instruction resulted in an invalid two's complement)
      * 7 => Negative
      */
     byte_t P;
}

int init_nes(char *binary_path);

void run_nes(void);

void step_nes(void);

void destroy_nes(void);

#endif
