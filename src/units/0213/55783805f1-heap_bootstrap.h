#pragma once
#include <cstdint>
#include <windows.h>
struct HeapBootstrapGlobals {
    HANDLE volatile* heap;
    std::uint32_t volatile *mode,*platform,*major,*headers,*spare_header,*count,*active_header,*threshold,*capacity,*spare_group;
};
struct HeapBootstrapApis {
    HANDLE (__stdcall *create)(DWORD,SIZE_T,SIZE_T);
    BOOL (__stdcall *destroy)(HANDLE);
    void* (__stdcall *allocate)(HANDLE,DWORD,SIZE_T);
};
extern HeapBootstrapGlobals heap_bootstrap_globals;
extern HeapBootstrapApis heap_bootstrap_apis;
extern "C" {
int __cdecl source_heap_init(int);
std::uint32_t __cdecl source_heap_select();
int __cdecl source_sbh_heap_init(std::uint32_t);
}
