// Lane W4 G15 (gui-hud, part 2): in-game menu bar / messages / status summary / pause / solo-mode query /
// area-transition panel slots, replacing the gui_slot_traps_successor.cpp trap lines of the same names/signatures.
// Transcribed from linear-sweep listings of swkotor.exe (claude-runs/tools/win.py <VA>); PCs cited per statement.
//
//   00624C60 CSWGuiInGameMenu::Update(float)                          thiscall RET 4
//   00624970 CSWGuiInGameMenu::HandleInputEvent(int, int)             thiscall RET 8
//   00626D90 CSWGuiInGameMessages::OnPanelAdded()                     thiscall RET 0
//   00628260 CSWGuiInGameMessages::HandleInputEvent(int, int)         thiscall RET 8
//   00625C60 CSWGuiStatusSummary::OnPanelAdded()                      thiscall RET 0
//   00624A60 CSWGuiStatusSummary::OnPanelRemoved()                    thiscall RET 0
//   00625AC0 CSWGuiStatusSummary::HandleInputEvent(int, int)          thiscall RET 8
//   006C02F0 CSWGuiInGamePause::OnPanelAdded()                        thiscall RET 0
//   006C2310 CSWGuiInGameSoloModeQuery::Draw(float)                   thiscall RET 4
//   006C2400 CSWGuiInGameSoloModeQuery::HandleInputEvent(int, int)    thiscall RET 8
//   006C8050 CSWGuiInGameAreaTransition::Draw(float)                  thiscall RET 4
// Callees reconstructed here (no rebuilt body existed in the c24 candidate):
//   00676470 CSWCJournal::HasNewEntries() -> int                                     RET 0
//   00624DF0 CSWGuiInGameMessages::ShowDialogMessages()                              RET 0
//   00624F70 CSWGuiInGameMessages::ShowFeedbackMessages()                            RET 0
//   00626920 CSWGuiInGameMessages::AddMessages(entry*, unsigned count)               RET 8
//   00626B10 CSWGuiInGameMessages::AddDialogMessages(entry*, unsigned count)         RET 8
//   0062AD60 CGuiInGame::UpdateMessageGUI()                                          RET 0
//   0062B100 CGuiInGame::SetupPauseGuiExtent(CSWGuiExtent*)                          RET 4
//   00688DB0 CSWGuiMainInterface::SetupPauseGuiExtent(CSWGuiExtent*)                 RET 4
//   0062E6D0 CGuiInGame::HideSoloMode()                                              RET 0
//   0062EBE0 CGuiInGame::GetBarkBubbleVisible(int*) -> int                           RET 4
//   005EDE60 CClientExoApp::TogglePartyFollow()                                      RET 0
//   005F2A20 CClientExoAppInternal::TogglePartyFollow()                              RET 0
//   00565500 CSWPartyTable::SetSoloMode(int, int)                                    RET 8
// Original quirks kept: AddDialogMessages calls free() on the (null) list buffer even when count == 0; the
// Messages input handler keeps the "Punch it, Chewie!" key-sequence easter egg (sequence state [00833900],
// camera toggle [008338E8]); StatusSummary::OnPanelAdded reuses one text string across the nine status rows.
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; }               // [007A39F4] CSWGuiManager*
namespace kotor_input_cells_required {
extern volatile int camera_toggle_008338e8;                                                      // [008338E8]
extern volatile int camera_sequence_00833900;                                                    // [00833900]
}
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };                 // CTlkTable::GetSimpleString
}
namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned count); } // CExoArrayList::Allocate
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // CExoString::operator!=(const char*)
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }  // CServerExoApp::GetPartyTable

