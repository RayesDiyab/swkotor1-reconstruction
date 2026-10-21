// lane-audit-r6allow unit xc-0073b670-vfx-list (authored for allowlist r6; __xc scan item (d)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D180 -> 0073B670, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B670 mov ecx,833BC8h / 0073B675 call 00605800 / 0073B67A push 73C170h / 0073B67F call 006FB332 / 0073B684 pop ecx / 0073B685 ret
//   00605800 CExoLinkedList<CSWBehaviorAttachSound>::CExoLinkedList: new(0Ch) (006FA7E6); non-null -> 005E9AA0;
//            [this] = internal (or 0 on allocation failure)  (00605800..00605863)
//   atexit disposer: 0073C170 mov ecx,[00833BC8h] / test ecx,ecx / push esi / mov esi,ecx / je 0073C18B / call 005E9AB0 /
//            push esi / call 006FA390 / add esp,4 / 0073C18B pop esi / ret   (the cell is not cleared)
// 00833BC8 (4-byte list wrapper, .bss loader zero) has NO storage owner in the M48 candidate (routes-lane
// winmain_routes::Cells::vfx_list_00833bc8 is an unfilled pointer view, see NOTES). This unit defines it, the same
// pattern as r5 xc-0073b6d0-requested-models (00834094). Readers: UnloadCachedVFX 00690910/00690931 (shutdown,
// from CClientExoAppInternal::Uninitialize 005F8474); writers: this initializer, CSWCArea ctor 006061C3/0060622D.
#include "fdfe2df39b-constructors.h"      // kotor_list::SoundList::Construct = 00605800 (owner: canonical-list-constructors)
#include "05a3c5e4fb-allocator_live.h"    // live_free = 006FA390
#include <cstdlib>
namespace base_ini_reconstruction { __declspec(noinline) void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*); }  // ini-list-cleanup
namespace kotor_vfx_list {
// 00833BC8: CExoLinkedList<CSWBehaviorAttachSound> wrapper (4 bytes, .bss zero in the original image).
kotor_list::SoundList list_00833bc8{};
}
// 0073C170: mov ecx,[00833BC8] / test / je -> 005E9AB0(internal) then 006FA390(internal); the cell is not cleared.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073c170() {
    kotor_list::Internal* const internal = kotor_vfx_list::list_00833bc8.internal;          // 0073C170
    if (internal == nullptr) return;                                                         // 0073C176..0073C17B
    base_ini_reconstruction::destroy_list_005e9ab0(internal, nullptr);                        // 0073C17D call 005E9AB0 (ecx = internal)
    live_free(internal);                                                                     // 0073C182..0073C183 call 006FA390
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b670() {
    kotor_vfx_list::list_00833bc8.Construct();                     // 0073B670 mov ecx,833BC8h / call 00605800
    return std::atexit(&kotor_xc_dispose_0073c170);                // push 73C170h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d180 {
extern const int root_0073b670;
const int root_0073b670 = kotor_xc_init_0073b670();   // .CRT$XCU record
}
