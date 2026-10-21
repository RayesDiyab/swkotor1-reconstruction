#include "0396c4a529-x87_exports.h"
extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
// Task25 owns the actual 117-byte provider. This alias cannot narrow its result
// or spill ST0, and introduces no additional return address or stack cleanup.
extern "C" __declspec(naked) void __cdecl genuine_x87_ftol_006fae8c() {
    __asm {





        jmp unresolved_ftol2_006fae8c
    }
}
