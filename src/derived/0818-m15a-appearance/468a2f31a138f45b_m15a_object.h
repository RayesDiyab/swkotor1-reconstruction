#pragma once
// lane-m15-a unit m15a-object (m15a_object.cpp): CSWCObject members, CSWCItem powered animations,
// SwitchWeaponEvent. Conventions: see m15a_common.h (D1 extern "C" __fastcall veneers of __thiscall entries,
// RET N noted per entry, void* object pointers). Parameter counts are final (from the original RET N).
#include "02417a06c1bef2bc_m15a_common.h"
#include <cstdint>

extern "C" {
// ---- CSWCObject (ECX = CSWCObject*; for a CSWCItem the embedded object is item+10h)
// 0063C060 GetCurrentAnimation() RET: AX = [this+48h] (word). Consumer name in f-input process_input.h:
// kotor_input_required::get_current_animation_0063c060.
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void* self, void* unused_edx);
// 0063C0A0 ReplaceTexture(BYTE part, const CResRef* old_tex, const CResRef* new_tex, int keep) RET 10h ->
// anim base [this+68h] slot +1Ch result, or 0 without anim base.
std::uint32_t __fastcall kotor_m15a_object_replace_texture_0063c0a0(void* self, void* unused_edx, std::uint32_t part,
    const void* old_tex, const void* new_tex, std::uint32_t keep) noexcept(false);
// 0063C0C0 ReplaceTexture(const CResRef*, const CResRef*, int) RET 0Ch -> anim base slot +18h result, or 0.
std::uint32_t __fastcall kotor_m15a_object_replace_texture_0063c0c0(void* self, void* unused_edx, const void* old_tex,
    const void* new_tex, std::uint32_t keep) noexcept(false);
// 0063C100 GetLoopingAnimation() RET: AX = [this+58h].
std::uint16_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void* self, void* unused_edx);
// 0063C170 AnimationStationary(WORD anim) RET 4 -> animations.2da stationary column != 0 (1 when no entry).
std::uint32_t __fastcall kotor_m15a_object_animation_stationary_0063c170(void* self, void* unused_edx,
    std::uint32_t animation) noexcept(false);
// 0063C300 AnimationLooping(WORD anim) RET 4 -> 1 for creature ranges / animations.2da looping column != 0.
std::uint32_t __fastcall kotor_m15a_object_animation_looping_0063c300(void* self, void* unused_edx,
    std::uint32_t animation) noexcept(false);
// 0063CAD0 AbortFireForgetAnimation(int keep_looping) RET 4. Consumer name:
// kotor_input_required::abort_fire_forget_animation_0063cad0.
void __fastcall kotor_m15a_object_abort_fire_forget_animation_0063cad0(void* self, void* unused_edx,
    std::uint32_t keep) noexcept(false);
// 0063CE40 RestoreTexture(BYTE part) RET 4 -> anim base slot +20h result, or 0.
std::uint32_t __fastcall kotor_m15a_object_restore_texture_0063ce40(void* self, void* unused_edx,
    std::uint32_t part) noexcept(false);
// 0063D4B0 GetServerObject() RET -> cached [this+F8h] / server object of this client object, or 0.
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* self, void* unused_edx) noexcept(false);
// 0063D540 EnableDistortion() / 0063D550 DisableDistortion() RET (tail jump through anim base +E4h / +E8h).
void __fastcall kotor_m15a_object_enable_distortion_0063d540(void* self, void* unused_edx) noexcept(false);
void __fastcall kotor_m15a_object_disable_distortion_0063d550(void* self, void* unused_edx) noexcept(false);
// 0063D830 SetLoopingAnimation(WORD anim) RET 4: [this+58h] = anim when AnimationLooping(anim) == 1.
void __fastcall kotor_m15a_object_set_looping_animation_0063d830(void* self, void* unused_edx,
    std::uint32_t animation) noexcept(false);
// 0063E530 CSWCObject::CSWCObject(BYTE object_type) RET 4, EAX = this; stores
// &kotor_m15e_object_methods_007516c8 (L15-E).
void* __fastcall kotor_m15a_object_construct_0063e530(void* self, void* unused_edx,
    std::uint32_t object_type) noexcept(false);

// ---- CSWCItem (ECX = CSWCItem*)
// 00646600 SetEquippedBy(CSWCCreature* creature) RET 4: [this+160h] = creature ? creature->id : 7F000000h.
void __fastcall kotor_m15a_item_set_equipped_by_00646600(void* self, void* unused_edx, void* creature);
// 00646BA0 ResolveCreaturePoweredAnimations(int powered) RET 4.
void __fastcall kotor_m15a_item_resolve_creature_powered_animations_00646ba0(void* self, void* unused_edx,
    std::uint32_t powered) noexcept(false);
// 00647130 PowerItem(int powered, int force, int silent, int instant) RET 10h.
void __fastcall kotor_m15a_item_power_item_00647130(void* self, void* unused_edx, std::uint32_t powered,
    std::uint32_t force, std::uint32_t silent, std::uint32_t instant) noexcept(false);

// ---- free function
// 00610F40 SwitchWeaponEvent(void* unused, const char* event, CSWCCreature* creature) __cdecl (RET; callers
// 006980B7 / 006989E3 push 0, "AppearanceForceUpdate", creature and clean 0Ch).
void __cdecl kotor_m15a_switch_weapon_event_00610f40(void* unused, const char* event, void* creature) noexcept(false);
}
