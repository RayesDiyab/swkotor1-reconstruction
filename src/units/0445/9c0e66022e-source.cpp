#include <cstdint>
// Exact observed body: C3. It reads no register/stack input and has no side effect.
// Historical C++ prototype is ambiguous; no meaningful return value is invented.
extern "C" __declspec(naked) void __cdecl renderer_void_004015a0(){



    // MSVC inline `ret` emits C2 0000 here. Emit the observed C3 explicitly.
    __asm { _emit 0xc3 }
}
