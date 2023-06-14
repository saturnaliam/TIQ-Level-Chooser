#include "pch.h"
#include "insert_instructions.h"

unsigned char mov_opcodes[7] = {
    MOV, 0x45, 0x08, NULL, 0x00, 0x00, 0x00
};

unsigned char* hook_location = (unsigned char*)0x5253B2;

void clear_instructions() {
    for (int i = 0; i < 10; i++) {
        *(hook_location + i) = NOP;
    }

    return;
}

void insert_mov() {
    for (int i = 0; i < 7; i++) {
        *(hook_location + i) = mov_opcodes[i];
    }

    return;
}

void insert_mov_number(unsigned char value) {
    mov_opcodes[3] = value;

    return;
}
