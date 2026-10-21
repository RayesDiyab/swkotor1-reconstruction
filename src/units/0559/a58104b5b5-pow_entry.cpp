#include "0396c4a529-x87_exports.h"
// These are unresolved genuine-provider ports, not fallback implementations.
extern "C" unsigned kotor_crt_pow_sse2_enabled_00834314;
// Internal 006FC24D receives exponent in ST0 and its binary64 high word in EAX;
// [ESP+4..19] hold base and exponent binary64. Returns ST0; caller removes 20.
extern "C" void __cdecl task8_pow_internal_006fc24d();
// 00704439 takes two binary64 stack arguments, returns ST0, caller cleans 16.
extern "C" void __cdecl task8_pow_sse2_internal_00704439();

extern "C" __declspec(naked) void __cdecl task8_pow_sse2_bridge_00704420() {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 16
        and esp, -16
        fxch st(1)
        fstp qword ptr [esp]
        fstp qword ptr [esp+8]
        call task8_pow_sse2_internal_00704439
        leave
        ret
    }
}

extern "C" __declspec(naked) void __cdecl genuine_x87_pow_006fc1f0() {
    __asm {



        cmp dword ptr [kotor_crt_pow_sse2_enabled_00834314], 0
        je x87_path
        sub esp, 8
        stmxcsr dword ptr [esp+4]
        mov eax, dword ptr [esp+4]
        and eax, 01f80h
        cmp eax, 01f80h
        jne restore_probe_stack
        fnstcw word ptr [esp]
        mov ax, word ptr [esp]
        and ax, 07fh
        cmp ax, 07fh
    restore_probe_stack:
        lea esp, [esp+8]
        jne x87_path
        jmp task8_pow_sse2_bridge_00704420
    x87_path:
        sub esp, 20
        fxch st(1)
        fstp qword ptr [esp]
        fst qword ptr [esp+8]
        mov eax, dword ptr [esp+12]
        call task8_pow_internal_006fc24d
        add esp, 20
        ret
    }
}
