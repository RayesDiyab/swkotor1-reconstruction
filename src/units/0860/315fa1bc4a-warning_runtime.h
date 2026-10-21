#pragma once
#include "fdf62edace-startup_string.h"

// Parent-owned mechanical projections of task19's actual algorithms, on the
// canonical nominal receiver. These are declarations, not callback substitutes.
namespace kotor_reconstruction {
__declspec(noinline) StartupString* __fastcall StartupAssign005e5c50(
    StartupString*, void*, StartupString*);
__declspec(noinline) void __cdecl StartupFormat005e5680(
    StartupString*, const char*, ...);
}
namespace kotor_functional {
const char* __cdecl modern_ini_strrchr(const char*, int);
}
namespace base_ini_reconstruction {
// Exact literal identities; parent supplies one pinned local data definition
// for each original address, with existing 0073D71C ownership reconciled.
extern const char warning_mode_0074e5a8[];
extern const char warning_path_0074e5ac[];
extern const char warning_prefix_0074e5c4[];
extern const char warning_message_format_0074e5e0[];
extern const char warning_source_format_0074e660[];
extern const char warning_empty_0073d71c[];

// Ignored ECX receiver and ignored EDX carrier; three physical stack words.
__declspec(noinline) void __fastcall warning_internal_005ec2b0(
    void*, void*, int line, const char* source, const char* message);
}
