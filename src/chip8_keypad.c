#include "chip8_keypad.h"


static const uint8_t BUF_SIZE = 50;

static int8_t buffer_head = 0;
static int8_t buffer_tail = 0;
static KeyOp key_op_buffer[BUF_SIZE] = {0};

void chip8_buffer_add_key_op(KeyOp key_op){
    
}

KeyOp chip8_buffer_get_key_op(){
    
}

void chip8_consume_buffer(Chip8VM* vm){
    
}