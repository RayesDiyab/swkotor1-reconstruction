// lane-audit-r7 r5 unit xc-0073b6d0-requested-models (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-startup (D71; the __xc entry nearest the requested 0073B6D1)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D188 -> 0073B6D0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B6D0 mov ecx,834094h / 0073B6D5 call 0070D2D0 / 0073B6DA push 73C1B0h / 0073B6DF call 006FB332 / 0073B6E4 pop ecx / 0073B6E5 ret
//   atexit disposer: 0073C1B0 mov ecx,dword ptr ds:[00834094h] / 0073C1B6 test ecx,ecx / 0073C1B8 push esi / 0073C1B9 mov esi,ecx / 0073C1BB je 0073C1CB / 0073C1BD call 005E9AB0 / 0073C1C2 push esi / 0073C1C3 call 006FA390 / 0073C1C8 add esp,4 / 0073C1CB pop esi / 0073C1CC ret
#include "65c9c926f6-constructors.h"      // kotor_list::ModelList / Internal (0070D2D0 / 005E9AA0 owner: canonical-list-constructors)
#include "39c670fc66-allocator_live.h"    // live_free = 006FA390
#include <cstdlib>
namespace base_ini_reconstruction { __declspec(noinline) void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*); }  // ini-list-cleanup (List = kotor_list::Internal)
namespace kotor_requested_models {
// 00834094: requested-model list wrapper (4 bytes, .bss zero in the original image). No other unit defines it (M46 stage).
kotor_list::ModelList list_00834094{};
}
// 0073C1B0: mov ecx,[00834094] / test / je -> 005E9AB0(internal) then 006FA390(internal); the cell is not cleared.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073c1b0() {
    kotor_list::Internal* const internal = kotor_requested_models::list_00834094.internal;   // 0073C1B0
    if (internal == nullptr) return;                                                        // 0073C1B6..0073C1BB
    base_ini_reconstruction::destroy_list_005e9ab0(internal, nullptr);                       // 0073C1BD call 005E9AB0 (ecx = internal)
    live_free(internal);                                                                    // 0073C1C2..0073C1C3 call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b6d0() {
    kotor_requested_models::list_00834094.Construct();            // 0073B6D0 mov ecx,834094h / call 0070D2D0
    return std::atexit(&kotor_xc_dispose_0073c1b0);                // push 73C1B0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d188 {
extern const int root_0073b6d0;
const int root_0073b6d0 = kotor_xc_init_0073b6d0();   // .CRT$XCU record
}
