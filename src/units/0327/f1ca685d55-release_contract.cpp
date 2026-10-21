// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit aggregate-release-contract
// (release_contract.cpp sha 27fe8978...). Only change: kotor_host_diag* report calls (D94 proposal). The
// stderr text, TerminateProcess code (90..95) and __fastfail fallback are unchanged.
#include "af34ecd272-release_contract.h"
#include "4e2136d0b8-functional_runtime.h"
#include "490217376c-live_publication.h"
#include "05a3c5e4fb-allocator_live.h"
#include <intrin.h>

namespace kotor_functional {
[[noreturn]] void fail_closed_release(ReleaseRefusal reason) noexcept {
    // Avoid CRT exit/abort handlers and stack unwinding: no owner or heap may
    // be torn down following a failed drain. Reporting failure cannot resume.
    static const char text[]="KOTOR source host: release postcondition refused; ownership retained.\r\n";
    // w0 = refusal code (90..95), w1 = return address inside the caller of fail_closed_release.
    ;
    DWORD written=0;
    WriteFile(GetStdHandle(STD_ERROR_HANDLE),text,sizeof(text)-1,&written,nullptr);
    TerminateProcess(GetCurrentProcess(),static_cast<unsigned>(reason));
    __fastfail(7); // Nonreturning fallback if the supported OS call fails.
}
void release_game_checked(PreparedGame* prepared) noexcept {
    if(!prepared || !source_heap_identity()) {
        ;
        fail_closed_release(ReleaseRefusal::InvalidMetadata);
    }
    kotor_functional_game_release(prepared);
    if(prepared->lifetime || prepared->window || prepared->start) {
        ;
        fail_closed_release(ReleaseRefusal::PreparedOwnershipRetained);
    }
    const auto* a=kotor_menu_window_attachment;
    if(!a || kotor_menu_window_publication!=&a->publication ||
       a->phase!=kotor_live_publication::Phase::Empty || a->views ||
       a->reinit || a->message || a->retiring_reinit || a->retiring_message ||
       a->quiescing || a->publication.winmain || a->publication.shell ||
       a->publication.enter_reinit || a->publication.leave_reinit ||
       a->publication.enter_message || a->publication.leave_message ||
       a->providers.owner || a->providers.ready || a->providers.acquire_reinit ||
       a->providers.release_reinit || a->providers.acquire_message ||
       a->providers.release_message || a->providers.quiesce ||
       a->prior_winmain || a->prior_shell || a->prior_reinit || a->prior_message) {
        ;
        fail_closed_release(ReleaseRefusal::WindowOwnershipRetained);
    }
    if(!aggregate_lifetime_idle()) {
        ;
        fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
    }
}
void require_empty_source_heap(HANDLE heap) noexcept {
    if(!source_heap_identity() || !heap || !live_globals.heap ||
       *live_globals.heap!=heap || !HeapValidate(heap,0,nullptr)) {
        ;
        fail_closed_release(ReleaseRefusal::HeapInspectionFailed);
    }
    PROCESS_HEAP_ENTRY entry{};
    while(HeapWalk(heap,&entry)) {
        if(entry.wFlags&PROCESS_HEAP_ENTRY_BUSY) {
            // w0 = first busy block address, w1 = its size: identifies the leaked owner.
            ;
            fail_closed_release(ReleaseRefusal::HeapAllocationsRetained);
        }
    }
    if(GetLastError()!=ERROR_NO_MORE_ITEMS) {
        ;
        fail_closed_release(ReleaseRefusal::HeapInspectionFailed);
    }
}
}
