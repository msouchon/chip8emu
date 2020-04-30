#ifndef OPCODES_H
#define OPCODES_H

#include "global.h"

void op_00nn(chip8*);
void op_8xyn(chip8*);
void op_fxnn(chip8*);

void op_undefined(chip8*);

void op_cls(chip8*);
void op_ret(chip8*);
void op_jmp(chip8*);
void op_call(chip8*);
void op_se_vx_nn(chip8*);
void op_sne_vx_nn(chip8*);
void op_ld_vx_nn(chip8*);
void op_add_vx_nn(chip8*);
void op_ld_vx_vy(chip8*);
void op_add_vx_vy(chip8*);
void op_sub_vx_vy(chip8*);
void op_shl_vx_vy(chip8*);
void op_ld_i_nnn(chip8*);
void op_rnd_vx_nn(chip8*);
void op_drw_vx_vy_n(chip8*);
void op_ld_vx_dt(chip8*);
void op_ld_dt_vx(chip8*);
void op_add_i_vx(chip8*);
void op_ld_i_vx(chip8*);
void op_ld_vx_i(chip8*);

void (*opcode_table[16])(chip8*);
void (*opcode_table_00nn[256])(chip8*);
void (*opcode_table_8xyn[16])(chip8*);
void (*opcode_table_fxnn[256])(chip8*);

#endif