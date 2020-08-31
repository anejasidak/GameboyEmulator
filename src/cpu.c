#include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int cpuExecute0(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute1(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute2(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute3(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute4(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute5(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute6(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute7(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute8(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecute9(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteA(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteB(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteC(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteD(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteE(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}
int cpuExecuteF(uint16_t opcode)
{
    int cycles = 0;
    switch (opcode & 0x0f00)
    {
    default:
    {
        printf("Instruction %d has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteInstruction(uint16_t opcode)
{
    int cycles = 0;

    switch (opcode & 0xf000)
    {
    case 0x0000:
    {
        cycles = cpuExecute0(opcode);
    }
    break;
    case 0x1000:
    {
        cycles = cpuExecute1(opcode);
    }
    break;
    case 0x2000:
    {
        cycles = cpuExecute2(opcode);
    }
    break;
    case 0x3000:
    {
        cycles = cpuExecute3(opcode);
    }
    break;
    case 0x4000:
    {
        cycles = cpuExecute4(opcode);
    }
    break;
    case 0x5000:
    {
        cycles = cpuExecute5(opcode);
    }
    break;
    case 0x6000:
    {
        cycles = cpuExecute6(opcode);
    }
    break;
    case 0x7000:
    {
        cycles = cpuExecute7(opcode);
    }
    break;
    case 0x8000:
    {
        cycles = cpuExecute8(opcode);
    }
    break;
    case 0x9000:
    {
        cycles = cpuExecute9(opcode);
    }
    break;
    case 0xA000:
    {
        cycles = cpuExecuteA(opcode);
    }
    break;
    case 0xB000:
    {
        cycles = cpuExecuteB(opcode);
    }
    break;
    case 0xC000:
    {
        cycles = cpuExecuteC(opcode);
    }
    break;
    case 0xD000:
    {
        cycles = cpuExecuteD(opcode);
    }
    break;
    case 0xE000:
    {
        cycles = cpuExecuteE(opcode);
    }
    break;
    case 0xF000:
    {
        cycles = cpuExecuteF(opcode);
    }
    break;
    }
    return cycles;
}
