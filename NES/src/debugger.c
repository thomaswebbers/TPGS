#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "nes.h"

extern char op_names[256][4];

int init_nes(struct NES **nes, char *binary_file)
{
    *nes = malloc(sizeof(struct NES));
    (**nes).master_clock = 0;
    init_mmc(*nes, binary_file);
    init_cpu(*nes);
    init_ppu(*nes);
    return INIT_SUCCES;
}

void run_nes(struct NES *nes)
{
    for(;;)
    {
        printf("> ");
        int size = 20;
        char *str = malloc(size * sizeof(char));
        fgets(str, size, stdin);
        uint16_t num;

        switch(str[0])
        {
            //get opcode and pgr-rom data around te current opcode
            case 'o':
            {
                byte_t opcode = *cpumem_readbp(nes, nes->cpu.pc);
                printf(
                    "  CPU memory:\n"
                    "  address\tdata\n"
                    "  [%x]\t%x\t\n"
                    "  [%x]\t%x\t\n"
                    "  [%x]\t%x\t\n"
                    "->[%x]\t%x\t%c%c%c\n"
                    "  [%x]\t%x\t\n"
                    "  [%x]\t%x\t\n"
                    "  [%x]\t%x\t\n",
                    nes->cpu.pc - 3, *cpumem_readbp(nes, nes->cpu.pc - 3),
                    nes->cpu.pc - 2, *cpumem_readbp(nes, nes->cpu.pc - 2),
                    nes->cpu.pc - 1, *cpumem_readbp(nes, nes->cpu.pc - 1),
                    nes->cpu.pc    , opcode,
                    op_names[opcode][0],
                    op_names[opcode][1],
                    op_names[opcode][2],
                    nes->cpu.pc + 1, *cpumem_readbp(nes, nes->cpu.pc + 1),
                    nes->cpu.pc + 2, *cpumem_readbp(nes, nes->cpu.pc + 2),
                    nes->cpu.pc + 3, *cpumem_readbp(nes, nes->cpu.pc + 3)
                );
                break;
            }

            //cpu data
            case 'c':
                printf(
                    "  CPU state:\n"
                    "  A      %x\n"
                    "  X      %x\n"
                    "  Y      %x\n"
                    "  P      %x\n"
                    "  sp     %x\n"
                    "  pc     %x\n",
                    nes->cpu.A,
                    nes->cpu.X,
                    nes->cpu.Y,
                    psr_as_byte(nes),
                    nes->cpu.sp,
                    nes->cpu.pc
                );
                break;

            //ppu data
            case 'p':
                break;

            //mmc and rom data
            case 'm':
                printf(
                    "  ROM data:\n"
                    "  prg banks        %x\n"
                    "  chr banks        %x\n"
                    "  mapper           %x\n"
                    "  mirroring        %x\n"
                    "  battery backed   %x\n",
                    nes->mmc.rom.num_prg,
                    nes->mmc.rom.num_chr,
                    nes->mmc.rom.mapper,
                    nes->mmc.rom.mirror,
                    nes->mmc.rom.battery
                );
                break;

            //get byte and bytes adjacent to the address
            case 'b':
                num = strtol(str + 2, NULL, 16);
                printf(
                    "  CPU memory:\n"
                    "  address\tdata\n"
                    "  [%x]  \t%x\t\n"
                    "  [%x]  \t%x\t\n"
                    "  [%x]  \t%x\t\n"
                    "->[%x]  \t%x\t\n"
                    "  [%x]  \t%x\t\n"
                    "  [%x]  \t%x\t\n"
                    "  [%x]  \t%x\t\n",
                    num - 3, *cpumem_readbp(nes, num - 3),
                    num - 2, *cpumem_readbp(nes, num - 2),
                    num - 1, *cpumem_readbp(nes, num - 1),
                    num    , *cpumem_readbp(nes, num),
                    num + 1, *cpumem_readbp(nes, num + 1),
                    num + 2, *cpumem_readbp(nes, num + 2),
                    num + 3, *cpumem_readbp(nes, num + 3)
                );
                break;

            //step machine x times
            case 's':
                num = strtol(str + 2, NULL, 16);
                for(int i = 0; i < num; i++)
                    step_nes(nes);
                break;

            //run vm
            case 'r':
                while(step_nes(nes))
                {}
                return;

            //quit vm
            case 'q':
                printf("Are you sure you want to quit? Y/n\n> ");
                fgets(str, size, stdin);
                if(str[0] == 'Y' || str[0] == 'y')
                    return;
                break;

            //help
            case 'h':
                printf(
                    "  c      show cpu registers\n"
                    "  p      show ppu registers\n"
                    "  m      show mmc and rom data\n"
                    "  b      get byte at specified address (hexadecimal)\n"
                    "  s      step x times (hexadecimal)\n"
                    "  r      run nes\n"
                    "  q      quit debugger\n"
                    "  h      help\n"
                );
                break;

            default:
                fprintf(stderr, "Unsupported command %c\n", str[0]);
                break;
        }
    }
}

bool step_nes(struct NES *nes)
{
    nes->master_clock++;
    bool test = cpu_step(nes);
    step_mmc(nes);
    return test;
}

void destroy_nes(struct NES *nes)
{
    destroy_cpu(nes);
    destroy_ppu(nes);
    destroy_mmc(nes);
    free(nes);
}
