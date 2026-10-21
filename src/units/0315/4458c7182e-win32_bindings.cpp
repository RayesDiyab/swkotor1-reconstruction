#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "37df5d1510-win32_bindings.h"
#include <type_traits>
using namespace kotor_missing_odd;
static_assert(sizeof(HANDLE)==4 && sizeof(DWORD)==4 && sizeof(Word)==4,
              "native x86 HANDLE and status bits");
static_assert(std::is_same<decltype(&rim_suspend_thread_0073d1c4),SuspendGate>::value,
              "exact existing RIM suspend slot");
static_assert(std::is_same<decltype(&rim_exit_thread_0073d1e0),ExitThreadGate>::value,
              "exact existing RIM terminating slot");
extern "C" Word __stdcall rim_suspend_thread_0073d1c4(Word handle_bits) {
    const HANDLE handle=reinterpret_cast<HANDLE>(static_cast<std::uintptr_t>(handle_bits));
    return static_cast<Word>(::SuspendThread(handle));
}
extern "C" __declspec(noreturn) void __stdcall rim_exit_thread_0073d1e0(Word status) {
    ::ExitThread(static_cast<DWORD>(status));
}
