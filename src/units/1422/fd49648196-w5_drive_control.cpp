// W5 G16 server input: CSWSMessage::HandlePlayerToServerInputDriveControl (00523450) and its state resets:
// CSWSCreature::SetCombatMode 0050EE80, CSWSCombatRound::ClearAllSpecialAttacks 004D4F60,
// CSWSObject::ClearAllActions 004CCD80 (node dtor 004CC4D0 is W1-owned),
// CSWSCreature::ActionManager 004F8770, CSWSCombatRound::ClearAllAttacks 004D37E0,
// CSWSCombatAttackData::ClearAttackData 004D3010. Explicit trap: SetActivity 004F00A0.
#include "8342c24c51-w5_drive_control.hpp"
#include "71303376b8-w5_action_queue.hpp"
#include "73b9db80fe-w5_pathfind_reset.hpp"
#include "294ff121b7-w5_server_env.hpp"

using namespace w5;

extern "C" {

// explicit trap (W5-owned, G18 member, reached only when the creature leaves/enters a nonzero combat mode)

// 0050EE80 CSWSCreature::SetCombatMode(byte mode, int checkWeapon): thiscall RET 8.
void __fastcall w5_creature_set_combat_mode_0050ee80(void* creature, void*, U mode_arg, U check) {
    const U8 mode = static_cast<U8>(mode_arg);
    U8 old = at<U8>(creature, 0x4d2);
    auto weapon = [&]() { return recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 0x10); };   // slot 10h
    auto ranged_flag = [&](void* item) -> U8 {                            // base item byte +1A
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr, at<I>(item, 0xc));
        return at<U8>(base, 0x1a);
    };
    auto feedback = [&]() { recovered_creature_feedback_004ede10(creature, nullptr, 0xbc, nullptr); };
    if (check == 1) {                                                     // 0050EE93..0050EF66
        if (mode == 5) { if (weapon() != nullptr) { feedback(); return; } }
        else if (mode == 6) { void* w = weapon(); if (w == nullptr || ranged_flag(w) == 0) { feedback(); return; } }
        else if (mode == 2 || mode == 3 || mode == 1) { void* w = weapon(); if (w != nullptr && ranged_flag(w) != 0) { feedback(); return; } }
    } else {                                                              // 0050EF6B..0050F006
        bool forced = false;
        if (mode == 4 && old != 4) { at<U8>(creature, 0x4d2) = 4; forced = true; }   // jumps straight to the switch
        if (!forced) {
            if (mode == at<U8>(creature, 0x4d2)) return;
            if (at<U>(at<void*>(creature, 0x9c8), 0x934) == 1) { at<U8>(creature, 0x4d3) = mode; return; }
            if (mode == 5) { if (weapon() != nullptr) { feedback(); return; } }
            else if (mode == 6) { void* w = weapon(); if (w == nullptr || ranged_flag(w) == 0) { feedback(); return; } }
            old = at<U8>(creature, 0x4d2);                                // 0050F006
        }
        if (forced) goto activities;
    }
    at<U8>(creature, 0x4d2) = mode;                                       // 0050F017
    if (mode == 1) { recovered_set_activity_004f00a0(creature, nullptr, 0x400, 1, 0); return; }
activities:
    if (old == 1) { recovered_set_activity_004f00a0(creature, nullptr, 0x400, 0, 0); return; }
    if (mode == 2) { recovered_set_activity_004f00a0(creature, nullptr, 0x100, 1, 0); return; }
    if (old == 2) { recovered_set_activity_004f00a0(creature, nullptr, 0x100, 0, 0); return; }
    if (mode == 3) { recovered_set_activity_004f00a0(creature, nullptr, 0x200, 1, 0); return; }
    if (old == 3) { recovered_set_activity_004f00a0(creature, nullptr, 0x200, 0, 0); return; }
    if (mode == 4) { recovered_set_activity_004f00a0(creature, nullptr, 0x800, 1, 0); return; }
    if (old == 4) { recovered_set_activity_004f00a0(creature, nullptr, 0x800, 0, 0); return; }
    if (mode == 5) { recovered_set_activity_004f00a0(creature, nullptr, 0x1000, 1, 0); return; }
    if (old == 5) { recovered_set_activity_004f00a0(creature, nullptr, 0x1000, 0, 0); return; }
    if (mode == 6) { recovered_set_activity_004f00a0(creature, nullptr, 0x2000, 1, 0); return; }
    if (old == 6) { recovered_set_activity_004f00a0(creature, nullptr, 0x2000, 0, 0); return; }
}

