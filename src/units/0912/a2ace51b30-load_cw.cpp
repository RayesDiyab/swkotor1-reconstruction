#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_load_cw_00704215() {
    __asm {
        // 00704215
        mov         edx,dword ptr [esp+4]
        // 00704219
        and         edx,300h
        // 0070421f
        or          edx,7Fh
        // 00704222
        mov         word ptr [esp+6],dx
        // 00704227
        fldcw       word ptr [esp+6]
        // 0070422b
        ret
    }
}
