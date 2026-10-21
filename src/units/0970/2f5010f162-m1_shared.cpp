// lane-m1-options / L0-shared: bodies of the shared Milestone-1 GUI leaves (see m1_shared.h).
// Original: swkotor.exe GOG K1 (sha256 9c10e045...91435), static disassembly only.
#include "cfd9ad6200-m1_shared.h"
#include <cstring>

// External providers already linked by M49 (functional.map 20261007T145743537700Z-3f436fae):
extern "C" {
// m48t-f-warp-mainmenu-warp.obj: 0041A5A0 CSWGuiListBox::GetControl(int) (thiscall, RET 4).
gui_load::Control* __fastcall kotor_fastcall_0041a5a0(gui_lb::ListBox* self, void*, std::int32_t index);
// sound-path.obj (joined name, m48b_joins.cpp:23): 005F9AF0 CClientExoAppInternal::StartMenuMusic (RET 0Ch).
void __fastcall frame_R4_005f9af0(void*, void*, unsigned, unsigned, unsigned);
}

namespace {
using Word = std::uint32_t;
void put_word(void* p, unsigned off, Word v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
void* ptr_at(const void* p, unsigned off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }

// Panel method-table slot taking the one stack word (panel tables 0073E010 / 00752F70 / 00758838 ...: +50/+54/+58
// are typed void (__fastcall*)(gui_load::Object*, void*, gui_load::Word) in gui_vtables.cpp).
using PanelWordSlot = void (__fastcall*)(gui_load::Object*, void*, Word);
void panel_tail(kotor_input_hover::GuiObject* self_object, unsigned slot_offset, kotor_input_hover::Control* control) {
    gui_load::Object* self = reinterpret_cast<gui_load::Object*>(self_object);      // same object, ECX unchanged
    const PanelWordSlot* table = gui_lb::vt<PanelWordSlot>(self);                    // mov eax,[ecx]
    table[slot_offset / 4](self, nullptr,
                           static_cast<Word>(reinterpret_cast<std::uintptr_t>(control)));   // jmp [eax+slot] (same stack word)
}
}

namespace kotor_m1_shared {
// ---- 00624BA0: MOV EAX,[ECX]; JMP [EAX+50h] (CSWGuiPanel::OnAButtonPressed slot; 0040B640 in 0073E010).
void __thiscall PanelEventThunks::AcceptButtonCallback_00624ba0(kih::Control* control) {
    panel_tail(this, 0x50, control);                                                 // 00624BA0..A2
}
// ---- 00624BB0: MOV EAX,[ECX]; JMP [EAX+54h] (CSWGuiPanel::OnBButtonPressed slot; 0040B650 in 0073E010).
void __thiscall PanelEventThunks::OnBButtonPressed_00624bb0(kih::Control* control) {
    panel_tail(this, 0x54, control);                                                 // 00624BB0..B2
}
// ---- 00624BC0: MOV EAX,[ECX]; JMP [EAX+58h] (CSWGuiPanel::OnXButtonPressed slot; 0040B660 in 0073E010).
void __thiscall PanelEventThunks::OnXButtonPressed_00624bc0(kih::Control* control) {
    panel_tail(this, 0x58, control);                                                 // 00624BC0..C2
}
// ---- 00418E80: JMP 004189A0 (lane-input kotor_input_hover::Control::HandleMouseOver, RET 4).
void __thiscall ControlLeaves::HandleMouseOver_00418e80(int over) {
    HandleMouseOver(over);                                                           // 00418E80
}
}

extern "C" {
// ---- 0041A5D0 CSWGuiListBox::GetSelectedControl (thiscall, RET).
gui_load::Control* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(gui_lb::ListBox* self, void*) {
    return kotor_fastcall_0041a5a0(self, nullptr, static_cast<std::int32_t>(self->selected));   // 0041A5D0..D8 (MOVSX +2C6)
}
// ---- 005ED9B0 CClientExoApp::StartMenuMusic: ECX = [ECX+4]; JMP 005F9AF0 (stack words unchanged).
void __fastcall kotor_m1_client_start_menu_music_005ed9b0(void* client_app, void*, std::uint32_t a, std::uint32_t b,
                                                          std::uint32_t c) {
    frame_R4_005f9af0(ptr_at(client_app, 4), nullptr, a, b, c);                     // 005ED9B0..B3
}
// ---- 005ED9E0 CClientExoApp::SetStartMenuMusicOnMovieEnd(int) (RET 4): [[ECX+4]+1ECh] = value.
void __fastcall kotor_m1_client_set_start_menu_music_on_movie_end_005ed9e0(void* client_app, void*, std::uint32_t value) {
    put_word(ptr_at(client_app, 4), 0x1EC, value);                                   // 005ED9E0..E7
}
}
