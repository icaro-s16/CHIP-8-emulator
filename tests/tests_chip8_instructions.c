#include <CUnit/Basic.h>
#include "../include/chip8_emulator.h"


void test_skne_rc(){
    Chip8VM vm = {0};
    
    vm.V[0] = 0x00;
    addr opcode = 0x40FF;

    DecodedOpcode decoded_opcode = {0};
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    void(*decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode);
    decoder(opcode, &decoded_opcode);

    decoded_opcode.execute(&vm, &decoded_opcode);

    CU_ASSERT_EQUAL(
        vm.pc,
        2
    );
}

void test_skne_rr(){
    Chip8VM vm = {0};
    
    vm.V[0] = 0x01;
    vm.V[1] = 0x02;
    addr opcode = 0x9010;

    DecodedOpcode decoded_opcode = {0};
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    void(*decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode);
    decoder(opcode, &decoded_opcode);

    decoded_opcode.execute(&vm, &decoded_opcode);

    CU_ASSERT_EQUAL(
        vm.pc,
        2
    );
}

void test_jsr(){
    Chip8VM vm = {0};
    
    vm.pc = 0x321;
    addr opcode = 0x2123;

    DecodedOpcode decoded_opcode = {0};
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    void(*decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode);
    decoder(opcode, &decoded_opcode);

    decoded_opcode.execute(&vm, &decoded_opcode);

    CU_ASSERT_EQUAL(
        vm.pc,
        0x123
    );

    CU_ASSERT_EQUAL(
        vm.stack[vm.sp - 1],
        0x321
    );
}

void test_rts(){
    Chip8VM vm = {0};
    
    vm.pc = 0x321;
    addr opcode = 0x2123;

    DecodedOpcode decoded_opcode = {0};
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    void(*decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode);
    decoder(opcode, &decoded_opcode);

    decoded_opcode.execute(&vm, &decoded_opcode);

    addr expected_val = vm.stack[vm.sp - 1];

    opcode = 0x00EE;
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    decoder = chip8_get_decoder(&decoded_opcode);

    decoder(opcode, &decoded_opcode);

    decoded_opcode.execute(&vm, &decoded_opcode);

    CU_ASSERT_EQUAL(
        vm.pc, 
        expected_val
    );

    CU_ASSERT_EQUAL(
        vm.sp,
        0
    );
}

void test_mov_rr(){
    Chip8VM vm = {0};
    
    vm.V[2] = 20;
    addr opcode = 0x8120;

    DecodedOpcode decoded_opcode = {0};
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    void(*decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode);
    decoder(opcode, &decoded_opcode);

    decoded_opcode.execute(&vm, &decoded_opcode);

    CU_ASSERT_EQUAL(
        vm.V[1],
        vm.V[2]
    );

}