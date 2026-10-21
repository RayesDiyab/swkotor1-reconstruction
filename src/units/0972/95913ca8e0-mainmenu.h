#pragma once
// Lane gui-load: CSWGuiMainMenu construction / destruction / Draw, the remaining
// non-3D DisplayMainMenu callees (CSWGuiManager::AddPanel family,
// CClientExoAppInternal::SetInputClass) and CExoResMan::CleanDirectory.
// Consumer-required export names are used where another lane already calls them
// (lane-routes client_wrappers.cpp, lane-r4-lowers r4_lowers.h).
#include "9bac6f8338-gui_objects.h"
#include "a1d4253b55-resources.h"

namespace gui_load {
// MainMenu event handlers (CSWGuiControlEvent callbacks; RET 4 thiscall members).
// 0067B450/0067B470 are lane-input members; the others are declared on a
// single-inheritance derived view so their PMFs are one code word (MISSING bodies).
struct MainMenuHandlers : kotor_input_hover::MainMenu {
    void __thiscall OnNewGamePicked(kotor_input_hover::Control*);       // 0067AFB0
    void __thiscall OnLoadSaveGame(kotor_input_hover::Control*);        // 0067B1A0
    void __thiscall OnMoviesPicked(kotor_input_hover::Control*);        // 0067B250
    void __thiscall OnOptionsPicked(kotor_input_hover::Control*);       // 0067B2F0
    void __thiscall OnWarpButtonPressed(kotor_input_hover::Control*);   // 0067C4B0
    void __thiscall OnQuitButtonPressed(kotor_input_hover::Control*);   // 0067B4A0
    void __thiscall LaunchDashboard(kotor_input_hover::Control*);       // 0067B490 (RET 4; message-box callback)
};
}

extern "C" {
// ---- authored here
gui_load::MainMenu* __fastcall gui_load_mainmenu_construct_0067c4c0(gui_load::MainMenu*, void*, gui_load::Object* manager) noexcept(false);
void __fastcall gui_load_mainmenu_destruct_0067b500(gui_load::MainMenu*, void*);
void* __fastcall gui_load_mainmenu_deleting_destruct_0067b910(gui_load::MainMenu*, void*, gui_load::Word);
void __fastcall gui_load_mainmenu_draw_0067ac10(gui_load::Panel*, void*, gui_load::Word);
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, std::uint32_t, std::uint32_t) noexcept(false); // AddPanel (consumer name)
gui_load::Word __fastcall gui_load_manager_push_modal_0040bd90(void* manager, void*, gui_load::Panel*) noexcept(false);
void __fastcall kotor_fastcall_0040acc0(void* manager, void*) noexcept(false);   // DetermineNeedToDraw (consumer name)
void __fastcall kotor_fastcall_006200e0(void* internal, void*, std::uint32_t input_class, std::uint32_t show) noexcept(false); // SetInputClass
gui_load::Word __fastcall gui_load_resman_clean_directory_00409460(menu_resources::Manager*, void*, gui_load::String*,
    gui_load::Word, gui_load::Word) noexcept(false);
// do3DGUI .data cell 0078D1E4 (image value 1, never written): proposed owner this lane.
extern volatile std::uint32_t kotor_cell_do3dgui_0078d1e4;

// rev2: OnPanelAdded 0067B6C0 (frame-1) and CSWGuiMessageBox::SetCallback 00624A40.
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, kotor_input_hover::GuiObject* object,
    gui_load::EventFunc func, gui_load::Word argument);
// ---- OnPanelAdded callees
void* __fastcall frame_R4_005ed690(void* app, void*);                                  // lane-r4-lowers: CClientExoApp::GetInGameGui
void __fastcall kotor_fastcall_005ee180(void* outer, void*, std::uint32_t);           // lane-routes: CClientExoApp::ShowMouse
gui_load::Word __fastcall gui_load_resman_free_disk_space_004067b0(menu_resources::Manager*, void*, gui_load::String*,
    std::uint64_t*) noexcept(false);                                                  // MISSING (proposed)
