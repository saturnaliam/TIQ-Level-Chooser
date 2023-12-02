#ifndef INSERT_INSTRUCTIONS_H
#define INSERT_INSTRUCTIONS_H

#define NOP 0x90
#define MOV 0xC7
typedef unsigned char byte;

#include <iostream>
extern byte* hook_location;

extern byte mov_opcodes[7];
extern byte old_opcodes[10];

void clear_instructions();
void insert_old();
void insert_mov();

void insert_mov_number(byte value);

#endif
