#pragma once
#include <cstdint>
namespace kotor_reconstruction { struct StartupString; }
extern "C" {
std::uint32_t __stdcall kotor_animation_sound_select_0060d430(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall kotor_animation_sound_item_refresh_00646880(void*, void*, kotor_reconstruction::StartupString*, kotor_reconstruction::StartupString*);
void __fastcall recovered_client_creature_play_sound_set_sound_00611470(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_00617470(void*, void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_animation_sound_get_soundset_0060b8a0(void*, void*, std::uint32_t, void*);
std::uint32_t __fastcall kotor_animation_sound_soundset_lookup_00678820(void*, void*, std::uint32_t, void*);
std::uint32_t __fastcall kotor_animation_sound_damage_effect_00616890(void*, void*);
std::uint32_t __fastcall kotor_animation_sound_animation_parry_0063c4f0(void*, void*, std::uint32_t);
}
