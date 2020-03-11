#include <stdio.h>
#include <stdlib.h>

#include "global.h"

/*
 * Initializes a chip8 struct with default values.
 */
chip8* chip8_initialize() {
    chip8* c = calloc(1, sizeof(*c));
    if (c == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    c->pc = ROM_LOAD_POINT;
    c->draw = false;
    return c;
}

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