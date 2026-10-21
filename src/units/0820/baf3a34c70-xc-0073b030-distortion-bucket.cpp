// lane-audit-r7 r5 unit xc-0073b030-distortion-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D094 -> 0073B030, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B030 push 73BAF0h / 0073B035 call 006FB332 / 0073B03A pop ecx / 0073B03B ret
//   atexit disposer: 0073BAF0 mov eax,dword ptr ds:[007FBFC0h] / 0073BAF5 test eax,eax / 0073BAF7 je 0073BB00 / 0073BAF9 push eax / 0073BAFA call 006FA390 / 0073BAFF pop ecx / 0073BB00 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BAF0: mov eax,[007FBFC0] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073baf0() {
    if (const std::uint32_t data = scene3d_distortion_bucket_007fbfc0.data)                               // 0073BAF0 mov eax,[007FBFC0h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b030() {
    return std::atexit(&kotor_xc_dispose_0073baf0);   // 0073B030 push 0073BAF0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d094 {
extern const int root_0073b030;
const int root_0073b030 = kotor_xc_init_0073b030();   // .CRT$XCU record
}
