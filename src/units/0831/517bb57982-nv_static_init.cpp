// nv_static_init.cpp - lane-3d-model rev2 (D66, gate G-NV-INIT): the one static initializer of the NvTriStrip
// closure, kept in its own unit for the static-initializer allowlist.
// Original: 0073B0E0 is entry __xc_a[48] at 0078D0C0 of the initializer table 0078D000..0078D190 walked by the CRT
// at 006FB11A..006FB136; it registers atexit(0073BCB0) (destructor of the 008302A8 face list).
// Here the table slot is the compiler-emitted .CRT$XCU pointer to this unit's dynamic initializer for
// nv_static_init::registered_0073b0e0.
#include "2645a3bdc3-nv_strip.hpp"
#include <cstdlib>

// 0073B0E0 static initializer: atexit(0073BCB0) (0073B0E0..0073B0E5; 006FB332 _atexit -> CRT atexit).
void __cdecl nv_static_init_faces_to_delete_0073b0e0() {
    std::atexit(&nv_dispose_faces_to_delete_0073bcb0);
}

namespace nv_static_init {
extern const int registered_0073b0e0;
const int registered_0073b0e0 = (nv_static_init_faces_to_delete_0073b0e0(), 0);
}
