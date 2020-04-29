#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "opcodes.h"

void (*opcode_table[16])(chip8*) = {
    op_00nn, op_jmp, op_call, op_se_vx_nn,
    op_undefined, op_undefined, op_ld_vx_nn, op_undefined,
    op_8xyn, op_undefined, op_ld_i_nnn, op_undefined,
    op_undefined, op_drw_vx_vy_n, op_undefined, op_fxnn
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
    [0x0] = op_ld_vx_vy
};

void op_8xyn(chip8* c) {
    opcode_table_8xyn[c->opcode & 0x000f](c);
}

void (*opcode_table_fxnn[256])(chip8*) = {
    [0 ... 255] = op_undefined,
    [0x07] = op_ld_vx_dt,
    [0x15] = op_ld_dt_vx,
    [0x1e] = op_add_i_vx
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

//6XNN
void op_ld_vx_nn(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->opcode & 0x00ff;
    c->pc += 2;
}

//8XY0
void op_ld_vx_vy(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->v_reg[(c->opcode & 0x00f0) >> 4];
    c->pc += 2;
}

//aNNN
void op_ld_i_nnn(chip8* c) {
    c->index_reg = c->opcode & 0x0fff;
    c->pc += 2;
}

//dXYN
void op_drw_vx_vy_n(chip8* c) {
    uint8_t x = c->v_reg[(c->opcode & 0x0f00) >> 8];
    uint8_t y = c->v_reg[(c->opcode & 0x00f0) >> 4];
    uint8_t n = c->opcode & 0x000f;

    uint8_t x1, y1, row, pixel;
    int x_pos, y_pos;

    for (y1 = 0; y1 < n; y1++) {

        row = c->memory[c->index_reg + y1];

        for (x1 = 0; x1 < 8; x1++) {
            
            pixel = row & (0x80 >> x1);
            if (!pixel) continue;

            x_pos = (x + x1) % X_SIZE;
            y_pos = (y + y1) % Y_SIZE;

            // If collision occurs between existing graphics and sprite
            // to draw V[F] = 1.
            if (c->graphics[y_pos * X_SIZE + x_pos]) {
                c->v_reg[0xf] = 1;
            }
            c->graphics[y_pos * X_SIZE + x_pos] ^= pixel;
        }
    }
    c->draw = true;
    c->pc += 2;
}

//fX07
void op_ld_vx_dt(chip8* c) {
    c->v_reg[(c->opcode & 0x0f00) >> 8] = c->delay_timer;
    c->pc += 2;
}

//fX15
void op_ld_dt_vx(chip8* c) {
    c->delay_timer = c->v_reg[(c->opcode & 0x0f00) >> 8];
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