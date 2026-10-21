// lane-m15-c (L15-C, Milestone 1.5): CSWGuiEditText and CSWGuiEditbox (character-name entry).
//   CSWGuiEditText: ctor 004173D0, dtor 00417460, deleting 00417440, RemoveLastChar 004162E0, AddNewChar 004163A0,
//                   SetText 00416490, SetCaretVisible 00416530; method table 0073E444
//   CSWGuiEditbox:  ctor 0041C6B0, dtor 0041C740, deleting 0041C720, Load 0041C7D0, Initialize 004183B0, Draw 00418420,
//                   SetExtent 00418450, SetEnabled 004184B0, HandleKeyPress 004184F0, SetFocus 00418660,
//                   GetIsSelectable 00418670, HandleLMouseUp 004186B0, ReSetFont 004186D0, HandleFocusChange 0041A820;
//                   method table 0073EAC8 (40 slots); one-character key buffer 007A4580
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; each behaviour cites its instruction
// addresses. Tables read from .rdata:
//   0073E444 = {00417440, 00416280, 00414EB0, 00416240, 00416050}
//   0073EAC8 = {0041C720, 00418450, 005B5E90, 0041A980, 00409EB0, 004015A0, 00418680, 004186B0, 004015A0, 0041AC70,
//               0063E7F0, 0063E7F0, 00641DB0, 004064A0, 00418420, 0041A9D0, 004181D0, 004187C0, 0041C7D0, 00641DB0,
//               0063E7F0 x10 (+50..+74), 00641DB0, 0041A820, 00418670, 0041AC30, 004184B0, 004186D0, 00418A90,
//               005B5E90, 004183B0, 004184F0}
// Conventions: D1 fastcall veneers, D21 typed tables (fields typed like the function they hold: fastcall pointers or the
// single-inheritance lane-input / lane-m1-saveload member PMFs), D24 vptr slot views at call sites, D30-FINAL (no float
// arithmetic; 1.0f is passed as its bit pattern 3F800000h like 004183F6).
#include "868937c3784da962_m15c_editbox.h"
#include "40bb793f0e269984_saveload.h"            // lane-m1-saveload: RadioButtonLeaves::HandleLMouseDown_00418680 (slot +18)
#include "c50d01ce958a4a5e_source_substring.h"    // startup_rand::SubString005e6270
#include "b3ed8cdd89724107_rules_common.h"        // kotor_reconstruction::StartupFormat005e5680, WindowCStr005e5670
#include <cstring>
#include <new>

using namespace kotor_m15c_editbox;
using gui_load::Object;
using gui_load::Word;
using gui_load::Extent;
using String = kotor_reconstruction::StartupString;
namespace kih = kotor_input_hover;

extern "C" {
// lane-gui-load / core47 / m48 providers (functional.map M60)
void __fastcall gui_load_text_params_set_color_00414e10(gui_load::TextParams* self, void*, const float* color);   // core47
void __fastcall gui_load_text_params_restore_default_color_00414e80(gui_load::TextParams* self, void*);           // core47
extern const float kotor_gui_disabled_color_0078d3b4[3];                                                          // core47 (.data 0078D3B4)
void __fastcall kotor_fastcall_00417310(void* text, void*, const void* extent, void* text_params,
                                        Word scale_bits) noexcept(false);                                         // gui-render
void __fastcall gui_m48_text_params_set_font_rename_00415df0(void* params, void*) noexcept(false);                // m48b
void __fastcall kotor_fastcall_0040a200(void* gui, void*, void* control);                                         // core47 ReleaseMouse
void __fastcall kotor_fastcall_005df520(kih::Input* in, void*);                                                   // m48t-f-keys KeyboardModeOn
void __fastcall kotor_fastcall_005df530(kih::Input* in, void*);                                                   // m48t-f-keys KeyboardModeOff
// .bss cell 007A4580: two-byte key string (only writer/reader HandleKeyPress 0041859C..A8). Single owner here.
char kotor_m15c_editbox_key_007a4580[2] = {0, 0};
}

