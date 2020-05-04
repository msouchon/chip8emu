#ifndef CHIP8_H
#define CHIP8_H

uint8_t chip8_fontset[80];

chip8* chip8_initialize(bool);
void chip8_loadgame(chip8*, char*);
void chip8_cycle(chip8*);

#endif