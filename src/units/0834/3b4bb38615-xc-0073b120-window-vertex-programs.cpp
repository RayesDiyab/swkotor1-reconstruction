// lane-audit-r7 r5 unit xc-0073b120-window-vertex-programs (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-window (D60)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0CC -> 0073B120, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B120 push 73BE70h / 0073B125 call 006FB332 / 0073B12A pop ecx / 0073B12B ret
//   atexit disposer: 0073BE70 mov eax,dword ptr ds:[008305CCh] / 0073BE75 test eax,eax / 0073BE77 je 0073BE80 / 0073BE79 push eax / 0073BE7A call 006FA390 / 0073BE7F pop ecx / 0073BE80 ret
#include <cstdlib>
extern "C" void __cdecl kotor_atexit_0073be70();   // lane-window: vertex-program list 008305CC disposer 0073BE70
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b120() {
    return std::atexit(&kotor_atexit_0073be70);   // 0073B120 push 0073BE70h / call 006FB332 _atexit / pop ecx / ret (EAX = atexit result)
}
namespace kotor_xc_0078d0cc {
extern const int root_0073b120;
const int root_0073b120 = kotor_xc_init_0073b120();   // .CRT$XCU record
}
