// Original swkotor.exe 0069A3A0, 0069A3C0, 006996D0, 00699730, 00699A70, 00699A80.
// Bound to tier t2 (0069D490 / 0069D4C0). Slot types are the canonical
// KotorM15eAnimBaseVtbl s0c8/s0cc declarations (selected 0837 header copy).
#include "102438969d-anim_base_animation_flag.h"
#include "3f0de2542c-anim_base_animation_flag_internal.h"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <type_traits>

using kotor_m15e::U;
using I = std::int32_t;
using B = std::uint8_t;

static_assert(std::is_same_v<decltype(&anim_base_add_animation_flag_0069a3a0), decltype(KotorM15eAnimBaseVtbl::s0c8)>, "s0c8");
static_assert(std::is_same_v<decltype(&anim_base_remove_animation_flag_0069a3c0), decltype(KotorM15eAnimBaseVtbl::s0cc)>, "s0cc");
static_assert(std::is_same_v<decltype(&anim_base_head_wield_add_animation_flag_006996d0), decltype(KotorM15eAnimBaseVtbl::s0c8)>, "s0c8");
static_assert(std::is_same_v<decltype(&anim_base_head_wield_remove_animation_flag_00699730), decltype(KotorM15eAnimBaseVtbl::s0cc)>, "s0cc");
static_assert(std::is_same_v<decltype(&anim_base_head_wield_add_animation_flag_vtordisp_00699a70), decltype(KotorM15eAnimBaseVtbl::s0c8)>, "s0c8");
static_assert(std::is_same_v<decltype(&anim_base_head_wield_remove_animation_flag_vtordisp_00699a80), decltype(KotorM15eAnimBaseVtbl::s0cc)>, "s0cc");
static_assert(offsetof(KotorM15eAnimBaseVtbl, s0c8) == 0xC8 && offsetof(KotorM15eAnimBaseVtbl, s0cc) == 0xCC, "slots");

namespace {
template<class T> T rd(const B* p) { T v; std::memcpy(&v, p, sizeof v); return v; }
aurora_model::Gob* gob_at(const B* p) { return rd<aurora_model::Gob*>(p); }

// 006996D0 / 00699730 shared shape. self = subobject whose vbptr is at self-8.
// first Gob  = [self + vbtbl[1] + B0]  (= [vbase1 + B8], vbase1 = self-8+vbtbl[1])
// second Gob = [self + vbtbl[2] + 3C]  (= [vbase2 + 44]); called only if the first returned non-zero.
// The original also forms ECX = self + vbtbl[1] - 8 for each call; 0069D490/0069D4C0 overwrite ECX
// before use (t2 contract), so that value is not an input and is not reproduced.
template<U (__stdcall* Internal)(aurora_model::Gob*, U)>
U head_wield_forward(void* self, U flag) {
    const B* s = static_cast<const B*>(self);
    const B* vbtable = rd<const B*>(s - 8);                                 // 006996D3 mov eax,[esi-8]
    const I first = rd<I>(vbtable + 4);                                     // 006996D6 mov ecx,[eax+4]
    if (!Internal(gob_at(s + first + 0xB0), flag)) return 0;                // 006996D9..006996F4
    vbtable = rd<const B*>(s - 8);                                          // 006996F6 re-read [esi-8]
    const I second = rd<I>(vbtable + 8);                                    // 006996F9 mov ecx,[eax+8]
    if (!Internal(gob_at(s + second + 0x3C), flag)) return 0;               // 006996FC..00699710
    return 1;                                                               // 00699713
}
// vtordisp thunk: ECX -= [ECX-4], then the body (00699A70 / 00699A80).
B* vtordisp_adjust(void* self) {
    B* s = static_cast<B*>(self);
    return s - rd<I>(s - 4);
}
}  // namespace

// 0069A3A0: push [esp+4]; push [ecx+B8]; call 0069D490; ret 4 (EAX passes through).
extern "C" U __fastcall anim_base_add_animation_flag_0069a3a0(void* self, void*, U flag) {
    return anim_base_add_animation_flag_internal_0069d490(gob_at(static_cast<const B*>(self) + 0xB8), flag);
}
// 0069A3C0: same with 0069D4C0.
extern "C" U __fastcall anim_base_remove_animation_flag_0069a3c0(void* self, void*, U flag) {
    return anim_base_remove_animation_flag_internal_0069d4c0(gob_at(static_cast<const B*>(self) + 0xB8), flag);
}
extern "C" U __fastcall anim_base_head_wield_add_animation_flag_006996d0(void* self, void*, U flag) {
    return head_wield_forward<&anim_base_add_animation_flag_internal_0069d490>(self, flag);
}
extern "C" U __fastcall anim_base_head_wield_remove_animation_flag_00699730(void* self, void*, U flag) {
    return head_wield_forward<&anim_base_remove_animation_flag_internal_0069d4c0>(self, flag);
}
extern "C" U __fastcall anim_base_head_wield_add_animation_flag_vtordisp_00699a70(void* self, void*, U flag) {
    return anim_base_head_wield_add_animation_flag_006996d0(vtordisp_adjust(self), nullptr, flag);
}
extern "C" U __fastcall anim_base_head_wield_remove_animation_flag_vtordisp_00699a80(void* self, void*, U flag) {
    return anim_base_head_wield_remove_animation_flag_00699730(vtordisp_adjust(self), nullptr, flag);
}
