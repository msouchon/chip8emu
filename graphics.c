#include <stdio.h>
#include <stdint.h>

#include "graphics.h"
#include "global.h"
#include "chip8.h"

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

void graphics_epx(uint8_t* in_pix, uint8_t* out_pix, int x_size, int y_size) {
    uint8_t p; //_1, _2, _3, _4, a, b, c, d;
    for (int y = 0; y < y_size * 2; y += 2) {
        for (int x = 0; x < x_size * 2; x += 2) {
            p = in_pix[(y / 2) * x_size + (x / 2)];printf("1 ");

            /*
            _1 = out_pix[y * (x_size * 2) + x];
            _2 = out_pix[y * (x_size * 2) + x];
            _3 = out_pix[(y + 1) * (x_size * 2) + x];
            _4 = out_pix[(y + 1) * (x_size * 2) + (x + 1)];

            a = in_pix[((y / 2) - 1) * x_size + (x / 2)];
            b = in_pix[(y / 2) * x_size + ((x / 2) + 1)];
            c = in_pix[(y / 2) * x_size + ((x / 2) - 1)];
            d = in_pix[((y / 2) + 1) * x_size + (x / 2)];
            */
            
            out_pix[y       * (x_size * 2) +  x     ] = p;printf("2 ");
            out_pix[y       * (x_size * 2) + (x + 1)] = p;printf("3 ");
            out_pix[(y + 1) * (x_size * 2) +  x     ] = p;printf("4 ");
            out_pix[(y + 1) * (x_size * 2) + (x + 1)] = p;printf("5 ");

            int sum;

            // C == A => 1 = A
            if (x > 0 && y > 0) {
                if (in_pix[(y / 2) * x_size + ((x / 2) - 1)] == in_pix[((y / 2) - 1) * x_size + (x / 2)]) {printf("6 ");
                    out_pix[y * (x_size * 2) + x] = in_pix[((y / 2) - 1) * x_size + (x / 2)];printf("7 ");
                }
                sum += in_pix[((y / 2) - 1) * x_size + (x / 2)];printf("8 ");
                sum += in_pix[(y / 2) * x_size + ((x / 2) - 1)];printf("9 ");
            }

            // A == B => 2 = B
            if (x < x_size * 2 && y > 0) {
                if (in_pix[((y / 2) - 1) * x_size + (x / 2)] == in_pix[(y / 2) * x_size + ((x / 2) + 1)]) {printf("10 ");
                    out_pix[y * (x_size * 2) + x] = in_pix[(y / 2) * x_size + ((x / 2) + 1)];printf("11 ");
                }
            }

            // D == C => 3 = C
            if (x > 0 && y < y_size * 2) {
                if (in_pix[((y / 2) + 1) * x_size + (x / 2)] == in_pix[(y / 2) * x_size + ((x / 2) - 1)]) {printf("12 ");
                    out_pix[(y + 1) * (x_size * 2) + x] = in_pix[(y / 2) * x_size + ((x / 2) - 1)];printf("13 ");
                }
            }

            // B == D => 4 = D
            if (x < x_size * 2 && y < y_size * 2) {
                if (in_pix[(y / 2) * x_size + ((x / 2) + 1)] == in_pix[((y / 2) + 1) * x_size + (x / 2)]) {printf("14 ");
                    out_pix[(y + 1) * (x_size * 2) + (x + 1)] = in_pix[((y / 2) + 1) * x_size + (x / 2)];printf("15 ");
                }
                sum += in_pix[(y / 2) * x_size + ((x / 2) + 1)];printf("16 ");
                sum += in_pix[((y / 2) + 1) * x_size + (x / 2)];printf("17 ");
            }

            // Three identical in A, B, C, D => 1 = 2 = 3 = 4 = P
            if (sum <= 1 || sum >= 3) {
                out_pix[y       * (x_size * 2) +  x     ] = p;printf("18 ");
                out_pix[y       * (x_size * 2) + (x + 1)] = p;printf("19 ");
                out_pix[(y + 1) * (x_size * 2) +  x     ] = p;printf("20 ");
                out_pix[(y + 1) * (x_size * 2) + (x + 1)] = p;printf("21 ");
            }
        }
    }
}