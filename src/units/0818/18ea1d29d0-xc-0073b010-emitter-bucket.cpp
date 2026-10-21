// lane-audit-r7 r5 unit xc-0073b010-emitter-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D08C -> 0073B010, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B010 push 73BAB0h / 0073B015 call 006FB332 / 0073B01A pop ecx / 0073B01B ret
//   atexit disposer: 0073BAB0 mov eax,dword ptr ds:[007FBF78h] / 0073BAB5 test eax,eax / 0073BAB7 je 0073BAC0 / 0073BAB9 push eax / 0073BABA call 006FA390 / 0073BABF pop ecx / 0073BAC0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BAB0: mov eax,[007FBF78] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bab0() {
    if (const std::uint32_t data = scene3d_emitter_bucket_007fbf78.data)                               // 0073BAB0 mov eax,[007FBF78h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b010() {
    return std::atexit(&kotor_xc_dispose_0073bab0);   // 0073B010 push 0073BAB0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d08c {
extern const int root_0073b010;
const int root_0073b010 = kotor_xc_init_0073b010();   // .CRT$XCU record
}
