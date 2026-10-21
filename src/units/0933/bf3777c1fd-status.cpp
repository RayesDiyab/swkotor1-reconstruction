#include "219cea7e34-helpers.h"
extern "C" __declspec(naked) int32_t __cdecl task8_math_port_00707e0a() {
    __asm {
        push ecx
        fwait
        // FNSTSW word[ESP+disp8(0)], preserving original addressing encoding.
        _emit 0DDh
        _emit 07Ch
        _emit 024h
        _emit 000h



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
