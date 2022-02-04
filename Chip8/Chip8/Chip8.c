// Chip8.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include "cpu.h"
#include "opcodes.h"
#include "rom.h"

int main()
{
    CPU cpu;

    setup(&cpu);
    initialize(&cpu);

    load_rom(&cpu, "C:\\Code\\Chip8\\roms\\IBM Logo.ch8");

    int running = 1;
    while (running) {
        if (cpu.pc >= 4096) {
            running = 0;
        }
        int err = cycle(&cpu);
        if (err) {
            return err;
        }
    }
}