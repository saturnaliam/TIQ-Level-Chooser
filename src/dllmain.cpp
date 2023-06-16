// dllmain.cpp : Defines the entry point for the DLL application.
#include "include/pch.h"
#include "include/insert_instructions.h"
#include "include/error_handle.h"

#include <conio.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4100)
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

//// void debug_on(int* level_addr) {
////     int i = 0;
////     int level_ids[110];
////     for (int z = 0; z < 110; z++) {
////         level_ids[z] = 0;
////     }

////     while (1) {
////         int* ebpfour;
////         __asm {
////             pushad
////             mov eax, dword ptr ss:[ebp + 0x8]
////             mov ebpfour, eax
////             popad
////         }

////         printf("Level Address: %p\nLevel: %d\nHook Address: %p\nebp + 8: %p\nebp + 8 value: %d\n\n", level_addr, *level_addr, hook_location, ebpfour, *ebpfour);

////         if (GetAsyncKeyState('Q')) break;

////         if (GetAsyncKeyState('G')) {
////             if (level_ids[i - 1] != *level_addr) {
////                 level_ids[i] = *level_addr;
////                 i++;
////             }
////         }

////         printf("\r\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A");
////     }

////     for (int x = 0; x < 109; x++) {
////         printf("%d, ", level_ids[x]);
////     }
////     printf("\n");

////     return;
//// }

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

    DWORD l_mode;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hStdout, &l_mode);
    SetConsoleMode(hStdout, l_mode |
        ENABLE_VIRTUAL_TERMINAL_PROCESSING |
        DISABLE_NEWLINE_AUTO_RETURN |
        ENABLE_PROCESSED_OUTPUT);
    if (fpr == 0 || fpw == 0) die("failed to open console.");

    printf("DEBUG VERSION. NOT FOR FULL RELEASE.\nVersion created %s at %s CDT.\n\n", __DATE__, __TIME__);

    int* level_addr = nullptr;
    obtain_level_address(level_addr);

    std::string user_input;

    while (1) {
        printf("Please enter the level you wish to go to, or [q]uit: ");
        getline(std::cin, user_input);

        if (tolower(user_input[0]) == 'q') break;

        int level = atoi(user_input.c_str());

        if (level > 0 && level <= 100) {
            insert_mov_number((unsigned char)level_ids[level - 1]);
            insert_mov();
            printf("Answer your current question!\n\n");
            while (*level_addr != level_ids[level - 1]);
            clear_instructions();
        } else {
            printf("Invalid number! Please input a valid level!\n\n");
        }
    }

    fclose(fpw);
    fclose(fpr);
    CloseHandle(hStdout);
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

