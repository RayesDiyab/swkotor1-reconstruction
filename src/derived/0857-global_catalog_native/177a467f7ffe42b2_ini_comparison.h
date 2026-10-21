#pragma once
#include "e1805047c9b7563b_startup_string.h"
#include <cstdint>
namespace ini_comparison {
// Original ECX String receiver; one CString stack word; RET4; full BOOL EAX.
// The unused EDX slot supplies a typed free-function physical thiscall carrier.
__declspec(noinline) std::int32_t __fastcall CompareNC005e58c0(
    kotor_reconstruction::StartupString*,void*,const char*);
}
