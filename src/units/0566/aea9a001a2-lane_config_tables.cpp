// lane-config: sole definitions of options_video::video_mode_bindings,
// client_intro::intro_bindings and client_options::client_options_bindings
// (D37 F3). Typed constant data in the D1 epoch: every slot is a function of the
// slot's own type (provider or exact-ABI typed adapter); no nullptr slot and no
// function-pointer cast. Runtime words that live in the WinMain/ReInit Views
// object are attached by the game_prepare owner (F1, attach_video_mode_cells).
// Providers not yet defined by any lane are declared once in
// lane_config_required (link-visible gaps, named owners in PACKET.json).
#include "b01b98864e-video_mode_settings.h"
#include "ef67052670-intro_queue.h"
#include "34dc360361-client_options_load.h"
#include "2e9f118037-movie_list.h"
#include "c534281817-startup_string.h"
#include "c7e191db10-native_strings.h"     // M40 window-native-strings.obj: WindowCStr005e5670
#include "356892d50c-gui_size.h"           // M40 service-gui_0040BE70: gui_size::Gui::set_size_0040be70
#include "b31d883255-growth.h"             // M40 viewport-list-growth.obj: menu_list_growth::allocate_005e03c0
#include "7b8c81fcff-allocator_live.h"     // M40 live_operator_new (006FA7E6 owner)
#include "9ea344dff0-d1_bodies.h"          // lane-menu-tables: providers.h Options, cells, menu_tables_required
#include <cstdint>
#include <new>

// ---- existing owners: declarations repeated exactly as their owners declare them
extern "C" void* kotor_cell_sound_007a39ec;                                   // lane-sound (D36)
extern "C" std::uint32_t __fastcall kotor_fastcall_005df510(void*,void*);     // lane-routes window_leaves.cpp (GetActive)
extern "C" void __fastcall kotor_fastcall_0061f9c0(void*,void*,std::uint32_t);// lane-routes window_leaves.cpp (HideMouse)
extern "C" void __cdecl kotor_cdecl_00403de0();                               // lane-routes (RestoreResolutionFromMovie)
extern "C" void __cdecl kotor_cdecl_00403cf0();                               // ChangeResolutionForMovie (same declaration as existing consumers)
extern "C" void __fastcall kotor_fastcall_0070c210(void*,void*);              // CExoMoviePlayer::ShowFadePanel (same declaration as existing consumers)
extern "C" void __fastcall kotor_fastcall_0070c220(void*,void*);              // CExoMoviePlayer::HideFadePanel
extern "C" void __fastcall kotor_fastcall_005df4e0(void*,void*);              // lane-prepare client_callees.cpp (ClearEvents)
extern "C" void __fastcall kotor_fastcall_005df540(void*,void*,int);          // lane-prepare client_callees.cpp (SetActive)
extern "C" void __fastcall kotor_fastcall_005f2980(void*,void*);              // lane-prepare client_callees.cpp (TurnOffKeyboardModal)
extern "C" void __cdecl kotor_window_reinit_aurora_00403800(std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t); // lane-window reinit_adapter.cpp (D46)
namespace kotor_original { void* __fastcall kotor_fastcall_0067a710(void* self,void* edx,void* gui_manager); } // lane-prepare load_screen.cpp
namespace kotor_game_prepare {                                                // lane-r4-lowers
void __fastcall SetGlobalUnpauseableState0062b410(void* self,void* edx,std::uint32_t);
void __fastcall ShowMouse0061fa00(void* self,void* edx,int);
}
namespace table29_required { void __fastcall gui_sound_mode_005d5e80(void* outer,void*,std::uint32_t mode); } // lane-sound donor
namespace kotor_sound_path { float __fastcall StringAsFloat005e5650(kotor_reconstruction::StartupString*,void*); } // lane-sound audio_retyped.cpp
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }   // lane-input input_cells.cpp
namespace gui_load { struct MainMenu; }
extern "C" void __fastcall gui_load_mainmenu_load_layout_0067ace0(gui_load::MainMenu*,void*,std::uint32_t); // lane-gui-load

