#pragma once

#include "raylib.h"
#include "chip8_core.h"



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

