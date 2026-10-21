// lane-audit-r7 r5 unit xc-0073b090-fading-in-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0AC -> 0073B090, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B090 push 73BBB0h / 0073B095 call 006FB332 / 0073B09A pop ecx / 0073B09B ret
//   atexit disposer: 0073BBB0 mov eax,dword ptr ds:[007FBFD8h] / 0073BBB5 test eax,eax / 0073BBB7 je 0073BBC0 / 0073BBB9 push eax / 0073BBBA call 006FA390 / 0073BBBF pop ecx / 0073BBC0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BBB0: mov eax,[007FBFD8] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bbb0() {
    if (const std::uint32_t data = scene3d_fading_in_bucket_007fbfd8.data)                               // 0073BBB0 mov eax,[007FBFD8h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b090() {
    return std::atexit(&kotor_xc_dispose_0073bbb0);   // 0073B090 push 0073BBB0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0ac {
extern const int root_0073b090;
const int root_0073b090 = kotor_xc_init_0073b090();   // .CRT$XCU record
}
