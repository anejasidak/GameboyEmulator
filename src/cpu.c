#include "cpu.h"
#include "memory.h"
#include "config.h"
#include "cb.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

static struct s_cpu *cpu;

static void ld8BitToRegister(uint8_t value, uint8_t *reg);
static void ld16BitToRegister(uint16_t value, uint16_t *reg);
static void ld8BitToMemory(uint8_t value, uint16_t index);
static void XOR(uint8_t value);

const uint8_t instructionSize[256] = {
    1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
    1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1,
    2, 1, 2, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1,
    2, 1, 2, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1};

uint8_t getInstructionSize(uint8_t index)
{
    return instructionSize[index];
}
void initializeCpu(struct s_cpu *c)
{
    cpu = c;
    clearFlags();
}

static int cpuExecute0(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0x01000000:
    {
        ld16BitToRegister((opcode & 0x00ffff00) >> 8, &cpu->bc);
    }
    break;
    case 0x06000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->b);
    }
    break;
    case 0x0E000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->c);
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

static int cpuExecute1(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0x11000000:
    {
        ld16BitToRegister((opcode & 0x00ffff00) >> 8, &cpu->de);
        cycles = 12;
    }
    break;
    case 0x16000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->d);
    }
    break;
    case 0x1E000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->e);
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

static int cpuExecute2(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    /*
    case 0x20000000:
    {
        
    }
    break;
    */
    case 0x21000000:
    {
        ld16BitToRegister((opcode & 0x00ffff00) >> 8, &cpu->hl);
        cycles = 12;
    }
    break;
    case 0x26000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->h);
    }
    break;
    case 0x2E000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->l);
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

static int cpuExecute3(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;
    switch (instructionCode)
    {
    case 0x31000000:
    {
        ld16BitToRegister((opcode & 0x00ffff00) >> 8, &cpu->sp);
        cycles = 12;
    }
    break;
    case 0x32000000:
    {
        ld8BitToMemory(cpu->a, cpu->hl);
        cpu->hl--;
        cycles = 8;
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
    case 0x40000000:
    {
        ld8BitToRegister(cpu->b, &cpu->b);
    }
    break;
    case 0x41000000:
    {
        ld8BitToRegister(cpu->c, &cpu->b);
    }
    break;
    case 0x42000000:
    {
        ld8BitToRegister(cpu->d, &cpu->b);
    }
    break;
    case 0x43000000:
    {
        ld8BitToRegister(cpu->e, &cpu->b);
    }
    break;
    case 0x44000000:
    {
        ld8BitToRegister(cpu->h, &cpu->b);
    }
    break;
    case 0x45000000:
    {
        ld8BitToRegister(cpu->l, &cpu->b);
    }
    break;
    case 0x46000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->b);
    }
    break;
    case 0x48000000:
    {
        ld8BitToRegister(cpu->b, &cpu->c);
    }
    break;
    case 0x49000000:
    {
        ld8BitToRegister(cpu->c, &cpu->c);
    }
    break;
    case 0x4A000000:
    {
        ld8BitToRegister(cpu->d, &cpu->c);
    }
    break;
    case 0x4B000000:
    {
        ld8BitToRegister(cpu->e, &cpu->c);
    }
    break;
    case 0x4C000000:
    {
        ld8BitToRegister(cpu->h, &cpu->c);
    }
    break;
    case 0x4D000000:
    {
        ld8BitToRegister(cpu->l, &cpu->c);
    }
    break;
    case 0x4E000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->c);
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

static int cpuExecute5(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0x50000000:
    {
        ld8BitToRegister(cpu->b, &cpu->d);
    }
    break;
    case 0x51000000:
    {
        ld8BitToRegister(cpu->c, &cpu->d);
    }
    break;
    case 0x52000000:
    {
        ld8BitToRegister(cpu->d, &cpu->d);
    }
    break;
    case 0x53000000:
    {
        ld8BitToRegister(cpu->e, &cpu->d);
    }
    break;
    case 0x54000000:
    {
        ld8BitToRegister(cpu->h, &cpu->d);
    }
    break;
    case 0x55000000:
    {
        ld8BitToRegister(cpu->l, &cpu->d);
    }
    break;
    case 0x56000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->d);
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
    case 0x78000000:
    {
        ld8BitToRegister(cpu->b, &cpu->a);
    }
    break;
    case 0x79000000:
    {
        ld8BitToRegister(cpu->c, &cpu->a);
    }
    break;
    case 0x7A000000:
    {
        ld8BitToRegister(cpu->d, &cpu->a);
    }
    break;
    case 0x7B000000:
    {
        ld8BitToRegister(cpu->e, &cpu->a);
    }
    break;
    case 0x7C000000:
    {
        ld8BitToRegister(cpu->h, &cpu->a);
    }
    break;
    case 0x7D000000:
    {
        ld8BitToRegister(cpu->l, &cpu->a);
    }
    break;
    case 0x7E000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->a);
    }
    break;
    case 0x7F000000:
    {
        ld8BitToRegister(cpu->a, &cpu->a);
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
        cycles = 4;
        XOR(cpu->b);
    }
    break;
    case 0xA9000000:
    {
        cycles = 4;
        XOR(cpu->c);
    }
    break;
    case 0xAA000000:
    {
        cycles = 4;
        XOR(cpu->d);
    }
    break;
    case 0xAB000000:
    {
        cycles = 4;
        XOR(cpu->e);
    }
    break;
    case 0xAC000000:
    {
        cycles = 4;
        XOR(cpu->h);
    }
    break;
    case 0xAD000000:
    {
        cycles = 4;
        XOR(cpu->l);
    }
    break;
    case 0xAE000000:
    {
        cycles = 8;
        XOR(memory_get(cpu->hl));
    }
    break;
    case 0xAF000000:
    {
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
    case 0xCB000000:
    {

        cpuExecuteCB(cpu, opcode);
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

void setFlag(uint8_t flag)
{
    cpu->f |= flag;
}
void resetFlag(uint8_t flag)
{
    cpu->f &= flag;
}

void clearFlags()
{
    cpu->f = 0;
}

static void ld16BitToRegister(uint16_t value, uint16_t *reg)
{

    uint8_t highByte = value >> 8;
    uint8_t lowByte = value & 0xff;
    *reg = (lowByte << 8) | highByte;
}

static void ld8BitToRegister(uint8_t value, uint8_t *reg)
{
    *reg = value;
}
static void ld8BitToMemory(uint8_t value, uint16_t index)
{
    memory_set(index, value);
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