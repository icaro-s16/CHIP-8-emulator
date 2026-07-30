#include "chip8_emulator.h"



static int chip8_sync_execution(
    double *last_time, 
    const double time_per_op
){
    static double delta = 0;
    double cur_time = GetTime();
    delta += (
        cur_time -
        *last_time
    );
    
    *last_time += (cur_time - *last_time);

    if (delta >= time_per_op){
        delta = delta - time_per_op;
        return 1;
    }
    
    return 0;
}

static int chip8_sync_timer_update(
    double *last_time
){
    const double timer_update_per_sec = 1 / 60.0 ;
    static double delta = 0; 
    double cur_time = GetTime();
    delta += (
        cur_time -
        *last_time
    );

    *last_time += (cur_time - *last_time);

    if (delta >= timer_update_per_sec){
        delta = delta - timer_update_per_sec;
        return 1;
    }

    return 0;
}

void chip8_emulator_run(
    const char* rom_name,
    const uint32_t op_per_sec
){
    assert(rom_name != NULL);

    srand(time(NULL));
        
    Chip8VM vm = chip8_vm_construct(rom_name);  

    vm.dissasembly = chip8_dissasembler_construct(&vm.memory);
    
    chip8_construct_display();

    double time_per_op = 1.0 / op_per_sec; 
    double cpu_last_time, timer_last_time = GetTime(), GetTime();

    while(!WindowShouldClose()){
        
        chip8_update_keys_map(&vm);
        while(chip8_sync_execution(&cpu_last_time, time_per_op)){
            chip8_emulate_cycle(&vm);
        }

        // DONE: Implement sound
        // DONE: Timer update sync
        while(chip8_sync_timer_update(&timer_last_time)){
            if (vm.delay_timer > 0) vm.delay_timer -= 1;
            if (vm.sound_timer > 0) {
                chip8_play_beep();
                vm.sound_timer -= 1;
            }
        }

        chip8_update_display(&vm);
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

        DecodedOpcode decoded_opcode = {0};
        decoded_opcode.optype= chip8_get_optype(opcode);
        void (*opcode_decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode); 

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

    DecodedOpcode decoded_opcode = {0};
    decoded_opcode.optype = chip8_get_optype(opcode);
    chip8_set_executor(&decoded_opcode);
    void (*opcode_decoder)(const addr, DecodedOpcode*) = chip8_get_decoder(&decoded_opcode); 
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