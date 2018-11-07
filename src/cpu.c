
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

void initCPU(struct CPU *cpu)
{
    /* Reset registers, */
    cpu->registers.af = 0x0080;
    cpu->registers.bc = 0x0000;
    cpu->registers.de = 0x00D8;
    cpu->registers.hl = 0xFFE2;

    cpu->registers.pc = 0x0200;
    cpu->registers.sp = 0xCFF5;
    
    /* Allocate memory for MMU, */
    cpu->mmu = (struct MMU *)malloc(sizeof(struct MMU));
}

void cycleCPU(struct CPU *cpu)
{
    unsigned char instruction = readByte(cpu->mmu, cpu->registers.pc);

    /* Debug, */
#ifdef CPU_DEBUG
    printf("%04X: Executing %02X\n", cpu->registers.pc, instruction);
    printf("Next BYTE: %02X | Next WORD: %04X\n",
        readByte(cpu->mmu, cpu->registers.pc + 1),
        readWord(cpu->mmu, cpu->registers.pc + 1)
    );
    printf("AF: %04X\nBC: %04X\nDE: %04X\nHL: %04X\nSP: %04X\nPC: %04X\n",
        cpu->registers.af, cpu->registers.bc,
        cpu->registers.de, cpu->registers.hl,
        cpu->registers.sp, cpu->registers.pc
    );
#endif

    unsigned short temp;

    switch (instruction)
    {
        case 0x00:
            cpu->registers.pc += 1;
            exit(EXIT_SUCCESS);
            break;
        case 0x0E:
            cpu->registers.c = readByte(cpu->mmu, cpu->registers.pc + 1);
            cpu->registers.pc += 2;
            break;
        case 0x11:
            cpu->registers.de = readWord(cpu->mmu, cpu->registers.pc + 1);
            cpu->registers.pc += 3;
            break;
        case 0x1A:
            cpu->registers.a = readByte(cpu->mmu, cpu->registers.de);
            cpu->registers.pc += 1;
            break;
        case 0x20:
            if (FLAG_Z == 0)
            {
                cpu->registers.pc += (signed char)readByte(cpu->mmu, cpu->registers.pc + 1) + 2;
            }
            else
            {
                cpu->registers.pc += 2;
            }
            break;
        case 0x21:
            cpu->registers.hl = NEXT_WORD;
            cpu->registers.pc += 3;
            break;
        case 0x31:
            cpu->registers.sp = NEXT_WORD;
            cpu->registers.pc += 3;
            break;
        case 0x32:
            temp = cpu->registers.hl;
            writeByte(cpu->mmu, temp, cpu->registers.a);
            cpu->registers.hl = temp - 1;
            cpu->registers.pc += 1;
            break;
        case 0x3C:
            cpu->registers.a += 1;
            SET_Z(!cpu->registers.a);
            SET_N(0);
            SET_H(((cpu->registers.a & 0xF) < ((cpu->registers.a - 1) & 0xF)));
            cpu->registers.pc += 1;
            break;
        case 0x3E:
            cpu->registers.a = readByte(cpu->mmu, cpu->registers.pc + 1);
            cpu->registers.pc += 2;
            break;
        case 0x77:
            writeByte(cpu->mmu, cpu->registers.hl, cpu->registers.a);
            cpu->registers.pc += 1;
            break;
        case 0xA1:
            cpu->registers.a &= cpu->registers.c;
            SET_Z(!cpu->registers.a);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            cpu->registers.pc += 1;
            break;
        case 0xAF:
            cpu->registers.a ^= cpu->registers.a;
            SET_Z(!cpu->registers.a);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            cpu->registers.pc += 1;
            break;
        case 0xC1:
            cpu->registers.bc = readWord(cpu->mmu, cpu->registers.sp);
            cpu->registers.sp += 2;
            cpu->registers.pc += 1;
            break;
        case 0xC3:
            cpu->registers.pc = NEXT_WORD;
            break;
        case 0xCB:
        {
            unsigned char CBInstruction = readByte(cpu->mmu, cpu->registers.pc + 1);
            switch (CBInstruction)
            {
                case 0x7C:
                    SET_Z(!(cpu->registers.h & 0x80));
                    SET_N(0);
                    SET_H(0);
                    break;
                default:
                    printf("ERROR: Unimplemented CB instruction 0x%X!\n", CBInstruction);
                    exit(EXIT_FAILURE);
            }
            cpu->registers.pc += 2;
            break;
        }
        case 0xCD:
            cpu->registers.pc -= 2;
            writeWord(cpu->mmu, cpu->registers.sp, cpu->registers.pc + 3);
            cpu->registers.pc = readWord(cpu->mmu, cpu->registers.pc + 1);
            break;
        case 0xD1:
            cpu->registers.de = readWord(cpu->mmu, cpu->registers.sp);
            cpu->registers.sp += 2;
            cpu->registers.pc += 1;
            break;
        case 0xE0:
            writeByte(cpu->mmu, 0xFF00 + readByte(cpu->mmu, cpu->registers.pc + 1), cpu->registers.a);
            cpu->registers.pc += 2;
            break;
        case 0xE1:
        {
            printf("%X : %X\n", cpu->registers.sp, readWord(cpu->mmu, cpu->registers.sp));
            cpu->registers.hl = readWord(cpu->mmu, cpu->registers.sp);
            cpu->registers.sp += 2;
            cpu->registers.pc += 1;
            break;
        }
        case 0xE2:
            writeByte(cpu->mmu, 0xFF00 + cpu->registers.c, cpu->registers.a);
            cpu->registers.pc += 2;
            break;
        case 0xF1:
            cpu->registers.af = readWord(cpu->mmu, cpu->registers.sp) & 0xFFF0;
            cpu->registers.sp += 2;
            cpu->registers.pc += 1;
            break;
        case 0xD9:
            cpu->registers.pc = readWord(cpu->mmu, cpu->registers.sp);
            cpu->registers.sp += 2;
            break;
        default:
            printf("ERROR: Unimplemented instruction 0x%X!\n", instruction);
            exit(EXIT_FAILURE);
        // TODO: Implement instructions.
    }
}
