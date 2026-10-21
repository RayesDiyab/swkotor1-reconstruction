// lane-audit-r7 r5 unit xc-0073aef0-hit-info (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D064 -> 0073AEF0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073AEF0 push 0 / 0073AEF2 push 0BFFFFFFFh / 0073AEF7 push 0 / 0073AEF9 mov ecx,7B93A8h / 0073AEFE call 004578F0 / 0073AF03 push 73B9B0h / 0073AF08 call 006FB332 / 0073AF0D pop ecx / 0073AF0E ret
//   atexit disposer: 0073B9B0 mov ecx,7B93A8h / 0073B9B5 jmp 00456B30
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073b9b0() {
    chit_info_destruct_00456b30(&scene_hit_info_007b93a8, nullptr);   // 0073B9B0 mov ecx,007B93A8h / jmp 00456B30 (CHitInfo::~CHitInfo)
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073aef0() {
    aurora_hit_info_construct_004578f0(&scene_hit_info_007b93a8, nullptr, 0u, 0xBFFFFFFFu, 0u);   // 0073AEF0 push 0 / push BFFFFFFFh / push 0 / mov ecx,007B93A8h / call 004578F0
    return std::atexit(&kotor_xc_dispose_0073b9b0);   // push 0073B9B0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d064 {
extern const int root_0073aef0;
const int root_0073aef0 = kotor_xc_init_0073aef0();   // .CRT$XCU record
}
