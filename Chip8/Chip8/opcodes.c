#pragma once
#include "opcodes.h"

int handle_opcode(CPU* cpu) {
	uint16_t opcode = cpu->opcode;

	uint8_t x, y, kk, n;

	x = (opcode & 0x0F00) >> 8;
	y = (opcode & 0x00F0) >> 4;
	kk = (opcode & 0x00FF);
	n = (opcode & 0x000F);

	switch (opcode & 0xF000) {
		case 0x0000:
			memset(cpu->gfx, 0, sizeof(cpu->gfx));
			break;
		case 0x1000:
			cpu->pc = opcode & 0x0FFF;
			break;
		case 0x2000:
			cpu->stack[cpu->sp] = cpu->pc;
			cpu->sp++;
			cpu->pc = opcode & 0x0FFF;
			break;
		case 0x3000:
			if (cpu->v[x] == kk) {
				cpu->pc += 2;
			}
			break;
		case 0x4000:
			if (cpu->v[x] != kk) {
				cpu->pc += 2;
			}
			break;
		case 0x5000:
			if (cpu->v[x] == cpu->v[y]) {
				cpu->pc += 2;
			}
			break;
		case 0x6000:
			cpu->v[x] = kk;
			break;
		case 0x7000:
			cpu->v[x] += kk;
			break;
		case 0x8000:
			// subswitch:
			switch (n) {
			case 0x00:
				cpu->v[x] = cpu->v[y];
				break;
			case 0x01:
				cpu->v[x] |= cpu->v[y];
				break;
			case 0x02:
				cpu->v[x] &= cpu->v[y];
				break;
			case 0x04:
				uint16_t sum = cpu->v[x] + cpu->v[y];

				if (sum > 255U)
				{
					cpu->v[0xF] = 1;
				}
				else
				{
					cpu->v[0xF] = 0;
				}

				cpu->v[x] = sum & 0xFF;
				break;
			case 0x05:
				if (cpu->v[x] > cpu->v[y])
				{
					cpu->v[0xF] = 1;
				}
				else
				{
					cpu->v[0xF] = 0;
				}

				cpu->v[x] -= cpu->v[y];
				break;
			case 0x06:
				cpu->v[0xF] = (cpu->v[x] & 0x1);
				cpu->v[x] >>= 1;
				break;
			case 0x07:
				if (cpu->v[y] > cpu->v[x])
				{
					cpu->v[0xF] = 1;
				}
				else
				{
					cpu->v[0xF] = 0;
				}

				cpu->v[x] = cpu->v[y] - cpu->v[x];
				break;
			case 0x0E:
				cpu->v[0xF] = (cpu->v[x] & 0x80) >> 7;

				cpu->v[x] <<= 1;
				break;
			default:
				return -1;
			}
			break;
		case 0x9000:
			if (cpu->v[x] != cpu->v[y]) {
				cpu->pc += 2;
			}
			break;
		case 0xA000:
			cpu->i = (opcode & 0x0FFF);
			break;
		case 0xB000:
			cpu->pc = cpu->v[0] + (opcode & 0x0FFF);
			break;
		case 0xC000:
			uint8_t rand_byte = (uint8_t)(rand() % 256);
			cpu->v[x] =  & kk;
			break;
		case 0xD000:
			// draw
			break;
		case 0xE000:
			// subswitch:
			switch (n) {
				switch (opcode & 0x00FF) {
				case 0x009E:
					// if (keypad[cpu->v[x]]) { pc += 2; }
					break;
				case 0x00A1:
					// if (!keypad[cpu->v[x]]) { pc += 2; }
					break;
				default:
					return -1;
				}
				break;
			}
			break;
		case 0xF000:
			// subswitch:
			switch (n) {
				switch (opcode & 0x00FF) {
				case 0x0007:
					cpu->v[x] = cpu->delay;
					break;
				case 0x000A:
					/*
					Wait for a key press, store the value of the key in Vx.
					The easiest way to “wait” is to decrement the PC by 2
					whenever a keypad value is not detected. This has the
					effect of running the same instruction repeatedly.
					*/
					break;
				case 0x0015:
					cpu->delay = cpu->v[x];
					break;
				case 0x0018:
					cpu->sound = cpu->v[x];
					break;
				case 0x001E:
					cpu->i += cpu->v[x];
					break;
				case 0x0029:
					cpu->i = 0x50 + (5 * cpu->v[x]);
					break;
				case 0x0033:
					cpu->memory[cpu->i]		= (cpu->v[x] / 100);
					cpu->memory[cpu->i + 1] = (cpu->v[x] % 100) / 10;
					cpu->memory[cpu->i + 2] = (cpu->v[x] % 10);
					break;
				case 0x0055:
					for (uint8_t i = 0; i <= x; ++i)
					{
						cpu->memory[cpu->i + i] = cpu->v[i];
					}
					break;
				case 0x0065:
					for (uint8_t i = 0; i <= x; ++i)
					{
						cpu->v[i] = cpu->memory[cpu->i + i];
					}
				default:
					return -1;
				}
				break;
			}
		default:
			return -1;
	}
	return 0;
}