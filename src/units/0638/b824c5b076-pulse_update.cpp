#include "9342737640-pulse_owner_request.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pulse proposal requires the actual reviewed MSVC x86 ABI
#endif

// Exact saved arithmetic/order, expressed through genuine typed owner borrows.
// All six new aliases are declaration-only Root obligations. Their backing
// objects and the existing zero/one aliases must be live and fixed for the call.
extern "C" __declspec(noinline) void __cdecl
GuiPulseUpdate00414a50(float elapsed) noexcept {
    using namespace kotor_pulse_canonical_required;
    volatile float* const timer = &timer_0078d3d0;
    volatile unsigned int* const output_bits = &producer_bits_0078d3d4;
    volatile unsigned int* const direction = &direction_007a4578;
    const volatile float* const zero =
        &kotor_gui_canonical_required::zero_0073d700;
    const volatile float* const one =
        &kotor_gui_canonical_required::one_0073d764;
    const volatile float* const low_input = &low_0073e320;
    const volatile float* const period = &period_0073e31c;
    const volatile float* const factor = &factor_0073e318;

    // The MSVC inline-assembler memory boundary writes actual object
    // representations. FSTP32 targets the unsigned-int bit carrier directly;
    // no float/U32 pointer cast, float reference alias or numeric conversion.
    // FST timer WITHOUT POP leaves the extended subtraction for FCOMP.
    // No FPU initialization/control/status clearing is introduced.
    __asm {
        mov edx, timer
        fld dword ptr [edx]
        fsub elapsed
        fst dword ptr [edx]
        mov ecx, zero
        fcomp dword ptr [ecx]
        fnstsw ax
        test ah, 041h
        jp pulse_reset_skipped
        mov ecx, direction
        mov ecx, dword ptr [ecx]
        xor eax, eax
        test ecx, ecx
        sete al
        mov edx, timer
        mov dword ptr [edx], 03f400000h
        mov edx, direction
        mov dword ptr [edx], eax
pulse_reset_skipped:
        mov ecx, direction
        mov eax, dword ptr [ecx]
        test eax, eax
        je pulse_direction_zero
        mov ecx, one
        fld dword ptr [ecx]
        mov ecx, low_input
        fld dword ptr [ecx]
        jmp pulse_interpolate
pulse_direction_zero:
        mov ecx, low_input
        fld dword ptr [ecx]
        mov ecx, one
        fld dword ptr [ecx]
pulse_interpolate:
        mov ecx, period
        fld dword ptr [ecx]
        mov edx, timer
        fsub dword ptr [edx]
        mov ecx, factor
        fmul dword ptr [ecx]
        fxch st(2)
        fsub st, st(1)
        fmulp st(2), st
        fxch st(1)
        fadd st, st(1)
        mov edx, output_bits
        fstp dword ptr [edx]
        fstp st(0)
    }
}
