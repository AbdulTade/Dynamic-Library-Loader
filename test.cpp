# ifndef __cplusplus
#error Use a c++ compiler to compile 
# endif

# include <iostream>
# include <Windows.h>
# include "DynLoader.hpp"

unsigned long long (*GetTickCount64)(void);


int main()
{
    std::string dll_name = "kernel32.dll";

    DynLibLoader loader;
    auto getter = loader[dll_name];
    auto func = getter["GetTickCount64"];
    if(func != NULL) 
        std::runtime_error("[-] Error: Could not resolve " + dll_name);

    *(FARPROC*)(&GetTickCount64) = func;
    std::cout << GetTickCount64() << std::endl;
}