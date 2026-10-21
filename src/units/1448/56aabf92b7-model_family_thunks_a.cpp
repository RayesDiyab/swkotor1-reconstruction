#include "22c2b63b02-model_family_thunks_a.h"
#include <cstring>
static_assert(sizeof(void*)==4,"original x86 pointer ABI");

// Exact selected provider contracts from m15e_animbase_internal.h and m15e_tables.cpp.
extern "C" {
void __fastcall kotor_m15e_anim_base_wield_set_wield_limited_00699fc0(void*,void*,std::uint32_t) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_show_wielded_items_0069a010(void*,void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_turn_on_shadows_0069a170(void*,void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_hide_wielded_items_0069a6b0(void*,void*,void*,void*) noexcept(false);
std::uint32_t __fastcall kotor_m15e_anim_base_wield_enable_distortion_0069a8f0(void*,void*) noexcept(false);
std::uint32_t __fastcall kotor_m15e_anim_base_wield_disable_distortion_0069a980(void*,void*) noexcept(false);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*,void*);
void __cdecl live_free(void*);
struct KotorM15eSwAnimBaseVtbl;
extern const KotorM15eSwAnimBaseVtbl kotor_m15e_sw_anim_base_vtable_00754ae4;
}
namespace {
// The original SUB operates on a DWORD, wrapping modulo2^32, even for a negative vtordisp.
void* adjusted(void* self) {
    std::uint32_t displacement;
    std::memcpy(&displacement,static_cast<const unsigned char*>(self)-4,4);
    const auto address=static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self));
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(address-displacement));
}
}
extern "C" {
void* __fastcall model_sw_anim_delete_00698ea0(void* self,void*,std::uint32_t flags) noexcept(false) {
    const void* table=&kotor_m15e_sw_anim_base_vtable_00754ae4;
    std::memcpy(self,&table,sizeof table);
    if (flags & 1u) live_free(self);
    return self;
}
void __fastcall model_wield_show_thunk_0069a340(void* self,void*) noexcept(false) {
    kotor_m15e_anim_base_wield_show_wielded_items_0069a010(adjusted(self),nullptr);
}
void __fastcall model_wield_limit_thunk_0069a350(void* self,void*,std::uint32_t value) noexcept(false) {
    kotor_m15e_anim_base_wield_set_wield_limited_00699fc0(adjusted(self),nullptr,value);
}
void __fastcall model_wield_shadows_thunk_0069a360(void* self,void*) noexcept(false) {
    kotor_m15e_anim_base_wield_turn_on_shadows_0069a170(adjusted(self),nullptr);
}
void __fastcall model_wield_hide_thunk_0069aa10(void* self,void*,void* right,void* left) noexcept(false) {
    kotor_m15e_anim_base_wield_hide_wielded_items_0069a6b0(adjusted(self),nullptr,right,left);
}
void __fastcall model_wield_head_void_thunk_0069aa20(void* self,void*) noexcept(false) {
    kotor_m15e_slot090_void0_004015a0(adjusted(self),nullptr);
}
std::uint32_t __fastcall model_wield_enable_distortion_thunk_0069aa30(void* self,void*) noexcept(false) {
    return kotor_m15e_anim_base_wield_enable_distortion_0069a8f0(adjusted(self),nullptr);
}
std::uint32_t __fastcall model_wield_disable_distortion_thunk_0069aa40(void* self,void*) noexcept(false) {
    return kotor_m15e_anim_base_wield_disable_distortion_0069a980(adjusted(self),nullptr);
}
}
