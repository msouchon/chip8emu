#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "graphics.h"
#include "global.h"
#include "chip8.h"



graphics_handler* graphics_init(int x_size, int y_size, int graphics_setting) {
    graphics_handler* gh = calloc(1, sizeof(*gh));
    if (gh == NULL) {
        perror("Failed to initalise graphics");
        exit(EXIT_FAILURE);
    }

    gh->x_size = x_size;
    gh->y_size = y_size;
    gh->graphics_setting = graphics_setting;
    
    switch (graphics_setting) {
        case EPX_2X:
            gh->out_pix = calloc(1, x_size * 2 * y_size * 2);
            gh->x_out_size = x_size * 2;
            gh->y_out_size = y_size * 2;
            break;
        case EPX_4X:
            gh->buffers[0] = calloc(1, x_size * 2 * y_size * 2);
            gh->out_pix = calloc(1, x_size * 4 * y_size * 4);
            gh->num_buffers = 1;
            gh->x_out_size = x_size * 4;
            gh->y_out_size = y_size * 4;
            break;
        case EPX_8X:
            gh->buffers[0] = calloc(1, x_size * 2 * y_size * 2);
            gh->buffers[1] = calloc(1, x_size * 4 * y_size * 4);
            gh->out_pix = calloc(1, x_size * 8 * y_size * 8);
            gh->num_buffers = 2;
            gh->x_out_size = x_size * 8;
            gh->y_out_size = y_size * 8;
            break;
        case EPX_16X:
            gh->buffers[0] = calloc(1, x_size * 2 * y_size * 2);
            gh->buffers[1] = calloc(1, x_size * 4 * y_size * 4);
            gh->buffers[2] = calloc(1, x_size * 8 * y_size * 8);
            gh->out_pix = calloc(1, x_size * 16 * y_size * 16);
            gh->num_buffers = 3;
            gh->x_out_size = x_size * 16;
            gh->y_out_size = y_size * 16;
            break;
        case SCALE2X:
            gh->out_pix = calloc(1, x_size * 2 * y_size * 2);
            gh->x_out_size = x_size * 2;
            gh->y_out_size = y_size * 2;
            break;
        case SCALE4X:
            gh->buffers[0] = calloc(1, x_size * 2 * y_size * 2);
            gh->out_pix = calloc(1, x_size * 4 * y_size * 4);
            gh->num_buffers = 1;
            gh->x_out_size = x_size * 4;
            gh->y_out_size = y_size * 4;
            break;
        case SCALE8X:
            gh->buffers[0] = calloc(1, x_size * 2 * y_size * 2);
            gh->buffers[1] = calloc(1, x_size * 4 * y_size * 4);
            gh->out_pix = calloc(1, x_size * 8 * y_size * 8);
            gh->num_buffers = 2;
            gh->x_out_size = x_size * 8;
            gh->y_out_size = y_size * 8;
            break;
        case SCALE16X:
            gh->buffers[0] = calloc(1, x_size * 2 * y_size * 2);
            gh->buffers[1] = calloc(1, x_size * 4 * y_size * 4);
            gh->buffers[2] = calloc(1, x_size * 8 * y_size * 8);
            gh->out_pix = calloc(1, x_size * 16 * y_size * 16);
            gh->num_buffers = 3;
            gh->x_out_size = x_size * 16;
            gh->y_out_size = y_size * 16;
            break;
    }
    return gh;
}

void graphics_destroy(graphics_handler* gh) {
    for (int i = 0; i < gh->num_buffers; i++) {
        free(gh->buffers[i]);
    }
    free(gh);
}

