#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

struct s_cpu
{
    struct
    {
        union {
            struct
            {
                uint8_t f;
                uint8_t a;
            };
            uint16_t af;
        };
    };

    struct
    {
        union {
            struct
            {
                uint8_t c;
                uint8_t b;
            };
            uint16_t bc;
        };
    };

    struct
    {
        union {
            struct
            {
                uint8_t e;
                uint8_t d;
            };
            uint16_t de;
        };
    };

    struct
    {
        union {
            struct
            {
                uint8_t l;
                uint8_t h;
            };
            uint16_t hl;
        };
    };

    uint16_t sp;
    uint16_t pc;
};

int cpuExecuteInstruction(uint16_t opcode);

#endif