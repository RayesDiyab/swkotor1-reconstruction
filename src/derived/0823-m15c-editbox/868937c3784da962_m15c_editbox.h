#pragma once
// lane-m15-c: CSWGuiEditText (0x80; vtable 0073E444) and CSWGuiEditbox (0x160; vtable 0073EAC8) - m15c_editbox.cpp.
// Used by CSWGuiNameChargen (editbox at +230h). Also the shape used by CSWGuiSaveNamePanel 006CAE70 (in-game save;
// not on the M1.5 path). Layout views cite the instruction establishing each named offset.
#include "8c61ee1430fb8439_gui_objects.h"       // gui_load control family (Navigable, Border, Text, TextParams), kotor_input_hover
#include <cstddef>
#include <cstdint>

namespace kotor_m15c_editbox {
// CSWGuiEditText: CSWGuiText (0x70, ctor 00417280) + max length, flags, edit string (ctor 004173D0).
struct EditText {
    gui_load::Text text;                 // +00 CSWGuiText; params at +18 (SetText 00415E00 at 00416378)
    std::int16_t max_length;             // +70 = -1 (0041741B); MOVSX compare 004163DB
    std::uint16_t pad72;
    std::uint32_t flags;                 // +74 bit0 caret visible (0041740B, 00416555..5E)
    unsigned char value[8];              // +78 CExoString (005B3190 at 00417406; "" at 00417421)
};
static_assert(sizeof(EditText) == 0x80 && offsetof(EditText, max_length) == 0x70 && offsetof(EditText, flags) == 0x74 &&
              offsetof(EditText, value) == 0x78, "CSWGuiEditText 0x80");
// CSWGuiEditbox: CSWGuiNavigable (0x6C, 0041B8B0) + border +6C (004167F0) + edit text +E0 (004173D0).
struct Editbox : gui_load::Navigable {
    gui_load::Border border;             // +6C (0041C6E3)
    EditText edit;                       // +E0 (0041C6F3)
};
static_assert(sizeof(Editbox) == 0x160 && offsetof(Editbox, border) == 0x6C && offsetof(Editbox, edit) == 0xE0,
              "CSWGuiEditbox 0x160 (CSWGuiNameChargen +230..+390)");
}

extern "C" {
// CSWGuiEditText
kotor_m15c_editbox::EditText* __fastcall kotor_m15c_edit_text_construct_004173d0(kotor_m15c_editbox::EditText* self,
    void* unused_edx) noexcept(false);
void __fastcall kotor_m15c_edit_text_destruct_00417460(kotor_m15c_editbox::EditText* self, void* unused_edx) noexcept;
void* __fastcall kotor_m15c_edit_text_deleting_destruct_00417440(kotor_m15c_editbox::EditText* self, void* unused_edx,
    std::uint32_t flags) noexcept;
void __fastcall kotor_m15c_edit_text_remove_last_char_004162e0(kotor_m15c_editbox::EditText* self, void* unused_edx) noexcept(false);
void __fastcall kotor_m15c_edit_text_add_new_char_004163a0(kotor_m15c_editbox::EditText* self, void* unused_edx,
    const char* text) noexcept(false);                                                         // RET 4
void __fastcall kotor_m15c_edit_text_set_text_00416490(kotor_m15c_editbox::EditText* self, void* unused_edx,
    gui_load::String* text) noexcept(false);                                                   // RET 4
void __fastcall kotor_m15c_edit_text_set_caret_visible_00416530(kotor_m15c_editbox::EditText* self, void* unused_edx,
    std::uint32_t visible) noexcept(false);                                                    // RET 4
// CSWGuiEditbox
kotor_m15c_editbox::Editbox* __fastcall kotor_m15c_editbox_construct_0041c6b0(kotor_m15c_editbox::Editbox* self,
    void* unused_edx) noexcept(false);
void __fastcall kotor_m15c_editbox_destruct_0041c740(kotor_m15c_editbox::Editbox* self, void* unused_edx) noexcept;
void* __fastcall kotor_m15c_editbox_deleting_destruct_0041c720(kotor_m15c_editbox::Editbox* self, void* unused_edx,
    std::uint32_t flags) noexcept;
void __fastcall kotor_m15c_editbox_set_focus_00418660(kotor_m15c_editbox::Editbox* self, void* unused_edx) noexcept(false);
}
