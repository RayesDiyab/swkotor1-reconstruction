// Lane p2s-a: CSWSMessage::HandlePlayerToServerGoldMessage 00523B20 and HandlePlayerToServerInventoryMessage 00523C20,
// plus the callees with no implementation in the c16 candidate (TransferGold, AddGiveItemActions, AddDropItemActions,
// AddPickUpItemActions, SendServerToPlayerInventory_LearnScroll).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are the original instructions.
// Handler ABI: original BOOL __thiscall CSWSMessage::HandlePlayerToServer<X>(CSWSPlayer* player, BYTE minor), RET 8,
// exported as the 0918 dispatcher calls handlers: extern "C" unsigned __fastcall f(void* message, void*, void* player,
// unsigned minor) (@16). Conventions as W5 (w5_types.hpp / w5_server_env.hpp): thiscall -> __fastcall(self, unused_edx,
// ...); x87 at 53-bit precision -> double intermediates, float where the original stores a dword; live_operator_new /
// live_free only.
// EH: 00523C20 and 004FD730 register SEH frames; their handlers only run when a callee throws (none of the bound
// callees throws in normal operation). The unwind cleanup is not reproduced.
#include "58f963adf7-w5_types.hpp"
#include "294ff121b7-w5_server_env.hpp"

using namespace w5;

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); } // CServerExoApp::GetPartyTable [0575]
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
namespace frame_control { extern unsigned message_manager_007a3a00; }                    // .data 007A3A00 g_pVirtualMachine [0456]

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                   // [007A3A28] CSWRules*
// ---------------- imports: existing providers in the c16 candidate (exact names) ----------------
unsigned char __fastcall kotor_fastcall_004d6720(void* message, void*, unsigned bits);   // CSWMessage::ReadBYTE
unsigned __fastcall kotor_fastcall_004d6230(void* message, void*, unsigned);              // MessageReadOverflow
unsigned __fastcall kotor_fastcall_004d6240(void* message, void*, unsigned);              // MessageReadUnderflow
unsigned __fastcall kotor_fastcall_004d66f0(void* message, void*);                        // ReadBOOL
unsigned __fastcall kotor_fastcall_004d6780(void* message, void*, unsigned bits);         // ReadDWORD
float __fastcall kotor_fastcall_004d67b0(void* message, void*, unsigned scale_bits, unsigned bits); // ReadFLOAT (ST0)
unsigned __fastcall kotor_fastcall_00578540(void* message, void*);                        // CSWSMessage::ReadOBJECTIDServer
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);          // CreateWriteMessage
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                      // WriteOBJECTIDServer
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);         // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
void* __fastcall kotor_fastcall_00561080(void* player, void*);                            // CSWSPlayer::GetGameObject
void* __fastcall recovered_object_array_004aed70(void* server, void*);                    // CServerExoApp::GetObjectArray
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);              // CServerExoApp::GetSWSMessage
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);               // GetItemByGameObjectID
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // CSWBaseItemArray::GetBaseItem
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U flag);        // CSWSObject::ClearAllActions
void __fastcall recovered_combat_round_remove_all_actions_004d3770(void* round, void*);   // CSWSCombatRound::RemoveAllActions
U __fastcall recovered_object_equip_004f0420(void* creature, void*, void* item, U slot, U, U, U, U, U); // AddEquipItemActions
U __fastcall recovered_object_unequip_004f06d0(void* creature, void*, void* item, U, U, U, U); // AddUnequipActions
U __fastcall kotor_fastcall_004fde40(void* creature, void*, U, U, U, U, U, U, U, U, U, U, U, U, U, U, U); // AddAttackActions
U __fastcall recovered_inventory_slot_message_0056fbd0(void* message, void*, U recipient, U id, U slot); // ..._EquipCancel
U __fastcall recovered_inventory_item_message_0056fb70(void* message, void*, U player_id, U item_id);   // ..._DropCancel
U __fastcall recovered_inventory_equip_message_0056fcb0(void* message, void*, U player_id, U item_id);  // ..._PickupCancel
U __fastcall recovered_inventory_unequip_cancel_message_0056fd80(void* message, void*, U player_id, U a); // ..._UnequipCancel
U __fastcall recovered_inventory_has_item_005a4950(void* inventory, void*, void* item);   // CSWInventory::GetItemInInventory
U __fastcall recovered_inventory_slot_from_item_005a4980(void* inventory, void*, void* item); // CSWInventory::GetSlotFromItem
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U slot);       // CSWInventory::GetItemInSlot
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U flag);    // CSWSCreature::GetItemRepository
U __fastcall recovered_item_in_repository_00555f40(void* repository, void*, void* item, U recurse); // GetItemInRepository
void __fastcall w5_creature_action_manager_004f8770(void* creature, void*, U mode);      // CSWSCreature::ActionManager
U __fastcall aurora_return_false_4_0063e7c0(void* self, void*, U);                       // 0063E7C0 return_false_4
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);     // CExoString(char const*)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                        // ~CExoString
void __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U id, U flag);           // CVirtualMachine::RunScript
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data); // SendFeedbackMessage
void* __fastcall recovered_item_construct_005530a0(void* self, void*, U id);              // CSWSItem::CSWSItem
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);           // CSWSItem::LoadFromTemplate
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c); // CSWSItem::SetPossessor
unsigned __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // CItemRepository::AddItem
void __fastcall kotor_fastcall_004f3ed0(void* creature, void*, U amount, U feedback);     // CSWSCreature::RemoveGold
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);                                                             // CSWSObject::AddAction (W1)

