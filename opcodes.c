#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "opcodes.h"
#include "chip8.h"

void (*opcode_table[16])(chip8*) = {
    op_00nn, op_jmp, op_call, op_se_vx_nn,
    op_sne_vx_nn, op_se_vx_vy, op_ld_vx_nn, op_add_vx_nn,
    op_8xyn, op_sne_vx_vy, op_ld_i_nnn, op_jmp_v0_nnn,
    op_rnd_vx_nn, op_drw_vx_vy_n, op_exnn, op_fxnn
};

void (*opcode_table_00nn[256])(chip8*) = {
    [0 ... 255] = op_undefined,
    [0xe0] = op_cls,
    [0xee] = op_ret
};

void op_00nn(chip8* c) {
    opcode_table_00nn[c->opcode & 0x00ff](c);
}

void (*opcode_table_8xyn[16])(chip8*) = {
    [0 ... 15] = op_undefined,
    [0x0] = op_ld_vx_vy,
    [0x1] = op_or_vx_vy,
    [0x2] = op_and_vx_vy,
    [0x3] = op_xor_vx_vy,
    [0x4] = op_add_vx_vy,
    [0x5] = op_sub_vx_vy,
    [0x6] = op_shr_vx_vy,
    [0x7] = op_subn_vx_vy,
    [0xe] = op_shl_vx_vy
};

void op_8xyn(chip8* c) {
    opcode_table_8xyn[c->opcode & 0x000f](c);
}

void (*opcode_table_exnn[256])(chip8*) = {
    [0 ... 255] = op_undefined,
    [0x9e] = op_skp_vx,
    [0xa1] = op_sknp_vx
};

void op_exnn(chip8* c) {
    opcode_table_exnn[c->opcode & 0x00ff](c);
}

void (*opcode_table_fxnn[256])(chip8*) = {
    [0 ... 255] = op_undefined,
    [0x07] = op_ld_vx_dt,
    [0x0a] = op_ld_vx_key,
    [0x15] = op_ld_dt_vx,
    [0x18] = op_ld_st_vx,
    [0x1e] = op_add_i_vx,
    [0x29] = op_ld_i_fontvx,
    [0x33] = op_bcd_vx,
    [0x55] = op_ld_i_vx,
    [0x65] = op_ld_vx_i
};

void op_fxnn(chip8* c) {
    opcode_table_fxnn[c->opcode & 0x00ff](c);
}

// For non existant opcodes, should indicate a fault with the rom if called.
void op_undefined(chip8* c) {
    printf("Opcode not implemented/extant: %04x\n", c->opcode);
    exit(EXIT_FAILURE);
}

//00e0
void op_cls(chip8* c) {
    memset(c->graphics, 0, sizeof(c->graphics));
    c->draw = true;
    c->pc += 2;
}

//00ee
void op_ret(chip8* c) {
    c->sp--;
    c->pc = c->stack[c->sp];
    c->pc += 2;
}

//1NNN
void op_jmp(chip8* c) {
    c->pc = c->opcode & 0x0fff;
}

//2NNN
void op_call(chip8* c) {
    c->stack[c->sp] = c->pc;
    c->sp++;
    c->pc = c->opcode & 0x0fff;
}

//3XNN
void op_se_vx_nn(chip8* c) {
    if (c->v_reg[(c->opcode & 0x0f00) >> 8] == (c->opcode & 0x00ff)) {
        c->pc += 4;
    }
    else c->pc += 2;
}

//4XNN
void op_sne_vx_nn(chip8* c) {
    if (c->v_reg[(c->opcode & 0x0f00) >> 8] != (c->opcode & 0x00ff)) {
        c->pc += 4;
    }
    else c->pc += 2;
}

//5XY0
void op_se_vx_vy(chip8* c) {
    if (c->v_reg[(c->opcode & 0x0f00) >> 8] == c->v_reg[(c->opcode & 0x00f0) >> 4]) {
        c->pc += 4;
    }
    else c->pc += 2;
}

//6XNN
void op_ld_vx_nn(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->opcode & 0x00ff;
    c->pc += 2;
}

//7XNN
void op_add_vx_nn(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] += c->opcode & 0x00ff;
    c->pc += 2;
}

