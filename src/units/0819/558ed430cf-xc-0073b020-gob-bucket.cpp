// lane-audit-r7 r5 unit xc-0073b020-gob-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D090 -> 0073B020, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B020 push 73BAD0h / 0073B025 call 006FB332 / 0073B02A pop ecx / 0073B02B ret
//   atexit disposer: 0073BAD0 mov eax,dword ptr ds:[007FBFCCh] / 0073BAD5 test eax,eax / 0073BAD7 je 0073BAE0 / 0073BAD9 push eax / 0073BADA call 006FA390 / 0073BADF pop ecx / 0073BAE0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BAD0: mov eax,[007FBFCC] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bad0() {
    if (const std::uint32_t data = scene3d_gob_bucket_007fbfcc.data)                               // 0073BAD0 mov eax,[007FBFCCh] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b020() {
    return std::atexit(&kotor_xc_dispose_0073bad0);   // 0073B020 push 0073BAD0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d090 {
extern const int root_0073b020;
const int root_0073b020 = kotor_xc_init_0073b020();   // .CRT$XCU record
}
