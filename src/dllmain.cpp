// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#define nop 0x90

unsigned char* hook_location = (unsigned char*)0x001953B2;
int goto_level = 0x9;

__declspec(naked) void move_value() {
    __asm {
        mov dword ptr ss:[ebp + 8], 3
    }
}

void clear_instructions() {
    for (int i = 0; i < 10; i++) {
        *(hook_location + i) = nop;
    }

    return;
}

DWORD WINAPI attached_main(LPVOID lpParameter) {
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    DWORD old_protect;


    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        VirtualProtect((void*)hook_location, 10, PAGE_EXECUTE_READWRITE, &old_protect);
        clear_instructions();
        *(DWORD*)hook_location = (DWORD)&move_value;
    }

    return TRUE;
}

