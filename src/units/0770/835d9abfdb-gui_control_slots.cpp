// lane-m47-core: CSWGuiLabel / CSWGuiButton virtual slot bodies of tables 0073E5B8 / 0073E658 (gui-load
// gui_vtables.cpp, slots +88/+8C/+98/+9C) and their CSWGuiTextParams colour leaves.
// Reachability: link-required by the gui-load method tables (gui-load classifies them "not first-frame").
// Evidence: dumpbin /disasm swkotor.exe (lane dis/<addr>.asm): 00418CD0..00418CF2, 00418D00..00418D31,
// 00418D40..00418D46, 00418D80..00418DA7, 00418DB0..00418DE7, 00418DF0..00418DF6, 00417970..00417A1B,
// 00414E80..00414E89, 00414E10..00414E44, 00415DF0 (JMP 00415D60), 004176A0 (preserves ECX: only reads
// [esp+4] and ECX, writes [ecx+44h]; the callers below rely on that, so `this` is unchanged after it).
// ABI: every original __thiscall entry is the D1 form extern "C" __fastcall(Self*, void* unused_edx, ...).
#include "3a413dff74-gui_objects.h"
#include <cstddef>
#include <cstring>

using gui_load::Word;

namespace {
// Typed view of the shared Label/Button table slot +9C (D24 view, same slot type in 0073E5B8 and 0073E658:
// gui_vtables.cpp GuiLoadLabelMethods::s9c / GuiLoadButtonMethods::s9c).
struct InitializeSlotView {
    const void* prefix[0x27];
    void (__fastcall* s9c)(gui_load::Object*, void*, Word, Word, Word, Word);
};
static_assert(offsetof(InitializeSlotView, s9c) == 0x9C, "slot +9C");
const InitializeSlotView* initialize_view(const gui_load::Object* self) {
    return reinterpret_cast<const InitializeSlotView*>(self->vtbl);
}
Word address_word(const void* p, unsigned displacement) {
    return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p) + displacement);
}
// CAurGUIString vtable slot +0C (colour), called by 00414E41 as a tail JMP with one stack word (RET 4).
using AurSetColor = void (__fastcall*)(gui_load::AurString*, void*, const float*);
}

extern "C" {
// Original .data cell 0078D3B4 (Vector 0.0, 0.33, 0.49; PE-initialised, never written: only readers are
// 004184D2, 00418D22, 00418DD3 per a full-image scan). Single owner here; constant-initialised (D57).
extern const float kotor_gui_disabled_color_0078d3b4[3] = {0.0f, 0x1.51eb86p-2f, 0x1.f5c29p-2f};

void __fastcall gui_load_text_params_set_color_00414e10(gui_load::TextParams* self, void*, const float* color);
void __fastcall gui_load_text_params_restore_default_color_00414e80(gui_load::TextParams* self, void*);
// gui-render aur_gui_string.cpp: CSWGuiText::Initialize 00417310 (identical declaration to gui_pcbg.cpp:43).
void __fastcall kotor_fastcall_00417310(void* text, void*, const void* extent, void* text_params,
                                        Word scale_bits) noexcept(false);

// 00414E10 CSWGuiTextParams::SetColor(Vector const*) RET 4.
__declspec(noinline) void __fastcall gui_load_text_params_set_color_00414e10(gui_load::TextParams* self, void*,
                                                                              const float* color) {
    Word words[3];
    std::memcpy(words, color, sizeof(words));                                    // 00414E16/24/2A
    std::memcpy(self->color, words, sizeof(words));                              // 00414E22/27/2E (+1C..+24)
    gui_load::TextObject* const text = self->text_object;                        // 00414E1B [ecx+50h]
    if (text == nullptr) return;                                                 // 00414E1E/32
    gui_load::AurString* const string = text->string;                            // 00414E34 [ecx+14h]
    if (string == nullptr) return;                                               // 00414E37/39
    AurSetColor slot;                                                            // 00414E3B [edx+0Ch]
    std::memcpy(&slot, reinterpret_cast<const unsigned char*>(string->vtbl) + 0x0C, sizeof(slot));
    slot(string, nullptr, self->color);                                          // 00414E3D..41 tail JMP, arg = this+1C
}

// 00414E80 CSWGuiTextParams::RestoreDefaultColor(): SetColor(this+2Ch).
__declspec(noinline) void __fastcall gui_load_text_params_restore_default_color_00414e80(gui_load::TextParams* self, void*) {
    gui_load_text_params_set_color_00414e10(self, nullptr, self->default_color); // 00414E80..84
}

// 00418CD0 CSWGuiLabel::Initialize(extent, CSWGuiLabel const* source, float scale) RET 0Ch:
// this->vtbl[+9C](extent, source+E8h (TextParams), source+70h (BorderParams), scale).
__declspec(noinline) void __fastcall gui_load_label_initialize_00418cd0(gui_load::Object* self, void*, Word extent,
                                                                        Word source, Word scale_bits) {
    const void* const label = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(source));
    initialize_view(self)->s9c(self, nullptr, extent, address_word(label, 0xE8), address_word(label, 0x70),
                               scale_bits);                                      // 00418CD4..00418CEB
}

