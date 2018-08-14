#include "nes.h"

void init_psr(struct NES *nes)
{
    nes->cpu.P.C = false;
    nes->cpu.P.Z = false;
    nes->cpu.P.I = false;
    nes->cpu.P.D = false;
    nes->cpu.P.B = false;
    nes->cpu.P.V = false;
    nes->cpu.P.S = false;
}

byte_t psr_as_byte(struct NES *nes)
{
    return  (nes->cpu.P.S << 7) |
            (nes->cpu.P.V << 6) |
            (nes->cpu.P.B << 4) |
            (nes->cpu.P.D << 3) |
            (nes->cpu.P.I << 2) |
            (nes->cpu.P.Z << 1) |
            (nes->cpu.P.C);
}

void byte_as_psr(struct NES *nes, byte_t flags)
{
    nes->cpu.P.C = ((flags & 0x01) == 0x01);
    nes->cpu.P.Z = ((flags & 0x02) == 0x02);
    nes->cpu.P.I = ((flags & 0x04) == 0x04);
    nes->cpu.P.D = ((flags & 0x08) == 0x08);
    nes->cpu.P.B = ((flags & 0x10) == 0x10);
    nes->cpu.P.V = ((flags & 0x40) == 0x40);
    nes->cpu.P.S = ((flags & 0x80) == 0x80);
}
