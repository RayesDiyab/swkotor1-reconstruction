// Private ACTION9 clear9 cleanup cluster. Source-only reconstruction of the known
// 15-body cleanup set (3,542 bytes) plus the 004D3440/004D3C30/004D3CE0 combat
// helpers required by the full 004F0420/004F06D0 bodies.
//
// Original evidence: swkotor.exe SHA-256
// 9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435 (read-only).
// Every function comment gives the original window and the observed RET immediate.
#include "b5464887b3-clear9_cleanup.hpp"

using clear9::at;
using clear9::vcall;
using clear9::U;
using clear9::I;
using clear9::U8;
using clear9::U16;
using clear9::uword;
using clear9::word;

namespace {
inline void* manager() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* server_app() { return at<void*>(manager(), 8); }
inline void* client_app() { return at<void*>(manager(), 4); }
inline U invalid_object() { return menu_tables_m41::cells::invalid_object_00746644; }
}  // namespace

extern "C" {

// [combat-gaps] 005E9BD0 CExoLinkedListInternal::GetPrev(CExoLinkedListNode**) RET 4 (selected w4-conjure-parent body).
void* __fastcall kotor_fastcall_005e9bd0(void*, void*, void**);

// 004F8280..004F8286 (7 bytes, RET): vtable +C4 raw getter -> self+944 DWORD.
U __fastcall recovered_creature_field_004f8280(void* self, void*) {
    return at<U>(self, 0x944);
}

// 004D3580..004D35B1 (50 bytes, RET): combat-round current-action predicate.
I __fastcall recovered_combat_round_has_current_action_004d3580(void* round, void*) {
    void* wrapper = at<void*>(round, 0x9b0);          // 004D3583
    if (!wrapper) return 0;                            // 004D3589/004D358B
    void* list = at<void*>(wrapper, 0);                // 004D358D (no null check in the original)
    void* position = at<void*>(list, 0);               // 004D358F
    if (!position) return 0;                           // 004D3591/004D3593
    void* node = unresolved_list_at_pos_005e9b60(list, nullptr, position);  // 004D3595..004D3598
    return at<I>(round, 0x944) >= at<I>(node, 0) ? 1 : 0;                  // 004D359D..004D35A5
}

// 004D3BA0..004D3C27 (136 bytes, RET): remove every round action of kind byte 2.
void __fastcall recovered_combat_round_clear_current_004d3ba0(void* round, void*) {
    void* wrapper = at<void*>(round, 0x9b0);           // 004D3BA4
    if (!wrapper) return;                              // 004D3BAA/004D3BAC
    void* list = at<void*>(wrapper, 0);                // 004D3BAE/004D3BB0
    if (!list) return;                                 // 004D3BB2/004D3BB4
    void* position = at<void*>(list, 0);               // 004D3BB0
    while (position) {
        wrapper = at<void*>(round, 0x9b0);             // 004D3BC0
        list = at<void*>(wrapper, 0);
        void* node = unresolved_list_at_pos_005e9b60(list, nullptr, position);  // 004D3BC9
        if (node && at<U8>(node, 0x10) == 2) {          // 004D3BD4/004D3BD8
            void* saved = position;                     // 004D3BE0/004D3BE4
            unresolved_list_next_005e9b90(list, nullptr, &position);            // 004D3BE9..004D3BEB
            c2da_005e9c10(list, nullptr, saved);        // 004D3BF8/004D3BF9
            live_free(node);                            // 004D3BFE..004D3C04
        } else {
            unresolved_list_next_005e9b90(list, nullptr, &position);            // 004D3C16
        }
    }
}

// 004EF4D0..004EF513 (68 bytes, RET): cancel the current player action through a message.
void __fastcall recovered_creature_cancel_action_004ef4d0(void* self, void*) {
    if (at<U>(self, 0x9d4) != 1) return;               // 004EF4D0..004EF4D7
    void* server = at<void*>(manager(), 8);            // 004EF4DC/004EF4E2
    void* client_object = kotor_fastcall_004ae910(server, nullptr, at<U>(self, 4)); // 004EF4D9..004EF4E7
    void* message = recovered_server_sws_message_004aedb0(server, nullptr);         // 004EF4EC..004EF4F7
    if (client_object && message)                      // 004EF4FC..004EF502
        recovered_action_cancel_message_0056b380(message, nullptr, client_object, 0, 0, 0); // 004EF504..004EF50D
}

// 0056B380..0056B3FD (128 bytes, RET 10h): cancel/stop action message.
U __fastcall recovered_action_cancel_message_0056b380(void* self, void*, void* object, U flag, U byte_value, U word_value) {
    kotor_fastcall_004d68a0(self, nullptr, 8, 0xffffffffu, 1);   // 004D68A0(this, 8, -1, 1)
    kotor_fastcall_004d68e0(self, nullptr, flag);                // 004D68E0(this, bool)
    if (flag) {
        kotor_fastcall_004d6340(self, nullptr, word_value, 0x20); // 004D6340(this, word, 20h)
        kotor_fastcall_004d62c0(self, nullptr, byte_value, 8);    // 004D62C0(this, byte, 8)
    }
    void* data = nullptr;
    U size = 0;
    if (!kotor_fastcall_004d6440(self, nullptr, &data, &size)) return 0; // 004D6440
    return kotor_fastcall_00567e60(self, nullptr, at<U>(object, 4), 0x30, 1, data, size); // 00567E60
}

// 0056FB70..0056FBC7 (90 bytes, RET 8): inventory item message (major 0Ch minor 4).
U __fastcall recovered_inventory_item_message_0056fb70(void* self, void*, U recipient, U object_id) {
    kotor_fastcall_004d68a0(self, nullptr, 4, 0xffffffffu, 1);
    kotor_fastcall_00578570(self, nullptr, object_id);
    void* data = nullptr;
    U size = 0;
    if (!kotor_fastcall_004d6440(self, nullptr, &data, &size)) return 0;
    return kotor_fastcall_00567e60(self, nullptr, recipient, 0x0c, 4, data, size);
}

// 0056FBD0..0056FC4C (127 bytes, RET 0Ch): inventory slot message (major 0Ch minor 2).
U __fastcall recovered_inventory_slot_message_0056fbd0(void* self, void*, U recipient, U object_id, U slot) {
    kotor_fastcall_004d68a0(self, nullptr, 8, 0xffffffffu, 1);
    kotor_fastcall_00578570(self, nullptr, object_id);
    kotor_fastcall_004d6340(self, nullptr, slot, 0x20);
    void* data = nullptr;
    U size = 0;
    if (!kotor_fastcall_004d6440(self, nullptr, &data, &size)) return 0;
    kotor_fastcall_00567e60(self, nullptr, recipient, 0x0c, 2, data, size);
    kotor_fastcall_004ae920(server_app(), nullptr, recipient, 0); // 0056FC32..0056FC3D
    return 1;
}

// 0056FCB0..0056FD07 (90 bytes, RET 8): inventory equip message (major 0Ch minor 6).
U __fastcall recovered_inventory_equip_message_0056fcb0(void* self, void*, U recipient, U object_id) {
    kotor_fastcall_004d68a0(self, nullptr, 4, 0xffffffffu, 1);
    kotor_fastcall_00578570(self, nullptr, object_id);
    void* data = nullptr;
    U size = 0;
    if (!kotor_fastcall_004d6440(self, nullptr, &data, &size)) return 0;
    return kotor_fastcall_00567e60(self, nullptr, recipient, 0x0c, 6, data, size);
}

// 004F0310..004F041B (270 bytes, RET 10h): update an existing equip action node.
U __fastcall recovered_action_node_clear_004f0310(void* self, void*, void* node, U arg1, U arg2, U arg3) {
    const U id = at<U>(node, 0x14);                     // 004F0315
    if (arg2 == 0) {                                    // 004F0318/004F031C
        void* round = at<void*>(self, 0x9c8);
        void* wrapper = at<void*>(round, 0x9b0);
        if (wrapper) {
            void* list = at<void*>(wrapper, 0);         // [wrapper] is not null-checked in the original
            {
                void* position = at<void*>(list, 0);
                while (position) {                      // 004F0350..004F0396
                    round = at<void*>(self, 0x9c8);     // 004F0350 re-reads the round
                    wrapper = at<void*>(round, 0x9b0);
                    list = at<void*>(wrapper, 0);
                    void* current = unresolved_list_at_pos_005e9b60(list, nullptr, position);
                    if (current && at<U8>(current, 0x10) == 6 &&
                        (at<U>(current, 0x1c) == arg1 || at<U>(current, 0x14) == id)) {
                        at<U>(current, 0x1c) = arg1;    // 004F03F1/004F03F3
                        at<U>(current, 0x14) = id;      // 004F03F5/004F03F7
                        return 1;
                    }
                    unresolved_list_next_005e9b90(list, nullptr, &position);
                }
            }
        }
    }
    void* list = at<void*>(self, 0xfc);                 // 004F0398 (no null check in the original)
    void* position = at<void*>(list, 0);
    while (position) {                                  // 004F03B0..004F03E7
        list = at<void*>(self, 0xfc);                   // 004F03B0 re-reads the list
        void* current = unresolved_list_at_pos_005e9b60(list, nullptr, position);
        if (current && at<U>(current, 0) == 8 &&
            (at<U>(current, 0x3c) == arg1 || at<U>(current, 0x38) == id)) {
            at<U>(current, 0x3c) = arg1;                // 004F0408/004F040A
            at<U>(current, 0x38) = id;                  // 004F0411/004F0413
            at<U>(current, 0x40) = arg3;                // 004F0404/004F0415
            return 1;
        }
        unresolved_list_next_005e9b90(list, nullptr, &position);
    }
    return 0;                                           // 004F03E9
}

// 004F05D0..004F06BE (241 bytes, RET 10h): update an existing unequip action node.
U __fastcall recovered_action_node_clear_unequip_004f05d0(void* self, void*, void* node, U /*arg1*/, U arg2, U arg3) {
    // Original 004F05D0 reads only arg2 (esp+18h) and arg3 (esp+1Ch);
    // arg1 is an intentional ABI argument, not a semantic input. Keep its slot,
    // omit only the unused name (no warning suppression/flag change).
    const U id = at<U>(node, 0x14);                     // 004F05D7
    if (arg2 == 0) {                                    // 004F05E0..004F05E4
        void* round = at<void*>(self, 0x9c8);
        void* wrapper = at<void*>(round, 0x9b0);
        if (wrapper) {
            void* list = at<void*>(wrapper, 0);         // [wrapper] is not null-checked in the original
            {
                void* position = at<void*>(list, 0);
                while (position) {                      // 004F0602..004F0643
                    round = at<void*>(self, 0x9c8);     // 004F0602 re-reads the round
                    wrapper = at<void*>(round, 0x9b0);
                    list = at<void*>(wrapper, 0);
                    void* current = unresolved_list_at_pos_005e9b60(list, nullptr, position);
                    if (current && at<U8>(current, 0x10) == 7 && at<U>(current, 0x14) == id) {
                        at<U>(current, 0x14) = id;      // 004F069A
                        return 1;
                    }
                    unresolved_list_next_005e9b90(list, nullptr, &position);
                }
            }
        }
    }
    void* list = at<void*>(self, 0xfc);                 // 004F0645 (no null check in the original)
    void* position = at<void*>(list, 0);
    while (position) {                                  // 004F0660..004F0690
        list = at<void*>(self, 0xfc);                   // 004F0660 re-reads the list
        void* current = unresolved_list_at_pos_005e9b60(list, nullptr, position);
        if (current && at<U>(current, 0) == 0x0b && at<U>(current, 0x38) == id) {
            at<U>(current, 0x38) = id;                  // 004F06AA/004F06AE
            at<U>(current, 0x3c) = arg3;                // 004F06B8
            return 1;
        }
        unresolved_list_next_005e9b90(list, nullptr, &position);
    }
    return 0;
}

// 004F0420..004F05C2 (421 bytes, RET 1Ch): CSWSCreature::AddEquipItemActions.
// Stack args a1..a7 = node(item), arg1(slot), arg2, arg3, arg4, arg5, arg6.
// [combat-gaps] corrected against the original: after 004F0495 the a1 stack slot holds the item's object id
// (the action parameter is &id, not &node); the slot passed to 0056FBD0/004F0310/004D3C30 and tested ==2 is
// arg1 (esp+18h after the four pushes = a2), not arg2. The add-action tail was already right: AddAction vs
// AddActionToFront tests arg2 (esp+48h-2Ch = a3) and the parameters are (3,&id),(1,&arg1),(1,&arg4).
U __fastcall recovered_object_equip_004f0420(void* self, void*, void* node, U arg1, U arg2, U arg3, U arg4, U arg5, U arg6) {
    if (at<U>(self, 0xa2c) != 0 &&               // 004F0429/004F042F
        (arg1 == 2 || arg1 == 1 || arg1 == 0x10 || arg1 == 0x20) && // 004F0438..004F044D
        node != nullptr &&                       // 004F044F/004F0451
        recovered_inventory_has_item_005a4950(word(at<U>(self, 0xa2c)), nullptr, node) != 0) // 004F0453..004F045B
        return 0;                                 // 004F045D..004F0462

    void* server = at<void*>(manager(), 8);       // 004F0468..004F046E
    void* client_object = kotor_fastcall_004ae910(server, nullptr, at<U>(self, 4)); // 004F0465..004F0473
    void* message = recovered_server_sws_message_004aedb0(at<void*>(manager(), 8), nullptr); // 004F0478..004F0483
    U id = at<U>(node, 0x14);                     // 004F0492/004F0495 stored over the a1 slot

    if (at<U>(self, 0xe8) == 0) {                 // 004F0488..004F0499
        if (client_object && message)             // 004F049B..004F04A1
            recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(client_object, 4), id, arg1); // 004F04A3..004F04AD
        return 0;                                 // 004F04B2..004F04B8
    }
    if (at<U>(self, 0x4e0) == 1 && at<U8>(self, 0xac0) == 1 && arg1 == 2) {       // 004F04BB..004F04D2
        if (client_object && message)             // 004F04D4..004F04DA
            recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(client_object, 4), id, arg1); // 004F04DC..004F04E2
        recovered_creature_feedback_004ede10(self, nullptr, 0xc1, nullptr);      // 004F04E7..004F04F0
        return 0;                                 // 004F04F5..004F04FB
    }
    if (arg6 == 0) {                              // 004F04FE..004F0508
        // 004F050A..004F0514 pushes arg4, arg3, arg1, node.
        if (recovered_action_node_clear_004f0310(self, nullptr, node, arg1, arg3, arg4) != 0) // 004F0514
            return 1;                             // 004F0519..004F051B -> 004F05BC
    }
    if (at<U>(self, 0x4e0) == 1 && at<U8>(self, 0xac0) == 1 && arg3 == 0) {       // 004F0529..004F053B
        recovered_combat_round_equip_004d3c30(at<void*>(self, 0x9c8), nullptr, id, arg1, arg4, arg5); // 004F053D..004F054F
        return 1;                                 // 004F0554..004F055A
    }
    if (arg2 == 0)                                // 004F0573/004F058F, je 004F05B7
        recovered_object_add_action_004cea20(self, nullptr, 8, 0xffffu,
            3, &id, 1, &arg1, 1, &arg4,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);   // 004F05B7
    else
        recovered_object_add_action_to_front_004ccb00(self, nullptr, 8, 0xffffu,
            3, &id, 1, &arg1, 1, &arg4,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);   // 004F05A9
    return 1;                                     // 004F05AE..004F05C2
}

