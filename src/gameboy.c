#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "gameboy.h"

static struct s_gameboy *gameboy;

static void loadHeaderToMemory();

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
    loadHeaderToMemory();
}

void executeNextInstruction()
{
    uint32_t opcode = memory_get_ins(gameboy->cpu.pc);
    gameboy->cpu.pc += getInstructionSize(opcode >> 24);

    int x = opcode >> (8 * (4 - getInstructionSize(opcode >> 24)));
    char c;
     cpuExecuteInstruction(opcode);

    if ((gameboy->cpu.pc >= 0x7 && gameboy->cpu.pc <= 0xa)
    || (gameboy->cpu.pc >= 0x95 && gameboy->cpu.pc <= 0xa7)
    ||  (gameboy->cpu.pc >= 0x27 && gameboy->cpu.pc <= 0x32))
    {

    }
    else
    {
        printf("Execute instruction?\n");
        scanf("%c", &c);
        printf("Executing %8x instruction\n", x);

        printf("Pc is: %0x\n", gameboy->cpu.pc);
        printf("A: %0x \tF: %0x \tB: %0x \tC: %0x \tD: %0x \tE: %0x \tH: %0x \tL: %0x \tSP: %0x\n",
               gameboy->cpu.a,  gameboy->cpu.f, gameboy->cpu.b, gameboy->cpu.c, gameboy->cpu.d, gameboy->cpu.e, gameboy->cpu.h,
               gameboy->cpu.l, gameboy->cpu.sp);
    }
}

static void loadHeaderToMemory()
{
    uint8_t headerValues[] = {
        0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
        0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
        0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
        0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
        0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
        0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e};

    for (uint16_t i = HEADER_START_ADDR, j = 0; i <= HEADER_END_ADDR; i++, j++)
    {

        gameboy->memory.memoryArray[i] = headerValues[j];
    }
}