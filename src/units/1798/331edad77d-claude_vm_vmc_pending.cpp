// Lane vmC, aoe-pending handlers: NWScript engine commands that need lane "aoe" bodies, split out so that
// claude_vm_vmC_effects.cpp / claude_vm_vmC_world.cpp link without them.  Reconstructed from the original
// swkotor.exe (read-only); conventions as claude_vm_vmC_effects.cpp.  Original handlers thiscall(routine, argc) RET 8
// -> extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc) (@16).
//  * 0052E430 ApplyEffectAtLocation -> CSWSArea::ApplyEffect 0050C6B0 (lane aoe, declared only)
//  * 00541280 PlayVisualAreaEffect  -> CSWSArea::PlayVisualEffect 00507E20 (lane aoe, declared only)
#include "1e78b2f608-claude_vm_vmc_common.hpp"

extern "C" {
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                   // ~CGameEffect
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                // CServerExoApp::GetPlayerCreatureId
void* __fastcall recovered_object_area_004cb120(void* object, void*);                     // CSWSObject::GetArea
// ---- owned by lane "aoe" (pending) ----
void __fastcall kotor_fastcall_0050c6b0(void* area, void*, void* effect, void* position, void* orientation); // CSWSArea::ApplyEffect RET 0Ch
void __fastcall kotor_fastcall_00507e20(void* area, void*, U visual, void* location);    // CSWSArea::PlayVisualEffect RET 8
}

namespace {
using namespace vmc;
inline void destroy_effect(void* e) { recovered_effect_destroy_00503590(e, nullptr); live_free(e); }
// 004015A0 is a bare `ret` (CScriptLocation trivial dtor), written as a file-local no-op as the sibling lane files do.
inline void location_dtor_004015a0(void*) {}
inline void free_location(void* l) { if (l != nullptr) { location_dtor_004015a0(l); live_free(l); } }
}  // namespace

extern "C" {

// ===================================================================================================== 0052E430
// ApplyEffectAtLocation(int nDurationType, effect eEffect, location lLocation, float fDuration=0.0f) [routine 216]
// PENDING lane "aoe" (CSWSArea::ApplyEffect 0050C6B0).
int __fastcall kotor_fastcall_0052e430(void*, void*, int, unsigned) {
    I duration_type; void* effect; void* location; float duration;                     // [esp+8] [esp+4] [esp] [esp+0C]
    if (!pop_int(&duration_type)) return k_pop_failed;                                  // 0052E439..45
    if (!pop_engine(0, &effect)) return k_pop_failed;                                   // 0052E447..5B
    if (!pop_engine(2, &location)) {                                                    // 0052E45D..71
        if (effect != nullptr) live_free(effect);                                       // 0052E4A0..AE quirk: raw free, no dtor
        return k_pop_failed;
    }
    if (!pop_float(&duration)) {                                                        // 0052E473..85
        if (location != nullptr) live_free(location);                                   // 0052E487..95 raw free
        if (effect != nullptr) live_free(effect);                                       // 0052E4A0..AE raw free
        return k_pop_failed;
    }
    if (duration_type == 0) {                                                           // 0052E4BC..CF
        at<U8>(effect, 0xa) = static_cast<U8>(at<U8>(effect, 0xa) & 0xf8u);            // 0052E505
    } else if (duration_type == 1) {                                                    // 0052E4D1..D2
        at<U16>(effect, 0xa) = static_cast<U16>((at<U16>(effect, 0xa) & 0xfff9u) | 1u); // 0052E4E9..F8
        at<float>(effect, 0xc) = duration;                                              // 0052E4FC..00
    } else {
        at<U16>(effect, 0xa) = static_cast<U16>((at<U16>(effect, 0xa) & 0xfffau) | 2u); // 0052E4D4..E3
    }
    void* server = server_app();                                                        // 0052E509..10
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 0052E513..1D
    void* area = player != nullptr ? recovered_object_area_004cb120(player, nullptr) : nullptr; // 0052E522..30
    if (area != nullptr) {
        kotor_fastcall_0050c6b0(area, nullptr, effect, location,
                                static_cast<unsigned char*>(location) + 0xc);           // 0052E532..3A
    } else {
        destroy_effect(effect);                                                         // 0052E541..4E
    }
    free_location(location);                                                            // 0052E551..62
    return 0;
}

// ===================================================================================================== 00541280
// PlayVisualAreaEffect(int nEffectID, location lTarget) [677].  Pending on lane aoe (00507E20).
// Quirk: when there is no player creature the popped location is leaked (00541303 -> 00541309).
int __fastcall kotor_fastcall_00541280(void*, void*, int, unsigned) {
    I visual = 0; void* location = nullptr;                                             // [esp+4], [esp]
    if (!pop_int(&visual)) return k_pop_failed;                                         // 00541286..95
    if (!pop_engine(2, &location)) return k_pop_failed;                                 // 00541297..AB
    void* server = server_app();
    void* creature = creature_by_id(kotor_fastcall_004aea40(server, nullptr));          // 005412B8..CC
    if (creature == nullptr) return 0;                                                  // 005412D1..D3
    void* area = recovered_object_area_004cb120(creature, nullptr);                     // 005412D5..D7
    if (area != nullptr) kotor_fastcall_00507e20(area, nullptr, static_cast<U>(visual), location); // 005412DC..EC
    if (location != nullptr) live_free(location);                                       // 005412F1..06 (004015A0 dtor is a bare ret)
    return 0;
}

}  // extern "C"
