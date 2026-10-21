#pragma once
// lane-m15-a unit m15a-creature (m15a_creature.cpp): CSWCCreature members, CSWCCombatAttackData,
// CSWCSpellCastVisual, Yaw/Pitch. Conventions: m15a_common.h (D1 fastcall veneers, RET N per entry).
#include "02417a06c1bef2bc_m15a_common.h"

extern "C" {
// 004AA0F0 Yaw(const Vector*) / 004AA130 Pitch(const Vector*): __cdecl, result in ST0 (degrees).
float __cdecl kotor_m15a_yaw_004aa0f0(const void* vector);
float __cdecl kotor_m15a_pitch_004aa130(const void* vector);
// 0060B630 CSWCCombatAttackData::CSWCCombatAttackData() RET 0, EAX = this (0x48 block).
void* __fastcall kotor_m15a_combat_attack_data_construct_0060b630(void* self, void*);
// 00695CE0 CSWCSpellCastVisual::CSWCSpellCastVisual() RET 0, EAX = this (0x30 block, zeroed).
void* __fastcall kotor_m15a_spell_cast_visual_construct_00695ce0(void* self, void*);
// 00616A20 CSWCCreature::CSWCCreature() RET 0, EAX = this (0x44C block).
void* __fastcall kotor_m15a_creature_construct_00616a20(void* self, void*) noexcept(false);
// 0060B7C0 SetSoundSet(WORD sound_set) RET 4 (low 16 bits used).
void __fastcall kotor_m15a_creature_set_sound_set_0060b7c0(void* self, void*, std::uint32_t sound_set) noexcept(false);
// 0060BAD0 UnloadMonkVisuals() RET 0.
void __fastcall kotor_m15a_creature_unload_monk_visuals_0060bad0(void* self, void*);
// 0060C8C0 SetCreatureType(BYTE type) RET 4 (low 8 bits used).
void __fastcall kotor_m15a_creature_set_creature_type_0060c8c0(void* self, void*, std::uint32_t type);
// 0060CA80 GetEquippedItemID(DWORD slot_mask) RET 4 -> object id (7F000000h when no slot matches).
std::uint32_t __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* self, void*, std::uint32_t slot);
// 0060CED0 GetLastInstant(DWORD slot_mask) RET 4 -> dword (0 when no slot matches).
std::uint32_t __fastcall kotor_m15a_creature_get_last_instant_0060ced0(void* self, void*, std::uint32_t slot);
// 0060D600 SetName(const CExoString& first, const CExoString& last) RET 8.
void __fastcall kotor_m15a_creature_set_name_0060d600(void* self, void*, const void* first, const void* last) noexcept(false);
// 0060E5B0 UpdateShadowBlob() RET 0.
void __fastcall kotor_m15a_creature_update_shadow_blob_0060e5b0(void* self, void*);
// 0060F7C0 CGPauseCycle(float delta_seconds, int restart) RET 8 (char-gen turntable idle animation).
void __fastcall kotor_m15a_creature_cg_pause_cycle_0060f7c0(void* self, void*, float delta_seconds,
                                                           std::uint32_t restart) noexcept(false);
// 0060FB20 GetServerCreature() RET 0 -> CSWSCreature* (server object vslot +30h) or 0.
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* self, void*);
// 006100F0 Setup3DScene(CSWGui3DSceneView* view, CSWCCreature* alignment_source (0 = this),
//   CSWSCreature* server_creature (0 = looked up), int play_pause) RET 10h.
void __fastcall kotor_m15a_creature_setup_3d_scene_006100f0(void* self, void*, void* scene_view, void* alignment_source,
                                                            void* server_creature, std::uint32_t play_pause) noexcept(false);
// 006105B0 LoadRollingFootstepSound() RET 0 -> 1 when a rolling sound source exists, else 0.
std::uint32_t __fastcall kotor_m15a_creature_load_rolling_footstep_sound_006105b0(void* self, void*) noexcept(false);
// 006134C0 SetAppearance(appearance block*, BYTE parts (bit0 body, bit1 head, bit2 tail, bit3 wing), int reattach)
//   RET 0Ch -> 0 when the body appearance could not be created, else 1.
std::uint32_t __fastcall kotor_m15a_creature_set_appearance_006134c0(void* self, void*, void* appearance_block,
                                                                     std::uint32_t parts, std::uint32_t reattach) noexcept(false);
// 00613710 UpdateWeaponClass() RET 0.
void __fastcall kotor_m15a_creature_update_weapon_class_00613710(void* self, void*);
}
