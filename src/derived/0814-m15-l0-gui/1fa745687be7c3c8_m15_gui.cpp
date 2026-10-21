// lane-m15-0 unit m15-l0-gui: GUI base gaps (panel background/visibility, scene model getter, Y-button thunk), the
// global fade (CGuiInGame::StartGlobalFade -> CSWGuiFade::SetFade), CSWGuiLoadScreen image/hint/message-buffer and the
// tutorial-window path (CGuiInGame::TutorialReasonWillShowWindow / SetTutorialReason -> CSWGuiTutorialBox) reached from
// CSWGuiSkillsCharGen::OnPanelAdded 006F49A0 (ShowTutorialWindow 0x11). Original: swkotor.exe GOG K1, static only.
//   CSWGuiPanel      +18 CSWGuiManager*, +44 flags (bit 2 visible), +5C CSWGuiBorder* (0040A874, 0040B9DB..0040B9E8)
//   CSWGuiScene      +1C model array, +20 model count (00416758..00416760)
//   CSWGuiFade       +0C/+10 size, +50 colour, +64/+68 args, +6C 0.1f, +70 fade_in, +1B4 start ms (006244E9..0062455E)
//   CGuiInGame       +38 CSWGuiManager*, +6C CSWGuiFade*, +A0 CSWGuiTutorialBox*, +B4 tutorial suppressed,
//                    +BA8 shown-reason bitset (0062ABF3..0062F477)
//   CSWGuiLoadScreen +29C LBL_HINT text params, +574 hint strref (0067A9F4, 0067AA49)
//   CSWGuiTutorialBox +990 paused-by-box flag, +994 reason, +995 message index (006AA233, 006AA908..006AA90E)
#include "b8824978385052ca_m15_l0_gui.h"
#include "c9d0373605f94236_mainmenu.h"          // gui-load: gui_load_messagebox_set_callback_00624a40, kotor_fastcall_0040acc0
#include "e1805047c9b7563b_startup_string.h"
#include "232ef6efbadb2bc2_startup_views.h"     // kotor_menu_10_19::{Tlk, String} (0041E8F0 GetSimpleString)
#include <cstring>

extern "C" {
extern void* kotor_cell_tlk_007a3a08;                                                    // [007A3A08] CTlkTable* (lane-tlk)
extern void* kotor_cell_rules_007a3a28;                                                  // [007A3A28] CSWRules* (lane-prepare)
menu_resources::Manager* __cdecl c2da_manager_007a39e8();                                // [007A39E8] CExoResMan*
void __fastcall frame_LogStringCtor_005e5a90(kotor_reconstruction::StartupString*, void*, const char*) noexcept(false); // CExoString(char const*)
void* __fastcall kotor_fastcall_005ed700(void* client_app, void*);                       // CClientExoApp::GetClientOptions
void __fastcall kotor_fastcall_0062b680(void* in_game, void*, kotor_reconstruction::StartupString* text,
                                        kotor_reconstruction::StartupString* speaker);   // CGuiInGame::AppendToDialogBuffer
void __fastcall kotor_fastcall_005edc20(void* client, void*, std::uint32_t paused, std::uint32_t reason, std::uint32_t force); // SetPausedByCombat
std::int32_t __fastcall c2da_current_get_int_00413660(void* table, void*, std::int32_t row,
                                                      kotor_reconstruction::StartupString* column, std::int32_t* out); // C2DA::GetINTEntry
}
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk* self, void* unused_edx, kotor_reconstruction::StartupString* text); }
namespace kotor_input_required { unsigned __int64 __fastcall get_snapshot_time_004adc70(void* timer, void*); } // CWorldTimer::GetSnapshotTime
extern "C" std::int32_t __fastcall gui_m48_tutorial_box_set_next_message_006aa670(void* box, void*) noexcept(false); // SetNextMessage

namespace {
using U = std::uint32_t;
using I32 = std::int32_t;
using String = kotor_reconstruction::StartupString;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }                // [007A39FC]
void* client_app() { return rp(app_manager(), 4); }                                     // [[007A39FC]+4]
const char kMessage0[] = "Message0";                                                     // 00755D54
const char kEmpty[] = "";                                                                // 0073D71C
// Panel method-table slot with one stack word (vcall thunk targets): raw table word read FROM the table.
using PanelWordSlot = void (__fastcall*)(void*, void*, U);
}

