#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "font.h"

bool font_load(SDL_Renderer* renderer, font_handler* fh, char* font_path) {

    SDL_Surface* surface = SDL_LoadBMP(font_path);

    if (surface == NULL) {
        printf("Font image could not be loaded, SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* rgba_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

    if (rgba_surface == NULL) {
        printf("Surface could not be converted to rgba format, SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);

    if (texture == NULL) {
        printf("Surface could not be converted to rgba format, SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_LockTexture(texture, NULL , (void**)&fh->sprite_pixels, &fh->pitch);

    memcpy(fh->sprite_pixels, rgba_surface->pixels, rgba_surface->pitch * rgba_surface->h);

    SDL_UnlockTexture(texture);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(rgba_surface);
    SDL_DestroyTexture(texture);

    return true;
}