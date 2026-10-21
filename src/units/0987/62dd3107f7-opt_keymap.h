#pragma once
// lane-m1-opt-gameplay (fork: key mappings): CSWGuiInGameOptKeyMappings (0xF34; operator new at 006E6443 in
// CSWGuiInGameGameplay::OnKeyMap 006E6420; ctor 006EDC90, table 00759358) and CSWGuiKeyMapButton (0x3B8; operator
// new at 006ED633 in CreateOption 006ED610; ctor 006EC630, table 007593C8), plus the CExoInput keyboard-capture
// leaves 005DF630 / 005E0230 that are not yet linked by M54.
// Original: swkotor.exe GOG K1, static disassembly only; every body cites its instruction addresses.
// Conventions: D1 / D21 / D24 / D47 / D30-FINAL / D2 as in opt_common.h.
#include "b8678b6f34-opt_common.h"

namespace kotor_m1_opt {
struct KeyMappings;

// CSWGuiKeyMapButton (0x3B8). Layout from the ctor 006EC630 and the field users cited.
struct KeyMapButton : Button {                 // +000 CSWGuiButton (0041C0F0 at 006EC64F), vptr 007593C8 (006EC662)
    Word desc_strref;                          // +1C4 "DescStrRef" (006EC859..70); copied to +24 / +1EC (006EC891..94)
    Button event_button;                       // +1C8 child button (0041C0F0 at 006EC668): shows the bound key
    KeyMappings* owner;                        // +38C ctor arg (006EC674)
    I32 id;                                    // +390 keymap.2da row / CExoInput event id (006ED683)
    I32 type;                                  // +394 0 normal, 1 "B", 2 "A" (006ED689; 006EC1CB..E9)
    Word changed;                              // +398 (006EC695 = 0; 006EDC3B; 006ECD84)
    Word new_key;                              // +39C (006EC6A1 = 0; 006EDC35; 006ECD76)
    Word orig_key;                             // +3A0 = -1 (006EC67A); written from the event at 006ECBFD / 006ECC44
    Word unchangeable;                         // +3A4 (006EC6A7 = 0; SetUnchangable 006EC773)
    Word class_flags;                          // +3A8 IC* input-class bits (006EC6B3 = 0; 006EC8EA..006ECB50)
    I32 page;                                  // +3AC "Page" filter (006EC6AD = 0; 006EC8AE..C5; FillListBox 006ED02A)
    Word u3b0;                                 // +3B0 not written or read by the cited functions
    I32 sort_pos;                              // +3B4 = -1 (006EC680); "SortPos" (006ED81C)
};
static_assert(offsetof(KeyMapButton, desc_strref) == 0x1C4 && offsetof(KeyMapButton, event_button) == 0x1C8 &&
              offsetof(KeyMapButton, owner) == 0x38C && offsetof(KeyMapButton, unchangeable) == 0x3A4 &&
              offsetof(KeyMapButton, page) == 0x3AC && offsetof(KeyMapButton, sort_pos) == 0x3B4 &&
              sizeof(KeyMapButton) == 0x3B8, "CSWGuiKeyMapButton 0x3B8");

// CSWGuiInGameOptKeyMappings (0xF34). Layout from the ctor 006EDC90 / dtor 006EC3D0.
struct KeyMappings : Panel {                   // +000 CSWGuiPanel (0040B570 at 006EDCBB), vptr 00759358 (006EDCC2)
    gui_load::Array buttons;                   // +064 CExoArrayList<CSWGuiKeyMapButton*> (006EDCCC..D2 zero)
    Label lbl_title;                           // +070 "LBL_Title" (006EDCDD)
    Button btn_default;                        // +1B0 "BTN_Default"
    Button btn_accept;                         // +374 "BTN_Accept"
    Button btn_cancel;                         // +538 "BTN_Cancel"
    Button btn_filter_move;                    // +6FC "BTN_Filter_Move" (text params +868)
    Button btn_filter_game;                    // +8C0 "BTN_Filter_Game" (text params +A2C)
    Button btn_filter_mini;                    // +A84 "BTN_Filter_Mini" (text params +BF0)
    ListBox list;                              // +C48 "LST_EventList" (proto at +F14)
    I32 filter;                                // +F28 (006EDD7D = 0; filters 006ED39E / 006ED3EE / 006ED43E)
    Word capturing;                            // +F2C (006EDD56 = 0; SetCaptureEvent 006ED529)
    KeyMapButton* capture_button;              // +F30 (006EDD83 = 0; SetCaptureEvent 006ED4DE)
};
static_assert(offsetof(KeyMappings, buttons) == 0x64 && offsetof(KeyMappings, lbl_title) == 0x70 &&
              offsetof(KeyMappings, btn_default) == 0x1B0 && offsetof(KeyMappings, btn_filter_mini) == 0xA84 &&
              offsetof(KeyMappings, list) == 0xC48 && offsetof(KeyMappings, filter) == 0xF28 &&
              offsetof(KeyMappings, capture_button) == 0xF30 && sizeof(KeyMappings) == 0xF34,
              "CSWGuiInGameOptKeyMappings 0xF34");

// Event callbacks registered by the panel ctor (thiscall, RET 4; D47 single-inheritance view).
struct KeyMappingsHandlers : kih::GuiObject {
    void __thiscall FixFilterColors_006ec580(kih::Control* control);   // event 1 on the three filter buttons
    void __thiscall OnDefaultClick_006ed2d0(kih::Control* control);    // 27h BTN_Default
    void __thiscall OnAcceptClick_006ed310(kih::Control* control);     // 27h BTN_Accept
    void __thiscall OnFilterMove_006ed390(kih::Control* control);      // 27h BTN_Filter_Move
    void __thiscall OnFilterGame_006ed3e0(kih::Control* control);      // 27h BTN_Filter_Game
    void __thiscall OnFilterMini_006ed430(kih::Control* control);      // 27h BTN_Filter_Mini
};
// Event callbacks registered on each CSWGuiKeyMapButton by CreateOption (target = the button itself).
struct KeyMapButtonHandlers : kih::GuiObject {
    void __thiscall onControlEntered_006ed5c0(kih::Control* control);  // event 0 (006ED8CF..D9)
    void __thiscall onEventClicked_006ed5e0(kih::Control* control);    // event 27h (006ED8C0..CA)
    void __thiscall VoidReturn_0060e760(kih::Control* control);        // event 1: 0060E760 (RET 4) typed body (D24)
};
// CSWGuiKeyMapButton table slot +44 (PMF slot type of the control family, D21).
struct KeyMapButtonHit : kih::Control {
    kih::Control* __thiscall HitCheckMouse_006ec380(int px, int py);
};
}

