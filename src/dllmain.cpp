// dllmain.cpp : Defines the entry point for the DLL application.
#include "include/pch.h"
#include "include/insert_instructions.h"
#include "include/error_handle.h"

#include <conio.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4100)
#pragma warning(disable: 5039) // disabling warnings that dont rly matter

DWORD_PTR __stdcall eject_thread(LPVOID lpParameter) {
    HMODULE hModule = reinterpret_cast<HMODULE>(lpParameter);
    fclose(stdin);
    fclose(stdout);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

DWORD_PTR WINAPI attached_main(HMODULE hModule) {
    AllocConsole();
    FILE* fpr;
    FILE* fpw;
    freopen_s(&fpw, "CONOUT$", "w", stdout);
    freopen_s(&fpr, "CONIN$", "r", stdin);
    if (fpr == 0 || fpw == 0) die("there was an error when opening the console");

    DWORD_PTR current_level = (DWORD_PTR)GetModuleHandle(NULL);
    current_level = *(DWORD_PTR*)(current_level + 0xC95B64);
    current_level = *(DWORD_PTR*)(current_level + 0x24);
    current_level = *(DWORD_PTR*)(current_level + 0xA8C);
    current_level = *(DWORD_PTR*)(current_level + 0x4);
    current_level = *(DWORD_PTR*)(current_level + 0x2C);
    current_level = *(DWORD_PTR*)(current_level + 0x50);
    current_level = *(DWORD_PTR*)(current_level + 0x264);
    int* level_addr = (int*)(current_level + 0x4C);


    std::string user_input;
    //printf("%p\n", hook_location);
    clear_instructions();
    while (1) {
        printf("Please enter the level you wish to go to, or [q]uit: ");
        getline(std::cin, user_input);

        if (user_input == "q") break;

        int level = atoi(user_input.c_str());

        if (level > 0) {
            insert_mov_number((unsigned char)level);
            insert_mov();
            while (*level_addr != level);
            clear_instructions();
        } else {
            printf("Invalid number! Please input a valid level!\n");
        }
    }

    fclose(fpw);
    fclose(fpr);
    CreateThread(0, 0, eject_thread, hModule, 0, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD_PTR  ul_reason_for_call, LPVOID lpReserved) {
    DWORD_PTR old_protect;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        VirtualProtect((void*)hook_location, 10, PAGE_EXECUTE_READWRITE, &old_protect);
        clear_instructions();
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)attached_main, hModule, 0, nullptr);
    }

    return TRUE;
}

