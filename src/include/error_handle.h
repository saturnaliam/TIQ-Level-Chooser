#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H

#include <string.h>
#include <conio.h>
#include <iostream>

[[noreturn]] void die(std::string error);
#endif
