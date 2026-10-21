// lane-m48-b fork CLIENT, unit client-pause: client pause-state bodies (R4 suffix SetAutoPaused 005F3F10, r4-lowers
// EnterControllerPauseState 0062F590) and their leaves.
// Evidence: dumpbin /disasm swkotor.exe 005F3F10..005F40D9, 005F2E10..005F2EC8, 005F2DF0..005F2E06, 005EDC10/005EDC20,
// 005DF570/005DF580 (thunks), 005DFFE0/005DFFF0, 0062F590..0062F5ED. .rdata 0073D700 = 0.0f; sound-mode words
// 0074C5EC (0) / 0074C5F4 (2) borrowed from sound-owners (external after lane-m48-b fix/sound/sound_owners.cpp).
// FCOM tests: `test ah,41h; jne` is taken for <=, == or unordered, so the fall-through is the ordered `>` case.
#include "e6446a7354-r4_lowers.h"

using namespace r4_lowers;

namespace table29_required { void __fastcall gui_sound_mode_005d5e80(void* outer, void*, std::uint32_t mode); }
extern "C" {
unsigned __fastcall frame_GetPauseState_004ae980(void*, void*, unsigned);   // CServerExoApp::GetPauseState (r4-lowers)
extern void* kotor_cell_sound_007a39ec;
extern const std::uint32_t kotor_rdata_sound_mode_0074c5ec;
extern const std::uint32_t kotor_rdata_sound_mode_0074c5f4;
void __fastcall kotor_fastcall_005dffe0(void* internal, void*);
void __fastcall kotor_fastcall_005dfff0(void* internal, void*);
void __fastcall kotor_fastcall_005df570(void* input, void*);
void __fastcall kotor_fastcall_005df580(void* input, void*);
unsigned __fastcall kotor_fastcall_005f2df0(void* internal, void*);
unsigned __fastcall kotor_fastcall_005edc10(void* client, void*);
void __fastcall kotor_fastcall_005f2e10(void* internal, void*, U32 paused, U32 reason, U32 force);
void __fastcall kotor_fastcall_005edc20(void* client, void*, U32 paused, U32 reason, U32 force);
void __fastcall frame_SetAutoPaused_005f3f10(menu_continuation::Client*, void*, unsigned, unsigned);
}

namespace {
constexpr float k_zero_0073d700 = 0.0f;
void* server() { return pw(menu_continuation::app_manager_007a39fc, 8); }
U32 server_paused() { void* s = server(); return s ? frame_GetPauseState_004ae980(s, nullptr, 2) : 0; }
void sound_mode(U32 mode) { table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, mode); }
}

// 005DFFE0 / 005DFFF0 CExoInputInternal::Pause / Unpause: [[this+140h]+10h] = 1 / 0.
extern "C" void __fastcall kotor_fastcall_005dffe0(void* internal, void*) { sw(pw(internal, 0x140), 0x10, 1); } // 005DFFE0..005DFFE6
extern "C" void __fastcall kotor_fastcall_005dfff0(void* internal, void*) { sw(pw(internal, 0x140), 0x10, 0); } // 005DFFF0..005DFFF6
// 005DF570 / 005DF580 CExoInput::Pause / Unpause: mov ecx,[ecx+4]; jmp internal.
extern "C" void __fastcall kotor_fastcall_005df570(void* input, void*) { kotor_fastcall_005dffe0(pw(input, 4), nullptr); } // 005DF570..005DF573
extern "C" void __fastcall kotor_fastcall_005df580(void* input, void*) { kotor_fastcall_005dfff0(pw(input, 4), nullptr); } // 005DF580..005DF583

// 005F2DF0 CClientExoAppInternal::GetPausedByCombat: server ? GetPauseState(2) : 0 (receiver unused).
extern "C" unsigned __fastcall kotor_fastcall_005f2df0(void*, void*) { return server_paused(); } // 005F2DF0..005F2E06
extern "C" unsigned __fastcall kotor_fastcall_005edc10(void* client, void*) { return kotor_fastcall_005f2df0(pw(client, 4), nullptr); } // 005EDC10

// 005F2E10 CClientExoAppInternal::SetPausedByCombat(paused, reason byte, force), RET 0Ch.
extern "C" void __fastcall kotor_fastcall_005f2e10(void* internal, void*, U32 paused, U32 reason, U32 force) {
    const U32 state = w(pw(internal, 0x40), 0xb4);                      // 005F2E13..005F2E16
    const bool proceed = (state == 1 && paused == 0) || state == 0 || force == 1; // 005F2E1C..005F2E33
    if (!proceed) return;
    const U32 current = server_paused();                                // 005F2E39..005F2E4F
    const U32 flags = w(internal, 0x37c);                               // 005F2E51
    if ((flags & 4) != 0 || current == paused) return;                  // 005F2E57..005F2E5E
    sw(internal, 0x37c, flags | 4);                                     // 005F2E64 / 005F2E6A
    bytes(internal)[0x388] = static_cast<unsigned char>(reason);        // 005F2E60 / 005F2E70
    sw(internal, 0x380, paused);                                        // 005F2E76
    if (paused == 1) {                                                  // 005F2E67 / 005F2E82
        kotor_fastcall_005df570(kotor_menu_window_cells::input_007a39e4, nullptr); // 005F2E7C..005F2E84
        sound_mode(kotor_rdata_sound_mode_0074c5f4);                    // 005F2E89..005F2E95
        sw(internal, 0x4c4, paused);                                    // 005F2E9A
    } else {
        kotor_fastcall_005df580(kotor_menu_window_cells::input_007a39e4, nullptr); // 005F2EA5
        sound_mode(kotor_rdata_sound_mode_0074c5ec);                    // 005F2EAA..005F2EB7
        sw(internal, 0x4c4, 0);                                         // 005F2EBC
    }
}
extern "C" void __fastcall kotor_fastcall_005edc20(void* client, void*, U32 paused, U32 reason, U32 force) {
    kotor_fastcall_005f2e10(pw(client, 4), nullptr, paused, reason, force); // 005EDC20..005EDC23
}

