// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>

__declspec(naked) void clear_instructions() {
    __asm {
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
    }
}

DWORD WINAPI attached_main(LPVOID lpParameter) {

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    DWORD old_protect;
    unsigned char* hook_location = (unsigned char*)0x001953B2;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        VirtualProtect((void*)hook_location, 10, PAGE_EXECUTE_READWRITE, &old_protect);
        *(DWORD*)(hook_location) = (DWORD)&clear_instructions;
    }

    return TRUE;
}

