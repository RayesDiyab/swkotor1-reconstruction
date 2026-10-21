// lane-m15-0 unit m15-l0-client: CClientExoApp getters / tail thunks and the CClientExoAppInternal bodies they reach
// on the character-creation path (load screen show/hide/hint/progress/module image, tutorial window, item lookup,
// player-creature test, load music stop) plus CExoInput::CoolDownEvent. Original: swkotor.exe GOG K1, static only.
//   CClientExoApp          +4 CClientExoAppInternal* (005ED550..005EDF43)
//   CClientExoAppInternal  +14 CGameObjectArray*, +20 player creature id, +40 CGuiInGame*, +1C4 CSoundSetList*,
//                          +23C load music (CExoStreamingSoundSource*), +274 CSWGuiManager*, +278 CSWGuiLoadScreen*,
//                          +288 area-not-ready, +2FC load-screen image (CExoString), +304 CExoString, +30C image-set flag,
//                          +3AC/+3B0/+3B4/+3B8 tutorial window args, +488/+48C hint timer (cited per statement)
#include "b0e1e980f2-m15_l0.h"
#include "3df78882af-startup_string.h"   // kotor_reconstruction::StartupString (005B3190/005E5A90/005E5C20/005E5C50/005E5790)
#include "723b658e51-resources.h"        // menu_resources::Ref::construct_00406d80, Manager::exists_00408bc0
#include "9d20f37300-normal.h"           // task-12 menu_normal_retained_40_49::MusicInternal::music_005f6480 (StartLoadMusic)
#include <cstring>
#include <new>

extern "C" {
void* __cdecl live_operator_new(std::uint32_t);                                           // 006FA7E6
void __cdecl live_free(void*);                                                             // 006FA390
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);      // D83 shared trap (m48t)
std::uint32_t __fastcall recovered_loading_hint_005f4760(void*, void*, const void*, std::uint32_t) noexcept(false); // 005F4760 RET 8 (0841 real body)
extern void* kotor_cell_sound_007a39ec;                                                    // [007A39EC] CExoSound* (lane-sound)
extern const std::uint32_t kotor_rdata_sound_mode_0074c5f8;                                // .rdata 0074C5F8 = 3 (lane-sound sound_owners.cpp)
extern const std::uint32_t kotor_rdata_sound_mode_0074c5fc;                                // .rdata 0074C5FC = 4 (lane-sound sound_owners.cpp)
extern void* kotor_cell_rules_007a3a28;                                                    // [007A3A28] CSWRules* (lane-prepare)
menu_resources::Manager* __cdecl c2da_manager_007a39e8();                                  // [007A39E8] CExoResMan*
// CExoString helpers (C-linkage carriers of the original entries)
kotor_reconstruction::StartupString* __fastcall c2da_assign_text_005e5140(kotor_reconstruction::StartupString*, void*,
                                                                          const char*) noexcept(false);   // operator=(char const*)
std::int32_t __fastcall exostring_compare_no_case_005e6450(kotor_reconstruction::StartupString*, void*,
                                                           kotor_reconstruction::StartupString*);           // CompareNoCase
