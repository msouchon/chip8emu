#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "opcodes.h"

void (*opcode_table[16])(chip8*) = {
    op_00nn, op_jmp, op_undefined, op_undefined,
    op_undefined, op_undefined, op_undefined, op_undefined
};

void (*opcode_table_00nn[256])(chip8*) = {[0 ... 255] = op_undefined};

void op_00nn(chip8* c) {
    opcode_table_00nn[c->opcode & 0x00ff](c);
}

// For non existant opcodes, should indicate a fault with the rom if called.
void op_undefined(chip8* c) {
    printf("Opcode not implemented: %04x\n", c->opcode);
    exit(EXIT_FAILURE);
}

//00e0
void op_cls(chip8* c) {

}

//1NNN
void op_jmp(chip8* c) {
    c->pc = c->opcode & 0x0fff;
}