extern "C" {
// ---- CSWGuiInGameOptKeyMappings
gui_load::Panel* __fastcall kotor_m1_opt_keymap_construct_006edc90(void* self, void*, gui_load::Object* manager) noexcept(false);
void __fastcall kotor_m1_opt_keymap_destruct_006ec3d0(kotor_m1_opt::KeyMappings*, void*);
void* __fastcall kotor_m1_opt_keymap_deleting_destruct_006ecfb0(kotor_m1_opt::KeyMappings*, void*, gui_load::Word);
void __fastcall kotor_m1_opt_keymap_handle_input_006ec510(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
void __fastcall kotor_m1_opt_keymap_update_006ee040(gui_load::Object*, void*, gui_load::Word);
void __fastcall kotor_m1_opt_keymap_fill_list_box_006ecfd0(kotor_m1_opt::KeyMappings*, void*, gui_load::I32 filter) noexcept(false);
void __fastcall kotor_m1_opt_keymap_fill_list_with_events_006ed930(kotor_m1_opt::KeyMappings*, void*) noexcept(false);
void __fastcall kotor_m1_opt_keymap_set_capture_event_006ed480(kotor_m1_opt::KeyMappings*, void*, kotor_m1_opt::KeyMapButton*);
void __fastcall kotor_m1_opt_keymap_set_capturing_006ec3b0(kotor_m1_opt::KeyMappings*, void*, gui_load::Word);
kotor_m1_opt::KeyMapButton* __fastcall kotor_m1_opt_keymap_create_option_006ed610(kotor_m1_opt::KeyMappings*, void*,
    gui_load::I32 id, gui_load::Control* proto, gui_load::I32 type) noexcept(false);
gui_load::I32 __fastcall kotor_m1_opt_keymap_update_event_006eda80(kotor_m1_opt::KeyMappings*, void*, gui_load::I32 id,
    gui_load::I32 type, gui_load::Word key, gui_load::Word class_flags) noexcept(false);
gui_load::I32 __fastcall kotor_m1_opt_keymap_is_key_used_006ed1e0(kotor_m1_opt::KeyMappings*, void*, gui_load::Word key,
    gui_load::I32 id, gui_load::I32 type, gui_load::Word class_flags) noexcept(false);
extern const struct GuiM1OptKeyMappingsMethods gui_m1_opt_keymap_methods_00759358;
// ---- CSWGuiKeyMapButton
kotor_m1_opt::KeyMapButton* __fastcall kotor_m1_opt_keymap_button_construct_006ec630(void* self, void*,
    kotor_m1_opt::KeyMappings* owner) noexcept(false);
void __fastcall kotor_m1_opt_keymap_button_destruct_006ed570(kotor_m1_opt::KeyMapButton*, void*);
void* __fastcall kotor_m1_opt_keymap_button_deleting_destruct_006ed550(kotor_m1_opt::KeyMapButton*, void*, gui_load::Word);
void __fastcall kotor_m1_opt_keymap_button_set_extent_006ec250(kotor_m1_opt::KeyMapButton*, void*, const gui_load::Extent*);
void __fastcall kotor_m1_opt_keymap_button_draw_006ec350(kotor_m1_opt::KeyMapButton*, void*, gui_load::Word);
gui_load::String* __fastcall kotor_m1_opt_keymap_button_get_action_id_006ec120(kotor_m1_opt::KeyMapButton*, void*,
    gui_load::String* out, gui_load::I32 id, gui_load::I32 type) noexcept(false);
void __fastcall kotor_m1_opt_keymap_button_update_color_006ec6e0(kotor_m1_opt::KeyMapButton*, void*);
void __fastcall kotor_m1_opt_keymap_button_set_unchangable_006ec770(kotor_m1_opt::KeyMapButton*, void*);
void __fastcall kotor_m1_opt_keymap_button_set_description_006ec7d0(kotor_m1_opt::KeyMapButton*, void*, gui_load::String* row) noexcept(false);
void __fastcall kotor_m1_opt_keymap_button_reset_to_default_006eccc0(kotor_m1_opt::KeyMapButton*, void*) noexcept(false);
void __fastcall kotor_m1_opt_keymap_button_post_change_006ece60(kotor_m1_opt::KeyMapButton*, void*) noexcept(false);
gui_load::I32 __fastcall kotor_m1_opt_keymap_button_update_event_006edbd0(kotor_m1_opt::KeyMapButton*, void*, gui_load::Word key) noexcept(false);
extern const struct GuiM1OptKeyMapButtonMethods gui_m1_opt_keymap_button_methods_007593c8;
// ---- CExoInput keyboard capture leaves (opt_input_leaves.cpp; not linked by M54 under any name)
gui_load::Word __fastcall kotor_m1_opt_input_last_captured_key_005df630(void* input, void*) noexcept(false);
gui_load::Word __fastcall kotor_m1_opt_input_internal_last_captured_key_005e0230(void* internal, void*) noexcept(false);
// .data colour cells first owned here (no M54 owner): 007A2390, 007A23FC.
extern const MenuColor kotor_m1_color_007a2390;
extern const MenuColor kotor_m1_color_007a23fc;
}
