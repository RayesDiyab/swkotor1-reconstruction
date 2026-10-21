// lane-m50-heap unit crt-heap-cells: single owner of the original CRT heap cells and of the startup-phase heap
// publication (G-HEAP fix). Original evidence: swkotor.exe sha256 9c10e045..., dumps in lane-m50-heap\evidence-frozen.
//
// What the original does (observed):
//   * WinMainCRTStartup 006FB38D: GetVersionExA(0x94-byte OSVERSIONINFOA) at 006FB3AD; dwPlatformId -> [00833BE8]
//     (006FB3B6), dwMajorVersion -> [00833BF4] (006FB3BF). Then _heap_init(1) at 006FB454..006FB456; a zero result
//     takes fast_error_exit(1Ch) 006FB460..006FB462. Only AFTER that: _mtinit 006FB468, _ioinit 006FB481, ...,
//     _cinit 006FB4C9 (C initializers __xi 006FB0E7..006FB10A, then C++ initializers __xc 006FB11A..006FB138), and
//     WinMain 006FB509. So operator new 006FA7E6 (-> _nh_malloc 006FD8CF(size,1) -> _heap_alloc 006FD854 -> HeapAlloc
//     [00834338]) is usable by every __xi/__xc initializer.
//   * _heap_init 007026DC: HeapCreate(arg==0, 1000h, 0) -> [00834338] (007026ED/007026F5); mode = __heap_select
//     007026C2 -> [0083433C] (00702704); mode 3 only: __sbh_heap_init(3F8h) 00702AE9, on its failure HeapDestroy
//     (00702720) and return 0. Body = task-23 heap-bootstrap6 donor source_heap_init/source_heap_select/
//     source_sbh_heap_init (staged verbatim as unit crt-heap-bootstrap; not re-authored here).
//   * __heap_select 007026C2: [00833BE8]==2 && [00833BF4]>=5 (unsigned) -> 1, else 3.
//   * Teardown: NONE. The only HeapDestroy call site in .text is 00702720 (the _heap_init failure path above; full
//     .text scan for "call [0073D0E8h]"). doexit 006FB13F runs the atexit/_onexit table (0083548C/00835490, reverse,
//     006FB17B..006FB1AC), __xp (0078D1B4..0078D1BC) and __xt (0078D1C0..0078D1C8), then __crtExitProcess 006FB098
//     -> ExitProcess. The CRT heap therefore outlives every atexit disposer and is released only by process exit.
//   * Writers of the cells: [00834338] only 007026F5; [0083433C] only 00702704; [00834324]/[0083431C]/[00834320]/
//     [0083432C]/[00834328]/[00834330] only __sbh_heap_init 00702AE9..00702B25 (mode 3) and the SBH internals;
//     [00833ECC] _newmode and [00833EC8] _pnhNewHandler have NO writer in .text (readers 006FD8FB/006FB8B1/00701C74/
//     00701CBF and 0070360C). All lie in .bss (beyond the .data raw end 007A3FFF) -> loader zero.
//
// What this unit does: owns those cells (constant zero, no dynamic initializer of its own) and provides
// kotor_crt_heap_init_007026dc_phase(), the body registered by unit xi-007026dc-heap-init in the C-initializer
// phase (.CRT$XIB) of the modern CRT - after the modern CRT's own heap/TLS/stdio initialization, before every
// .CRT$XIC and every .CRT$XCU entry, i.e. before all original __xi/__xc initializers, as in the original. It
// reproduces the 006FB3AD..006FB3BF writes, calls the donor _heap_init with the original argument 1, takes the
// original fast_error_exit 006FB369 sequence on failure, and then publishes the unchanged task-23 live allocator
// (live_globals/live_dependencies) onto these cells. Nothing ever destroys the heap (original: none).
#include "55783805f1-heap_bootstrap.h"   // task-23 heap-bootstrap6 donor: source_heap_init 007026DC (unit crt-heap-bootstrap)
#include "218ede3a87-allocator_live.h"   // task-23 sbh-slice2 live allocator: 006FA7E6/006FD8CF/006FD854/006FD8FB/006FB7B2 family
#include "ee6ecb1b67-g11_crt_state.h"    // G11-core: original 006FEF50 TLS bootstrap / 006FB369(10h) failure route
#include <cstring>