// 00418D80 CSWGuiButton::Initialize(extent, CSWGuiButton const* source) RET 8:
// this->vtbl[+9C](extent, source+16Ch (TextParams), source+80h, source+F4h (BorderParams x2)).
__declspec(noinline) void __fastcall gui_load_button_initialize_00418d80(gui_load::Object* self, void*, Word extent,
                                                                         Word source) {
    const void* const button = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(source));
    initialize_view(self)->s9c(self, nullptr, extent, address_word(button, 0x16C), address_word(button, 0x80),
                               address_word(button, 0xF4));                      // 00418D84..00418DA0
}

// 00418D00 CSWGuiLabel::SetEnabled(int) RET 4.
__declspec(noinline) void __fastcall gui_load_label_set_enabled_00418d00(gui_load::Object* self, void*, Word enabled) {
    gui_load_control_set_enabled_004176a0(self, nullptr, enabled);               // 00418D05..06
    gui_load::TextParams* const params = reinterpret_cast<gui_load::TextParams*>(
        reinterpret_cast<unsigned char*>(self) + 0xE8);                          // 00418D10 / 00418D26
    if (enabled != 0) {                                                          // 00418D0B..0E
        gui_load_text_params_restore_default_color_00414e80(params, nullptr);    // 00418D16
        return;
    }
    gui_load_text_params_set_color_00414e10(params, nullptr, kotor_gui_disabled_color_0078d3b4); // 00418D1E..2C tail
}

// 00418DB0 CSWGuiButton::SetEnabled(int) RET 4.
__declspec(noinline) void __fastcall gui_load_button_set_enabled_00418db0(gui_load::Object* self, void*, Word enabled) {
    gui_load_control_set_enabled_004176a0(self, nullptr, enabled);               // 00418DB6..B9
    gui_load::TextParams* const params = reinterpret_cast<gui_load::TextParams*>(
        reinterpret_cast<unsigned char*>(self) + 0x16C);                         // 00418DC0
    if (enabled != 0) {                                                          // 00418DBE / 00418DC6
        gui_load_text_params_restore_default_color_00414e80(params, nullptr);    // 00418DC8
        return;
    }
    gui_load_text_params_set_color_00414e10(params, nullptr, kotor_gui_disabled_color_0078d3b4); // 00418DD2..D7
    self->vtbl->set_active(self, nullptr, 0);                                    // 00418DDC..E2 vtbl[+40](0)
}

// 00418D40 CSWGuiLabel::ReSetFont: JMP 00415DF0 (= JMP 00415D60 SetFontRename) with ECX = this+E8h.
__declspec(noinline) void __fastcall gui_load_label_reset_font_00418d40(gui_load::Object* self, void*) {
    gui_load_text_params_set_font_rename_00415d60(reinterpret_cast<gui_load::TextParams*>(
        reinterpret_cast<unsigned char*>(self) + 0xE8), nullptr);
}
// 00418DF0 CSWGuiButton::ReSetFont: same with ECX = this+16Ch.
__declspec(noinline) void __fastcall gui_load_button_reset_font_00418df0(gui_load::Object* self, void*) {
    gui_load_text_params_set_font_rename_00415d60(reinterpret_cast<gui_load::TextParams*>(
        reinterpret_cast<unsigned char*>(self) + 0x16C), nullptr);
}

// 00417970 CSWGuiButton::Initialize(extent*, TextParams*, BorderParams* border, BorderParams* hilight) RET 10h.
__declspec(noinline) void __fastcall gui_load_button_initialize_00417970(gui_load::Object* self, void*, Word extent_word,
                                                                         Word text_params, Word border_params,
                                                                         Word hilight_params) {
    unsigned char* const base = reinterpret_cast<unsigned char*>(self);
    const unsigned char* const extent = reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(extent_word));
    for (unsigned offset = 0; offset != 16; offset += 4) {                       // 00417970..00417995 four dwords -> +4
        Word word;
        std::memcpy(&word, extent + offset, 4);
        std::memcpy(base + 4 + offset, &word, 4);
    }
    gui_load::Border* const border = reinterpret_cast<gui_load::Border*>(base + 0x6C);   // EBX
    gui_load::Border* const hilight = reinterpret_cast<gui_load::Border*>(base + 0xE0);  // EBP
    const gui_load::Extent* const own = &self->extent;                           // EDI = this+4
    gui_load_border_initialize_00415190(border, nullptr, own,
        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(border_params)));    // 00417998..004179A3
    gui_load_border_initialize_00415190(hilight, nullptr, own,
        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(hilight_params)));   // 004179A8..004179B6
    gui_load::Extent l10{}, l20{}, l30{}, l40{}, l50{};  // D2: original locals are pure outputs of 00415240/0040A4A0
    (void)gui_load_border_inner_extent_00415240(border, nullptr, &l20);         // 004179BB..C2 (EAX ignored)
    (void)gui_load_border_inner_extent_00415240(hilight, nullptr, &l30);        // 004179C7..CE (EAX ignored)
    gui_load::Extent* const a = gui_load_border_inner_extent_00415240(hilight, nullptr, &l40); // 004179D3..DA
    gui_load::Extent* const c = gui_load_border_inner_extent_00415240(border, nullptr, &l50);  // 004179E0..E7
    (void)gui_load_extent_intersection_0040a4a0(&l10, c, a);                    // 004179EC..F2 (cdecl, ADD ESP,0Ch)
    kotor_fastcall_00417310(base + 0x154, nullptr, &l10,
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(text_params)), 0x3F800000u); // 004179F7..00417A0F (1.0f)
}
}
