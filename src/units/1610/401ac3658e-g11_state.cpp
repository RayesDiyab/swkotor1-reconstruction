// G11-core original CRT per-thread state.
//
// Original ownership (swkotor.exe):
//   006FEEDF  lazy getter: save LastError -> TlsGetValue -> if null allocate a
//             zeroed 0x88 PTD -> TlsSetValue -> failure calls 006FB344(0x10),
//             which may return -> restore LastError -> return the (possibly
//             null) PTD. Creation writes +54, +14=1, +0=tid, +4=-1.
//   006FEF50  startup init: TlsAlloc -> index cell -> same 0x88 allocation ->
//             TlsSetValue -> same creation stores -> 1. Any failure runs the
//             006FEEC1 rollback (TlsFree + index=-1, never frees the PTD) and
//             returns 0.
//   006FEEC1  cleanup: TlsFree the current index when it is not -1, then set it
//             to -1. No PTD free and no normal-teardown caller.
//   007A29B0  TLS index cell, initial 0xFFFFFFFF.
//
// Chosen bounded providers: the existing published CRT heap cell
// (kotor_crt_heap::crtheap_00834338) with real HeapAlloc(heap,HEAP_ZERO_MEMORY,
// 0x88), real OS TLS, the accepted 0109 first-copy fatal for getter failure and
// the fatal lane's bounded second-copy init fatal. The old lock table, SBH
// calloc and general CRT allocation policy are deliberately not part of G11.
#include "13bcb552ba-g11_crt_state.h"

// Include layout.h BEFORE <windows.h>: signal_dispatch.h declares
// kotor_signal::Action::exception_code, and the Windows exception_code function
// macro would rewrite that member before layout.h's own #undef runs. layout.h
// parses signal_dispatch.h before runtime_reporter.h includes <windows.h>.
#include "6fb268be1e-layout.h"   // kotor_reporter_action_data::tables (selected 0727 owner)

#include <windows.h>

namespace kotor_crt_heap {
// Same cell published by the selected 0006 startup-phase heap owner.
extern HANDLE volatile crtheap_00834338;
}

namespace {

// Original 007A29B0: one process-wide TLS index word, loader/initial value -1.
// Not a magic constant: g11_tls_init stores TlsAlloc's result here.
unsigned long g11_tls_index_007a29b0 = 0xFFFFFFFFu;

// Original 006FB823(1,0x88) narrowed to the chosen fixed-size call. The
// HEAP_ZERO_MEMORY flag is the original's mode-1 path; HeapAlloc's LastError
// effect and null-on-OOM behavior are preserved. The PTD is never freed.
void* g11_alloc_ptd() {
    HANDLE const heap = kotor_crt_heap::crtheap_00834338;
    return g11::platform.heap_alloc(heap, HEAP_ZERO_MEMORY, 0x88);
}

// Original creation stores, in instruction order: +54 first, +14=1,
// GetCurrentThreadId, +4=-1, +0=tid. Unwritten bytes stay zero.
void g11_init_ptd(g11::Ptd* ptd) {
    ptd->actions = kotor_reporter_action_data::tables.actions;
    ptd->holdrand = 1u;
    const unsigned long tid = g11::platform.get_current_thread_id();
    ptd->thread_sentinel = -1;
    ptd->tid = static_cast<std::uint32_t>(tid);
}

}  // namespace

extern "C" __declspec(noinline) int __cdecl g11_tls_init() {
    const unsigned long index = g11::platform.tls_alloc();
    g11_tls_index_007a29b0 = index;                 // 006FEF62 mov [007A29B0],eax
    if (index == TLS_OUT_OF_INDEXES) {              // 006FEF64 cmp eax,-1
        g11_tls_cleanup();                          // 006FEF69
        return 0;
    }
    g11::Ptd* const ptd = static_cast<g11::Ptd*>(g11_alloc_ptd());
    if (!ptd) {                                     // 006FEF7E test esi,esi
        g11_tls_cleanup();                          // 006FEFB6
        return 0;
    }
    if (!g11::platform.tls_set(index, ptd)) {       // 006FEF8D
        g11_tls_cleanup();                          // 006FEFB6; PTD leaked by design
        return 0;
    }
    g11_init_ptd(ptd);
    return 1;
}

extern "C" void __cdecl g11_tls_cleanup() {
    // 006FEEC1: only the index is rolled back; the PTD is never freed. The
    // original lock-table clear is intentionally omitted (no old lock table).
    const unsigned long index = g11_tls_index_007a29b0;
    if (index != TLS_OUT_OF_INDEXES) {
        g11::platform.tls_free(index);
        g11_tls_index_007a29b0 = TLS_OUT_OF_INDEXES;
    }
}

extern "C" __declspec(noinline) g11::Ptd* __cdecl g11_getptd() {
    const unsigned long saved = g11::platform.get_last_error();  // saved across the original body
    g11::Ptd* ptd = static_cast<g11::Ptd*>(g11::platform.tls_get(g11_tls_index_007a29b0));
    if (!ptd) {
        ptd = static_cast<g11::Ptd*>(g11_alloc_ptd());
        if (!ptd) {
            g11_fatal_getter(0x10);                 // 006FEF3E; returns when it returns
        } else if (!g11::platform.tls_set(g11_tls_index_007a29b0, ptd)) {
            g11_fatal_getter(0x10);                 // allocated PTD is leaked
        } else {
            g11_init_ptd(ptd);
        }
    }
    g11::platform.set_last_error(saved);
    return ptd;
}

extern "C" int* __cdecl g11_errno() {
    // Original 006FFE78: call 006FEEDF; add eax,8; ret. No null guard.
    return &g11_getptd()->errno_value;
}
