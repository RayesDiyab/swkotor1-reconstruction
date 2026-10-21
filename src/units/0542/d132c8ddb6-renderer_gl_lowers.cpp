// Authored lower bodies (lane-renderer-bodies).
#include "f90e611d0e-renderer_gl_cells.h"
#include "0f48c8b46c-check_extension.h"
#include <cstring>
namespace kotor_renderer_init {
// 0046ED70 AurIsAAliasingAvailable (0x23 bytes, no args, plain ret).
// 0046ED70 reads cache 0078E5D8; any value other than FFFFFFFF is returned as is
// (full EAX). Cold path: ECX=mask 0078E4EC (0046ED7A), EAX=caps 007BB85C
// (0046ED80), (caps&mask)==mask -> 1 else 0 via and/sub/neg/sbb/inc
// (0046ED85..0046ED8D), stored to the cache (0046ED8E) and returned.
// Declared __stdcall by the reviewed aa_query.h consumer; with no stack
// arguments the physical contract equals the original's plain ret.
I32 __stdcall AntiAliasingAvailable_0046ed70() {
    using namespace kotor_renderer_gl_cells;
    U32 value = *static_cast<volatile U32*>(&cache_antialias_0078e5d8);
    if (value == 0xffffffffu) {
        const U32 mask = *static_cast<volatile U32*>(&mask_multisample_0078e4ec);
        const U32 caps = *static_cast<volatile U32*>(&capabilities_007bb85c);
        value = ((caps & mask) == mask) ? 1u : 0u;
        *static_cast<volatile U32*>(&cache_antialias_0078e5d8) = value;
    }
    return static_cast<I32>(value);
}
// 0045F9B0 pushes the immediate address 0073D5D8, an .rdata " " literal.
const char* const token_delimiter_0073d5d8 = " ";
// 006FCB40 _strcspn and 006FAFF0 _strncmp are statically linked MSVC CRT library
// functions (Ghidra LIBRARY_FUNCTION="y"); the cdecl callers (0045F9B6, 0045F9CB)
// pop their own arguments. The pinned libcmt/libucrt functions provide them.
U32 __cdecl StringCSpan_006fcb40(const char* text, const char* delimiters) {
    return static_cast<U32>(std::strcspn(text, delimiters));
}
I32 __cdecl StringNCompare_006faff0(const char* first, const char* second, U32 count) {
    return std::strncmp(first, second, count);
}
}
