#include "19ac7cee16-aur_make_current.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
// Reconstruction of AurMakeCurrent 0044ED90 (swkotor.exe 9c10e045...1435).
// Evidence: lane-routes/evidence (dumpbin /disasm of 0044ED90..0044EE00) and the
// independent saved listing 2026-10-06/task-8/texture-scheduling-evidence/0044ed90.txt.
// Constants are the original .rdata values: 0073D700=0.0f, 0073D6FC=1000.0f,
// 0073D770=4294967296.0f, 00741548=0.0009765625f (1/1024). IAT 0073D1C0=GlobalMemoryStatus.
namespace {
const float zero_0073d700 = 0.0f;
const float thousand_0073d6fc = 1000.0f;
const float two32_0073d770 = 4294967296.0f;
const float kib_00741548 = 0.0009765625f;

// 0044ED90..0044EDA4: fld [7B9270]; fcomp [0.0]; fnstsw; test ah,44h; jnp skip.
// Busy-wait unless the x87 compare reports exactly "equal" (C3=1,C2=0). Unordered
// (NaN) and less/greater all wait, exactly as the parity test does.
bool delay_requested(const float volatile* delay) {
    std::uint32_t requested;
    __asm {
        mov ecx, delay
        fld dword ptr [ecx]
        fcomp dword ptr zero_0073d700
        fnstsw ax
        test ah, 44h
        setpe al
        movzx eax, al
        mov requested, eax
    }
    return requested != 0;
}

// 0044EDA6..0044EDB9: fld [7B9270]; fmul [1000.0]; call 006FAE8C; mov edi,eax.
// Uses the existing FTOL provider body with the live x87 state (no C++ conversion).
std::int32_t delay_ticks(const float volatile* delay) {
    std::int32_t ticks;
    __asm {
        mov ecx, delay
        fld dword ptr [ecx]
        fmul dword ptr thousand_0073d6fc
        call unresolved_ftol2_006fae8c
        mov ticks, eax
    }
    return ticks;
}

// 0044EDDA..0044EDF2: fild dword dwAvailPhys; if signed<0 fadd 2^32; fmul 1/1024;
// fstp [7B9274]. Kept on x87 so the single final rounding follows the live control word.
void publish_available_kib(const MEMORYSTATUS* status, float volatile* bias) {
    __asm {
        mov edx, status
        fild dword ptr [edx]MEMORYSTATUS.dwAvailPhys
        mov ecx, dword ptr [edx]MEMORYSTATUS.dwAvailPhys
        test ecx, ecx
        jge positive
        fadd dword ptr two32_0073d770
    positive:
        fmul dword ptr kib_00741548
        mov eax, bias
        fstp dword ptr [eax]
    }
}
}

extern "C" void __cdecl frame_RenderBoundary_0044ed90() {
    const auto& c = aur_make_current::cells;
    if (delay_requested(c.delay_007b9270)) {
        // 0044EDB4..0044EDCB: ftol first, then clock; target = clock()+ticks (32-bit
        // wrap, ADD ESI,EDI); poll clock while target > now (signed JG).
        const std::int32_t ticks = delay_ticks(c.delay_007b9270);
        const std::uint32_t target_bits = static_cast<std::uint32_t>(crt_clock_006fc546()) + static_cast<std::uint32_t>(ticks);
        const std::int32_t target = static_cast<std::int32_t>(target_bits);
        while (target > static_cast<std::int32_t>(crt_clock_006fc546())) {
        }
    }
    // 0044EDCF..0044EDD4: GlobalMemoryStatus(&local) with dwLength left unwritten,
    // exactly as the original 0x24-byte frame (the API sets dwLength itself).
    MEMORYSTATUS status;
    GlobalMemoryStatus(&status);
    publish_available_kib(&status, c.bias_007b9274);
    // 0044EDF8..0044EDFB: release frame, JMP 00421EE0. The scheduler's EAX is dead at
    // every original caller, so the consumer-declared void shape drops it.
    (void)texture_make_current_00421ee0();
}
