#pragma once
// lane-m1-options / L0-shared (Milestone 1): single-owner shared GUI leaves consumed by L1 options,
// L2 movies and L3 save/load. Authored from the original swkotor.exe (GOG K1, image base 0x400000);
// every body cites its instruction addresses (static disassembly only, nothing executed).
//
// Conventions (DECISIONS D1/D21/D24/D47):
//  * original __thiscall entries with a stack argument used as CSWGuiControl event callbacks are
//    __thiscall members of single-inheritance views of kotor_input_hover::GuiObject / ::Control, so their
//    PMFs are one code word; registration converts them with static_cast to the base PMF (D47);
//  * other __thiscall entries are extern "C" __fastcall(Self*, void* unused_edx, ...) veneers with the
//    original stack arguments and callee cleanup (D1);
//  * virtual calls read the receiver's vptr at the call site through a typed slot view (D24 object view).
//
// 00411240 CResGFF::GetTopLevelStruct is NOT authored here: M49 already links it as the addressless member
// kotor_q05::CResGFF::GetTopLevelStruct(StructHandle*) (gff-core/gff_views.cpp:78, unit gff-views,
// ?GetTopLevelStruct@CResGFF@kotor_q05@@QAEIPAUStructHandle@2@@Z), byte-checked below; consumers include
// "gff_views.h" (header root gff-core) and call it.
//   00411240 mov eax,[esp+4]; test eax,eax; jne 0041124B; ret 4   -> returns 0 (the null argument)
//   0041124B mov dword ptr [eax],0; mov eax,1; ret 4               -> handle->index = 0, returns 1
#include "3370aef598-gui_objects.h"   // gui_load (Object/Control/Panel views, EventFunc), gui_lb::ListBox, kotor_input_hover
#include <cstdint>

namespace kotor_m1_shared {
namespace kih = kotor_input_hover;

// CSWGuiPanel event-callback thunks (MSVC vcall thunks for &CSWGuiPanel::On?ButtonPressed): ECX = the
// registering panel, one stack word (the activating CSWGuiControl*), tail-jump through the panel's vptr with
// the same stack word, so the slot's own RET 4 cleans it.
struct PanelEventThunks : kih::GuiObject {
    void __thiscall AcceptButtonCallback_00624ba0(kih::Control* control);   // 00624BA0 mov eax,[ecx]; jmp [eax+50h]
    void __thiscall OnBButtonPressed_00624bb0(kih::Control* control);       // 00624BB0 mov eax,[ecx]; jmp [eax+54h]
    void __thiscall OnXButtonPressed_00624bc0(kih::Control* control);       // 00624BC0 mov eax,[ecx]; jmp [eax+58h]
};
// Registration helper: derived-to-base PMF conversion (D47), never a function-pointer cast.
inline gui_load::EventFunc event_func(void (__thiscall PanelEventThunks::*f)(kih::Control*)) {
    return static_cast<gui_load::EventFunc>(f);
}

// 00418E80 (Ghidra: CSWGuiControl::HandleMouseOver; a second entry distinct from 004189A0): JMP 004189A0 with
// ECX and the stack word unchanged (thiscall, RET 4 performed by 004189A0). Referenced only from .rdata method
// tables 007566E4 / 00756904 / 00756BC4 / 0075774C (slot +0C); typed for PMF slots of type
// void (kih::Control::*)(int) via control_mouse_over_slot().
struct ControlLeaves : kih::Control {
    void __thiscall HandleMouseOver_00418e80(int over);
};
inline void (kih::Control::*control_mouse_over_slot())(int) {
    return static_cast<void (kih::Control::*)(int)>(&ControlLeaves::HandleMouseOver_00418e80);
}
}

extern "C" {
// 0041A5D0 CSWGuiListBox::GetSelectedControl() (thiscall, RET): MOVSX EAX,[ECX+2C6h]; PUSH EAX;
// CALL 0041A5A0 GetControl(index) (EAX passes through).
gui_load::Control* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(gui_lb::ListBox* self, void* unused_edx);

// 005ED9B0 CClientExoApp::StartMenuMusic(a, b, c): MOV ECX,[ECX+4]; JMP 005F9AF0
// (CClientExoAppInternal::StartMenuMusic, RET 0Ch; 3 stack words unchanged). Callers pass (1, 7Fh, 2).
void __fastcall kotor_m1_client_start_menu_music_005ed9b0(void* client_app, void* unused_edx,
                                                          std::uint32_t a, std::uint32_t b, std::uint32_t c);

// 005ED9E0 CClientExoApp::SetStartMenuMusicOnMovieEnd(int value) (thiscall, RET 4):
// MOV EAX,[ECX+4]; MOV ECX,[ESP+4]; MOV [EAX+1ECh],ECX; RET 4.
void __fastcall kotor_m1_client_set_start_menu_music_on_movie_end_005ed9e0(void* client_app, void* unused_edx,
                                                                           std::uint32_t value);
}
