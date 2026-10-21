#pragma once
// lane-m15-c (L15-C): layouts, method-table type, event-callback (PMF) views and internal declarations of the
// char-gen flow panels: CSWGuiMainCharGen, CSWGuiQuickOrCustomPanel, CSWGuiQuickPanel, CSWGuiCustomPanel,
// CSWGuiPortraitCharGen, CSWGuiNameChargen. Original swkotor.exe GOG K1 (image base 0x400000), static disassembly.
// Every named offset cites the ctor instruction establishing it; unknown bytes stay opaque.
// Conventions (D1/D21/D24/D47/D107): see m15c_chargen.h; callbacks are __thiscall members of single-inheritance views
// of kotor_input_hover::GuiObject (one-word PMFs) converted with static_cast to gui_load::EventFunc.
#include "8c61ee1430fb8439_gui_objects.h"
#include "c9d0373605f94236_mainmenu.h"
#include "a200cba35d5f8b9c_m15c_chargen.h"
#include "868937c3784da962_m15c_editbox.h"
#include "1026d9ab1ae1c129_m15c_namegen.h"
#include <cstddef>
#include <cstdint>

namespace kotor_m15c {
namespace kih = kotor_input_hover;
using gui_load::Word;
using gui_load::I32;
using gui_load::Panel;
using gui_load::Label;
using gui_load::Button;
using gui_load::Object;
using Array = kotor_reconstruction::ArrayMutations;

// CSWGuiLabelHilight (0x1B4; ctor 0041B5C0, dtor 0041B610): label + hilight border at +140.
struct LabelHilight : Label { gui_load::Border hilight; };
static_assert(sizeof(LabelHilight) == 0x1B4, "CSWGuiLabelHilight 0x1B4");
// CSWGui3DSceneView (0x90; CSWGuiControl 0041AA80 + table 00752E30 + CSWGuiScene 004174B0 at +5C).
struct SceneView : gui_load::Control { gui_load::GuiScene scene; };
static_assert(sizeof(SceneView) == 0x90 && offsetof(SceneView, scene) == 0x5C, "CSWGui3DSceneView 0x90");

struct QuickOrCustom; struct Quick; struct Custom;

// ---------------------------------------------------------------- CSWGuiMainCharGen (0x22DC; operator new 006DBA14)
struct MainCharGen : Panel {
    void* creature;                 // +0064 ctor arg2 (006EB673)
    void* class_selection;          // +0068 ctor arg3 (006EB654)
    QuickOrCustom* quick_or_custom; // +006C (006EBC8F)
    Quick* quick;                   // +0070 (006EBCC6)
    Custom* custom;                 // +0074 (006EBD04)
    Label labels[27];               // +0078..+2238 (27 label ctors 006EB463..603; InitControl 006EB6B2..BC49)
    SceneView model;                // +2238 "MODEL_LBL" (006EB619..62C)
    Word visible_panel;             // +22C8 1 QuickOrCustom, 2 Quick, 3 Custom, 4 none (006EBD15; 006EA760)
    unsigned char class_name[8];    // +22CC CExoString (006EB63C; DetermineClass 006EA940)
    unsigned char name[8];          // +22D4 CExoString (006EB64C; SetNameString 006EA9B8)
};
static_assert(sizeof(MainCharGen) == 0x22DC && offsetof(MainCharGen, labels) == 0x78 && offsetof(MainCharGen, model) == 0x2238 &&
              offsetof(MainCharGen, visible_panel) == 0x22C8 && offsetof(MainCharGen, name) == 0x22D4, "CSWGuiMainCharGen");
// labels[] index by offset (+78 + 140h*k)
enum MainLabel {
    kMainTitle = 0, kName, kClass, kStr, kDex, kCon, kInt, kWis, kCha,                // +078..+A78
    kStrAb, kDexAb, kConAb, kIntAb, kWisAb, kChaAb,                                    // +BB8..+11F8
    kVit, kDef, kNewWill, kNewRefl, kNewFort, kPortrait, kBevelL, kBevelM, kBevelR,    // +1338..+1D38
    kFortitude, kReflex, kWill                                                         // +1E78..+20F8
};

// ---------------------------------------------------------------- CSWGuiQuickOrCustomPanel (0xD98; new 006EBC62)
struct QuickOrCustom : Panel {
    void* creature;                 // +064 ctor arg2 (006F0AAD)
    MainCharGen* parent;            // +068 (006EBC9B)
    Button quick;                   // +06C "QUICK_CHAR_BTN"
    Button custom;                  // +230 "CUST_CHAR_BTN"
    Label desc_item;                // +3F4 (Initialize from LB_DESC proto, 006F0C4B..63)
    Label decoration;               // +534 "LBL_DECORATION"
    Label ranalog;                  // +674 "LBL_RANALOG" (flags bit1 = scroll hint, 006F0989..9F)
    Label rbg;                      // +7B4 "LBL_RBG"
    gui_lb::ListBox lb_desc;        // +8F4 "LB_DESC" (+BB5 = lb+2C1 = 1 at 006F0B6E)
    Button back;                    // +BD4 "BTN_BACK"
};
static_assert(sizeof(QuickOrCustom) == 0xD98 && offsetof(QuickOrCustom, desc_item) == 0x3F4 &&
              offsetof(QuickOrCustom, lb_desc) == 0x8F4 && offsetof(QuickOrCustom, back) == 0xBD4, "CSWGuiQuickOrCustomPanel");

// ---------------------------------------------------------------- CSWGuiQuickPanel (0x1360; new 006EBC92)
struct Quick : Panel {
    void* creature;                 // +0064 (006F047D)
    MainCharGen* parent;            // +0068 (006EBCD2)
    Label decoration;               // +006C "LBL_DECORATION"
    LabelHilight steps[3];          // +01AC "LBL_%d" (eh vector ctor 006F03F5)
    Label nums[3];                  // +06C8 "LBL_NUM%d"
    Button buttons[3];              // +0A88 "BTN_STEPNAME%d": portrait, name, play
    Button back;                    // +0FD4 "BTN_BACK"
    Button cancel;                  // +1198 "BTN_CANCEL"
    I32 state;                      // +135C (SetState 006EFC21)
};
static_assert(sizeof(Quick) == 0x1360 && offsetof(Quick, steps) == 0x1AC && offsetof(Quick, nums) == 0x6C8 &&
              offsetof(Quick, buttons) == 0xA88 && offsetof(Quick, back) == 0xFD4 && offsetof(Quick, state) == 0x135C,
              "CSWGuiQuickPanel");

// ---------------------------------------------------------------- CSWGuiCustomPanel (0x2188; new 006EBCC9)
struct Custom : Panel {
    void* creature;                 // +0064 (006EF830)
    MainCharGen* parent;            // +0068 (SetParentPanel 006EEF64)
    LabelHilight steps[6];          // +006C "LBL_%d"
    Label nums[6];                  // +0AA4 "LBL_NUM%d"
    Button buttons[6];              // +1224 "BTN_STEPNAME%d": portrait, abilities, skills, feats, name, play
    Label bg;                       // +1CBC "LBL_BG"
    Button back;                    // +1DFC "BTN_BACK"
    Button cancel;                  // +1FC0 "BTN_CANCEL"
    I32 state;                      // +2184 (SetState 006EEFE1)
};
static_assert(sizeof(Custom) == 0x2188 && offsetof(Custom, nums) == 0xAA4 && offsetof(Custom, buttons) == 0x1224 &&
              offsetof(Custom, bg) == 0x1CBC && offsetof(Custom, back) == 0x1DFC && offsetof(Custom, state) == 0x2184,
              "CSWGuiCustomPanel");

// ---------------------------------------------------------------- CSWGuiPortraitCharGen (0x1240; new 006EF156)
struct PortraitAnim { Object* control; float time; };   // +1224 element (8 bytes; Draw 006F8C90..CFE)
struct Portrait : Panel {
    void* creature;                 // +0064 (006F9558)
    Panel* parent;                  // +0068 (SetParentPanel 006F8A08)
    Label title;                    // +006C "MAIN_TITLE_LBL"
    Label subtitle;                 // +01AC "SUB_TITLE_LBL"
    Label portrait;                 // +02EC "LBL_PORTRAIT" (border params +35C)
    Label bevels[5];                // +042C "LBL_BEVEL_L/M/R/T/B"
    SceneView head;                 // +0A6C "LBL_HEAD" (scene +AC8; aur scene +ADC; camera +AE0)
    Button accept;                  // +0AFC "BTN_ACCEPT"
    Button back;                    // +0CC0 "BTN_BACK"
    Button right;                   // +0E84 "BTN_ARRR"
    Button left;                    // +1048 "BTN_ARRL"
    Array rows;                     // +120C CExoArrayList<int> portraits.2da rows (006F951F..2B)
    Array appearances;              // +1218 CExoArrayList<int> appearance numbers (006F9531..3D)
    Array anims;                    // +1224 CExoArrayList<PortraitAnim> (006F9543..4F)
    float idle;                     // +1230 idle-animation timer (006F89F5, 006F9A0E)
    I32 current;                    // +1234 index into rows (006F9846)
    I32 original;                   // +1238 portrait id at open (006F9840)
    unsigned char parent_kind;      // +123C 1 Quick, 2 Custom (SetParentPanel 006F8A0B)
    unsigned char pad[3];
};
static_assert(sizeof(Portrait) == 0x1240 && offsetof(Portrait, head) == 0xA6C && offsetof(Portrait, accept) == 0xAFC &&
              offsetof(Portrait, rows) == 0x120C && offsetof(Portrait, idle) == 0x1230 && offsetof(Portrait, parent_kind) == 0x123C,
              "CSWGuiPortraitCharGen");

// ---------------------------------------------------------------- CSWGuiNameChargen (0x9C4; new 006EF26F / 006EFDAF)
struct NameChargen : Panel {
    void* creature;                 // +064 (006F9F2B)
    Panel* parent;                  // +068 (SetParentPanel 006F9B88)
    Button end;                     // +06C "END_BTN"
    kotor_m15c_editbox::Editbox edit;   // +230 "NAME_BOX_EDIT" (edit string +388, max length +380)
    Label title;                    // +390 "MAIN_TITLE_LBL"
    Label subtitle;                 // +4D0 "SUB_TITLE_LBL"
    Button back;                    // +610 "BTN_BACK"
    Button random;                  // +7D4 "BTN_RANDOM"
    unsigned char parent_kind;      // +998 1 Quick, 2 Custom (006F9B8B)
    unsigned char pad[3];
    Word flags;                     // +99C bit0 = name generated (006F9CA6; ctor &= ~1 at 006FA0FC)
    kotor_m15c_namegen::NameGen namegen;   // +9A0 (006F9F15)
};
static_assert(sizeof(NameChargen) == 0x9C4 && offsetof(NameChargen, edit) == 0x230 && offsetof(NameChargen, back) == 0x610 &&
              offsetof(NameChargen, parent_kind) == 0x998 && offsetof(NameChargen, namegen) == 0x9A0, "CSWGuiNameChargen");

// ---------------------------------------------------------------- event callbacks (thiscall, RET 4)
struct QuickOrCustomHandlers : kih::GuiObject {
    void __thiscall OnSelectQuick_006f0800(kih::Control* control);
    void __thiscall OnSelectCustom_006f0830(kih::Control* control);
    void __thiscall OnEnterQuick_006f09d0(kih::Control* control);
    void __thiscall OnEnterCustom_006f09e0(kih::Control* control);
};
struct QuickHandlers : kih::GuiObject {
    void __thiscall OnSelectPlay_006efd60(kih::Control* control);
    void __thiscall OnSelectName_006efd80(kih::Control* control);
    void __thiscall OnSelectPortrait_006efe10(kih::Control* control);
    void __thiscall ConfirmCancel_006f0220(kih::Control* control);
    void __thiscall OnCancelPressed_006f0310(kih::Control* control);
};
struct CustomHandlers : kih::GuiObject {
    void __thiscall OnSelectPortrait_006ef120(kih::Control* control);
    void __thiscall OnSelectPlay_006ef220(kih::Control* control);
    void __thiscall OnSelectName_006ef240(kih::Control* control);
    void __thiscall OnSelectFeats_006ef2d0(kih::Control* control);
    void __thiscall OnSelectSkills_006ef360(kih::Control* control);
    void __thiscall OnSelectAbilities_006ef3f0(kih::Control* control);
    void __thiscall ConfirmCancel_006ef5b0(kih::Control* control);
    void __thiscall OnCancelPressed_006ef6a0(kih::Control* control);
};
struct NameHandlers : kih::GuiObject {
    void __thiscall HandleDoneButton_006f9cd0(kih::Control* control);
};
static_assert(sizeof(void (__thiscall QuickHandlers::*)(kih::Control*)) == 4 &&
              sizeof(void (__thiscall NameHandlers::*)(kih::Control*)) == 4, "single-inheritance PMFs (D107)");
template <class H> inline gui_load::EventFunc handler(void (__thiscall H::*f)(kih::Control*)) {
    return static_cast<gui_load::EventFunc>(f);
}
}

