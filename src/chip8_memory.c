#include "chip8_memory.h"

static void chip8_load_rom(
    Memory* mem, 
    const char* rom_name
){
    assert(rom_name != NULL);
    assert(mem != NULL);
    
    FILE* rom = NULL; 
    if ((rom = fopen(rom_name, "rb")) == NULL){
        fprintf(stderr, "ERROR: Unable to open the ROM named %s\n", rom_name);
        exit(1);
    }

    fseek(rom, 0, SEEK_END);
    mem->rom_size = ftell(rom);

    if (mem->rom_size > MEM_MAX_SIZE - PC_INITIAL_OFFSET){
        fprintf(stderr, "ERROR:The ROM has a size of %i, but should be less than or equal to %i\n", MEM_MAX_SIZE - PC_INITIAL_OFFSET);
        fclose(rom);
        exit(1);
    }

    fseek(rom, 0, SEEK_SET);
    fread(
        &mem->mem[PC_INITIAL_OFFSET],  
        mem->rom_size, 
        sizeof(byte),
        rom
    );
    
    fclose(rom);
}

Memory chip8_memory_construct(
    const char* rom_name
){
    assert(rom_name != NULL);

    const byte fontset[] = {
        0xF0u, 0x90u, 0x90u, 0x90u, 0xF0u, // 0
        0x20u, 0x60u, 0x20u, 0x20u, 0x70u, // 1
        0xF0u, 0x10u, 0xF0u, 0x80u, 0xF0u, // 2
        0xF0u, 0x10u, 0xF0u, 0x10u, 0xF0u, // 3
        0x90u, 0x90u, 0xF0u, 0x10u, 0x10u, // 4
        0xF0u, 0x80u, 0xF0u, 0x10u, 0xF0u, // 5
        0xF0u, 0x80u, 0xF0u, 0x90u, 0xF0u, // 6
        0xF0u, 0x10u, 0x20u, 0x40u, 0x40u, // 7
        0xF0u, 0x90u, 0xF0u, 0x90u, 0xF0u, // 8
        0xF0u, 0x90u, 0xF0u, 0x10u, 0xF0u, // 9
        0xF0u, 0x90u, 0xF0u, 0x90u, 0x90u, // A
        0xE0u, 0x90u, 0xE0u, 0x90u, 0xE0u, // B
        0xF0u, 0x80u, 0x80u, 0x80u, 0xF0u, // C
        0xE0u, 0x90u, 0x90u, 0x90u, 0xE0u, // D
        0xF0u, 0x80u, 0xF0u, 0x80u, 0xF0u, // E
        0xF0u, 0x80u, 0xF0u, 0x80u, 0x80u  // F
    };

    Memory mem;

    for(int i = FONTSET_INITIAL_OFFSET; i < sizeof(fontset); i++)
        mem.mem[i] = fontset[i - FONTSET_INITIAL_OFFSET];

    chip8_load_rom(&mem, rom_name);

    
    return mem;
}


