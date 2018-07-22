#include "nes.h"

int main()
{
    if((int error_flag = init_nes()) != INIT_SUCCES)
    {
        fprintf(stderr, "Error initializing TNES\n
                         Error flag: %d", error_flag);
        return -1;
    }

    run_nes();

    destroy_nes();
}