namespace kotor_m15_l0 {
// ---- 00644720: MOV EAX,[ECX]; JMP [EAX+5Ch] (same ECX and stack word).
void __thiscall PanelEventThunks15::OnYButtonPressed_00644720(kih::Control* control) {
    void* const self = this;
    PanelWordSlot slot; std::memcpy(&slot, at(rp(self, 0), 0x5C), 4);                   // 00644720 MOV EAX,[ECX]
    slot(self, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(control)));      // 00644722 JMP [EAX+5Ch]
}
// ---- 006AA230 CSWGuiTutorialBox::MessageBoxUnpause(control), RET 4.
void __thiscall TutorialBoxHandlers::MessageBoxUnpause_006aa230(kih::Control*) {
    void* const self = this;
    if (rd(self, 0x990) == 0)                                                            // 006AA233..006AA23B
        kotor_fastcall_005edc20(client_app(), nullptr, 0, 2, 0);                         // 006AA23D..006AA24B
    wr(self, 0x990, 0);                                                                  // 006AA250
}
}

extern "C" {
// ---- 0040A870 CSWGuiPanel::SetBackground(const CResRef&), RET 4: border(+5C)+14h params -> SetFillImage(ref, 0).
void __fastcall kotor_m15_panel_set_background_0040a870(void* panel, void*, const void* ref) noexcept(false) {
    gui_load_border_params_set_fill_00414c00(at(rp(panel, 0x5C), 0x14), nullptr,
                                             static_cast<const gui_load::Ref*>(ref), 0);  // 0040A870..0040A87D
}
// ---- 0040B9D0 CSWGuiPanel::SetVisible(int), RET 4: flags bit 2 = bit 0 of the argument; manager redraw check.
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void* panel, void*, U visible) noexcept(false) {
    const U flags = rd(panel, 0x44);                                                     // 0040B9DB
    wr(panel, 0x44, flags ^ (((visible << 2) ^ flags) & 4u));                            // 0040B9D4..0040B9E5 (LEA x4 / XOR / AND 4 / XOR)
    if (void* manager = rp(panel, 0x18)) kotor_fastcall_0040acc0(manager, nullptr);     // 0040B9E8..0040B9EF
}
// ---- 00416750 CSWGuiScene::GetModel(int), RET 4.
void* __fastcall kotor_m15_gui_scene_get_model_00416750(void* scene, void*, I32 index) {
    if (index < 0 || index >= static_cast<I32>(rd(scene, 0x20))) return nullptr;        // 00416754..0041675B / 00416766
    return rp(rp(scene, 0x1C), static_cast<unsigned>(index) * 4u);                       // 0041675D..00416760
}
// ---- 006244E0 CSWGuiFade::SetFade(fade_in, a2, a3, const Vector* colour), RET 10h.
void __fastcall kotor_m15_fade_set_fade_006244e0(void* fade, void*, U fade_in, U a2, U a3, const void* color) noexcept(false) {
    std::memcpy(at(fade, 0x50), color, 12);                                              // 006244E3..006244F7
    void* const manager = rp(fade, 0x18);                                                // 006244FA
    std::int16_t w; std::memcpy(&w, at(manager, 0x6C), 2);
    wr(fade, 0x0C, static_cast<U>(static_cast<I32>(w)));                                 // 006244FD..00624501 (MOVSX)
    std::memcpy(&w, at(manager, 0x6E), 2);
    wr(fade, 0x10, static_cast<U>(static_cast<I32>(w)));                                 // 00624504..0062450F (MOVSX)
    gui_load_panel_set_alpha_00404b00(static_cast<gui_load::Panel*>(fade), nullptr,
                                      fade_in != 0 ? 1.0f : 0.0f);                       // 0062450D..0062451F (PUSH 3F800000h / PUSH 0)
    wr(fade, 0x64, a2);                                                                  // 00624524 / 0062452C
    wr(fade, 0x68, a3);                                                                  // 00624528 / 0062452F
    wr(fade, 0x70, fade_in);                                                             // 00624532
    wr(fade, 0x6C, 0x3DCCCCCDu);                                                         // 00624535 (0.1f)
    void* const timer = rp(rp(rp(app_manager(), 4), 4), 0x2C);                           // 0062453C..00624547 ([[[[7A39FC]+4]+4]+2C])
    const unsigned __int64 now = kotor_input_required::get_snapshot_time_004adc70(timer, nullptr); // 0062454A
    wr(fade, 0x1B4, static_cast<U>(static_cast<__int64>(now) / 1000));                   // 0062454F..0062455E (__alldiv, signed)
}
// ---- 0062ABF0 CGuiInGame::StartGlobalFade(fade_in, a2, a3, colour), RET 10h.
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void* in_game, void*, U fade_in, U a2, U a3,
                                                            const void* color) noexcept(false) {
    void* const fade = rp(in_game, 0x6C);                                                // 0062ABF3
    if (fade == nullptr) return;                                                         // 0062ABF6..0062ABF8
    kotor_fastcall_0040bc70(rp(in_game, 0x38), nullptr, fade, 4, 1);                     // 0062ABFA..0062AC02 AddPanel
    kotor_m15_panel_set_visible_0040b9d0(rp(in_game, 0x6C), nullptr, 1);                 // 0062AC07..0062AC0C
    kotor_m15_fade_set_fade_006244e0(rp(in_game, 0x6C), nullptr, fade_in, a2, a3, color); // 0062AC11..0062AC28
}
// ---- 0062B0D0 CGuiInGame::SetTutorialReason(reason), RET 4: no tutorial box -> 0, else tail JMP 006AA900.
I32 __fastcall kotor_m15_ingame_set_tutorial_reason_0062b0d0(void* in_game, void*, U reason) noexcept(false) {
    void* const box = rp(in_game, 0xA0);                                                 // 0062B0D0
    if (box == nullptr) return 0;                                                        // 0062B0D6..0062B0DC
    return kotor_m15_tutorial_box_set_tutorial_reason_006aa900(box, nullptr, reason);    // 0062B0DF
}
// ---- 0062F420 CGuiInGame::TutorialReasonWillShowWindow(reason), RET 4.
I32 __fastcall kotor_m15_ingame_tutorial_reason_will_show_0062f420(void* in_game, void*, U reason) noexcept(false) {
    void* const options = kotor_fastcall_005ed700(client_app(), nullptr);               // 0062F420..0062F42B
    if ((*at(options, 0x14) & 2u) == 0) return 0;                                        // 0062F430..0062F434 (tutorial option)
    const unsigned char r = static_cast<unsigned char>(reason);                          // 0062F436 DL
    if (r < 0x2B) {                                                                      // 0062F43A..0062F43D
        const unsigned char bits = *at(in_game, 0xBA8u + (r >> 3));                      // 0062F440..0062F45B
        if (((1u << (r & 7u)) & bits) != 0) return 0;                                    // 0062F44A..0062F46B (already shown)
    }
    if (rd(in_game, 0xB4) != 0) return 0;                                                // 0062F46D..0062F475
    if (kotor_m15_tutorial_box_reason_is_valid_006aa1a0(rp(in_game, 0xA0), nullptr, reason) == 0) return 0; // 0062F477..0062F485
    return 1;                                                                            // 0062F487
}
// ---- 0067A6D0 CSWGuiLoadScreen::SetImage(const CResRef&), RET 4: background only if a TGA (3) or TPC (0BBFh) exists.
void __fastcall kotor_m15_load_screen_set_image_0067a6d0(void* screen, void*, const void* ref) noexcept(false) {
    menu_resources::Ref* const r = static_cast<menu_resources::Ref*>(const_cast<void*>(ref));
    if (c2da_manager_007a39e8()->exists_00408bc0(r, 3, nullptr) == 0 &&                 // 0067A6D6..0067A6EA
        c2da_manager_007a39e8()->exists_00408bc0(r, 0x0BBF, nullptr) == 0)               // 0067A6EC..0067A700 (out = EAX = 0)
        return;
    kotor_m15_panel_set_background_0040a870(screen, nullptr, ref);                       // 0067A702..0067A705
}
// ---- 0067A9C0 CSWGuiLoadScreen::SetHint(strref), RET 4. EH 0072E1B0: state 0 = text, state 1 = fetched temporary.
void __fastcall kotor_m15_load_screen_set_hint_0067a9c0(void* screen, void*, U strref) noexcept(false) {
    String text;                                                                         // 0067A9DB..0067A9DF
    if (strref != 0) {                                                                   // 0067A9E4..0067A9F2
        wr(screen, 0x574, strref);                                                       // 0067A9F4
        auto* const tlk = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08);
        alignas(4) unsigned char fetched[8];                                             // [esp+0Ch] returned CExoString
        (void)tlk->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(fetched), strref); // 0067A9FA..0067AA06
        try { text.Assign005e5c50(reinterpret_cast<String*>(fetched)); }                 // 0067AA0B..0067AA15
        catch (...) { reinterpret_cast<String*>(fetched)->~String(); throw; }
        reinterpret_cast<String*>(fetched)->~String();                                   // 0067AA1A..0067AA23
        tlk_core::ParseStr0041dd30(tlk, nullptr, &text);                                 // 0067AA28..0067AA33
    } else {
        wr(screen, 0x574, 0);                                                            // 0067AA3A
    }
    gui_load_text_params_set_text_00415e00(at(screen, 0x29C), nullptr, &text);           // 0067AA44..0067AA4F
}                                                                                        // ~text 0067AA54..0067AA60
// ---- 0067AA80 CSWGuiLoadScreen::AddHintToMessageBuffer, RET 0: AppendToDialogBuffer(hint text, "").
void __fastcall kotor_m15_load_screen_add_hint_to_buffer_0067aa80(void* screen, void*) noexcept(false) {
    alignas(String) unsigned char storage[sizeof(String)];                               // [esp+4]
    String* const speaker = reinterpret_cast<String*>(storage);
    frame_LogStringCtor_005e5a90(speaker, nullptr, kEmpty);                              // 0067AA9B..0067AAA4
    struct Guard { String* s; ~Guard() { s->~String(); } } guard{speaker};               // EH 0072E1C8 state 0 / 0067AAD1..0067AADD
    void* const in_game = frame_R4_005ed690(client_app(), nullptr);                  // 0067AAA9..0067AAC5 (mainmenu.h decl)
    kotor_fastcall_0062b680(in_game, nullptr, reinterpret_cast<String*>(at(screen, 0x29C)), speaker); // 0067AAB1..0067AACC
}
// ---- 006AA1A0 CSWGuiTutorialBox::TutorialReasonIsValid(reason), RET 4: tutorial 2DA row (byte) column "Message0".
I32 __fastcall kotor_m15_tutorial_box_reason_is_valid_006aa1a0(void*, void*, U reason) noexcept(false) {
    I32 value;                                                                           // [esp+4] (written by GetINTEntry)
    String column(kMessage0);                                                            // 006AA1B9..006AA1C2
    void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x118);                  // 006AA1C7..006AA1D2
    const I32 found = c2da_current_get_int_00413660(table, nullptr, static_cast<I32>(reason & 0xFFu), &column, &value); // 006AA1D8..006AA1F2
    return found != 0 ? 1 : 0;                                                           // 006AA1F7..006AA218 (NEG/SBB/INC; SETE)
}                                                                                        // ~column 006AA1FD..006AA20B
// ---- 006AA900 CSWGuiTutorialBox::SetTutorialReason(reason), RET 4.
// Jump table 006AAA6C (12 cases) indexed by byte table 006AAA9C (reasons 00h..27h); anything else -> MessageBoxUnpause.
I32 __fastcall kotor_m15_tutorial_box_set_tutorial_reason_006aa900(void* box, void*, U reason) noexcept(false) {
    using H = kotor_m15_l0::TutorialBoxHandlers;
    const unsigned char r = static_cast<unsigned char>(reason);                          // 006AA901 MOV BL,[ESP+8]
    *at(box, 0x994) = r;                                                                 // 006AA908
    *at(box, 0x995) = 0;                                                                 // 006AA90E
    if (gui_m48_tutorial_box_set_next_message_006aa670(box, nullptr) == 0) return 0;     // 006AA915..006AA920
    gui_load::EventFunc func;
    switch (r) {                                                                         // 006AA923..006AA936
    case 0x00: func = kotor_m15_l0::event_func(&H::PerformCombatFeat_006aa260); break;   // 006AA93D (case 0)
    case 0x01: func = kotor_m15_l0::event_func(&H::PerformUseGrenade_006aa350); break;   // 006AA956 (case 1)
    case 0x02: func = kotor_m15_l0::event_func(&H::PerformSetMine_006aa3d0); break;      // 006AA96F (case 2)
    case 0x03: case 0x04:
               func = kotor_m15_l0::event_func(&H::PerformForcePower_006aa2c0); break;   // 006AA988 (case 3)
    case 0x1D: func = kotor_m15_l0::event_func(&H::PerformNonCombatXButton_006aa590); break; // 006AA9EC (case 4)
    case 0x1E: func = kotor_m15_l0::event_func(&H::PerformNonCombatXButton_006aa590); break; // 006AAA05 (case 5)
    case 0x1F: func = kotor_m15_l0::event_func(&H::PerformPlayerFlourishWeapons_006aa440); break; // 006AA9A1 (case 6)
    case 0x20: func = kotor_m15_l0::event_func(&H::PerformCombatYButton_006aa5d0); break; // 006AAA1E (case 7)
    case 0x21: func = kotor_m15_l0::event_func(&H::PerformBash_006aa480); break;         // 006AA9BA (case 8)
    case 0x22: func = kotor_m15_l0::event_func(&H::PerformAttack_006aa520); break;       // 006AA9D3 (case 9)
    case 0x27: func = kotor_m15_l0::event_func(&H::PerformReturnToBase_006aa620); break; // 006AAA37 (case 0Ah)
    default:   func = kotor_m15_l0::event_func(&H::MessageBoxUnpause_006aa230); break;   // 006AAA50 (case 0Bh; > 27h too)
    }
    gui_load_messagebox_set_callback_00624a40(box, nullptr, static_cast<kotor_input_hover::GuiObject*>(box), func, 0); // PUSH 0; PUSH func; PUSH ESI
    return 1;                                                                            // MOV EAX,1
}
}
