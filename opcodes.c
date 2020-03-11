#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "opcodes.h"

void (*opcode_table[16])(chip8*) = {
    op_00nn, op_jmp, op_call, op_se_vx_nn,
    op_undefined, op_undefined, op_ld_vx_nn, op_undefined,
    op_undefined, op_undefined, op_undefined, op_undefined,
    op_undefined, op_undefined, op_undefined, op_fxnn
};

void (*opcode_table_00nn[256])(chip8*) = {
    [0 ... 255] = op_undefined,
    [0xe0] = op_cls
};

void op_00nn(chip8* c) {
    opcode_table_00nn[c->opcode & 0x00ff](c);
}

void (*opcode_table_fxnn[256])(chip8*) = {
    [0 ... 255] = op_undefined,
    [0x07] = op_ld_vx_dt,
    [0x15] = op_ld_dt_vx
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