extern "C" {
// ---- panel / manager / controls
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, U32 id, U32 down);      // CSWGuiPanel::HandleInputEvent
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                           // CSWGuiPanel::OnPanelAdded
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, U32 delta_bits);                   // CSWGuiPanel::Draw
void __fastcall gui_m48_panel_set_to_destroy_00624a00(void* panel, void*, U32 flag);            // CSWGuiPanel::setToDestroy
void __fastcall gui_m48_message_box_handle_input_006250f0(void* box, void*, U32 id, U32 down);   // CSWGuiMessageBox::HandleInputEvent
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                        // CSWGuiManager::PlayGuiSound
void* __fastcall gui_m48_manager_store_mouse_0040a3d0(void* manager, void*);                    // CSWGuiManager::StoreCurrentMousePosition
void __fastcall gui_m48_manager_move_mouse_0040c790(void* manager, void*, I32 x, I32 y);         // CSWGuiManager::MoveMouseToPosition
void* __fastcall gui_m48_manager_restore_mouse_0040c7d0(void* manager, void*);                  // CSWGuiManager::RestoreOldMousePosition
void* __fastcall kotor_fastcall_0040be00(void* manager, void*);                                  // CSWGuiManager::PopModalPanel
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, U32, U32, U32); // CSWGuiBorderParams::SetPulsingAlpha
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
std::int32_t __fastcall gui_load_border_params_dim_00414cd0(void* border, void*);               // CSWGuiBorderParams::GetBorderDim
std::int32_t __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                  // CSWGuiText::GetFontHeight
void __fastcall gui_m48_text_params_set_font_rename_00415df0(void* params, void*);              // CSWGuiTextParams::SetFontRename
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U32 suppress, U32 restore, U32 variable); // CSWGuiListBox::AddControls
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, I32 index, I32 flag); // CSWGuiListBox::SetSelectedControl
void __fastcall exo_array_list_destruct_0056b6f0(void* list, void*);                             // ~CExoArrayList (EH unwind)
// ---- client / in-game gui
void __fastcall kotor_fastcall_005eda60(void* client, void*, U32 input_class, U32 show);          // CClientExoApp::SetInputClass
void __fastcall kotor_fastcall_005edc20(void* client, void*, U32 a, U32 b, U32 c);               // CClientExoApp::SetPausedByCombat
U32 __fastcall show_tutorial_window_005edf40(void* client, void*, U32 window, U32 a, U32 b, U32 c); // CClientExoApp::ShowTutorialWindow
void* __fastcall kotor_fastcall_005edeb0(void* client, void*, void* out, U32 strref);             // CClientExoApp::GetGUIString
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* client, void*);                 // CClientExoApp::GetMiniGame
void* __fastcall kotor_fastcall_005ed320(void* client, void*);                                   // CClientExoApp::GetQuestJournal
void* __fastcall client_get_player_creature_005ed540(void* client, void*);                      // CClientExoApp::GetPlayerCreature
U32 __fastcall kotor_m15_client_get_area_not_ready_005edad0(void* client, void*);               // CClientExoApp::GetAreaNotReadyToDisplay
void* __fastcall kotor_fastcall_005ed6f0(void* client, void*);                                   // CClientExoApp::GetSWCMessage
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                                   // CClientExoApp::GetSWParty
U32 __fastcall frame_R4_0062cba0(void* in_game, void*, U32 keep_paused);                        // CGuiInGame::HideSWInGameGui
void __fastcall feedback_append_0062b5c0(void* in_game, void*, void* text, U32 color, U32 flag);  // CGuiInGame::AppendToMsgBuffer
void __fastcall kotor_fastcall_0062b0b0(void* in_game, void*, U32 index);                        // CGuiInGame::ShowFlashingStatus
U32 __fastcall frame_TogglePauseRequest_00677800(void* message, void*);                          // CSWCMessage::SendPlayerToServerInput_TogglePauseRequest
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);       // CSWCCreature::GetServerCreature
void __fastcall kotor_fastcall_0060ee00(void* creature, void*, U32 skill, U32 a, U32 target);    // CSWCCreature::PerformSkill
void* __fastcall frame_GetServerArea_00605640(void* area, void*);                                // CSWCArea::GetServerArea
U32 __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);                 // CSWSCreature::GetDeadTemp
U32 __fastcall kotor_fastcall_00635350(void* party, void*);                                      // CSWParty::IsPartyTogether
// ---- server / party
void* __fastcall recovered_party_repository_00563340(void* table, void*);                        // CSWPartyTable::GetPartyItems
void __fastcall recovered_unstealth_party_00563c60(void* party, void*, U32 flag);               // CSWPartyTable::UnstealthParty
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
// ---- strings / tlk
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, std::int32_t value) noexcept(false);  // CExoString::CExoString(int)
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, U32 token, void* value);               // CTlkTable::SetCustomToken
std::int32_t __fastcall async_string_length_005e5790(void* self, void*);                         // CExoString::GetLength
void __fastcall gui_load_text_params_set_strref_00415e50(void* params, void*, U32 strref) noexcept(false); // SetStrRef
// ---- .data colours (3 floats each), bound by their rebuilt storage
extern const unsigned char kotor_m1_color_007a2378[12];                                           // [007A2378]
extern const unsigned char kotor_m1_color_007a2384[12];                                           // [007A2384]
extern const unsigned char borrowed_exit_color_007a23b4[12];                                      // [007A23B4]
extern const unsigned char kotor_m15d_color_007a23d8[12];                                         // [007A23D8]
extern const unsigned char kotor_m1_color_007a2408[12];                                           // [007A2408]
// ---- this unit
void __fastcall kotor_fastcall_00624c60(void* self, void*, U32 delta_bits);
void __fastcall kotor_fastcall_00624970(void* self, void*, U32 id, U32 down);
void __fastcall kotor_fastcall_00626d90(void* self, void*);
void __fastcall kotor_fastcall_00628260(void* self, void*, U32 id, U32 down);
void __fastcall kotor_fastcall_00625c60(void* self, void*);
void __fastcall kotor_fastcall_00624a60(void* self, void*);
void __fastcall kotor_fastcall_00625ac0(void* self, void*, U32 id, U32 down);
void __fastcall kotor_fastcall_006c02f0(void* self, void*);
void __fastcall kotor_fastcall_006c2310(void* self, void*, U32 delta_bits);
void __fastcall kotor_fastcall_006c2400(void* self, void*, U32 id, U32 down);
void __fastcall kotor_fastcall_006c8050(void* self, void*, U32 delta_bits);
U32 __fastcall kotor_fastcall_00676470(void* journal, void*);
void __fastcall kotor_fastcall_00624df0(void* self, void*);
void __fastcall kotor_fastcall_00624f70(void* self, void*);
void __fastcall kotor_fastcall_00626920(void* self, void*, void* entries, U32 count);
void __fastcall kotor_fastcall_00626b10(void* self, void*, void* entries, U32 count);
void __fastcall kotor_fastcall_0062ad60(void* in_game, void*);
void __fastcall kotor_fastcall_0062b100(void* in_game, void*, void* extent);
void __fastcall kotor_fastcall_00688db0(void* main_interface, void*, void* extent);
void __fastcall kotor_fastcall_0062e6d0(void* in_game, void*);
U32 __fastcall kotor_fastcall_0062ebe0(void* in_game, void*, I32* bottom);
void __fastcall kotor_fastcall_005ede60(void* client, void*);
void __fastcall kotor_fastcall_005f2a20(void* internal, void*);
void __fastcall kotor_fastcall_00565500(void* party, void*, U32 solo, U32 unstealth);
}

namespace {
void* gui_manager() { return kotor_menu_window_cells::gui_manager_007a39f4; }                    // [007A39F4]
void* in_game_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }                     // [[7A39FC]+4]->GetInGameGui
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); } // [007A3A08]
// A stack CExoString destroyed at scope end (normally or while unwinding) once constructed.
struct Str {
    U32 s[2];
    bool live = false;
    Str() = default;
    Str(const Str&) = delete;
    Str& operator=(const Str&) = delete;
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }
    void born() { live = true; }
    void* p() { return s; }
};
void* simple(Str& out, U32 strref) {                                                             // CTlkTable::GetSimpleString
    void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out.p()), strref);
    out.born();
    return r;
}
void* gui_string(Str& out, U32 strref) {                                                         // CClientExoApp::GetGUIString
    void* r = kotor_fastcall_005edeb0(client_app(), nullptr, out.p(), strref);
    out.born();
    return r;
}
void* concat(void* lhs, Str& out, const void* rhs) {                                             // CExoString::operator+
    void* r = GuiConcat005e5d10(static_cast<SS*>(lhs), nullptr, static_cast<SS*>(out.p()), static_cast<const SS*>(rhs));
    out.born();
    return r;
}
void assign(void* dst, void* src) { async_assign_string_005e5c50(dst, nullptr, src); }            // CExoString::operator=
// Hide a panel control: flags &= ~100h; unless (flags & 600h) == 400h, flags = (flags & ~400h) | 200h.
void hide_flags(void* control) {
    const U32 flags = lw(control, 0x44) & ~0x100u;
    sw(control, 0x44, flags);
    if ((flags & 0x600u) != 0x400u) sw(control, 0x44, (flags & ~0x400u) | 0x200u);
}
// CExoArrayList<CSWGuiControl*> on the stack: {data, size, allocated}.
struct ControlList { U32 data = 0, size = 0, allocated = 0; };
// "Punch it, Chewie!" encoded at [007501B4] (byte i stored + (i & 3)); 16 dwords+word copied, 17 bytes decoded.
const unsigned char kChewieEncoded[18] = {'P', 'v', 'p', 'f', 'h', '!', 'k', 'w', ',', '!', 'E', 'k', 'e', 'x', 'k', 'h', '!', 0};
// Shared body of ShowDialogMessages / ShowFeedbackMessages (00624E5B..00624F50 / 00624FDB..006250D0).
void set_messages_title(void* self, U32 label_strref) {
    Str title;                                                                                 // [esp+08] (EH 0)
    exo_default(title.p()); title.born();                                                      // 00624E5B..00624E66
    {
        Str sep, label, kind, joined, full;                                                    // [esp+10] [esp+30] [esp+28] [esp+20] [esp+18]
        frame_LogStringCtor_005e5a90(sep.p(), nullptr, " - "); sep.born();                     // 00624E6B..00624E7C (00748E98)
        void* name = simple(label, label_strref);                                              // 00624E81..00624E96 (edi)
        void* what = simple(kind, 0x61Bu);                                                     // 00624E9D..00624EB2
        void* a = concat(what, joined, sep.p());                                               // 00624EB7..00624EC8
        void* b = concat(a, full, name);                                                       // 00624ECD..00624EDA
        assign(title.p(), b);                                                                  // 00624EDF..00624EE9
    }                                                                                          // 00624EEE..00624F2F ~full ~joined ~kind ~label ~sep
    gui_load_text_params_set_text_00415e00(at(self, 0x70C), nullptr, title.p());               // 00624F34..00624F3F
}                                                                                              // 00624F44..00624F50 ~title
// Message list control layout shared by AddMessages / AddDialogMessages (the per-item tail).
void layout_message(void* control, I32* extent) {
    gui_m48_text_params_set_font_rename_00415df0(at(control, 0xE8), nullptr);                  // SetFontRename
    vcall<void, const void*>(control, 0x04, extent);                                           // SetExtent
    const I32 dim = gui_load_border_params_dim_00414cd0(at(control, 0x154), nullptr);          // GetBorderDim
    const I32 height = vcall<I32>(at(control, 0xD0), 0x08);                                    // text height
    extent[3] = height + dim * 2;                                                              // lea eax,[eax+ecx*2]
    vcall<void, const void*>(control, 0x04, extent);                                           // SetExtent
    vcall<void, U32>(control, 0x40, 0);                                                        // slot 40h (0)
}
}