extern "C" {
// ---------------------------------------------------------------- panel method table type (27 slots, like 0073E010)
struct KotorM15cPanelMethods {
    void* (__fastcall* s00)(gui_load::Panel*, void*, gui_load::Word);                        // +00 deleting dtor
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);               // +04 0040A5A0
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);     // +08 0040A630
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);                        // +0C 0060E760
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);   // +10 00409EB0
    void (__fastcall* s14_24[5])(gui_load::Object*, void*);                                  // +14..+24 004015A0
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);                           // +28 00641DB0
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);                              // +2C 0063E7F0
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);                              // +30 0063E7F0
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);                         // +34 Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);                        // +38 0060E760
    void (__fastcall* s3c)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);          // +3C HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);                           // +44 0040B630
    void (__fastcall* s48)(gui_load::Panel*, void*);                                         // +48 OnPanelAdded
    void (__fastcall* s4c)(gui_load::Panel*, void*);                                         // +4C 0040C170
    void (__fastcall* s50_60[5])(gui_load::Object*, void*, gui_load::Word);                  // +50..+60 0040B640..80
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);         // +64 0040A900
    void (__fastcall* s68)(gui_load::Panel*, void*);                                         // +68 0040A9C0
};
static_assert(sizeof(KotorM15cPanelMethods) == 0x6C, "27-slot panel table");
extern const KotorM15cPanelMethods kotor_m15c_main_chargen_methods_007592a8;
extern const KotorM15cPanelMethods kotor_m15c_quick_or_custom_methods_00759710;
extern const KotorM15cPanelMethods kotor_m15c_quick_methods_00759668;
extern const KotorM15cPanelMethods kotor_m15c_custom_methods_007595e0;
extern const KotorM15cPanelMethods kotor_m15c_portrait_methods_00759ea8;
extern const KotorM15cPanelMethods kotor_m15c_name_methods_00759f38;

