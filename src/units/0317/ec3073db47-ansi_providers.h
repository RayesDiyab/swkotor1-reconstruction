#pragma once
#include <windows.h>
#include <stdint.h>
struct AnsiProviderBindings {
    decltype(&GetLocaleInfoA) locale_info;
    decltype(&GetCPInfo) cp_info;
    decltype(&MultiByteToWideChar) to_wide;
    decltype(&WideCharToMultiByte) to_narrow;
    int (__cdecl *atoi_006faf5f)(const char*);
    unsigned (__cdecl *strlen_00701300)(const char*);
    void* (__cdecl *calloc_006fb823)(unsigned,unsigned);
    void (__cdecl *free_006fb7b2)(void*);
};
extern AnsiProviderBindings ansi_bindings;
extern "C" __declspec(noreturn) void __cdecl ansi_outside_contract(const char*);
extern "C" int __cdecl ansi_locale_cp_00709419(uint32_t locale);
extern "C" char* __cdecl ansi_transcode_00709462(uint32_t source_cp,uint32_t target_cp,
    const char* source,int* length,char* output,int capacity);