// ---- REQUIRED: no defining lane yet (D1 form, exact slot types) -------------
namespace lane_config_required {
using options_video::I32;using options_video::Client;
void __fastcall PlayMoviesAsync_0070c200(void*,void*,void*,std::uint32_t,void*); // RET 0Ch (movies enabled only)
I32 __fastcall WaitForMovie_005f6f20(Client*,void*,I32);                     // RET 4 (movies enabled only)
I32 __fastcall PanelExists_0040bd70(void*,void*,void*);                      // RET 4 (gui!=null only)
void __fastcall InGameReset_0062f5f0(void*,void*);                           // (in-game GUI only)
void __fastcall SetFrameBuffer_0061d750(void*,void*,I32);                    // RET 4 ("Frame Buffer" key only)
void* __fastcall GetPartyTable_004aee70(void*,void*);                        // ("Auto Level Up NPCs"=1 only)
void __fastcall AutoLevelUpNPCs_00563b90(void*,void*);
void __cdecl EnableShadows_0044ee20();  void __cdecl DisableShadows_0044f080();
void __cdecl UnlockSoftShadows_0044ef90();
void __cdecl EnableSoftShadows_0044ee50(); void __cdecl DisableSoftShadows_0044ee60();
void __cdecl EnableGrass_0044ee30();    void __cdecl DisableGrass_0044ee40();
void __fastcall SoundMusicVolume_005d5c50(void*,void*,float,std::uint32_t);  // RET 8 (reached: ApplySoundOptions)
void __fastcall SoundDialogVolume_005d5d50(void*,void*,float);               // RET 4 (reached)
void __fastcall SoundEffectVolume_005d5cd0(void*,void*,float);               // RET 4 (reached)
}

namespace {
using options_video::String;using options_video::I32;using options_video::U32;using options_video::U8;
using Canonical=kotor_reconstruction::StartupString;
Canonical* C(String* s){return std::launder(static_cast<Canonical*>(static_cast<void*>(s)));}
// Typed adapters (object-pointer conversions only).
void __fastcall GuiSetSize_0040be70(void* gui,void*,I32 width,I32 height){static_cast<gui_size::Gui*>(gui)->set_size_0040be70(width,height);}
void __fastcall MainMenuLoadLayout_0067ace0(void* menu,void*,void* gui){
    gui_load_mainmenu_load_layout_0067ace0(static_cast<gui_load::MainMenu*>(menu),nullptr,static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(gui))); // 005F19FD..005F1A0A push gui
}
void __fastcall IntListAllocate_005e03c0(void* list,void*,U32 size){menu_list_growth::allocate_005e03c0(static_cast<menu_list_growth::List*>(list),nullptr,size);}
void __fastcall SetDefaults_0061da60(void* options,void*){static_cast<menu_providers_40_49::Options*>(options)->defaults_0061da60();}
const char* __fastcall CStr_005e5670(String* s,void*){return WindowCStr005e5670(C(s),nullptr);}
float __fastcall AsFloat_005e5650(String* s,void*){return kotor_sound_path::StringAsFloat005e5650(C(s),nullptr);}
// Pinned PE literals.
const char width_key_00749ff8[]="Width";const char height_key_00747580[]="Height";
const char high_frequency_key_0074e818[]="AllowHighMonitorFrequency";const char refresh_key_0074e80c[]="RefreshRate";
const char probe_movie_0074ecd8[]=".\\movies\\55.bik";const char leclogo_0074ecf8[]="leclogo";
const char biologo_0074ecf0[]="biologo";const char legal_0074ece8[]="legal";
const float sentinel_00740cbc=-1.0f;    // .rdata 0xBF800000
const float volume_scale_0073dfec=0.01f;// .rdata 0x3C23D70A
static_assert(sizeof(float)==4,"x86 float");

options_video::VideoModeBindings video_mode_storage={
    width_key_00749ff8,height_key_00747580,high_frequency_key_0074e818,refresh_key_0074e80c,
    nullptr,                                        // width_0078d1d4: runtime attach (WinMain Views)
    &kotor_menu_window_cells::height_0078d1d8,      // canonical U32 volatile cell
    nullptr,                                        // refresh_0078d1dc: runtime attach (ReInit Views mode_78d1dc)
    nullptr,                                        // depth_0078d1e0: runtime attach (WinMain Views)
    &kotor_window_reinit_aurora_00403800,
    &GuiSetSize_0040be70,&lane_config_required::PanelExists_0040bd70,&MainMenuLoadLayout_0067ace0,
    &live_operator_new,&kotor_original::kotor_fastcall_0067a710,&lane_config_required::InGameReset_0062f5f0};
