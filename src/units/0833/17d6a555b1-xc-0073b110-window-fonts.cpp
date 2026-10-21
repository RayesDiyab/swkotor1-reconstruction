// lane-audit-r7 r5 unit xc-0073b110-window-fonts (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-window (D60)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0C8 -> 0073B110, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B110 push 73BE50h / 0073B115 call 006FB332 / 0073B11A pop ecx / 0073B11B ret
//   atexit disposer: 0073BE50 mov eax,dword ptr ds:[0083052Ch] / 0073BE55 test eax,eax / 0073BE57 je 0073BE60 / 0073BE59 push eax / 0073BE5A call 006FA390 / 0073BE5F pop ecx / 0073BE60 ret
#include <cstdlib>
extern "C" void __cdecl kotor_atexit_0073be50();   // lane-window: fonts list 0083052C disposer 0073BE50
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b110() {
    return std::atexit(&kotor_atexit_0073be50);   // 0073B110 push 0073BE50h / call 006FB332 _atexit / pop ecx / ret (EAX = atexit result)
}
namespace kotor_xc_0078d0c8 {
extern const int root_0073b110;
const int root_0073b110 = kotor_xc_init_0073b110();   // .CRT$XCU record
}
