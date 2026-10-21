// Full original AddAttackActions. Existing queue/perception/event storage owners are reused.
#include "d6165c6b23-w5_server_env.hpp"
#include "648dcae80c-w5_action_queue.hpp"
#include "3ecda8a38e-w5_foreign.hpp"
using namespace w5;
extern "C" {
void __fastcall recovered_combat_round_remove_all_actions_004d3770(void*, void*);
void __fastcall kotor_fastcall_004d38b0(void*, void*, U, U, U, U, U);
void* __fastcall kotor_fastcall_004cc630(void*, void*, U);
void __fastcall recovered_creature_compute_ai_state_004f1460(void*, void*);
void* __fastcall recovered_creature_visible_element_00517a20(void*, void*, U);
U __fastcall recovered_object_ai_state_reputation_0057cd90(void*, void*, U);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, U);
void __fastcall w5_combat_clear_all_attacks_004d37e0(void*, void*);
void* __fastcall w5_server_get_door_by_id_004ae7c0(void*, void*, U);
U __fastcall recovered_object_add_action_to_front_004ccb00(void*, void*, U, U,
    U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*, U, void*);
U __fastcall recovered_party_table_leader_00563a00(void*, void*, U);
void __fastcall kotor_fastcall_004fbbe0(void*, void*, U);
void __fastcall kotor_fastcall_004ec8e0(void*, void*);
I __cdecl crt_rand_006fc447();
}
namespace {
void* party_table() { return static_cast<unsigned char*>(at<void*>(server_app(), 4)) + 0x1b770; } // exact inline004AEE70, no new address credit
void queue_attack(void* creature, bool front, U target, U mode, U p8, U p9, U p10, U p11, U p12, U p13, U p14, U p15) {
    if (front) {
        recovered_object_add_action_to_front_004ccb00(creature, nullptr, 0x0c, 0xffff,
            1, &p12, 3, &target, 1, &p8, 1, &p9, 1, &p10, 1, &p11, 1, &mode, 1, &p13, 1, &p14, 1, &p15,
            0, nullptr, 0, nullptr, 0, nullptr);
    } else {
        (void)recovered_object_add_action_004cea20(creature, nullptr, 0x0c, 0xffff,
            1, &p12, 3, &target, 1, &p8, 1, &p9, 1, &p10, 1, &p11, 1, &mode, 1, &p13, 1, &p14, 1, &p15,
            0, nullptr, 0, nullptr, 0, nullptr);
    }
}
}
extern "C" {
// [004FDE40,004FE20F),975bytes,RET3C/native@68; two original return sites.
U __fastcall kotor_fastcall_004fde40(void* creature, void*, U target, U mode, U round_arg,
    U state, U clear, U front, U immediate, U p8, U p9, U p10, U p11, U p12, U p13, U p14, U p15) {
    if (immediate == 0) {
        if (round_arg != 0 && clear != 0) {
            w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
            recovered_combat_round_remove_all_actions_004d3770(at<void*>(creature, 0x9c8), nullptr);
            w5_combat_clear_all_attacks_004d37e0(at<void*>(creature, 0x9c8), nullptr);
        }
        kotor_fastcall_004d38b0(at<void*>(creature, 0x9c8), nullptr, target, mode, round_arg, 1500, 1);
        if (at<U>(creature, 0x4e0) == 0 && kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) != nullptr) {
            recovered_creature_compute_ai_state_004f1460(creature, nullptr);
            w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
        }
        if (kotor_fastcall_004cc630(creature, nullptr, 0x3f) == nullptr) {
            U one = 1;
            (void)recovered_object_add_action_004cea20(creature, nullptr, 0x3f, 0xffff,
                1, &one, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
                0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
        }
        return 1;
    }
    recovered_creature_compute_ai_state_004f1460(creature, nullptr);
    if (at<U>(creature, 0xe8) == 0) return 0;
    void* visible = recovered_creature_visible_element_00517a20(creature, nullptr, target);
    if (visible != nullptr && (at<U8>(visible, 4) & 0x0c) == 4) return 0;
    if (target != 0x7f000000u) {
        void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);
        if (object != nullptr && vcall<void*>(object, 0x30) != nullptr &&
            static_cast<U8>(recovered_object_ai_state_reputation_0057cd90(creature, nullptr, target)) == 1) {
            recovered_creature_feedback_004ede10(creature, nullptr, 0xbb, nullptr);
            return 0;
        }
    }
    if (clear != 0) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
    if (target != 0x7f000000u) {
        queue_attack(creature, front != 0, target, mode, p8, p9, p10, p11, p12, p13, p14, p15);
        void* other = kotor_fastcall_004ae770(server_app(), nullptr, target);
        if (other != nullptr && (at<U>(other, 0x9d4) == 1 ||
            recovered_party_table_leader_00563a00(party_table(), nullptr, at<U>(other, 4)) == 1))
            at<U>(other, 0x520) = at<U>(creature, 4);
        kotor_fastcall_004fbbe0(creature, nullptr, at<U>(creature, 4));
        if (other != nullptr) at<U>(creature, 0x4dc) = at<U>(creature, 0xa88) == 0 ? 1u : 0u;
        bool bark = false;
        if (at<U>(creature, 0x4e0) == 0 && at<U>(creature, 0x9d4) == 1) bark = crt_rand_006fc447() % 4 < 3;
        else if (at<U>(creature, 0xa88) == 1) bark = crt_rand_006fc447() % 10 == 0;
        if (bark) kotor_fastcall_004ec8e0(creature, nullptr);
        if (at<U>(creature, 0x50c) == 0x7f000000u) at<U>(creature, 0x50c) = target;
        at<U>(creature, 0x4e8) = state;
    }
    void* door = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, target);
    if (door != nullptr && at<U>(door, 0xf8) == 1) {
        void* allocation = live_operator_new(0x34);
        void* event;
        try { event = allocation != nullptr ? recovered_event_construct_004d7540(allocation, nullptr) : nullptr; }
        catch (...) { live_free(allocation); throw; } // original0071B65B state0 allocation cleanup
        at<U16>(event, 0) = 0x22; // original null allocation fault retained
        (void)recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, at<U>(creature, 4), target,
            k_event_queue_type_007441dc, event);
    }
    return 1;
}
}
