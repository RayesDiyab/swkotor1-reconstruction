// lane-audit-r7 r5 unit xc-0073b0c0-old-active-bins (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0B8 -> 0073B0C0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B0C0 push 73BBF0h / 0073B0C5 call 006FB332 / 0073B0CA pop ecx / 0073B0CB ret
//   atexit disposer: 0073BBF0 mov eax,dword ptr ds:[007FBFFCh] / 0073BBF5 test eax,eax / 0073BBF7 je 0073BC00 / 0073BBF9 push eax / 0073BBFA call 006FA390 / 0073BBFF pop ecx / 0073BC00 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BBF0: mov eax,[007FBFFC] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bbf0() {
    if (const std::uint32_t data = scene3d_old_active_bins_007fbffc.data)                               // 0073BBF0 mov eax,[007FBFFCh] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b0c0() {
    return std::atexit(&kotor_xc_dispose_0073bbf0);   // 0073B0C0 push 0073BBF0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0b8 {
extern const int root_0073b0c0;
const int root_0073b0c0 = kotor_xc_init_0073b0c0();   // .CRT$XCU record
}
