#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "chip8_memory.h"

typedef uint8_t byte;

typedef uint16_t addr;

/*

Instruction type | Opcode | Executor | Decoder

*/
#define CHIP8_INSTRUCTION_TABLE(X)                                   \
    X(CLS, 0x00E0u, execute_cls, decode_cls)                         \
    X(RTS, 0x00EEu, execute_rts, decode_rts)                         \
    X(CALL, 0x0FFFu, execute_call, decode_call)                      \
    X(JMP, 0x1FFFu, execute_jmp, decode_jmp)                         \
    X(JSR, 0x2FFFu, execute_jsr, decode_jsr)                         \
    X(SKEQ_RC, 0x3FFFu, execute_skeq_rc, decode_skeq_rc)             \
    X(SKNE_RC, 0x4FFFu, execute_skne_rc, decode_skne_rc)             \
    X(SKEQ_RR, 0x5FF0u, execute_skeq_rr, decode_skeq_rr)             \
    X(MOV_RC, 0x6FFFu, execute_mov_rc, decode_mov_rc)                \
    X(ADD_RC, 0x7FFFu, execute_add_rc, decode_add_rc)                \
    X(MOV_RR, 0x8FF0u, execute_mov_rr, decode_mov_rr)                \
    X(OR_RR, 0x8FF1u, execute_or_rr, decode_or_rr)                   \
    X(AND_RR, 0x8FF2u, execute_and_rr, decode_and_rr)                \
    X(XOR_RR, 0x8FF3u, execute_xor_rr, decode_xor_rr)                \
    X(ADD_RR, 0x8FF4u, execute_add_rr, decode_add_rr)                \
    X(SUB_RR, 0x8FF5u, execute_sub_rr, decode_sub_rr)                \
    X(SHR, 0x8FF6u, execute_shr, decode_shr)                         \
    X(RSB, 0x8FF7u, execute_rsb, decode_rsb)                         \
    X(SHL, 0x8FFEu, execute_shl, decode_shl)                         \
    X(SKNE_RR, 0x9FF0u, execute_skne_rr, decode_skne_rr)             \
    X(MVI, 0xAFFFu, execute_mvi, decode_mvi)                         \
    X(JMI, 0xBFFFu, execute_jmi, decode_jmi)                         \
    X(RAND, 0xCFFFu, execute_rand, decode_rand)                      \
    X(SPRITE, 0xDFFFu, execute_sprite, decode_sprite)                \
    X(SKPR, 0xEF9Eu, execute_skpr, decode_skpr)                      \
    X(SKUP, 0xEFA1u, execute_skup, decode_skup)                      \
    X(GDELAY, 0xFF07u, execute_gdelay, decode_gdelay)                \
    X(KEY, 0xFF0Au, execute_key, decode_key)                         \
    X(SDELAY, 0xFF15u, execute_sdelay, decode_sdelay)                \
    X(SSOUND, 0xFF18u, execute_ssound, decode_ssound)                \
    X(ADDI, 0xFF1Eu, execute_addi, decode_addi)                      \
    X(FONT, 0xFF29u, execute_font, decode_font)                      \
    X(BCD, 0xFF33u, execute_bcd, decode_bcd)                         \
    X(STR, 0xFF55u, execute_str, decode_str)                         \
    X(LDR, 0xFF65u, execute_ldr, decode_ldr)                         

#define X(optype, opcode, executor, decoder) optype, 

typedef enum OpType OpType;
enum OpType{
    CHIP8_INSTRUCTION_TABLE(X) 
    RAW
};

#undef X

typedef struct Chip8VM Chip8VM;
struct Chip8VM{
    char **dissasembly;
    Memory memory;                                              // Done: Change the memory logic
    byte display[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
    addr stack[16];
    byte keys[KEYS_SIZE];
    byte V[V_REG_SIZE];
    addr I;
    addr pc;
    byte sp;
    byte delay_timer;
    byte sound_timer;     
};

typedef struct DecodedOpcode DecodedOpcode;
struct DecodedOpcode{
    char mnemonic[50];
    OpType optype;
    union{

        addr address;       
        
        struct{
            byte vx; 
            byte vy;
            byte constant;      
        };

    };
    void (*execute)(Chip8VM*, const DecodedOpcode*);
};

OpType chip8_get_optype(const addr _opcode);

void chip8_set_executor(DecodedOpcode* decoded_opcode);

void* chip8_get_decoder(DecodedOpcode* decoded_opcode);



// Foward declaration from chip8_executor

void execute_mov_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_bcd(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_or_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_and_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_xor_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_shr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_shl(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_call(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_skeq_rc(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_skne_rc(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_skeq_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_skne_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_mov_rc(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_add_rc(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_sprite(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_cls(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_rts(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_jmp(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_jsr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_jmi(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_skpr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_skup(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_key(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_addi(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_font(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_str(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_ldr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_mvi(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_add_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_sub_rr(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_rsb(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_rand(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_ssound(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_gdelay(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

void execute_sdelay(Chip8VM* vm, const DecodedOpcode* decoded_opcode);

// Foward declaration from chip8_decode.h

void decode_rts(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_cls(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_call(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_jmp(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_jsr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_skeq_rc(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_skne_rc(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_skeq_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_mov_rc(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_add_rc(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_mov_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_or_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_and_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_xor_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_add_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_sub_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_shr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_rsb(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_shl(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_skne_rr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_mvi(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_jmi(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_rand(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_sprite(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_skpr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_skup(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_key(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_sdelay(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_ssound(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_gdelay(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_bcd(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_addi(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_str(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_ldr(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_font(const addr opcode, DecodedOpcode* decoded_opcode);

void decode_raw(const addr opcode, DecodedOpcode* decoded_opcode);