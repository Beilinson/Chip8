#pragma once
#include <stdint.h>

typedef struct _cpu {
	uint8_t* memory;
	uint8_t* v;
	uint16_t* stack;
	uint16_t pc, i, opcode;
	uint8_t sp, delay, sound;
	uint32_t* gfx;

} CPU;

void setup(CPU* cpu);

void initialize(CPU* cpu);