#include "pch.h"
#include "error_handle.h"

[[noreturn]] void die(std::string error) {
    printf("Error: %s!", error.c_str());
    _getch();
    exit(-1);
}
