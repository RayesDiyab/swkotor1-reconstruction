// lane-audit-r7 r5 unit xc-0073afe0-static-shadow-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D080 -> 0073AFE0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073AFE0 push 73BA50h / 0073AFE5 call 006FB332 / 0073AFEA pop ecx / 0073AFEB ret
//   atexit disposer: 0073BA50 mov eax,dword ptr ds:[007FBF84h] / 0073BA55 test eax,eax / 0073BA57 je 0073BA60 / 0073BA59 push eax / 0073BA5A call 006FA390 / 0073BA5F pop ecx / 0073BA60 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BA50: mov eax,[007FBF84] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073ba50() {
    if (const std::uint32_t data = scene3d_static_shadow_bucket_007fbf84.data)                               // 0073BA50 mov eax,[007FBF84h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073afe0() {
    return std::atexit(&kotor_xc_dispose_0073ba50);   // 0073AFE0 push 0073BA50h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d080 {
extern const int root_0073afe0;
const int root_0073afe0 = kotor_xc_init_0073afe0();   // .CRT$XCU record
}
