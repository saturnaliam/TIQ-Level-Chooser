// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#define NOP 0x90

unsigned char* hook_location = (unsigned char*)0x005253B2;

unsigned char MOVE_OPCODES[] = {
    0xC7, 0x45, 0x08, 0x09, 0x00, 0x00, 0x00
};

void move_value() {
    for (int i = 0; i < 7; i++) {
            *(hook_location + i) = MOVE_OPCODES[i];
    }
}

void clear_instructions() {
    for (int i = 0; i < 10; i++) {
        *(hook_location + i) = NOP;
    }
}

DWORD WINAPI attached_main(LPVOID lpParameter) {
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    DWORD old_protect;


    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        VirtualProtect((void*)hook_location, 10, PAGE_EXECUTE_READWRITE, &old_protect);
        clear_instructions();
        move_value();
    }

    return TRUE;
}

