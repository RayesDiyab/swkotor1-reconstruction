// lane-audit-r7 r5 unit xc-0073af30-mesh-buckets (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D070 -> 0073AF30, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073AF30 push 47ABC0h / 0073AF35 push 5C5A30h / 0073AF3A push 1388h / 0073AF3F push 0Ch / 0073AF41 push 8194E0h / 0073AF46 call 006FB55F / 0073AF4B push 73BC30h / 0073AF50 call 006FB332 / 0073AF55 pop ecx / 0073AF56 ret
//   atexit disposer: 0073BC30 push 47ABC0h / 0073BC35 push 1388h / 0073BC3A push 0Ch / 0073BC3C push 8194E0h / 0073BC41 call 006FA852 / 0073BC46 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
extern "C" { void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*)); }  // 006FA852 (strict-link owner)
// 0073AF30: eh_vector_constructor_iterator 006FB55F(008194E0, 0Ch, 1388h, 005C5A30, 0047ABC0). 005C5A30 cannot throw
// (mov eax,ecx / xor ecx,ecx / [eax+4]=0 / [eax+8]=0 / [eax]=0 / ret), so the iterator is its forward loop.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bc30() {
    c2da_destroy_array_006fa852(scene3d_mesh_buckets_008194e0, 0x0C, 0x1388, &exo_array_list_free_0047abc0);   // 0073BC30 push 47ABC0h/1388h/0Ch/008194E0h / call 006FA852 / ret
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073af30() {
    for (std::int32_t index = 0; index < 0x1388; ++index) {      // 006FB55F loop, element size 0Ch
        scene3d_mesh_buckets_008194e0[index].size = 0;                                         // 005C5A34 [eax+4] = 0
        scene3d_mesh_buckets_008194e0[index].capacity = 0;                                     // 005C5A37 [eax+8] = 0
        scene3d_mesh_buckets_008194e0[index].data = 0;                                         // 005C5A3A [eax] = 0
    }
    return std::atexit(&kotor_xc_dispose_0073bc30);   // push 0073BC30h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d070 {
extern const int root_0073af30;
const int root_0073af30 = kotor_xc_init_0073af30();   // .CRT$XCU record
}
