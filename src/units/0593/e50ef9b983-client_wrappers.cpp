// Authored CClientExoApp / CClientExoAppInternal / CExoMoviePlayer leaves on the WinMain
// frame-1 route set (swkotor.exe 9c10e045...1435; lane-routes/evidence/full.asm.txt).
#include "aefc11afe2-kotor_original_calls.h"
#include "8af36a49a4-allocator_live.h"
#include <cstring>

namespace {
using Word = std::uint32_t;
Word word_at(const void* object, Word offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
void put_word(void* object, Word offset, Word v) { std::memcpy(static_cast<unsigned char*>(object) + offset, &v, 4); }
void* pointer_at(const void* object, Word offset) { return reinterpret_cast<void*>(word_at(object, offset)); }
// Vtable slot 0 of a panel: scalar deleting destructor, thiscall(flags) -> transported in
// the ABI-identical fastcall(this, EDX, flags) form (same as R4 delete_virtual).
using DeletingDestructor = void* (__fastcall*)(void*, void*, Word);
}

// R4 consumes the same body under its own name; one definition, linker alias.
#pragma comment(linker, "/alternatename:@frame_R4_005fca30@12=@kotor_fastcall_005fca30@12")

// 005ED420..005ED427: mov ecx,[ecx+4]; jmp 005FCA30 (stack word forwarded, callee RET4).
extern "C" void __fastcall kotor_fastcall_005ed420(void* outer, void*, std::uint32_t argument) {
    kotor_fastcall_005fca30(pointer_at(outer, 4), nullptr, argument);
}

// 005FCA30..005FCB4E CClientExoAppInternal::DisplayMainMenu(unused), RET4.
// EH: state 0 spans the MainMenu constructor; unwind action 00726560 = _free(allocation)
// (FuncInfo 00777060, one unwind entry) -> live_free, the allocator paired with 006FA7E6.
extern "C" void __fastcall kotor_fastcall_005fca30(void* internal, void*, std::uint32_t) {
    if (void* current = pointer_at(internal, 0x280)) {                                   // 005FCA49
        if (frame_ContainsPanel_0040bd70(static_cast<gui_ctor::Gui*>(pointer_at(internal, 0x274)), nullptr, current)) return; // 005FCA53..005FCA61
    }
    if (void* previous = pointer_at(internal, 0x284)) {                                  // 005FCA67
        if (frame_ContainsPanel_0040bd70(static_cast<gui_ctor::Gui*>(pointer_at(internal, 0x274)), nullptr, previous)) { // 005FCA78
            void* reloaded = pointer_at(internal, 0x284);                                 // 005FCA81
            put_word(reloaded, 0x44, (word_at(reloaded, 0x44) & 0xfffffcffu) | 0x400u);   // 005FCA87..005FCA96
        } else if (void* reloaded = pointer_at(internal, 0x284)) {                       // 005FCA9B..005FCAA3
            const auto destroy = reinterpret_cast<DeletingDestructor>(word_at(pointer_at(reloaded, 0), 0));
            destroy(reloaded, nullptr, 1);                                                // 005FCAA7..005FCAA9
        }
        put_word(internal, 0x284, 0);                                                     // 005FCAAB
    }
    void* allocation = live_operator_new(0x1414);                                         // 005FCABA
    void* menu = nullptr;
    if (allocation) {
        struct FreeOnUnwind {                                                             // EH state 0
            void* block; bool armed;
            ~FreeOnUnwind() { if (armed) live_free(block); }
        } guard{allocation, true};
        menu = kotor_fastcall_0067c4c0(allocation, nullptr,
                                       static_cast<gui_ctor::Gui*>(pointer_at(internal, 0x274))); // 005FCADB
        guard.armed = false;                                                              // state -1 (005FCAEA)
    }
    put_word(internal, 0x280, reinterpret_cast<Word>(menu));                              // 005FCAF2
    kotor_fastcall_006200e0(internal, nullptr, 2, 1);                                     // 005FCAF8
    kotor_fastcall_0040bc70(static_cast<gui_ctor::Gui*>(pointer_at(internal, 0x274)), nullptr,
                            pointer_at(internal, 0x280), 2, 1);                          // 005FCB0E
    frame_R4_005f9af0(internal, nullptr, 1, 0x7f, 2);                                     // 005FCB1B
    if (word_at(internal, 0x494) == 1) put_word(internal, 0x494, 0);                       // 005FCB20..005FCB29
    kotor_game_prepare::SetGlobalUnpauseableState0062b410(pointer_at(internal, 0x40), nullptr, 0);                      // 005FCB38
}

// 005EDFD0..005EDFE2 SetLoadStep(step, value): eax=[ecx+4]; byte [eax+step+3E4h]=low byte of
// value; RET8.
extern "C" void __fastcall kotor_fastcall_005edfd0(void* outer, void*, std::uint32_t step, std::uint32_t value) {
    unsigned char* const internal = static_cast<unsigned char*>(pointer_at(outer, 4));
    internal[step + 0x3e4] = static_cast<unsigned char>(value);
}

// 005EDB40..005EDB47: mov ecx,[ecx+4]; jmp 005F2BD0.
extern "C" std::uint32_t __fastcall kotor_fastcall_005edb40(void* outer, void*) {
    return kotor_fastcall_005f2bd0(pointer_at(outer, 4), nullptr);
}
// 005F2BD0..005F2BE1: ecx=[ecx+12Ch]; null -> xor eax,eax; else jmp 0070C1F0.
extern "C" std::uint32_t __fastcall kotor_fastcall_005f2bd0(void* internal, void*) {
    void* const player = pointer_at(internal, 0x12c);
    if (!player) return 0;
    return static_cast<std::uint32_t>(kotor_fastcall_0070c1f0(player, nullptr));
}
// 0070C1F0..0070C1F6 CExoMoviePlayer::IsMoviePlaying: mov ecx,[ecx]; jmp 00404DD0.
extern "C" int __fastcall kotor_fastcall_0070c1f0(void* player, void*) {
    return static_cast<int>(kotor_fastcall_00404dd0(pointer_at(player, 0), nullptr));
}
// 00404DD0 GetProperty0x30: single body = existing M43 provider @c2da_current_resource_data_00404dd0@8 (tpc-lowers; D65 C2);
// joined in m45-joins/integrator_joins_m45.cpp (fastcall @8 both).

// 005EE1F0..005EE1F7: mov ecx,[ecx+4]; jmp 005F4DB0.
extern "C" void __fastcall kotor_fastcall_005ee1f0(void* outer, void*) {
    kotor_fastcall_005f4db0(pointer_at(outer, 4), nullptr);
}
// 005F4DB0..005F4DE4: if [+12C] && IsMoviePlaying(player) return; if [+200]: [+200]=0 then
// ShowMouse(this, 8) (store precedes the call, 005F4DD4/005F4DDE).
extern "C" void __fastcall kotor_fastcall_005f4db0(void* internal, void*) {
    if (void* player = pointer_at(internal, 0x12c)) {
        if (kotor_fastcall_0070c1f0(player, nullptr)) return;
    }
    if (word_at(internal, 0x200)) {
        put_word(internal, 0x200, 0);
        kotor_game_prepare::ShowMouse0061fa00(internal, nullptr, 8);
    }
}
