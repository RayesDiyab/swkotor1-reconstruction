#pragma once
// lane-m1-movies (Milestone 1, L2): CSWGuiMainMenu::OnMoviesPicked post-guard body and the CSWGuiTitleMovies
// panel (open, list, Back), from the original swkotor.exe (GOG K1, image base 0x400000; static disassembly only).
//
// Conventions (DECISIONS D1/D21/D24/D47): original __thiscall entries are extern "C" __fastcall(Self*, void*
// unused_edx, original stack args...) with identical callee cleanup; event callbacks are __thiscall members of
// single-inheritance views of kotor_input_hover::GuiObject registered by derived-to-base PMF conversion (D47);
// the method table 00758130 is a typed constant struct; vptrs receive it through an object-pointer conversion.
#include "4a27cd265a-gui_objects.h"   // gui_load / gui_lb views, kotor_input_hover
#include <cstddef>
#include <cstdint>

namespace kotor_m1_movies {
// CExoArrayList<CExoString> {data, size, allocated} (12 bytes): CSWGuiTitleMovies+648..+653 (zeroed 006DD978..986).
struct StringArray {
    kotor_reconstruction::StartupString* data;   // +0 (006DD052, 006DD749)
    std::int32_t size;                           // +4
    std::int32_t allocated;                      // +8
};
static_assert(sizeof(StringArray) == 12, "CExoArrayList<CExoString>");

// CSWGuiTitleMovies (0x654 bytes: operator new(654h) at 0067B29A). Every member offset is established by the cited
// instruction of the constructor 006DD910; no other fields are claimed.
struct TitleMovies : gui_load::Panel {
    gui_lb::ListBox lb_movies;    // +064 "LB_MOVIES"  (CSWGuiListBox ctor 006DD94F, InitControl 006DD9C8)
    gui_load::Label lbl_title;    // +344 "LBL_TITLE"  (CSWGuiLabel ctor 006DD961, InitControl 006DD9F8)
    gui_load::Button btn_back;    // +484 "BTN_BACK"   (CSWGuiButton ctor 006DD973, InitControl 006DDA28)
    StringArray names;            // +648 movie names parallel to the list items (006DD40D Add, 006DD841 Insert)
};
static_assert(offsetof(TitleMovies, lb_movies) == 0x64 && offsetof(TitleMovies, lbl_title) == 0x344 &&
              offsetof(TitleMovies, btn_back) == 0x484 && offsetof(TitleMovies, names) == 0x648 &&
              sizeof(TitleMovies) == 0x654, "CSWGuiTitleMovies layout");
}

extern "C" {
// ---- title_movies.cpp
kotor_m1_movies::TitleMovies* __fastcall kotor_fastcall_006dd910(kotor_m1_movies::TitleMovies* self, void* unused_edx,
                                                                 gui_load::Object* manager) noexcept(false);   // ctor RET 4
void __fastcall kotor_fastcall_006dcfe0(kotor_m1_movies::TitleMovies* self, void* unused_edx);              // dtor RET
void* __fastcall kotor_fastcall_006ddaf0(kotor_m1_movies::TitleMovies* self, void* unused_edx, gui_load::Word flags); // deleting dtor RET 4
void __fastcall kotor_fastcall_006dce80(gui_load::Object* self, void* unused_edx, gui_load::I32 event,
                                        gui_load::I32 value);                                                // HandleInputEvent RET 8
void __fastcall kotor_fastcall_006dd100(kotor_m1_movies::TitleMovies* self, void* unused_edx) noexcept(false); // PopulateMovies RET
extern const struct KotorM1TitleMoviesMethods kotor_m1_title_movies_methods_00758130;
// ---- movies_client.cpp
void __fastcall kotor_fastcall_005ee120(void* client_app, void* unused_edx);                // CClientExoApp::BusyWaitMenuMusic RET
void __fastcall kotor_fastcall_005f4a60(void* client_internal, void* unused_edx);           // CClientExoAppInternal (unnamed) RET
gui_load::Word __fastcall kotor_fastcall_0061d3d0(void* client_options, void* unused_edx,
                                                  menu_resources::Ref* movie);              // CClientOptions::GetMovieShown RET 4
// ---- movies_empty_name_init.cpp (or the D69-interim movies_empty_name_storage.cpp; exactly one is staged)
// .data 008338D8: CExoString default-constructed by the original static initializer 0073B630 (xc slot 0078D178),
// destroyed by 0073C150; no other .text reference than 0061D40A (read-only use by GetMovieShown).
extern unsigned char kotor_cell_movie_name_008338d8[8];
}