void __fastcall frame_LogStringCtor_005e5a90(kotor_reconstruction::StartupString*, void*, const char*) noexcept(false); // CExoString(char const*)
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, kotor_reconstruction::StartupString*); // CResRef(CExoString const&)
// GUI / client / input callees (M60 owners named in PACKET.json)
unsigned __fastcall frame_ContainsPanel_0040bd70(void* gui, void*, void* panel);          // CSWGuiManager::PanelExists
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, std::uint32_t, std::uint32_t) noexcept(false); // AddPanel
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                               // CSWGuiManager::PopModalPanel
std::uint32_t __fastcall frame_RemovePanel_0040c830(void* gui, void*, void* panel);       // RemovePanel (EAX 0/1: see adapt/)
void __fastcall gui_load_text_params_set_strref_00415e50(void*, void*, std::uint32_t) noexcept(false); // CSWGuiTextParams::SetStrRef
void __fastcall kotor_fastcall_005df540(void* input, void*, int);                         // CExoInput::SetActive
void __fastcall kotor_fastcall_005f2980(void* internal, void*);                           // TurnOffKeyboardModal
void __fastcall kotor_fastcall_0061f9c0(void* internal, void*, std::uint32_t);            // HideMouse
void __fastcall kotor_fastcall_0061fa00(void* internal, void*, int);                      // ShowMouse
void __cdecl kotor_cdecl_00401c10(float delta, std::int32_t run_server, std::int32_t keep_frame); // UpdateScreen
void __fastcall kotor_fastcall_005d5b80(void* source, void*);                             // CExoStreamingSoundSource::Stop
void __fastcall kotor_fastcall_005d6200(void* source, void*);                             // ~CExoStreamingSoundSource
void __fastcall kotor_fastcall_005ed310(void* object, void*);                             // void_return_this+4
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out); // CGameObjectArray::GetGameObject
std::uint32_t __fastcall kotor_fastcall_004aea40(void* server, void*);                    // CServerExoApp::GetPlayerCreatureId
std::uint32_t __fastcall kotor_fastcall_005eda50(void* client, void*, std::uint32_t id);  // ServerToClientObjectId
// lists (CExoLinkedListInternal)
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);           // GetAtPos
std::uint32_t __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);    // GetNext
void* __fastcall resource_append_005e9dc0(void* list, void*, void* item);                 // AddTail
void __fastcall c2da_005e9c10(void* list, void*, void* pos);                              // Remove
}
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // CExoString::Format
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // CExoString::operator!=(char const*)
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; extern void* volatile input_007a39e4; }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }
namespace kotor_input_required { std::uint32_t __fastcall low_resolution_timer_005e9250(void* timers, void*); }
namespace table29_required { void __fastcall gui_sound_mode_005d5e80(void*, void*, std::uint32_t); } // CExoSoundInternal::SetSoundMode
namespace kotor_original {
void* __fastcall kotor_fastcall_0067a710(void* self, void*, void* gui_manager);           // CSWGuiLoadScreen ctor (C++ linkage, may throw)
void __fastcall kotor_fastcall_00417fb0(void* bar, void*, std::int32_t value);           // CSWGuiProgressBar::SetCurValue
}

namespace {
using U = std::uint32_t;
using I32 = std::int32_t;
using String = kotor_reconstruction::StartupString;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, const void* v) { std::memcpy(at(p, off), &v, 4); }
String* str(void* p, unsigned off) { return reinterpret_cast<String*>(at(p, off)); }
void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }                  // [007A39FC]
void* timers() { return rp(kotor_menu_10_19::dependencies::base_007a39e0, 4); }           // [[007A39E0]+4]
// [[[007A3A28]+B8h]+18h] loadscreens 2DA (005F34E5..005F34F0)
void* loadscreens_2da() { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x18); }
const char kEmpty[] = "";                                                                 // 0073D71C
const char kBmpResRef[] = "BMPResRef";                                                    // 00745588
const char kLoadFormat[] = "load_%s";                                                     // 00745580
const char kDefault[] = "DEFAULT";                                                        // 0073E304
const char kLoadBar[] = "Load Bar = %d";                                                  // 0074EC0C
struct ZeroVector { float x = 0.0f, y = 0.0f, z = 0.0f; };                                 // [esp+0Ch] 3 dwords = 0
}

