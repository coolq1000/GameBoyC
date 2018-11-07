
#include <stdlib.h>
#include <stdio.h>
#include "mmu.h"

/* TODO: Rewrite read, write, ripped from: https://github.com/sacert/Gameboy/blob/master/MMU.c */

unsigned char readByte(struct MMU *mmu, unsigned short addr)
{
    if (0x0000 <= addr && addr <= 0x7FFF) 
        return mmu->full[addr];
    else if (0x8000 <= addr && addr <= 0x9FFF) 
        return mmu->vram[addr - 0x8000];
    else if (0xA000 <= addr && addr <= 0xBFFF)
        return mmu->sram[addr - 0xA000];
    else if (0xC000 <= addr && addr <= 0xDFFF)
        return mmu->wram[addr - 0xC000];
    else if (0xE000 <= addr && addr <= 0xFDFF)
        return mmu->wram[addr - 0xE000];
    else if (0xFE00 <= addr && addr <= 0xFEFF) 
        return mmu->oam[addr - 0xFE00];
    else if (addr == 0xFF00) {
        return 0; // FIXME.
    }
    else if (addr == 0xFF04)
        return 0; // return getDiv(); FIXME (TIMERS TODO).
    else if (addr == 0xFF05)
        return 0; // return getTima(); FIXME (TIMERS TODO).
    else if (addr == 0xFF06)
        return 0; // return getTma(); FIXME (TIMERS TODO).
    else if (addr == 0xFF04)
        return 0; // return getTac(); FIXME (TIMERS TODO).
    else if (addr == 0xFF0F)
        return 0; // FIXME.
    else if (addr == 0xFF40)
        return 0; // return getLCDC(); FIXME (TIMERS TODO).
    else if (addr == 0xFF41)
        return 0; // return getLCDS(); FIXME (TIMERS TODO).
    else if (addr == 0xFF42)
        return 0; // return getScrollY(); FIXME (TIMERS TODO).
    else if (addr == 0xFF43)
        return 0; // return getScrollX(); FIXME (TIMERS TODO).
    else if (addr == 0xFF44)
        return 0; // return getLine(); FIXME (TIMERS TODO).
    else if(0xFF00 <= addr && addr <= 0xFF7F)
    	return mmu->io[addr - 0xFF00];
    else if (0xFF80 <= addr && addr <= 0xFFFE) 
        return mmu->hram[addr - 0xFF80];
    else if (addr == 0xFFFF)
        return 0; // FIXME.

    return 0;
}

unsigned short readWord(struct MMU *mmu, unsigned short addr)
{
    return (readByte(mmu, addr + 1) << 8) | readByte(mmu, addr);
}

void writeByte(struct MMU *mmu, unsigned short addr, unsigned char data)
{
    if (0x8000 <= addr && addr <= 0x9FFF)
        mmu->vram[addr - 0x8000] = data;
    else if (0xA000 <= addr && addr <= 0xBFFF)
        mmu->sram[addr - 0xA000] = data;
    else if (0xC000 <= addr && addr <= 0xDFFF)
        mmu->wram[addr - 0xC000] = data;
    else if (0xE000 <= addr && addr <= 0xFDFF)
        mmu->wram[addr - 0xE000] = data;
    else if (0xFE00 <= addr && addr <= 0xFEFF)  
        mmu->oam[addr - 0xFE00] = data; 
    else if (addr == 0xFF04) 
        ;// setDiv(data);
    else if (addr == 0xFF05) 
        ;// setTima(data);
    else if (addr == 0xFF06) 
        ;// setTma(data);
    else if (addr == 0xFF04) 
        ;// setTac(data);
    else if (addr == 0xFF40) 
        ;// setLCDC(data);
    else if (addr == 0xFF41) 
        ;// setLCDS(data);
    else if (addr == 0xFF42) 
        ;// setScrollY(data);
    else if (addr == 0xFF43) 
        ;// setScrollX(data);
    else if (addr == 0xFF45) 
        ;// setLyCompare(data);
    else if(addr == 0xff46) 
        ;// copyToOAM(0xfe00, data << 8, 160); // OAM DMA
    else if (addr == 0xFF47) 
        ;// setBGPalette(data);
    else if (addr == 0xFF48) 
        ;// setSpritePalette1(data);
    else if (addr == 0xFF49) 
        ;// setSpritePalette2(data);
    else if (addr == 0xFF4A) 
        ;// setWindowY(data);
    else if (addr == 0xFF4B) 
        ;// setWindowX(data);
    else if (addr == 0xFF00) {
        // FIXME.
    }
    else if(0xFF00 <= addr && addr <= 0xFF7F) 
    	mmu->io[addr - 0xFF00] = data;
    else if (0xFF80 <= addr && addr <= 0xFFFE) 
        mmu->hram[addr - 0xFF80] = data;
    else if (addr == 0xFF0F) 
        ; // FIXME.
    else if (addr == 0xFFFF) 
        ; // FIXME.
}

void writeWord(struct MMU *mmu, unsigned short addr, unsigned short data)
{
    writeByte(mmu, addr, data & 0x00FF);
    writeByte(mmu, addr + 1, (data & 0xFF00) >> 8);
}

void loadROM(struct MMU *mmu, char *path)
{
    /* Open file, */
    FILE *file = fopen(path, "rb");

    /* Get file size, */
    fseek(file, 0, SEEK_END);
    unsigned long size = ftell(file);

    /* Allocate buffer, */
    unsigned char *buffer = (unsigned char *)malloc(size * sizeof(unsigned char));

    /* Read bytes into buffer, */
    fseek(file, 0, SEEK_SET);
    fread(buffer, size, 1, file);

    /* Copy into ROM, */
    for (int i = 0; i < size; i++)
    {
        /* We check to see if it's inside the CART ROM or NEXT ROM, */
        if (i < CART_SIZE)
        {
            mmu->cart[i] = buffer[i];
        }
        else
        {
            mmu->next[i - CART_SIZE] = buffer[i];
        }
    }
}
