#pragma once
#include <cstdint>
#include <cstddef>
namespace kotor_signal {
using Word=std::uint32_t;
struct Action {Word exception_code;std::int32_t signal;Word handler;};
struct PtdView {
 unsigned char opaque[0x54];
 Action* volatile actions;
 void* volatile exception;
 std::int32_t volatile fpe_code;
};
struct Globals {
 Word volatile *signal2,*signal21,*signal22,*signal15;
 Word volatile *fpe_first,*fpe_count;
};
extern const Globals globals; // Existing shared storage must supply these aliases.
static_assert(sizeof(void*)==4 && sizeof(Action)==12,"original x86 words/record");
static_assert(offsetof(PtdView,actions)==0x54 && offsetof(PtdView,exception)==0x58 && offsetof(PtdView,fpe_code)==0x5c,"observed PTD fields only");
}
extern "C" void* __cdecl compat_getptd(); // Existing task35 provider, not defined here.
namespace kotor_signal_dependencies {
// Typed source boundary. Original00705B5F reads table inEDX and signal inESI;
// its saved ECX/EDX fastcall prototype is not an original ABI adapter.
kotor_signal::Action* __cdecl lookup_00705b5f(kotor_signal::Action*,int);
void __cdecl lock_00701a74(int);
void __cdecl unlock_007019e0(int);
}
namespace kotor_selected_termination_dependencies {
// Exact accepted task25 caller symbol, with original full-width EAX result.
int __cdecl raise_signal_00705b8d(int);
__declspec(noreturn) void __cdecl quick_exit_006fb223(int); // Unimplemented here.
}
