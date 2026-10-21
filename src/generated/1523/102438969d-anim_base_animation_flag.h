// Original swkotor.exe animbase AddAnimationFlag (+C8) / RemoveAnimationFlag (+CC) slot bodies:
//   0069A3A0 CSWCAnimBase::AddAnimationFlag            0069A3C0 CSWCAnimBase::RemoveAnimationFlag
//   006996D0 CSWCAnimBaseHeadWield::AddAnimationFlag   00699730 CSWCAnimBaseHeadWield::RemoveAnimationFlag
//   00699A70 / 00699A80 vtordisp thunks (sub ecx,[ecx-4]; jmp 006996D0 / 00699730)
// All: thiscall (ECX = self, one stack word), RET 4, EAX 0/1. Typed exactly as
// KotorM15eAnimBaseVtbl::s0c8 / s0cc (m15e_animbase.h): U(__fastcall*)(void*, void*, U).
#pragma once
#include "2c14b89d43-m15e_animbase.h"
extern "C" {
kotor_m15e::U __fastcall anim_base_add_animation_flag_0069a3a0(void* self, void* edx, kotor_m15e::U flag);
kotor_m15e::U __fastcall anim_base_remove_animation_flag_0069a3c0(void* self, void* edx, kotor_m15e::U flag);
kotor_m15e::U __fastcall anim_base_head_wield_add_animation_flag_006996d0(void* self, void* edx, kotor_m15e::U flag);
kotor_m15e::U __fastcall anim_base_head_wield_remove_animation_flag_00699730(void* self, void* edx, kotor_m15e::U flag);
kotor_m15e::U __fastcall anim_base_head_wield_add_animation_flag_vtordisp_00699a70(void* self, void* edx, kotor_m15e::U flag);
kotor_m15e::U __fastcall anim_base_head_wield_remove_animation_flag_vtordisp_00699a80(void* self, void* edx, kotor_m15e::U flag);
}