// 004F06D0..004F0831 (356 bytes, RET 14h): unequip operation.
U __fastcall recovered_object_unequip_004f06d0(void* self, void*, void* node, U arg1, U arg2, U arg3, U arg4) {
    U id = at<U>(node, 0x14);                     // 004F06D8; 004F06E3 stores it over the a1 (node) slot
    void* server = at<void*>(manager(), 8);       // 004F06DD..004F06E3
    void* client_object = kotor_fastcall_004ae910(server, nullptr, at<U>(self, 4)); // 004F06ED
    void* message = recovered_server_sws_message_004aedb0(server, nullptr);        // 004F06F3..004F06FE
    if (at<U>(self, 0xe8) == 0) return 0;         // 004F0705..004F070D

    if (at<U>(self, 0x4e0) == 1 && at<U8>(self, 0xac0) == 1) {                    // 004F070F..004F071F
        void* item = recovered_equipment_item_005a4c20(word(at<U>(self, 0xa2c)), nullptr, 2); // 004F0721..004F0729
        if (item && at<U>(item, 0x14) == id) {    // 004F072E..004F0739
            if (client_object && message)
                recovered_inventory_unequip_cancel_message_0056fd80(message, nullptr, at<U>(client_object, 4), id); // 004F073B..004F074A
            recovered_creature_feedback_004ede10(self, nullptr, 0xc2, nullptr);  // 004F074F..004F0758
            return 0;                             // 004F075D
        }
    }
    if (recovered_action_node_clear_unequip_004f05d0(self, nullptr, node, arg1, arg3, arg4) != 0) // 004F0778
        return 1;                                 // 004F077D
    if (at<U>(self, 0x4e0) == 1 && at<U8>(self, 0xac0) == 1 && arg3 == 0) {       // 004F0785..004F0799
        recovered_combat_round_unequip_004d3ce0(at<void*>(self, 0x9c8), nullptr, id, arg1, arg4); // 004F079B..004F07AC
        return 1;
    }
    if (arg2 == 0)                                // 004F07F7/004F0823
        recovered_object_add_action_004cea20(self, nullptr, 0x0b, 0xffffu,
            3, &id, 3, &arg1, 1, &arg4,       // [combat-gaps] 004F07F9 lea [esp+74h] = a1 slot = id
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    else
        recovered_object_add_action_to_front_004ccb00(self, nullptr, 0x0b, 0xffffu,
            3, &id, 3, &arg1, 1, &arg4,       // [combat-gaps] 004F07F9 lea [esp+74h] = a1 slot = id
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;                                     // 004F0828..004F0831
}

// 004D3440..004D3577 (312 bytes, RET 8): advance the combat-round clocks and
// scale every queued round action's remaining time.
void __fastcall recovered_combat_round_advance_004d3440(void* self, void*, U arg0, U arg1) {
    if (at<U>(self, 0x934) == 0) return;              // 004D3443..004D344B
    const U old = at<U>(self, 0x94c);                 // 004D3451/004D345C
    const I diff = static_cast<I>(old) - static_cast<I>(arg0);
    if (diff < 0) {                                    // 004D345E/004D3461
        const U delta = arg0 - old;                    // 004D3469/004D346B
        const U room = 1000u - at<U>(self, 0x950);     // 004D346D..004D3472
        if (static_cast<I>(room) >= static_cast<I>(delta)) { // 004D3474 jl
            at<U>(self, 0x94c) = arg0 + 1;             // 004D3478..004D3481
            // 004D3487 re-stores self+950 unchanged.
        } else if (arg1 != 0) {                        // 004D3493..004D3499
            at<U>(self, 0x94c) = arg0;                 // 004D34A1
            // 004D34A7 re-stores self+954 unchanged.
        }
    }
    void* wrapper = at<void*>(self, 0x9b0);
    void* list = at<void*>(wrapper, 0);
    void* position = at<void*>(list, 0);
    while (position) {                                 // 004D34AD..004D3515
        wrapper = at<void*>(self, 0x9b0);              // 004D34C0 re-reads the wrapper
        list = at<void*>(wrapper, 0);
        void* node = unresolved_list_at_pos_005e9b60(list, nullptr, position);
        const I current = at<I>(self, 0x94c);          // 004D34D0
        if (current > 0) {                             // 004D34D6/004D34DC
            double value = static_cast<double>(at<I>(node, 0));
            value /= static_cast<double>(current);     // fidiv dword
            value *= static_cast<double>(current - static_cast<I>(arg0)); // fimul dword
            at<I>(node, 0) = clear9::ftol2_low(value); // __ftol2 low dword
        } else {
            at<U>(node, 0) = 0;                        // 004D34F7
        }
        unresolved_list_next_005e9b90(list, nullptr, &position);
    }
    U current = at<U>(self, 0x94c);                    // 004D3517
    double ratio;
    if (current == 0) ratio = 1.0;                     // 0073D764 float 1.0
    else ratio = static_cast<double>(at<I>(self, 0x944)) / static_cast<double>(static_cast<I>(current));
    const I delta = static_cast<I>(current) - static_cast<I>(arg0);
    at<U>(self, 0x944) = static_cast<U>(clear9::ftol2_low(ratio * static_cast<double>(delta)));
    if (delta < 0) {                                   // 004D354A/004D3552
        at<U>(self, 0x94c) = arg0;                     // 004D355A
        // 004D3560 re-stores self+950 unchanged.
    }
    at<U>(self, 0x94c) = at<U>(self, 0x94c) - arg0;    // 004D3566..004D356F
}

// 004D3C30..004D3CDD (176 bytes, RET 10h): queue a round equip action (type 6).
void __fastcall recovered_combat_round_equip_004d3c30(void* self, void*, U item_id, U arg1, U arg2, U arg3) {
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id); // 004D3C30..004D3C41
    if (!item) return;                                  // 004D3C46/004D3C48
    recovered_combat_round_advance_004d3440(self, nullptr, 0x5dc, 1); // 004D3C4E..004D3C57 push 1; push 5DCh -> (5DCh, 1) [combat-gaps]
    void* memory = live_operator_new(0x88);             // 004D3C5C
    void* node = nullptr;
    if (memory) {                                       // 004D3C68..004D3C7D
        at<U>(memory, 0x38) = 0;
        at<U>(memory, 0x3c) = 0;
        at<U>(memory, 0x40) = 0;
        kotor_fastcall_004d1d50(memory, nullptr);        // 004D3C84, returns 0; caller keeps ECX
        node = memory;                                   // 004D3C89 mov eax,ecx
    }
    at<U>(node, 0x84) = arg3;                           // 004D3C8F/004D3C93/004D3C97
    at<U8>(node, 0x10) = 6;                             // 004D3CA1
    at<U>(node, 0) = 0;                                 // 004D3CA5
    at<U>(node, 8) = 0x5dc;                             // 004D3CAB
    at<U>(node, 0x14) = item_id;                        // 004D3CB2
    at<U>(node, 0x1c) = arg1;                           // 004D3CB5
    at<U>(node, 0x54) = arg2;                           // 004D3CB8
    // [combat-gaps] 004D3CBB..004D3CC3: edx=[self+9B0] (wrapper), ecx=[wrapper] (CExoLinkedListInternal), edx=[ecx] head.
    void* list = at<void*>(at<void*>(self, 0x9b0), 0);  // 004D3CBB/004D3CC1
    void* head = at<void*>(list, 0);                    // 004D3CC3
    if (head) recovered_event_list_before_005e9ad0(list, nullptr, node, head); // 004D3CC9..004D3CCB
    else resource_append_005e9dc0(list, nullptr, node); // 004D3CD6
}

// 004D3CE0..004D3D93 (182 bytes, RET 0Ch): queue a round unequip action (type 7).
void __fastcall recovered_combat_round_unequip_004d3ce0(void* self, void*, U item_id, U arg1, U arg2) {
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id); // 004D3CE0..004D3CF1
    // [combat-gaps] 004D3D06 test al,al / je 004D3D91: return only when CanUnEquipWeapon's AL is zero
    // (it returns 1 or 2, so the original never takes this exit; a null item falls through to queue).
    if (item && (recovered_creature_can_unequip_weapon_00513e30(at<void*>(self, 0x9b4), nullptr, item) & 0xffu) == 0) // 004D3CFA..004D3D08
        return;
    recovered_combat_round_advance_004d3440(self, nullptr, 0x5dc, 1); // 004D3D0E..004D3D17 push 1; push 5DCh -> (5DCh, 1) [combat-gaps]
    void* memory = live_operator_new(0x88);             // 004D3D1C
    void* node = nullptr;
    if (memory) {                                       // 004D3D2B..004D3D3D
        at<U>(memory, 0x38) = 0;
        at<U>(memory, 0x3c) = 0;
        at<U>(memory, 0x40) = 0;
        kotor_fastcall_004d1d50(memory, nullptr);        // 004D3D44, returns 0; caller keeps ECX
        node = memory;                                   // 004D3D49 mov eax,ecx
    }
    at<U8>(node, 0x10) = 7;                             // 004D3D57
    at<U>(node, 0) = 0;                                 // 004D3D5B
    at<U>(node, 8) = 0x5dc;                             // 004D3D61
    at<U>(node, 0x14) = item_id;                        // 004D3D68
    at<U>(node, 0x20) = arg1;                           // 004D3D6B
    at<U>(node, 0x54) = arg2;                           // 004D3D6E
    // [combat-gaps] 004D3D71..004D3D79: ecx=[[self+9B0]] (CExoLinkedListInternal), edx=[ecx] head.
    void* list = at<void*>(at<void*>(self, 0x9b0), 0);  // 004D3D71/004D3D77
    void* head = at<void*>(list, 0);                    // 004D3D79
    if (head) recovered_event_list_before_005e9ad0(list, nullptr, node, head); // 004D3D81
    else resource_append_005e9dc0(list, nullptr, node); // 004D3D8C
}

