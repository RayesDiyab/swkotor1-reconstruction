// Original eight-byte vtordisp thunks: SUB ECX,[ECX-4]; JMP real family provider.
#include "d14b72eb41-model_family_b.h"
#include "628decf93e-m15e_animbase_internal.h"
using namespace kotor_m15e_ab;
extern "C" {
U __fastcall recovered_anim_wield_internal_0069a3e0(void*,void*,U,U,U,U,U);
kotor_m15e::U __fastcall model_family_b_0069aa50(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2, kotor_m15e::U a3, kotor_m15e::U a4) { return recovered_anim_wield_internal_0069a3e0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2, a3, a4); }
void* __fastcall model_family_b_0069afc0(void* self, void*, kotor_m15e::U a0) { return kotor_m15e_anim_base_wield_get_model_0069aa90(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
kotor_m15e::U __fastcall model_family_b_0069afd0(void* self, void*, kotor_m15e::U a0, void* a1, kotor_m15e::U a2, kotor_m15e::U a3) { return kotor_m15e_anim_base_wield_wield_0069ab80(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2, a3); }
kotor_m15e::U __fastcall model_family_b_0069b020(void* self, void*, void* a0, kotor_m15e::U a1) { return kotor_m15e_anim_base_wield_load_model_0069afe0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1); }
void* __fastcall model_family_b_0069ba30(void* self, void*, kotor_m15e::U a0) { return model_family_b_0069bc80(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
void* __fastcall model_family_b_0069ba40(void* self, void*, kotor_m15e::U a0) { return kotor_m15e_anim_base_head_get_model_0069b060(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
kotor_m15e::U __fastcall model_family_b_0069ba50(void* self, void*, void* a0, kotor_m15e::U a1) { return kotor_m15e_anim_base_head_load_model_0069b0a0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1); }
kotor_m15e::U __fastcall model_family_b_0069ba60(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2, kotor_m15e::U a3, kotor_m15e::U a4, kotor_m15e::U a5) { return model_family_b_0069b410(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2, a3, a4, a5); }
kotor_m15e::U __fastcall model_family_b_0069ba70(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2, kotor_m15e::U a3, kotor_m15e::U a4, kotor_m15e::U a5, kotor_m15e::U a6) { return kotor_m15e_anim_base_head_replace_texture_0069b310(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2, a3, a4, a5, a6); }
kotor_m15e::U __fastcall model_family_b_0069ba80(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2) { return kotor_m15e_anim_base_head_replace_texture_0069b1c0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2); }
kotor_m15e::U __fastcall model_family_b_0069ba90(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2, kotor_m15e::U a3) { return kotor_m15e_anim_base_head_replace_texture_0069b250(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2, a3); }
kotor_m15e::U __fastcall model_family_b_0069baa0(void* self, void*, kotor_m15e::U a0) { return kotor_m15e_anim_base_head_restore_texture_0069b570(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
kotor_m15e::U __fastcall model_family_b_0069bab0(void* self, void*) { return model_family_b_0069b4f0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
kotor_m15e::U __fastcall model_family_b_0069bac0(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1) { return model_family_b_0069b870(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1); }
kotor_m15e::U __fastcall model_family_b_0069bad0(void* self, void*) { return model_family_b_0069b8e0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
kotor_m15e::U __fastcall model_family_b_0069bae0(void* self, void*, kotor_m15e::U a0) { return model_family_b_0069b910(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
kotor_m15e::U __fastcall model_family_b_0069baf0(void* self, void*, kotor_m15e::U a0) { return kotor_m15e_anim_base_head_unload_model_0069b650(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
kotor_m15e::U __fastcall model_family_b_0069bb10(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2, void* a3) { return kotor_m15e_anim_base_head_set_env_map_0069b720(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2, a3); }
kotor_m15e::U __fastcall model_family_b_0069bb20(void* self, void*, kotor_m15e::U a0, kotor_m15e::U a1, kotor_m15e::U a2) { return model_family_b_0069b6a0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0, a1, a2); }
kotor_m15e::U __fastcall model_family_b_0069bb30(void* self, void*) { return model_family_b_0069b7c0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
void __fastcall model_family_b_0069bb40(void* self, void*) { kotor_m15e_anim_base_head_turn_on_shadows_0069b810(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
void __fastcall model_family_b_0069bb50(void* self, void*) { model_family_b_0069b840(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
kotor_m15e::U __fastcall model_family_b_0069bb60(void* self, void*) { return kotor_m15e_anim_base_head_enable_distortion_0069b950(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
kotor_m15e::U __fastcall model_family_b_0069bb70(void* self, void*) { return kotor_m15e_anim_base_head_disable_distortion_0069b9c0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr); }
void __fastcall model_family_b_0069bc70(void* self, void*, kotor_m15e::U a0) { model_family_b_00699ad0(reinterpret_cast<void*>(wd(self)-ld<U>(self,-4)), nullptr, a0); }
}
