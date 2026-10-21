// Phase-2 A-directed thunks. Source-only; no compiler/linker/native action.
//
// Each body is exactly 8 bytes: mov ecx,[ecx+4] + jmp rel32.
// There is no local frame, no local ret, and no extra dereference/call.
#include "92bc5955d7-load_screen_a_thunks.h"

// A's accepted @kotor_fastcall_005eeab0@16 takes ECX plus a 12-byte aggregate on
// the stack (three dwords, RET 0xC). The aggregate is local to this declaration; it
// only shapes the C-linkage symbol/parameter passing and is never laid out elsewhere.
struct ALoadGameArgs {
    unsigned int arg1;
    unsigned int arg2;
    unsigned int arg3;
};

// Accepted lane-A providers; these are declarations, not definitions.
extern "C" unsigned __fastcall kotor_fastcall_005eeab0(
    void* internal, ALoadGameArgs args);
extern "C" void __fastcall kotor_fastcall_005f4ae0(
    void* internal);

extern "C" __declspec(naked) void __fastcall
load_screen_send_load_game_request_005ed680(
    void*, void*, int, void*, void*) {
    __asm {
        mov ecx, dword ptr [ecx + 4]
        jmp kotor_fastcall_005eeab0
    }
}

extern "C" __declspec(naked) void __fastcall
load_screen_busy_wait_mute_sound_005ee130(void*, void*) {
    __asm {
        mov ecx, dword ptr [ecx + 4]
        jmp kotor_fastcall_005f4ae0
    }
}
