#include <stdio.h>

#include "test.h"
#include "config.h"
#include "cpu.h"

static void test_ld8(struct s_gameboy *gameboy);

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
void test_allTests(struct s_gameboy *gameboy)
{
    test_ld8(gameboy);
}

static void test_ld8(struct s_gameboy *gameboy)
{
    cpuExecuteInstruction(0x06100000);

    printf("After, 0x06 Instruction (LD B, d8), value in reg b is: %0x\n", gameboy->cpu.b);

    cpuExecuteInstruction(0x0E010000);

    printf("After, 0x0E Instruction (LD C, d8), value in reg c is: %0x\n", gameboy->cpu.c);

    cpuExecuteInstruction(0x26ab0000);

    printf("After, 0x26 Instruction (LD H, d8), value in reg h is: %0x\n", gameboy->cpu.h);

    cpuExecuteInstruction(0x1Eef0000);

    printf("After, 0x1E Instruction (LD E, d8), value in reg e is: %0x\n", gameboy->cpu.e);

    /*------------------------*/

    gameboy->cpu.b = 0xf4;
    cpuExecuteInstruction(0x50000000);

    printf("After storing 0xf4 in reg b and, 0x50 Instruction (LD D,B), value in reg d is: %0x\n", gameboy->cpu.d);

    gameboy->cpu.hl = 0xfffe;
    cpuExecuteInstruction(0x36590000);
    cpuExecuteInstruction(0x46000000);

    printf("After storing 0x59 in memory pointed by hl reg and, 0x46 Instruction (LD B, (HL)), value in reg b is: %0x\n", gameboy->cpu.b);

    cpuExecuteInstruction(0x31feff00);
    printf("The address pointed to by stack pointer is: %0x\n", gameboy->cpu.sp);

    gameboy->cpu.hl = 0x5123;
    cpuExecuteInstruction(0xE5000000);
    cpuExecuteInstruction(0xF1000000);
    printf("After putting 0x5123 to register hl, and pushing it onto the stack, then popping it off the stack, it's value is: %0x\n",gameboy->cpu.af);
}