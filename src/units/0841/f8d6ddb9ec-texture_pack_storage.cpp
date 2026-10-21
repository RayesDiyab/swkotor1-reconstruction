// lane-resman: CAuroraInterface texture-pack name storage 00834098 (4 x CExoString,
// stride 8, .bss loader zero-fill in the original image) and its CRT initializer
// root 0073B6F0 / exit callback 0073C190 (initializer table slot 0078D18C).
// Same exported names as the unreviewed 2026-10-02 task-34 cpp-roots-80-82 donor so
// exactly one of the two may be admitted; this version needs no vector-iterator helpers.
#include "c534281817-startup_string.h"
#include <cstdlib>
#include <new>

extern "C" {
alignas(4) unsigned char Receiver_00834098[32] = {};               // 00834098..008340B7, zero-fill
__declspec(noinline) int __cdecl Init_0073b6f0();
__declspec(noinline) void __cdecl Dispose_0073c190();
}

namespace {
kotor_reconstruction::StartupString* slot(unsigned index) {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(Receiver_00834098 + index * 8u);
}
}

// 0073C190: eh_vector_destructor_iterator(00834098, 8, 4, 005E5C20) -> reverse order.
extern "C" __declspec(noinline) void __cdecl Dispose_0073c190() {
    for (unsigned index = 4; index-- > 0;) slot(index)->~StartupString();
}

// 0073B6F0: eh_vector_constructor_iterator(00834098, 8, 4, 005B3190, 005E5C20) forward
// (005B3190 cannot throw: two zero stores), then _atexit(0073C190) (006FB332); EAX = atexit result.
extern "C" __declspec(noinline) int __cdecl Init_0073b6f0() {
    for (unsigned index = 0; index < 4; ++index) ::new (slot(index)) kotor_reconstruction::StartupString();
    return std::atexit(&Dispose_0073c190);
}

// The original runs 0073B6F0 from the CRT _initterm table (slot 0078D18C); a C++ dynamic
// initializer is the modern CRT's _initterm entry for the same role.
// Named namespace so the .CRT$XCU initializer symbol is path-independent (lane-audit-r7 allowlist).
namespace kotor_texture_packs { extern const int texture_packs_root_0078d18c; const int texture_packs_root_0078d18c = Init_0073b6f0(); }
