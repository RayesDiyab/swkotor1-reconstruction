#pragma once
// lane-m1-opt-gameplay: CSWGuiInGameGameplay (0x22A0; operator new at 006DE263, ctor 006E69A0, table 00758E00)
// and the declarations shared with CSWGuiOptionsMouse (opt_mouse.h): the common HandleInputEvent 006E6180 and
// OnPanelAdded 006E6170 slots and the 27-slot panel method-table type.
// Original: swkotor.exe GOG K1, static disassembly only. Conventions D1/D21/D24/D47 (see opt_common.h).
#include "b8678b6f34-opt_common.h"

namespace kotor_m1_opt {
// Layout established by the ctor 006E69A0 (member ctor calls, InitControl tags) and dtor 006E7130 (reverse order).
struct Gameplay : Panel {                       // +0000 CSWGuiPanel (0040B570 at 006E69CB)
    Label desc_item;                            // +0064 (006E69E1) description item; Initialize98 from LB_DESC proto (006E6E9E)
    Label lbl_title;                            // +01A4 (006E69F1) "LBL_TITLE" (006E6D3C)
    ListBox lb_desc;                            // +02E4 (006E6A01) "LB_DESC" (006E6D71); +5A5 = lb+2C1
    Button btn_back;                            // +05C4 (006E6A11) "BTN_BACK" (006E6DA6)
    Button btn_default;                         // +0788 (006E6A21) "BTN_DEFAULT" (006E6DDB)
    Button btn_keymap;                          // +094C (006E6A31) "BTN_KEYMAP" (006E6E10)
    Button btn_mouse;                           // +0B10 (006E6A41) "BTN_MOUSE" (006E6E45)
    Button btn_difficulty;                      // +0CD4 (006E6A53) "BTN_DIFFICULTY" (006E6B30); text params +E40
    Button btn_diffright;                       // +0E98 (006E6A63) "BTN_DIFFRIGHT" (006E6B5F)
    Button btn_diffleft;                        // +105C (006E6A73) "BTN_DIFFLEFT" (006E6B94)
    Checkbox cb_levelup;                        // +1220 (006E6A85) "CB_LEVELUP" (006E6BFE)
    Checkbox cb_invertcam;                      // +14D4 (006E6A9D) "CB_INVERTCAM" (006E6BC9)
    Checkbox cb_autosave;                       // +1788 (006E6AB5) "CB_AUTOSAVE" (006E6C33)
    Checkbox cb_reverse;                        // +1A3C (006E6ACD) "CB_REVERSE" (006E6C68)
    Checkbox cb_disablemove;                    // +1CF0 (006E6AE5) "CB_DISABLEMOVE" (006E6C9D)
    Slider sli_mousesen;                        // +1FA4 (006E6AFB) "SLI_MOUSESEN" (006E6CD2)
    Label lbl_mousesen;                         // +215C (006E6B0B) "LBL_MOUSESEN" (006E6D07)
    I32 difficulty;                             // +229C (SetUpOptions 006E678B..8F from options+4 byte)
};
static_assert(offsetof(Gameplay, lb_desc) == 0x2E4 && offsetof(Gameplay, btn_difficulty) == 0xCD4 &&
              offsetof(Gameplay, btn_diffleft) == 0x105C && offsetof(Gameplay, cb_levelup) == 0x1220 &&
              offsetof(Gameplay, cb_disablemove) == 0x1CF0 && offsetof(Gameplay, sli_mousesen) == 0x1FA4 &&
              offsetof(Gameplay, lbl_mousesen) == 0x215C && offsetof(Gameplay, difficulty) == 0x229C &&
              sizeof(Gameplay) == 0x22A0, "CSWGuiInGameGameplay 0x22A0");

// CSWGuiControlEvent callbacks registered by 006E69A0 (thiscall members, RET 4; D47).
struct GameplayHandlers : kih::GuiObject {
    void __thiscall OnAutoLevelUp_006e62a0(kih::Control* control);
    void __thiscall OnAutoSave_006e62e0(kih::Control* control);
    void __thiscall OnInvertCam_006e6330(kih::Control* control);
    void __thiscall OnReverse_006e6380(kih::Control* control);
    void __thiscall OnCombatMovement_006e63d0(kih::Control* control);
    void __thiscall OnKeyMap_006e6420(kih::Control* control);
    void __thiscall OnMouse_006e64a0(kih::Control* control);
    void __thiscall OnEnterDiff_006e6710(kih::Control* control);
    void __thiscall OnExitDiff_006e6750(kih::Control* control);
    void __thiscall OnEnterSlot_006e68a0(kih::Control* control);
    void __thiscall OnDefault_006e68b0(kih::Control* control);
    void __thiscall OnDifficultyLeft_006e68e0(kih::Control* control);
    void __thiscall OnDifficultyRight_006e6930(kih::Control* control);
};

// 27-slot CSWGuiPanel-family method table shape (tables 00758E00 / 007585F8; read from the original .rdata).
struct GuiM1OptGameplayMethods {
    void* (__fastcall* s00)(Object*, void*, Word);                            // +00 scalar deleting dtor
    void (__fastcall* s04)(Object*, void*, const Extent*);                    // +04 0040A5A0
    void (__fastcall* s08)(Object*, void*, Object*, Word);                    // +08 0040A630
    void (__fastcall* s0c)(Object*, void*, Word);                             // +0C 0060E760
    Word (__fastcall* s10)(Object*, void*, Word, Word);                       // +10 00409EB0
    void (__fastcall* s14)(Object*, void*);                                   // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                   // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                   // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                   // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                   // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                // +28 00641DB0
    Word (__fastcall* s2c)(Object*, void*);                                   // +2C 0063E7F0
    Word (__fastcall* s30)(Object*, void*);                                   // +30 0063E7F0
    void (__fastcall* s34)(Panel*, void*, Word);                              // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(Object*, void*, Word);                             // +38 0060E760
    void (__fastcall* s3c)(Object*, void*, I32, I32);                         // +3C 006E6180 HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630
    void (__fastcall* s48)(Panel*, void*);                                    // +48 006E6170 (JMP 0040B870)
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0
};
static_assert(sizeof(GuiM1OptGameplayMethods) == 0x6C, "27 slots");
}

