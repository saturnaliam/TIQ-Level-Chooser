// dllmain.cpp : Defines the entry point for the DLL application.
#include "include/pch.h"
#include "include/insert_instructions.h"

#include <conio.h>
#include <iostream>
#include <string>

#pragma warning(disable: 5039) // disabling warnings that dont rly matter

int level_ids[100] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192 };

void obtain_level_address(int* &address) {
    int temp = (int)GetModuleHandle(NULL);
    temp = *(int*)(temp + 0xC95B64);
    temp = *(int*)(temp + 0x24);
    temp = *(int*)(temp + 0xA8C);
    temp = *(int*)(temp + 0x4);
    temp = *(int*)(temp + 0x2C);
    temp = *(int*)(temp + 0x50);
    temp = *(int*)(temp + 0x264);
    address = (int*)(temp + 0x4C);

    return;
}

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

    if (fpr == 0 || fpw == 0) exit(-1); // opening console for i/o, exiting if it fails

    int* level_addr = nullptr;
    obtain_level_address(level_addr);

    printf("[Q]uit");
    while (1) {
        printf("\nLevel: ");

        std::string user_input;
        getline(std::cin, user_input);

        if (tolower(user_input[0]) == 'q') break;

        int level = atoi(user_input.c_str());
        if (level > 0 && level <= 100) {
            insert_mov_number((unsigned char)level_ids[level - 1]);
            insert_mov();
            printf("Answer your current question!");
            while (*level_addr != level_ids[level - 1]);

            clear_instructions();
        } else {
            printf("Input invalid!");
        }
    }

    // cleaning up
    insert_old();
    fclose(fpw);
    fclose(fpr);
    CreateThread(0, 0, eject_thread, hModule, 0, 0);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD_PTR  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // setting up the executable so we can overwrite the memory
        DWORD_PTR old_protect;
        VirtualProtect((void*)hook_location, 10, PAGE_EXECUTE_READWRITE, &old_protect);
        clear_instructions();

        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)attached_main, hModule, 0, nullptr);
    }

    (void)lpReserved;
    return TRUE;
}

