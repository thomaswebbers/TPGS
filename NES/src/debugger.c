#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "nes.h"

char op_mode[256][7] =
{
	"IMP", "PREII", "IMP", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "ACC", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXX", "ZPIXX", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXX", "IXX",
	"ABS", "PREII", "IMP", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "ACC", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXX", "ZPIXX", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXX", "IXX",
	"IMP", "PREII", "IMP", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "ACC", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXX", "ZPIXX", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXX", "IXX",
	"IMP", "PREII", "IMP", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "ACC", "IMM", "IDR", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXX", "ZPIXX", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXX", "IXX",
	"IMM", "PREII", "IMM", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXY", "ZPIXY", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXY", "IXY",
	"IMM", "PREII", "IMM", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXY", "ZPIXY", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXY", "IXY",
	"IMM", "PREII", "IMM", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXX", "ZPIXX", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXX", "IXX",
	"IMM", "PREII", "IMM", "PREII", "ZP", "ZP", "ZP", "ZP", "IMP", "IMM", "IMP", "IMM", "ABS", "ABS", "ABS", "ABS",
	"REL", "POSII", "IMP", "POSII", "ZPIXX", "ZPIXX", "ZPIXX", "ZPIXX", "IMP", "IXY", "IMP", "IXY", "IXX", "IXX", "IXX", "IXX",
};


extern char op_names[256][4];

static uint16_t nes_number(struct NES *nes, char *str)
{
    if(memcmp(str, "pc", 2) == 0)
        return nes->cpu.pc;
    else if(memcmp(str, "sp", 2) == 0)
        return nes->cpu.sp + 0x100;
    else
        return strtol(str, NULL, 16);
}

static void print_cpumem(struct NES *nes, uint16_t address)
{
    printf(
        "  CPU memory:\n"
        "  address\tdata\toperation\n"
    );

    byte_t data;
    for(int i = address - 3; i < 4 + address; i++)
    {
        data = *cpumem_readbp(nes, i);
        printf(
            "  [%x]   \t%x \t%s_%s   \n",
            i, data, op_mode[data], op_names[data]
        );
    }
}

static void cpumem_scroll(struct NES *nes, uint16_t address)
{
	char str[20];
	uint16_t num;
	system("clear");
	for(;;)
	{
		print_cpumem(nes, address);
		printf("> ");
		fgets(str, 20, stdin);
		num = nes_number(nes, str + 2);
		printf("\033[2K\033[10A\033[7D");
		switch(str[0])
		{
			case 'u':
				address -= num;
				break;
			case 'd':
				address += num;
				break;
			case 'g':
				address = num;
				break;
			case 'q':
				printf("\033[10B");
				return;
		}
	}
}

static void print_cpu(struct NES *nes)
{

}

static void print_ppu(struct NES *nes)
{

}

static void print_mmc(struct NES *nes)
{

}

static void print_ppumem(struct NES *nes)
{

}

static void ppumem_scroll(struct NES *nes)
{

}

static void step(struct NES *nes)
{

}

static void run(struct NES *nes)
{

}

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
	int size = 20;
	char str[size];
    for(;;)
    {
        printf("> ");
        fgets(str, size, stdin);
        uint16_t num;
        FILE *fp;

        switch(str[0])
        {
            //cpu data
            case 'c':
				print_cpu(nes);
                break;

            //ppu data
            case 'p':
				print_ppu(nes);
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
            case 'd':
				num = nes_number(nes, str + 2);
				cpumem_scroll(nes, num);
				printf("test\n");
                break;

            //step machine x times
            case 's':
                num = strtol(str + 2, NULL, 16);
                for(int i = 0; i < num; i++)
                    step_nes(nes);
                break;

            //run vm
            case 'r':
                num = strtol(str + 2, NULL, 16);
                while(step_nes(nes) && nes->cpu.pc != num)
                {}
                break;

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
                    "  d      get data at specified address (hexadecimal)\n"
                    "  s x    step x times (hexadecimal)\n"
                    "  r x    run nes, breakpoint at x\n"
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