// ---------------------------------------------------------------- CSWGuiMainCharGen (m15c_maincg.cpp)
void __fastcall kotor_m15c_main_chargen_set_visible_panel_006ea760(kotor_m15c::MainCharGen* self, void*, gui_load::Word panel) noexcept(false);
void __fastcall kotor_m15c_main_chargen_on_panel_added_006ea810(gui_load::Panel* self, void*) noexcept(false);
void __fastcall kotor_m15c_main_chargen_remove_sub_panels_006ea830(kotor_m15c::MainCharGen* self, void*) noexcept(false);
void __fastcall kotor_m15c_main_chargen_determine_class_006ea910(kotor_m15c::MainCharGen* self, void*) noexcept(false);
void __fastcall kotor_m15c_main_chargen_set_name_string_006ea9b0(kotor_m15c::MainCharGen* self, void*, gui_load::String* name) noexcept(false);
void __fastcall kotor_m15c_main_chargen_portrait_changed_006ea9f0(kotor_m15c::MainCharGen* self, void*) noexcept(false);
void __fastcall kotor_m15c_main_chargen_abilities_changed_006eac30(kotor_m15c::MainCharGen* self, void*) noexcept(false);
void __fastcall kotor_m15c_main_chargen_clear_abilities_006eb010(kotor_m15c::MainCharGen* self, void*) noexcept(false);
void __fastcall kotor_m15c_main_chargen_draw_006eb340(gui_load::Panel* self, void*, gui_load::Word delta_bits) noexcept(false);
void* __fastcall kotor_m15c_main_chargen_deleting_destruct_006ebee0(gui_load::Panel* self, void*, gui_load::Word flags);
void __fastcall kotor_m15c_main_chargen_destruct_006ebf00(kotor_m15c::MainCharGen* self, void*);
void __fastcall kotor_m15c_main_chargen_name_changed_006eef70(kotor_m15c::MainCharGen* self, void*) noexcept(false);