client_intro::IntroBindings intro_storage={
    probe_movie_0074ecd8,leclogo_0074ecf8,biologo_0074ecf0,legal_0074ece8,
    &client_intro::CountStrings_004b2640,&client_movies::AddMovieToExoArrayList_005fbbf0,&IntListAllocate_005e03c0,
    &kotor_fastcall_0061f9c0,&kotor_game_prepare::ShowMouse0061fa00,&kotor_game_prepare::SetGlobalUnpauseableState0062b410,
    &kotor_fastcall_005df510,&kotor_fastcall_005df540,&kotor_fastcall_005df4e0,&table29_required::gui_sound_mode_005d5e80,
    &kotor_cdecl_00403cf0,&kotor_cdecl_00403de0,&kotor_fastcall_0070c210,&lane_config_required::PlayMoviesAsync_0070c200,
    &kotor_fastcall_0070c220,&lane_config_required::WaitForMovie_005f6f20,&kotor_fastcall_005f2980};
client_options::ClientOptionsBindings client_options_storage={
    &SetDefaults_0061da60,&CStr_005e5670,&AsFloat_005e5650,&client_options::ClientExoAppSetTexturePack_005ed8f0,
    &lane_config_required::SetFrameBuffer_0061d750,&client_options::ApplySoundOptions_0061d1c0,
    &menu_tables_required::UpdateAllFonts_0040b420,&lane_config_required::GetPartyTable_004aee70,&lane_config_required::AutoLevelUpNPCs_00563b90,
    &lane_config_required::EnableShadows_0044ee20,&lane_config_required::DisableShadows_0044f080,&lane_config_required::UnlockSoftShadows_0044ef90,
    &lane_config_required::EnableSoftShadows_0044ee50,&lane_config_required::DisableSoftShadows_0044ee60,
    &lane_config_required::EnableGrass_0044ee30,&lane_config_required::DisableGrass_0044ee40,
    &menu_tables_required::SetVSyncState_0044ee90,
    &lane_config_required::SoundMusicVolume_005d5c50,&lane_config_required::SoundDialogVolume_005d5d50,&lane_config_required::SoundEffectVolume_005d5cd0,
    &kotor_input_cells::gui_manager_007a39f4,&kotor_cell_sound_007a39ec,
    &menu_tables_m41::cells::keyboard_camera_007a2320,&menu_tables_m41::cells::keyboard_camera_007a2324,&menu_tables_m41::cells::keyboard_camera_007a2328,
    &sentinel_00740cbc,&volume_scale_0073dfec};
}
namespace options_video {
VideoModeBindings& video_mode_bindings=video_mode_storage;
bool attach_video_mode_cells(U32* width_78d1d4,U32* mode_78d1dc,U8* depth_78d1e0) noexcept {
    if(!width_78d1d4||!mode_78d1dc||!depth_78d1e0)return false;
    auto& v=video_mode_storage;
    if((v.width_0078d1d4&&v.width_0078d1d4!=width_78d1d4)||(v.refresh_0078d1dc&&v.refresh_0078d1dc!=mode_78d1dc)||
       (v.depth_0078d1e0&&v.depth_0078d1e0!=depth_78d1e0))return false;
    v.width_0078d1d4=width_78d1d4;v.refresh_0078d1dc=mode_78d1dc;v.depth_0078d1e0=depth_78d1e0;return true;
}
void detach_video_mode_cells() noexcept {
    video_mode_storage.width_0078d1d4=nullptr;video_mode_storage.refresh_0078d1dc=nullptr;video_mode_storage.depth_0078d1e0=nullptr;
}
}
namespace client_intro { IntroBindings& intro_bindings=intro_storage; }
namespace client_options { ClientOptionsBindings& client_options_bindings=client_options_storage; }
