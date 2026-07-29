#pragma once 

#include "chip8_core.h"
#include "raylib.h"

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

#define X(key_name, key_value)  key_name##_UP, key_name##_DOWN,

typedef enum{
    CHIP8_KEYS_TABLE(X)
}KeyOp;

#undef X

int chip8_is_key_down(Chip8VM* vm);

void chip8_buffer_add_key_op(KeyOp key_op);

void chip8_consume_buffer(Chip8VM* vm);