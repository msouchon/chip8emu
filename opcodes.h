#ifndef OPCODES_H
#define OPCODES_H

#include "global.h"

void op_00nn(chip8*);

void op_undefined(chip8*);

void op_jmp(chip8*);

void (*opcode_table[16])(chip8*);
void (*opcode_table_00nn[256])(chip8*);

#endif