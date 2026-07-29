#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


enum {
    BUFFER_MAX_SIZE         = 50,
    CHIP8_DISPLAY_WIDTH     = 64,
    CHIP8_DISPLAY_HEIGHT    = 32,
    KEYS_SIZE               = 16,
    MEM_MAX_SIZE            = 4096,
    V_REG_SIZE              = 16,
    PC_INITIAL_OFFSET       = 512,
    FONTSET_INITIAL_OFFSET  = 80
};

typedef uint8_t byte;

typedef uint16_t addr;

typedef struct Memory Memory;
struct Memory{
    byte mem[MEM_MAX_SIZE];
    size_t rom_size;
};

static void load_rom(Memory* mem, const char* rom_name);

Memory chip8_memory_construct(const char* rom_name);

