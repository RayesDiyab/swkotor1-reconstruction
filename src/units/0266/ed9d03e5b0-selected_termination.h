#pragma once
#include <cstddef>

namespace kotor_selected_termination {
using Handler = void (__cdecl*)();
// A view of the observed field, not a reconstruction of the complete PTD.
struct PtdTerminationView {
    unsigned char opaque_prefix[0x6c];
    Handler handler;
};
static_assert(sizeof(void*) == 4, "Original x86 ABI");
static_assert(offsetof(PtdTerminationView, handler) == 0x6c, "Original handler field");
}

// Exact existing source entry for006FEEDF; its real PTD/lifecycle bindings must
// come from its accepted owner. No provider is defined by this package.
extern "C" void* __cdecl compat_getptd();

namespace kotor_selected_termination_dependencies {
void __cdecl report_runtime_error_00701d50(int);
int __cdecl raise_signal_00705b8d(int);
//006FB223 supplies doexit(code, quick=1, return_to_caller=0), whose terminal
// branch reaches006FB098 and the imported ExitProcess. Never bind a returning
// fixture, host CRT _exit, or a direct ExitProcess shortcut in production.
__declspec(noreturn) void __cdecl quick_exit_006fb223(int);
}

namespace kotor_filter_callback_dependencies {
// Same cdecl zero-argument linkage as the accepted callback's existing symbol.
// This declaration adds the independently established nonreturn contract.
__declspec(noreturn) void __cdecl delegate_006fee5f();
}