extern "C" {
// ================================================================== CClientExoApp (outer)
U __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*) {
    return rd(rp(app, 4), 0x20);                                                          // 005ED550..005ED553
}
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, U id) {
    return kotor_m15_client_internal_get_item_by_id_005f1130(rp(app, 4), nullptr, id);    // 005ED5A0..005ED5A3
}
void* __fastcall kotor_m15_client_get_sound_set_list_005ed910(void* app, void*) {
    return rp(rp(app, 4), 0x1C4);                                                         // 005ED910..005ED913
}
I32 __fastcall kotor_m15_client_is_player_creature_005eda30(void* app, void*, void* object) {
    return kotor_m15_client_internal_is_player_creature_005f2b50(rp(app, 4), nullptr, object); // 005EDA30..005EDA33
}
void __fastcall kotor_m15_client_show_load_screen_005eda80(void* app, void*, U fade, U music, U alt) noexcept(false) {
    kotor_m15_client_internal_show_load_screen_005f6c60(rp(app, 4), nullptr, fade, music, alt); // 005EDA80..005EDA83
}
void __fastcall kotor_m15_client_hide_load_screen_005eda90(void* app, void*, U fade, U update) noexcept(false) {
    kotor_m15_client_internal_hide_load_screen_005f6e20(rp(app, 4), nullptr, fade, update); // 005EDA90..005EDA93
}
U __fastcall kotor_m15_client_get_area_not_ready_005edad0(void* app, void*) {
    return rd(rp(app, 4), 0x288);                                                         // 005EDAD0..005EDAD3
}
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(void* app, void*, void* module) noexcept(false) {
    kotor_m15_client_internal_set_load_screen_by_module_005f3480(rp(app, 4), nullptr, module); // 005EDCF0..005EDCF3
}
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* app, void*, I32 progress, U update_hint) noexcept(false) {
    kotor_m15_client_internal_set_load_bar_progress_005fb3f0(rp(app, 4), nullptr, progress, update_hint); // 005EDD40..005EDD43
}
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(void* app, void*, U strref) noexcept(false) {
    kotor_m15_client_internal_set_load_screen_hint_005f37f0(rp(app, 4), nullptr, strref); // 005EDF00..005EDF03
}
int __fastcall show_tutorial_window_005edf40(void* app, void*, U reason, U a2, U a3, U a4) noexcept(false) {
    return kotor_m15_client_internal_show_tutorial_window_005f4120(rp(app, 4), nullptr, reason, a2, a3, a4); // 005EDF40..005EDF43
}
// 005DF4B0 CExoInput::CoolDownEvent: ECX=[ECX+4]; JMP 005E0670.
void __fastcall cool_down_event_005df4b0(void* input, void*, U event, U enable) noexcept(false) {
    kotor_m15_input_internal_cool_down_event_005e0670(rp(input, 4), nullptr, event, enable); // 005DF4B0..005DF4B3
}