extern "C" {
// ======================= CSWGuiInGameMenu =======================
// 00676470 CSWCJournal::HasNewEntries() (thiscall, RET 0).
U32 __fastcall kotor_fastcall_00676470(void* journal, void*) {
    const I32 count = li(journal, 0x24);                                                       // 00676471
    for (I32 i = 0; i < count; ++i)                                                            // 00676474..0067648B
        if ((lb(lp(journal, 0x20), 0x2C + static_cast<U32>(i) * 0x3Cu) & 4u) != 0) return 1;  // 0067647A..00676483
    return 0;                                                                                  // 0067648D
}
// 00624C60 CSWGuiInGameMenu::Update(float) (thiscall, RET 4): pulse the equip / journal buttons.
void __fastcall kotor_fastcall_00624c60(void* self, void*, U32) {
    void* items = recovered_party_repository_00563340(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), nullptr); // 00624C60..00624C72
    if (lw(items, 0x04) == 0)                                                                  // 00624C77..00624C7C (jbe)
        gui_load_border_params_set_pulsing_00414d00(at(self, 0x288), nullptr, 0, 0, 0);       // 00624C8F..00624C9B
    else if ((lb(self, 0x2A4) & 4u) == 0)                                                      // 00624C7E..00624C85
        gui_load_border_params_set_pulsing_00414d00(at(self, 0x288), nullptr, 1, 0, 1);       // 00624C87..00624C9B
    if (kotor_fastcall_00676470(kotor_fastcall_005ed320(client_app(), nullptr), nullptr) != 0) { // 00624CA0..00624CB7
        if ((lb(self, 0x974) & 4u) == 0)                                                       // 00624CB9..00624CC0
            gui_load_border_params_set_pulsing_00414d00(at(self, 0x958), nullptr, 1, 0, 1);   // 00624CC2..00624CCE
    } else {
        gui_load_border_params_set_pulsing_00414d00(at(self, 0x958), nullptr, 0, 0, 0);       // 00624CD7..00624CE3
    }
}
// 00624970 CSWGuiInGameMenu::HandleInputEvent(int, int) (thiscall, RET 8).
void __fastcall kotor_fastcall_00624970(void* self, void*, U32 id, U32 down) {
    if (id == 0xF3u) {                                                                         // 0062497B..00624984
        if ((down & 1u) != 0) vcall<void, U32, U32>(self, 0x3C, 0x35, 1);                      // 006249A6..006249B3
    } else if (id == 0xF4u) {                                                                  // 00624986..00624987
        if ((down & 1u) != 0) vcall<void, U32, U32>(self, 0x3C, 0x36, 1);                      // 00624989..00624994
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 00624997..0062499B / 006249B6..006249BA
}

// ======================= CSWGuiInGameMessages =======================
// 00626920 CSWGuiInGameMessages::AddMessages(entry*, unsigned) (thiscall, RET 8). entry = {CExoString, int, byte}.
void __fastcall kotor_fastcall_00626920(void* self, void*, void* entries, U32 count) {
    ControlList list;                                                                          // [esp+18..20] (00626940..00626948)
    void* listbox = at(self, 0x64);                                                            // 00626957 ([esp+14])
    I32 extent[4] = {0, 0, li(listbox, 0x294) - static_cast<I32>(lb(self, 0x324)) * 2, 0x0A};  // 0062694C..0062697C
    auto guard = on_throw([&]() { exo_array_list_destruct_0056b6f0(&list, nullptr); });        // EH state 0 (0056B6F0)
    if (count != 0) menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, count); // 00626984..0062698B
    list.size = count;                                                                         // 00626992
    for (U32 i = 0; i < count; ++i) {                                                          // 00626996 / 00626AA1..00626AB1
        void* entry = static_cast<U8*>(entries) + i * 0x10u;                                   // ebx
        void* control = static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u;
        gui_load_text_params_set_text_00415e00(at(control, 0xE8), nullptr, entry);             // 006269B0..006269BE
        gui_m48_text_params_set_font_rename_00415df0(at(static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u, 0xE8), nullptr); // 006269C3..006269D0
        U32 color[3];                                                                          // [esp+24]
        std::memcpy(color, lb(entry, 0x0C) == 1 ? kotor_m15d_color_007a23d8 : borrowed_exit_color_007a23b4, 12); // 006269D5..00626A1A
        gui_load_text_params_set_color_00414e10(at(static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u, 0xE8), nullptr, color); // 00626A08..00626A1E
        control = static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u;                              // 00626A23..00626A2C
        vcall<void, const void*>(control, 0x04, extent);                                       // 00626A2F..00626A34 SetExtent
        control = static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u;                              // 00626A37..00626A3D
        const I32 dim = gui_load_border_params_dim_00414cd0(at(control, 0x154), nullptr);      // 00626A40..00626A4C
        const I32 height = vcall<I32>(at(control, 0xD0), 0x08);                                // 00626A51..00626A59
        extent[3] = height + dim * 2;                                                          // 00626A5C..00626A69
        vcall<void, const void*>(static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u, 0x04, extent); // 00626A63..00626A77
        vcall<void, U32>(static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u, 0x40, 0);             // 00626A7A..00626A87
        sw(pt(list.data), i * 4u, ad(static_cast<U8*>(lp(self, 0x764)) + i * 0x1B4u));         // 00626A8A..00626A9A
    }
    if (count != 0) {                                                                          // 00626ABF..00626AC1
        kotor_fastcall_0041c1d0(listbox, nullptr, &list, 0, 0, 1);                             // 00626AC3..00626ACE AddControls
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, static_cast<I32>(count - 1u), 0); // 00626AD3..00626AD8
    }
    guard.armed = false;
    live_free(pt(list.data));                                                                  // 00626ADD..00626AEA
}
// 00626B10 CSWGuiInGameMessages::AddDialogMessages(entry*, unsigned) (thiscall, RET 8). entry = {text, speaker}.
void __fastcall kotor_fastcall_00626b10(void* self, void*, void* entries, U32 count) {
    ControlList list;                                                                          // [esp+3C..44] (00626B35..00626B3D)
    void* listbox = at(self, 0x344);                                                           // 00626B48 ([esp+18])
    I32 extent[4] = {0, 0, li(listbox, 0x294) - static_cast<I32>(lb(self, 0x604)) * 2, 0x0A};  // 00626B41..00626B72
    auto guard = on_throw([&]() { exo_array_list_destruct_0056b6f0(&list, nullptr); });        // EH state 0 (0056B6F0)
    {
        Str line;                                                                              // [esp+1C] (EH 1)
        exo_default(line.p()); line.born();                                                    // 00626B5A..00626B7A
        if (count == 0) {                                                                      // 00626B7F..00626B89
            if (list.data != 0) { live_free(pt(list.data)); list.data = 0; list.allocated = 0; } // 00626B8B..00626BA0
        } else {
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, count); // 00626BA6..00626BAB
        }
        list.size = count;                                                                     // 00626BB0..00626BB5
        for (U32 i = 0; i < count; ++i) {                                                      // 00626BBD / 00626D09..00626D20
            void* entry = static_cast<U8*>(entries) + i * 0x10u;                               // ebx ([esp+10])
            assign(line.p(), at(entry, 8));                                                    // 00626BD0..00626BD8 line = speaker
            if (async_string_length_005e5790(line.p(), nullptr) > 0) {                         // 00626BDD..00626BE8 (jle)
                Str sep, named, full;                                                          // [esp+24] [esp+34] [esp+2C] (EH 2..4)
                frame_LogStringCtor_005e5a90(sep.p(), nullptr, ": "); sep.born();             // 00626BEA..00626BF3 (00748ED0)
                void* a = concat(line.p(), named, sep.p());                                    // 00626BF8..00626C0B
                void* b = concat(a, full, entry);                                              // 00626C10..00626C1D
                gui_load_text_params_set_text_00415e00(at(static_cast<U8*>(lp(self, 0x768)) + i * 0x1B4u, 0xE8), nullptr, b); // 00626C22..00626C35
            } else {                                                                           // 00626C3A..00626C5F ~full ~named ~sep
                gui_load_text_params_set_text_00415e00(at(static_cast<U8*>(lp(self, 0x768)) + i * 0x1B4u, 0xE8), nullptr, entry); // 00626C66..00626C74
            }
            layout_message(static_cast<U8*>(lp(self, 0x768)) + i * 0x1B4u, extent);            // 00626C79..00626CEF
            sw(pt(list.data), i * 4u, ad(static_cast<U8*>(lp(self, 0x768)) + i * 0x1B4u));     // 00626CF2..00626D06
        }
        if (count != 0) {                                                                      // 00626D2E..00626D30
            kotor_fastcall_0041c1d0(listbox, nullptr, &list, 0, 0, 1);                         // 00626D32..00626D3D AddControls
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, static_cast<I32>(count - 1u), 0); // 00626D42..00626D4A
        }
    }                                                                                          // 00626D4F..00626D58 ~line
    guard.armed = false;
    live_free(pt(list.data));                                                                  // 00626D5D..00626D6A
}
// 0062AD60 CGuiInGame::UpdateMessageGUI() (thiscall, RET 0).
void __fastcall kotor_fastcall_0062ad60(void* in_game, void*) {
    kotor_fastcall_00626920(lp(in_game, 0x1C), nullptr, lp(in_game, 0xF8), lw(in_game, 0x100)); // 0062AD63..0062AD74 AddMessages
    kotor_fastcall_00626b10(lp(in_game, 0x1C), nullptr, lp(in_game, 0xFC), lw(in_game, 0x104)); // 0062AD79..0062AD8A AddDialogMessages
}
// 00624DF0 CSWGuiInGameMessages::ShowDialogMessages() (thiscall, RET 0).
void __fastcall kotor_fastcall_00624df0(void* self, void*) {
    sw(self, 0xA8, lw(self, 0xA8) & ~2u);                                                      // 00624E11..00624E1B (feedback list +64h)
    sw(self, 0x388, lw(self, 0x388) | 2u);                                                     // 00624E21..00624E3B (dialog list +344h)
    gui_load_text_params_set_strref_00415e50(at(self, 0x8D8), nullptr, 0xA49Eu);               // 00624E30..00624E3E
    vcall<void, void*, U32>(self, 0x08, at(self, 0x344), 0);                                   // 00624E43..00624E4A
    sb(in_game_gui(), 0xBC8, 0);                                                               // 00624E4D..00624E5F
    set_messages_title(self, 0x173u);                                                          // 00624E5B..00624F50
}
// 00624F70 CSWGuiInGameMessages::ShowFeedbackMessages() (thiscall, RET 0).
void __fastcall kotor_fastcall_00624f70(void* self, void*) {
    sw(self, 0xA8, lw(self, 0xA8) | 2u);                                                       // 00624F91..00624F9B
    sw(self, 0x388, lw(self, 0x388) & ~2u);                                                    // 00624FA1..00624FBB
    gui_load_text_params_set_strref_00415e50(at(self, 0x8D8), nullptr, 0xA49Fu);               // 00624FB0..00624FBE
    vcall<void, void*, U32>(self, 0x08, at(self, 0x64), 0);                                    // 00624FC3..00624FCA
    sb(in_game_gui(), 0xBC8, 1);                                                               // 00624FCD..00624FDF
    set_messages_title(self, 0xA4B7u);                                                         // 00624FDB..006250D0
}
// 00626D90 CSWGuiInGameMessages::OnPanelAdded() (thiscall, RET 0).
void __fastcall kotor_fastcall_00626d90(void* self, void*) {
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 00626D93
    kotor_fastcall_0062ad60(in_game_gui(), nullptr);                                           // 00626D98..00626DA7 UpdateMessageGUI
    if (lb(in_game_gui(), 0xBC8) == 0) kotor_fastcall_00624df0(self, nullptr);                 // 00626DAC..00626DC6
    else kotor_fastcall_00624f70(self, nullptr);                                               // 00626DCD
    (void)show_tutorial_window_005edf40(client_app(), nullptr, 0x0C, 0x7F000000u, 0x7F000000u, 0); // 00626DD2..00626DE9
}
// 00628260 CSWGuiInGameMessages::HandleInputEvent(int, int) (thiscall, RET 8).
void __fastcall kotor_fastcall_00628260(void* self, void*, U32 id, U32 down) {
    if (down != 0) {                                                                           // 00628279..00628287
        switch (id) {                                                                          // 0062828D..0062829E (table 00628418)
        case 0x28: case 0x2D: case 0x2E: case 0xDF:                                            // 006282C8
            if (frame_R4_0062cba0(in_game_gui(), nullptr, 0) != 0)                             // 006282C8..006282E1
                kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                          // 006282E3..006282F0
            break;
        case 0x29:                                                                             // 006282A5
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                               // 006282A5..006282AA
            if ((lb(self, 0xA8) & 2u) != 0) kotor_fastcall_00624df0(self, nullptr);            // 006282AF..006282BA
            else kotor_fastcall_00624f70(self, nullptr);                                       // 006282C1
            break;
        default: break;                                                                        // 006282F5
        }
        const U32 sequence[4] = {0x29, 0x2F, 0x27, 0x29};                                      // 006282F5..0062830F [esp+0C]
        I32 step = kotor_input_cells_required::camera_sequence_00833900;                       // 00628302
        bool advance = true;
        if (id != sequence[step]) {                                                            // 00628317..0062831B
            step = 0;                                                                          // 0062831D
            advance = id == sequence[0];                                                       // 0062831F
            kotor_input_cells_required::camera_sequence_00833900 = 0;                          // 00628323
        }
        if (advance) {
            ++step;                                                                            // 0062832E
            kotor_input_cells_required::camera_sequence_00833900 = step;                       // 00628332
            if (step == 4) {                                                                   // 0062832F..00628337
                unsigned char text[18];                                                        // [esp+1C]
                std::memcpy(text, kChewieEncoded, sizeof(text));                               // 0062833D..0062836A
                kotor_input_cells_required::camera_sequence_00833900 = 0;                      // 0062836F
                for (U32 i = 0; i < 0x11u; ++i) text[i] = static_cast<unsigned char>(text[i] - (i & 3u)); // 00628379..00628393
                {
                    Str message;                                                               // [esp+0C] (EH 0)
                    frame_LogStringCtor_005e5a90(message.p(), nullptr, reinterpret_cast<const char*>(text)); message.born(); // 00628395..0062839E
                    feedback_append_0062b5c0(in_game_gui(), nullptr, message.p(), 0x80, 0);  // 006283A3..006283C7 AppendToMsgBuffer
                }                                                                              // 006283CC..006283D8
                kotor_input_cells_required::camera_toggle_008338e8 = 1;                        // 006283E2
                kotor_fastcall_0062ad60(in_game_gui(), nullptr);                               // 006283DD..006283F6 UpdateMessageGUI
            }
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 006283FB..006283FF
}

// ======================= CSWGuiStatusSummary =======================
// 00624A60 CSWGuiStatusSummary::OnPanelRemoved() (thiscall, RET 0): `jmp 0040C7D0` on [007A39F4].
void __fastcall kotor_fastcall_00624a60(void*, void*) {
    gui_m48_manager_restore_mouse_0040c7d0(gui_manager(), nullptr);                            // 00624A60..00624A66
}
// 00625AC0 CSWGuiStatusSummary::HandleInputEvent(int, int) (thiscall, RET 8).
void __fastcall kotor_fastcall_00625ac0(void* self, void*, U32 id, U32 down) {
    if (down != 0 && (id == 0x27 || id == 0x28 || id == 0x2D || id == 0x2E)) {                 // 00625ACB..00625AE0 (table 00625B38)
        gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0);                               // 00625AE7..00625AEB
        kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                                  // 00625AF0..00625AFD SetInputClass
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                                   // 00625B02..00625B07
        if ((lb(self, 0x7C) & 1u) == 0)                                                        // 00625B0C..00625B10
            kotor_fastcall_005edc20(client_app(), nullptr, 0, 0, 0);                           // 00625B12..00625B21 SetPausedByCombat
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 00625B26..00625B2A
}
// 00625C60 CSWGuiStatusSummary::OnPanelAdded() (thiscall, RET 0): lay out the nine status rows.
void __fastcall kotor_fastcall_00625c60(void* self, void*) {
    I32 y = 0x0A;                                                                              // [esp+14]
    I32 width = 0x96;                                                                          // [esp+28]
    U32 tutorial = 0;                                                                          // [esp+3C]
    Str text;                                                                                  // [esp+2C] (EH 0)
    exo_default(text.p()); text.born();                                                        // 00625C89..00625CA1
    I32 rect[4] = {0, 0, 0, 0};                                                                // [esp+18..24] (00625CB1..00625CBD)
    if ((lb(self, 0x64) & 0x20u) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0x0C); // 00625CA6..00625CC8
    if ((lb(self, 0x64) & 0x40u) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0x0D); // 00625CCD..00625CD8
    if ((lb(self, 0x64) & 0x80u) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0x0E); // 00625CDD..00625CE9 (jns)
    for (U32 row = 0; row < 9; ++row) {                                                        // 00625CEE / 00626258..00626260 ([esp+38])
        U32 flash = 1;                                                                         // [esp+34] (00625CFC)
        U32 has_text = 0;                                                                      // [esp+10] (00625D00)
        U32 show = 0;                                                                          // edi
        switch (row) {                                                                         // 00625D0A (table 006263D0)
        case 0:                                                                                // 00625D11
            show = (lw(self, 0x64) >> 4) & 1u;
            tutorial = show;                                                                   // 00625D19
            break;
        case 1: {                                                                              // 00625D22
            const I32 xp = li(self, 0x6C);
            if (xp == 0 && (lw(self, 0x64) & 0x100u) == 0) break;                              // 00625D25..00625D2F
            has_text = 1;                                                                      // 00625D37
            if (xp >= 0) {                                                                     // 00625D35..00625D3B (jl)
                { Str value; kotor_fastcall_005e5bc0(value.p(), nullptr, xp); value.born();     // 00625D3D..00625D42 (EH 1)
                  kotor_fastcall_0041db50(tlk(), nullptr, 0, value.p()); }                     // 00625D47..00625D6C SetCustomToken(0)
                Str got; void* r = gui_string(got, 0xA5C5u);                                   // 00625D71..00625D84 (EH 2)
                assign(text.p(), r);                                                           // 00625D89..00625D96
            } else {                                                                           // 00625DA1
                { Str value; kotor_fastcall_005e5bc0(value.p(), nullptr, static_cast<I32>(0u - static_cast<U32>(xp))); value.born(); // 00625DA1..00625DA8 (EH 3)
                  kotor_fastcall_0041db50(tlk(), nullptr, 0, value.p()); }                     // 00625DAD..00625DD2
                Str got; void* r = gui_string(got, 0xA683u);                                   // 00625DD7..00625DED (EH 4)
                assign(text.p(), r);                                                           // 00625DF2..00625DFF
            }                                                                                  // 00625E04..00625E13 ~got
            if ((lw(self, 0x64) & 0x100u) != 0) {                                              // 00625E18..00625E1E
                Str prefix; void* p = gui_string(prefix, 0xBB3Du);                             // 00625E24..00625E36 (EH 5)
                Str joined; void* j = concat(p, joined, text.p());                             // 00625E3B..00625E52 (EH 6)
                assign(text.p(), j);                                                           // 00625E57..00625E64
            }                                                                                  // 00625E69..00625E89 ~joined ~prefix
            show = 1;                                                                          // jmp 006260D7 (no edi test)
            break;
        }
        case 2: case 3: {                                                                      // 00625E93 / 00625F24
            const I32 value_word = li(self, row == 2 ? 0x70u : 0x74u);
            show = value_word != 0 ? 1u : 0u;                                                  // 00625E96..00625E9D
            if (show == 0) break;                                                              // 00625E9F..00625EA1
            has_text = 1;                                                                      // 00625EAC / 00625F3D
            { Str value; kotor_fastcall_005e5bc0(value.p(), nullptr, value_word); value.born(); // 00625EA7..00625EB0 (EH 7 / 9)
              kotor_fastcall_0041db50(tlk(), nullptr, 0, value.p()); }                         // 00625EB5..00625EDA
            Str got; void* r = gui_string(got, row == 2 ? 0xA5C6u : 0xA5C7u);                  // 00625EDF..00625EF4 (EH 8 / 10)
            assign(text.p(), r);                                                               // 00625EF9..00625F06
            break;                                                                             // 00625F0B..00625F1A ~got
        }
        case 4:                                                                                // 00625FAF
            show = lb(self, 0x69) > 0 ? 1u : 0u;                                               // 00625FB4..00625FBD
            flash = lb(self, 0x68) < lb(self, 0x69) ? 1u : 0u;                                 // 00625FBF..00625FC5
            break;
        case 5:                                                                                // 00625FCE
            show = lb(self, 0x68) > 0 ? 1u : 0u;                                               // 00625FD0..00625FDC
            flash = lb(self, 0x69) < lb(self, 0x68) ? 1u : 0u;                                 // 00625FD7..00625FE4
            break;
        case 6: {                                                                              // 00625FED
            const U8 a = lb(self, 0x68);
            show = (a > 0 && lb(self, 0x69) > 0 && a != lb(self, 0x69)) ? 1u : 0u;             // 00625FF0..00626005
            flash = 0;                                                                         // 00626007
            if (show == 0) break;                                                              // 0062600B
            has_text = 1;                                                                      // 00626014
            if (a > lb(self, 0x69)) {                                                          // 00626011..00626018 (jbe)
                { Str got; void* r = gui_string(got, 0xBB3Fu); assign(text.p(), r); }         // 0062601A..0062604F (EH 11)
                gui_load_border_params_set_fill_00414c00(at(self, 0xAF0), nullptr, at(self, 0x9F0), 0); // 00626054..00626062
            } else {
                { Str got; void* r = gui_string(got, 0xBB3Eu); assign(text.p(), r); }         // 00626069..006260A5 (EH 12)
                gui_load_border_params_set_fill_00414c00(at(self, 0xAF0), nullptr, at(self, 0x8B0), 0); // 006260AA..006260B8
            }
            break;
        }
        case 7: show = (lw(self, 0x64) >> 2) & 1u; break;                                      // 006260BF..006260CD
        default: show = (lw(self, 0x64) >> 3) & 1u; break;                                     // 006260C7..006260CD (row 8)
        }
        U8* entry = static_cast<U8*>(self) + row * 0x140u;                                     // 006260D7..006260DD / 00626235..00626242
        if (show != 0) {                                                                       // 006260CF..006260D1
            void* box = entry + 0x300;                                                         // ebx
            void* label = entry + 0xE40;                                                       // edi
            sw(entry, 0x344, lw(entry, 0x344) | 2u);                                           // 006260E0..006260F9
            sw(entry, 0xE84, lw(entry, 0xE84) | 2u);                                           // 006260FC..00626107
            if (has_text != 0) {                                                               // 00626101..0062610A
                gui_load_text_params_set_text_00415e00(entry + 0xF28, nullptr, text.p());      // 0062610C..00626117
                gui_m48_text_params_set_font_rename_00415df0(entry + 0xF28, nullptr);          // 0062611C..00626122
            }
            std::memcpy(rect, entry + 0x304, 16); rect[1] = y;                                 // 00626127..00626150
            vcall<void, const void*>(box, 0x04, rect);                                         // 00626154..00626159 SetExtent
            I32 w = width;                                                                     // 0062615C ebx
            std::memcpy(rect, entry + 0xE44, 16); rect[1] = y - 1; rect[2] = w;                // 00626160..00626193
            vcall<void, const void*>(label, 0x04, rect);                                       // 00626186..00626197 SetExtent
            if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(entry + 0xF28), nullptr, "") != 0 // 0062619A..006261AC (0073D71C)
                && w < 0x1B8) {                                                                // 006261AE..006261B4
                void* rendered = entry + 0xF10;                                                // 006261B6..006261BC ([esp+28] temp)
                for (;;) {
                    const I32 lines = vcall<I32>(rendered, 0x08);                              // 006261C6..006261D0
                    if (lines == gui_load_text_font_height_00414ee0(rendered, nullptr)) break;  // 006261D4..006261DF
                    w += 0x14; rect[2] = w;                                                    // 006261E8..006261ED
                    vcall<void, const void*>(label, 0x04, rect);                               // 006261E1..006261F1
                    if (w >= 0x1B8) break;                                                     // 006261F4..006261FA
                }
                width = w;                                                                     // 006261FC
            }
            if (flash != 0) kotor_fastcall_0062b0b0(in_game_gui(), nullptr, row);              // 00626200..0062621D ShowFlashingStatus
            y += 0x25;                                                                         // 00626222..0062622D
        } else {
            sw(entry, 0x344, lw(entry, 0x344) & ~2u);                                          // 00626235..0062624C
            sw(entry, 0xE84, lw(entry, 0xE84) & ~2u);                                          // 00626252
        }
    }
    const I32 panel_w = li(self, 0xE44) + width + 0x0A;                                        // 00626266..00626274
    const I32 button_y = y - 7;                                                                // 0062627B..0062627E ([esp+14])
    const I32 panel_h = button_y + 0x20;                                                       // 00626282..00626285
    void* manager = lp(self, 0x18);                                                            // 00626278
    rect[3] = panel_h; rect[2] = panel_w;                                                      // 00626285..00626289
    rect[1] = (static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6E))) - panel_h) / 2;  // 0062628D..00626298
    rect[0] = (static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6C))) - panel_w) / 2;  // 0062629C..006262A9
    vcall<void, const void*>(self, 0x04, rect);                                                // 006262AD..006262B4 SetExtent
    const I32 full_w = rect[2];                                                                // 006262B7 ([esp+20])
    std::memcpy(rect, at(self, 0x1984), 16);                                                   // 006262BB..006262D6
    rect[1] = button_y;                                                                        // 006262DA..006262E0
    rect[0] = (full_w - rect[2]) / 2;                                                          // 006262DE..006262F7
    vcall<void, const void*>(at(self, 0x1980), 0x04, rect);                                    // 006262ED..006262FE SetExtent (OK button)
    sw(self, 0x64, lw(self, 0x64) & 0xFFFFFE00u);                                              // 00626301
    sw(self, 0x6C, 0); sb(self, 0x68, 0); sb(self, 0x69, 0); sw(self, 0x70, 0); sw(self, 0x74, 0); // 0062630A..00626318
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 0062631B
    if (tutorial != 0)                                                                         // 00626320..00626324
        (void)show_tutorial_window_005edf40(client_app(), nullptr, 0x0E, 0x7F000000u, 0x7F000000u, 0); // 00626326..0062633B
    const U32 bx = lw(self, 0x1984), by = lw(self, 0x1988);                                    // 00626340..00626367
    const I32 bw = li(self, 0x198C), bh = li(self, 0x1990);
    const U32 px = lw(self, 0x04), py = lw(self, 0x08);                                        // 00626342..00626347
    const I32 mx = static_cast<I32>(static_cast<U32>(bw / 2) + px + bx);                       // 0062636E..00626384
    const I32 my = static_cast<I32>(static_cast<U32>(bh / 2) + by + py);                       // 00626373..00626391
    gui_m48_manager_store_mouse_0040a3d0(gui_manager(), nullptr);                              // 0062637A..00626393
    gui_m48_manager_move_mouse_0040c790(gui_manager(), nullptr, mx, my);                       // 00626398..006263A0
}                                                                                              // 006263A5..006263B4 ~text

