// lane-audit-r6allow unit xc-0073b180-retry-log-string (authored for allowlist r6; lane-m49-file NOTES "Shutdown destructor").
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0E0 -> 0073B180, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B180 mov ecx,8328D8h / 0073B185 call 005B3190 / 0073B18A push 73BEF0h / 0073B18F call 006FB332 / 0073B194 pop ecx / 0073B195 ret
//   005B3190 (CExoString::CExoString()): mov eax,ecx / mov [eax],0 / mov [eax+4],0 / ret  (EAX = this)
//   atexit disposer: 0073BEF0 mov ecx,8328D8h / 0073BEF5 jmp 005E5C20 (CExoString::~CExoString)
//   005E5C20: if [this] != 0 { [this+4]=0; free([this]) (006FA390); [this]=0 }
// Storage 008328D8 (8-byte CExoString, .bss loader zero) has ONE owner: lane-r4-lowers r4_cells.cpp
// frame_control::log_request_008328d8 (U32[2]); this unit defines no storage. Readers: CExoFile::Read retry log
// 005EBA40..005EBBC9 (lane-m49-file file_engine_owner.cpp view) and MainLoop log flush 00603FD1..006040C6.
// The typed view of the U32[2] cell as kotor_reconstruction::StartupString is the D24 object-pointer view already
// used by lane-m49-file (file_engine_owner.cpp) and frame-gui active_log.cpp; StartupString() is the stage's
// 005B3190 lifetime entry and ~StartupString() its 005E5C20 entry (same owners gui_suffix_names uses).
#include "397879e70b-startup_string.h"
#include <cstdint>
#include <cstdlib>
#include <new>
namespace frame_control { extern std::uint32_t log_request_008328d8[2]; }   // lane-r4-lowers r4_cells.cpp (single storage)
static_assert(sizeof(kotor_reconstruction::StartupString) == sizeof(frame_control::log_request_008328d8), "008328D8 extent");
namespace kotor_xc_0078d0e0_view {   // named (not anonymous): path-independent symbols, as r5 units
inline kotor_reconstruction::StartupString* retry_log() {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(frame_control::log_request_008328d8);   // D24 view of 008328D8
}
}
// 0073BEF0: mov ecx,8328D8h / jmp 005E5C20.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bef0() {
    kotor_xc_0078d0e0_view::retry_log()->~StartupString();                                     // 0073BEF0..0073BEF5 -> 005E5C20
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b180() {
    ::new (kotor_xc_0078d0e0_view::retry_log()) kotor_reconstruction::StartupString();        // 0073B180 mov ecx,8328D8h / 0073B185 call 005B3190
    return std::atexit(&kotor_xc_dispose_0073bef0);                    // 0073B18A push 73BEF0h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0e0 {
extern const int root_0073b180;
const int root_0073b180 = kotor_xc_init_0073b180();   // .CRT$XCU record
}
