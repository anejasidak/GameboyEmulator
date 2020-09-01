#include <assert.h>
#include "memory.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "config.h"

static struct s_memory *memory;

void intializeMemory(struct s_memory *mem)
{
    memory = mem;
}
static void memory_is_in_bound(int index)
{
    assert(index >= 0 && index < MEMORY_SIZE);
}
void memory_set(uint16_t index, uint8_t value)
{
    // Need to implement banking

    memory_is_in_bound(index);
    if (index <= ROM_BANK_0_END)
    {
    }
    else if (index <= ROM_BANK_1_END)
    {
    }
    else if (index >= ECHO_MEM_START && index <= ECHO_MEM_END)
    {
        memory->memoryArray[index] = value;
        memory_set(WRAM_BANK_0_START + index - ECHO_MEM_START, value);
    }
    else if (index >= RESTRICTED_RAM_START && index <= RESTRICTED_RAM_END)
    {
    }
    else
    {
        memory->memoryArray[index] = value;
    }
}
uint8_t memory_get(uint16_t index)
{
    // Need to implement banking
    memory_is_in_bound(index);
    return memory->memoryArray[index];
}

uint32_t memory_get_ins(int index)
{
    uint32_t opcode = 0;
    for (int i = index; i < MEMORY_SIZE && i - index < 4; i++)
    {
        opcode = opcode << 8;
        opcode |= memory->memoryArray[i];
    }
    return opcode;
}