namespace {
String* value_of(EditText* self) { return reinterpret_cast<String*>(self->value); }        // +78
gui_load::TextParams* params_of(EditText* self) {                                          // +18 (CSWGuiText params)
    return reinterpret_cast<gui_load::TextParams*>(reinterpret_cast<unsigned char*>(&self->text) + 0x18);
}
// CSWGuiText::Draw slot +0C (00416240) and CSWGuiBorder::Draw slot +0C (004168C0): thiscall(int) RET 4.
using DrawSlot = void (__fastcall*)(void*, void*, Word);
struct DrawView { const void* s00; const void* s04; const void* s08; DrawSlot draw; };
static_assert(offsetof(DrawView, draw) == 0x0C, "draw slot +0C");
template <class T> const DrawView* draw_view(T* object) {
    const void* vtbl;
    std::memcpy(&vtbl, object, sizeof(vtbl));
    return static_cast<const DrawView*>(vtbl);
}
// Displayed text: the edit string, or "%s_" of it while the caret is visible (+74 bit0) - shared tail of
// 0041634D..78, 00416430..5B, 004164CD..F8.
void show(EditText* self, String* shown_source) {
    if (self->flags & 1u) {
        String caret;                                                                // 005B3190 (function-entry local)
        kotor_reconstruction::StartupFormat005e5680(&caret, "%s_", WindowCStr005e5670(shown_source, nullptr));  // 0073E3EC
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, &caret);
    } else {
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, shown_source);
    }
}
struct Fresh {
    alignas(String) unsigned char raw[sizeof(String)];
    String* object = nullptr;
    String* storage() { return reinterpret_cast<String*>(raw); }
    ~Fresh() { if (object) object->~StartupString(); }
};
}

