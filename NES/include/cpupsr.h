#ifndef CPUPS_H
#define CPUPS_H
#include <stdbool.h>
#include "opcodes.h"
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

struct PSR
{
    bool C;   //Carry
    bool Z;   //Zero
    bool I;   //Interrupt Disable
    //decimal mode unsupported so omitted
    bool B;   //Break Command
    //empty bit
    bool V;   //overflow
    bool S;   //Negative
};

void init_psr(struct PSR *psr_handle);

/*
 * Returns the Processor Status Register P as a byte with the same structure as
 * the P register in the original NES.
 *
 * @param psr_handle Processor Status Register P handle
 *
 * @return Processor Status Register P as byte
 */
byte_t psr_as_byte(struct PSR *psr_handle);

#endif
