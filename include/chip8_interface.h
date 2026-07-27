#pragma once

#include "raylib.h"
#include "chip8_core.h"

#define CHIP8_KEYS_TABLE(X)  \
    X(KEY_KP_1, 0x0u)        \
    X(KEY_KP_2, 0x1u)        \
    X(KEY_KP_3, 0x2u)        \
    X(KEY_KP_4, 0x3u)        \
    X(KEY_Q, 0x4u)           \
    X(KEY_W, 0x5u)           \
    X(KEY_E, 0x6u)           \
    X(KEY_R, 0x7u)           \
    X(KEY_A, 0x8u)           \
    X(KEY_S, 0x9u)           \
    X(KEY_D, 0x10u)          \
    X(KEY_F, 0x11u)          \
    X(KEY_Z, 0x12u)          \
    X(KEY_X, 0x13u)          \
    X(KEY_C, 0x14u)          \
    X(KEY_V, 0x15u)

static Font font;

static void chip8_draw_hardware_components(
    const Vector2 pos, 
    const char* name, 
    const char* value,
    const int spacing,
    const Color color,
    const int font_size,
    const int font_spacing
);

static void chip8_draw_display_area(const Chip8VM* vm);

static void chip8_draw_hardware_components_area(const Chip8VM* vm);

static void chip8_draw_memory_line(
    const Vector2 pos,
    const char* instruction, 
    const char* memory_address, 
    const int spacing,
    const Color color,
    const int font_size,
    const int font_spacing
);

static void chip8_draw_memory_area(const Chip8VM* vm);

int chip8_is_key_down(Chip8VM* vm);

void chip8_update_key_map(Chip8VM* vm);

void chip8_update_display(Chip8VM* vm);

void chip8_construct_display();

void chip8_destroy_display();