namespace menu_storage { struct Text; }
namespace gui_wrap_draw { void __fastcall text_draw_00416240(menu_storage::Text*, void*, unsigned int); }  // M40 gui-text-draw.obj
extern "C" {
// ======================================================================= CSWGuiEditText method table 0073E444
struct KotorM15cEditTextMethods {
    void* (__fastcall* s00)(EditText*, void*, Word);                                         // +00 00417440
    void (__fastcall* s04)(gui_load::Text*, void*, const Extent*);                           // +04 00416280 CSWGuiText::SetExtent
    Word (__fastcall* s08)(gui_load::Text*, void*);                                          // +08 00414EB0 GetIdealHeight
    void (__fastcall* s0c)(menu_storage::Text*, void*, unsigned int);                        // +0C 00416240 CSWGuiText::Draw
    void (__fastcall* s10)(gui_load::Text*, void*, gui_load::Gff*, gui_load::Struct*, gui_load::String*);   // +10 00416050 Load
};
static_assert(sizeof(KotorM15cEditTextMethods) == 20, "0073E444 slot count (5)");
extern const KotorM15cEditTextMethods kotor_m15c_edit_text_methods_0073e444;
const KotorM15cEditTextMethods kotor_m15c_edit_text_methods_0073e444 = {
    &kotor_m15c_edit_text_deleting_destruct_00417440,
    &gui_load_text_set_extent_00416280,
    &gui_load_text_ideal_height_00414eb0,
    &gui_wrap_draw::text_draw_00416240,
    &gui_load_text_load_00416050,
};

// ======================================================================= CSWGuiEditText
// 004173D0 ctor (RET; EAX = this): CSWGuiText::CSWGuiText 00417280; table 0073E444; CExoString +78; +74 &= ~1;
// +70 = FFFFh; +78 = "".
EditText* __fastcall kotor_m15c_edit_text_construct_004173d0(EditText* self, void*) noexcept(false) {
    (void)gui_load_text_construct_00417280(&self->text, nullptr);                // 004173EE
    try {
        self->text.vtbl = reinterpret_cast<const gui_load::TextVtbl*>(&kotor_m15c_edit_text_methods_0073e444);   // 00417400
        new (self->value) String();                                               // 00417406 (005B3190)
        // 0041740B AND [esi+74h],FFFFFFFEh: only bit0 of +74 is ever read (TEST byte at 0041634D / 00416430 /
        // 004164CD; SetCaretVisible keeps the other bits). D2 (0041740B): the other 31 bits are indeterminate
        // heap bytes in the original; value-initialised to 0 here (no reader).
        self->flags = 0;
        self->max_length = -1;                                                    // 0041741B
        (void)GuiAssign005e5140(value_of(self), nullptr, "");                    // 0041740F..21 (0073D71C)
    } catch (...) {
        gui_load_text_destruct_00415ff0(&self->text, nullptr);                    // EH state 0 (FuncInfo 00715073)
        throw;
    }
    return self;                                                                  // 0041742B
}
// 00417460 dtor: ~CExoString(+78) then CSWGuiText::~CSWGuiText 00415FF0 (no vptr store).
void __fastcall kotor_m15c_edit_text_destruct_00417460(EditText* self, void*) noexcept {
    value_of(self)->~StartupString();                                             // 0041747D..88 (005E5C20)
    gui_load_text_destruct_00415ff0(&self->text, nullptr);                        // 0041748D..97
}
// 00417440 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_edit_text_deleting_destruct_00417440(EditText* self, void*, Word flags) noexcept {
    kotor_m15c_edit_text_destruct_00417460(self, nullptr);                        // 00417443
    if (flags & 1u) live_free(self);                                              // 00417448..55
    return self;
}
// 004162E0 RemoveLastChar (RET).
void __fastcall kotor_m15c_edit_text_remove_last_char_004162e0(EditText* self, void*) noexcept(false) {
    String caret;                                                                 // 004162FC..300 (005B3190)
    String* const value = value_of(self);                                         // 00416305
    if (value->BorrowedLength005e5790() <= 0) return;                             // 0041630D..19
    {
        Fresh shorter;
        shorter.object = startup_rand::SubString005e6270(value, nullptr, shorter.storage(), 0,
                                                         value->BorrowedLength005e5790() - 1);   // 0041631B..2D
        value->Assign005e5c50(shorter.object);                                    // 00416332..3A
    }                                                                             // 0041633F..48
    if (self->flags & 1u) {                                                       // 0041634D..51
        kotor_reconstruction::StartupFormat005e5680(&caret, "%s_", WindowCStr005e5670(value, nullptr));  // 00416353..6A
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, &caret); // 0041636D..78
    } else {
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, value);  // 00416374..78
    }
}
// 004163A0 AddNewChar(const char* text) (RET 4): refused once the length reaches max_length (unless -1).
void __fastcall kotor_m15c_edit_text_add_new_char_004163a0(EditText* self, void*, const char* text) noexcept(false) {
    String caret;                                                                 // 004163BB..BF
    if (static_cast<std::uint16_t>(self->max_length) != 0xFFFFu &&                // 004163C4..D1
        value_of(self)->BorrowedLength005e5790() >= static_cast<std::int32_t>(self->max_length)) return;   // 004163D3..E1
    {
        String added(text);                                                       // 004163E3..ED (005E5A90)
        Fresh sum;
        sum.object = GuiConcat005e5d10(value_of(self), nullptr, sum.storage(), &added);   // 004163F2..402
        value_of(self)->Assign005e5c50(sum.object);                               // 00416407..0F
    }                                                                             // 00416414..2B
    if (self->flags & 1u) {                                                       // 00416430..34
        kotor_reconstruction::StartupFormat005e5680(&caret, "%s_", WindowCStr005e5670(value_of(self), nullptr));  // 00416436..4D
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, &caret); // 00416450..5B
    } else {
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, value_of(self));   // 00416457..5B
    }
}
// 00416490 SetText(const CExoString& text) (RET 4).
void __fastcall kotor_m15c_edit_text_set_text_00416490(EditText* self, void*, gui_load::String* text) noexcept(false) {
    String caret;                                                                 // 004164AD..B1
    value_of(self)->Assign005e5c50(text);                                         // 004164B6..C8
    if (self->flags & 1u) {                                                       // 004164CD..D1: "%s_" of the ARGUMENT
        kotor_reconstruction::StartupFormat005e5680(&caret, "%s_", WindowCStr005e5670(text, nullptr));   // 004164D3..EA
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, &caret); // 004164ED..F8
    } else {
        gui_load_text_params_set_text_00415e00(params_of(self), nullptr, value_of(self));   // 004164F4..F8
    }
}
// 00416530 SetCaretVisible(int visible) (RET 4): +74 bit0 = visible bit0, then show.
void __fastcall kotor_m15c_edit_text_set_caret_visible_00416530(EditText* self, void*, Word visible) noexcept(false) {
    self->flags ^= (self->flags ^ visible) & 1u;                                  // 0041654F..5E
    show(self, value_of(self));                                                   // 0041655C..C6 (test full word)
}

