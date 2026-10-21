// lane-audit-r7 r5 unit xc-0073b040-light-saber-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D098 -> 0073B040, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B040 push 73BB10h / 0073B045 call 006FB332 / 0073B04A pop ecx / 0073B04B ret
//   atexit disposer: 0073BB10 mov eax,dword ptr ds:[007FBFF0h] / 0073BB15 test eax,eax / 0073BB17 je 0073BB20 / 0073BB19 push eax / 0073BB1A call 006FA390 / 0073BB1F pop ecx / 0073BB20 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BB10: mov eax,[007FBFF0] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bb10() {
    if (const std::uint32_t data = scene3d_light_saber_bucket_007fbff0.data)                               // 0073BB10 mov eax,[007FBFF0h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b040() {
    return std::atexit(&kotor_xc_dispose_0073bb10);   // 0073B040 push 0073BB10h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d098 {
extern const int root_0073b040;
const int root_0073b040 = kotor_xc_init_0073b040();   // .CRT$XCU record
}