gui_load::Word __fastcall gui_load_resman_create_directory_004068c0(menu_resources::Manager*, void*, gui_load::String*) noexcept(false); // MISSING
gui_load::Word __fastcall gui_load_resman_nuke_directory_00409480(menu_resources::Manager*, void*, gui_load::String*,
    gui_load::Word, gui_load::Word) noexcept(false);                                  // MISSING
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, gui_load::Word token, gui_load::String* value) noexcept(false); // MISSING CTlkTable::SetCustomToken
void __fastcall kotor_fastcall_00627130(void* box, void*, gui_load::Word allow);      // MISSING CSWGuiMessageBox::SetAllowCancel
extern void* kotor_cell_tlk_007a3a08;                                                 // lane-tlk (D23)
// ---- lane-r4-lowers (defined there)
std::uint32_t __fastcall kotor_fastcall_0040aba0(void* gui, void*, void* panel);    // GetPosInPanelList
unsigned __fastcall frame_FindPopup_0040ab70(void* gui, void*, void* panel);       // GetPosInModalStack
// ---- 3D lanes (not authored here)
void* __fastcall gui_scene_construct_004174b0(void* gui_scene, void*) noexcept(false);                // lane-3d-render
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* gui_scene, void*, gui_load::String*, std::int32_t) noexcept(false); // MISSING lane-3d-model (proposed)
// ---- MISSING (proposed names)
void __fastcall kotor_panel_draw_0040B760(gui_load::Panel*, void*, gui_load::Word);                 // lane-gui-render
gui_load::Word __fastcall gui_load_resman_remove_file_00406b20(menu_resources::Manager*, void*, gui_load::String*, gui_load::Word) noexcept(false);
gui_load::Word __fastcall gui_load_resman_wipe_directory_00408e90(menu_resources::Manager*, void*, gui_load::String*,
    gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word) noexcept(false);
void __fastcall kotor_fastcall_0059ea30(void*, void*);
void __fastcall kotor_fastcall_005df4e0(void* input, void*);                         // CExoInput::ClearEvents
void __fastcall frame_R4_0062aa00(void*, void*, unsigned, unsigned);                 // CGuiInGame::SetSWGuiStatus
void __fastcall kotor_fastcall_0061f9c0(void*, void*, std::uint32_t);                // HideMouse (lane-routes window_leaves.cpp)
void __fastcall kotor_fastcall_005f2c60(void* internal, void*, std::uint32_t, std::uint32_t); // IndicateObject
void __fastcall kotor_fastcall_00671670(void* mini_game, void*, void* camera);       // CSWMiniGame::SetCamera
void __fastcall kotor_fastcall_006714e0(void* mini_game, void*);                     // CSWMiniGame::Go
void* __fastcall kotor_fastcall_004b2820(void* server, void*);                       // CServerExoApp::GetPlayerCreature
std::uint32_t __fastcall kotor_fastcall_005eda50(void* internal, void*, std::uint32_t); // ServerToClientObjectId
void __fastcall kotor_fastcall_005f2bb0(void* internal, void*, std::uint32_t);       // SetPlayerCreature
void __fastcall kotor_fastcall_005f5cf0(void* internal, void*);                      // AttachPlayerGameObjectToArea
void* __fastcall kotor_fastcall_005f1420(void* internal, void*);                     // GetPlayerCreature (client)
void __fastcall kotor_fastcall_0060f090(void* creature, void*, std::uint32_t);       // CSWCCreature::SwitchMode
void __fastcall kotor_fastcall_006792e0(void* control, void*, std::uint32_t);        // CSWPlayerControl::SetEnabled
}
namespace kotor_menu_10_19 { struct Tlk; }
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk* self, void* unused_edx, kotor_reconstruction::StartupString* text); } // lane-tlk
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }               // M40 canonical-assignment-format.obj
namespace kotor_game_prepare { void __fastcall ShowMouse0061fa00(void* self, void* edx, int); }     // lane-r4-lowers adopted body
namespace table29_required { void __fastcall gui_sound_mode_005d5e80(void* outer, void* unused_edx, std::uint32_t mode); } // reviewed donor (lane-sound D25)
extern "C" void* kotor_cell_sound_007a39ec;                                          // lane-sound (D25)
namespace sound_mode29_required { extern const volatile std::uint32_t* const word_0074c5ec; }  // lane-sound view of .rdata 0074C5EC (D25)
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; }
