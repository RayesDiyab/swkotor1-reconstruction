#pragma once
#include "ed86855822-startup_string.h"
#if !defined(KOTOR_STARTUP_STRING_OWNED_COPY_LIFETIME)
#error Concat requires the canonical explicit owned-copy lifetime shell.
#endif
namespace base_ini_string_joins {
// Support-only construction at the genuine concat copier phase. This is a
// potentially-throwing C++ entry; do not add extern "C" or noexcept.
// ECX is output, EDX is ignored, source is the one stack word; callee RET4.
kotor_reconstruction::StartupString* __fastcall ConstructFreshConcatOutput(
    kotor_reconstruction::StartupString* output, void*,
    const kotor_reconstruction::StartupString* input);
}
