#pragma once
#include "rom.h"
#include <stdio.h>

int load_rom(CPU* cpu, const char* path) {
	int err;
	FILE* rom;

	err = fopen_s(&rom, path, "rb");
	if (err) {
		return err;
	}
	else {
		uint8_t bufferSize = 50;
		uint8_t buffer[50];

		uint8_t valid = 1;
		uint8_t bufferCount = 0;
		while (valid) {
			size_t bytesRead = fread_s(buffer, sizeof(buffer), sizeof(uint8_t), bufferSize, rom);
		
			// copying buffer to cpu memory starting from 0x200 (512)
			for (size_t i = 0; i < bufferSize; i++) {
				cpu->memory[0x200 + bufferCount * bufferSize + i] = buffer[i];
			}

			bufferCount++;
			if (bytesRead < 50) {
				valid = 0;
			}
		}
	}
}
