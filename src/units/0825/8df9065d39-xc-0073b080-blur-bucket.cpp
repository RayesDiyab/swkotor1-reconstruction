// lane-audit-r7 r5 unit xc-0073b080-blur-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0A8 -> 0073B080, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B080 push 73BB90h / 0073B085 call 006FB332 / 0073B08A pop ecx / 0073B08B ret
//   atexit disposer: 0073BB90 mov eax,dword ptr ds:[007FBF90h] / 0073BB95 test eax,eax / 0073BB97 je 0073BBA0 / 0073BB99 push eax / 0073BB9A call 006FA390 / 0073BB9F pop ecx / 0073BBA0 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BB90: mov eax,[007FBF90] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bb90() {
    if (const std::uint32_t data = scene3d_blur_bucket_007fbf90.data)                               // 0073BB90 mov eax,[007FBF90h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b080() {
    return std::atexit(&kotor_xc_dispose_0073bb90);   // 0073B080 push 0073BB90h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0a8 {
extern const int root_0073b080;
const int root_0073b080 = kotor_xc_init_0073b080();   // .CRT$XCU record
}
