#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

#include "global.h"

void graphics_debug_print(chip8*);
void graphics_to_rgba(uint8_t*, uint32_t*, int, int);
void graphics_epx(uint8_t*, uint8_t*, int, int);

#endif