// ---------------- exports ----------------
U __fastcall kotor_fastcall_004f0840(void* creature, void*, U item_id, Vector* position, U flag);
U __fastcall kotor_fastcall_004eb620(void* creature, void*, U item_id, U a2, U a3);
U __fastcall kotor_fastcall_0057c870(void* object, void*, U item_id, U target_id, I count, U a4);
U __fastcall kotor_fastcall_004fd730(void* creature, void*, U target_id, Vector* position, U amount);
U __fastcall kotor_fastcall_0056b240(void* message, void*, U player_id, U id, U minor);
U __fastcall recovered_player_to_server_gold_00523b20(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_inventory_00523c20(void* message, void*, void* player, U minor);
}  // extern "C"

namespace {
constexpr U k_invalid_00746644 = 0x7f000000u;          // .rdata 00746644
constexpr U k_one_0x3f800000 = 0x3f800000u;            // 1.0f immediate (ReadFLOAT scale)
const float k_drop_lift_0073e798 = 0.1f;               // .rdata 0073E798
const char k_credits_007471c0[] = "g_i_credits001";    // .rdata 007471C0
const char k_itemuse_00748028[] = "itemuse";           // .rdata 00748028
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* base_item(void* item) {                    // GetBaseItem(rules+34, item+C)
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr, at<int>(item, 0xc));
}
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }       // vtbl+10 AsSWSObject
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }     // vtbl+30 AsSWSCreature
inline void* as_item(void* o) { return vcall<void*>(o, 0x38); }         // vtbl+38 AsSWSItem
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }    // vtbl+48 AsSWSPlaceable
inline void* message_of_server() { return recovered_server_sws_message_004aedb0(server_app(), nullptr); }
inline bool lookup(U id, void** out) {                  // GetObjectArray()->GetGameObject; AL == [0074666C] (0)
    return static_cast<unsigned char>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr),
                                                              nullptr, id, out)) == 0;
}
}  // namespace

