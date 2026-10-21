#include "f0fff50cf7-file_thread.h"
#include <type_traits>

// Support binding for the existing Task30 declaration. No new game address.
// Original IAT0073D1DC is kernel32!ResumeThread, authenticated in R30.
namespace kotor_file_dependencies {
DWORD __stdcall resume(HANDLE handle) { return ::ResumeThread(handle); }
}
using Resume=DWORD(WINAPI*)(HANDLE);
static_assert(std::is_same<decltype(&kotor_file_dependencies::resume),Resume>::value,
              "exact Win32 ResumeThread contract");
