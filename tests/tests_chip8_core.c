#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <unistd.h>
#include <time.h>
#include "../include/chip8_emulator.h"

void test_chip8_get_optype(){

    #define X(optype, opcode, executor, decoder) opcode, 

    addr opcodes[] = {
        CHIP8_INSTRUCTION_TABLE(X)
    };

    #undef X 

    #define X(optype, opcode, executor, decoder) optype, 

    OpType optypes[] = {
        CHIP8_INSTRUCTION_TABLE(X)
    };

    #undef X 

    for(int i = 0; i < sizeof(opcodes)/sizeof(opcodes[0]); i++)
        opcodes[i] |= ( rand() % 0xFFFF ) & opcodes[i];

    for(int i = 0; i < sizeof(opcodes)/sizeof(opcodes[0]); i++){
        OpType optype = chip8_get_optype(opcodes[i]);

        CU_ASSERT_EQUAL(
            optypes[i],
            optype
        );

    }

}

void test_chip8_get_decoder(){
    #define X(optype, opcode, executor, decoder) opcode, 

    addr opcodes[] = {
        CHIP8_INSTRUCTION_TABLE(X)
    };

    #undef X 


    for(int i = 0; i < sizeof(opcodes)/sizeof(opcodes[0]); i++)
        opcodes[i] |= ( rand() % 0xFFFF ) & opcodes[i];

    #define X(optype, opcode, executor, decoder) (void*)decoder, 
    
    void *decoders[] = {
        CHIP8_INSTRUCTION_TABLE(X)
    };

    #undef X 

    for (int i = 0; i < sizeof(opcodes)/sizeof(opcodes[0]); i++){
        DecodedOpcode decoded_opcode = {0};
        decoded_opcode.optype = chip8_get_optype(opcodes[i]);
        
        void *decoder = chip8_get_decoder(&decoded_opcode); 

        CU_ASSERT_EQUAL(
            decoder, 
            decoders[i]
        );

    }
}