// ---------------------------------------------------------------- CSWGuiQuickOrCustomPanel / Quick / Custom (m15c_flow.cpp)
kotor_m15c::QuickOrCustom* __fastcall kotor_m15c_quick_or_custom_construct_006f09f0(kotor_m15c::QuickOrCustom* self, void*,
    gui_load::Object* manager, void* creature) noexcept(false);                                                   // RET 8
void __fastcall kotor_m15c_quick_or_custom_handle_cancel_006f07d0(kotor_m15c::QuickOrCustom* self, void*) noexcept(false);
void __fastcall kotor_m15c_quick_or_custom_set_description_006f0860(kotor_m15c::QuickOrCustom* self, void*, gui_load::Word strref) noexcept(false);
void* __fastcall kotor_m15c_quick_or_custom_deleting_destruct_006f0d30(gui_load::Panel* self, void*, gui_load::Word flags);
void __fastcall kotor_m15c_quick_or_custom_destruct_006f0d50(kotor_m15c::QuickOrCustom* self, void*);
void __fastcall kotor_m15c_quick_or_custom_handle_input_006f0e10(gui_load::Object* self, void*, gui_load::I32 event, gui_load::I32 pressed) noexcept(false);

kotor_m15c::Quick* __fastcall kotor_m15c_quick_construct_006f0390(kotor_m15c::Quick* self, void*, gui_load::Object* manager,
    void* creature) noexcept(false);                                                                               // RET 8
