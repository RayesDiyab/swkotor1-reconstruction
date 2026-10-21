// lane-audit-r7 r5 unit xc-0073b0b0-active-bins (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0B4 -> 0073B0B0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B0B0 push 73BC10h / 0073B0B5 call 006FB332 / 0073B0BA pop ecx / 0073B0BB ret
//   atexit disposer: 0073BC10 mov eax,dword ptr ds:[007FBF9Ch] / 0073BC15 test eax,eax / 0073BC17 je 0073BC20 / 0073BC19 push eax / 0073BC1A call 006FA390 / 0073BC1F pop ecx / 0073BC20 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BC10: mov eax,[007FBF9C] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bc10() {
    if (const std::uint32_t data = scene3d_active_bins_007fbf9c.data)                               // 0073BC10 mov eax,[007FBF9Ch] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b0b0() {
    return std::atexit(&kotor_xc_dispose_0073bc10);   // 0073B0B0 push 0073BC10h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0b4 {
extern const int root_0073b0b0;
const int root_0073b0b0 = kotor_xc_init_0073b0b0();   // .CRT$XCU record
}