extern "C" {
// crt-fatal-cells (m48/b/crt/fatal/crt_fatal.cpp): __error_mode 00833C34 owner and __FF_MSGBANNER 00701EC7 body.
extern volatile int crt_error_mode_00833c34;
void __cdecl crt_ff_msgbanner_00701ec7();
}
namespace kotor_selected_termination_dependencies {
void __cdecl report_runtime_error_00701d50(int);   // crt-fatal-reporter: __NMSG_WRITE 00701D50
}

namespace kotor_crt_heap {
// Original .bss cells (loader zero). Names carry the original addresses; this unit is their only definition.
HANDLE volatile crtheap_00834338 = nullptr;              // _crtheap
std::uint32_t volatile active_heap_0083433c = 0;         // __active_heap
std::uint32_t volatile osplatform_00833be8 = 0;          // _osplatform
std::uint32_t volatile winmajor_00833bf4 = 0;            // _winmajor
std::uint32_t volatile sbh_header_list_00834324 = 0;     // __sbh_pHeaderList
std::uint32_t volatile sbh_header_defer_0083431c = 0;    // __sbh_pHeaderDefer
std::uint32_t volatile sbh_header_count_00834320 = 0;    // __sbh_cntHeaderList
std::uint32_t volatile sbh_header_scan_0083432c = 0;     // __sbh_pHeaderScan
std::uint32_t volatile sbh_threshold_00834328 = 0;       // __sbh_threshold (stays 0 in mode 1, as in the original)
std::uint32_t volatile sbh_header_size_00834330 = 0;     // __sbh_sizeHeaderList
std::uint32_t volatile sbh_group_defer_00834334 = 0;     // __sbh_indGroupDefer
std::uint32_t volatile newmode_00833ecc = 0;             // _newmode (no writer in the original)
NewHandler volatile new_handler_00833ec8 = nullptr;      // _pnhNewHandler (no writer in the original)

// True once the startup-phase publication below succeeded; stays true until process exit (original: no teardown).
bool published() noexcept {
    return crtheap_00834338 != nullptr && active_heap_0083433c == 1u &&
        heap_bootstrap_globals.heap == &crtheap_00834338 && heap_bootstrap_globals.mode == &active_heap_0083433c &&
        live_globals.heap == &crtheap_00834338 && live_globals.mode == &active_heap_0083433c &&
        live_globals.threshold == &sbh_threshold_00834328 && live_globals.new_mode == &newmode_00833ecc &&
        live_globals.handler == &new_handler_00833ec8 && live_globals.header_base == &sbh_header_list_00834324 &&
        live_globals.header_count == &sbh_header_count_00834320 &&
        live_dependencies.heap_alloc == &HeapAlloc && live_dependencies.heap_free == &HeapFree &&
        !live_dependencies.lock && !live_dependencies.unlock &&
        !live_dependencies.sbh_alloc && !live_dependencies.sbh_free;
}
}

namespace {
// 006FB369 fast_error_exit(code): [00833C34]==1 -> __FF_MSGBANNER 00701EC7 (006FB369..006FB372); __NMSG_WRITE(code)
// 00701D50 (006FB377..006FB37B); __crtExitProcess(0FFh) 006FB098 (006FB380..006FB385).
// EQUIVALENCE (__crtExitProcess): 006FB098 calls mscoree!CorExitProcess only when GetModuleHandleA("mscoree.dll")
// succeeds (006FB098..006FB0BB); this native image never loads mscoree, so it reaches ExitProcess(0FFh) (006FB0C1),
// which is called directly here (no FARPROC conversion).
[[noreturn]] void fast_error_exit_006fb369(int code) {
    if (crt_error_mode_00833c34 == 1) crt_ff_msgbanner_00701ec7();
    kotor_selected_termination_dependencies::report_runtime_error_00701d50(code);
    ::ExitProcess(0xFFu);
}
}

