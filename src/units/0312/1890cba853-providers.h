#pragma once
#include <cstddef>
#include <cstdint>
#include "163d10ec4d-resources.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error MSVC x86 required
#endif
namespace kotor_missing_odd {
using Word=std::uint32_t;
using Manager=menu_resources::Manager;
using ManagerGate=Manager* (__cdecl*)();
using ThisGate=void (__fastcall*)(void*,void*);
using SuspendGate=Word (__stdcall*)(Word);
using ExitThreadGate=void (__stdcall*)(Word);
struct RimBindings {
    ManagerGate load_manager_007a39e8;
    ThisGate enter_005e9300;
    ThisGate load_004094a0;
    ThisGate leave_005e9310;
    SuspendGate suspend_0073d1c4;
    ExitThreadGate exit_thread_0073d1e0;
};
using ProtectedExit=void (__cdecl*)(int);
struct FatalBindings {
    volatile Word* error_mode_00833c34;
    void (__cdecl* banner_00701ec7)();
    void (__cdecl* report_00701d50)(int);
    ProtectedExit volatile* protected_exit_007a2760;
};
// Support pointer tables only. Original receiver/cells/storage remain external.
extern RimBindings rim_bindings;
extern FatalBindings fatal_bindings;
// Caller publication validation; not additional guards in original functions.
bool BindRim(const RimBindings&) noexcept;
bool BindFatal(const FatalBindings&) noexcept;
static_assert(sizeof(Word)==4 && sizeof(void*)==4 && sizeof(Manager)==0x60,"x86 access view");
static_assert(sizeof(RimBindings)==24 && sizeof(FatalBindings)==16,"pointer tables");
}
extern "C" {
// Original ignores its one argument and terminates the current thread. Word is
// the consumer's Win32 ThreadStart type, not an observed EAX return contract.
std::uint32_t __stdcall unresolved_async_rim_thread_00409b90(void*);
// Preserve the complete37-byte original's return if its exit callback returns.
void __cdecl unresolved_fatal_protected_006FB344(int);
}
