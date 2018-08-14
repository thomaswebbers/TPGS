#ifndef MMC1_H
#define MMC1_H
#include "mmc.h"

struct NES;

void init_mmc1(struct NES *nes);

void step_mmc1(struct NES *nes);

#endif
