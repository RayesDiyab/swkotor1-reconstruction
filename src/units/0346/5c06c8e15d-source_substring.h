#pragma once
#include "e38bd534c7-startup_string.h"
#include <cstdint>
#if !defined(KOTOR_STARTUP_STRING_OWNED_COPY_LIFETIME)
#error SubString requires the existing canonical owned-copy lifetime shell.
#endif
namespace startup_rand {
// ECX source, ignored EDX; fresh output/start/length are three stack words.
// Full output pointer EAX; RET12. Output storage must not contain a live String.
__declspec(noinline) kotor_reconstruction::StartupString* __fastcall SubString005e6270(
    kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* output,
    std::int32_t start, std::int32_t length);
}
