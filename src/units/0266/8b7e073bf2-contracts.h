#pragma once
#include "b6a6f59d48-startup_views.h"
#include "c21a44d9b0-bindings.h"
#include "45c36ce452-quick_exit.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef exception_code
#undef exception_code
#endif
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error MSVC x86 required
#endif
extern "C" int __cdecl unresolved_exception_filter_00701F00(std::uint32_t,EXCEPTION_POINTERS*);
extern "C" void __cdecl unresolved_normal_exit_006FB212(int);
// Imported providers; os_imports.cpp supplies typed Kernel32 forwarding only.
extern "C" DWORD WINAPI unresolved_GetTickCount_005ec200();
extern "C" BOOL WINAPI unresolved_QueryPerformanceCounter_0073d1a8(LARGE_INTEGER*);
extern "C" LONG WINAPI unresolved_UnhandledExceptionFilter_0073d100(EXCEPTION_POINTERS*);
// Raw x87 ABI: ST0 input, EDX:EAX result, no stack parameters. Assembly only.
// Existing conversion-provider ownership remains separate; no implementation here.
extern "C" std::uint64_t __cdecl unresolved_ftol2_006fae8c();
