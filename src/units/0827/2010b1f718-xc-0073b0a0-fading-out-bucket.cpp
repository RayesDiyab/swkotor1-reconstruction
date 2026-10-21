// lane-audit-r7 r5 unit xc-0073b0a0-fading-out-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0B0 -> 0073B0A0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B0A0 push 73BBD0h / 0073B0A5 call 006FB332 / 0073B0AA pop ecx / 0073B0AB ret
//   atexit disposer: 0073BBD0 mov eax,dword ptr ds:[007FBFA8h] / 0073BBD5 test eax,eax / 0073BBD7 je 0073BBE0 / 0073BBD9 push eax / 0073BBDA call 006FA390 / 0073BBDF pop ecx / 0073BBE0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BBD0: mov eax,[007FBFA8] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bbd0() {
    if (const std::uint32_t data = scene3d_fading_out_bucket_007fbfa8.data)                               // 0073BBD0 mov eax,[007FBFA8h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b0a0() {
    return std::atexit(&kotor_xc_dispose_0073bbd0);   // 0073B0A0 push 0073BBD0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0b0 {
extern const int root_0073b0a0;
const int root_0073b0a0 = kotor_xc_init_0073b0a0();   // .CRT$XCU record
}