// 004F7530..004F764B (284 bytes, RET): clear the combat actions of a round.
void __fastcall recovered_combat_round_cleanup_004f7530(void* self, void*) {
    void* round = at<void*>(self, 0x9c8);               // 004F7536
    if (!recovered_combat_round_has_current_action_004d3580(round, nullptr)) return; // 004F753C..004F7543
    void* wrapper = at<void*>(round, 0x9b0);            // 004F7549/004F754F
    if (!wrapper) return;                               // 004F7557/004F755B
    // [combat-gaps] 004F7561..004F7563: the walk starts at the TAIL ([[wrapper]+4]) and steps with
    // GetPrev (004F760D call 005E9BD0); the original never tests the GetAtPos result (004F757D re-tests the position).
    void* position = at<void*>(at<void*>(wrapper, 0), 4); // 004F7561/004F7563
    if (!position) return;                              // 004F7566..004F756C
    void* item = unresolved_list_at_pos_005e9b60(at<void*>(wrapper, 0), nullptr, position); // 004F7572..004F7576
    // 004F757D..004F7583 re-tests the (nonzero) position, not the item.
    for (;;) {
        const U8 kind = at<U8>(item, 0x10);             // 004F7590
        void* remove_position = nullptr;
        void* remove_item = nullptr;
        if (kind == 6) {                                // 004F7597..004F7599
            void* object = recovered_item_lookup_004ae760(server_app(), nullptr, at<U>(item, 0x14)); // 004F759B..004F75A7
            if (object) {                               // 004F75AC/004F75AE
                recovered_object_equip_004f0420(self, nullptr, object, at<U>(item, 0x1c), 1, 1,
                    at<U>(item, 0x54), at<U>(item, 0x84), 0); // 004F75B0..004F75C7
                remove_position = position;             // 004F75FC
                remove_item = item;                     // 004F7600
            }
        } else if (kind == 7) {                         // 004F75CE..004F75D0
            void* object = recovered_item_lookup_004ae760(server_app(), nullptr, at<U>(item, 0x14)); // 004F75D2..004F75DF
            if (object) {                               // 004F75E4/004F75E6
                recovered_object_unequip_004f06d0(self, nullptr, object, at<U>(item, 0x20), 1, 1,
                    at<U>(item, 0x54));                 // 004F75E8..004F75F7
                remove_position = position;
                remove_item = item;
            }
        }
        void* next_item = kotor_fastcall_005e9bd0(at<void*>(wrapper, 0), nullptr, &position); // 004F7602..004F760D GetPrev
        if (remove_position) {                          // 004F7612..004F7616
            c2da_005e9c10(at<void*>(wrapper, 0), nullptr, remove_position); // 004F7618..004F761F
            if (remove_item) {
                kotor_m15e_slot090_void0_004015a0(remove_item, nullptr); // 004F7628..004F762A
                live_free(remove_item);                 // 004F762F/004F7630
            }
        }
        if (!position) break;                           // 004F7638..004F763E
        item = next_item;                               // 004F7614 esi = 005E9BD0 return
    }
}

