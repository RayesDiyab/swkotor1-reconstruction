#pragma once
#include <windows.h>
#include <stdint.h>
struct NlsGlobals {
    uint32_t *classification_mode_00834034;
    uint32_t *mapping_mode_00833ec4;
    uint32_t *locale_codepage_0083402c;
    uint32_t *locale_id_0083401c;
};
struct NlsBindings {
    decltype(&GetStringTypeW) type_w;
    decltype(&GetStringTypeA) type_a;
    decltype(&LCMapStringW) map_w;
    decltype(&LCMapStringA) map_a;
    decltype(&MultiByteToWideChar) to_wide;
    decltype(&WideCharToMultiByte) to_narrow;
    decltype(&GetLastError) last_error;
    int (__cdecl *locale_cp_00709419)(uint32_t);
    char* (__cdecl *transcode_00709462)(uint32_t,uint32_t,const char*,int*,char*,int);
    void (__cdecl *free_006fb7b2)(void*);
};
extern NlsGlobals nls_globals;
extern NlsBindings nls_bindings;
// Explicit candidate boundary, never a fabricated original success/failure value.
extern "C" __declspec(noreturn) void __cdecl nls_outside_contract(const char* reason);
extern "C" int __cdecl nls_classify_007073f8(uint32_t type,const char* input,int count,
    uint16_t* output,uint32_t codepage,uint32_t locale,int strict);
extern "C" int __cdecl nls_map_0070272d(uint32_t locale,uint32_t flags,const char* input,
    int count,char* output,int capacity,uint32_t codepage,int strict);