extern "C" __declspec(noinline) int __cdecl kotor_crt_heap_init_007026dc_phase() {
    using namespace kotor_crt_heap;
    // Single publication: this body runs once from its .CRT$XIB entry; any prior publication is a composition error.
    if (live_globals.heap || live_globals.mode || live_dependencies.heap_alloc || heap_bootstrap_globals.heap ||
        heap_bootstrap_apis.create || crtheap_00834338) ::ExitProcess(86u);
    // 006FB399..006FB3BF: OSVERSIONINFOA (dwOSVersionInfoSize = 94h) filled by GetVersionExA (result ignored);
    // platform and major version stored to the cells __heap_select reads. (The original also stores minor/build/
    // winver/osver to 00833BEC..00833BF8; no reconstructed reader exists for those, so they are not owned here.)
    OSVERSIONINFOA info;
    std::memset(&info, 0, sizeof info);
    static_assert(sizeof(OSVERSIONINFOA) == 0x94, "original 006FB399 mov edi,94h");
    info.dwOSVersionInfoSize = sizeof info;
#pragma warning(push)
#pragma warning(disable:4996) // GetVersionExA: the original CRT's own source of these words (006FB3AD); same as crt-fatal-message-box
    ::GetVersionExA(&info);
#pragma warning(pop)
    osplatform_00833be8 = info.dwPlatformId;     // 006FB3B3..006FB3B6
    winmajor_00833bf4 = info.dwMajorVersion;     // 006FB3BC..006FB3BF
    // Bind the donor _heap_init to the single cells and to the original imports (IAT 0073D0E4 HeapCreate,
    // 0073D0E8 HeapDestroy, 0073D14C HeapAlloc).
    heap_bootstrap_globals = {&crtheap_00834338, &active_heap_0083433c, &osplatform_00833be8, &winmajor_00833bf4,
                              &sbh_header_list_00834324, &sbh_header_defer_0083431c, &sbh_header_count_00834320,
                              &sbh_header_scan_0083432c, &sbh_threshold_00834328, &sbh_header_size_00834330,
                              &sbh_group_defer_00834334};
    heap_bootstrap_apis = {&HeapCreate, &HeapDestroy, &HeapAlloc};
    if (!source_heap_init(1)) fast_error_exit_006fb369(0x1C);   // 006FB454..006FB462 push 1 / call 007026DC / push 1Ch
    // Mode 3 (small-block heap) is selected only for non-NT or NT < 5 (007026C2); the modern static CRT this image
    // links cannot start on such systems, and the SBH allocator is outside this profile (live_dependencies.sbh_* stay
    // null). Fail closed rather than run 006FD854's mode-3 branch through null callbacks.
    if (active_heap_0083433c != 1u) ::ExitProcess(86u);
    // Publish the unchanged task-23 live allocator onto the original cells: 006FD854 reads [0083433C]/[00834328]/
    // [00834338]; 006FD8FB reads [00833ECC]; 0070360C reads [00833EC8]; free 006FB7B2 reads [0083433C]/[00834338].
    live_globals = {&crtheap_00834338, &active_heap_0083433c, &sbh_threshold_00834328, &newmode_00833ecc,
                    &new_handler_00833ec8, &sbh_header_list_00834324, &sbh_header_count_00834320};
    live_dependencies = {&HeapAlloc, &HeapFree, nullptr, nullptr, nullptr, nullptr};   // IAT 0073D14C / 0073D150
    // G11-core: original 006FB468 _mtinit calls 006FEF50 and, on 0, calls 006FB369(10h) at 006FB473. Run the
    // bootstrap immediately after successful heap publication and before any original compat consumer. The fatal
    // lane owns the bounded second-copy effect; if it returns, the unchanged heap-success return below still runs.
    if (g11_tls_init() == 0) g11_fatal_init(0x10);
    return published() ? 0 : 86;   // _initterm_e: 0 = continue startup
}
