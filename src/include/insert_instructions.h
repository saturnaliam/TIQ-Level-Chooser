#ifndef INSERT_INSTRUCTIONS_H
#define INSERT_INSTRUCTIONS_H

#define NOP 0x90
#define MOV 0xC7

#include <iostream>
extern unsigned char* hook_location;

extern unsigned char mov_opcodes[7];

void clear_instructions();
void insert_mov();

void insert_mov_number(unsigned char value);

#endif
