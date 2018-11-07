
#ifndef CPUH
#define CPUH

#include "mmu.h"

#define NEXT_WORD readWord(cpu->mmu, cpu->registers.pc + 1)
#define NEXT_BYTE readByte(cpu->mmu, cpu->registers.pc + 1)

#define SET_Z(x) cpu->registers.f = ((cpu->registers.f & 0x7F) | (x << 7))
#define SET_N(x) cpu->registers.f = ((cpu->registers.f & 0xBF) | (x << 6))
#define SET_H(x) cpu->registers.f = ((cpu->registers.f & 0xDF) | (x << 5))
#define SET_C(x) cpu->registers.f = ((cpu->registers.f & 0xEF) | (x << 4))

#define FLAG_Z ((cpu->registers.f >> 7) & 0x1)
#define FLAG_N ((cpu->registers.f >> 6) & 0x1)
#define FLAG_H ((cpu->registers.f >> 5) & 0x1)
#define FLAG_C ((cpu->registers.f >> 4) & 0x1)

struct CPU
{
    struct
    {
        union
        {
            unsigned short af;
            struct
            {
                unsigned char f;
                unsigned char a;
            };
        };
        union
        {
            unsigned short bc;
            struct
            {
                unsigned char c;
                unsigned char b;
            };
        };
        union
        {
            unsigned short de;
            struct
            {
                unsigned char e;
                unsigned char d;
            };
        };
        union
        {
            unsigned short hl;
            struct
            {
                unsigned char l;
                unsigned char h;
            };
        };
        unsigned short pc, sp;
    } registers;

    struct MMU *mmu;
};

void initCPU(struct CPU *cpu);
void cycleCPU(struct CPU *cpu);

#endif
