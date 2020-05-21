#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FONT_NUM_CHARS 256
#define FONT_X_SIZE 32
#define FONT_Y_SIZE 32

typedef struct {
    uint32_t sprite_pixels[FONT_NUM_CHARS * FONT_X_SIZE * FONT_Y_SIZE];
    int pitch;
    SDL_Rect chars[FONT_NUM_CHARS];
} font_handler;

bool font_load(SDL_Renderer* renderer, font_handler* fh, char* font_path);