// ======================================================================= CSWGuiEditbox method table 0073EAC8
struct KotorM15cEditboxMethods {
    void* (__fastcall* s00)(Editbox*, void*, Word);                              // +00 0041C720 (this lane)
    void (__fastcall* s04)(Editbox*, void*, const Extent*);                      // +04 00418450 (this lane)
    void (__fastcall* s08)(Object*, void*, Word, Word);                          // +08 005B5E90 void_return [RET 8]
    void (kih::Navigable::* s0c)(int);                                           // +0C 0041A980 Navigable::HandleMouseOver
    Word (__fastcall* s10)(Object*, void*, Word, Word);                          // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(Object*, void*);                                      // +14 004015A0 void_return
    void (kotor_m1_saveload::RadioButtonLeaves::* s18)();                        // +18 00418680 (lane-m1-saveload)
    void (__fastcall* s1c)(Editbox*, void*);                                     // +1C 004186B0 (this lane)
    void (__fastcall* s20)(Object*, void*);                                      // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                      // +24 0041AC70 HandleRMouseUp
    Word (__fastcall* s28)(Object*, void*);                                      // +28 0063E7F0
    Word (__fastcall* s2c)(Object*, void*);                                      // +2C 0063E7F0
    Object* (__fastcall* s30)(Object*, void*);                                   // +30 00641DB0
    Word (__fastcall* s34)(Object*, void*);                                      // +34 004064A0
    void (__fastcall* s38)(Editbox*, void*, Word);                               // +38 00418420 (this lane)
    void (kih::Navigable::* s3c)(int, int);                                      // +3C 0041A9D0 Navigable::HandleInputEvent
    void (kih::Control::* s40)(int);                                             // +40 004181D0 Control::SetActive
    kih::Control* (kih::Control::* s44)(int, int);                               // +44 004187C0 Control::HitCheckMouse
    void (__fastcall* s48)(Editbox*, void*, gui_load::Gff*, gui_load::Struct*);  // +48 0041C7D0 (this lane)
    Object* (__fastcall* s4c)(Object*, void*);                                   // +4C 00641DB0
    Word (__fastcall* s50_74[10])(Object*, void*);                               // +50..+74 0063E7F0
    Object* (__fastcall* s78)(Object*, void*);                                   // +78 00641DB0
    void (__fastcall* s7c)(Editbox*, void*, Word);                               // +7C 0041A820 (this lane)
    Word (__fastcall* s80)(Editbox*, void*);                                     // +80 00418670 (this lane)
    void (__fastcall* s84)(Object*, void*);                                      // +84 0041AC30 HandleRMouseDown
    void (__fastcall* s88)(Editbox*, void*, Word);                               // +88 004184B0 (this lane)
    void (__fastcall* s8c)(Editbox*, void*);                                     // +8C 004186D0 (this lane)
    Word (__fastcall* s90)(Object*, void*);                                      // +90 00418A90 DisplayToolTip
    void (__fastcall* s94)(Object*, void*, Word, Word);                          // +94 005B5E90
    void (__fastcall* s98)(Editbox*, void*, const Extent*, Word, Word);          // +98 004183B0 (this lane)
    void (__fastcall* s9c)(Editbox*, void*, Word);                               // +9C 004184F0 (this lane)
};
static_assert(sizeof(KotorM15cEditboxMethods) == 0xA0, "0073EAC8 slot count (40)");
static_assert(sizeof(void (kotor_m1_saveload::RadioButtonLeaves::*)()) == 4 && sizeof(void (kih::Navigable::*)(int)) == 4,
              "single-inheritance PMFs (D107)");