extern "C" {
void __fastcall kotor_m1_opt_gameplay_destruct_006e7130(kotor_m1_opt::Gameplay* self, void*);
void* __fastcall kotor_m1_opt_gameplay_deleting_destruct_006e7110(gui_load::Object* self, void*, gui_load::Word flags);
void __fastcall kotor_m1_opt_gameplay_handle_input_006e6180(gui_load::Object* self, void*, gui_load::I32 event, gui_load::I32 pressed);
void __fastcall kotor_m1_opt_gameplay_on_panel_added_006e6170(gui_load::Panel* self, void*);
void __fastcall kotor_m1_opt_gameplay_set_up_options_006e6770(kotor_m1_opt::Gameplay* self, void*);
void __fastcall kotor_m1_opt_gameplay_set_difficulty_text_006e6520(kotor_m1_opt::Gameplay* self, void*) noexcept(false);
void __fastcall kotor_m1_opt_gameplay_set_description_006e6620(kotor_m1_opt::Gameplay* self, void*, gui_load::Word strref) noexcept(false);
// CClientOptions (ECX = options object returned by 005ED700)
void __fastcall kotor_m1_opt_client_options_set_auto_level_up_npcs_0061b740(void* options, void*, gui_load::Word value);
void __fastcall kotor_m1_opt_client_options_set_default_gameplay_0061d4e0(void* options, void*);
extern const kotor_m1_opt::GuiM1OptGameplayMethods gui_m1_opt_gameplay_methods_00758e00;
}
