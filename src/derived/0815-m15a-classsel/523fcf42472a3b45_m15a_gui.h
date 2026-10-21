#pragma once
// lane-m15-a (L15-A): CSWGuiMainMenu::OnNewGamePicked post-guard body (lifts trap #71 0067AFFB),
// CSWGuiClassSelection (0x1560; operator new at 0067B0F7) and CSWGuiClassSelChar (0x25C x 6), method table 00758020.
// Original: swkotor.exe GOG K1, static disassembly only. Conventions as gui-load / lane-m1-options (D1/D21/D24/D47).
#include "8c61ee1430fb8439_gui_objects.h"
#include "c9d0373605f94236_mainmenu.h"
#include "02417a06c1bef2bc_m15a_common.h"
#include <cstddef>
#include <cstdint>

namespace kotor_m15a {
namespace kih = kotor_input_hover;

// CSWGuiClassSelChar (0x25C; ctor 006DC350, dtor 006DB6D0).
struct ClassSelChar {
    gui_load::Button button;           // +000 CSWGuiButton (0041C0F0 at 006DC370) "BTN_SEL%d" (006DC7F7)
    gui_load::Control view;            // +1C4 CSWGui3DSceneView: CSWGuiControl (0041AA80 at 006DC389) + vptr 00752E30
                                       //      (006DC396) "3D_MODEL%d" (006DC820)
    gui_load::GuiScene scene;          // +220 CSWGuiScene (004174B0 at 006DC39C); +234 aur scene, +238 aur camera
    void* creature;                    // +254 CSWCCreature* (006DCC07; deleted by the panel dtor 006DB760..6C)
    float timer;                       // +258 = -1.0f (006DCC12); turntable/extent timer of Draw 006DC123..
};
static_assert(offsetof(ClassSelChar, view) == 0x1C4 && offsetof(ClassSelChar, scene) == 0x220 &&
              offsetof(ClassSelChar, creature) == 0x254 && offsetof(ClassSelChar, timer) == 0x258 &&
              sizeof(ClassSelChar) == 0x25C, "CSWGuiClassSelChar 0x25C");

// CSWGuiClassSelection (0x1560): members per the ctor 006DC3C0 (construction order) and dtor 006DB720.
struct ClassSelection : gui_load::Panel {   // +0000 CSWGuiPanel (0040B570 at 006DC3F1)
    gui_load::Word u64;                // +0064 (no access in the L15-A bodies)
    void* selected;                    // +0068 creature of the picked class (OnSelectButton 006DBA19; Draw 006DC0AF)
    ClassSelChar chars[6];             // +006C eh_vector_constructor_iterator(+6C, 25Ch, 6) 006DC41A
    gui_load::Label lbl_char_gen;      // +0E94 "LBL_CHAR_GEN" (006DCCDD)
    gui_load::Label lbl_instruction;   // +0FD4 "LBL_INSTRUCTION" (006DCCA1)
    gui_load::Label lbl_desc;          // +1114 "LBL_DESC" (006DCD19); text params +11FC (OnEnterButton 006DBCE8)
    gui_load::Label lbl_class;         // +1254 "LBL_CLASS" (006DCD55); text params +133C (006DBCF8)
    gui_load::Button btn_back;         // +1394 "BTN_BACK" (006DCD91)
    unsigned char module[8];           // +1558 CExoString (005B3190 at 006DC48C; assigned 006DCDD8; 005E5C20 at 006DB7B8)
};
static_assert(offsetof(ClassSelection, selected) == 0x68 && offsetof(ClassSelection, chars) == 0x6C &&
              offsetof(ClassSelection, lbl_char_gen) == 0xE94 && offsetof(ClassSelection, lbl_instruction) == 0xFD4 &&
              offsetof(ClassSelection, lbl_desc) == 0x1114 && offsetof(ClassSelection, lbl_class) == 0x1254 &&
              offsetof(ClassSelection, btn_back) == 0x1394 && offsetof(ClassSelection, module) == 0x1558 &&
              sizeof(ClassSelection) == 0x1560, "CSWGuiClassSelection 0x1560");

// CSWGuiControlEvent callbacks registered by the ctor (thiscall members, RET 4; single-inheritance view, D47).
struct ClassSelectionHandlers : kih::GuiObject {
    void __thiscall OnSelectButton_006db9b0(kih::Control* control);   // events 27h/2Dh on chars[k].button
    void __thiscall OnEnterButton_006dba70(kih::Control* control);    // event 0 on chars[k].button
};
}

extern "C" {
// 006DC3C0 CSWGuiClassSelection::CSWGuiClassSelection(CSWGuiManager*, const CExoString* module) RET 8, EAX = this.
kotor_m15a::ClassSelection* __fastcall kotor_m15a_class_selection_construct_006dc3c0(kotor_m15a::ClassSelection* self,
    void* unused_edx, gui_load::Object* manager, const gui_load::String* module) noexcept(false);
// 006DB720 ~CSWGuiClassSelection RET 0; 006DC330 scalar deleting dtor (flags) RET 4, EAX = this.
void __fastcall kotor_m15a_class_selection_destruct_006db720(kotor_m15a::ClassSelection* self, void* unused_edx);
void* __fastcall kotor_m15a_class_selection_deleting_destruct_006dc330(kotor_m15a::ClassSelection* self, void* unused_edx,
                                                                      gui_load::Word flags);
// 006DB8F0 OnPanelAdded RET 0 (method table +48).
void __fastcall kotor_m15a_class_selection_on_panel_added_006db8f0(gui_load::Panel* self, void* unused_edx) noexcept(false);
// 006DBD30 HandleInputEvent(event, pressed) RET 8 (method table +3C).
void __fastcall kotor_m15a_class_selection_handle_input_006dbd30(gui_load::Object* self, void* unused_edx,
                                                                gui_load::I32 event, gui_load::I32 pressed) noexcept(false);
// 006DC030 Draw(float delta; one stack word) RET 4 (method table +34).
void __fastcall kotor_m15a_class_selection_draw_006dc030(gui_load::Panel* self, void* unused_edx, gui_load::Word delta) noexcept(false);
// 006DC350 CSWGuiClassSelChar::CSWGuiClassSelChar() RET 0, EAX = this; 006DB6D0 dtor RET 0.
kotor_m15a::ClassSelChar* __fastcall kotor_m15a_class_sel_char_construct_006dc350(kotor_m15a::ClassSelChar* self,
                                                                                 void* unused_edx) noexcept(false);
void __fastcall kotor_m15a_class_sel_char_destruct_006db6d0(kotor_m15a::ClassSelChar* self, void* unused_edx);
// D21 method table 00758020 (27 slots).
extern const struct KotorM15aClassSelectionMethods kotor_m15a_class_selection_methods_00758020;

// .data 007A2684: 6 x 8-byte class choices {BYTE class (written by the dynamic initializer 0073B690), BYTE gender,
// BYTE sound set, BYTE pad, DWORD description strref}; owner unit m15a-xc-classsel (m15a_xc_classsel.cpp).
struct KotorM15aClassChoice { std::uint8_t class_id; std::uint8_t gender; std::uint8_t sound_set; std::uint8_t pad;
                              std::uint32_t description; };
extern KotorM15aClassChoice kotor_m15a_class_choices_007a2684[6];
}
