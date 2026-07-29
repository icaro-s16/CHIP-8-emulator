#pragma once

#include <memory.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "chip8_core.h"
#include "chip8_decoder.h"
#include "chip8_interface.h"



static int chip8_sync_execution(double* last_time, const double time_per_op);

static int chip8_sync_timer_update(double* last_time);

void chip8_emulator_run(const char* room_name, const uint32_t op_per_sec);

static Chip8VM chip8_vm_construct(const char* rom_name);

static void chip8_vm_destroy(Chip8VM* vm);

static void chip8_emulate_cycle(Chip8VM* vm);

static char** chip8_dissasembler_construct(Memory* mem);

static void chip8_dissasembler_destroy(Chip8VM* vm);



