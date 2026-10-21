#include "4583578ecb-feedback_sound_words.h"
#include "b0e1e980f2-m15_l0.h"
extern "C" void* kotor_cell_sound_007a39ec;
namespace feedback {
__declspec(noinline) void __cdecl play_feedback_sound_words(
    void* reference, const void* position, std::uint32_t z_offset_bits) noexcept(false) {
    // 005FDAB7..005FDAC9 / 005FDB9C..005FDBAE copy x, then y, then z as DWORDs.
    // The canonical typed callee takes ten stack words and returns with RET 28h.
    // No C++ float aggregate transport occurs before the original sound provider.
    __asm {
        push 0
        push 0
        push 0
        push 0
        push 16h
        push z_offset_bits
        mov eax, position
        mov edx, dword ptr [eax]
        sub esp, 0Ch
        mov ecx, esp
        mov dword ptr [ecx], edx
        mov edx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [ecx + 4], edx
        mov dword ptr [ecx + 8], eax
        push reference
        mov ecx, dword ptr [kotor_cell_sound_007a39ec]
        call kotor_m15_sound_play_3d_one_shot_005d5e10
    }
}
}
