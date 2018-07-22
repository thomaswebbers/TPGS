#include "nes.h"
#include "stdio.h"

struct NES *nes_handle;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
      fprintf(stderr, "Usage: ./tnes rom\n");
      return 1;
    }

    if((int exit_flag = init_nes(&nes_handle, argv[1])) != INIT_SUCCES)
    {
        fprintf(stderr, "Error initializing TNES\n
                         Error flag: %d\n", exit_flag);
        return -1;
    }

    run_nes(nes_handle);

    destroy_nes(nes_handle);
}
