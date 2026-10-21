#pragma once
#include "5c4ce2fd68-critical_section.h" // supplied by task-27; no local implementation
#include <stddef.h>

namespace kotor_file_dependencies {
HANDLE __stdcall create(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, void*, DWORD, DWORD*);
BOOL __stdcall priority(HANDLE, int);
DWORD __stdcall resume(HANDLE);
DWORD __stdcall wait(HANDLE, DWORD);
BOOL __stdcall close(HANDLE);
DWORD __stdcall suspend(HANDLE);
__declspec(noreturn) void __stdcall exit(DWORD);
uint32_t __cdecl read(void*, uint32_t, uint32_t, void*);
}

namespace kotor_file {
struct FileThread {
    HANDLE handle;                                  // +00
    DWORD thread_id;                                 // +04
    kotor_startup::CriticalSection critical_section; // +08
    uint32_t complete;                               // +0c
    volatile uint32_t terminate;                     // +10; legacy cross-thread flag
    void* file;                                      // +14; original CRT FILE*
    uint32_t element_size;                           // +18
    uint32_t element_count;                          // +1c
    uint32_t result;                                 // +20
    void* buffer;                                    // +24
    FileThread* __thiscall Construct();              // 005EBED0
    void __thiscall Destroy();                       // 005EB570
};
extern FileThread global_file_thread;                // binding for original 008328B0
DWORD __stdcall Callback(void* ignored_parameter);   // 005EBE10; ExitThread(1), no return
static_assert(sizeof(FileThread) == 40, "original x86 prefix");
static_assert(offsetof(FileThread, critical_section) == 8, "critical wrapper");
static_assert(offsetof(FileThread, complete) == 12, "completion");
static_assert(offsetof(FileThread, terminate) == 16, "termination");
static_assert(offsetof(FileThread, file) == 20, "FILE pointer");
static_assert(offsetof(FileThread, element_size) == 24, "fread element size");
static_assert(offsetof(FileThread, element_count) == 28, "fread count");
static_assert(offsetof(FileThread, result) == 32, "byte result");
static_assert(offsetof(FileThread, buffer) == 36, "buffer");
}