// 004D4F60 CSWSCombatRound::ClearAllSpecialAttacks.
void __fastcall w5_combat_clear_all_special_attacks_004d4f60(void* round, void*) {
    for (I i = at<U8>(round, 0x96c); i < static_cast<I>(static_cast<U8>(at<U8>(round, 0x990) + at<U8>(round, 0x994))); ++i) {
        const I k = i < 0 ? 0 : i;
        U16& special = at<U16>(round, 0x68 + k * 0x14c);
        if (special != 0) special = 0;
    }
    if (at<void*>(round, 0x918) != nullptr) {
        live_free(at<void*>(round, 0x918));
        at<U>(round, 0x918) = 0;
        at<U>(round, 0x920) = 0;
    }
    at<U>(round, 0x91c) = 0;
}

// 004D3010 CSWSCombatAttackData::ClearAttackData (field resets; owned effects destroyed through the import).
void __fastcall w5_combat_attack_clear_004d3010(void* data, void*) {
    at<U8>(data, 0) = 0xff; at<U>(data, 4) = 0; at<U16>(data, 8) = 0; at<U>(data, 0x18) = 0;
    std::memset(static_cast<unsigned char*>(data) + 0x1c, 0xff, 7 * 4 + 2);   // rep stosd x7 + stosw (-1)
    at<U>(data, 0x68) = 0; at<U>(data, 0x6c) = 0; at<U>(data, 0x70) = 0;
    at<U8>(data, 0x5c) = 0; at<U>(data, 0x60) = 0; at<U>(data, 0xc) = k_object_invalid;
    at<U16>(data, 0x10) = 0; at<U16>(data, 0x12) = 0x2711; at<U16>(data, 0x14) = 0;
    at<U8>(data, 0x3c) = 0; at<U16>(data, 0x64) = 0; at<U8>(data, 0x3b) = 0; at<U16>(data, 0x66) = 0xff;
    at<U8>(data, 0x3a) = 0; at<U>(data, 0x44) = 0; at<U>(data, 0x40) = 0; at<U>(data, 0x48) = 0; at<U>(data, 0x4c) = 0;
    at<U>(data, 0x50) = 0; at<U8>(data, 0x54) = 0; at<U>(data, 0x58) = 0; at<U>(data, 0x74) = k_object_invalid;
    c2da_assign_text_005e5140(static_cast<unsigned char*>(data) + 0x78, nullptr, "");   // .rdata 0073D71C ""
    c2da_assign_text_005e5140(static_cast<unsigned char*>(data) + 0x80, nullptr, "");
    for (I k = 0; k < at<I>(data, 0x8c); ++k) {
        void* effect = at<void**>(data, 0x88)[k];
        if (effect != nullptr) { w5_import_game_effect_destroy_00503590(effect, nullptr); live_free(effect); }
    }
    at<U>(data, 0x8c) = 0; at<U>(data, 0x98) = 0; at<U>(data, 0xa4) = 0; at<U>(data, 0xb0) = 0;
    at<U>(data, 0xb8) = 0; at<U>(data, 0xbc) = 0;
    for (U o = 0xc0; o <= 0xc7; ++o) at<U8>(data, o) = 0;
    at<U>(data, 0xc8) = 0;
    for (U o = 0xcc; o <= 0xd5; ++o) at<U8>(data, o) = 0;
    at<U>(data, 0xd8) = 0; at<U>(data, 0xdc) = 0; at<U8>(data, 0xe0) = 0; at<U>(data, 0xe4) = 0;
    at<U8>(data, 0xe8) = 0; at<U>(data, 0xec) = 0; at<U8>(data, 0xf0) = 0; at<U8>(data, 0xf1) = 0;
    for (U o = 0xf4; o <= 0xfc; ++o) at<U8>(data, o) = 0;
    for (U o = 0xfe; o <= 0x109; ++o) at<U8>(data, o) = 0;
    std::memset(static_cast<unsigned char*>(data) + 0x10a, 0xff, 7 * 4 + 2);
    at<U>(data, 0x128) = k_object_invalid;
    for (U o = 0x12c; o <= 0x133; ++o) at<U8>(data, o) = 0;
    at<U8>(data, 0x134) = 0xff;
    at<U>(data, 0x138) = 0; at<U>(data, 0x13c) = 0;
    for (U o = 0x140; o <= 0x148; ++o) at<U8>(data, o) = 0;
}

// 004D37E0 CSWSCombatRound::ClearAllAttacks: ClearAttackData on the five 14Ch-byte attack records at +4.
void __fastcall w5_combat_clear_all_attacks_004d37e0(void* round, void*) {
    for (I i = 0; i < 5; ++i) w5_combat_attack_clear_004d3010(static_cast<unsigned char*>(round) + 4 + (i < 0 ? 0 : i) * 0x14c, nullptr);
}

