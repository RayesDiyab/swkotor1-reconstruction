// lane-m48-b crt ADAPTED COPY of task-8 pow-runtime-closure-r1/source/canonical_math_cells.cpp (single owner of the
// original CRT math cells consumed by the task-8 pow ports). Changes vs donor:
//  (1) 007A316C _matherr callback: the donor initialised it with reinterpret_cast<int(__cdecl*)(void*)>(&naked port) -
//      a function-pointer cast (forbidden). Per D24 the cell holds a typed function of the cell's own type that
//      implements the original 00706A22 body exactly (xor eax,eax; ret: argument word ignored, EAX32 = 0, caller
//      cleans). The naked assembly port task8_math_port_00706a22 (direct call at x87_error 0070969x) stays the
//      address-credited body; image value of 007A316C is 00706A22 (dumpbin /rawdata).
//  (2) 00834318 __sse2_available added beside 00834314 __use_sse2_mathfcns (same writer), and the original writer
//      __sse2_available_init 007043C1 (+ its probe 0070438C) is reproduced below. It is the __xi C initializer at slot
//      0078D1A0 of the original (run by _cinit 006FB0E7..006FB10A before the C++ __xc table). It is NOT registered:
//      the R7 initializer audit admits only the 28 allowlisted .CRT$XCU objects. OPEN GATE (crt/FORK-PACKET.json):
//      until an allowlist entry admits it, both cells keep the loader-zero value, so 006FC1F0 always takes the x87
//      _pow_internal path, whereas the original takes __pow_pentium4 (00704439) on SSE2 CPUs whenever MXCSR and the
//      x87 CW have all exceptions masked.
// Cold values: all four .bss/.data words verified with dumpbin /rawdata (00833BDC/00834314/00834318 beyond the .data
// raw end 007A3FFF = loader zero; 007A36D0 = 00002694; 007A316C = 00706A22).
#include "95da263a94-canonical_math_cells.h"
#include <cstdint>
#include <excpt.h>
static_assert(sizeof(unsigned)==4 && sizeof(void*)==4, "original cell width");
static_assert(sizeof(task8_crt_matherr_callback_007a316c)==4, "callback slot width");
extern "C" {
// Typed (D24) equivalent of 00706A22 for the callback cell's own type.
int __cdecl crt_default_matherr_typed_00706a22(void*) { return 0; }
unsigned kotor_crt_pow_sse2_enabled_00834314 = 0;
unsigned crt_sse2_available_00834318 = 0;
unsigned task8_crt_fast_math_enabled_00833bdc = 0;
int (__cdecl* task8_crt_matherr_callback_007a316c)(void*) = &crt_default_matherr_typed_00706a22;
unsigned task8_crt_matherr_user_007a36d0 = 0x2694u;
}
// 0070438C (SEH-framed probe): __try { movapd xmm0,xmm1 (0070439C..007043A0); result=1 } __except(1) { result=0 }.
static int crt_sse2_os_probe_0070438c() {
    int result = 0;                                   // 00704398 and [ebp-1Ch],0
    __try {
        __asm { movapd xmm0, xmm1 }                   // 007043A0
        result = 1;                                   // 007043A4
    } __except (EXCEPTION_EXECUTE_HANDLER) {          // filter 007043AD: xor eax,eax; inc eax
        result = 0;                                   // 007043B1 handler resumes with [ebp-1Ch] still 0
    }
    return result;                                    // 007043B8
}
// 007043C1 __sse2_available_init: CPUID leaf 1 only if EFLAGS.ID toggles (007043CA..007043E2; otherwise EDX = the
// pushfd difference, which is 0 here because the branch is taken only when it is 0); clear both cells (007043E7/EE);
// if EDX bit 26 (SSE2) and the OS probe succeeds, both cells = 1 (00704404..0070440C). Returns 0 (00704411).
extern "C" int __cdecl crt_sse2_available_init_007043c1() {
    unsigned features = 0;
    __asm {
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 200000h
        push eax
        popfd
        pushfd
        pop edx
        sub edx, ecx
        je no_cpuid
        push ecx
        popfd
        push ebx
        mov eax, 1
        cpuid
        pop ebx
    no_cpuid:
        mov features, edx
    }
    crt_sse2_available_00834318 = 0;
    kotor_crt_pow_sse2_enabled_00834314 = 0;
    if ((features & 0x04000000u) != 0 && crt_sse2_os_probe_0070438c() != 0) {
        crt_sse2_available_00834318 = 1;
        kotor_crt_pow_sse2_enabled_00834314 = 1;
    }
    return 0;
}
