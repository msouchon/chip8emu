#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "chip8.h"

/*
 * Provides basic debug printing which prints the display to
 * the console/terminal. Uses X's to display active pixels.
 */
void debug_print(chip8* c) {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            if (c->graphics[y * X_SIZE + x]) printf("X");
            else printf(" ");
        }
        printf("\n");
    }
}

int
main(int argc, char* argv[]) {
    chip8* c = chip8_initialize(false);
    printf("Sucessfully Initialized CHIP8 Struct\n");

    chip8_loadgame(c, argv[1]);
    printf("Sucessfully Loaded Game\n");

    clock_t start, diff;

    while (true) {
        //printf("%04x\n", c->opcode);
        start = clock();
        chip8_cycle(c);
        if (c->delay_timer > 0) c->delay_timer--;
        if (c->sound_timer > 0) c->sound_timer--;
        diff = start - clock();
        if (c->draw) {
            debug_print(c);
            c->draw = false;
        }
        sleep((1 / CHIP8_CLOCKS_PER_SEC) - diff);
    }

    return EXIT_SUCCESS;
}