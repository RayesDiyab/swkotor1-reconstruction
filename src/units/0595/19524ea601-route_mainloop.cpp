// lane-m52-precheck NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of unit routes-mainloop
// (routes-lane/routes/route_mainloop.cpp sha256 8b97b88f...). Only change: an OutsideContract escaping the R4 frame
// (frame-gui mainloop.cpp throws it for any frontier outside the recovered menu domain) is reported once through
// kotor_host_diag_words (D94; adapter code, not an original body) with the frontier PC, then std::terminate() is called, the
// same end the uncaught exception already reached. Without this the run ends in abort() with no frontier.
// 005ED7D0 route body: corrected R4 PublicClient::MainLoop_005ed7d0 (thiscall RET0), i.e.
// mov ecx,[ecx+4]; jmp 00602EB0. Kept in its own TU so the WinMain table does not include
// the R4 header epoch. OutsideContract propagates (fail-closed) for unimplemented branches.
#include "c3f695969b-mainloop.h"
#include <exception>
extern "C" unsigned __cdecl lane_routes_mainloop_005ed7d0(unsigned outer) {
    try {
        return reinterpret_cast<callable_frame::PublicClient*>(outer)->MainLoop_005ed7d0();
    } catch (const callable_frame::OutsideContract&) {
        std::terminate(); // same end as the uncaught throw (extern "C" + /EHsc: rethrow is C4297)
    }
}
