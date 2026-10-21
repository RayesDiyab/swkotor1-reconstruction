// Full original UseSkillAction switch. All lower providers retain separate ownership.
#include "eb237338e8-w5_server_env.hpp"
#include "0a6194515c-w5_action_queue.hpp"
#include "b6881892fb-w5_foreign.hpp"
#include <cstring>
using namespace w5;
extern "C" {
extern void* kotor_cell_rules_007a3a28;
U __fastcall recovered_stats_can_use_skill_005af880(void*, void*, U);
U __fastcall recovered_object_ai_state_reputation_0057cd90(void*, void*, U);
void __fastcall recovered_creature_compute_ai_state_004f1460(void*, void*);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, U);
U __cdecl aurora_vector_equal_004aa980(const float*, const float*);
U __fastcall kotor_fastcall_004f9da0(void*, void*, U, U, U, const Vector*, U);
U __fastcall kotor_fastcall_004f0900(void*, void*, U, U, U);
U __fastcall kotor_fastcall_004f2a50(void*, void*, U);
U __fastcall kotor_fastcall_004cfb80(void*, void*, U, U, U);
U __fastcall slot_return_zero_0063e7f0_a0(void*, void*);
U __fastcall kotor_fastcall_005089d0(void*, void*);
}
namespace {
// Exact original lock checks, including repeated casts and live field rereads.
bool locked_for_skill(void* target) {
    if (at<U8>(target, 8) == 7 && at<U>(vcall<void*>(target, 0x40), 0x2c4) == 0 &&
        at<U>(vcall<void*>(target, 0x40), 0x2bc) == 1) return true;
    if (at<U8>(target, 8) == 10 && at<U>(vcall<void*>(target, 0x18), 0x2fc) == 0 &&
        at<U>(vcall<void*>(target, 0x18), 0x2e8) == 1) return true;
    if (at<U8>(target, 8) == 9 && at<U>(vcall<void*>(target, 0x48), 0x280) == 0 &&
        at<U>(vcall<void*>(target, 0x48), 0x278) == 1) return true;
    return false;
}
void clear_player_skill(void* creature) {
    if (kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) != nullptr) {
        recovered_creature_compute_ai_state_004f1460(creature, nullptr);
        if (at<U8>(creature, 0x9f2) < 10) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
    }
}
}
extern "C" {
// [004F9DA0,004F9FA8),520bytes,RET14/native@28; original trap-action producer.
U __fastcall kotor_fastcall_004f9da0(void* creature, void*, U target_id, U action_arg, U arg3,
    const Vector* position, U) {
    if (at<U>(creature, 0xe8) == 0) return 0;
    const U action = static_cast<U8>(action_arg);
    if (action == 0x1d && recovered_object_area_004cb120(creature, nullptr) != nullptr &&
        kotor_fastcall_005089d0(recovered_object_area_004cb120(creature, nullptr), nullptr) == 0) return 0;
    clear_player_skill(creature);
    void* target = kotor_fastcall_004ae750(server_app(), nullptr, target_id);
    if (target == nullptr) return 0;
    if ((action == 0x19 || action == 0x1a) && locked_for_skill(target)) return 0;
    const U available = recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, 1);
    if (available == 0) return 0;
    if (action == 0x1d) {
        U x, y, z;
        std::memcpy(&x, &position->x, 4); std::memcpy(&y, &position->y, 4); std::memcpy(&z, &position->z, 4);
        (void)recovered_object_add_action_004cea20(creature, nullptr, 0x1d, 0xffff,
            3, &target_id, 3, &arg3, 2, &x, 2, &y, 2, &z,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    } else {
        (void)recovered_object_add_action_004cea20(creature, nullptr, action, 0xffff,
            3, &target_id, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    }
    return 1;
}
// [004FBE40,004FC1EB),939 code bytes; table004FC1EC..004FC207,RET1C/native@36.
U __fastcall kotor_fastcall_004fbe40(void* creature, void*, U skill, U subtype, U target_id,
    const Vector* position, U arg5, U arg6, U arg7) {
    // Original reuses the target argument slot for the lookup output; do not invent a null substitute.
    void* target = reinterpret_cast<void*>(static_cast<std::uintptr_t>(target_id));
    const U status = kotor_fastcall_004d8230(object_array(), nullptr, target_id, &target);
    const float zero[3] = {0.0f, 0.0f, 0.0f};
    if (static_cast<U8>(status) != 0 && aurora_vector_equal_004aa980(&position->x, zero) != 0) return 1;
    if (recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, skill) == 0) {
        recovered_creature_feedback_004ede10(creature, nullptr, 0, nullptr);
        return 0;
    }
    if (target != nullptr && vcall<void*>(target, 0x30) != nullptr &&
        (at<U8>(at<void*>(kotor_cell_rules_007a3a28, 0xb4), static_cast<U8>(skill) * 0x20u + 0x1c) & 1) != 0 &&
        static_cast<U8>(recovered_object_ai_state_reputation_0057cd90(creature, nullptr, target_id)) == 1) {
        recovered_creature_feedback_004ede10(creature, nullptr, 0xbb, nullptr);
        return 0;
    }
    switch (static_cast<U8>(skill)) {
    case 1:
        if (static_cast<U8>(subtype) == 0x64 || static_cast<U8>(subtype) == 0x65 || static_cast<U8>(subtype) == 0x66) {
            const U action = static_cast<U8>(subtype) == 0x64 ? 0x1b : (static_cast<U8>(subtype) == 0x65 ? 0x1a : 0x1c);
            (void)kotor_fastcall_004f9da0(creature, nullptr, target_id, action, 0x7f000000u, position, arg5);
            return 1;
        }
        if (at<U>(creature, 0xe8) == 0) return 1;
        clear_player_skill(creature);
        target = kotor_fastcall_004ae750(server_app(), nullptr, target_id);
        if (target == nullptr || locked_for_skill(target)) return 1;
        if (recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, 1) == 0) return 1;
        (void)recovered_object_add_action_004cea20(creature, nullptr, 0x19, 0xffff,
            3, &target_id, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
        return 1;
    case 2:
        if (recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, 2) != 0)
            (void)kotor_fastcall_004f2a50(creature, nullptr, 1);
        return 1;
    case 3:
        if (vcall<U>(creature, 0x94) == 0 && slot_return_zero_0063e7f0_a0(creature, nullptr) == 0)
            (void)recovered_creature_incapacitated_004ef890(creature, nullptr);
        return 1;
    case 6:
        clear_player_skill(creature);
        (void)kotor_fastcall_004cfb80(creature, nullptr, target_id, arg5, arg6);
        return 1;
    case 7:
        (void)kotor_fastcall_004f0900(creature, nullptr, target_id, arg5, arg6);
        return 1;
    default:
        (void)arg7;
        return 1;
    }
}
}
