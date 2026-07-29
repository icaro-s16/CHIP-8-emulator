#pragma once

#include "raylib.h"
#include "chip8_core.h"

#define CHIP8_KEYS_TABLE(X)  \
    X(KEY_ONE, 0x0u)         \
    X(KEY_TWO, 0x1u)         \
    X(KEY_THREE, 0x2u)       \
    X(KEY_FOUR, 0x3u)        \
    X(KEY_Q, 0x4u)           \
    X(KEY_W, 0x5u)           \
    X(KEY_E, 0x6u)           \
    X(KEY_R, 0x7u)           \
    X(KEY_A, 0x8u)           \
    X(KEY_S, 0x9u)           \
    X(KEY_D, 0xAu)           \
    X(KEY_F, 0xBu)           \
    X(KEY_Z, 0xCu)           \
    X(KEY_X, 0xDu)           \
    X(KEY_C, 0xEu)           \
    X(KEY_V, 0xFu)

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

int chip8_is_key_down(Chip8VM* vm);

void chip8_update_key_map_down(Chip8VM* vm);

void chip8_update_key_map_up(Chip8VM* vm);

void chip8_update_display(Chip8VM* vm);

void chip8_construct_display();

void chip8_destroy_display();