extern const KotorM15cEditboxMethods kotor_m15c_editbox_methods_0073eac8;
// Slot bodies (this lane)
void __fastcall kotor_m15c_editbox_set_extent_00418450(Editbox* self, void*, const Extent* extent);
void __fastcall kotor_m15c_editbox_handle_lmouse_up_004186b0(Editbox* self, void*);
void __fastcall kotor_m15c_editbox_draw_00418420(Editbox* self, void*, Word flags);
void __fastcall kotor_m15c_editbox_load_0041c7d0(Editbox* self, void*, gui_load::Gff* gff, gui_load::Struct* st) noexcept(false);
void __fastcall kotor_m15c_editbox_focus_change_0041a820(Editbox* self, void*, Word focus) noexcept(false);
Word __fastcall kotor_m15c_editbox_is_selectable_00418670(Editbox* self, void*);
void __fastcall kotor_m15c_editbox_set_enabled_004184b0(Editbox* self, void*, Word enabled);
void __fastcall kotor_m15c_editbox_reset_font_004186d0(Editbox* self, void*) noexcept(false);
void __fastcall kotor_m15c_editbox_initialize_004183b0(Editbox* self, void*, const Extent* extent, Word text_source,
                                                       Word border_source) noexcept(false);
void __fastcall kotor_m15c_editbox_handle_key_press_004184f0(Editbox* self, void*, Word key) noexcept(false);

const KotorM15cEditboxMethods kotor_m15c_editbox_methods_0073eac8 = {
    &kotor_m15c_editbox_deleting_destruct_0041c720,                  // +00
    &kotor_m15c_editbox_set_extent_00418450,                         // +04
    &gui_load_slot_ret8_005b5e90,                                    // +08
    &kih::Navigable::HandleMouseOver,                                // +0C
    &gui_load_slot_zero8_00409eb0,                                   // +10
    &gui_load_slot_ret_004015a0,                                     // +14
    &kotor_m1_saveload::RadioButtonLeaves::HandleLMouseDown_00418680,   // +18
    &kotor_m15c_editbox_handle_lmouse_up_004186b0,                   // +1C
    &gui_load_slot_ret_004015a0,                                     // +20
    &gui_load_control_handle_rmouse_up_0041ac70,                     // +24
    &gui_load_slot_zero_0063e7f0,                                    // +28
    &gui_load_slot_zero_0063e7f0,                                    // +2C
    &gui_load_slot_self_00641db0,                                    // +30
    &gui_load_slot_prop10_004064a0,                                  // +34
    &kotor_m15c_editbox_draw_00418420,                               // +38
    &kih::Navigable::HandleInputEvent,                               // +3C
    &kih::Control::SetActive,                                        // +40
    &kih::Control::HitCheckMouse,                                    // +44
    &kotor_m15c_editbox_load_0041c7d0,                               // +48
    &gui_load_slot_self_00641db0,                                    // +4C
    {&gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,
     &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,
     &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0},   // +50..+74
    &gui_load_slot_self_00641db0,                                    // +78
    &kotor_m15c_editbox_focus_change_0041a820,                       // +7C
    &kotor_m15c_editbox_is_selectable_00418670,                      // +80
    &gui_load_control_rmouse_down_0041ac30,                          // +84
    &kotor_m15c_editbox_set_enabled_004184b0,                        // +88
    &kotor_m15c_editbox_reset_font_004186d0,                         // +8C
    &gui_load_control_tooltip_00418a90,                              // +90
    &gui_load_slot_ret8_005b5e90,                                    // +94
    &kotor_m15c_editbox_initialize_004183b0,                         // +98
    &kotor_m15c_editbox_handle_key_press_004184f0,                   // +9C
};

