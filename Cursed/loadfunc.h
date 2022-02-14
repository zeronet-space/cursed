#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <windows.h>

template <typename T>
std::function<T> loadfunc(const std::string& dllName, const std::string& funcName) {
    HINSTANCE hGetProcIDDLL = LoadLibraryA(dllName.c_str());

    if (hGetProcIDDLL == NULL) {
        std::cerr << "Could not load DLL \"" << dllName << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }

    FARPROC lpfnGetProcessID = GetProcAddress(hGetProcIDDLL, funcName.c_str());

    if (!lpfnGetProcessID) {
        std::cerr << "Could not locate the function \"" << funcName << "\" in DLL\"" << dllName << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::function<T> func(reinterpret_cast<T*>(lpfnGetProcessID));

    return func;
}