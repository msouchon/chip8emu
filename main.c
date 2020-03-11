#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "chip8.h"

int
main(int argc, char* argv[]) {
    chip8* c = chip8_initialize();
    printf("Sucessfully Initialized CHIP8 Struct\n");

    chip8_loadgame(c, argv[1]);
    printf("Sucessfully Loaded Game\n");

    while (true) {
        chip8_cycle(c);
    }

    return EXIT_SUCCESS;
}