#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "tests_chip8_instructions.c"
#include "tests_chip8_core.c"

int main(){
    srand(time(NULL));

    CU_basic_set_mode(CU_BRM_VERBOSE);

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    
    CU_pSuite corepSuite = CU_add_suite("CHIP-8 core tests", NULL, NULL);

    if (corepSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(corepSuite, "CHIP-8 get optype test", test_chip8_get_optype) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(corepSuite, "CHIP-8 get decoder test", test_chip8_get_decoder) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite instructionspSuite = CU_add_suite("CHIP-8 instructions tests", NULL, NULL);

    if (instructionspSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(instructionspSuite, "CHIP-8 skne rc", test_skne_rc) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(instructionspSuite, "CHIP-8 skne rr", test_skne_rr) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(instructionspSuite, "CHIP-8 jsr", test_jsr) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(instructionspSuite, "CHIP-8 rts", test_rts) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}