// ======================= CSWGuiInGamePause =======================
// 00688DB0 CSWGuiMainInterface::SetupPauseGuiExtent(CSWGuiExtent*) (thiscall, RET 4).
void __fastcall kotor_fastcall_00688db0(void* self, void*, void* extent) {
    sw(extent, 0x04, 3);                                                                       // 00688DB5
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                            // 00688DBC..00688DC7 GetClientOptions
    if (options == nullptr || ((~(lw(options, 0x14) >> 9)) & 1u) != 0)                         // 00688DCC..00688DDD
        sw(extent, 0x04, lw(extent, 0x04) + (lw(self, 0xB01C) + lw(self, 0xB014) - 1u));       // 00688DDF..00688DEF
    sw(extent, 0x00, static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(lh(lp(self, 0x18), 0x6C))) - li(extent, 0x08) - 3)); // 00688DF2..00688E00
}
// 0062B100 CGuiInGame::SetupPauseGuiExtent(CSWGuiExtent*) (thiscall, RET 4): `jmp 00688DB0` on [this+90].
void __fastcall kotor_fastcall_0062b100(void* in_game, void*, void* extent) {
    kotor_fastcall_00688db0(lp(in_game, 0x90), nullptr, extent);                               // 0062B100..0062B106
}
// 006C02F0 CSWGuiInGamePause::OnPanelAdded() (thiscall, RET 0).
void __fastcall kotor_fastcall_006c02f0(void* self, void*) {
    if (kotor_m15e_client_get_mini_game_005edb00(client_app(), nullptr) != nullptr) {          // 006C02F0..006C0302
        void* manager = lp(self, 0x18);                                                        // 006C0304
        sw(self, 0x04, static_cast<U32>((static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6C))) - li(self, 0x0C)) / 2)); // 006C0307..006C0316
        sw(self, 0x08, static_cast<U32>((static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6E))) - li(self, 0x10)) / 2)); // 006C0319..006C0324
    } else {
        kotor_fastcall_0062b100(in_game_gui(), nullptr, at(self, 0x04));                       // 006C032F..006C0343
    }
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006C032A / 006C034B (jmp)
}

