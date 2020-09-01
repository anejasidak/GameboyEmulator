#include "cb.h"
#include "config.h"
#include <assert.h>
#include <stdio.h>

static void bit(uint8_t b, uint8_t registerValue)
{
    if (((1 << b) & registerValue) == 0)
    {
        setFlag(FLAGS_ZERO);
    }
    else
    {
        resetFlag(FLAGS_ZERO);
    }
    resetFlag(FLAGS_NEGATIVE);
    setFlag(FLAGS_HALFCARRY);
}
void cpuExecuteCB(struct s_cpu *cpu, uint32_t opcode)
{

    switch (opcode & 0x00ff0000)
    {
    case 0x007c0000:
    {
        bit(7, cpu->h);
    }
    break;
    default:
    {
        printf("Instruction %0x has not been implemented\n", (opcode & 0xffff0000) >> 16);
        assert(0);
    }
    break;
    }
    cpu->pc += 1;
}