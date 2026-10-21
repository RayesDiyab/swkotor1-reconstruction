// lane-audit-r7 r5 unit xc-0073b050-static-light-bucket (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D09C -> 0073B050, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B050 push 73BB30h / 0073B055 call 006FB332 / 0073B05A pop ecx / 0073B05B ret
//   atexit disposer: 0073BB30 mov eax,dword ptr ds:[007FBFE4h] / 0073BB35 test eax,eax / 0073BB37 je 0073BB40 / 0073BB39 push eax / 0073BB3A call 006FA390 / 0073BB3F pop ecx / 0073BB40 ret
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
// 0073BB30: mov eax,[007FBFE4] / test eax,eax / je / push eax / call 006FA390 (free) / pop ecx / ret.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bb30() {
    if (const std::uint32_t data = scene3d_static_light_bucket_007fbfe4.data)                               // 0073BB30 mov eax,[007FBFE4h] / test / je
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));   // push eax / call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b050() {
    return std::atexit(&kotor_xc_dispose_0073bb30);   // 0073B050 push 0073BB30h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d09c {
extern const int root_0073b050;
const int root_0073b050 = kotor_xc_init_0073b050();   // .CRT$XCU record
}
