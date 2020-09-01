#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>
#include "config.h"

struct s_memory
{
    uint8_t memoryArray[MEMORY_SIZE];
};

void intializeMemory(struct s_memory *mem);

void memory_set(int index, uint8_t value);

uint8_t memory_get(int index);

uint32_t memory_get_ins(int index);
#endif