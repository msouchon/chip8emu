#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "chip8.h"

int
main(int argc, char* argv[]) {
    chip8* c = chip8_initialize();
    printf("Sucessfully Initialized CHIP8 Struct\n");

    chip8_loadgame(c, argv[1]);
    printf("Sucessfully Loaded Game\n");

    clock_t start, diff;

    while (true) {
        start = clock();
        chip8_cycle(c);
        if (c->delay_timer > 0) c->delay_timer--;
        if (c->sound_timer > 0) c->sound_timer--;
        diff = start - clock();
        sleep((1 / CHIP8_CLOCKS_PER_SEC) - diff);
    }

    return EXIT_SUCCESS;
}