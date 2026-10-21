// G11 fatal lane: original 006FB344 (accepted selected 0109 first copy, reused)
// and the bounded faithful 006FB369 second copy authored here.
//
// Original evidence (swkotor.exe, static):
//   006FB344..006FB368 (37 B, 71FAC908...): cmp [00833C34],1; if 1 call
//     00701EC7; push arg; call 00701D50; push 0FFh; call [007A2760] (aexit
//     _exit 006FB223 -> doexit -> 006FB098); pop ecx; pop ecx; ret.
//   006FB369..006FB38C (36 B, F894484F...): cmp [00833C34],1; if 1 call
//     00701EC7; push arg; call 00701D50; push 0FFh; call 006FB098; pop ecx;
//     pop ecx; ret.
//   006FB098 (96 B, 124768C0...): GetModuleHandleA("mscoree.dll"); if non-null
//     GetProcAddress(module,"CorExitProcess"); if found call it with 0FFh;
//     then ExitProcess(0FFh); int3.  It does NOT FreeLibrary.
//
// The first copy is the accepted selected 0109 wrapper.  This lane does not
// author a second body for it and does not merge 006FB344/006FB369.
// g11_fatal_getter is deliberately NOT noreturn: the selected wrapper has a
// real ret at 006310F0 and the original first copy ends in ret at 006FB368.
//
// Test seam: g11::platform carries the OS calls (and the selected first-copy
// wrapper through platform.fatal_first).  Production core initializes it to the
// real Win32 APIs and to _unresolved_fatal_protected_006FB344.  There is no
// test-mode branch in this file.
#include "238d4f4ecb-g11_crt_state.h"

#include <cstring>

extern "C" {
// Frozen data binding (BINDING-NAMES.md).  This is a declaration only: core
// owns the /alternatename alias _g11_error_mode -> _crt_error_mode_00833c34.
// The selected banner/reporter are reached through g11::platform.banner/report,
// which core binds to _crt_ff_msgbanner_00701ec7 and
// report_runtime_error_00701d50; this lane defines no local provider body.
extern int g11_error_mode;
}

namespace {

// CorExitProcess is a __stdcall managed export taking the exit code.
using cor_exit_process_fn = void(__stdcall*)(int);

// Convert the void* returned by the platform GetProcAddress seam without a
// function-pointer reinterpret_cast.  x86 pointer widths are asserted.
cor_exit_process_fn proc_to_fn(void* proc) noexcept {
    cor_exit_process_fn fn = nullptr;
    static_assert(sizeof(fn) == sizeof(proc), "x86 function/data pointer width");
    std::memcpy(&fn, &proc, sizeof fn);
    return fn;
}

}  // namespace

extern "C" void __cdecl g11_fatal_getter(unsigned code) {
    // Accepted selected 0109 first-copy wrapper, called indirectly through the
    // frozen platform entry so a test can observe the exact 0x10 argument and
    // the non-noreturn return without executing the shipped wrapper.  The
    // production default is the selected _unresolved_fatal_protected_006FB344.
    g11::platform.fatal_first(static_cast<int>(code));
}

extern "C" void __cdecl g11_fatal_init(unsigned code) {
    // Original 006FB369 banner condition (006FB369..006FB372).  platform.banner
    // is core-bound to _crt_ff_msgbanner_00701ec7 and is replaceable only by
    // test-only code before this call.
    if (g11_error_mode == 1) {
        g11::platform.banner();
    }

    // Original 006FB377..006FB37B: report the incoming code, not 0xFF.
    // platform.report is core-bound to report_runtime_error_00701d50(code).
    g11::platform.report(static_cast<int>(code));

    // Original 006FB098: probe mscoree.dll / CorExitProcess, call it if found,
    // then ExitProcess(0xFF).  No FreeLibrary, no direct 006FB098 call, and no
    // noreturn decoration on this function (the original second copy ends ret).
    void* const module = g11::platform.get_module_handle_a("mscoree.dll");
    if (module != nullptr) {
        void* const proc = g11::platform.get_proc_address(module, "CorExitProcess");
        if (proc != nullptr) {
            proc_to_fn(proc)(0xFF);
        }
    }
    g11::platform.exit_process(0xFFu);
}