extern "C" {
// ---------------------------------------------------------------------------------------------------------
// 004F0840 CSWSCreature::AddDropItemActions(item id, Vector* position, flag). RET 0Ch, EAX 0/1.
U __fastcall kotor_fastcall_004f0840(void* creature, void*, U item_id, Vector* position, U flag) {
    if (recovered_item_lookup_004ae760(server_app(), nullptr, item_id) == nullptr) return 0; // 004F0840..57
    if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return 0;           // 004F0859..62
    if (at<U>(creature, 0x4e0) == 1) {                                                    // 004F0864..6B
        void* slot2 = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 2); // 004F086D..75
        if (slot2 != nullptr && at<U>(slot2, 0x14) == item_id) {                          // 004F087A..85
            recovered_creature_feedback_004ede10(creature, nullptr, 0xc2, nullptr);       // 004F0887..90
            return 0;                                                                     // 004F0895
        }
    }
    if (at<U>(creature, 0xe8) == 0) return 0;                                             // 004F089B..A3
    recovered_object_add_action_004cea20(creature, nullptr, 9, 0xffff,                    // 004F08A5..EF
        3, &item_id, 2, &position->x, 2, &position->y, 2, &position->z, 1, &flag,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;                                                                             // 004F08F4
}

// 004EB620 CSWSCreature::AddPickUpItemActions(item id, a2, a3). RET 0Ch, EAX 0/1.
U __fastcall kotor_fastcall_004eb620(void* creature, void*, U item_id, U a2, U a3) {
    if (recovered_item_lookup_004ae760(server_app(), nullptr, item_id) == nullptr) return 0; // 004EB620..37
    if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return 0;           // 004EB639..42
    if (at<U>(creature, 0xe8) == 0) return 0;                                             // 004EB644..4C
    recovered_object_add_action_004cea20(creature, nullptr, 7, 0xffff,                    // 004EB64E..94
        3, &item_id, 3, &a2, 1, &a3,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;                                                                             // 004EB699
}

// 0057C870 CSWSObject::AddGiveItemActions(item id, target id, int count, a4). RET 10h, EAX 0/1.
U __fastcall kotor_fastcall_0057c870(void* object, void*, U item_id, U target_id, I count, U a4) {
    void* target = kotor_fastcall_004ae750(server_app(), nullptr, target_id);             // 0057C870..81
    if (target == nullptr || as_object(target) == nullptr) return 0;                       // 0057C886..95
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);           // 0057C89C..AF
    if (item == nullptr) return 0;                                                        // 0057C8B1..B3 -> 0057C971
    void* self_creature = as_creature(object);                                            // 0057C8B9..C9
    void* possessor = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(item, 0x268));  // 0057C8C0..D2
    const bool party_pair = possessor != nullptr && self_creature != nullptr &&
                            at<U>(possessor, 0xa88) != 0 && at<U>(self_creature, 0xa88) != 0; // 0057C8D7..F1
    if (!party_pair && at<U>(item, 0x268) != at<U>(object, 4)) return 0;                  // 0057C8F3..FC
    if (at<U>(object, 0xe8) == 0) return 0;                                               // 0057C8FE..06
    const I stack = at<I>(item, 0x28c);                                                   // 0057C908
    if (count > stack) count = stack;                                                     // 0057C90E..14 (signed jle)
    recovered_object_add_action_004cea20(object, nullptr, 0x22, 0xfffe,                   // 0057C918..61
        3, &item_id, 3, &target_id, 1, &count, 1, &a4,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;                                                                             // 0057C968
}

// 004FD730 CSWSCreature::TransferGold(target id, Vector* drop position, amount). RET 0Ch, EAX 0/1.
// Builds a g_i_credits001 item stack of min(amount, gold) owned by the creature, then: INVALID target -> drop
// actions; creature target -> into own repository + RemoveGold; placeable/item container -> give actions.
U __fastcall kotor_fastcall_004fd730(void* creature, void*, U target_id, Vector* position, U amount) {
    if (amount == 0) return 0;                                                            // 004FD749..66 (ja: unsigned)
    U gold = at<U>(creature, 0xa88) != 0                                                  // 004FD769..71
        ? at<U>(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), 0xfc) // 004FD773..80
        : at<U>(creature, 0x9d0);                                                         // 004FD788
    if (amount > gold) {                                                                  // 004FD78E..90 (jbe)
        amount = at<U>(creature, 0xa88) != 0                                              // 004FD792..B2 (re-read)
            ? at<U>(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), 0xfc)
            : at<U>(creature, 0x9d0);
    }
    void* memory = live_operator_new(0x29c);                                              // 004FD7B8..C5
    void* item = memory != nullptr ? recovered_item_construct_005530a0(memory, nullptr, k_invalid_00746644) : nullptr; // 004FD7C9..E5
    menu_resources::Ref resref;                                                           // [esp+14]
    resref.construct_00406d80(k_credits_007471c0);                                        // 004FD7E7..FC
    recovered_item_template_005608b0(item, nullptr, &resref);                             // 004FD801..0A
    at<U>(item, 0x28c) = amount;                                                          // 004FD80F..1A (stack size)
    recovered_item_owner_00553210(item, nullptr, at<U>(creature, 4), 0, 0, k_invalid_00746644); // 004FD813..2A
    if (item == nullptr) return 0;                                                        // 004FD82F..35 (checked after use)
    if (target_id == k_invalid_00746644) {                                                // 004FD83B..45 -> 004FD916
        kotor_fastcall_004f0840(creature, nullptr, at<U>(item, 0x14), position, 1);       // 004FD916..23
        return 1;
    }
    void* target = kotor_fastcall_004ae750(server_app(), nullptr, target_id);             // 004FD84B..55
    if (target != nullptr) {                                                              // 004FD85A..5E
        if (as_creature(target) != nullptr) {                                             // 004FD864..6D
            recovered_inventory_insert_0055d330(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr,
                                                &item, 1, 0, 1);                          // 004FD86F..85
            kotor_fastcall_004f3ed0(creature, nullptr, amount, 1);                        // 004FD88A..8F
            return 1;
        }
        if (as_placeable(target) != nullptr) {                                            // 004FD89E..AB
            if (at<U>(as_placeable(target), 0x260) == 0) {                                // 004FD8AD..B8
                kotor_fastcall_0057c870(creature, nullptr, at<U>(item, 0x14), target_id, -1, 0); // 004FD8BA..C9
                return 1;
            }
        } else if (as_item(target) != nullptr) {                                          // 004FD8D5..DA
            void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr,
                                                                           at<int>(as_item(target), 0xc)); // 004FD8DC..EF
            if (at<U>(base, 0x70) != 0) {                                                 // 004FD8F4..F9
                kotor_fastcall_0057c870(creature, nullptr, at<U>(item, 0x14), target_id, -1, 0); // 004FD8FB..0A
                return 1;
            }
        }
    }
    vcall<void*>(item, 0, 1u);                                                            // 004FD92F..3B deleting dtor
    return 0;                                                                             // 004FD93D
}