void graphics_cycle(graphics_handler* gh, chip8* c) {
    gh->in_pix = c->graphics;
    switch (gh->graphics_setting) {
        case EPX_2X:
            graphics_epx2x(gh, gh->in_pix, gh->out_pix, gh->x_size, gh->y_size);
            break;
        case EPX_4X:
            graphics_epx2x(gh, gh->in_pix, gh->buffers[0], gh->x_size, gh->y_size);
            graphics_epx2x(gh, gh->buffers[0], gh->out_pix, gh->x_size * 2, gh->y_size * 2);
            break;
        case EPX_8X:
            graphics_epx2x(gh, gh->in_pix, gh->buffers[0], gh->x_size, gh->y_size);
            graphics_epx2x(gh, gh->buffers[0], gh->buffers[1], gh->x_size * 2, gh->y_size * 2);
            graphics_epx2x(gh, gh->buffers[1], gh->out_pix, gh->x_size * 4, gh->y_size * 4);
            break;
        case EPX_16X:
            graphics_epx2x(gh, gh->in_pix, gh->buffers[0], gh->x_size, gh->y_size);
            graphics_epx2x(gh, gh->buffers[0], gh->buffers[1], gh->x_size * 2, gh->y_size * 2);
            graphics_epx2x(gh, gh->buffers[1], gh->buffers[2], gh->x_size * 4, gh->y_size * 4);
            graphics_epx2x(gh, gh->buffers[2], gh->out_pix, gh->x_size * 8, gh->y_size * 8);
            break;
        case SCALE2X:
            graphics_scale2x(gh, gh->in_pix, gh->out_pix, gh->x_size, gh->y_size);
            break;
        case SCALE4X:
            graphics_scale2x(gh, gh->in_pix, gh->buffers[0], gh->x_size, gh->y_size);
            graphics_scale2x(gh, gh->buffers[0], gh->out_pix, gh->x_size * 2, gh->y_size * 2);
            break;
        case SCALE8X:
            graphics_scale2x(gh, gh->in_pix, gh->buffers[0], gh->x_size, gh->y_size);
            graphics_scale2x(gh, gh->buffers[0], gh->buffers[1], gh->x_size * 2, gh->y_size * 2);
            graphics_scale2x(gh, gh->buffers[1], gh->out_pix, gh->x_size * 4, gh->y_size * 4);
            break;
        case SCALE16X:
            graphics_scale2x(gh, gh->in_pix, gh->buffers[0], gh->x_size, gh->y_size);
            graphics_scale2x(gh, gh->buffers[0], gh->buffers[1], gh->x_size * 2, gh->y_size * 2);
            graphics_scale2x(gh, gh->buffers[1], gh->buffers[2], gh->x_size * 4, gh->y_size * 4);
            graphics_scale2x(gh, gh->buffers[2], gh->out_pix, gh->x_size * 8, gh->y_size * 8);
            break;
    }
}

void graphics_debug_print(chip8* c) {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            if (c->graphics[y * X_SIZE + x]) printf("X");
            else printf(" ");
        }
        printf("\n");
    }
}

void graphics_to_rgba(uint8_t* in_pix, uint32_t* out_pix, int x_size, int y_size) {
    for (int y = 0; y < y_size; y++) {
        for (int x = 0; x < x_size; x++) {
            if (in_pix[y * x_size + x]) {
                out_pix[y * x_size + x] = 0xffffff00;
            }
            else out_pix[y * x_size + x] = 0;
        }
    }
}

