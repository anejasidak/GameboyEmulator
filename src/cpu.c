#include "cpu.h"
#include "memory.h"
#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

static struct s_cpu *cpu;

static void setFlag(uint8_t flag);
static void resetFlag(uint8_t flag);
static void clearFlags();
static void load16BitRegister(uint16_t value, uint16_t *reg);
static void XOR(uint8_t value);

void initializeCpu(struct s_cpu *c)
{
    cpu = c;
}

static int cpuExecute0(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute1(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute2(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute3(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;
    switch (instructionCode)
    {
    case 0x31000000:
    {
        cpu->pc += 3;
        opcode = (opcode & 0x00ffff00) >> 8;
        load16BitRegister(opcode, &cpu->sp);
        cycles = 12;
    }
    break;
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute4(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute5(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute6(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute7(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute8(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecute9(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecuteA(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0xA8000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->b);
    }
    break;
    case 0xA9000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->c);
    }
    break;
    case 0xAA000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->d);
    }
    break;
    case 0xAB000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->e);
    }
    break;
    case 0xAC000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->h);
    }
    break;
    case 0xAD000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->l);
    }
    break;
    case 0xAE000000:
    {
        cpu->pc += 1;
        cycles = 8;
        XOR(memory_get(cpu->hl));
    }
    break;
    case 0xAF000000:
    {
        cpu->pc += 1;
        cycles = 4;
        XOR(cpu->a);
    }
    break;
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecuteB(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecuteC(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecuteD(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecuteE(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0xEE000000:
    {
        cpu->pc += 2;
        cycles = 8;
        XOR((opcode & 0x00ff0000) >> 16);
    }
    break;
    default:
    {
        printf("Instruction %ox has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

static int cpuExecuteF(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    default:
    {
        printf("Instruction %0x has not been implemented\n", opcode & 0xfff);
        assert(0);
    }
    break;
    }
    return cycles;
}

int cpuExecuteInstruction(uint32_t opcode)
{
    int cycles = 0;
    switch (opcode & 0xf0000000)
    {
    case 0x00000000:
    {
        cycles = cpuExecute0(opcode);
    }
    break;
    case 0x10000000:
    {
        cycles = cpuExecute1(opcode);
    }
    break;
    case 0x20000000:
    {
        cycles = cpuExecute2(opcode);
    }
    break;
    case 0x30000000:
    {
        cycles = cpuExecute3(opcode);
    }
    break;
    case 0x40000000:
    {
        cycles = cpuExecute4(opcode);
    }
    break;
    case 0x50000000:
    {
        cycles = cpuExecute5(opcode);
    }
    break;
    case 0x60000000:
    {
        cycles = cpuExecute6(opcode);
    }
    break;
    case 0x70000000:
    {
        cycles = cpuExecute7(opcode);
    }
    break;
    case 0x80000000:
    {
        cycles = cpuExecute8(opcode);
    }
    break;
    case 0x90000000:
    {
        cycles = cpuExecute9(opcode);
    }
    break;
    case 0xA0000000:
    {
        cycles = cpuExecuteA(opcode);
    }
    break;
    case 0xB0000000:
    {
        cycles = cpuExecuteB(opcode);
    }
    break;
    case 0xC0000000:
    {
        cycles = cpuExecuteC(opcode);
    }
    break;
    case 0xD0000000:
    {
        cycles = cpuExecuteD(opcode);
    }
    break;
    case 0xE0000000:
    {
        cycles = cpuExecuteE(opcode);
    }
    break;
    case 0xF0000000:
    {
        cycles = cpuExecuteF(opcode);
    }
    break;
    }
    return cycles;
}

static void setFlag(uint8_t flag)
{
    cpu->f |= flag;
}
static void resetFlag(uint8_t flag)
{
    cpu->f &= flag;
}

static void clearFlags()
{
    cpu->f = 0;
}

static void load16BitRegister(uint16_t value, uint16_t *reg)
{
    uint8_t highByte = value >> 8;
    uint8_t lowByte = value & 0xff;
    *reg = (lowByte << 8) | highByte;
}

static void XOR(uint8_t value)
{
    cpu->a ^= value;

    if (!cpu->a)
    {
        setFlag(FLAGS_ZERO);
    }

    resetFlag(FLAGS_CARRY | FLAGS_HALFCARRY | FLAGS_NEGATIVE);
}