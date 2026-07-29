#pragma once

#include "raylib.h"
#include "chip8_core.h"

#define CHIP8_KEYS_TABLE(X)     \
    X(KEY_ONE, 0x1)             \
    X(KEY_TWO, 0x2)             \
    X(KEY_THREE, 0x3)           \
    X(KEY_FOUR, 0xC)            \
    X(KEY_Q, 0x4)               \
    X(KEY_W, 0x5)               \
    X(KEY_E, 0x6)               \
    X(KEY_R, 0xD)               \
    X(KEY_A, 0x7)               \
    X(KEY_S, 0x8)               \
    X(KEY_D, 0x9)               \
    X(KEY_F, 0xE)               \
    X(KEY_Z, 0xA)               \
    X(KEY_X, 0x0)               \
    X(KEY_C, 0xB)               \
    X(KEY_V, 0xF)


void chip8_update_keys_map(Chip8VM* vm);

int chip8_is_key_down(Chip8VM* vm);

void chip8_play_beep();

static void chip8_draw_hardware_component(
    const Vector2 pos, 
    const char* name, 
    const char* value,
    const Color color
);

static void chip8_draw_display_area(const Chip8VM* vm);

static void chip8_draw_hardware_components_area(const Chip8VM* vm);

static void chip8_draw_memory_line(
    const Vector2 pos,
    const char* instruction, 
    const char* memory_address, 
    const Color color
);

static void chip8_draw_memory_area(const Chip8VM* vm);

void chip8_update_display(Chip8VM* vm);

void chip8_construct_display();

void chip8_destroy_display();

