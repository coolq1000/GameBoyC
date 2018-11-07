
#ifndef MMUH
#define MMUH

#define CART_SIZE 0x4000 // ROM, Cart 1.
#define NEXT_SIZE 0x4000 // ROM, Cart 2.
#define VRAM_SIZE 0x2000 // Video RAM.
#define SRAM_SIZE 0x2000 // Switchable RAM.
#define WRAM_SIZE 0x2000 // Work RAM.
#define OAM_SIZE  0x100  // Sprite Attribute Memory.
#define IO_SIZE   0x100  // I/O RAM.
#define HRAM_SIZE 0x80   // High RAM.

struct MMU
{
    union
    {
        unsigned char full[CART_SIZE + NEXT_SIZE];
        struct
        {
            unsigned char cart[CART_SIZE];
            unsigned char next[NEXT_SIZE];
        };
    };
    unsigned char vram[VRAM_SIZE];
    unsigned char sram[SRAM_SIZE];
    unsigned char wram[WRAM_SIZE];
    unsigned char oam [OAM_SIZE ];
    unsigned char io  [IO_SIZE  ];
    unsigned char hram[HRAM_SIZE];

};

unsigned char  readByte(struct MMU *mmu, unsigned short addr);
unsigned short readWord(struct MMU *mmu, unsigned short addr);

void writeByte(struct MMU *mmu, unsigned short addr, unsigned char  data);
void writeWord(struct MMU *mmu, unsigned short addr, unsigned short data);

void loadROM(struct MMU *mmu, char *path);

#endif
