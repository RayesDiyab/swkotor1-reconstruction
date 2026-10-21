#pragma once
// lane-config: CClientOptions (swkotor.exe GOG K1, base 0x400000)
//   0061DB60 CClientOptions::CClientOptions   0061DBE0 CClientOptions::LoadOptions
//   0061D1C0 CClientOptions::ApplySoundOptions 005ED8F0 CClientExoApp::SetTexturePack
// D1 form: __fastcall(self, unused EDX). Shared String/Ini ops from options_bindings.
#include "67a3cbeb44-options.h"
#include "369787b919-window_shared_cells.h"   // manager_007a39fc
#include <cstdint>
namespace client_options {
using options_video::U8;using options_video::U32;using options_video::I32;
using options_video::String;using options_video::Ini;
struct ClientOptionsBindings {
    void (__fastcall* set_defaults_0061da60)(void*,void*);              // CClientOptions::SetDefaultOptions
    const char* (__fastcall* c_str_005e5670)(String*,void*);             // CExoString::CStr
    float (__fastcall* as_float_005e5650)(String*,void*);                // CExoString::AsFLOAT (result in ST0)
    void (__fastcall* set_texture_pack_005ed8f0)(void*,void*,U8);        // CClientExoApp::SetTexturePack RET 4
    void (__fastcall* set_frame_buffer_0061d750)(void*,void*,I32);       // CClientOptions::SetFrameBuffer RET 4
    void (__fastcall* apply_sound_options_0061d1c0)(void*,void*);        // CClientOptions::ApplySoundOptions
    void (__fastcall* update_all_fonts_0040b420)(void*,void*);           // CSWGuiManager::UpdateAllFonts
    void* (__fastcall* get_party_table_004aee70)(void*,void*);           // CServerExoApp::GetPartyTable
    void (__fastcall* auto_level_up_npcs_00563b90)(void*,void*);         // CSWPartyTable::AutoLevelUpNPCs
    void (__cdecl* enable_shadows_0044ee20)();
    void (__cdecl* disable_shadows_0044f080)();
    void (__cdecl* unlock_soft_shadows_0044ef90)();                      // AurATIUnlockSoftShadowsForHotFix
    void (__cdecl* enable_soft_shadows_0044ee50)();
    void (__cdecl* disable_soft_shadows_0044ee60)();
    void (__cdecl* enable_grass_0044ee30)();
    void (__cdecl* disable_grass_0044ee40)();
    void (__cdecl* set_vsync_state_0044ee90)(unsigned char);             // pushed as 0/1 dword (0061EEF6/0061EEF9)
    void (__fastcall* sound_music_volume_005d5c50)(void*,void*,float,std::uint32_t); // CExoSound::SetMusicVolume RET 8
    void (__fastcall* sound_dialog_volume_005d5d50)(void*,void*,float);  // CExoSound::SetDialogVolume RET 4
    void (__fastcall* sound_effect_volume_005d5cd0)(void*,void*,float);  // CExoSound::SetSoundEffectVolume RET 4
    void* volatile* gui_manager_007a39f4;    // CSWGuiManager* cell
    void* const* sound_007a39ec;             // CExoSound* cell (lane-sound owner)
    float* camera_dps_007a2320;              // .data -1.0f
    float* camera_accel_007a2324;            // .data -1.0f
    float* camera_decel_007a2328;            // .data -1.0f
    const float* sentinel_00740cbc;          // .rdata -1.0f
    const float* volume_scale_0073dfec;      // .rdata 0.01f (0x3C23D70A)
};
extern ClientOptionsBindings& client_options_bindings;
void* __fastcall CClientOptions_0061db60(void* self,void* unused_edx);            // RET, returns this
I32 __fastcall LoadOptions_0061dbe0(void* self,void* unused_edx);                 // RET, returns 1
void __fastcall ApplySoundOptions_0061d1c0(void* self,void* unused_edx);          // RET
void __fastcall ClientExoAppSetTexturePack_005ed8f0(void* app,void* unused_edx,U8 index); // RET 4
}
