// dllmain.cpp : Defines the entry point for the DLL application.
#include "include/pch.h"
#include "include/insert_instructions.h"

#include <conio.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4100)
#pragma warning(disable: 5039) // todo fix the bugs, rather than disable their warnings

DWORD __stdcall eject_thread(LPVOID lpParameter) {
    HMODULE hModule = reinterpret_cast<HMODULE>(lpParameter);
    fclose(stdin);
    fclose(stdout);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

DWORD WINAPI attached_main(HMODULE hModule) {
    AllocConsole();
    FILE* fpr;
    FILE* fpw;
    freopen_s(&fpw, "CONOUT$", "w", stdout);
    freopen_s(&fpr, "CONIN$", "r", stdin); // todo error handling for if either of these turn out to be 0

    clear_instructions();

    std::string user_input;
    while (1) {
        printf("Please put in the level you wish to go to: ");
        getline(std::cin, user_input);

        if (user_input == "q") break;
    }

    fclose(fpw);
    fclose(fpr);
    CreateThread(0, 0, eject_thread, hModule, 0, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    DWORD old_protect;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        VirtualProtect((void*)hook_location, 10, PAGE_EXECUTE_READWRITE, &old_protect);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)attached_main, hModule, 0, nullptr);
    }

    return TRUE;
}