void graphics_epx2x(graphics_handler* gh, uint8_t* in_pix, uint8_t* out_pix, int x_size, int y_size) {

    uint8_t p;

    for (int y = 0; y < y_size * 2; y += 2) {
        for (int x = 0; x < x_size * 2; x += 2) {
            p = in_pix[(y / 2) * x_size + (x / 2)];
            /*
            _1 = out_pix[y * (x_size * 2) + x];
            _2 = out_pix[y * (x_size * 2) + (x + 1)];
            _3 = out_pix[(y + 1) * (x_size * 2) + x];
            _4 = out_pix[(y + 1) * (x_size * 2) + (x + 1)];

            a = in_pix[((y / 2) - 1) * x_size + (x / 2)];
            b = in_pix[(y / 2) * x_size + ((x / 2) + 1)];
            c = in_pix[(y / 2) * x_size + ((x / 2) - 1)];
            d = in_pix[((y / 2) + 1) * x_size + (x / 2)];
            */
            
            out_pix[y       * (x_size * 2) +  x     ] = p;
            out_pix[y       * (x_size * 2) + (x + 1)] = p;
            out_pix[(y + 1) * (x_size * 2) +  x     ] = p;
            out_pix[(y + 1) * (x_size * 2) + (x + 1)] = p;

            int sum = 0;

            // C == A => 1 = A
            if (x > 0 && y > 0) {
                if (in_pix[(y / 2) * x_size + ((x / 2) - 1)] == in_pix[((y / 2) - 1) * x_size + (x / 2)]) {
                    out_pix[y * (x_size * 2) + x] = in_pix[((y / 2) - 1) * x_size + (x / 2)];
                }
                sum += in_pix[((y / 2) - 1) * x_size + (x / 2)];
                sum += in_pix[(y / 2) * x_size + ((x / 2) - 1)];
            }

            // A == B => 2 = B
            if (x < x_size * 2 && y > 0) {
                if (in_pix[((y / 2) - 1) * x_size + (x / 2)] == in_pix[(y / 2) * x_size + ((x / 2) + 1)]) {
                    out_pix[y * (x_size * 2) + (x + 1)] = in_pix[(y / 2) * x_size + ((x / 2) + 1)];
                }
            }

            // D == C => 3 = C
            if (x > 0 && y < y_size * 2) {
                if (in_pix[((y / 2) + 1) * x_size + (x / 2)] == in_pix[(y / 2) * x_size + ((x / 2) - 1)]) {
                    out_pix[(y + 1) * (x_size * 2) + x] = in_pix[(y / 2) * x_size + ((x / 2) - 1)];
                }
            }

            // B == D => 4 = D
            if (x < x_size * 2 && y < y_size * 2) {
                if (in_pix[(y / 2) * x_size + ((x / 2) + 1)] == in_pix[((y / 2) + 1) * x_size + (x / 2)]) {
                    out_pix[(y + 1) * (x_size * 2) + (x + 1)] = in_pix[((y / 2) + 1) * x_size + (x / 2)];
                }
                sum += in_pix[(y / 2) * x_size + ((x / 2) + 1)];
                sum += in_pix[((y / 2) + 1) * x_size + (x / 2)];
            }

            // Three identical in A, B, C, D => 1 = 2 = 3 = 4 = P
            if (sum <= 1 || sum >= 3) {
                out_pix[y       * (x_size * 2) +  x     ] = p;
                out_pix[y       * (x_size * 2) + (x + 1)] = p;
                out_pix[(y + 1) * (x_size * 2) +  x     ] = p;
                out_pix[(y + 1) * (x_size * 2) + (x + 1)] = p;
            }
        }
    }
}

void graphics_scale2x(graphics_handler* gh, uint8_t* in_pix, uint8_t* out_pix, int x_size, int y_size) {
    
    uint8_t p, a, b, c, d;
    uint8_t *_1, *_2, *_3, *_4;
    
    for (int y = 0; y < y_size * 2; y += 2) {
        for (int x = 0; x < x_size * 2; x += 2) {
            p = in_pix[(y / 2) * x_size + (x / 2)];
            
            _1 = &out_pix[y * (x_size * 2) + x];
            _2 = &out_pix[y * (x_size * 2) + (x + 1)];
            _3 = &out_pix[(y + 1) * (x_size * 2) + x];
            _4 = &out_pix[(y + 1) * (x_size * 2) + (x + 1)];

            *_1 = p;
            *_2 = p;
            *_3 = p;
            *_4 = p;
            
            if (y > 0) {
                a = in_pix[((y / 2) - 1) * x_size + (x / 2)];
            }
            if (x < x_size * 2) {
                b = in_pix[(y / 2) * x_size + ((x / 2) + 1)];
            }
            if (x > 0) {
                c = in_pix[(y / 2) * x_size + ((x / 2) - 1)];
            }
            if (y < y_size * 2) {
                d = in_pix[((y / 2) + 1) * x_size + (x / 2)];
            }
            
            if ((c == a) && (c != d) && (a != b)) {
                *_1 = a;
            }
            if ((a == b) && (a != c) && (b != d)) {
                *_2 = b;
            }
            if ((d == c) && (d != b) && (c != a)) {
                *_3 = c;
            }
            if ((b == d) && (b != a) && (d != c)) {
                *_4 = d;
            }
        }
    }
}