// 004F3690..004F376C (223 bytes, RET 4): stop the current action and (when the
// action counter is set) queue the 0x23 feedback event.
void __fastcall recovered_creature_feedback_cleanup_004f3690(void* self, void*, U arg0) {
    vcall<U>(self, 0x7c, 0x2710);                      // 004F36AA..004F36B1
    recovered_creature_feedback_004ede10(self, nullptr, arg0, nullptr); // 004F36B4..004F36BD
    if ((at<U8>(self, 0xa00) & 8) == 0)                // 004F36C2..004F36C9
        at<U>(self, 0x9fc) &= ~8u;                     // 004F36CB
    if (at<U>(self, 0x9d4) != 0) {                     // 004F36D2..004F36DA
        void* server = server_app();
        void* module = server_get_module_004ae6b0(server, nullptr); // 004F36DC..004F36E5
        void* memory = live_operator_new(0x34);        // 004F36EA..004F36EE
        void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 004F36F6..004F3708
        at<U16>(event, 0) = 0x23;                      // 004F3721
        recovered_event_integer_004d76b0(event, nullptr, 0, 3); // 004F3713..004F3726
        const U target = at<U>(module, 0x20);          // 004F372B
        const U object_id = at<U>(self, 4);            // 004F372E
        void* ai = at<void*>(at<void*>(server, 4), 0x10044); // inline view of 004AED80
        recovered_event_insert_004afdb0(ai, nullptr, 0, 0, object_id, target, 10, event); // 004F373A..004F374F
        recovered_creature_cancel_action_004ef4d0(self, nullptr); // 004F3754/004F3756
    }
}

