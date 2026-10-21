#pragma once
#include "e53f4d496b-exorand.h"
#include "e38bd534c7-startup_string.h"
#include <cstdint>

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error GetString requires the pinned MSVC x86 cohort.
#endif
#if !defined(KOTOR_STARTUP_STRING_OWNED_COPY_LIFETIME)
#error GetString requires the canonical owned-copy lifetime shell.
#endif

namespace startup_rand {
// ECX is the actual Task17 receiver; EDX is an unused transport slot.
// The fresh output pointer and ushort length occupy two stack words (RET8).
// This begins the output's actual String lifetime through the owned copy shell.
__declspec(noinline) kotor_reconstruction::StartupString* __fastcall GetString005ecea0(
    kotor_reconstruction::CExoRandInternal* self, void* unused_edx,
    kotor_reconstruction::StartupString* output, std::uint16_t length);
}
