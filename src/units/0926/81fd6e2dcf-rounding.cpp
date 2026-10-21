// Original 0070A53B: ambient x87 rounding followed by binary64 spill/reload.
// The return is ST0, not a C++ long double (MSVC long double is binary64).
extern "C" __declspec(naked) void __cdecl task8_math_port_0070a53b() {
    __asm {
        push ecx
        push ecx
        fld qword ptr [esp + 12]
        frndint



        fstp qword ptr [esp]
        fld qword ptr [esp]
        pop ecx
        pop ecx






        ret

    }
}
