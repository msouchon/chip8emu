#ifndef OPCODES_H
#define OPCODES_H

#include "global.h"

void op_00nn(chip8*);
void op_fxnn(chip8*);

void op_undefined(chip8*);

void op_cls(chip8*);
void op_jmp(chip8*);
void op_call(chip8*);
void op_se_vx_nn(chip8*);
void op_ld_vx_nn(chip8*);
void op_ld_vx_dt(chip8*);
void op_ld_dt_vx(chip8*);

void (*opcode_table[16])(chip8*);
void (*opcode_table_00nn[256])(chip8*);
void (*opcode_table_fxnn[256])(chip8*);

#endif