// 0056B240 CSWSMessage::SendServerToPlayerInventory_LearnScroll(player id, object id, minor). RET 0Ch, EAX 0/1.
U __fastcall kotor_fastcall_0056b240(void* message, void*, U player_id, U id, U minor) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                         // 0056B244..4C
    kotor_fastcall_00578570(message, nullptr, id);                                        // 0056B251..58
    void* data = nullptr; U size = 0;                                                     // [esp+8] / [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;           // 0056B25D..76
    kotor_fastcall_00567e60(message, nullptr, player_id, 0xc, minor, data, size);         // 0056B279..91
    return 1;                                                                             // 0056B296
}

// ---------------------------------------------------------------------------------------------------------
// 00523B20 CSWSMessage::HandlePlayerToServerGoldMessage(CSWSPlayer* player, BYTE minor). RET 8.
// Quirks kept: always returns 0 (also after a successful transfer); when the player has no creature object the
// receiver of TransferGold is the incoming minor dword itself ([esp+20] at 00523B65); the drop position is only read
// when the target is OBJECT_INVALID and is otherwise {0,0,0}; the overflow check after the floats runs twice.
U __fastcall recovered_player_to_server_gold_00523b20(void* message, void*, void* player, U minor) {
    Vector position;                                                                      // [esp+C]..[esp+14]
    position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;                              // 00523B2C..3C
    void* object = kotor_fastcall_00561080(player, nullptr);                              // 00523B27..44
    void* creature;
    if (object != nullptr && as_creature(object) != nullptr) creature = as_creature(object); // 00523B4B..63
    else creature = reinterpret_cast<void*>(static_cast<std::uintptr_t>(minor));          // 00523B65
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                      // 00523B69..74
    if (static_cast<unsigned char>(minor) != 1) return 0;                                 // 00523B7A..81
    const U target = kotor_fastcall_00578540(message, nullptr);                           // 00523B87..89
    const U amount = kotor_fastcall_004d6780(message, nullptr, 0x20);                     // 00523B8E..94
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                      // 00523B99..A6
    if (target == k_invalid_00746644) {                                                   // 00523BA8..AE
        position.x = kotor_fastcall_004d67b0(message, nullptr, k_one_0x3f800000, 0x20);   // 00523BB0..BE
        position.y = kotor_fastcall_004d67b0(message, nullptr, k_one_0x3f800000, 0x20);   // 00523BC2..D0
        position.z = kotor_fastcall_004d67b0(message, nullptr, k_one_0x3f800000, 0x20);   // 00523BD4..E2
    }
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                      // 00523BE6..F1
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                      // 00523BF3..FE
    kotor_fastcall_004fd730(creature, nullptr, target, &position, amount);                // 00523C00..09
    return 0;                                                                             // 00523C0E..17
}

