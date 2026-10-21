// Phase-1 load-screen thin providers: four exact field setters and the 005D0FC0
// 16-byte naked thunk. Source-only; no compiler/linker/native action by this lane.
//
// The four setters intentionally keep the original unconditional [ecx+4] chain and
// the exact field widths. There is no null guard and no extra [0x7A39FC] load.
#include "b0286adc3f-load_screen_thin.h"

extern "C" void __fastcall load_screen_set_load_from_save_game_004af060(
    void* server, void*, unsigned one) {
    void* const internal =
        *reinterpret_cast<void**>(static_cast<char*>(server) + 4);
    *reinterpret_cast<unsigned*>(static_cast<char*>(internal) + 0x100C4) = one;
}

extern "C" void __fastcall load_screen_set_load_screen_hint_update_timer_005ee040(
    void* internal, void*, unsigned zero) {
    void* const target =
        *reinterpret_cast<void**>(static_cast<char*>(internal) + 4);
    *reinterpret_cast<unsigned*>(static_cast<char*>(target) + 0x488) = zero;
}

extern "C" void __fastcall load_screen_set_gameplay_hint_005ee060(
    void* internal, void*, unsigned hint) {
    void* const target =
        *reinterpret_cast<void**>(static_cast<char*>(internal) + 4);
    *reinterpret_cast<unsigned char*>(static_cast<char*>(target) + 0x490) =
        static_cast<unsigned char>(hint);
}

extern "C" void __fastcall load_screen_set_story_hint_005ee080(
    void* internal, void*, unsigned hint) {
    void* const target =
        *reinterpret_cast<void**>(static_cast<char*>(internal) + 4);
    *reinterpret_cast<unsigned char*>(static_cast<char*>(target) + 0x491) =
        static_cast<unsigned char>(hint);
}

// The genuine 0886 provider's main body is 240 B and ends RET 0xC. This declaration
// exists so the naked thunk can name the import; it is not a definition.
extern "C" int __fastcall recovered_vm_run_script_005d45d0(
    void* internal, void* unused, kotor_reconstruction::StartupString* script,
    unsigned arg1, unsigned arg2);

// 16-byte census slot: 7 B code (mov ecx,[ecx]; jmp rel32) + nine NOPs.
// EDX is untouched. There is no local ret and no extra helper call.
extern "C" __declspec(naked) void __fastcall frame_R4_005d0fc0(
    void*, void*, kotor_reconstruction::StartupString*, unsigned, unsigned) {
    __asm {
        mov ecx, dword ptr [ecx]
        jmp recovered_vm_run_script_005d45d0
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
    }
}
