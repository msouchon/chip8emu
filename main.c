#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>

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

void to_rgba(uint8_t* in_pix, uint32_t* out_pix) {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            if (in_pix[y * X_SIZE + x]) {
                out_pix[y * X_SIZE + x] = 0xffffff00;
            }
            else out_pix[y * X_SIZE + x] = 0;
        }
    }
}

int
main(int argc, char* argv[]) {

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Event event;
    uint32_t rgba_graphics[X_SIZE * Y_SIZE];
    clock_t start, diff;
    bool running;
    chip8* c;

    // Init chip8 core
    c = chip8_initialize(false);
    chip8_loadgame(c, argv[1]);

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialise, SDL Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, X_WINDOW_SIZE, Y_WINDOW_SIZE, SDL_WINDOW_SHOWN);
    if (!window) {
	    printf("Window could not be created SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
	}
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, X_SIZE, Y_SIZE);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, X_SIZE, Y_SIZE);
    
    // Begin event loop
    running = true;
    while (running) {
        

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        start = clock();
        chip8_cycle(c);
        if (c->delay_timer > 0) c->delay_timer--;
        if (c->sound_timer > 0) c->sound_timer--;
        diff = start - clock();
        if (c->draw) {
            to_rgba(c->graphics, rgba_graphics);
            SDL_UpdateTexture(texture, NULL, rgba_graphics, X_SIZE * sizeof(uint32_t));
            c->draw = false;
        }
        sleep((1 / CHIP8_CLOCKS_PER_SEC) - diff);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}