// 004CCD80 CSWSObject::ClearAllActions(int clearCombat): thiscall RET 4.
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat) {
    // 004CCD80..004CCDAC: debug-only CStr of the tag when [0083281C] != 0 (no state change; not reproduced)
    if (at<U>(object, 0xe8) != 1) return;                                 // 004CCDB1..004CCDB8
    void* list = at<void*>(object, 0xfc);
    void* position = *static_cast<void**>(list);
    while (position != nullptr) {                                         // 004CCDD0..004CCE3E
        void* current = position;
        void* node = unresolved_list_at_pos_005e9b60(at<void*>(object, 0xfc), nullptr, current);
        unresolved_list_next_005e9b90(at<void*>(object, 0xfc), nullptr, &position);
        void* head = *static_cast<void**>(at<void*>(object, 0xfc));
        void* head_node = head ? at<void*>(head, 8) : nullptr;
        if (vcall<U>(object, 0x74, node, node == head_node ? 1u : 0u) == 1) {
            // Remove 005E9C10 returns pos->data; the reused provider (c2da_005e9c10, 0191) is declared void, so the
            // data fetched at this position by GetAtPos (identical value) is used as the removed node.
            c2da_005e9c10(at<void*>(object, 0xfc), nullptr, current);
            void* removed = node;
            if (removed != nullptr) { recovered_action_node_destroy_004cc4d0(removed, nullptr); live_free(removed); }
        }
    }
    void* creature = vcall<void*>(object, 0x30);                          // AsSWSCreature
    if (creature == nullptr) return;
    w5_pathfind_reset_at_end_of_path_005d0ec0(at<void*>(vcall<void*>(object, 0x30), 0x340), nullptr);
    at<U>(vcall<void*>(object, 0x30), 0xa8c) = 1;
    at<U>(vcall<void*>(object, 0x30), 0x508) = k_object_invalid;
    void* pf = at<void*>(vcall<void*>(object, 0x30), 0x340);
    at<U>(pf, 0x254) = k_object_invalid;
    at<U>(pf, 0x258) = 0; at<U>(pf, 0x25c) = 0; at<U>(pf, 0x260) = 0;
    if (clear_combat != 0) w5_combat_clear_all_attacks_004d37e0(at<void*>(vcall<void*>(object, 0x30), 0x9c8), nullptr);
    if (at<U>(vcall<void*>(object, 0x30), 0xa88) == 0) return;            // party member only
    void* client = client_app();
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(vcall<void*>(object, 0x30), 4));
    const I index = static_cast<I>(recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, client_id));
    if (index == -1) return;
    at<U>(kotor_fastcall_005ed8b0(client_app(), nullptr), 0x28 + index * 0x88) = 0xffffffffu;   // 004CCF39..004CCF4C
}

// 004F8770 CSWSCreature::ActionManager(int reason): thiscall RET 4.
void __fastcall w5_creature_action_manager_004f8770(void* creature, void*, U reason) {
    if (kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) == nullptr) return;   // player-controlled only
    recovered_creature_compute_ai_state_004f1460(creature, nullptr);
    if ((reason == 1 || reason == 2) && at<U8>(creature, 0x9f2) < 10) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
    if (reason == 8 || reason == 4) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
}

// 00523450 CSWSMessage::HandlePlayerToServerInputDriveControl(player): thiscall RET 4.
// Payload: float x, float y (client position), word heading*10 (12 bits), byte a, byte key flags.
U __fastcall w5_message_input_drive_control_00523450(void* message, void*, void* player) {
    const float x = static_cast<float>(kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u /*1.0f*/, 0x20));   // 0052345F
    const float y = static_cast<float>(kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u /*1.0f*/, 0x20));   // 00523471
    const U heading = static_cast<U16>(kotor_fastcall_004d6750(message, nullptr, 0x0c));
    const U a = static_cast<U8>(kotor_fastcall_004d6720(message, nullptr, 8));
    const U flags = static_cast<U8>(kotor_fastcall_004d6720(message, nullptr, 8));
    if (recovered_player_message::overrun_004d6230(message) != 0) return 0;     // 005234A1 MessageReadOverflow(1)
    if (recovered_player_message::underrun_004d6240(message) != 0) return 0;    // 005234B2 MessageReadUnderflow(1)
    void* object = kotor_fastcall_00561080(player, nullptr);                // CSWSPlayer::GetGameObject
    if (object == nullptr) return 0;
    const Vector client{x, y, 0.0f};
    void* creature = vcall<void*>(object, 0x30);
    if (creature == nullptr) return 1;                                    // 005234F3..00523551
    if (at<U>(creature, 0x9dc) != 0) return 0;                            // 005234F5..005234FD
    if (at<U>(creature, 0xe8) != 1) return 1;                             // 005234FF..00523506 (not commandable)
    w5_creature_set_combat_mode_0050ee80(creature, nullptr, 0, 1);        // 0052350E
    w5_combat_clear_all_special_attacks_004d4f60(at<void*>(creature, 0x9c8), nullptr);
    w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
    w5_creature_action_manager_004f8770(creature, nullptr, 8);
    w5_add_drive_action_004eb510(creature, nullptr, 0xffff, &client, heading, a, flags, 2);   // 0052354C
    return 1;
}
}
