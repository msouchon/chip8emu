#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

#include "global.h"

enum graphics_settings {
    EPX_2X,
    EPX_4X,
    EPX_8X,
    EPX_16X,
    SCALE2X,
    SCALE4X,
    SCALE8X,
    SCALE16X
};

typedef struct {
    uint8_t* in_pix;
    uint8_t* out_pix;
    int x_size;
    int y_size;
    int x_out_size;
    int y_out_size;
    uint8_t* buffers[10];
    int num_buffers;
    int graphics_setting;
} graphics_handler;

graphics_handler* graphics_init(int, int, int);
void graphics_destroy(graphics_handler*);
void graphics_cycle(graphics_handler*, chip8*);
void graphics_debug_print(chip8*);
void graphics_to_rgba(uint8_t*, uint32_t*, int, int);
void graphics_epx2x(graphics_handler*, uint8_t*, uint8_t*, int, int);
void graphics_scale2x(graphics_handler*, uint8_t*, uint8_t*, int, int);

#endif