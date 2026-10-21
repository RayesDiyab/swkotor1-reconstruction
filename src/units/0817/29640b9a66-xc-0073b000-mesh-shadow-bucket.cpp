// lane-audit-r7 r5 unit xc-0073b000-mesh-shadow-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D088 -> 0073B000, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B000 push 73BA90h / 0073B005 call 006FB332 / 0073B00A pop ecx / 0073B00B ret
//   atexit disposer: 0073BA90 mov eax,dword ptr ds:[007FBFB4h] / 0073BA95 test eax,eax / 0073BA97 je 0073BAA0 / 0073BA99 push eax / 0073BA9A call 006FA390 / 0073BA9F pop ecx / 0073BAA0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BA90: mov eax,[007FBFB4] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073ba90() {
    if (const std::uint32_t data = scene3d_mesh_shadow_bucket_007fbfb4.data)                               // 0073BA90 mov eax,[007FBFB4h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b000() {
    return std::atexit(&kotor_xc_dispose_0073ba90);   // 0073B000 push 0073BA90h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d088 {
extern const int root_0073b000;
const int root_0073b000 = kotor_xc_init_0073b000();   // .CRT$XCU record
}
