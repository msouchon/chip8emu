#ifndef OPCODES_H
#define OPCODES_H

#include "global.h"

void op_00nn(chip8*);
void op_8xyn(chip8*);
void op_exnn(chip8*);
void op_fxnn(chip8*);

void op_undefined(chip8*);

void op_cls(chip8*);
void op_ret(chip8*);
void op_jmp(chip8*);
void op_call(chip8*);
void op_se_vx_nn(chip8*);
void op_sne_vx_nn(chip8*);
void op_se_vx_vy(chip8*);
void op_ld_vx_nn(chip8*);
void op_add_vx_nn(chip8*);
void op_ld_vx_vy(chip8*);
void op_or_vx_vy(chip8*);
void op_and_vx_vy(chip8*);
void op_xor_vx_vy(chip8*);
void op_add_vx_vy(chip8*);
void op_sub_vx_vy(chip8*);
void op_shr_vx_vy(chip8*);
void op_subn_vx_vy(chip8*);
void op_shl_vx_vy(chip8*);
void op_sne_vx_vy(chip8*);
void op_ld_i_nnn(chip8*);
void op_jmp_v0_nnn(chip8*);
void op_rnd_vx_nn(chip8*);
void op_drw_vx_vy_n(chip8*);
void op_skp_vx(chip8*);
void op_sknp_vx(chip8*);
void op_ld_vx_dt(chip8*);
void op_ld_vx_key(chip8*);
void op_ld_dt_vx(chip8*);
void op_ld_st_vx(chip8*);
void op_add_i_vx(chip8*);
void op_ld_i_fontvx(chip8*);
void op_bcd_vx(chip8*);
void op_ld_i_vx(chip8*);
void op_ld_vx_i(chip8*);

void (*opcode_table[16])(chip8*);
void (*opcode_table_00nn[256])(chip8*);
void (*opcode_table_8xyn[16])(chip8*);
void (*opcode_table_exnn[256])(chip8*);
void (*opcode_table_fxnn[256])(chip8*);

#endif