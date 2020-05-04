#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "opcodes.h"
#include "chip8.h"


uint8_t chip8_fontset[FONTSET_LENGTH] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/*
 * Initializes a chip8 struct with default values.
 */
chip8* chip8_initialize(bool fontset_in_memory) {
    chip8* c = calloc(1, sizeof(*c));
    if (c == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Some ROMS require fontset data to be stored at the start of memory
    // e.g 15 Puzzle
    c->fontset_in_memory = fontset_in_memory;
    if (c->fontset_in_memory) {
        for (int i = 0; i < FONTSET_LENGTH; i++) {
            c->memory[i] = chip8_fontset[i];
        }
    }

    c->pc = ROM_LOAD_POINT;
    c->draw = false;
    return c;
}

/*
 * Given a filename loads that file into the default rom load point.
 * Provides error handling.
 */
void chip8_loadgame(chip8* c, char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    if (0 != fseek(fp, 0, SEEK_END)) {
        perror("Failed to find the end of the file");
        exit(EXIT_FAILURE);
    }

    long fsize = ftell(fp);
    if (-1 == fsize) {
        perror("Failed to find position indicator");
        exit(EXIT_FAILURE);
    }

    rewind(fp);

    fread(c->memory+ROM_LOAD_POINT, 1, fsize, fp);

    fclose(fp);
    return;
}

void chip8_cycle(chip8* c) {
    c->opcode = c->memory[c->pc] << 8 | c->memory[c->pc + 1];
    opcode_table[(c->opcode & 0xf000) >> 12](c);
    return;
}