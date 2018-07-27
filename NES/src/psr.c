#include "psr.h"

void init_psr(struct PSR *psr_handle)
{
    psr_handle->C = false;
    psr_handle->Z = false;
    psr_handle->I = false;
    psr_handle->D = false;
    psr_handle->B = false;
    psr_handle->V = false;
    psr_handle->S = false;
}

byte_t psr_as_byte(struct PSR *psr_handle)
{
    return  (psr_handle->S << 7) |
            (psr_handle->V << 6) |
            (psr_handle->B << 4) |
            (psr_handle->D << 3) |
            (psr_handle->I << 2) |
            (psr_handle->Z << 1) |
            (psr_handle->C);
}

void byte_as_psr(struct PSR *psr_handle, byte_t flags)
{
    psr_handle->C = ((flags & 0x01) == 0x01);
    psr_handle->Z = ((flags & 0x02) == 0x02);
    psr_handle->I = ((flags & 0x04) == 0x04);
    psr_handle->D = ((flags & 0x08) == 0x08);
    psr_handle->B = ((flags & 0x10) == 0x10);
    psr_handle->V = ((flags & 0x40) == 0x40);
    psr_handle->S = ((flags & 0x80) == 0x80);
}
