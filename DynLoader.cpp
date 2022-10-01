#if defined(_WIN32)
#include <Windows.h>
#endif

#if  defined(__linux__)
#include <unistd.h>
#include <dlfcn.h>
#endif

#include <iostream>
#include <vector>
#include <map>
#include "DynLoader.hpp"


#if defined(_WIN32)
FunctionGetter DynLibLoader::operator[] (const std::string lib_name)
{
    HMODULE hDll = nullptr;
    if(!Name2Handle.count(lib_name) || force_resolve)
    {
        hDll = LoadLibraryA(lib_name.c_str());
        Name2Handle[lib_name] = hDll;
    }
    else
         hDll = Name2Handle[lib_name];
    FunctionGetter getter{hDll};
    return getter;
}

FARPROC FunctionGetter::operator[] (const std::string function_name)
{
    if(this->isValid())
    {
        if(!FuncMap.count(function_name))
        {
            FARPROC proc = GetProcAddress(lib_handle,function_name.c_str());
            FuncMap[function_name] = proc;
            return proc;
        }

        return FuncMap[function_name];
    }

    return nullptr;
}

void DynLibLoader::free()
{
    std::map<std::string,HMODULE>::iterator it = Name2Handle.begin();
    while(++it != Name2Handle.end())
    {
        HMODULE hDll = it->second;
        if(hDll == nullptr)
            continue;
        FreeLibrary(hDll);
    }
}

#endif


#if defined(__linux__)
FunctionGetter DynLibLoader::operator[] (const std::string lib_name)
{
    void* handle = nullptr;
    if(!Name2Handle.count(lib_name) || force_resolve)
    {
        handle = dlopen(lib_name.c_str(),RTLD_LAZY);
        Name2Handle[lib_name] = handle;
    }

    else
        handle = Name2Handle[lib_name];
    
    FunctionGetter getter{handle};
    return getter;
}

void DynLibLoader::free()
{
    std::map<std::string, void*>::iterator it = Name2Handle.begin();
    while(++it != Name2Handle.end())
    {
        void* handle = it->second;
        if(handle == nullptr)
            continue;
        dlclose(handle);
    }
}

void* FunctionGetter::operator[] (const std::string function_name)
{
    if(this->isValid())
    {
        if(!FuncMap.count(function_name))
        {
            void* function = dlsym(lib_handle,function_name.c_str());
            FuncMap[function_name] = function;
            return function;
        }

        return FuncMap[function_name];
    }

    return nullptr;
}
#endif

BOOL FunctionGetter::isValid()
{
    if(lib_handle == nullptr)
        return FALSE;
    return TRUE;
}

