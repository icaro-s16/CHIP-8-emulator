#pragma once

#include <SDL2/SDL.h>
#include "chip8_core.h"



int chip8_update_keys_map(Chip8VM* vm);

void chip8_update_display(Chip8VM* vm);

void chip8_construct_display();

void chip8_destroy_display();

