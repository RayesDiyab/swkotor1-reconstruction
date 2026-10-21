// Bounded original queue helpers; use existing W1 action/storage owners.
#include "851cded6c4-w5_server_env.hpp"
#include "0f8794c60e-w5_action_queue.hpp"
#include "c5a559a231-w5_foreign.hpp"
using namespace w5;
extern "C" {
U __fastcall recovered_stats_can_use_skill_005af880(void*, void*, U);
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void __fastcall recovered_set_stealth_mode_0050ee50(void*, void*, U, U);
void __fastcall w5_creature_set_combat_mode_0050ee80(void*, void*, U, U);
U __fastcall slot_return_zero_0063e7f0_a0(void*, void*);
// [004F2A50,004F2C24),468bytes,RET4. Existing W2/W5 real mode setters; no effect rewrite.
U __fastcall kotor_fastcall_004f2a50(void* creature, void*, U mode_arg) {
    if (vcall<U>(creature, 0x94) != 0 || slot_return_zero_0063e7f0_a0(creature, nullptr) != 0 ||
        recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) return 0;
    const U8 mode = static_cast<U8>(mode_arg);
    if (mode == 1) {
        if (recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, 2) == 0) return 1;
        if (at<U8>(creature, 0x4d1) == 1) {
            if ((at<U8>(creature, 0xa00) & 1) != 0) return 1;
            const U prior = at<U>(creature, 0x9fc);
            at<U>(creature, 0x9fc) = prior & 0xfffffffeu;
            if ((prior & 1) != 0) recovered_set_stealth_mode_0050ee50(creature, nullptr, 0, 0);
            return 1;
        }
        if (at<U>(creature, 0x4e0) != 0) {
            recovered_creature_feedback_004ede10(creature, nullptr, 0x3c, nullptr);
            return 1;
        }
        if ((at<U8>(creature, 0xa00) & 1) != 0) return 1;
        const U prior = at<U>(creature, 0x9fc), updated = prior | 1;
        at<U>(creature, 0x9fc) = updated;
        if ((prior & 1) == 0) {
            if ((updated & 0x0c) == 0) recovered_set_stealth_mode_0050ee50(creature, nullptr, 1, 0);
            else at<U>(creature, 0x9fc) = updated & 0xfffffffeu;
        }
        return 1;
    }
    if (mode >= 2 && mode <= 7) {
        if (at<U8>(creature, 0x4d2) == static_cast<U8>(mode - 1))
            w5_creature_set_combat_mode_0050ee80(creature, nullptr, 0, 1);
        else w5_creature_set_combat_mode_0050ee80(creature, nullptr, mode - 1, 0);
    }
    return 1;
}
// [004CC630,004CC684),84bytes,RET4. EAX is the actual matching action pointer.
void* __fastcall kotor_fastcall_004cc630(void* object, void*, U type) {
    void* list = at<void*>(object, 0xfc);
    void* position = at<void*>(list, 0);
    if (position == nullptr) return nullptr;
    void* action = unresolved_list_at_pos_005e9b60(list, nullptr, position);
    while (position != nullptr) {
        if (at<U>(action, 0) == static_cast<U16>(type)) return action;
        action = reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(at<void*>(object, 0xfc), nullptr, &position)));
    }
    return nullptr;
}
// [004F0900,004F098F),143bytes,RET0C. Existing W1 AddAction38; all original guards.
U __fastcall kotor_fastcall_004f0900(void* creature, void*, U target, U arg2, U arg3) {
    if (recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, 7) == 0) {
        recovered_creature_feedback_004ede10(creature, nullptr, 0, nullptr);
        return 0;
    }
    if (at<U>(creature, 0xe8) == 0) return 0;
    U one = 1;
    (void)recovered_object_add_action_004cea20(creature, nullptr, 0x38, 0xffff,
        3, &target, 3, &arg2, 1, &arg3, 1, &one,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;
}
// [004CFB80,004CFC1A),154bytes,RET0C. Original repeats virtual creature casts.
U __fastcall kotor_fastcall_004cfb80(void* object, void*, U target, U arg2, U arg3) {
    if (vcall<void*>(object, 0x30) != nullptr &&
        recovered_stats_can_use_skill_005af880(at<void*>(vcall<void*>(object, 0x30), 0xa74), nullptr, 6) == 0) {
        recovered_creature_feedback_004ede10(vcall<void*>(object, 0x30), nullptr, 0, nullptr);
        return 0;
    }
    if (at<U>(object, 0xe8) == 0) return 0;
    (void)recovered_object_add_action_004cea20(object, nullptr, 0x26, 0xffff,
        3, &target, 3, &arg2, 1, &arg3,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;
}
}