void __fastcall kotor_m15c_quick_set_state_006efc10(kotor_m15c::Quick* self, void*, gui_load::I32 state) noexcept(false);
void __fastcall kotor_m15c_quick_cancelled_by_child_006eff10(kotor_m15c::Quick* self, void*) noexcept(false);
void __fastcall kotor_m15c_quick_portrait_creation_completed_006eff60(kotor_m15c::Quick* self, void*) noexcept(false);
void __fastcall kotor_m15c_quick_name_creation_completed_006eff80(kotor_m15c::Quick* self, void*) noexcept(false);
gui_load::String* __fastcall kotor_m15c_quick_get_current_name_006effa0(kotor_m15c::Quick* self, void*, gui_load::String* out) noexcept(false);
void __fastcall kotor_m15c_quick_do_quick_stats_006effd0(kotor_m15c::Quick* self, void*) noexcept(false);
void __fastcall kotor_m15c_quick_on_panel_added_006f0260(gui_load::Panel* self, void*) noexcept(false);
void __fastcall kotor_m15c_quick_handle_input_006f0280(gui_load::Object* self, void*, gui_load::I32 event, gui_load::I32 pressed) noexcept(false);
void* __fastcall kotor_m15c_quick_deleting_destruct_006f06e0(gui_load::Panel* self, void*, gui_load::Word flags);
void __fastcall kotor_m15c_quick_destruct_006f0700(kotor_m15c::Quick* self, void*);

kotor_m15c::Custom* __fastcall kotor_m15c_custom_construct_006ef730(kotor_m15c::Custom* self, void*, gui_load::Object* manager,
    void* creature) noexcept(false);                                                                               // RET 8
void __fastcall kotor_m15c_custom_set_parent_panel_006eef60(kotor_m15c::Custom* self, void*, kotor_m15c::MainCharGen* parent);
void __fastcall kotor_m15c_custom_set_state_006eefd0(kotor_m15c::Custom* self, void*, gui_load::I32 state) noexcept(false);
void __fastcall kotor_m15c_custom_portrait_creation_completed_006ef4c0(void* custom_panel, void*) noexcept(false);
void __fastcall kotor_m15c_custom_name_creation_completed_006ef4e0(void* custom_panel, void*) noexcept(false);
void __fastcall kotor_m15c_custom_clear_abilities_006ef540(kotor_m15c::Custom* self, void*) noexcept(false);
void __fastcall kotor_m15c_custom_cancel_panel_006ef570(kotor_m15c::Custom* self, void*) noexcept(false);
void __fastcall kotor_m15c_custom_handle_input_006ef610(gui_load::Object* self, void*, gui_load::I32 event, gui_load::I32 pressed) noexcept(false);
void* __fastcall kotor_m15c_custom_deleting_destruct_006efb20(gui_load::Panel* self, void*, gui_load::Word flags);
void __fastcall kotor_m15c_custom_destruct_006efb40(kotor_m15c::Custom* self, void*);

// ---------------------------------------------------------------- CSWGuiPortraitCharGen / CSWGuiNameChargen (m15c_portrait_name.cpp)
kotor_m15c::Portrait* __fastcall kotor_m15c_portrait_construct_006f9430(kotor_m15c::Portrait* self, void*, gui_load::Object* manager,
    void* creature) noexcept(false);                                                                               // RET 8
void __fastcall kotor_m15c_portrait_set_parent_panel_006f8a00(kotor_m15c::Portrait* self, void*, gui_load::Panel* parent,
    gui_load::Word kind);                                                                                          // RET 8
kotor_m15c::NameChargen* __fastcall kotor_m15c_name_construct_006f9e70(kotor_m15c::NameChargen* self, void*, gui_load::Object* manager,
    void* creature) noexcept(false);                                                                               // RET 8
void __fastcall kotor_m15c_name_set_parent_panel_006f9b80(kotor_m15c::NameChargen* self, void*, gui_load::Panel* parent,
    gui_load::Word kind);                                                                                          // RET 8
}
