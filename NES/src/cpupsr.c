#include "cpupsr.h"

void init_psr(struct PSR *psr_handle)
{
    psr_handle->C = false;
    psr_handle->Z = false;
    psr_handle->I = false;
    psr_handle->B = false;
    psr_handle->V = false;
    psr_handle->S = false;
}

byte_t psr_as_byte(struct PSR *psr_handle)
{
    return  (psr_handle->S << 7) |
            (psr_handle->V << 6) |
            (psr_handle->B << 4) |
            (psr_handle->I << 2) |
            (psr_handle->Z << 1) |
            (psr_handle->C);
}
