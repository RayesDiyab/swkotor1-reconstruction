#pragma once
#include "3b3c807af5-startup_string.h"
#if !defined(KOTOR_STARTUP_STRING_OWNED_COPY_LIFETIME)
#error Left/Right require the coherent parent-owned StartupString copy-constructor shell.
#endif

namespace base_ini_reconstruction {
using String = kotor_reconstruction::StartupString;

// ECX receiver, unused EDX carrier, then output/count on stack (original RET8).
// Output must be fresh String return-object storage; this begins its lifetime.
__declspec(noinline) String* __fastcall left_005e5f70(
    String* self, void* unused_edx, String* output, int count);
__declspec(noinline) String* __fastcall right_005e6170(
    String* self, void* unused_edx, String* output, int count);
} // namespace base_ini_reconstruction