// ======================================================================= CSWGuiEditbox
// 0041C6B0 ctor (RET; EAX = this): CSWGuiNavigable 0041B8B0; table 0073EAC8; border +6C (004167F0); edit text +E0
// (004173D0); +44 |= 4.
Editbox* __fastcall kotor_m15c_editbox_construct_0041c6b0(Editbox* self, void*) noexcept(false) {
    (void)gui_load_navigable_construct_0041b8b0(self, nullptr);                  // 0041C6CD
    int built = 0;                                                                // EH state 0 (FuncInfo 00715353)
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_editbox_methods_0073eac8);   // 0041C6DD
        (void)gui_load_border_construct_004167f0(&self->border, nullptr);         // 0041C6E3
        built = 1;                                                                // 0041C6EE
        (void)kotor_m15c_edit_text_construct_004173d0(&self->edit, nullptr);      // 0041C6F3
    } catch (...) {
        if (built >= 1) gui_load_border_destruct_004151c0(&self->border, nullptr);
        gui_load_navigable_destruct_00419ce0(self, nullptr);                      // state 0 funclet: ~CSWGuiNavigable
        throw;
    }
    self->flags |= 4u;                                                            // 0041C6F8..702
    return self;                                                                  // 0041C705
}
// 0041C740 dtor: table 0073EAC8; edit text members (~CExoString +158, ~CSWGuiText +E0) inline; ~CSWGuiBorder +6C;
// table 0073E520 (CSWGuiNavigable, inlined base dtor) and CSWGuiControl::~CSWGuiControl 00419A20.
void __fastcall kotor_m15c_editbox_destruct_0041c740(Editbox* self, void*) noexcept {
    self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_editbox_methods_0073eac8);   // 0041C760
    value_of(&self->edit)->~StartupString();                                      // 0041C76E..80 (edi+78)
    gui_load_text_destruct_00415ff0(&self->edit.text, nullptr);                   // 0041C785..8C
    gui_load_border_destruct_004151c0(&self->border, nullptr);                    // 0041C791..99
    self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_load_navigable_methods_0073e520);   // 0041C7A8
    gui_load_control_destruct_00419a20(self, nullptr);                            // 0041C79E..AE
}
// 0041C720 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_editbox_deleting_destruct_0041c720(Editbox* self, void*, Word flags) noexcept {
    kotor_m15c_editbox_destruct_0041c740(self, nullptr);                          // 0041C723
    if (flags & 1u) live_free(self);                                              // 0041C728..35
    return self;
}
// 0041C7D0 Load(CResGFF*, CResGFFStruct*) (RET 8): text "TEXT" (slot +10), border "BORDER" (slot +10),
// then CSWGuiControl::Load 0041B8E0.
void __fastcall kotor_m15c_editbox_load_0041c7d0(Editbox* self, void*, gui_load::Gff* gff, gui_load::Struct* st) noexcept(false) {
    {
        String label("TEXT");                                                     // 0041C7ED..F6 (0073E3CC)
        self->edit.text.vtbl->load(&self->edit.text, nullptr, gff, st, &label);   // 0041C803..1E [E0]+10
    }
    {
        String label("BORDER");                                                   // 0041C832..3B (0073DFD4)
        self->border.vtbl->load(&self->border, nullptr, gff, st, &label);         // 0041C840..55 [6C]+10
    }
    gui_load_control_load_0041b8e0(self, nullptr, gff, st);                       // 0041C869..6D
}
// 004183B0 Initialize(const CSWGuiExtent* extent, text source, border source) (RET 0Ch).
void __fastcall kotor_m15c_editbox_initialize_004183b0(Editbox* self, void*, const Extent* extent, Word text_source,
                                                       Word border_source) noexcept(false) {
    self->extent = *extent;                                                       // 004183B6..D3
    gui_load_border_initialize_00415190(&self->border, nullptr, &self->extent,
                                        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(border_source)));  // 004183D6..E1
    Extent inner;
    (void)gui_load_border_inner_extent_00415240(&self->border, nullptr, &inner);  // 004183E6..ED
    kotor_fastcall_00417310(&self->edit.text, nullptr, &inner,
                            reinterpret_cast<void*>(static_cast<std::uintptr_t>(text_source)), 0x3F800000u);   // 004183F2..407 (1.0f)
}
// 00418420 Draw(int) (RET 4): border slot +0C, then edit text slot +0C, same argument.
void __fastcall kotor_m15c_editbox_draw_00418420(Editbox* self, void*, Word flags) {
    draw_view(&self->border)->draw(&self->border, nullptr, flags);                // 00418428..2F
    draw_view(&self->edit.text)->draw(&self->edit.text, nullptr, flags);          // 00418432..3F
}
// 00418450 SetExtent(const CSWGuiExtent*) (RET 4).
void __fastcall kotor_m15c_editbox_set_extent_00418450(Editbox* self, void*, const Extent* extent) {
    self->extent = *extent;                                                       // 00418456..73
    self->border.vtbl->set_extent(&self->border, nullptr, &self->extent);         // 00418476..7F slot +04
    Extent inner;
    (void)gui_load_border_inner_extent_00415240(&self->border, nullptr, &inner);  // 00418482..89
    self->edit.text.vtbl->set_extent(&self->edit.text, nullptr, &inner);          // 0041848E..9F slot +04
}
// 004184B0 SetEnabled(int) (RET 4): CSWGuiControl::SetEnabled 004176A0 (leaves ECX = this), then the edit text
// colour: RestoreDefaultColor 00414E80 (enabled) or SetColor(0078D3B4) 00414E10 (tail jump).
void __fastcall kotor_m15c_editbox_set_enabled_004184b0(Editbox* self, void*, Word enabled) {
    gui_load_control_set_enabled_004176a0(self, nullptr, enabled);               // 004184B1..B6
    if (enabled) gui_load_text_params_restore_default_color_00414e80(params_of(&self->edit), nullptr);   // 004184C0..C6 (+F8)
    else gui_load_text_params_set_color_00414e10(params_of(&self->edit), nullptr, kotor_gui_disabled_color_0078d3b4);  // 004184CE..DC
}
// 004184F0 HandleKeyPress(int key) (RET 4). Jump table 004185BC / index bytes 004185DC (key - 8, 0..77h):
//   08h,7Fh -> RemoveLastChar; 0Ah,0Dh -> owner panel event 27h; 1Bh -> owner panel event 28h;
//   1Ch/1Dh/1Eh/1Fh -> own HandleInputEvent 2Fh/31h/30h/32h; others -> printable path 00418588.
void __fastcall kotor_m15c_editbox_handle_key_press_004184f0(Editbox* self, void*, Word key) noexcept(false) {
    const Word index = key - 8u;                                                  // 004184F4
    if (index <= 0x77u) {                                                         // 004184F7..FA (JA)
        switch (key) {
        case 0x08: case 0x7F:                                                     // 0041853E
            kotor_m15c_edit_text_remove_last_char_004162e0(&self->edit, nullptr);
            return;
        case 0x0A: case 0x0D: case 0x1B: {                                        // 0041856A / 0041854C
            Object* const owner = self->owner;                                    // [this+34h]
            if (!owner) return;
            gui_load::Panel* const panel = owner->vtbl->as_panel(owner, nullptr); // slot +28
            if (!panel) return;
            panel->vtbl->handle_input_event(panel, nullptr, key == 0x1B ? 0x28 : 0x27, 1);   // slot +3C
            return;
        }
        case 0x1C: self->vtbl->handle_input_event(self, nullptr, 0x2F, 1); return;   // 0041850E
        case 0x1D: self->vtbl->handle_input_event(self, nullptr, 0x31, 1); return;   // 0041851A
        case 0x1E: self->vtbl->handle_input_event(self, nullptr, 0x30, 1); return;   // 00418526
        case 0x1F: self->vtbl->handle_input_event(self, nullptr, 0x32, 1); return;   // 00418532
        default: break;                                                           // index byte 07 -> 00418588
        }
    }
    if (key == 0x5Fu || key == 0x2Fu || key == 0x5Cu) return;                    // 00418588..95 ('_', '/', '\')
    if (static_cast<std::int32_t>(key) < 0x20) return;                            // 00418597..9A (JL)
    kotor_m15c_editbox_key_007a4580[0] = static_cast<char>(key);                  // 0041859C
    kotor_m15c_editbox_key_007a4580[1] = 0;                                       // 004185A1
    kotor_m15c_edit_text_add_new_char_004163a0(&self->edit, nullptr, kotor_m15c_editbox_key_007a4580);   // 004185A8..B6
}
// 00418660 SetFocus (RET): own slot +7C(1).
void __fastcall kotor_m15c_editbox_set_focus_00418660(Editbox* self, void*) noexcept(false) {
    self->vtbl->focus_change(self, nullptr, 1);                                   // 00418660..64
}
// 00418670 GetIsSelectable (RET): (byte +44 >> 3) & 1.
Word __fastcall kotor_m15c_editbox_is_selectable_00418670(Editbox* self, void*) {
    return (static_cast<Word>(static_cast<unsigned char>(self->flags)) >> 3) & 1u;   // 00418670..77
}
// 004186B0 HandleLMouseUp (RET): CSWGuiManager::ReleaseMouse([007A39F4], this), then JMP own slot +14.
void __fastcall kotor_m15c_editbox_handle_lmouse_up_004186b0(Editbox* self, void*) {
    kotor_fastcall_0040a200(kotor_input_cells::gui_manager_007a39f4, nullptr, self);   // 004186B3..BA
    self->vtbl->release_capture(self, nullptr);                                   // 004186BF..C4 [eax+14h]
}
// 004186D0 ReSetFont (RET): CSWGuiTextParams::SetFontRename 00415DF0 on the edit text params (+F8).
void __fastcall kotor_m15c_editbox_reset_font_004186d0(Editbox* self, void*) noexcept(false) {
    gui_m48_text_params_set_font_rename_00415df0(params_of(&self->edit), nullptr);   // 004186D0..D6
}
// 0041A820 HandleFocusChange(int focus) (RET 4): keyboard mode on/off through CExoInput [007A39E4], focused edit
// box cell [[007A39F4]+18h], caret visibility, then CSWGuiControl::HandleFocusChange 00418960.
void __fastcall kotor_m15c_editbox_focus_change_0041a820(Editbox* self, void*, Word focus) noexcept(false) {
    kih::Input* const input = static_cast<kih::Input*>(kotor_menu_window_cells::input_007a39e4);   // 0041A82A
    if (focus) {                                                                  // 0041A826..30
        kotor_fastcall_005df520(input, nullptr);                                  // 0041A832
        Editbox* const focused = self;
        std::memcpy(static_cast<unsigned char*>(kotor_input_cells::gui_manager_007a39f4) + 0x18, &focused, 4);  // 0041A837..3C
        kotor_m15c_edit_text_set_caret_visible_00416530(&self->edit, nullptr, 1); // 0041A83F / 0041A857..5D
    } else {
        kotor_fastcall_005df530(input, nullptr);                                  // 0041A843
        std::memset(static_cast<unsigned char*>(kotor_input_cells::gui_manager_007a39f4) + 0x18, 0, 4);   // 0041A848..4E
        kotor_m15c_edit_text_set_caret_visible_00416530(&self->edit, nullptr, 0); // 0041A855..5D
    }
    gui_load_control_focus_change_00418960(self, nullptr, focus);                 // 0041A862..65
}
}
