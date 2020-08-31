#include <stdio.h>

#include "test.h"
#include "config.h"


void test_printRomBanks(struct s_gameboy *gameboy, int size)
{
    for (int i = 1; i <= size; i++)
    {
        printf("%02x", gameboy->memory.memoryArray[i - 1]);
        if (i % 2 == 0)
        {
            printf("\n");
        }
    }
}