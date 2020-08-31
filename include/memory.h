#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>
#include "config.h"

struct s_memory
{
    uint8_t memoryArray[MEMORY_SIZE];
};

void intializeMemory(struct s_memory *mem);
#endif