// ======================= CSWGuiInGameSoloModeQuery =======================
// 00565500 CSWPartyTable::SetSoloMode(int solo, int unstealth) (thiscall, RET 8).
void __fastcall kotor_fastcall_00565500(void* party, void*, U32 solo, U32 unstealth) {
    if (solo == 0 && unstealth != 0) recovered_unstealth_party_00563c60(party, nullptr, solo);  // 00565506..00565515
    sw(party, 0x190, solo);                                                                    // 0056551A
}
// 005F2A20 CClientExoAppInternal::TogglePartyFollow() (thiscall, RET 0).
void __fastcall kotor_fastcall_005f2a20(void*, void*) {
    Str script;                                                                                // [esp+00] (EH 0)
    frame_LogStringCtor_005e5a90(script.p(), nullptr, "k_sup_solo"); script.born();           // 005F2A38..005F2A41 (0074E930)
    const U32 solo = lw(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), 0x190); // 005F2A46..005F2A5B
    kotor_fastcall_00565500(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), nullptr, solo == 0 ? 1u : 0u, 1); // 005F2A61..005F2A7B
    frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, static_cast<unsigned*>(script.p()), 0x7F000000u, 0); // 005F2A80..005F2A92 RunScript
}                                                                                              // 005F2A97..005F2AA3 ~script
// 005EDE60 CClientExoApp::TogglePartyFollow() (thiscall, RET 0): `jmp 005F2A20` on [this+4].
void __fastcall kotor_fastcall_005ede60(void* client, void*) {
    kotor_fastcall_005f2a20(lp(client, 0x04), nullptr);                                        // 005EDE60..005EDE63
}
// 0062E6D0 CGuiInGame::HideSoloMode() (thiscall, RET 0).
void __fastcall kotor_fastcall_0062e6d0(void* in_game, void*) {
    if (frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, lp(in_game, 0x8C)) == 0) return; // 0062E6D3..0062E6E4
    if (lw(in_game, 0xB38) == 0)                                                               // 0062E6E6..0062E6EE
        (void)frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062E6F0..0062E700
    (void)kotor_fastcall_0040be00(lp(in_game, 0x38), nullptr);                                 // 0062E705..0062E708 PopModalPanel
    hide_flags(lp(in_game, 0x8C));                                                             // 0062E70D..0062E73B
    kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                                      // 0062E73E..0062E74A SetInputClass
}
// 006C2310 CSWGuiInGameSoloModeQuery::Draw(float) (thiscall, RET 4): hide while the query cannot apply.
void __fastcall kotor_fastcall_006c2310(void* self, void*, U32 delta_bits) {
    void* player = client_get_player_creature_005ed540(client_app(), nullptr);                // 006C2310..006C231D
    void* server_creature = nullptr;                                                           // edi
    if (player != nullptr) server_creature = kotor_m15a_creature_get_server_creature_0060fb20(player, nullptr); // 006C2324..006C232F
    void* area = nullptr;                                                                      // esi
    if (kotor_fastcall_005ed530(client_app(), nullptr) != nullptr                              // 006C2331..006C2343
        && lp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48) != nullptr)                // 006C2345..006C2358
        area = frame_GetServerArea_00605640(lp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48), nullptr); // 006C235A..006C235F
    bool hide = false;
    if (lw(in_game_gui(), 0xB4) != 0) hide = true;                                             // 006C2361..006C2376
    else if (area != nullptr && lw(area, 0x2C4) != 0) hide = true;                             // 006C2378..006C2384
    else if (kotor_m15_client_get_area_not_ready_005edad0(client_app(), nullptr) != 0) hide = true; // 006C2386..006C2396
    else if (server_creature != nullptr) {                                                     // 006C2398..006C239A
        if (vcall<U32>(server_creature, 0x94) != 0) hide = true;                               // 006C239C..006C23A8
        else if (recovered_creature_incapacitated_004ef890(server_creature, nullptr) != 0) hide = true; // 006C23AA..006C23B3
    }
    if (hide) hide_flags(self);                                                                // 006C23B5..006C23DD
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 006C23E0..006C23E7
}
// 006C2400 CSWGuiInGameSoloModeQuery::HandleInputEvent(int, int) (thiscall, RET 8).
void __fastcall kotor_fastcall_006c2400(void* self, void*, U32 id, U32 down) {
    bool toggle = false, close = false;
    if (down != 0) {                                                                           // 006C2400..006C240D
        const I32 key = static_cast<I32>(id);
        if (key > 0x2E) {                                                                      // 006C240F..006C2412 (jg)
            if (key == 0x1F6) toggle = true;                                                   // 006C2432..006C243A
            else if (key == 0x1F7) close = true;                                               // 006C243C..006C243D
        } else if (key == 0x2E || key == 0x28) close = true;                                   // 006C2414 / 006C241D..006C241E
        else if (key == 0x27 || key == 0x2D) toggle = true;                                    // 006C2416..006C241B / 006C2420..006C2423
    }
    if (!toggle && !close) {
        gui_m48_message_box_handle_input_006250f0(self, nullptr, id, down);                    // 006C2425..006C2429 / 006C243F..006C2443
        return;
    }
    if (toggle) {                                                                              // 006C244C
        void* player = client_get_player_creature_005ed540(client_app(), nullptr);            // 006C244C..006C2455
        kotor_fastcall_005ede60(client_app(), nullptr);                                        // 006C245A..006C2465 TogglePartyFollow
        if (player != nullptr && lw(self, 0xFC4) != 0)                                         // 006C246A..006C2476
            kotor_fastcall_0060ee00(player, nullptr, 2, 0, lw(player, 0x04));                  // 006C2478..006C2482 PerformSkill
    }
    kotor_fastcall_0062e6d0(in_game_gui(), nullptr);                                           // 006C2488..006C2497 HideSoloMode
}

