// lane-m48-b crt: original CRT fatal-path owners for 006FB344 _amsg_exit (D81(2)). Authored from swkotor.exe.
// Owners (single, constant-initialised, no dynamic initializer):
//   00833C34 __error_mode   int, .bss (beyond .data raw end 007A3FFF) -> loader zero. Readers 006FB344/006FB369/00701D8E/
//            00701EC7; no writer instruction in .text (full-disasm scan for "[00833C34h]," finds none: _set_error_mode is
//            never called by the game).
//   007A2764 __app_type     int, .data image value 00000002 (_GUI_APP). Readers 006FD2B7/006FD33A/00701DA0/00701ED5.
//   00833DB0 _adbgmsg       void(__cdecl*)(), .bss loader zero, read 00701EE8; no writer.
//   007A2760 _aexit_rtn     void(__cdecl*)(int), .data image value 006FB223 (_exit); read 006FB360; no writer.
// Donor cross-check: canonical-inputs/runtime_policy_words.cpp (unlinked task-25 owner proposal) records the same two
// cold values (error mode 0, application type 2).
#include "2513c38919-runtime_reporter.h"
#include <stdlib.h>

extern "C" {
volatile int crt_error_mode_00833c34 = 0;
volatile int crt_app_type_007a2764 = 2;
void (__cdecl* volatile crt_adbgmsg_00833db0)() = nullptr;
void __cdecl crt_exit_006fb223(int code);
void (__cdecl* volatile crt_aexit_rtn_007a2760)(int) = &crt_exit_006fb223;
void __cdecl crt_ff_msgbanner_00701ec7();
}
// Views consumed by the task-25 reporter (runtime_reporter.h): borrowed addresses of the owners above (no publication).
namespace kotor_runtime_reporter_dependencies {
const volatile int* error_mode_00833c34 = &crt_error_mode_00833c34;
const volatile int* application_type_007a2764 = &crt_app_type_007a2764;
}

// 00701EC7 __FF_MSGBANNER (57 bytes):
//   mov eax,[00833C34]; cmp eax,1; je banner; test eax,eax; jne ret; cmp [007A2764],1; jne ret
//   banner: push 0FCh; call 00701D50; mov eax,[00833DB0]; test eax,eax; pop ecx; je +; call eax; push 0FFh; call 00701D50; pop ecx; ret
extern "C" void __cdecl crt_ff_msgbanner_00701ec7() {
    const int mode = crt_error_mode_00833c34;                                          // 00701EC7
    if (mode == 1 || (mode == 0 && crt_app_type_007a2764 == 1)) {                     // 00701ECC..00701EDC
        kotor_selected_termination_dependencies::report_runtime_error_00701d50(0xFC); // 00701EDE..00701EE3
        if (void (__cdecl* const hook)() = crt_adbgmsg_00833db0) hook();              // 00701EE8..00701EF2
        kotor_selected_termination_dependencies::report_runtime_error_00701d50(0xFF); // 00701EF4..00701EF9
    }
}

// 006FB223 _exit(code): push 0; push 1; push code; call 006FB13F doexit(code, quick=1, retcaller=0).
// EQUIVALENCE RECORD (D16/D35 style; typed adapter onto the linked UCRT _exit, which is noreturn):
//   original doexit(code,1,0) (006FB13F..006FB211): _lock(8); if [00833C28]==1 (termination already done) ->
//   TerminateProcess(GetCurrentProcess(),code); [00833C24]=1, [00833C20]=0; quick => atexit/_onexit table and __xp
//   pre-terminators (0078D1B4..0078D1BC) are SKIPPED; __xt terminators 0078D1C0..0078D1C8 run: exactly one entry,
//   00705901 = SetUnhandledExceptionFilter([00833FC8]) restoring the filter the ORIGINAL CRT saved at its startup;
//   retcaller=0 => [00833C28]=1 and ___crtExitProcess 006FB098: CorExitProcess via mscoree if loaded, else
//   ExitProcess(code).
//   UCRT _exit(code): no atexit/onexit, no C/C++ terminators, managed-app CorExitProcess check, ExitProcess(code).
//   Differences: (a) the single original __xt terminator restores a filter only the original CRT startup installs; that
//   startup does not run in this build (modern CRT entry), so there is nothing to restore; (b) the original lock-8 /
//   termination-done re-entry state belongs to the original CRT, which is not the runtime of this build. Observable
//   process effect on this (error-only) path is identical: no user atexit handlers, process exit code = code.
extern "C" void __cdecl crt_exit_006fb223(int code) {
    ::_exit(code);
}
