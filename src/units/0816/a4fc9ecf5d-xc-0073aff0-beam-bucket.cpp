// lane-audit-r7 r5 unit xc-0073aff0-beam-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D084 -> 0073AFF0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073AFF0 push 73BA70h / 0073AFF5 call 006FB332 / 0073AFFA pop ecx / 0073AFFB ret
//   atexit disposer: 0073BA70 mov eax,dword ptr ds:[007FBF6Ch] / 0073BA75 test eax,eax / 0073BA77 je 0073BA80 / 0073BA79 push eax / 0073BA7A call 006FA390 / 0073BA7F pop ecx / 0073BA80 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BA70: mov eax,[007FBF6C] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073ba70() {
    if (const std::uint32_t data = scene3d_beam_bucket_007fbf6c.data)                               // 0073BA70 mov eax,[007FBF6Ch] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073aff0() {
    return std::atexit(&kotor_xc_dispose_0073ba70);   // 0073AFF0 push 0073BA70h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d084 {
extern const int root_0073aff0;
const int root_0073aff0 = kotor_xc_init_0073aff0();   // .CRT$XCU record
}
