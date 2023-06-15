#include "pch.h"
#include "insert_instructions.h"

#pragma warning(disable: 5045)

unsigned char mov_opcodes[7] = {
    MOV, 0x45, 0x08, NULL, 0x00, 0x00, 0x00
};

int* hook_location = (int*)((int)(GetModuleHandle(NULL)) + 0xE47B2);

void clear_instructions() {
    printf("%p\n", hook_location);
    for (int i = 0; i < 10; i++) {
        *(hook_location + i) = NOP;
        printf("%d\n", *(hook_location + i));
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
