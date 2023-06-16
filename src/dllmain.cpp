// dllmain.cpp : Defines the entry point for the DLL application.
#include "include/pch.h"
#include "include/insert_instructions.h"
#include "include/error_handle.h"

#include <conio.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4100)
#pragma warning(disable: 5039) // disabling warnings that dont rly matter

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

void debug_on(int* level_addr) {
    while (1) {
        int* ebpfour;
        __asm {
            pushad
            mov eax, dword ptr ss:[ebp + 0x8]
            mov ebpfour, eax
            popad
        }

        printf("Level Address: %p\nLevel: %d\nHook Address: %p\nebp + 8: %p\nebp + 8 value: %d\n\n", level_addr, *level_addr, hook_location, ebpfour, *ebpfour);

        if (GetAsyncKeyState('Q')) break;

        printf("\r\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A\x1b[A");
    }

    return;
}

DWORD_PTR __stdcall eject_thread(LPVOID lpParameter) {
    HMODULE hModule = reinterpret_cast<HMODULE>(lpParameter);
    fclose(stdin);
    fclose(stdout);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

// todo complete rewrite kinda needed lol this shit is a MESS
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

        if (user_input == "debug") {
            debug_on(level_addr);
            continue;
        }

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

