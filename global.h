#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#include <stdbool.h>

// Screen size
#define X_SIZE 64
#define Y_SIZE 32

// Default ROM load point
#define ROM_LOAD_POINT 0x200

#define MEMORY_SIZE 4096

#define V_REG_NUM 16

// Most chip8 implementations have 16 levels of stack but
// the original 1802 versions allocated 24 levels
#define STACK_SIZE 24

#define NUM_OF_KEYS 16

#define CHIP8_CLOCKS_PER_SEC 60

typedef struct {
    uint16_t opcode;
    uint8_t memory[MEMORY_SIZE];
    uint8_t v_reg[V_REG_NUM];
    uint16_t index_reg;
    uint16_t pc;
    uint8_t graphics[X_SIZE * Y_SIZE];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t key[NUM_OF_KEYS];
    bool draw;
} chip8;

#endif