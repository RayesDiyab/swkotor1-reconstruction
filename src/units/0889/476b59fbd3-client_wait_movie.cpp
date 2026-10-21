// lane-m48-b fork CLIENT, unit client-wait-movie: CClientExoAppInternal::WaitForMovie 005F6F20 (lane-config intro queue,
// movies-enabled path 00602B9F) and its leaves: messagepump 00401510, CClientExoApp::RemoveMoveToModuleMovies 005EE380,
// CGuiInGame::LoadAmbientTrackForDialog 0062A8B0.
// Evidence: dumpbin /disasm swkotor.exe 005F6F20..005F7185, 00401510..00401587, 005EE380..005EE38D, 0062A8B0..0062A95A;
// IAT (dumpbin /imports): 0073D3F4 PeekMessageA, 0073D3B4 GetMessageA, 0073D3A0 TranslateMessage, 0073D3B8
// DispatchMessageA. .rdata 0074EA14 = 3FA28F5Ch (1.27f), 0074D330 "evil_ending", 0073D71C "".
// D30-FINAL: the volume is fild(byte) * 1.27f in x87 and converted by the original _ftol2 port (genuine_x87_ftol_006fae8c).
#include "e6446a7354-r4_lowers.h"
#include "e7f5cbab44-resources.h"
#include "f9309c6a48-window_live_views.h"
#include "424ed88ca0-live_publication.h"

using namespace r4_lowers;

namespace kotor_menu20 {   // token-identical to canonical menu_targets.h / input-hover hover_path.h (task-19 menu-startup-20-29)
using Word=std::uint32_t;
struct ClientInternal {Word unknown;void* volatile options;};
struct ClientApp {Word unknown;ClientInternal* volatile internal;__declspec(noinline) void* __thiscall get_options_005ed700();};
}
namespace table29_required {
void __fastcall gui_sound_destroy_005d6200(void*, void*);
void* __fastcall gui_sound_construct_005d5a50(void*, void*);
void __fastcall gui_volume_005d5b50(void* outer, void*, std::uint32_t volume);
std::uint32_t __fastcall gui_play_005d5b10(void*, void*, menu_resources::Ref*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall gui_priority_005d5ba0(void*, void*, std::uint32_t);
void __fastcall gui_sound_mode_005d5e80(void* outer, void*, std::uint32_t mode);
}
namespace kotor_game_prepare { void __fastcall SetGlobalUnpauseableState0062b410(void* self, void* edx, std::uint32_t); }
extern "C" {
void __cdecl genuine_x87_ftol_006fae8c();                                  // renderer-x87-ftol-alias owner
std::uint32_t __fastcall kotor_fastcall_005df510(void* input, void*);      // CExoInput::GetActive (routes)
int __fastcall kotor_fastcall_0070c1f0(void* player, void*);               // IsMoviePlaying
void __fastcall kotor_fastcall_0070c1e0(void* player, void*, U32, U32);   // CancelMovie
void __fastcall kotor_fastcall_005d5b80(void* source, void*);              // client-dtors
void __fastcall kotor_fastcall_004ae030(void* timer, void*);               // CWorldTimer::UnpauseWorldTimer
void __fastcall kotor_fastcall_005f2980(void* internal, void*);            // TurnOffKeyboardModal
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*);
void* __cdecl live_operator_new(std::uint32_t);
extern void* kotor_cell_sound_007a39ec;
extern const std::uint32_t kotor_rdata_sound_mode_0074c5ec;               // sound-owners (extern after lane-m48-b fix/sound)
extern volatile std::uint32_t kotor_cell_render_active_007a3a38;           // aurora-window window_activation
void __cdecl kotor_cdecl_00401510();
void __fastcall kotor_fastcall_005ee380(void* client, void*);
void __fastcall kotor_fastcall_0062a8b0(void* in_game, void*, menu_resources::Ref* track);
std::uint32_t __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*, void*);   // CResRef::IsValid (gui-load)
}
namespace options_video { struct Client; }
namespace lane_config_required { std::int32_t __fastcall WaitForMovie_005f6f20(options_video::Client* internal, void*, std::int32_t remove_module_movies); }

