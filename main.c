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
    uint32_t *rgba_graphics;
    Uint64 ticks;
    Uint64 last_chip8_cycle;
    Uint64 last_timer_cycle;
    int pitch;
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
    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, X_WINDOW_SIZE, Y_WINDOW_SIZE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
	    printf("Window could not be created SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
	}
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, X_SIZE, Y_SIZE);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, X_SIZE, Y_SIZE);
    
    // Begin event loop
    running = true;
    last_chip8_cycle = 0;
    last_timer_cycle = 0;
    while (running) {



        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_1:
                            c->key[0x1] = 1;
                            break;
                        case SDLK_2:
                            c->key[0x2] = 1;
                            break;
                        case SDLK_3:
                            c->key[0x3] = 1;
                            break;
                        case SDLK_4:
                            c->key[0xc] = 1;
                            break;
                        case SDLK_q:
                            c->key[0x4] = 1;
                            break;
                        case SDLK_w:
                            c->key[0x5] = 1;
                            break;
                        case SDLK_e:
                            c->key[0x6] = 1;
                            break;
                        case SDLK_r:
                            c->key[0xd] = 1;
                            break;
                        case SDLK_a:
                            c->key[0x7] = 1;
                            break;
                        case SDLK_s:
                            c->key[0x8] = 1;
                            break;
                        case SDLK_d:
                            c->key[0x9] = 1;
                            break;
                        case SDLK_f:
                            c->key[0xe] = 1;
                            break;
                        case SDLK_z:
                            c->key[0xa] = 1;
                            break;
                        case SDLK_x:
                            c->key[0x0] = 1;
                            break;
                        case SDLK_c:
                            c->key[0xb] = 1;
                            break;
                        case SDLK_v:
                            c->key[0xf] = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_1:
                            c->key[0x1] = 0;
                            break;
                        case SDLK_2:
                            c->key[0x2] = 0;
                            break;
                        case SDLK_3:
                            c->key[0x3] = 0;
                            break;
                        case SDLK_4:
                            c->key[0xc] = 0;
                            break;
                        case SDLK_q:
                            c->key[0x4] = 0;
                            break;
                        case SDLK_w:
                            c->key[0x5] = 0;
                            break;
                        case SDLK_e:
                            c->key[0x6] = 0;
                            break;
                        case SDLK_r:
                            c->key[0xd] = 0;
                            break;
                        case SDLK_a:
                            c->key[0x7] = 0;
                            break;
                        case SDLK_s:
                            c->key[0x8] = 0;
                            break;
                        case SDLK_d:
                            c->key[0x9] = 0;
                            break;
                        case SDLK_f:
                            c->key[0xe] = 0;
                            break;
                        case SDLK_z:
                            c->key[0xa] = 0;
                            break;
                        case SDLK_x:
                            c->key[0x0] = 0;
                            break;
                        case SDLK_c:
                            c->key[0xb] = 0;
                            break;
                        case SDLK_v:
                            c->key[0xf] = 0;
                            break;
                    }
                    break;
            }
        }

        ticks = SDL_GetPerformanceCounter();

        if (ticks - last_chip8_cycle > (SDL_GetPerformanceFrequency() / CHIP8_CLOCKS_PER_SEC)) {
            last_chip8_cycle = ticks;

            chip8_cycle(c);
            if (c->draw) {
                SDL_LockTexture(texture, NULL, (void**)&rgba_graphics, &pitch);
                to_rgba(c->graphics, rgba_graphics);
                SDL_UnlockTexture(texture);
                c->draw = false;
            }
        }
        
        if (ticks - last_timer_cycle > (SDL_GetPerformanceFrequency() / TIMER_CLOCKS_PER_SEC)) {
            last_timer_cycle = ticks;
            if (c->delay_timer > 0) c->delay_timer--;
            if (c->sound_timer > 0) c->sound_timer--;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}