// ================================================================== CClientExoAppInternal
// 005F1130 GetItemByGameObjectID(id), RET 4: object type byte +8 == 6 (CGameObject item) -> vslot +34h, else 0.
void* __fastcall kotor_m15_client_internal_get_item_by_id_005f1130(void* internal, void*, U id) {
    void* object = nullptr;                                                               // [esp] = 0 (005F113E)
    const unsigned char found = kotor_fastcall_004d8230(rp(internal, 0x14), nullptr, id, &object); // 005F1131..005F1146
    if (found != 0x00) return nullptr;                                                    // 005F114B..005F1151 (CMP AL,[0074666C]=00h)
    if (object == nullptr) return nullptr;                                                // 005F1153..005F1159
    if (*at(object, 8) != 0x06) return nullptr;                                           // 005F115B..005F1164 (CMP AL,[00746635]=06h)
    using AsItem = void* (__fastcall*)(void*, void*);                                     // vslot +34h, thiscall, no stack args
    AsItem slot; std::memcpy(&slot, at(rp(object, 0), 0x34), 4);                          // 005F1166 MOV EDX,[ECX] / CALL [EDX+34h]
    return slot(object, nullptr);                                                         // 005F1168
}
// 005F1B90 StopLoadMusic, RET 0.
void __fastcall kotor_m15_client_internal_stop_load_music_005f1b90(void* internal, void*) {
    void* const music = rp(internal, 0x23C);                                              // 005F1B93
    if (music == nullptr) return;                                                         // 005F1B99..005F1B9B
    kotor_fastcall_005d5b80(music, nullptr);                                              // 005F1B9E Stop
    if (void* again = rp(internal, 0x23C)) {                                              // 005F1BA3..005F1BAB
        kotor_fastcall_005d6200(again, nullptr);                                          // 005F1BAF ~CExoStreamingSoundSource
        live_free(again);                                                                 // 005F1BB4..005F1BB5
    }
    wr(internal, 0x23C, 0);                                                               // 005F1BBD
}
// 005F2B50 IsPlayerCreature(object), RET 4.
I32 __fastcall kotor_m15_client_internal_is_player_creature_005f2b50(void* internal, void*, void* object) {
    if (object == nullptr) return 0;                                                      // 005F2B51..005F2B5C
    const U player = rd(internal, 0x20);                                                  // 005F2B5F
    if (player != 0x7F000000u) return rd(object, 4) == player ? 1 : 0;                    // 005F2B62..005F2B76
    const U server_id = kotor_fastcall_004aea40(rp(app_manager(), 8), nullptr);           // 005F2B79..005F2B82
    const U client_id = kotor_fastcall_005eda50(rp(app_manager(), 4), nullptr, server_id); // 005F2B87..005F2B91
    return rd(object, 4) == client_id ? 1 : 0;                                            // 005F2B96..005F2BA3
}
// 005F3480 (no symbol; tail target of 005EDCF3) SetLoadScreenByModuleName(const CExoString& module), RET 4.
// EH 00725B70: state 0 = image string; states 1..3 = temporaries.
void __fastcall kotor_m15_client_internal_set_load_screen_by_module_005f3480(void* internal, void*, void* module_arg) noexcept(false) {
    String* const module = static_cast<String*>(module_arg);
    String image;                                                                          // 005F34A3..005F34A7 [esp+10h]
    if (module->BorrowedLength005e5790() == 0) {                                          // 005F34AC..005F34C0
        (void)c2da_assign_text_005e5140(str(internal, 0x2FC), nullptr, kEmpty);           // 005F34C2..005F34CD
        wr(internal, 0x30C, 1);                                                           // 005F34D2 -> 005F361D
        return;                                                                           // ~image (005F3627..005F3633)
    }
    I32 found;
    {
        String column(kBmpResRef);                                                        // 005F34D7..005F34E0 [esp+18h]
        found = kotor_m15_c2da_get_string_entry_00413de0(loadscreens_2da(), nullptr, module, &column, &image); // 005F34E5..005F3505
    }                                                                                     // 005F3510..005F351B
    if (found == 0) {                                                                     // 005F350A..005F3522 (NEG/SBB/INC; BL=1 when 0)
        char* text; U capacity;                                                           // CExoString pushed BY VALUE (2 words) as the vararg
        std::memcpy(&text, module, 4); std::memcpy(&capacity, at(module, 4), 4);          // 005F3528..005F352E
        kotor_reconstruction::StartupFormat005e5680(&image, kLoadFormat, text, capacity); // 005F352F..005F353E
        alignas(4) unsigned char ref[16];                                                 // [esp+20h] CResRef
        (void)async_ref_construct_string_00406d60(ref, nullptr, &image);                  // 005F3541..005F354A
        if (c2da_manager_007a39e8()->exists_00408bc0(reinterpret_cast<menu_resources::Ref*>(ref), 3, nullptr) == 0) { // 005F354F..005F3565 (TGA)
            (void)async_ref_construct_string_00406d60(ref, nullptr, &image);              // 005F356B..005F3574
            if (c2da_manager_007a39e8()->exists_00408bc0(reinterpret_cast<menu_resources::Ref*>(ref), 0x0BBF, nullptr) == 0) { // 005F3579..005F3592 (TPC)
                String column(kBmpResRef);                                                // 005F3594..005F359D [esp+20h]
                String row(kDefault);                                                     // 005F35A4..005F35B1 [esp+18h]
                (void)kotor_m15_c2da_get_string_entry_00413de0(loadscreens_2da(), nullptr, &row, &column, &image); // 005F35B6..005F35DB
            }                                                                             // ~row 005F35E0..005F35E8, ~column 005F35ED..005F35F6
        }
    }
    String* const current = str(internal, 0x2FC);                                         // 005F35FF
    if (exostring_compare_no_case_005e6450(current, nullptr, &image) == 0) {              // 005F35FB..005F360F
        current->Assign005e5c50(&image);                                                  // 005F3611..005F3618
        wr(internal, 0x30C, 1);                                                           // 005F361D
    }
}                                                                                         // ~image 005F3627..005F3633
// 005F37F0 SetLoadScreenHint(strref), RET 4: load screen ? tail JMP 0067A9C0 : RET 4.
void __fastcall kotor_m15_client_internal_set_load_screen_hint_005f37f0(void* internal, void*, U strref) noexcept(false) {
    void* const screen = rp(internal, 0x278);                                             // 005F37F0
    if (screen != nullptr) kotor_m15_load_screen_set_hint_0067a9c0(screen, nullptr, strref); // 005F37F6..005F37FA
}
// 005F4120 ShowTutorialWindow(reason, a2, a3, a4), RET 10h.
int __fastcall kotor_m15_client_internal_show_tutorial_window_005f4120(void* internal, void*, U reason, U a2, U a3,
                                                                       U a4) noexcept(false) {
    if (kotor_m15_ingame_tutorial_reason_will_show_0062f420(rp(internal, 0x40), nullptr, reason) == 0) return 0; // 005F4128..005F4133
    if (kotor_m15_ingame_set_tutorial_reason_0062b0d0(rp(internal, 0x40), nullptr, reason) == 0) return 0;      // 005F4135..005F4140
    wr(internal, 0x3AC, a2);                                                              // 005F4149 / 005F4155
    *at(internal, 0x3B8) = static_cast<unsigned char>(reason);                            // 005F415B (BL)
    wr(internal, 0x3B0, a3);                                                              // 005F414D / 005F4161
    wr(internal, 0x3B4, a4);                                                              // 005F4151 / 005F4167
    return 1;                                                                             // 005F416E
}
// 005F6C60 ShowLoadScreen(fade, music, alt), RET 0Ch. EH 00725E1B state 0 around the CSWGuiLoadScreen ctor.
void __fastcall kotor_m15_client_internal_show_load_screen_005f6c60(void* internal, void*, U fade, U music, U alt) noexcept(false) {
    if (kotor_cell_sound_007a39ec != nullptr) {                                           // 005F6C7B..005F6C88
        if (alt != 0) {                                                                   // 005F6C8A..005F6C8C
            table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, kotor_rdata_sound_mode_0074c5f8); // 005F6C8E..005F6CB7
        } else if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(str(internal, 0x304)),
                                                          nullptr, kEmpty) != 0) {       // 005F6C96..005F6CA8 (005E5390)
            table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr, kotor_rdata_sound_mode_0074c5fc); // 005F6CAA..005F6CB7
        }
    }
    if (rd(internal, 0x30C) == 0) return;                                                 // 005F6CBC..005F6CC4
    if (str(internal, 0x2FC)->BorrowedLength005e5790() == 0) return;                      // 005F6CCB..005F6CDA
    if (rp(internal, 0x278) == nullptr) {                                                 // 005F6CE0..005F6CE8
        void* const raw = live_operator_new(0x6B8);                                       // 005F6CEA..005F6CEF
        void* screen = nullptr;
        if (raw != nullptr) {                                                             // 005F6CFB..005F6D05
            try { screen = kotor_original::kotor_fastcall_0067a710(raw, nullptr, rp(internal, 0x274)); } // 005F6D07..005F6D10
            catch (...) { live_free(raw); throw; }                                        // unwind state 0 (00725E1B): delete raw
        }
        wp(internal, 0x278, screen);                                                      // 005F6D19..005F6D21
    }
    alignas(4) unsigned char ref[16];                                                     // [esp+18h] CResRef
    (void)async_ref_construct_string_00406d60(ref, nullptr, str(internal, 0x2FC));        // 005F6D27..005F6D2C
    kotor_m15_load_screen_set_image_0067a6d0(rp(internal, 0x278), nullptr, ref);          // 005F6D31..005F6D3C
    gui_load_text_params_set_strref_00415e50(at(rp(internal, 0x278), 0x51C), nullptr,
                                             alt != 0 ? 0xA620u : 0xA5FDu);               // 005F6D41..005F6D5B (NEG/SBB/AND 23h/ADD 0A5FDh)
    void* const gui = rp(internal, 0x274);                                                // 005F6D60
    if (gui != nullptr && frame_ContainsPanel_0040bd70(gui, nullptr, rp(internal, 0x278)) == 0) { // 005F6D66..005F6D78
        wr(internal, 0x48C, kotor_input_required::low_resolution_timer_005e9250(timers(), nullptr)); // 005F6D7A..005F6D87
        if (fade != 0 && rp(internal, 0x40) != nullptr) {                                 // 005F6D8D..005F6D9A
            ZeroVector color;                                                             // 005F6DA7..005F6DB7
            kotor_m15_ingame_start_global_fade_0062abf0(rp(internal, 0x40), nullptr, 0, 0, 0, &color); // 005F6D95..005F6DBF
        }
        kotor_fastcall_0040bc70(rp(internal, 0x274), nullptr, rp(internal, 0x278), 1, 1); // 005F6DC4..005F6DD5
    }
    if (void* input = kotor_menu_window_cells::input_007a39e4)                            // 005F6DDA..005F6DE2
        kotor_fastcall_005df540(input, nullptr, 0);                                       // 005F6DE4..005F6DE6
    kotor_fastcall_005f2980(internal, nullptr);                                           // 005F6DEB..005F6DED
    kotor_fastcall_0061f9c0(internal, nullptr, 2);                                        // 005F6DF2..005F6DF6
    if (music != 0)                                                                       // 005F6DFB..005F6E01
        reinterpret_cast<menu_normal_retained_40_49::MusicInternal*>(internal)->music_005f6480(); // 005F6E03..005F6E05
}
// 005F6E20 HideLoadScreen(fade, update), RET 8.
void __fastcall kotor_m15_client_internal_hide_load_screen_005f6e20(void* internal, void*, U fade, U update) noexcept(false) {
    kotor_fastcall_0061fa00(internal, nullptr, 2);                                        // 005F6E24..005F6E28
    if (rp(internal, 0x278) == nullptr) return;                                           // 005F6E2D..005F6E35
    if (rp(internal, 0x274) == nullptr) return;                                           // 005F6E3B..005F6E43
    if (fade != 0) {                                                                      // 005F6E4A..005F6E50
        ZeroVector color;                                                                 // 005F6E60..005F6E70
        kotor_m15_ingame_start_global_fade_0062abf0(rp(internal, 0x40), nullptr, 0, 0, 0, &color); // 005F6E52..005F6E78 (no null test)
    }
    (void)kotor_fastcall_0040be00(rp(internal, 0x274), nullptr);                          // 005F6E7D..005F6E83 PopModalPanel
    if (frame_RemovePanel_0040c830(rp(internal, 0x274), nullptr, rp(internal, 0x278)) != 0) // 005F6E88..005F6E9C
        kotor_m15_load_screen_add_hint_to_buffer_0067aa80(rp(internal, 0x278), nullptr);  // 005F6E9E..005F6EA4
    kotor_original::kotor_fastcall_00417fb0(at(rp(internal, 0x278), 0x64), nullptr, 0);   // 005F6EA9..005F6EB4
    alignas(4) unsigned char ref[16];                                                     // [esp+8] CResRef
    (void)reinterpret_cast<menu_resources::Ref*>(ref)->construct_00406d80(kEmpty);        // 005F6EB9..005F6EC2
    kotor_m15_panel_set_background_0040a870(rp(internal, 0x278), nullptr, ref);           // 005F6EC7..005F6ED2
    if (void* input = kotor_menu_window_cells::input_007a39e4)                            // 005F6ED7..005F6EDF
        kotor_fastcall_005df540(input, nullptr, 1);                                       // 005F6EE1..005F6EE3
    kotor_fastcall_0061fa00(internal, nullptr, 0);                                        // 005F6EE8..005F6EEC
    kotor_m15_client_internal_stop_load_music_005f1b90(internal, nullptr);                // 005F6EF1..005F6EF3
    if (fade == 0 && update != 0) {                                                       // 005F6EF8..005F6F03
        const U delta_bits = 0x3D088889u;                                                 // 005F6F09 PUSH 3D088889h (1/30 f)
        float delta; std::memcpy(&delta, &delta_bits, 4);
        kotor_cdecl_00401c10(delta, 0, 0);                                                // 005F6F05..005F6F13
    }
}
// 005F7B20 UpdateLoadingScreenHint, RET 0. The branch from 005F7BA5 on needs a live server (AppManager+8 and its +4)
// and calls the unauthored GetNextLoadScreenHintSTRREF 005F4760: D83 branch trap (TRAPS-DELTA.json); the rest is authored.
void __fastcall kotor_m15_client_internal_update_loading_screen_hint_005f7b20(void* internal, void*) noexcept(false) {
    void* const server = rp(app_manager(), 8);                                            // 005F7B2E..005F7B3A
    if (server == nullptr || rp(server, 4) == nullptr) return;                            // 005F7B40..005F7B50
    void* const screen = rp(internal, 0x278);                                             // 005F7B56
    if (screen == nullptr || rd(screen, 0x574) == 0) return;                              // 005F7B5C..005F7B6C
    const U now = kotor_input_required::low_resolution_timer_005e9250(timers(), nullptr); // 005F7B72..005F7B7B
    const U previous = rd(internal, 0x48C);                                               // 005F7B80
    wr(internal, 0x48C, now);                                                             // 005F7B88
    const U accumulated = rd(internal, 0x488) + (now - previous);                         // 005F7B8E..005F7B96
    wr(internal, 0x488, accumulated);                                                     // 005F7B9D
    if (accumulated < 10000u) return;                                                     // 005F7B98..005F7BA3 (CMP 2710h / JB)
    wr(internal, 0x488, 0);                                                               // 005F7BAE
    String module("");                                                                    // 005F7BA5..005F7BB8 CExoString(0073D71C "") (EH state 0)
    const U strref = recovered_loading_hint_005f4760(internal, nullptr, &module, 2);      // 005F7BBD..005F7BCE GetNextLoadScreenHintSTRREF(&s, 2), RET 8
    void* const screen_now = rp(internal, 0x278);                                         // 005F7BD3 (re-read)
    if (screen_now != nullptr) kotor_m15_load_screen_set_hint_0067a9c0(screen_now, nullptr, strref); // 005F7BD9..005F7BDE
}
// 005FB3F0 (no symbol; tail target of 005EDD43) SetLoadBarProgress(progress, update_hint), RET 8.
void __fastcall kotor_m15_client_internal_set_load_bar_progress_005fb3f0(void* internal, void*, I32 progress, U update_hint) noexcept(false) {
    kotor_fastcall_005ed310(kotor_menu_window_cells::input_007a39e4, nullptr);            // 005FB40B..005FB411 (void_return_this+4)
    if (rp(internal, 0x278) == nullptr) return;                                           // 005FB416..005FB41E
    if (update_hint != 0 && progress < 0x50)                                              // 005FB420..005FB430 (JGE signed)
        kotor_m15_client_internal_update_loading_screen_hint_005f7b20(internal, nullptr); // 005FB432..005FB434
    kotor_original::kotor_fastcall_00417fb0(at(rp(internal, 0x278), 0x64), nullptr, progress); // 005FB439..005FB443
    String text;                                                                          // 005FB448..005FB44C (EH state 0)
    kotor_reconstruction::StartupFormat005e5680(&text, kLoadBar, progress);               // 005FB451..005FB464 (result unused)
}                                                                                         // ~text 005FB46C..005FB478