namespace {
const char k_empty_0073d71c[] = "";
const char k_evil_ending_0074d330[] = "evil_ending";
const std::uint32_t k_volume_scale_0074ea14 = 0x3FA28F5Cu;   // 1.27f
void* client() { return pw(menu_continuation::app_manager_007a39fc, 4); }
// fild dword (byte value) ; fmul dword [0074EA14] ; call 006FAE8C (_ftol2) -> EAX  (005F7016..005F7028)
__declspec(noinline) std::int32_t scaled_volume(std::int32_t value) {
    std::int32_t result;
    const std::uint32_t* scale = &k_volume_scale_0074ea14;
    __asm {
        mov eax, scale
        fild dword ptr [value]
        fmul dword ptr [eax]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}
}

// 00401510 messagepump: drain the queue with PeekMessage/GetMessage; GetMessage returning 0 (WM_QUIT) sets the WinMain
// quit word 007A39B8.
// M48 composer: 00401510 body dropped (single owner lane-m48-a winmain_callback_cell.cpp _winmain_message_pump_00401510; joined).

// 005EE380 CClientExoApp::RemoveMoveToModuleMovies: [[this+4]+2DCh] = 0.
extern "C" void __fastcall kotor_fastcall_005ee380(void* client_app, void*) { sw(pw(client_app, 4), 0x2dc, 0); } // 005EE380..005EE383

// 0062A8B0 CGuiInGame::LoadAmbientTrackForDialog(CResRef*), RET 4.
extern "C" void __fastcall kotor_fastcall_0062a8b0(void* in_game, void*, menu_resources::Ref* track) {
    if (void* old = pw(in_game, 0xbbc)) {                               // 0062A8C9..0062A8D1
        table29_required::gui_sound_destroy_005d6200(old, nullptr);     // 0062A8D3..0062A8D5
        live_free(old);                                                 // 0062A8DA..0062A8DB
        sw(in_game, 0xbbc, 0);                                          // 0062A8E3
    }
    if (gui_load_resref_is_valid_00406030(track, nullptr) == 0) return; // 0062A8ED..0062A8FA
    void* memory = live_operator_new(4);                                // 0062A8FC..0062A8FE
    void* source = memory ? table29_required::gui_sound_construct_005d5a50(memory, nullptr) : nullptr; // 0062A90A..0062A91F (EH state 0)
    sw(in_game, 0xbbc, static_cast<U32>(reinterpret_cast<std::uintptr_t>(source))); // 0062A92B
    if (!source) return;                                                // 0062A921 / 0062A931
    table29_required::gui_priority_005d5ba0(source, nullptr, 4);        // 0062A933..0062A937
    table29_required::gui_play_005d5b10(pw(in_game, 0xbbc), nullptr, track, 8, 0, 1, 0); // 0062A93C..0062A94B
}

// 005F6F20 CClientExoAppInternal::WaitForMovie(remove_module_movies), RET 4, EAX 0.
std::int32_t __fastcall lane_config_required::WaitForMovie_005f6f20(options_video::Client* client_internal, void*, std::int32_t remove_module_movies) {
    void* const self = client_internal;
    U32 ticks = 0;                                                      // 005F6F42 (ebx)
    U32 cancel_sent = 0;                                                // 005F6F45 [esp+0C]
    const U32 active = kotor_fastcall_005df510(kotor_menu_window_cells::input_007a39e4, nullptr); // 005F6F3C..005F6F49
    const U32 saved_render = kotor_cell_render_active_007a3a38;         // 005F6F58..005F6F61
    if (kotor_menu_window_cells::input_007a39e4)                        // 005F6F4E..005F6F65
        kotor_fastcall_005df540(kotor_menu_window_cells::input_007a39e4, nullptr, 1); // 005F6F67..005F6F69
    kotor_game_prepare::ShowMouse0061fa00(self, nullptr, 0);            // 005F6F6E..005F6F72
    if (w(self, 0x4bc) != 0) {                                          // 005F6F77..005F6F7F
        if (pw(self, 0x3c4)) {                                          // 005F6F85..005F6F8D
            kotor_fastcall_005d5b80(pw(self, 0x3c4), nullptr);          // 005F6F8F
            if (void* old = pw(self, 0x3c4)) {                          // 005F6F94..005F6F9C
                table29_required::gui_sound_destroy_005d6200(old, nullptr); // 005F6F9E..005F6FA0
                live_free(old);                                         // 005F6FA5..005F6FA6
            }
        }
        menu_resources::Ref empty_track;                                // [esp+1C]
        empty_track.construct_00406d80(k_empty_0073d71c);               // 005F6FAE..005F6FB7
        void* in_game = at(frame_R4_005ed690(client(), nullptr));       // 005F6FBC..005F6FCA
        kotor_fastcall_0062a8b0(in_game, nullptr, &empty_track);        // 005F6FCF..005F6FD1
        void* memory = live_operator_new(4);                            // 005F6FD6..005F6FD8
        void* source = memory ? table29_required::gui_sound_construct_005d5a50(memory, nullptr) : nullptr; // 005F6FE0..005F6FF9 (EH state 0)
        sw(self, 0x3c4, static_cast<U32>(reinterpret_cast<std::uintptr_t>(source))); // 005F6FFB
        const unsigned char* options = static_cast<const unsigned char*>(static_cast<kotor_menu20::ClientApp*>(client())->get_options_005ed700()); // 005F7001..005F7011
        const std::int32_t volume = scaled_volume(static_cast<std::int32_t>(options[0x48])); // 005F7016..005F7028
        table29_required::gui_volume_005d5b50(pw(self, 0x3c4), nullptr, static_cast<U32>(volume)); // 005F702D..005F7034
        menu_resources::Ref ending;                                     // [esp+2C]
        ending.construct_00406d80(k_evil_ending_0074d330);              // 005F7039..005F7042
        table29_required::gui_play_005d5b10(pw(self, 0x3c4), nullptr, &ending, 8, 0, 1, 0); // 005F7047..005F705A
    }
    for (;;) {                                                          // 005F7063
        void* player = pw(self, 0x12c);
        if (!player || kotor_fastcall_0070c1f0(player, nullptr) != 1) break; // 005F7063..005F7075
        if (ticks == 0x1f4 && cancel_sent == 0) {                       // 005F7077..005F7085
            kotor_cdecl_00401510();                                     // 005F7087
            if (w(self, 0x254) == 1) {                                  // 005F708C..005F7093
                if (void* p = pw(self, 0x12c)) kotor_fastcall_0070c1e0(p, nullptr, 1, 0); // 005F7095..005F70A3
                cancel_sent = 1;                                        // 005F70A8
            }
            ticks = 0;                                                  // 005F70B0
        } else {
            ++ticks;                                                    // 005F70B4
        }
    }
    if (remove_module_movies != 0) {                                    // 005F70B7..005F70BD
        kotor_fastcall_005ee380(client(), nullptr);                     // 005F70BF..005F70C7
        table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, kotor_rdata_sound_mode_0074c5ec); // 005F70CC..005F70D9
        if (w(self, 0x200) != 0) {                                      // 005F70DE..005F70E6
            kotor_game_prepare::SetGlobalUnpauseableState0062b410(pw(self, 0x40), nullptr, 0); // 005F70E8..005F70ED
            table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, kotor_rdata_sound_mode_0074c5ec); // 005F70F2..005F70FF
            kotor_fastcall_004ae030(frame_GetWorldTimer_005ed4f0(client(), nullptr), nullptr); // 005F7104..005F7113
            kotor_fastcall_004ae030(kotor_fastcall_004aede0(pw(menu_continuation::app_manager_007a39fc, 8), nullptr), nullptr); // 005F7118..005F7128
            sw(self, 0x200, 0);                                         // 005F712D
        }
    }
    if (kotor_cell_render_active_007a3a38 == saved_render) {            // 005F7137..005F7141
        if (kotor_menu_window_cells::input_007a39e4)                    // 005F7143..005F714B
            kotor_fastcall_005df540(kotor_menu_window_cells::input_007a39e4, nullptr, active); // 005F714D..005F714E
        if (active == 0) kotor_fastcall_005f2980(self, nullptr);        // 005F7153..005F7159
        else kotor_game_prepare::ShowMouse0061fa00(self, nullptr, 0);   // 005F7160..005F7162
    }
    kotor_game_prepare::ShowMouse0061fa00(self, nullptr, 8);            // 005F7167..005F716B
    return 0;                                                           // 005F7176
}