// 004FAB00..004FAF16 (1051 bytes, RET 8): queued action callback (vtable +74).
U __fastcall recovered_object_action_queue_callback_004fab00(void* self, void*, void* action, U mode) {
    if (!at<U>(action, 0x70)) return 0;                // 004FAB06..004FAB17
    void* server = server_app();
    void* message = recovered_server_sws_message_004aedb0(server, nullptr); // 004FAB1A..004FAB23
    void* client_object = kotor_fastcall_004b2a50(at<void*>(server, 4), nullptr, at<U>(self, 4)); // 004FAB28..004FAB3E
    const U type = at<U>(action, 0);
    if (client_object && message) {                     // 004FAB43..004FAB49
        switch (type) {
        case 8:  recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(client_object, 4),
                     at<U>(action, 0x38), at<U>(action, 0x3c)); break; // 004FAB4B..004FAB66
        case 7:  recovered_inventory_equip_message_0056fcb0(message, nullptr, at<U>(client_object, 4),
                     at<U>(action, 0x38)); break;                       // 004FAB68..004FAB7C
        case 9:  recovered_inventory_item_message_0056fb70(message, nullptr, at<U>(client_object, 4),
                     at<U>(action, 0x38)); break;                       // 004FAB7E..004FAB92
        case 0x0b: recovered_inventory_unequip_cancel_message_0056fd80(message, nullptr,
                     at<U>(client_object, 4), at<U>(action, 0x38)); break; // 004FAB94..004FABA3
        default: break;
        }
    }
    if (type == 0x2a) {                                // 004FABA8..004FABB9
        recovered_creature_feedback_cleanup_004f3690(self, nullptr, 0x14);
    } else if (type == 0x0c) {                         // 004FABBE..004FABE7
        at<U>(self, 0x504) = 0x7f000000u;
        at<U>(self, 0x50c) = 0x7f000000u;
        vcall<U>(self, 0x7c, 0x2710);
        recovered_combat_round_cleanup_004f7530(self, nullptr);
    } else if (type == 1) {                            // 004FABEC..004FAC12
        void* object = at<void*>(self, 0x340);
        at<U>(object, 0x30) = 0x7f000000u;
        at<U>(object, 0x38) = 0x7f000000u;
        at<U>(self, 0xa8c) = 1;
    } else if (type == 0x0f) {                         // 004FAC17..004FAC51
        at<U>(self, 0x524) = 0x7f000000u;
        at<U>(self, 0x528) = 0x7f000000u;
        recovered_combat_round_clear_current_004d3ba0(at<void*>(self, 0x9c8), nullptr);
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x960) = 0;
        recovered_combat_round_cleanup_004f7530(self, nullptr);
    } else if (type == 0x2e) {                         // 004FAC56..004FAC90
        at<U>(self, 0x524) = 0x7f000000u;
        at<U>(self, 0x528) = 0x7f000000u;
        recovered_combat_round_clear_current_004d3ba0(at<void*>(self, 0x9c8), nullptr);
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x96c) = 0;
        recovered_combat_round_cleanup_004f7530(self, nullptr);
    } else if (type == 0x32) {                         // 004FAC95..004FACA4
        w5_creature_set_combat_mode_0050ee80(self, nullptr, 0, 1);
    } else if (type == 9) {                            // 004FACA9..004FACC0
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x988) = 0;
    } else if (type == 7) {                            // 004FACC5..004FACDC
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x98c) = 0;
    } else if (type == 0x1d || type == 0x19 || type == 0x1a || type == 0x1c || type == 0x1b) { // 004FACE1..004FAD08
        vcall<U>(self, 0x7c, 0x2710);
        recovered_creature_cancel_action_004ef4d0(self, nullptr);
        at<U>(self, 0x97c) = 0;
    } else if (type == 0x36) {                         // 004FAD0E..004FAD2B
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x970) = 0;
        at<U>(self, 0x974) = 0;
    } else if (type == 0x38) {                         // 004FAD30..004FAD47
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x978) = 0;
    } else if (type == 0x26) {                         // 004FAD4C..004FAD6A
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x980) = 0;
        recovered_creature_cancel_action_004ef4d0(self, nullptr);
    } else if (type == 0x27) {                         // 004FAD6F..004FAD8D
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x984) = 0;
        recovered_creature_cancel_action_004ef4d0(self, nullptr);
    } else if (type == 0x14) {                         // 004FAD8F..004FADA6
        vcall<U>(self, 0x7c, 0x2710);
        at<U>(self, 0x1f0) = 0;
    } else if (type == 0x3d) {                         // 004FADA8..004FAE08
        if (at<U>(self, 0xa88) != 0) {
            void* client = client_app();
            const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(self, 4));
            void* party = kotor_fastcall_005ed8b0(client, nullptr);
            const I index = recovered_party_index_006349c0(party, nullptr, word(client_id));
            if (index >= 0 && index <= 2) {             // 004FADE3..004FADEA jl/jg
                void* party2 = kotor_fastcall_005ed8b0(client_app(), nullptr);
                kotor_game_prepare::ResetPartyMemberData004eae80(
                    static_cast<unsigned char*>(party2) + index * 0x88 + 0x24, nullptr); // 004FADFF..004FAE03
            }
        }
    } else if (type == 0x18) {                         // 004FAE72..004FAE8D
        void* gui = frame_R4_005ed690(client_app(), nullptr);
        kotor_fastcall_0062ec60(gui, nullptr, 0);
    } else if (type == 0x20) {                         // 004FAE92..004FAE9A
        at<U>(self, 0x50) = 0;
    } else if (type == 0x28) {                         // 004FAE9F..004FAEF8
        void* placeable = recovered_placeable_lookup_004ae7a0(server, nullptr, at<U>(action, 0x38));
        if (placeable && at<U>(placeable, 0x450) != 0) {
            const U d4 = at<U>(placeable, 0xd4);
            at<U>(placeable, 0x450) = 0;
            if (d4 == 0x275b) vcall<U>(placeable, 0x7c, 0x275c);
            at<U>(placeable, 0xd8) = 0x3f800000u;
        }
    }

    // 004FAE0C..004FAE6F tail.
    if (message && type == 1 && mode != 0)             // 004FAE0C..004FAE1C
        vcall<U>(self, 0x7c, 0x2710);
    if (vcall<U>(self, 0xc4) != invalid_object()) {     // 004FAE2A..004FAE3A
        vcall<U>(self, 0xc4);                           // 004FAE3C..004FAE40
        w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, invalid_object(), 0); // 004FAE4C..004FAE50
    }
    if (mode == 1) {                                   // 004FAE55..004FAE61
        at<U>(self, 0xbc) = 0;
        at<U>(self, 0xc0) = 0;
    }
    return 1;                                          // 004FAE69
}

}  // extern "C"
