#include "chip8_emulator.h"

void chip8_emulator_run(
    const char* rom_name,
    const uint32_t op_per_sec
){
    assert(rom_name != NULL);
    
    srand(time(NULL));
        
    Chip8VM vm = chip8_vm_construct(rom_name);  

    vm.dissasembly = chip8_dissasembler_construct(&vm.memory);
    
    chip8_construct_display();

    int quit = 0;

    int op_per_frame = ceil(op_per_sec/FPS);
    double delay = 1.0/FPS; 
    struct timespec last_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    while(!quit){
        
        quit = chip8_update_keys_map(&vm);
        
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);

        double delta = 
            (current_time.tv_sec + current_time.tv_nsec / (double)1e9) 
            - (last_time.tv_sec + last_time.tv_nsec / (double)1e9);

        if (delta < delay) continue;

        delta -= delay;
        last_time = current_time;
        
        for (int _op = 0; _op < op_per_frame; _op ++ ){
            chip8_emulate_cycle(&vm);
        }

        if (vm.should_update_display) {
            chip8_update_display(&vm);
            vm.should_update_display = 0;
        }
        if (vm.delay_timer > 0) vm.delay_timer -= 1;
        if (vm.sound_timer > 0) vm.sound_timer -= 1;
    }

    chip8_destroy_display();
    chip8_vm_destroy(&vm);
}

static char** chip8_dissasembler_construct(
    Memory* mem
){  
    assert(mem != NULL);

    char **dissasembly = calloc(mem->rom_size/2, sizeof(char*));

    for(
        int pc = PC_INITIAL_OFFSET, offset = 0; 
        pc < mem->rom_size + PC_INITIAL_OFFSET; 
        pc += 2, offset ++
    ){
        addr opcode = (
            (mem->mem[pc] << 8) | 
            mem->mem[pc + 1]
        );

        DecodedOpcode decoded_opcode = {
            .optype = chip8_get_optype(opcode),
            .execute = chip8_get_executor(decoded_opcode.optype)
        };

        const DecoderFunc opcode_decoder = chip8_get_decoder(&decoded_opcode); 

        assert(opcode_decoder != NULL);

        opcode_decoder(opcode, &decoded_opcode);
        
        printf("%s\n", decoded_opcode.mnemonic);

        dissasembly[offset] = calloc(50, sizeof(char));
        strcpy(dissasembly[offset], decoded_opcode.mnemonic);
    }

    return dissasembly;
}

static void chip8_dissasembler_destroy(
    Chip8VM* vm
){
    assert(vm != NULL);
    assert(vm->dissasembly != NULL);

    for(int i = 0; i < vm->memory.rom_size / 2; i ++)
        free(vm->dissasembly[i]);
    
    free(vm->dissasembly);

}

static void chip8_emulate_cycle(
    Chip8VM* vm
){
    assert(vm != NULL);
    
    addr opcode = (
        (vm->memory.mem[vm->pc] << 8) | 
        vm->memory.mem[vm->pc + 1]
    );
    
    vm->pc += 2;

    DecodedOpcode decoded_opcode = {
        .optype = chip8_get_optype(opcode),
        .execute = chip8_get_executor(decoded_opcode.optype)
    };

    const DecoderFunc opcode_decoder = chip8_get_decoder(&decoded_opcode); 
    
    opcode_decoder(opcode, &decoded_opcode);
    
    decoded_opcode.execute(vm, &decoded_opcode);

    if (vm->pc >= MEM_MAX_SIZE){
        fprintf(stderr, "ERROR: Program counter exceeds available memory limits...\n");
        exit(1);
    }
}

static Chip8VM chip8_vm_construct(const char* rom_name){
    assert(rom_name != NULL);

    Chip8VM vm = {};
    vm.memory = chip8_memory_construct(rom_name);
    vm.pc = PC_INITIAL_OFFSET;
    return vm;
}

static void chip8_vm_destroy(
    Chip8VM* vm
){
    assert(vm != NULL);
    assert(vm->dissasembly != NULL);
    chip8_dissasembler_destroy(vm);
}