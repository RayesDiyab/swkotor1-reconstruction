#include "219cea7e34-helpers.h"
extern "C" __declspec(naked) int32_t __cdecl task8_math_port_00707e17() {
    __asm {
        push ecx
        // FNSTSW word[ESP+disp8(0)]; intentionally no FWAIT before FNCLEX.
        _emit 0DDh
        _emit 07Ch
        _emit 024h
        _emit 000h



        fnclex

        // MOVSX EAX,word[ESP+disp8(0)].
        _emit 00Fh
        _emit 0BFh
        _emit 044h
        _emit 024h
        _emit 000h
        pop ecx
        ret
    }
}
