
#include <stdio.h>
#include "cpu.h"

/* 
 * TODO list,
 * readByte(): MMU.c
 * Timers
 */

int main()
{
    struct CPU cpu;
    initCPU(&cpu);

    loadROM(cpu.mmu, "ROMs/tetris.gb");

    for (;;)
    {
        cycleCPU(&cpu);
        getchar();
    }

    return 0;
}
