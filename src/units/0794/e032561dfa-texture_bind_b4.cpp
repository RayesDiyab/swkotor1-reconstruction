// texture_call_bind_b4 (fork F2, single owner; decision D21: naked inline asm, no ml.exe).
// Original sites (0042E600: 50 of them, e.g. 0042E7CD..0042E7D3): mov ecx,<texture>;
// mov edx,[ecx]; call [edx+0B4h] -> CAurTexture::Bind (004214B0 in Basic table 0073F160).
// Reloads the receiver's live vtable and tail-jumps the slot with ECX = receiver; zero stack
// words; the RET0 callee returns directly to the C++ caller. No original bytes copied.
#include "ccf5cb551c-render_shared_lowers.h"
extern "C" __declspec(naked) void __fastcall texture_call_bind_b4(texture::Basic*) {
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx+0B4h]
    }
}
