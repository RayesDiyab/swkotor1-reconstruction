// Nine missing CSWCAnimBase methods. Original image SHA and byte windows: evidence/OWN-ORIGINALS.json.
// Native fastcall veneers preserve the thiscall stack parameters; EDX is unused.
#include "45d12a6f86-model_methods_a.h"
#include "cc436f6baa-m15e_animbase.h"
#include "94d6ff964a-scene3d.h"
#include <cstring>

namespace {
using U = std::uint32_t;
template<class T> T load(const void* self, U offset) {
    T value; std::memcpy(&value, static_cast<const unsigned char*>(self)+offset, sizeof value); return value;
}
aurora_model::Gob* body(const void* self) { return load<aurora_model::Gob*>(self,0xb8); }
const aurora_model::GobVTable* table(const void* gob) { return load<const aurora_model::GobVTable*>(gob,0); }
U address_word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
const char talk_00754c00[] = "talk";

// Small x87 transport helpers are necessary here: the original FCOMP is signalling,
// and NaN payload selection/status from FMUL is not the SSE C++ operation.
unsigned short compare_x87(const U* lhs, const U* rhs) {
    unsigned short status;
    __asm {
        mov eax, lhs
        fld dword ptr [eax]
        mov eax, rhs
        fcomp dword ptr [eax]
        fnstsw ax
        mov status, ax
    }
    return status;
}
U multiply_x87(const U* lhs, const float* rhs) {
    U result;
    __asm {
        mov eax, lhs
        fld dword ptr [eax]
        mov eax, rhs
        fmul dword ptr [eax]
        fstp dword ptr result
    }
    return result;
}
}

extern "C" {
float model_lean_scale_007a2680 = 5.0f;

U __fastcall model_set_animation_on_subobject_0069d520(void* self, void*, U part, U a2, U a3, U a4, U a5) noexcept(false) {
    if ((part & 0xffu) != 0xffu) return 0;
    // Original replaces arg1 with this->body, retaining this and virtual dispatch.
    const U gob = address_word(body(self));
    return load<const KotorM15eAnimBaseVtbl*>(self,0)->s0f0(self,nullptr,gob,a2,a3,a4,a5);
}
U __fastcall model_set_animation_visemes_0069d540(void* self, void*, U a1, U a2, U a3, U a4) noexcept(false) {
    auto* gob = body(self);
    if (!gob) return 0;
    if (a1 && a2) table(gob)->s120_gob_play_out_of_order_animation(gob,nullptr,address_word(talk_00754c00),a1,a2,a3,a4);
    else table(gob)->s124_gob_stop_out_of_order_animation(gob,nullptr,address_word(talk_00754c00));
    return 1;
}
U __fastcall model_set_animation_on_subobject_internal_0069d5a0(void* self, void*, U a1, U part, U a3, U a4, U a5, U a6) noexcept(false) {
    if ((part & 0xffu) != 0xffu) return 0;
    return load<const KotorM15eAnimBaseVtbl*>(self,0)->s0f0(self,nullptr,a1,a3,a4,a5,a6);
}
void __fastcall model_set_always_render_0069d6e0(void* self, void*, U on) noexcept(false) {
    auto* gob = body(self);
    if (!gob) return;
    if (on) table(gob)->s130_gob_enable_always_render(gob,nullptr);
    else table(gob)->s134_gob_disable_always_render(gob,nullptr);
}
void __fastcall model_set_in_cutscene_0069d710(void* self, void*, U on) noexcept(false) {
    auto* gob = body(self);
    if (!gob) return;
    if (on) table(gob)->s138_gob_enable_in_cutscene(gob,nullptr);
    else table(gob)->s13c_gob_disable_in_cutscene(gob,nullptr);
}
void __fastcall model_set_lean_0069dd30(void* self, void*, U raw) noexcept(false) {
    const U low=0xbf800000u, high=0x3f800000u;
    U selected=raw;
    if ((compare_x87(&low,&raw) & 0x4100u) == 0) selected=low;
    else {
        const unsigned short status=compare_x87(&high,&raw);
        // JP after TEST AH,5: take raw on equality/greater/unordered; clamp only below.
        if (((status >> 8) & 5u) == 1u) selected=high;
    }
    const U result=multiply_x87(&selected,&model_lean_scale_007a2680);
    std::memcpy(static_cast<unsigned char*>(self)+0xd8,&result,sizeof result);
}
U __fastcall model_disable_bumped_out_shell_0069ddf0(void* self, void*) noexcept(false) {
    auto* gob=body(self);
    if (!gob) return 0;
    table(gob)->s154_gob_disable_render_bumped_out(gob,nullptr);
    return 1;
}
U __fastcall model_set_motion_blur_0069de10(void* self, void*, U on) noexcept(false) {
    auto* gob=body(self);
    if (!gob) return 0;
    // TEST EDX,EDX / SETNE DL / PUSH EDX changes only the low byte.
    const U forwarded=(on & 0xffffff00u) | (on != 0 ? 1u : 0u);
    table(gob)->s144_gob_set_motion_blurred(gob,nullptr,forwarded);
    return 1;
}
U __fastcall model_set_max_motion_blur_length_0069de40(void* self, void*, U) noexcept(false) {
    // Faithful original stub: reports body presence; it never invokes Gob::SetMaxBlurLength.
    return body(self) != nullptr ? 1u : 0u;
}
}
