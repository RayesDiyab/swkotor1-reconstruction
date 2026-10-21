// lane-audit-r7 r5 unit xc-0073b130-grass-info (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0D0 -> 0073B130, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B130 push 73BEB0h / 0073B135 call 006FB332 / 0073B13A pop ecx / 0073B13B ret
//   atexit disposer: 0073BEB0 mov eax,dword ptr ds:[00831E44h] / 0073BEB5 test eax,eax / 0073BEB7 je 0073BEC0 / 0073BEB9 push eax / 0073BEBA call 006FA390 / 0073BEBF pop ecx / 0073BEC0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BEB0: mov eax,[00831E44] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073beb0() {
    if (const std::uint32_t data = scene3d_grass_info_00831e44.data)                               // 0073BEB0 mov eax,[00831E44h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b130() {
    return std::atexit(&kotor_xc_dispose_0073beb0);   // 0073B130 push 0073BEB0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0d0 {
extern const int root_0073b130;
const int root_0073b130 = kotor_xc_init_0073b130();   // .CRT$XCU record
}
