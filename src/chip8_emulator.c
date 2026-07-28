#include "chip8_emulator.h"


static inline uint32_t sec_to_nsec(
    uint32_t sec
){
    return sec * 1000000000;
}

static void chip8_sync_execution(
    struct timespec* last_time, 
    const uint32_t time_per_op
){

    struct timespec cur_time = {0};
    clock_gettime(CLOCK_MONOTONIC, &cur_time);

    uint32_t delta = (
        (cur_time.tv_sec * sec_to_nsec(1) + cur_time.tv_nsec) -
        (last_time->tv_sec * sec_to_nsec(1) + last_time->tv_nsec)
    );
    
    *last_time = cur_time;

    if (delta >= time_per_op)
        return;

    struct timespec delta_sleep = {
        .tv_sec = (time_per_op - delta) / sec_to_nsec(1),
        .tv_nsec = (time_per_op - delta) % sec_to_nsec(1) 
    };

    nanosleep(&delta_sleep, NULL);
    
}

static int chip8_sync_timer_update(
    struct timespec* last_time
){
    const uint32_t timer_update_per_sec = 60 / sec_to_nsec(1);
    static uint32_t delta = 0; 
    struct timespec cur_time = {0};
    clock_gettime(CLOCK_MONOTONIC, &cur_time);
    
    delta += (
        (cur_time.tv_sec * sec_to_nsec(1) + cur_time.tv_nsec) -
        (last_time->tv_sec * sec_to_nsec(1) + last_time->tv_nsec)
    );

    if (delta >= timer_update_per_sec){
        delta = 0;
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

    size_t time_per_op = sec_to_nsec(1) / op_per_sec; 
    struct timespec last_time = {0};
    clock_gettime(CLOCK_MONOTONIC, &last_time);
    
    while(!WindowShouldClose()){
        chip8_emulate_cycle(&vm, &last_time);
        chip8_sync_execution(&last_time, op_per_sec);
    }

    chip8_destroy_display();
    chip8_vm_destroy(&vm);
}

static char** chip8_dissasembler_construct(
    Memory* mem
){  
    assert(mem != NULL);

    char **dissasembly = calloc(mem->rom_size/2, sizeof(char*));
    for(int i = 0; i < mem->rom_size/2; i++)
        dissasembly[i] = calloc(50, sizeof(char));

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
    Chip8VM* vm,
    struct timespec* last_time
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

    chip8_update_display(vm);
    chip8_update_key_map(vm);

    // TODO: Implement sound
    // DONE: Timer update sync
    if (chip8_sync_timer_update(last_time)){
        if (vm->delay_timer > 0) vm->delay_timer -= 1;
        if (vm->sound_timer > 0) vm->sound_timer -= 1;
    }

    if (vm->pc >= MEM_MAX_SIZE){
        fprintf(stderr, "ERROR: Program counter exceeds available memory limits...\n");
        exit(1);
    }
}

static Chip8VM chip8_vm_construct(const char* rom_name){
    assert(rom_name != NULL);

    Chip8VM vm = {0};
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