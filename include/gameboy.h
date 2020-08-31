#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdint.h>
#include "cpu.h"
#include "memory.h"
#include "config.h"

struct s_gameboy
{
    uint8_t cartridge[CARTRIDGE_SIZE];
    struct s_cpu cpu;
    struct s_memory memory;
};

struct s_gameboy* intializeSystem();

_Bool readCartridge(const char* filename);

void loadGameIntoMemory();

void executeNextInstruction();


#endif