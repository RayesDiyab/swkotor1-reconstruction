// m46_ptd_join.cpp - lane-m46-3dmodel: compat_getptd (zero original-address credit; TRANSPORT JOIN, integrator decision).
// Consumer: M45 3d/model/model_lifetime.cpp:233 `extern "C" void* __cdecl compat_getptd();` used by the D52 single RNG
// provider crt_rand_006fc447 (original CRT rand 006FC447: holdrand at _getptd()+14h).
// The donor named by the consumer (task-35 heap-tls-shared-composition helpers.cpp, the historical 0x88-byte TLS PTD) is
// an EXCLUDED unit of the M45 stage ("Historical 0x88 PTD and lock/bootstrap internals excluded"), so it is not staged.
// The M45 stage's accepted per-thread PTD owner is locale-publisher (locale/0 modern_locale.cpp LocaleScope::thread,
// "supported modern actual PTD transport, no original body credit" - m24-review/9-HANDOFF.json), published as the typed
// cell kotor_locale::dependencies.getptd_006feedf (locale/1 locale.h:21). This adapter forwards to that same owner, so
// every 006FEEDF consumer that uses the published dependency shares ONE PTD per thread (typed call, no casts; the
// Ptd* -> void* conversion is an implicit object-pointer conversion).
// OPEN GATES (recorded in PACKET.json, not resolved here):
//  G-PTD-1 cold holdrand: the original _getptd/_initptd stores PTD+14h = 1 for a fresh PTD (task-35 helpers.cpp line
//          p[0x14/4]=1 reproduces it); LocaleScope value-initialises its Ptd (+14h = 0). Equivalent only if srand
//          (006FC43A) runs on the thread before the first rand.
//  G-PTD-2 srand route: M41 services_tail5::Seed (006FC43A) obtains its PTD through bindings.dispatch(0x006feedf); the
//          integrator must route that target to the same owner (kotor_locale::dependencies.getptd_006feedf) or the
//          menu's srand seeds a different object than crt_rand_006fc447 reads (D52).
#include "ce2a7ca4fe-locale.h"   // M45 stage locale/1/source (kotor_locale::Ptd, kotor_locale::dependencies)
#include "ee6ecb1b67-g11_crt_state.h"   // G11 core: the single original 0x88 PTD owner

// G11-core successor (2026-10-18): the modern-locale cold-seed path and its
// thread_local flag are removed. The four original-layout compat consumers now
// share the one typed original PTD; creation owns +14h=1 once, so later lookups
// never reseed. LocaleScope/Ptd+64 remain untouched and unused here.
extern "C" void* __cdecl compat_getptd() {
    return g11_getptd();
}
