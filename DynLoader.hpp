#pragma once

#if defined(_WIN32)
#include <Windows.h>
#endif

#if defined(__linux__)
#include <unistd.h>
#include <dlfcn.h>

typedef enum
{
    FALSE,
    TRUE
} BOOL;
#endif

#ifndef __cplusplus
#error Source must be compiled with C++
#endif


#include <iostream>
#include <vector>
#include <map>

class FunctionGetter
{
private:
#ifdef _WIN32
    std::map<std::string, FARPROC> FuncMap; /* Maps function name to the function pointer */
    HMODULE lib_handle = nullptr;           /* A handle to the current library of interest */
#endif

#ifdef __linux__
    std::map<std::string, void *> FuncMap; /* Maps function_name to function pointer */
    void *lib_handle = nullptr;            /* A handle to the current library of interest */
#endif

public:
    BOOL isValid();
#ifdef _WIN32
    FunctionGetter(HMODULE lib_handle) : lib_handle(lib_handle){};
    FARPROC operator[](const std::string function_name);
#endif

#ifdef __linux__
    FunctionGetter(void *lib_handle) : lib_handle(lib_handle){};
    void *operator[](const std::string function_name);
#endif
};

/**
 * @brief Dynamic Library loader. Cross-platform for both linux and windows
 * 
 */
class DynLibLoader
{
private:
    BOOL force_resolve = FALSE;
#if defined(_WIN32)
    std::map<std::string, HMODULE> Name2Handle;
#endif

#if defined(__linux__)
    std::map<std::string, void *> Name2Handle;
#endif

public:
    DynLibLoader(BOOL force_resolve = FALSE) : force_resolve(force_resolve) {};
    FunctionGetter operator[](const std::string lib_name);
    void free();
    ~DynLibLoader()
    {
        free();
    }
};