// ---------------------------------------------------------------------------------------------------------
// 00523C20 CSWSMessage::HandlePlayerToServerInventoryMessage(CSWSPlayer* player, BYTE minor). RET 8.
// Jump table 005244A8 on minor-1 (13 entries): 1 equip, 3 drop, 5 pick up, 7 unequip, 11 swap/equip pair,
// 12 itemuse script, 13 learn scroll; 2/4/6/8/9/10 and minor 0 / >13 return 0.
U __fastcall recovered_player_to_server_inventory_00523c20(void* message, void*, void* player, U minor) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                      // 00523C3C..4B
    const U8 case_minor = static_cast<U8>(minor);                                         // 00523C62
    void* creature;
    void* object;                                                                          // [esp+10]
    switch (case_minor) {
    case 1: {                                                                             // 00523C7A equip
        const U item_id = kotor_fastcall_00578540(message, nullptr);                      // 00523C7A..7C (ebx)
        const U slot = kotor_fastcall_004d6780(message, nullptr, 0x20);                   // 00523C81..87 (edi)
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 00523C8C..99
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 00523C9F..AA
        creature = as_creature(kotor_fastcall_00561080(player, nullptr));                 // 00523CB0..C2
        if (creature != nullptr && lookup(item_id, &object) && as_item(object) != nullptr && // 00523CC4..FD
            (at<U>(base_item(as_item(object)), 4) & slot) != 0) {                         // 00523D03..20
            w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                   // 00523D22..26
            recovered_combat_round_remove_all_actions_004d3770(at<void*>(creature, 0x9c8), nullptr); // 00523D2B..31
            recovered_object_equip_004f0420(creature, nullptr, as_item(object), slot, 0, 1, 0, 1, 0); // 00523D36..4D
            goto resume_attack;                                                           // 00523D52
        }
        recovered_inventory_slot_message_0056fbd0(message_of_server(), nullptr, at<U>(player, 4), item_id, slot); // 00523D96..AC
        return 0;                                                                         // 00523DB1
    }
    case 3: {                                                                             // 005242F4 drop
        const U item_id = kotor_fastcall_00578540(message, nullptr);                      // 005242F4..F6 (edi)
        const float x = kotor_fastcall_004d67b0(message, nullptr, k_one_0x3f800000, 0x20); // 005242FB..0B [esp+4C]
        const float y = kotor_fastcall_004d67b0(message, nullptr, k_one_0x3f800000, 0x20); // 0052430F..1D [esp+28]
        const float z = kotor_fastcall_004d67b0(message, nullptr, k_one_0x3f800000, 0x20); // 00524321..2F [esp+24]
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 00524333..3E
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 00524344..4F
        Vector position;                                                                  // [esp+2C]
        position.z = static_cast<float>(static_cast<double>(z) + static_cast<double>(k_drop_lift_0073e798)); // 00524355..6F
        position.y = y;                                                                   // 00524359 / 0052436B
        position.x = x;                                                                   // 00524363 / 00524375
        creature = as_creature(kotor_fastcall_00561080(player, nullptr));                 // 00524367..85
        if (creature != nullptr) {                                                        // 00524387..89
            w5_creature_action_manager_004f8770(creature, nullptr, 2);                    // 0052438B..8F
            kotor_fastcall_004f0840(creature, nullptr, item_id, &position, 0);            // 00524394..9E
            return 1;                                                                     // 005243A3
        }
        recovered_inventory_item_message_0056fb70(message_of_server(), nullptr, at<U>(player, 4), item_id); // 005243AD..C2
        return 0;
    }
    case 5: {                                                                             // 00524190 pick up
        const U item_id = kotor_fastcall_00578540(message, nullptr);                      // 00524190..92 (edi)
        const U second = kotor_fastcall_00578540(message, nullptr);                       // 00524197..9B (ebx)
        const U8 b = kotor_fastcall_004d6720(message, nullptr, 8);                        // 005241A0..A6
        // 005241AF: the byte is stored into the low byte of the incoming minor dword ([esp+50] = arg 2 slot); the
        // dword passed at 005241F2 keeps the upper three bytes of that argument.
        const U packed = (minor & 0xffffff00u) | b;
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 005241AB..BA
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 005241C0..CB
        creature = as_creature(kotor_fastcall_00561080(player, nullptr));                 // 005241D1..E3
        if (creature != nullptr) {                                                        // 005241E5..E7
            w5_creature_action_manager_004f8770(creature, nullptr, 2);                    // 005241E9..ED
            kotor_fastcall_004eb620(creature, nullptr, item_id, second, packed);          // 005241F2..FB
            return 1;                                                                     // 00524200
        }
        recovered_inventory_equip_message_0056fcb0(message_of_server(), nullptr, at<U>(player, 4), item_id); // 0052420A..1F
        return 0;
    }
    case 7: {                                                                             // 00524229 unequip
        const U item_id = kotor_fastcall_00578540(message, nullptr);                      // 00524229..2B (ebp)
        const U slot = kotor_fastcall_00578540(message, nullptr);                         // 00524230..34 (ebx)
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 00524239..46
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 0052424C..57
        creature = as_creature(kotor_fastcall_00561080(player, nullptr));                 // 0052425D..6F
        if (creature != nullptr && lookup(item_id, &object) && as_item(object) != nullptr) { // 00524271..A2
            w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                   // 005242A4..A8
            recovered_combat_round_remove_all_actions_004d3770(at<void*>(creature, 0x9c8), nullptr); // 005242AD..B3
            recovered_object_unequip_004f06d0(creature, nullptr, as_item(object), slot, 0, 1, 0); // 005242B8..CB
            goto resume_attack;                                                           // 005242D0 -> 00523D52
        }
        recovered_inventory_unequip_cancel_message_0056fd80(message_of_server(), nullptr, at<U>(player, 4), item_id); // 005242D5..EA
        return 0;
    }
    case 11: {                                                                            // 00523DC8 equip pair
        const U first_id = kotor_fastcall_00578540(message, nullptr);                     // 00523DC8..CA (edi)
        const U has_second = kotor_fastcall_004d66f0(message, nullptr);                   // 00523DCF..D3 (ebp)
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 00523DD8..E5
        const U second_id = has_second != 0 ? kotor_fastcall_00578540(message, nullptr)   // 00523DE9..F4
                                            : k_invalid_00746644;                         // 00523DF6 ([esp+14])
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 00523DFB..0A
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 00523E0C..17
        creature = as_creature(kotor_fastcall_00561080(player, nullptr));                 // 00523E19..2B ([esp+28])
        if (creature == nullptr) return 0;                                                // 00523E2F
        void* const inventory = at<void*>(creature, 0xa2c);                               // 00523E31..39 ([esp+4C])
        if (inventory == nullptr) return 0;                                               // 00523E3D
        void* const first = recovered_item_lookup_004ae760(server_app(), nullptr, first_id);   // 00523E43..4C (ebp)
        void* const second = recovered_item_lookup_004ae760(server_app(), nullptr, second_id); // 00523E51..61 ([esp+48])
        U first_equipped = 0;                                                             // [esp+1C]
        U second_ok = 0;                                                                  // [esp+18]
        if (first != nullptr && recovered_inventory_has_item_005a4950(inventory, nullptr, first) != 0) { // 00523E66..82
            void* const base = base_item(first);                                          // 00523E84..97 ([esp+24])
            if (base != nullptr) {                                                        // 00523E9B
                for (U i = 0; i < 0xe; ++i) {                                             // 00523EA1..E6
                    const U bit = 1u << i;
                    if ((at<U>(base, 4) & bit) != 0 &&                                    // 00523EAC..B3
                        recovered_inventory_slot_from_item_005a4980(inventory, nullptr, first) == bit && // 00523EB5..BF
                        (second_id == k_invalid_00746644 ||                               // 00523EC1..CB
                         recovered_inventory_slot_from_item_005a4980(inventory, nullptr, first) == 0x10)) // 00523ECD..D8
                        first_equipped = 1;                                               // 00523EDA
                }
            }
        }
        if (second_id == k_invalid_00746644) {                                            // 00523EE8..F1
            second_ok = 1;                                                                // 00523EF3
        } else if (recovered_inventory_has_item_005a4950(inventory, nullptr, second) != 0) { // 00523EFD..0D
            void* const base = base_item(second);                                         // 00523F0F..24 (ebx)
            if (base != nullptr) {                                                        // 00523F26..28
                for (U i = 0; i < 0xe; ++i) {                                             // 00523F2A..5E
                    const U bit = 1u << i;
                    if ((at<U>(base, 4) & bit) != 0 &&                                    // 00523F30..3E (+4 re-read)
                        recovered_inventory_slot_from_item_005a4980(inventory, nullptr, second) == bit) // 00523F40..50
                        second_ok = 1;                                                    // 00523F52
                }
            }
        }
        if (first == nullptr) return 0;                                                   // 00523F60..62
        if (recovered_item_in_repository_00555f40(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr,
                                                  first, 1) == 0 &&                       // 00523F68..85
            recovered_inventory_has_item_005a4950(inventory, nullptr, first) == 0)        // 00523F87..91
            return 0;
        if (first_equipped != 0 && second_ok != 0) {                                      // 00523F97..A5
            recovered_object_unequip_004f06d0(creature, nullptr, first, k_invalid_00746644, 1, 0, 0); // 00523FA7..B7
            if (second != nullptr)                                                        // 00523FBC..C2
                recovered_object_unequip_004f06d0(creature, nullptr, second, k_invalid_00746644, 1, 0, 0); // 00523FC4..D4
            return 1;                                                                     // 00523FD9
        }
        if (second != nullptr &&                                                          // 00523FE3..E9
            (recovered_item_in_repository_00555f40(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr,
                                                   second, 1) != 0 ||                     // 00523FEF..04
             recovered_inventory_has_item_005a4950(inventory, nullptr, second) != 0)) {   // 00524006..10
            if (at<U8>(base_item(second), 9) != 0) {                                      // 00524012..29 (no null check)
                recovered_object_equip_004f0420(creature, nullptr, second, 0x20, 1, 0, 0, 1, 0); // 0052402B..7F
            } else {
                void* const base = base_item(second);                                     // 00524039..45
                if (base == nullptr) return 0;                                            // 0052404A..4C
                const U slots = at<U>(base, 4);                                           // 00524052
                for (U i = 0; i < 0x12; ++i) {                                            // 00524055..68
                    if ((slots & (1u << i)) != 0) {                                       // 00524057..60
                        recovered_object_equip_004f0420(creature, nullptr, second, 1u << i, 1, 0, 0, 1, 0); // 0052406A..7F
                        break;
                    }
                }
            }
        }
        if (recovered_item_in_repository_00555f40(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr,
                                                  first, 1) != 0 ||                       // 00524084..99
            recovered_inventory_has_item_005a4950(inventory, nullptr, first) != 0) {      // 0052409B..A5
            void* const base = base_item(first);                                          // 005240A7..B9 (esi)
            if (base == nullptr) return 0;                                                // 005240BB..BD
            if (at<U8>(base_item(first), 9) != 0 && (at<U8>(base, 4) & 0x10) != 0) {      // 005240C3..E0
                recovered_object_equip_004f0420(creature, nullptr, first, 0x10, 1, 0, 0, 1, 0); // 005240E2..1D
            } else {
                const U slots = at<U>(base, 4);                                           // 005240F0
                for (U i = 0; i < 0x12; ++i) {                                            // 005240F3..06
                    if ((slots & (1u << i)) != 0) {                                       // 005240F5..FE
                        recovered_object_equip_004f0420(creature, nullptr, first, 1u << i, 1, 0, 0, 1, 0); // 00524108..1D
                        break;
                    }
                }
            }
        }
        if (second != nullptr) {                                                          // 00524122..24
            void* const slot16 = recovered_equipment_item_005a4c20(inventory, nullptr, 0x10); // 00524126..2E (esi)
            void* const slot32 = recovered_equipment_item_005a4c20(inventory, nullptr, 0x20); // 00524133..39 (edi)
            if (slot16 != nullptr && at<U>(slot16, 0x14) != at<U>(first, 0x14))          // 0052413E..4A
                recovered_object_unequip_004f06d0(creature, nullptr, slot16, k_invalid_00746644, 1, 0, 0); // 0052414C..5C
            if (slot32 != nullptr && at<U>(slot32, 0x14) != at<U>(second, 0x14))         // 00524161..6F
                recovered_object_unequip_004f06d0(creature, nullptr, slot32, k_invalid_00746644, 1, 0, 0); // 00524171..81
        }
        return 1;                                                                         // 00524186
    }
    case 12: {                                                                            // 005243CC itemuse script
        const U id = kotor_fastcall_00578540(message, nullptr);                           // 005243CC..CE (edi)
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 005243D3..E0
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 005243E6..F1
        alignas(4) unsigned char script[8];                                               // CExoString [esp+1C]
        frame_LogStringCtor_005e5a90(script, nullptr, k_itemuse_00748028);                // 005243F7..400
        frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)),
                          nullptr, script, id, 1);                                        // 00524405..17
        frame_LogStringDtor_005e5c20(script, nullptr);                                    // 0052441C..28
        return 0;                                                                         // 0052442D (returns 0)
    }
    case 13: {                                                                            // 00524432 learn scroll
        const U id = kotor_fastcall_00578540(message, nullptr);                           // 00524432..34 (ebp)
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                  // 00524439..46
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                  // 0052444C..57
        void* const out = message_of_server();                                            // 0052445D..66 (esi)
        creature = as_creature(kotor_fastcall_00561080(player, nullptr));                 // 0052446B..7C
        if (creature == nullptr) return 0;                                                // 0052447F..81
        aurora_return_false_4_0063e7c0(creature, nullptr, id);                            // 00524487..8A
        kotor_fastcall_0056b240(out, nullptr, at<U>(player, 4), id, 0xe);                 // 0052448F..98
        return 1;                                                                         // 0052449D
    }
    default:                                                                              // 00523C68..6D / table -> 00523DB1
        return 0;
    }
resume_attack:                                                                            // 00523D52
    {
        const U target = at<U>(at<void*>(creature, 0x9c8), 0x9cc);                        // 00523D52..58
        if (target != k_invalid_00746644)                                                 // 00523D5E..63
            kotor_fastcall_004fde40(creature, nullptr, target, 0, 0, 0, 0, 0, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0); // 00523D65..8A
    }
    return 1;                                                                             // 00523D8F
}
}  // extern "C"
