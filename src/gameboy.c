#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "gameboy.h"

static struct s_gameboy *gameboy;

void intializeSystem(struct s_gameboy* gb)
{
    memset(gb, 0, sizeof(struct s_gameboy));

    intializeMemory(&gb->memory);
    initializeCpu(&gb->cpu);

    gameboy = gb;
}

_Bool readCartridge(const char *filename)
{
    FILE *f = fopen(filename, "r");

    if (!f)
    {
        printf("Failed to open file\n");
        return 0;
    }
    if (fread(gameboy->cartridge, 1, CARTRIDGE_SIZE, f) <= 1)
    {
        printf("Could not read file\n");
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}

void loadGameIntoMemory()
{
    memcpy(&gameboy->memory, &gameboy->cartridge, ROM_BANK_1_END + 1);
}

void executeNextInstruction()
{
    uint32_t opcode = memory_get_ins(gameboy->cpu.pc);
    printf("Executing %08x instruction\n", opcode);
    cpuExecuteInstruction(opcode);
}