// ================================================================== CExoInputInternal
// 005E0670 CoolDownEvent(event, enable), RET 8. +128h event table (CExoInputEvent* per id), +134h cool-down list holding
// pointers to table slots; event +0Ch = enable, +10h = compared key.
void __fastcall kotor_m15_input_internal_cool_down_event_005e0670(void* internal, void*, U event, U enable) noexcept(false) {
    void* pos = rp(rp(internal, 0x134), 0);                                               // 005E0674..005E068A (list head)
    void** const slot = static_cast<void**>(rp(internal, 0x128)) + event;                 // 005E067C..005E0687
    void* const entry = *slot;                                                            // 005E068E
    if (entry == nullptr) return;                                                         // 005E0690..005E0692
    wr(entry, 0x0C, enable);                                                              // 005E0695..005E0699
    while (pos != nullptr) {                                                              // 005E069C..005E06A2 / 005E06D4..005E06D6
        void* const item = *static_cast<void**>(unresolved_list_at_pos_005e9b60(rp(internal, 0x134), nullptr, pos)); // 005E06A4..005E06B0
        if (item != nullptr && rd(item, 0x10) == rd(*slot, 0x10)) {                       // 005E06B2..005E06BE
            if (enable != 0) return;                                                      // 005E06EB..005E06ED
            c2da_005e9c10(rp(internal, 0x134), nullptr, pos);                             // 005E06EF..005E06FA Remove
            return;
        }
        (void)unresolved_list_next_005e9b90(rp(internal, 0x134), nullptr, &pos);          // 005E06C0..005E06D0
    }
    (void)resource_append_005e9dc0(rp(internal, 0x134), nullptr, slot);                   // 005E06D8..005E06DF AddTail(&slot)
}
}