// 005F3F10 CClientExoAppInternal::SetAutoPaused(on, reason byte), RET 8.
extern "C" void __fastcall frame_SetAutoPaused_005f3f10(menu_continuation::Client* client_internal, void*, unsigned on, unsigned reason) {
    void* const self = client_internal;
    const U32 flags = w(self, 0x384);                                   // 005F3F13
    if ((flags & 1u) == on) return;                                     // 005F3F1D..005F3F24
    if (on != 0) {                                                      // 005F3F2D..005F3F2F
        if (server() && frame_GetPauseState_004ae980(server(), nullptr, 2) != 0) return; // 005F3F35..005F3F4A
        if (static_cast<double>(f(self, 0x38c)) > k_zero_0073d700) {    // 005F3F50..005F3F61
            sw(self, 0x38c, 0);                                         // 005F3F67
            sw(self, 0x390, 0x3F800000u);                               // 005F3F6E (1.0f)
            bytes(self)[0x398] = static_cast<unsigned char>(reason);    // 005F3F78
            return;
        }
        void* in_game = pw(self, 0x40);                                 // 005F3F82
        if (w(in_game, 0xb4) != 0) return;                              // 005F3F85..005F3F8B
        if (static_cast<unsigned char>(reason) == 9) {                  // 005F3F91..005F3F96
            if (static_cast<double>(f(self, 0x39c)) > k_zero_0073d700) return; // 005F3F98..005F3FA9
            sw(self, 0x39c, 0x40000000u);                               // 005F3FAF (2.0f)
        }
        sw(self, 0x384, w(self, 0x384) | 1u);                           // 005F3FB9
        if (w(in_game, 0xb4) != 0) return;                              // 005F3FC0..005F3FC6
        const U32 current = server_paused();                            // 005F3FCC..005F3FE2
        const U32 combat = w(self, 0x37c);                              // 005F3FE4
        if ((combat & 4) != 0 || current == 1) return;                  // 005F3FEA..005F3FF6
        sw(self, 0x37c, combat | 4);                                    // 005F4000..005F4003
        bytes(self)[0x388] = static_cast<unsigned char>(reason);        // 005F3FFC / 005F4009
        sw(self, 0x380, 1);                                             // 005F400F
        kotor_fastcall_005df570(kotor_menu_window_cells::input_007a39e4, nullptr); // 005F4019..005F401F
        sound_mode(kotor_rdata_sound_mode_0074c5f4);                    // 005F4024..005F4031
        sw(self, 0x4c4, 1);                                             // 005F4037
        return;
    }
    sw(self, 0x384, flags & ~1u);                                       // 005F4045..005F4048
    if (!server()) return;                                              // 005F404E..005F4059
    if (frame_GetPauseState_004ae980(server(), nullptr, 2) == 0) return; // 005F405B..005F4064
    const U32 state = w(pw(self, 0x40), 0xb4);                          // 005F4066..005F4069
    if (state != 1 && state != 0) return;                               // 005F406F..005F4076
    const U32 current = server_paused();                                // 005F4078..005F408E
    const U32 combat = w(self, 0x37c);                                  // 005F4090
    if ((combat & 4) != 0 || current == 0) return;                      // 005F4096..005F409D
    sw(self, 0x37c, combat | 4);                                        // 005F409F..005F40A2
    bytes(self)[0x388] = 0;                                             // 005F40A8
    sw(self, 0x380, 0);                                                 // 005F40AE
    kotor_fastcall_005df580(kotor_menu_window_cells::input_007a39e4, nullptr); // 005F40B4..005F40BA
    sound_mode(kotor_rdata_sound_mode_0074c5ec);                        // 005F40BF..005F40CC
    sw(self, 0x4c4, 0);                                                 // 005F40D1
}

// 0062F590 CGuiInGame::EnterControllerPauseState.
extern "C" void __fastcall kotor_fastcall_0062f590(void* in_game, void*) {
    void* gui = at(frame_R4_005ed690(pw(menu_continuation::app_manager_007a39fc, 4), nullptr)); // 0062F590..0062F59B
    if (void* panel = pw(gui, 0xa4))                                    // 0062F5A0..0062F5A8
        kotor_fastcall_0040bc70(static_cast<gui_ctor::Gui*>(pw(in_game, 0x38)), nullptr, panel, 1, 1); // 0062F5AA..0062F5B2
    void* manager = menu_continuation::app_manager_007a39fc;            // 0062F5B7
    if (!manager || !pw(manager, 8)) return;                            // 0062F5BC..0062F5C5
    const U32 paused = kotor_fastcall_005edc10(pw(manager, 4), nullptr); // 0062F5C7..0062F5CA
    sw(in_game, 0xe0, paused);                                          // 0062F5D1
    if (paused == 0)                                                    // 0062F5CF / 0062F5D7
        kotor_fastcall_005edc20(pw(menu_continuation::app_manager_007a39fc, 4), nullptr, 1, 0, 1); // 0062F5D9..0062F5E7
}
