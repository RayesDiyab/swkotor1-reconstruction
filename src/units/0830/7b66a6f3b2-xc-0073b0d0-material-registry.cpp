// lane-audit-r7 r5 unit xc-0073b0d0-material-registry (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-startup (D71)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0BC -> 0073B0D0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B0D0 push 73BC90h / 0073B0D5 call 006FB332 / 0073B0DA pop ecx / 0073B0DB ret
//   atexit disposer: 0073BC90 mov eax,dword ptr ds:[00830298h] / 0073BC95 test eax,eax / 0073BC97 je 0073BCA0 / 0073BC99 push eax / 0073BC9A call 006FA390 / 0073BC9F pop ecx / 0073BCA0 ret
#include <cstdlib>
namespace material_owner { void __cdecl dispose_0073bc90(); }   // lane-m46-startup material_lowers.cpp (0073BC90)
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b0d0() {
    return std::atexit(&material_owner::dispose_0073bc90);   // 0073B0D0 push 73BC90h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0bc {
extern const int root_0073b0d0;
const int root_0073b0d0 = kotor_xc_init_0073b0d0();   // .CRT$XCU record
}
