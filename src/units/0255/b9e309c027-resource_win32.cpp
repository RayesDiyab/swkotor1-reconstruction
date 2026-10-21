// Stateless task6 OS binding proposal. These are IAT seams, not game bodies.
#include "1adde386db-resources.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstddef>
#include <type_traits>

using menu_resources::Word;
static_assert(sizeof(void*) == 4 && sizeof(Word) == 4 && sizeof(DWORD) == 4 &&
              sizeof(SIZE_T) == 4 && sizeof(HANDLE) == 4, "x86 physical words");
static_assert(sizeof(menu_resources::MemoryStatus) == sizeof(MEMORYSTATUS) &&
              sizeof(MEMORYSTATUS) == 32 &&
              alignof(menu_resources::MemoryStatus) == alignof(MEMORYSTATUS),
              "eight aligned DWORD output words");
static_assert(offsetof(menu_resources::MemoryStatus, words) == 0 &&
              offsetof(MEMORYSTATUS, dwLength) == 0 &&
              offsetof(MEMORYSTATUS, dwMemoryLoad) == 4 &&
              offsetof(MEMORYSTATUS, dwTotalPhys) == 8 &&
              offsetof(MEMORYSTATUS, dwAvailPhys) == 12 &&
              offsetof(MEMORYSTATUS, dwTotalPageFile) == 16 &&
              offsetof(MEMORYSTATUS, dwAvailPageFile) == 20 &&
              offsetof(MEMORYSTATUS, dwTotalVirtual) == 24 &&
              offsetof(MEMORYSTATUS, dwAvailVirtual) == 28, "legacy MEMORYSTATUS layout");
using NativeThread = DWORD (WINAPI*)(LPVOID);
using NativeCreate = HANDLE (WINAPI*)(LPSECURITY_ATTRIBUTES, SIZE_T,
                                    LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
using NativeMemory = void (WINAPI*)(LPMEMORYSTATUS);
static_assert(std::is_same<LPTHREAD_START_ROUTINE, NativeThread>::value &&
              std::is_same<decltype(&::CreateThread), NativeCreate>::value &&
              std::is_same<decltype(&::GlobalMemoryStatus), NativeMemory>::value,
              "actual installed SDK conventions");

// Preserve the physical x86 call frame, callback address and output pointer.
// DWORD is unsigned long; Word is unsigned int in this owner cohort. A tail
// transfer avoids C++ function-pointer or structure type punning and copies.
// The actual SDK import supplies RET4/RET24 and the raw EAX handle result.
extern "C" __declspec(naked) void __stdcall
unresolved_global_memory_status_0073d1c0(menu_resources::MemoryStatus*) {




    __asm jmp GlobalMemoryStatus
}

extern "C" __declspec(naked) Word __stdcall
unresolved_create_thread_0073d1f0(void*, Word, menu_resources::ThreadStart,
                                 void*, Word, Word*) {



















    __asm jmp CreateThread

}
