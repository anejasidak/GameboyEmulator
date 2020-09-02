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
static uint16_t get16BitFromOpcode(uint32_t opcode);

static void push(uint16_t value);
static uint16_t pop();

static void add_A(uint8_t value);
static void sub_A(uint8_t value);

static void OR(uint8_t value);
static void AND(uint8_t value);
static void XOR(uint8_t value);

static void inc(uint8_t *reg);
static void dec(uint8_t *reg);

static void cp(uint8_t value);

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
    2, 1, 1, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1,
    2, 1, 1, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1};

uint8_t getInstructionSize(uint8_t index)
{
    return instructionSize[index];
}
void initializeCpu(struct s_cpu *c)
{
    cpu = c;
    clearFlags();
}
static uint16_t getBigEndianValue(uint16_t value)
{

    uint8_t highByte = value >> 8;
    uint8_t lowByte = value & 0xff;
    return (lowByte << 8) | highByte;
}

static int cpuExecute0(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0x01000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        ld16BitToRegister(getBigEndianValue(imm), &cpu->bc);
    }
    break;
    case 0x02000000:
    {
        memory_set(cpu->bc, cpu->a);
    }
    break;
    case 0x03000000:
    {
        cpu->bc += 1;
    }
    break;
    case 0x04000000:
    {
        inc(&cpu->b);
    }
    break;
    case 0x05000000:
    {
        dec(&cpu->b);
    }
    break;
    case 0x06000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->b);
    }
    break;
    case 0x08000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        imm = getBigEndianValue(imm);
        memory_set(imm, (cpu->sp & 0xff00) >> 8);
        memory_set(imm + 1, cpu->sp & 0xff);
    }
    break;
    case 0x0A000000:
    {
        ld8BitToRegister(memory_get(cpu->bc), &cpu->a);
    }
    break;
    case 0x0C000000:
    {
        inc(&cpu->c);
    }
    break;

    case 0x0D000000:
    {
        dec(&cpu->c);
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
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        ld16BitToRegister(getBigEndianValue(imm), &cpu->de);
    }
    break;
    case 0x12000000:
    {
        memory_set(cpu->de, cpu->a);
    }
    break;
    case 0x13000000:
    {
        cpu->de += 1;
    }
    break;
    case 0x14000000:
    {
        inc(&cpu->d);
    }
    break;
    case 0x15000000:
    {
        dec(&cpu->d);
    }
    break;
    case 0x16000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->d);
    }
    break;
    case 0x17000000:
    {
        rl(&cpu->a);
    }
    break;
    case 0x18000000:
    {
        signed char imm = (opcode & 0x00ff0000) >> 16;

        cpu->pc += imm;
    }
    break;
    case 0x1A000000:
    {

        ld8BitToRegister(memory_get(cpu->de), &cpu->a);
        printf("putting value %0x from address %0x. (Memory[cpu->de]) to register A. Now value at register A is %0x\n",
               memory_get(cpu->de), cpu->de, cpu->a);
    }
    break;
    case 0x1C000000:
    {
        inc(&cpu->e);
    }
    break;
    case 0x1D000000:
    {
        dec(&cpu->e);
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
    case 0x20000000:
    {
        signed char imm = (opcode & 0x00ff0000) >> 16;
        if (!isFlagSet(FLAGS_ZERO))
        {
            printf("0x20 Flag is not zero, jumping\n");
            cpu->pc += imm;
        }
    }
    break;
    case 0x21000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        ld16BitToRegister(getBigEndianValue(imm), &cpu->hl);
    }
    break;
    case 0x22000000:
    {
        memory_set(cpu->hl, cpu->a);
        cpu->hl++;
    }
    break;
    case 0x23000000:
    {
        cpu->hl += 1;
    }
    break;
    case 0x24000000:
    {
        inc(&cpu->h);
    }
    break;
    case 0x25000000:
    {
        dec(&cpu->h);
    }
    break;
    case 0x26000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->h);
    }
    break;
    case 0x28000000:
    {
        signed char imm = (opcode & 0x00ff0000) >> 16;
        if (isFlagSet(FLAGS_ZERO))
        {
            cpu->pc += imm;
        }
    }
    break;
    case 0x2A000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->a);
        cpu->hl++;
    }
    break;
    case 0x2c000000:
    {
        inc(&cpu->l);
    }
    break;
    case 0x2D000000:
    {
        dec(&cpu->l);
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
    case 0x30000000:
    {
        signed char imm = (opcode & 0x00ff0000) >> 16;
        if (!isFlagSet(FLAGS_CARRY))
        {
            cpu->pc += imm;
        }
    }
    break;
    case 0x31000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        ld16BitToRegister(getBigEndianValue(imm), &cpu->sp);
    }
    break;
    case 0x32000000:
    {
        printf("putting value %0x from register A at address %0x. (hl)\n",
               cpu->a, cpu->hl);
        memory_set(cpu->hl, cpu->a);
        cpu->hl--;
        printf("hl now is:  %0x and value at address (hl + 1) is %0x\n", cpu->hl, memory_get(cpu->hl + 1));
    }
    break;
    case 0x03000000:
    {
        cpu->sp += 1;
    }
    break;
    case 0x34000000:
    {
        uint8_t val = memory_get(cpu->hl);

        if ((val & 0x0f) + 1 > 0x0f)
        {
            setFlag(FLAGS_HALFCARRY);
        }
        else
        {
            resetFlag(FLAGS_HALFCARRY);
        }

        val += 1;
        memory_set(cpu->hl, val);
        if (val == 0)
        {
            setFlag(FLAGS_ZERO);
        }
        else
        {
            resetFlag(FLAGS_ZERO);
        }
        resetFlag(FLAGS_NEGATIVE);
    }
    break;
    case 0x35000000:
    {
        uint8_t val = memory_get(cpu->hl);

        if ((val & 0x0f))
        {
            setFlag(FLAGS_HALFCARRY);
        }
        else
        {
            resetFlag(FLAGS_HALFCARRY);
        }
        val -= 1;
        memory_set(cpu->hl, val);
        if (val == 0)
        {
            setFlag(FLAGS_ZERO);
        }
        else
        {
            resetFlag(FLAGS_ZERO);
        }
        setFlag(FLAGS_NEGATIVE);
    }
    break;
    case 0x36000000:
    {
        memory_set(cpu->hl, (opcode & 0x00ff0000) >> 16);
    }
    break;
    case 0x38000000:
    {
        signed char imm = (opcode & 0x00ff0000) >> 16;
        if (isFlagSet(FLAGS_CARRY))
        {
            cpu->pc += imm;
        }
    }
    break;
    case 0x3A000000:
    {
        printf("Bringing value %0x at address %0x to register A\n",
               memory_get(cpu->hl), cpu->hl);
        ld8BitToRegister(memory_get(cpu->hl), &cpu->a);
        cpu->hl--;
    }
    break;
    case 0x3C000000:
    {
        inc(&cpu->a);
    }
    break;
    case 0x3D000000:
    {
        dec(&cpu->a);
    }
    break;
    case 0x3E000000:
    {
        ld8BitToRegister((opcode & 0x00ff0000) >> 16, &cpu->a);
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
    case 0x47000000:
    {
        ld8BitToRegister(cpu->a, &cpu->b);
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
    case 0x4F000000:
    {
        ld8BitToRegister(cpu->a, &cpu->c);
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
    case 0x57000000:
    {
        ld8BitToRegister(cpu->a, &cpu->d);
    }
    break;
    case 0x58000000:
    {
        ld8BitToRegister(cpu->b, &cpu->e);
    }
    break;
    case 0x59000000:
    {
        ld8BitToRegister(cpu->c, &cpu->e);
    }
    break;
    case 0x5A000000:
    {
        ld8BitToRegister(cpu->d, &cpu->e);
    }
    break;
    case 0x5B000000:
    {
        ld8BitToRegister(cpu->e, &cpu->e);
    }
    break;
    case 0x5C000000:
    {
        ld8BitToRegister(cpu->h, &cpu->e);
    }
    break;
    case 0x5D000000:
    {
        ld8BitToRegister(cpu->l, &cpu->e);
    }
    break;
    case 0x5E000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->e);
    }
    case 0x5F000000:
    {
        ld8BitToRegister(cpu->a, &cpu->e);
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
    case 0x60000000:
    {
        ld8BitToRegister(cpu->b, &cpu->h);
    }
    break;
    case 0x61000000:
    {
        ld8BitToRegister(cpu->c, &cpu->h);
    }
    break;
    case 0x62000000:
    {
        ld8BitToRegister(cpu->d, &cpu->h);
    }
    break;
    case 0x63000000:
    {
        ld8BitToRegister(cpu->e, &cpu->h);
    }
    break;
    case 0x64000000:
    {
        ld8BitToRegister(cpu->h, &cpu->h);
    }
    break;
    case 0x65000000:
    {
        ld8BitToRegister(cpu->l, &cpu->h);
    }
    break;
    case 0x66000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->h);
    }
    break;
    case 0x67000000:
    {
        ld8BitToRegister(cpu->a, &cpu->h);
    }
    break;
    case 0x68000000:
    {
        ld8BitToRegister(cpu->b, &cpu->l);
    }
    break;
    case 0x69000000:
    {
        ld8BitToRegister(cpu->c, &cpu->l);
    }
    break;
    case 0x6A000000:
    {
        ld8BitToRegister(cpu->d, &cpu->l);
    }
    break;
    case 0x6B000000:
    {
        ld8BitToRegister(cpu->e, &cpu->l);
    }
    break;
    case 0x6C000000:
    {
        ld8BitToRegister(cpu->h, &cpu->l);
    }
    break;
    case 0x6D000000:
    {
        ld8BitToRegister(cpu->l, &cpu->l);
    }
    break;
    case 0x6E000000:
    {
        ld8BitToRegister(memory_get(cpu->hl), &cpu->l);
    }
    break;
    case 0x6F000000:
    {
        ld8BitToRegister(cpu->a, &cpu->l);
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

static int cpuExecute7(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0x70000000:
    {
        memory_set(cpu->hl, cpu->b);
    }
    break;
    case 0x71000000:
    {
        memory_set(cpu->hl, cpu->c);
    }
    break;
    case 0x72000000:
    {
        memory_set(cpu->hl, cpu->d);
    }
    break;
    case 0x73000000:
    {
        memory_set(cpu->hl, cpu->e);
    }
    break;
    case 0x74000000:
    {
        memory_set(cpu->hl, cpu->h);
    }
    break;
    case 0x75000000:
    {
        memory_set(cpu->hl, cpu->l);
    }
    break;
    case 0xF6000000:
    {
        OR((opcode & 0x00ff0000) >> 16);
    }
    break;
    case 0x77000000:
    {
        printf("putting value %0x from register A at address %0x. (hl)\n",
               cpu->a, cpu->hl);
        memory_set(cpu->hl, cpu->a);
    }
    break;
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
    case 0x80000000:
    {
        add_A(cpu->b);
    }
    break;
    case 0x81000000:
    {
        add_A(cpu->c);
    }
    break;
    case 0x82000000:
    {
        add_A(cpu->d);
    }
    break;
    case 0x83000000:
    {
        add_A(cpu->e);
    }
    break;
    case 0x84000000:
    {
        add_A(cpu->h);
    }
    break;
    case 0x85000000:
    {
        add_A(cpu->l);
    }
    break;
    case 0x86000000:
    {
        add_A(memory_get(cpu->hl));
    }
    break;
    case 0x87000000:
    {
        add_A(cpu->a);
    }
    break;
    case 0x88000000:
    {
        add_A(cpu->b + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x89000000:
    {
        add_A(cpu->c + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x8A000000:
    {
        add_A(cpu->d + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x8B000000:
    {
        add_A(cpu->e + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x8C000000:
    {
        add_A(cpu->h + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x8D000000:
    {
        add_A(cpu->l + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x8E000000:
    {
        add_A(memory_get(cpu->hl) + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x8F000000:
    {
        add_A(cpu->a + isFlagSet(FLAGS_CARRY));
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

static int cpuExecute9(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0x9000000:
    {
        sub_A(cpu->b);
    }
    break;
    case 0x9100000:
    {
        sub_A(cpu->c);
    }
    break;
    case 0x9200000:
    {
        sub_A(cpu->d);
    }
    break;
    case 0x9300000:
    {
        sub_A(cpu->e);
    }
    break;
    case 0x9400000:
    {
        sub_A(cpu->h);
    }
    break;
    case 0x9500000:
    {
        sub_A(cpu->l);
    }
    break;
    case 0x9600000:
    {
        sub_A(memory_get(cpu->hl));
    }
    break;
    case 0x9700000:
    {
        sub_A(cpu->a);
    }
    break;
    case 0x9800000:
    {
        sub_A(cpu->b + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9900000:
    {
        sub_A(cpu->c + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9A00000:
    {
        sub_A(cpu->d + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9B00000:
    {
        sub_A(cpu->e + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9C00000:
    {
        sub_A(cpu->h + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9D00000:
    {
        sub_A(cpu->l + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9E00000:
    {
        sub_A(memory_get(cpu->hl) + isFlagSet(FLAGS_CARRY));
    }
    break;
    case 0x9F00000:
    {
        sub_A(cpu->a + isFlagSet(FLAGS_CARRY));
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

static int cpuExecuteA(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0xA0000000:
    {
        AND(cpu->b);
    }
    break;

    case 0xA1000000:
    {
        AND(cpu->c);
    }
    break;
    case 0xA2000000:
    {
        AND(cpu->d);
    }
    break;
    case 0xA3000000:
    {
        AND(cpu->e);
    }
    break;
    case 0xA4000000:
    {
        AND(cpu->h);
    }
    break;
    case 0xA5000000:
    {
        AND(cpu->l);
    }
    break;
    case 0xA6000000:
    {
        AND(memory_get(cpu->hl));
    }
    break;
    case 0xA7000000:
    {
        AND(cpu->a);
    }
    break;
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
    case 0xB0000000:
    {
        OR(cpu->b);
    }
    break;
    case 0xB1000000:
    {
        OR(cpu->c);
    }
    break;
    case 0xB2000000:
    {
        OR(cpu->d);
    }
    break;
    case 0xB3000000:
    {
        OR(cpu->e);
    }
    break;
    case 0xB4000000:
    {
        OR(cpu->h);
    }
    break;
    case 0xB5000000:
    {
        OR(cpu->l);
    }
    break;
    case 0xB6000000:
    {
        OR(memory_get(cpu->hl));
    }
    break;
    case 0xB7000000:
    {
        OR(cpu->b);
    }
    break;
    case 0xB8000000:
    {
        cp(cpu->b);
    }
    break;
    case 0xB9000000:
    {
        cp(cpu->c);
    }
    break;

    case 0xBA000000:
    {
        cp(cpu->d);
    }
    break;
    case 0xBB000000:
    {
        cp(cpu->e);
    }
    break;
    case 0xBC000000:
    {
        cp(cpu->h);
    }
    break;
    case 0xBD000000:
    {
        cp(cpu->l);
    }
    break;
    case 0xBE000000:
    {
        cp(memory_get(cpu->hl));
    }
    break;
    case 0xBF000000:
    {
        cp(cpu->a);
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

static int cpuExecuteC(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0xC0000000:
    {
        if (!isFlagSet(FLAGS_ZERO))
        {
            cpu->pc = pop();
        }
    }
    break;
    case 0xC1000000:
    {
        ld16BitToRegister(pop(), &cpu->bc);
    }
    break;
    case 0xC2000000:
    {
        if (!isFlagSet(FLAGS_ZERO))
        {
            cpu->pc = getBigEndianValue(get16BitFromOpcode(opcode));
        }
    }
    break;
    case 0xC3000000:
    {
        cpu->pc = getBigEndianValue(get16BitFromOpcode(opcode));
    }
    break;
    case 0xC4000000:
    {
        if (!isFlagSet(FLAGS_ZERO))
        {
            push(cpu->pc);
            cpu->pc = getBigEndianValue(get16BitFromOpcode(opcode));
        }
    }
    break;
    case 0xC5000000:
    {
        push(cpu->bc);
    }
    break;
    case 0xC8000000:
    {
        if (isFlagSet(FLAGS_ZERO))
        {
            cpu->pc = pop();
        }
    }
    break;
    case 0xC6000000:
    {
        add_A((opcode & 0x00ff0000) >> 16);
    }
    break;
    case 0xC9000000:
    {
        cpu->pc = pop();
    }
    break;
    case 0xCA000000:
    {
        if (isFlagSet(FLAGS_ZERO))
        {
            cpu->pc = getBigEndianValue(get16BitFromOpcode(opcode));
        }
    }
    break;
    case 0xCB000000:
    {

        cpuExecuteCB(cpu, opcode);
    }
    break;
    case 0xCC000000:
    {
        if (isFlagSet(FLAGS_ZERO))
        {
            push(cpu->pc);
            cpu->pc = getBigEndianValue(get16BitFromOpcode(opcode));
        }
    }
    break;
    case 0xCD000000:
    {
        push(cpu->pc);
        cpu->pc = getBigEndianValue(get16BitFromOpcode(opcode));
    }
    break;
    case 0xCE000000:
    {
        add_A(((opcode & 0x00ff0000) >> 16) + isFlagSet(FLAGS_CARRY));
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
    case 0xD0000000:
    {
        if (!isFlagSet(FLAGS_CARRY))
        {
            cpu->pc = pop();
        }
    }
    break;
    case 0xD1000000:
    {
        ld16BitToRegister(pop(), &cpu->de);
    }
    break;
    case 0xD2000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        if (!isFlagSet(FLAGS_CARRY))
        {
            cpu->pc = getBigEndianValue(imm);
        }
    }
    break;
    case 0xD4000000:
    {
        if (!isFlagSet(FLAGS_CARRY))
        {
            push(cpu->pc);
            uint16_t imm = (opcode & 0x00ffff00) >> 8;
            cpu->pc = getBigEndianValue(imm);
        }
    }
    break;
    case 0xD5000000:
    {
        push(cpu->de);
    }
    break;
    case 0xD600000:
    {
        sub_A((opcode & 0x00ff0000) >> 16);
    }
    break;
    case 0xD8000000:
    {
        if (isFlagSet(FLAGS_CARRY))
        {
            cpu->pc = pop();
        }
    }
    break;
    case 0xDA000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        if (isFlagSet(FLAGS_CARRY))
        {
            cpu->pc = getBigEndianValue(imm);
        }
    }
    break;
    case 0xDC000000:
    {
        if (isFlagSet(FLAGS_CARRY))
        {
            push(cpu->pc);
            uint16_t imm = (opcode & 0x00ffff00) >> 8;
            cpu->pc = getBigEndianValue(imm);
        }
    }
    break;
    case 0xDE00000:
    {
        sub_A(((opcode & 0x00ff0000) >> 16) + isFlagSet(FLAGS_CARRY));
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

static int cpuExecuteE(uint32_t opcode)
{
    int cycles = 0;
    int instructionCode = opcode & 0xff000000;

    switch (instructionCode)
    {
    case 0xE0000000:
    {
        uint8_t imm = (opcode & 0x00ff0000) >> 16;
        printf("putting value %0x from register A at address %0x. (0xFF00 + %0x)\n",
               cpu->a, 0xff00 + imm, imm);
        memory_set(0xFF00 + imm, cpu->a);
    }
    break;
    case 0xE1000000:
    {
        ld16BitToRegister(pop(), &cpu->hl);
    }
    break;
    case 0xE2000000:
    {
        printf("putting value %0x from register A at address %0x. (FF + cpu->c)\n",
               cpu->a, 0xff00 + cpu->c);
        memory_set(0xFF00 + cpu->c, cpu->a);
    }
    break;
    case 0xE5000000:
    {
        push(cpu->hl);
    }
    break;

    case 0xE6000000:
    {
        AND((opcode & 0x00ff0000) >> 16);
    }
    break;
    case 0xE9000000:
    {
        cpu->pc = cpu->hl;
    }
    break;
    case 0xEA000000:
    {
        memory_set(getBigEndianValue((opcode & 0x00ffff00) >> 8), cpu->a);
    }
    break;
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
    case 0xF0000000:
    {
        uint8_t imm = (opcode & 0x00ff0000) >> 16;
        ld8BitToRegister(memory_get(0xFF00 + imm), &cpu->a);
    }
    break;
    case 0xF1000000:
    {

        ld16BitToRegister(pop(), &cpu->af);
    }
    break;
    case 0xF2000000:
    {
        ld8BitToRegister(memory_get(0xFF00 + cpu->c), &cpu->a);
    }
    break;
    case 0xF5000000:
    {
        push(cpu->af);
    }
    break;
    case 0xF8000000:
    {
        signed char operand = (opcode & 0x00ff0000) >> 16;
        if ((cpu->sp & 0x0f) + (operand & 0x0f) > 0x0f)
        {
            setFlag(FLAGS_HALFCARRY);
        }
        else
        {
            resetFlag(FLAGS_HALFCARRY);
        }

        uint32_t result = cpu->sp + operand;
        if (result & 0xffff0000)
        {
            setFlag(FLAGS_CARRY);
        }
        else
        {
            resetFlag(FLAGS_CARRY);
        }

        resetFlag(FLAGS_ZERO | FLAGS_NEGATIVE);

        ld16BitToRegister(result & 0xffff, &cpu->hl);
    }
    break;
    case 0xF9000000:
    {
        ld16BitToRegister(cpu->hl, &cpu->sp);
    }
    break;
    case 0xFA000000:
    {
        uint16_t imm = (opcode & 0x00ffff00) >> 8;
        ld8BitToRegister(memory_get(getBigEndianValue(imm)), &cpu->a);
    }
    break;
    case 0xFE000000:
    {
        cp((opcode & 0x00ff0000) >> 16);
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
    cpu->f &= ~flag;
}

void clearFlags()
{
    cpu->f = 0;
}

_Bool isFlagSet(uint8_t flag)
{
    return cpu->f & flag ? 1 : 0;
}
static void ld16BitToRegister(uint16_t value, uint16_t *reg)
{
    *reg = value;
}

static void ld8BitToRegister(uint8_t value, uint8_t *reg)
{
    *reg = value;
}
static void add_A(uint8_t value)
{
    if ((cpu->a & 0x0f) + (value & 0x0f) > 0x0f)
    {
        setFlag(FLAGS_HALFCARRY);
    }
    else
    {
        resetFlag(FLAGS_HALFCARRY);
    }
    if (cpu->a + value > 0xff)
    {
        setFlag(FLAGS_CARRY);
    }
    else
    {
        resetFlag(FLAGS_CARRY);
    }
    cpu->a += value;
    if (cpu->a == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }
    resetFlag(FLAGS_NEGATIVE);
}
static void sub_A(uint8_t value)
{
    if ((cpu->a & 0x0f) - (value & 0x0f) > 0)
    {
        resetFlag(FLAGS_HALFCARRY);
    }
    else
    {
        setFlag(FLAGS_HALFCARRY);
    }
    if (value > cpu->a)
    {
        setFlag(FLAGS_CARRY);
    }
    else
    {
        resetFlag(FLAGS_CARRY);
    }
    cpu->a -= value;
    if (cpu->a == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }
    setFlag(FLAGS_NEGATIVE);
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
static void AND(uint8_t value)
{
    cpu->a = cpu->a & value;
    if (cpu->a == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }

    setFlag(FLAGS_HALFCARRY);
    resetFlag(FLAGS_NEGATIVE | FLAGS_CARRY);
}
static void OR(uint8_t value)
{
    cpu->a = cpu->a | value;
    if (cpu->a == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }

    resetFlag(FLAGS_NEGATIVE | FLAGS_CARRY | FLAGS_HALFCARRY);
}
static void push(uint16_t value)
{
    memory_set(cpu->sp, (value & 0xff00) >> 8);
    memory_set(cpu->sp - 1, value & 0xff);
    cpu->sp -= 2;
}
static uint16_t pop()
{
    uint8_t low = memory_get(cpu->sp + 1);
    uint8_t high = memory_get(cpu->sp + 2);

    uint16_t result = (high << 8) | low;
    cpu->sp += 2;
    return result;
}
static void inc(uint8_t *reg)
{
    if ((*reg & 0x0f) + 1 > 0x0f)
    {
        setFlag(FLAGS_HALFCARRY);
    }
    else
    {
        resetFlag(FLAGS_HALFCARRY);
    }
    *reg += 1;
    if (*reg == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }
    resetFlag(FLAGS_NEGATIVE);
}
static void dec(uint8_t *reg)
{
    if ((*reg & 0x0f))
    {
        resetFlag(FLAGS_HALFCARRY);
    }
    else
    {
        setFlag(FLAGS_HALFCARRY);
    }
    *reg -= 1;
    if (*reg == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }
    setFlag(FLAGS_NEGATIVE);
}
static void cp(uint8_t value)
{
    if (cpu->a == value)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }
    if (cpu->a < value)
    {
        setFlag(FLAGS_CARRY);
    }
    else
    {
        resetFlag(FLAGS_CARRY);
    }
    if ((cpu->a & 0x0f) < (value & 0x0f))
    {
        setFlag(FLAGS_HALFCARRY);
    }
    else
    {
        resetFlag(FLAGS_HALFCARRY);
    }
    setFlag(FLAGS_NEGATIVE);
}

static uint16_t get16BitFromOpcode(uint32_t opcode)
{
    return (opcode & 0x00ffff00) >> 8;
}