//8XY0
void op_ld_vx_vy(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//8XY1
void op_or_vx_vy(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] |= c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//8XY2
void op_and_vx_vy(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] &= c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//8XY3
void op_xor_vx_vy(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] ^= c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//8XY4
void op_add_vx_vy(chip8* c) {
    if (c->v_reg[(c->opcode & 0x0f00) >> 8] < (0xff - c->v_reg[(c->opcode & 0x00f0) >> 4])) {
        c->v_reg[0xf] = 0;
    }
    else c->v_reg[0xf] = 1;
    c->v_reg[(c->opcode & 0x0f00) >> 8] += c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//8XY5
void op_sub_vx_vy(chip8* c) {
    if (c->v_reg[(c->opcode & 0x0f00) >> 8] > c->v_reg[(c->opcode & 0x00f0) >> 4]) {
        c->v_reg[0xf] = 1;
    }
    else c->v_reg[0xf] = 0;
    c->v_reg[(c->opcode & 0x0f00) >> 8] -= c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//8XY6
void op_shr_vx_vy(chip8* c) {
    c->v_reg[0xf] = c->v_reg[(c->opcode & 0x0f00) >> 8] & 1;
    c->v_reg[(c->opcode & 0x0f00) >> 8] >>= 1;
    c->pc += 2;
}

//8XY7
void op_subn_vx_vy(chip8* c) {
    if (c->v_reg[(c->opcode & 0x00f0) >> 4] > c->v_reg[(c->opcode & 0x0f00) >> 8]) {
        c->v_reg[0xf] = 1;
    }
    else c->v_reg[0xf] = 0;
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->v_reg[(c->opcode & 0x00f0) >> 4] - c->v_reg[(c->opcode & 0x0f00) >> 8];
    c->pc += 2;
}

//8XYe
void op_shl_vx_vy(chip8* c) {
    c->v_reg[0xf] = c->v_reg[(c->opcode & 0x0f00) >> 8] >> 7;
    c->v_reg[(c->opcode & 0x0f00) >> 8] <<= 1;
    c->pc += 2;
}

//9XY0
void op_sne_vx_vy(chip8* c) {
    if (c->v_reg[(c->opcode & 0x0f00) >> 8] != c->v_reg[(c->opcode & 0x00f0) >> 4]) {
        c->pc += 4;
    }
    else c->pc += 2;
}

//aNNN
void op_ld_i_nnn(chip8* c) {
    c->index_reg = c->opcode & 0x0fff;
    c->pc += 2;
}

//bNNN
void op_jmp_v0_nnn(chip8* c) {
    c->pc = c->v_reg[0x0] + (c->opcode & 0x0fff);
}

//cXNN
void op_rnd_vx_nn(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = (c->opcode & 0x00ff) & (rand() % 0x100);
    c->pc += 2;
}

//dXYN
void op_drw_vx_vy_n(chip8* c) {
    uint8_t x = c->v_reg[(c->opcode & 0x0f00) >> 8];
    uint8_t y = c->v_reg[(c->opcode & 0x00f0) >> 4];
    uint8_t n = c->opcode & 0x000f;

    uint8_t x1, y1, row, pixel;
    int x_pos, y_pos;
    bool collision = false;

    x = x % X_SIZE;
    y = y % Y_SIZE;

    for (y1 = 0; y1 < n; y1++) {

        row = c->memory[c->index_reg + y1];

        for (x1 = 0; x1 < 8; x1++) {
            
            pixel = row & (0x80 >> x1);
            if (!pixel) continue;

            x_pos = (x + x1);
            y_pos = (y + y1);

            if (x_pos >= X_SIZE || y_pos >= Y_SIZE) continue;

            if (c->graphics[y_pos * X_SIZE + x_pos] == 1) {
                collision = true;
            } 

            c->graphics[y_pos * X_SIZE + x_pos] ^= 1;
        }
    }

    if (collision) c->v_reg[0xf] = 1;
    else c->v_reg[0xf] = 0;
    
    c->draw = true;
    c->pc += 2;
}

//eX9e
void op_skp_vx(chip8* c) {
    if (c->key[c->v_reg[(c->opcode & 0x0f00) >> 8]]) {
        c->pc += 4;
    }
    else c->pc += 2;
}

//eXa1
void op_sknp_vx(chip8* c) {
    if (!c->key[c->v_reg[(c->opcode & 0x0f00) >> 8]]) {
        c->pc += 4;
    }
    else c->pc += 2;
}

//fX07
void op_ld_vx_dt(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->delay_timer;
    c->pc += 2;
}

//fX0a
void op_ld_vx_key(chip8* c) {
   for (int i = 0; i < NUM_OF_KEYS; i++) {
        if (c->key[i]) {
            c->key[i] = 0;
            c->v_reg[(c->opcode & 0x0f00) >> 8] = i;
            c->pc += 2;
        }
    }
}

//fX15
void op_ld_dt_vx(chip8* c) {
    c->delay_timer = c->v_reg[(c->opcode & 0x0f00) >> 8];
    c->pc += 2;
}

//fX18
void op_ld_st_vx(chip8* c) {
    c->sound_timer = c->v_reg[(c->opcode & 0x0f00) >> 8];
    c->pc += 2;
}

//fX1e
void op_add_i_vx(chip8* c) {
    if (c->index_reg + c->v_reg[(c->opcode & 0x0f00) >> 8] > 0x0fff) {
        c->v_reg[0xf] = 1;
    }
    else c->v_reg[0xf] = 0;
    c->index_reg += c->v_reg[(c->opcode & 0x0f00) >> 8];
    c->pc += 2;
}

//fX29
void op_ld_i_fontvx(chip8* c) {
    c->index_reg = c->v_reg[(c->opcode & 0x0f00) >> 8] * 5;
    c->pc += 2;
}

//fX33
void op_bcd_vx(chip8* c) {
    c->memory[c->index_reg + 2] = c->v_reg[(c->opcode & 0x0f00) >> 8] % 10;
    c->memory[c->index_reg + 1] = (c->v_reg[(c->opcode & 0x0f00) >> 8] % 100) / 10;
    c->memory[c->index_reg + 0] = c->v_reg[(c->opcode & 0x0f00) >> 8] / 100;
    c->pc += 2;
}

//fX55
void op_ld_i_vx(chip8* c) {
    for (int i = 0; i <= (c->opcode & 0x0f00) >> 8; i++) {
        c->memory[c->index_reg + i] = c->v_reg[i];
    }
    c->pc += 2;
}

//fX65
void op_ld_vx_i(chip8* c) {
    for (int i = 0; i <= (c->opcode & 0x0f00) >> 8; i++) {
        c->v_reg[i] = c->memory[c->index_reg + i];
    }
    c->pc += 2;
}