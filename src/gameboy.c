#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "gameboy.h"

static struct s_gameboy *gameboy;

void intializeSystem(struct s_gameboy *gb)
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
    gameboy->cpu.pc += getInstructionSize(opcode >> 24);

    int x = opcode >> (8 * (4 - getInstructionSize(opcode >> 24)));
    printf("Executing %8x instruction\n", x);

    cpuExecuteInstruction(opcode);
    printf("A: %0x \tB: %0x \tC: %0x \tD: %0x \tE: %0x \tH: %0x \tL: %0x \tSP: %0x\n",
           gameboy->cpu.a, gameboy->cpu.b, gameboy->cpu.c, gameboy->cpu.d, gameboy->cpu.e, gameboy->cpu.h,
           gameboy->cpu.l, gameboy->cpu.sp);
    printf("\tF: %0x\n", gameboy->cpu.f);
    printf("Pc is: %0x\n", gameboy->cpu.pc);
}