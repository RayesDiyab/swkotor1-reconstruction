// lane-m48-b crt ADAPTED COPY of task-31 security-cookie/cookie.cpp (original cookie cell 007A3180, init 0070A988,
// check 00705D37, failure 00705D06). Changes vs donor (recorded in crt/FORK-PACKET.json):
//  (1) the two API tables were initialised with addresses of __declspec(dllimport) functions, which MSVC emits as a
//      DYNAMIC initializer (.CRT$XCU) - not admitted by the R7 audit. They are now initialised with this unit's own
//      typed WINAPI forwarders (same signatures, constant addresses): constant initialisation, same calls.
//  (2) cookie_failure_api.report was nullptr (the donor trapped with ExitProcess(86) when the cookie check failed; a
//      D76-style unfilled binding). It now holds the genuine port of 0070A9DE __security_error_handler (authored
//      below from swkotor.exe), the callee of 00705D1A.
// legacy_initialize_cookie (0070A988, original __xc slot 0078D004) is still NOT registered (not an allowlisted
// initializer): 007A3180 keeps its image value BB40E64E. Every check compares against the same cell, so the pow
// error ports' cookie stores/checks stay consistent (open gate: cookie entropy initialisation not reproduced).
#include "274d20d3c9-cookie.h"
#include "2513c38919-runtime_reporter.h"   // lane-m48-b crt/fatal: 00701300/00705910/00705920/006FAB80/007092DE typed owners
#include <intrin.h>
#include <malloc.h>
#include <stdlib.h>
namespace {
void WINAPI entropy_time(LPFILETIME t) { ::GetSystemTimeAsFileTime(t); }
DWORD WINAPI entropy_process() { return ::GetCurrentProcessId(); }
DWORD WINAPI entropy_thread() { return ::GetCurrentThreadId(); }
DWORD WINAPI entropy_tick() {
#pragma warning(suppress:28159)
    return ::GetTickCount();
}
BOOL WINAPI entropy_counter(LARGE_INTEGER* c) { return ::QueryPerformanceCounter(c); }
void WINAPI terminate_process(UINT code) { ::ExitProcess(code); }
}
extern "C" {
// 00834078 user security-error handler (_set_security_error_handler): .bss loader zero; no writer in .text.
void (__cdecl* volatile crt_security_user_handler_00834078)(int, void*) = nullptr;
void __cdecl crt_security_error_handler_0070a9de(int code, int data);
}
// 0070A9DE __security_error_handler(code, data), authored from swkotor.exe 0070A9DE..0070AB27 (SEH frame 006FF00C,
// state 0 around the user handler). 0070A9ED..0070A9F5 (cookie ^ return address into a local) has no reader in the
// body and is not reproduced.
extern "C" void __cdecl crt_security_error_handler_0070a9de(int code, int data) {
    namespace d = kotor_runtime_reporter_dependencies;
    if (void (__cdecl* const handler)(int, void*) = crt_security_user_handler_00834078) {      // 0070A9F8..0070AA01
        __try { handler(code, reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<unsigned>(data)))); } // 0070AA06..0070AA0C
        __except (EXCEPTION_EXECUTE_HANDLER) {}                                                // filter 0070AA19: EAX=1
        ::_exit(3);                                                                             // 0070AA14 -> 0070AB20
    }
    const char* title; const char* text; unsigned text_length;
    if (code == 1) { title = "Buffer overrun detected!";                                        // 0070AA3B [0075F960]
        text = "A buffer overrun has been detected which has corrupted the program's\ninternal state.  The program cannot safely continue execution and must\nnow be terminated.\n"; // [0075F8C0]
        text_length = 0xB9; }                                                                   // 0070AA47
    else { title = "Unknown security failure detected!";                                       // 0070AA28 [0075FA34]
        text = "A security error of unknown cause has been detected which has\ncorrupted the program's internal state.  The program cannot safely\ncontinue execution and must now be terminated.\n"; // [0075F980]
        text_length = 0xD4; }                                                                   // 0070AA34
    char path[0x105];
    path[0x104] = 0;                                                                            // 0070AA4C mov [ebp-24h],cl
    if (::GetModuleFileNameA(nullptr, path, 0x104) == 0)                                       // 0070AA4F..0070AA64
        d::string_copy_00705910(path, "<program name unknown>");                              // 0070AA66..0070AA72 [0075A8C8]
    char* display = path;                                                                       // 0070AA79
    if (d::string_length_00701300(path) + 0xB > 0x3C) {                                        // 0070AA7F..0070AA92
        display = path + d::string_length_00701300(path) - 0x31;                               // 0070AA94..0070AAAB
        d::string_copy_n_006fab80(display, "...", 3);                                          // 0070AAAD..0070AAB5 [0075A8C4]
    }
    const unsigned size = (static_cast<unsigned>(d::string_length_00701300(display)) + text_length + 0xC + 3) & ~3u; // 0070AABD..0070AACB
    char* const out = static_cast<char*>(_alloca(size));                                      // 0070AACE _alloca_probe 006FC090
    d::string_copy_00705910(out, title);                                                       // 0070AAD8..0070AADA
    d::string_concat_00705920(out, "\n\n");                                                   // 0070AADF..0070AAE6 [00748DD0]
    d::string_concat_00705920(out, "Program: ");                                               // 0070AAEB..0070AAF1 [0075F8B4]
    d::string_concat_00705920(out, display);                                                   // 0070AAF6..0070AAF8
    d::string_concat_00705920(out, "\n\n");                                                   // 0070AAFD..0070AAFF
    d::string_concat_00705920(out, text);                                                      // 0070AB04..0070AB08
    d::message_box_007092de(out, "Microsoft Visual C++ Runtime Library", 0x12010u);            // 0070AB0D..0070AB18 [0075A880]
    ::_exit(3);                                                                                 // 0070AB20..0070AB22 call 006FB223 (_exit)
}
CookieEntropyApi cookie_entropy{&entropy_time,&entropy_process,&entropy_thread,&entropy_tick,&entropy_counter};
CookieFailureApi cookie_failure_api{&crt_security_error_handler_0070a9de,&terminate_process};
extern "C" std::uint32_t legacy_security_cookie=0xbb40e64eu;
// Explicit integer/register sequence preserves original unconditional reads,
// API order, zero-only fallback, EAX and arithmetic flags. No GS option disabled.
extern "C" __declspec(naked) std::uint32_t __cdecl legacy_initialize_cookie(){
 __asm {




  push ebp
  mov ebp,esp
  sub esp,16
  push esi
  lea eax,[ebp-8]
  push eax
  call dword ptr [cookie_entropy]
  mov esi,[ebp-4]
  xor esi,[ebp-8]
  call dword ptr [cookie_entropy+4]
  xor esi,eax
  call dword ptr [cookie_entropy+8]
  xor esi,eax
  call dword ptr [cookie_entropy+12]
  xor esi,eax
  lea eax,[ebp-16]
  push eax
  call dword ptr [cookie_entropy+16]
  mov eax,[ebp-12]
  xor eax,[ebp-16]
  xor esi,eax
  mov legacy_security_cookie,esi
  jne initialized

  mov legacy_security_cookie,0bb40e64eh

initialized:
  pop esi
  leave
  ret




 }
}
extern "C" __declspec(naked) void __fastcall legacy_check_cookie(std::uint32_t){
 __asm {
  cmp ecx,legacy_security_cookie



  je matched
  jmp legacy_cookie_failure

matched:
  ret
 }
}
extern "C" void __cdecl legacy_cookie_failure(){
 // Missing report source is an integration error. It never becomes a no-op
 // report or a successful cookie check. This guard is outside accepted inputs.
 if(!cookie_failure_api.report||!cookie_failure_api.terminate)::ExitProcess(86);
 __try { cookie_failure_api.report(1,0); }
 __except(EXCEPTION_EXECUTE_HANDLER) {}



 cookie_failure_api.terminate(3);

 __debugbreak(); // exact terminal trap if a declared noreturn dependency returns
}
