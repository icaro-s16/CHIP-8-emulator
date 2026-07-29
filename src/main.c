#include "chip8_emulator.h"


int main(
    int argc, 
    char** argv
){
    uint32_t op_per_sec = 700;

    if (argc > 3 || argc < 2){
        fprintf(
            stderr, 
            "ERROR: Incorrect usage. The correct usage is \"./<bin> <rom path> <operation per sec>\"..\n"
        );
        exit(1);
    }

    if (argc < 3)
        printf("INFO: The emulator will use 700 operation per sec, you can change it passing the desired value \"./<bin> <rom path> <operation per sec>\"..\n");
    else if (
        sscanf(argv[2], "%lu", &op_per_sec) != 1 
    ){
        fprintf(
            stderr, 
            "ERROR: Invalid command, expected to find a ulong and found \"%s\"..\n", 
            argv[2]
        );
        exit(1);
    }

    if (op_per_sec > 10000000 || op_per_sec < 1){
        fprintf(
            stderr, 
            "ERROR: %lu operations per second may cause unexpected behavior..\n", 
            op_per_sec
        );
        exit(1);
    }


    chip8_emulator_run(
        argv[1], 
        op_per_sec
    );

    return 0;
}