// ======================= CSWGuiInGameAreaTransition =======================
// 0062EBE0 CGuiInGame::GetBarkBubbleVisible(int* bottom) (thiscall, RET 4) -> int.
U32 __fastcall kotor_fastcall_0062ebe0(void* in_game, void*, I32* bottom) {
    void* bubble = lp(in_game, 0x4C);                                                          // 0062EBE3
    if (bubble == nullptr) return 0;                                                           // 0062EBE6..0062EBE8
    if (frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, bubble) == 0) return 0;       // 0062EBEA..0062EBF5
    bubble = lp(in_game, 0x4C);                                                                // 0062EBF7
    *bottom = li(bubble, 0x10) + li(bubble, 0x08);                                             // 0062EBFA..0062EC09
    return 1;                                                                                  // 0062EC0B
}
// 006C8050 CSWGuiInGameAreaTransition::Draw(float) (thiscall, RET 4).
void __fastcall kotor_fastcall_006c8050(void* self, void*, U32 delta_bits) {
    I32 extent[4];                                                                             // [esp+08]
    std::memcpy(extent, at(self, 0x04), 16);                                                   // 006C8056..006C8079
    I32 bottom;                                                                                // [esp+04]
    if (kotor_fastcall_0062ebe0(in_game_gui(), nullptr, &bottom) != 0) extent[1] = bottom;     // 006C8075..006C8095
    else extent[1] = li(self, 0x434);                                                          // 006C809B..006C80A1
    vcall<void, const void*>(self, 0x04, extent);                                              // 006C80A5..006C80AE SetExtent
    if (kotor_fastcall_00635350(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr) != 0) { // 006C80B1..006C80C8 IsPartyTogether
        std::memcpy(at(self, 0xE4), kotor_m1_color_007a2378, 12);                              // 006C80CA..006C80E5
        std::memcpy(at(self, 0x224), kotor_m1_color_007a2378, 12);                             // 006C80E8..006C8104
        gui_load_text_params_set_color_00414e10(at(self, 0x3CC), nullptr, borrowed_exit_color_007a23b4); // 006C8107..006C8157
    } else {
        std::memcpy(at(self, 0xE4), kotor_m1_color_007a2384, 12);                              // 006C810E..006C812B
        std::memcpy(at(self, 0x224), kotor_m1_color_007a2384, 12);                             // 006C812E..006C8149
        gui_load_text_params_set_color_00414e10(at(self, 0x3CC), nullptr, kotor_m1_color_007a2408); // 006C814C..006C8157
    }
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 006C815C..006C8163
}
}
