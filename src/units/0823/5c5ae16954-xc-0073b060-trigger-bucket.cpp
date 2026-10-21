// lane-audit-r7 r5 unit xc-0073b060-trigger-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0A0 -> 0073B060, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B060 push 73BB50h / 0073B065 call 006FB332 / 0073B06A pop ecx / 0073B06B ret
//   atexit disposer: 0073BB50 mov eax,dword ptr ds:[0080AA74h] / 0073BB55 test eax,eax / 0073BB57 je 0073BB60 / 0073BB59 push eax / 0073BB5A call 006FA390 / 0073BB5F pop ecx / 0073BB60 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BB50: mov eax,[0080AA74] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bb50() {
    if (const std::uint32_t data = scene3d_trigger_bucket_0080aa74.data)                               // 0073BB50 mov eax,[0080AA74h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b060() {
    return std::atexit(&kotor_xc_dispose_0073bb50);   // 0073B060 push 0073BB50h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0a0 {
extern const int root_0073b060;
const int root_0073b060 = kotor_xc_init_0073b060();   // .CRT$XCU record
}
