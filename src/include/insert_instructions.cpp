#include "pch.h"
#include "insert_instructions.h"

#pragma warning(disable: 5045)

byte mov_opcodes[7] = {
    MOV, 0x45, 0x08, NULL, 0x00, 0x00, 0x00
};

byte old_opcodes[10] = {
    0x80, 0x7D, 0x0C, 0x00, 0x0F, 0x84, 0xA3, 0x00, 0x00, 0x00
};

byte* hook_location = (byte*)((int)GetModuleHandle(NULL) + 0xE53B2);

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

void insert_old() {
    for (int i = 0; i < 10; i++) {
        *(hook_location + i) = old_opcodes[i];
    }
}

void insert_mov_number(byte value) {
    mov_opcodes[3] = value;

    return;
}
