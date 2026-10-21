// W5 queued-action bodies (lane worker "w5-creature-actions"): CSWSCreature::AIAction* for item pick-up / equip / drop /
// unequip, wait-for-end-of-round, encounter destroy-self, rest, appear / disappear, heal and surrender, plus the callees
// that have no implementation in any lane (reuse gate: c03-gui map, srcw1/srcw3/srcw4/src0886, build/1127 w5c tiers,
// Codex lane trees, kotor-pilot windows/ and docs/, all local/remote branch heads). Transcribed from the original
// swkotor.exe (read-only); addresses in the comments are the original instructions.
//
// Contract of every action body: original int __thiscall Body(CSWSObjectActionNode* node) -> EAX disposition, exported as
// extern "C" unsigned __fastcall w5_action_<name>_<addr>(self, unused_edx, node) (@12, w1_action_bodies_decl.inc).
// Dispositions are the proven .rdata values: [00745DBC]=1 in progress, [00745DC0]=2 complete, [00745DC4]=3 failed.
// x87: 53-bit precision -> double intermediates, float exactly where the original stores a dword.
// Memory: live_operator_new / live_free only. The original SEH states around operator new + constructor and around the
// CExoString locals are kept as catch(...) cleanups that free/destroy and rethrow.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // 0575 owner, mov eax,[ecx+4]; add eax,1B770h

extern "C" {
// ---------------- existing providers (c03-gui map; exact decorated names; no direct trap reachable) ----------------
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);                    // CWorldTimer::GetWorldTime
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                       // CServerExoApp::GetWorldTimer
unsigned __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day1, U time1, U day2, U time2,
                                                             U* out_day, U* out_time);               // SubtractWorldTimes
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                          // GetSWSMessage
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                            // GetServerAIMaster
unsigned __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U id_lo, U id_hi);    // RemoveEffectById
void __fastcall recovered_object_remove_effect_004d05c0(void* object, void*, void* effect);            // RemoveEffect
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);          // ApplyEffect
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                        // CSWCCMessageData ctor
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                           // CSWCCMessageData dtor
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);                // SetInteger
unsigned __fastcall recovered_creature_run_unequip_005023a0(void* creature, void*, U a, U b, U c, void* d, U e); // RunUnequip
unsigned __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                  // CGameEffect::GetInteger
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);             // CGameEffect::SetInteger
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                         // CGameEffect::SetCreator
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                    // CGameEffect ctor
unsigned __fastcall recovered_creature_remove_item_00510ef0(void* creature, void*, void* item, U a, U b, U c); // RemoveItem
unsigned __fastcall recovered_creature_acquire_item_005158e0(void* creature, void*, void** item, U a, U b, U c); // AcquireItem
unsigned __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);         // CSWRules::RollDice (AX)
void __fastcall recovered_item_owner_00553210(void* item, void*, U a, U b, U c, U d);                 // CSWSItem::SetPossessor
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag); // AddToArea
unsigned __fastcall recovered_item_in_repository_00555f40(void* repository, void*, void* item, U flag); // GetItemInRepository
unsigned __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);              // CItemRepository::RemoveItem
unsigned __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // AddItem
unsigned __fastcall recovered_item_update_notify_00569fc0(void* message, void*, void* player, U item_id); // ..._DestroyItem
unsigned __fastcall recovered_inventory_item_message_0056fb70(void* message, void*, U player_id, U item_id); // ..._DropCancel
unsigned __fastcall recovered_inventory_slot_message_0056fbd0(void* message, void*, U player_id, U a, U b); // ..._EquipCancel
unsigned __fastcall recovered_inventory_equip_message_0056fcb0(void* message, void*, U player_id, U item_id); // ..._PickupCancel
unsigned __fastcall recovered_inventory_unequip_cancel_message_0056fd80(void* message, void*, U player_id, U a); // ..._UnequipCancel
unsigned __fastcall recovered_cc_message_00568570(void* message, void*, U player_id, U kind, void* data, U d); // SendServerToPlayerCCMessage
unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, U skill, void* object, U flag); // GetSkillRank (AL)
unsigned __fastcall kotor_fastcall_004d68a0(void* message, void*, U a, U b, U c);                     // CreateWriteMessage
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                                 // WriteOBJECTIDServer
unsigned __fastcall kotor_fastcall_004d6440(void* message, void*, void* buffer, void* size);          // GetWriteMessage
unsigned __fastcall kotor_fastcall_00567e60(void* message, void*, U player_id, U major, U minor, U buffer, U size); // SendServerToPlayerMessage
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                       // CSWSPlayer::GetGameObject
unsigned __fastcall recovered_compare_item_00553cf0(void* item, void*, void* other);                  // CSWSItem::CompareItem
void __fastcall w5_creature_set_combat_mode_0050ee80(void* creature, void*, U mode, U force);         // SetCombatMode
void __fastcall recovered_creature_equip_004feb90(void* creature, void*, U slot, void* item, U a, U b, U c); // EquipItem
unsigned __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, U* slot, U a, U b, U c); // CanEquipItem (AL)
void __fastcall recovered_creature_unequip_item_004faa70(void* creature, void*, void* item);          // UnequipItem
void __fastcall recovered_creature_set_last_instant_equip_004efe00(void* creature, void*, U slot, U value); // SetLastInstantEquip
unsigned __fastcall recovered_party_is_member_00563790(void* party_table, void*, U id);               // CSWPartyTable::GetIsAvailable
unsigned __fastcall recovered_inventory_slot_from_item_005a4980(void* inventory, void*, void* item);  // GetSlotFromItem
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U id);         // GetBaseItem
void __fastcall recovered_creature_cancel_combat_004fdaa0(void* creature, void*, U flag);             // CancelCombat
unsigned __fastcall recovered_area_index_00506f20(void* area, void*, I* out, float x);                // GetFirstObjectIndiceByX
I __fastcall recovered_object_get_reputation_0057cb80(void* object, void*, U id, U flag);             // GetReputation
void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, U id, U flag);                  // CSWSFaction::AddMember
void* __fastcall recovered_item_construct_005530a0(void* item, void*, U id);                          // CSWSItem ctor
void __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);         // AddObjectToLookupTable
void* __fastcall recovered_localized_assign_005e9e70(void* self, void*, void* other);                 // CExoLocString::operator=
void __cdecl genuine_x87_pow_006fc1f0();                                                              // CRT _CIpow (ST1^ST0 -> ST0)

// ---------------- W5 exports written here (missing callees; see the trailing sections) ----------------
void* __fastcall kotor_fastcall_004ae7f0(void* server, void*, U id);
void* __fastcall kotor_fastcall_004b1a10(void* internal, void*, U id);
void* __fastcall kotor_fastcall_004aefc0(void* server, void*);
U __fastcall kotor_fastcall_004d35c0(void* round, void*);
void __fastcall kotor_fastcall_004d0940(void* object, void*, U keep_own);
void __fastcall kotor_fastcall_004ec700(void* creature, void*, void* data);
void __fastcall kotor_fastcall_004f3ed0(void* creature, void*, U amount, U feedback);
U __fastcall kotor_fastcall_00501de0(void* creature, void*, U item_id, U slot, U value);
void __fastcall kotor_fastcall_00518990(void* creature, void*, U keep_own);
void __fastcall kotor_fastcall_0055d800(void* item, void*, U id, U take);
U __fastcall kotor_fastcall_0055d950(void* item, void*, void* source);
void* __fastcall kotor_fastcall_0055f280(void* item, void*, I count);
U __fastcall kotor_fastcall_0056fb10(void* message, void*, U player_id, U item_id);
U __fastcall kotor_fastcall_0056fc50(void* message, void*, U player_id, U item_id);
double __fastcall kotor_fastcall_005919e0(void* encounter, void*, float cr);
void __fastcall kotor_fastcall_00594310(void* encounter, void*, float cr, U flag);
extern float* w5_cell_encounter_points_table_00832860;
}

namespace {
constexpr U k_in_progress_00745dbc = 1;          // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;             // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;               // .rdata 00745DC4
constexpr U k_ai_event_destroy_007441e0 = 11;    // .rdata 007441E0
constexpr U k_ai_event_script_007441dc = 10;     // .rdata 007441DC
constexpr U k_effect_poison_00746708 = 0x23;     // .rdata 00746708
constexpr U k_effect_disease_00746690 = 5;       // .rdata 00746690
constexpr U16 k_effect_heal_00746718 = 0x27;     // .rdata 00746718 (word)
constexpr U16 k_effect_visual_007466f4 = 0x1e;   // .rdata 007466F4 (word)
constexpr U16 k_subtype_magical_007473b8 = 0;    // .rdata 007473B8 (word)
constexpr U16 k_duration_permanent_007473bc = 3; // .rdata 007473BC (word)
constexpr U16 k_duration_innate_007473c8 = 4;    // .rdata 007473C8 (word)
constexpr U k_effect_type_keep_007466a0 = 9;     // .rdata 007466A0
constexpr U8 k_lookup_found_0074666c = 0;        // .rdata 0074666C (byte)
constexpr U8 k_type_encounter_0074663c = 0x0d;   // .rdata 0074663C (byte)
const float k_near_sq_00747c50 = 1.21f;          // .rdata 00747C50
const float k_run_sq_0073f404 = 4.0f;            // .rdata 0073F404
const float k_one_0073d764 = 1.0f;               // .rdata 0073D764
const float k_zero_0073d700 = 0.0f;              // .rdata 0073D700
const float k_fifty_00740cc0 = 50.0f;            // .rdata 00740CC0
const float k_drop_lift_0073e46c = 0.2f;         // .rdata 0073E46C
const float k_surrender_range_00747440 = 250.0f; // .rdata 00747440
const double k_points_base_00749d78 = 1.5;       // .rdata 00749D78
const double k_minus1_0073d768 = -1.0;           // .rdata 0073D768
const double k_minus2_00749d70 = -2.0;           // .rdata 00749D70
const double k_minus3_00749d68 = -3.0;           // .rdata 00749D68
const double k_minus4_00749d60 = -4.0;           // .rdata 00749D60
const char k_save_dc_00747d3c[] = "Save_DC";     // .rdata 00747D3C
const char k_subs_save_007470bc[] = "Subs_Save"; // .rdata 007470BC
constexpr U k_anim_take_low = 0x274b, k_anim_take_high = 0x274c, k_anim_heal = 0x2721;
constexpr U k_bits_one = 0x3f800000u, k_bits_one_half = 0x3fc00000u, k_bits_two = 0x40000000u;

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T> inline T* ptr(U v) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(v)); }
inline void* server() { return server_app(); }
inline void* sws_message() { return recovered_server_sws_message_004aedb0(server(), nullptr); }
inline void* ai() { return recovered_server_ai_master_004aed80(server(), nullptr); }
inline U group_of(void* node) { return at<U16>(node, 0x6c); }                 // xor r,r; mov r16,[node+6C]
inline bool fail_checks(void* self) {                                       // vtbl+94, return_zero 0063E7F0, GetDeadTemp
    if (vcall<U>(self, 0x94) == 1) return true;
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return true;
    return recovered_creature_incapacitated_004ef890(self, nullptr) == 1;
}
inline bool fail_checks_nonzero(void* self) {                               // the same three with test eax,eax; jne
    if (vcall<U>(self, 0x94) != 0) return true;
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) != 0) return true;
    return recovered_creature_incapacitated_004ef890(self, nullptr) != 0;
}
// CSWSObject::AddActionToFront 004CCB00 (W1 export) from 13 (type, value*) pairs
void front(void* self, U type, U group, const U (&q)[26]) {
    auto v = [&](int i) { return ptr<void>(q[i]); };
    recovered_object_add_action_to_front_004ccb00(self, nullptr, type, group,
        q[0], v(1), q[2], v(3), q[4], v(5), q[6], v(7), q[8], v(9), q[10], v(11), q[12], v(13),
        q[14], v(15), q[16], v(17), q[18], v(19), q[20], v(21), q[22], v(23), q[24], v(25));
}
// operator new(size) + constructor; the original EH state frees the block when the constructor throws
template<class F> void* construct(U size, F ctor) {
    void* memory = live_operator_new(size);
    if (memory == nullptr) return nullptr;
    try { return ctor(memory); } catch (...) { live_free(memory); throw; }
}
void* new_message_data() { return construct(0x34, [](void* m) { return recovered_message_data_construct_004d69f0(m, nullptr); }); }
void* new_script_event() { return construct(0x34, [](void* m) { return recovered_event_construct_004d7540(m, nullptr); }); }
void* new_effect() { return construct(0x8c, [](void* m) { return recovered_effect_construct_00503e40(m, nullptr, 1); }); }
// C2DA::GetINTEntry on a CExoString column; the original EH state destroys the string on unwind
void table_int(void* table, U row, const char* name, I* out) {
    alignas(4) unsigned char column[8];
    frame_LogStringCtor_005e5a90(column, nullptr, name);
    try { c2da_current_get_int_00413660(table, nullptr, row, column, out); }
    catch (...) { frame_LogStringDtor_005e5c20(column, nullptr); throw; }
    frame_LogStringDtor_005e5c20(column, nullptr);
}
// pow 006FC1F0 on x87 operands (fld base; fld exponent; call; result in ST0)
double x87_pow(double base, double exponent) {
    double result;
    __asm {
        fld qword ptr base
        fld qword ptr exponent
        call genuine_x87_pow_006fc1f0
        fstp qword ptr result
    }
    return result;
}
// 32-bit value of an int parameter read from `p` (AddActionToFront reads type-1 values as a DWORD)
struct ByteThenId { U8 b[8]; };
}  // namespace

extern "C" {

// =========================================================================================================
// 00517410 CSWSCreature::AIActionPickUpItem(node): +38 item id, +3C container/source id, +44 (byte) flag.
// Original thiscall, RET 4, frame 58h. [esp+6C] (the node argument slot) is reused for the CSWSPlayer*.
U __fastcall w5_action_pick_up_item_00517410(void* self, void*, void* node) {
    Vector target = {0.0f, 0.0f, 0.0f};                                             // [esp+2C] 0051741B..2B
    Vector safe = {0.0f, 0.0f, 0.0f};                                               // [esp+44] 00517433..43
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00517417..4B
    // [esp+13] byte flag directly followed by [esp+14] item id: the queued int parameter reads 4 bytes from [esp+13]
    ByteThenId slot;                                                                // [esp+13..1B]
    U item_id = at<U>(node, 0x38);                                                  // 00517454 -> [esp+14]
    U source_id = at<U>(node, 0x3c);                                                // 00517457 -> [esp+1C]
    const U8 flag = at<U8>(node, 0x44);                                             // 0051745A -> [esp+13]
    void* message = sws_message();                                                  // 00517471 (EBP)
    void* player = kotor_fastcall_004b2a50(at<void*>(server(), 4), nullptr, at<U>(self, 4)); // 00517488 (EBX, [esp+6C])
    void* item = recovered_item_lookup_004ae760(server(), nullptr, item_id);        // 005174A1 -> [esp+18]
    auto cancel = [&]() {                                                           // 005178B6..D8
        if (message != nullptr && player != nullptr)
            recovered_inventory_equip_message_0056fcb0(message, nullptr, at<U>(player, 4), item_id);
        at<U>(self, 0x98c) = 0;
        return k_failed_00745dc4;
    };
    if (fail_checks(self)) return cancel();                                         // 005174AA..D7
    if (item == nullptr) return cancel();                                           // 005174DD..E3
    if (at<U>(item, 0x268) != k_object_invalid) return k_failed_00745dc4;           // 005174E9..F3 -> 005178D8
    slot.b[0] = flag; std::memcpy(&slot.b[1], &item_id, 4); slot.b[5] = slot.b[6] = slot.b[7] = 0;
    Vector d;                                                                       // [esp+38]
    kotor_cdecl_00423bc0(&d, static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90)),
                         static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(item) + 0xa0))); // 005174F9..0B
    const double sum = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x; // 00517510..2D
    if (sum > static_cast<double>(k_near_sq_00747c50)) {                            // 0051752F..3A (test ah,41h; jne: !(a>b))
        // far: walk to a safe point beside the item, then this action again
        U run = (sum < static_cast<double>(k_run_sq_0073f404)) ? addr(player) : 1u;   // 00517540..52 (player slot as the run flag)
        {
            const U p[26] = {3, addr(&item_id), 3, addr(&source_id), 1, addr(&slot.b[0])};
            front(self, 7, group_of(node), p);                                       // 00517556..9E
        }
        {
            const U p[26] = {3, addr(&item_id)};
            front(self, 0x13, group_of(node), p);                                    // 005175A3..E5
        }
        Vector diff, n, sum_v;                                                       // [esp+50], [esp+5C], [esp+5C]
        kotor_cdecl_00423bc0(&diff, static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90)),
                             static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(item) + 0xa0))); // 005175EA..FB
        const Vector* r = normalize_004ab180(&n, &diff);                             // 00517600..06
        Vector dir; copy_vector(&dir, r);                                            // 0051760B..3A ([esp+50]; also [esp+38])
        const Vector* s = vector_sum_00423b90(&sum_v, static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(item) + 0xa0)), &dir); // 0051763E..44
        copy_vector(&target, s);                                                     // 00517649..63
        if (w1x_area_safe_location_004be860(recovered_object_area_004cb120(self, nullptr), nullptr, addr(&target), 0x3f000000u,
                                            at<U>(self, 0x340), 1, addr(&safe)) == 1) // 0051765C..86
            target = safe;                                                           // 0051768B..A3
        else
            copy_vector(&target, static_cast<unsigned char*>(item) + 0xa0);          // 005176A5..BE
        Vector zero = {0.0f, 0.0f, 0.0f};                                            // [esp+50] 005176F5..0E
        w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &target, at<U>(self, 0x8c),
            k_object_invalid, run, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);    // 005176C2..19
        return k_complete_00745dc0;                                                  // 0051771E
    }
    if (at<U>(self, 0x98c) == 0) {                                                   // 0051772D..39
        // near, first pass: play the pick-up animation, then this action again
        at<U>(self, 0x98c) = 1;                                                      // 0051774E
        U one = 1;                                                                   // [esp+20]
        U duration = k_bits_one_half;                                                // [esp+24] 1.5
        U speed = k_bits_one;                                                        // [esp+28] 1.0
        U anim = k_anim_take_high;                                                   // [esp+6C]
        if (!(target.z > k_one_0073d764)) anim = k_anim_take_low;                    // 0051773F..77 ([esp+34] is still the zeroed local)
        {
            const U p[26] = {3, addr(&item_id), 3, addr(&source_id), 1, addr(&slot.b[0])};
            front(self, 7, group_of(node), p);                                       // 0051777F..B2
        }
        {
            const U p[26] = {1, addr(&anim), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
            front(self, 6, group_of(node), p);                                       // 005177B7..F4
        }
        {
            const U p[26] = {3, addr(&item_id)};
            front(self, 0x13, group_of(node), p);                                    // 005177F9..23
        }
        return k_complete_00745dc0;                                                  // 00517828
    }
    // near, second pass: acquire it
    const U result = recovered_creature_acquire_item_005158e0(self, nullptr, &item, k_object_invalid, source_id, flag); // 00517837..4E
    if (result == 1) {                                                               // 00517853..5A
        if (player != nullptr && message != nullptr)                                 // 0051785C..62
            kotor_fastcall_0056fc50(message, nullptr, at<U>(player, 4), item_id);    // 00517864..6F
        at<U>(self, 0x98c) = 0;                                                      // 00517875
        return k_complete_00745dc0;
    }
    if (player != nullptr && message != nullptr)                                     // 00517889..8F
        recovered_inventory_equip_message_0056fcb0(message, nullptr, at<U>(player, 4), item_id); // 00517891..9C
    at<U>(self, 0x98c) = 0;                                                          // 005178A2
    return k_failed_00745dc4;
}

// =========================================================================================================
// 00510FD0 CSWSCreature::AIActionEquipItem(node): needs 3 parameters (+38 item id, +3C slot, +40 value).
U __fastcall w5_action_equip_item_00510fd0(void* self, void*, void* node) {
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00510FD1..D5
    if (at<U16>(node, 0x6e) < 3) return k_failed_00745dc4;                          // 00510FDA..EB (unsigned)
    const U item_id = at<U>(node, 0x38), slot = at<U>(node, 0x3c), value = at<U>(node, 0x40); // 00510FEF..F7
    if (!fail_checks_nonzero(self) &&                                               // 00510FFA..1C
        kotor_fastcall_00501de0(self, nullptr, item_id, slot, value) == 1)          // 0051101E..2B
        return k_complete_00745dc0;
    void* player = kotor_fastcall_004ae910(server(), nullptr, at<U>(self, 4));      // 00511039..46
    void* message = sws_message();                                                  // 0051104B..56
    if (message != nullptr && player != nullptr)                                    // 0051105B..61
        recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(player, 4), item_id, slot); // 00511063..6B
    return k_failed_00745dc4;
}

// =========================================================================================================
// 00513830 CSWSCreature::AIActionDropItem(node): +38 item id, +3C/+40/+44 drop point, +48 gold flag.
// Frame 50h; [esp+64] (node slot) is reused for the CSWSPlayer* and later the animation id.
U __fastcall w5_action_drop_item_00513830(void* self, void*, void* node) {
    void* repository = nullptr;                                                     // EBP 00513836
    U owned = 0;                                                                    // [esp+28] 00513849
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00513839..4D
    U item_id = at<U>(node, 0x38);                                                  // [esp+10]
    U x = at<U>(node, 0x3c), y = at<U>(node, 0x40), z = at<U>(node, 0x44);          // [esp+1C], [esp+18], [esp+14]
    U gold = at<U>(node, 0x48);                                                     // [esp+24]
    Vector p; p.x = flt(x); p.y = flt(y); p.z = flt(z);                             // [esp+30..38] 00513885..A2
    void* message = sws_message();                                                  // 005138A6 (EBX, [esp+20])
    void* player = kotor_fastcall_004b2a50(at<void*>(server(), 4), nullptr, at<U>(self, 4)); // 005138BB..C5 ([esp+64])
    if (fail_checks(self)) {                                                        // 005138C9..F6 -> 00513DEF
        if (message != nullptr && player != nullptr)
            recovered_inventory_item_message_0056fb70(message, nullptr, at<U>(player, 4), item_id);
        at<U>(self, 0x9a4) = 0;                                                     // 00513E0B
        return k_failed_00745dc4;
    }
    void* item = recovered_item_lookup_004ae760(server(), nullptr, item_id);        // 005138FC..0F (EBX)
    if (item != nullptr) {                                                          // 00513911..13
        const U possessor = at<U>(item, 0x268);
        if (possessor == at<U>(self, 4)) {                                          // 00513915..1E
            owned = 1;                                                              // 00513928
            repository = recovered_creature_inventory_004ef770(self, nullptr, 1);   // 00513924..35
        } else {
            void* container = recovered_item_lookup_004ae760(server(), nullptr, possessor); // 00513B2B..35
            if (container == nullptr || at<U>(container, 0x268) != at<U>(self, 4)) { // 00513B3A..47
                if (message != nullptr && player != nullptr)                        // 00513B5C..6E
                    recovered_inventory_item_message_0056fb70(message, nullptr, at<U>(player, 4), item_id); // 00513B74..E06
                at<U>(self, 0x9a4) = 0;                                             // 00513E0B
                return k_failed_00745dc4;
            }
            repository = at<void*>(container, 0x26c);                               // 00513B49
            owned = 1;                                                              // 00513B4F
        }
    }
    const double dx = static_cast<double>(at<float>(self, 0x90)) - p.x;             // 00513937..3D
    const double dy = static_cast<double>(at<float>(self, 0x94)) - p.y;             // 00513941..47
    const double dz = static_cast<double>(at<float>(self, 0x98)) - p.z;             // 0051394B..51
    const double sum = (dz * dz + dy * dy) + dx * dx;                               // 00513955..69
    if (sum > static_cast<double>(k_near_sq_00747c50)) {                            // 0051396B..76
        U run = (sum < static_cast<double>(k_run_sq_0073f404)) ? addr(player) : 1u;   // 0051397C..8E (player slot as the run flag)
        {
            const U q[26] = {3, addr(&item_id), 2, addr(&x), 2, addr(&y), 2, addr(&z), 1, addr(&gold)};
            front(self, 9, group_of(node), q);                                       // 00513992..E0
        }
        {
            const U q[26] = {2, addr(&p.x), 2, addr(&p.y), 2, addr(&p.z)};
            front(self, 0x31, group_of(node), q);                                    // 005139E5..36
        }
        Vector diff;                                                                 // [esp+48]
        diff.x = static_cast<float>(static_cast<double>(at<float>(self, 0x90)) - p.x); // 00513A3B..45
        diff.y = static_cast<float>(static_cast<double>(at<float>(self, 0x94)) - p.y); // 00513A49..53
        diff.z = static_cast<float>(static_cast<double>(at<float>(self, 0x98)) - p.z); // 00513A57..6A
        Vector nbuf; const Vector* r = normalize_004ab180(&nbuf, &diff);             // 00513A65..6F
        Vector n; copy_vector(&n, r);                                                // 00513A74..8F ([esp+3C])
        Vector zero = {0.0f, 0.0f, 0.0f};                                            // [esp+48] 00513AA1..B1
        p.x = static_cast<float>(static_cast<double>(n.x) + p.x);                    // 00513A97..C1
        p.y = static_cast<float>(static_cast<double>(n.y) + p.y);                    // 00513AC5..E2, 00513B00
        p.z = static_cast<float>(static_cast<double>(n.z) + p.z);                    // 00513AE6..F7, 00513B0A
        w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &p, at<U>(self, 0x8c),
            k_object_invalid, run, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);    // 00513A7F..17
        return k_complete_00745dc0;                                                  // 00513B1C
    }
    if (at<U>(self, 0x9a4) == 0) {                                                   // 00513B82..8C
        U one = 1;                                                                   // [esp+28] (over `owned`)
        U speed = k_bits_one;                                                        // [esp+2C]
        U duration = k_bits_one_half;                                                // [esp+20] (over the message slot)
        U anim = k_anim_take_high;                                                   // [esp+64]
        if (!(p.z > k_one_0073d764)) anim = k_anim_take_low;                         // 00513B92..C3
        {
            const U q[26] = {3, addr(&item_id), 2, addr(&x), 2, addr(&y), 2, addr(&z), 1, addr(&gold)};
            front(self, 9, group_of(node), q);                                       // 00513BCB..19
        }
        {
            const U q[26] = {1, addr(&anim), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
            front(self, 6, group_of(node), q);                                       // 00513C1E..6F
        }
        {
            const U q[26] = {2, addr(&p.x), 2, addr(&p.y), 2, addr(&p.z)};
            front(self, 0x31, group_of(node), q);                                    // 00513C74..C5
        }
        at<U>(self, 0x9a4) = 1;                                                      // 00513CCB
        return k_complete_00745dc0;
    }
    at<U>(self, 0x9a4) = 0;                                                          // 00513CE9
    auto drop_cancel = [&]() {                                                       // 00513DC6..E0
        if (message != nullptr && player != nullptr)
            recovered_inventory_item_message_0056fb70(message, nullptr, at<U>(player, 4), item_id);
        return k_failed_00745dc4;
    };
    if (repository == nullptr) return drop_cancel();                                 // 00513CE3..F3
    bool placed = false;
    if (owned == 1 && gold == 0) {                                                   // 00513CF9..06
        if (at<U>(item, 0x26c) != 0) {                                               // 00513D08..10 (a container)
            void* open = at<void*>(player, 0x58);                                    // 00513D12 (player not checked)
            if (open != nullptr && at<U>(open, 0) == at<U>(item, 0x14))              // 00513D15..1E
                kotor_fastcall_0055d800(item, nullptr, at<U>(self, 4), 1);           // 00513D20..28
        }
        if (recovered_creature_remove_item_00510ef0(self, nullptr, item, 1, 1, 0) == 1) { // 00513D2D..3E
            recovered_item_add_to_area_00555720(item, nullptr, recovered_object_area_004cb120(self, nullptr),
                                                flt(x), flt(y), flt(z), 1);           // 00513D40..99
            placed = true;
        }
    }
    if (!placed) {
        if (gold != 1) return drop_cancel();                                         // 00513D52..57
        kotor_fastcall_004f3ed0(self, nullptr, at<U>(item, 0x28c), 1);               // 00513D59..64
        recovered_item_owner_00553210(item, nullptr, k_object_invalid, 0, 0, k_object_invalid); // 00513D69..79
        recovered_item_add_to_area_00555720(item, nullptr, recovered_object_area_004cb120(self, nullptr),
                                            flt(x), flt(y), flt(z), 1);               // 00513D7E..99
    }
    if (player != nullptr && message != nullptr)                                     // 00513D9E..A8
        kotor_fastcall_0056fb10(message, nullptr, at<U>(player, 4), at<U>(item, 0x14)); // 00513DAA..B2
    return k_complete_00745dc0;                                                      // 00513DB7
}

// =========================================================================================================
// 00513EC0 CSWSCreature::AIActionUnequipItem(node): +38 item id, +3C repository, +40 value.
U __fastcall w5_action_unequip_item_00513ec0(void* self, void*, void* node) {
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00513EC4..C8
    const U item_id = at<U>(node, 0x38), b = at<U>(node, 0x3c), c = at<U>(node, 0x40); // 00513ECD..D7
    if (!fail_checks_nonzero(self) &&                                               // 00513EDA..FC
        recovered_creature_run_unequip_005023a0(self, nullptr, item_id, b, 1, nullptr, c) == 1) // 00513EFE..0E (EAX = 0)
        return k_complete_00745dc0;
    void* player = kotor_fastcall_004ae910(server(), nullptr, at<U>(self, 4));      // 00513F1C..29
    void* message = sws_message();                                                  // 00513F2E..39
    if (player != nullptr && message != nullptr)                                    // 00513F3E..44
        recovered_inventory_unequip_cancel_message_0056fd80(message, nullptr, at<U>(player, 4), item_id); // 00513F46..4D
    return k_failed_00745dc4;
}

// =========================================================================================================
// 00513F60 CSWSCreature::AIActionWaitForEndOfRound(node)
U __fastcall w5_action_wait_for_end_of_round_00513f60(void* self, void*, void*) {
    if (fail_checks(self)) return k_failed_00745dc4;                                // 00513F63..86
    void* round = at<void*>(self, 0x9c8);                                           // 00513F88
    if (at<U>(round, 0x934) == 1 && kotor_fastcall_004d35c0(round, nullptr) == 0)   // 00513F8E..9E
        return k_in_progress_00745dbc;
    return k_complete_00745dc0;                                                     // 00513FA9
}

// =========================================================================================================
// 00510C20 CSWSCreature::AIActionEncounterCreatureDestroySelf. The original ends with RET 0 (it never reads the node
// and leaves the caller's argument on the stack); the export keeps the runner's @12 contract.
U __fastcall w5_action_encounter_creature_destroy_self_00510c20(void* self, void*, void*) {
    if (fail_checks(self)) return k_failed_00745dc4;                                // 00510C23..4E
    void* encounter = kotor_fastcall_004ae7f0(server(), nullptr, at<U>(self, 0xa24)); // 00510C50..60
    if (encounter != nullptr) {                                                     // 00510C65..67
        kotor_fastcall_00594310(encounter, nullptr, at<float>(at<void*>(self, 0xa74), 0x84), 1); // 00510C69..7D
        at<U>(self, 0xa28) = 1;                                                     // 00510C82
    }
    recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0x1388, at<U>(self, 4), at<U>(self, 4),
                                     k_ai_event_destroy_007441e0, nullptr);         // 00510C8C..B1
    at<U>(self, 0xe8) = 0;                                                          // 00510CB6
    return k_complete_00745dc0;                                                     // 00510CC0
}

// =========================================================================================================
// 005140E0 CSWSCreature::AIActionRest(node)
U __fastcall w5_action_rest_005140e0(void*, void*, void*) {
    return k_complete_00745dc0;                                                     // 005140E0
}

}  // extern "C"
namespace {
// 005140F0 / 005141B0 shared shape: animate (anim) for 2000 ms after +BC/+C0 (the action's start time), then idle
U appear_disappear(void* self, U anim) {
    U time0 = 0, day1 = 0;                                                          // [esp+4], [esp+8]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr, &day1, &time0); // 005140F0..0F
    U out_time = 0, out_day = 0;                                                    // [esp+C], [esp+10]
    recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server(), nullptr), nullptr, day1, time0,
        at<U>(self, 0xbc), at<U>(self, 0xc0), &out_day, &out_time);                 // 00514114..46
    if (out_time < 0x7d0) {                                                         // 0051414B..55 (unsigned)
        at<U>(self, 0xd8) = k_bits_one;                                             // 0051415E
        vcall<void>(self, 0x7c, anim);                                              // 00514157..68
        return k_in_progress_00745dbc;
    }
    if (vcall<void*>(self, 0x30) != nullptr) {                                      // 00514177..7E
        at<U>(self, 0xd8) = k_bits_one;                                             // 00514189
        vcall<void>(self, 0x7c, 0x2711u);                                           // 00514180..93
    }
    return k_complete_00745dc0;                                                     // 00514196
}
}  // namespace
extern "C" {

// 005140F0 CSWSCreature::AIActionAppear(node)
U __fastcall w5_action_appear_005140f0(void* self, void*, void*) { return appear_disappear(self, 0x274f); }
// 005141B0 CSWSCreature::AIActionDisappear(node)
U __fastcall w5_action_disappear_005141b0(void* self, void*, void*) { return appear_disappear(self, 0x274e); }

// =========================================================================================================
// 00517A60 CSWSCreature::AIActionHeal(node): +38 target creature id, +3C med-pack item id, +40, +44 (move first flag).
// Frame 50h + SEH; [esp+70] (node slot) holds the target creature, then 1 in the animation branch.
U __fastcall w5_action_heal_00517a60(void* self, void*, void* node) {
    if (vcall<U>(self, 0x94) == 1 && kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) // 00517A7B..92 (only both)
        return k_failed_00745dc4;
    if (recovered_creature_incapacitated_004ef890(self, nullptr) == 1) return k_failed_00745dc4; // 00517A94..B4
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00517ABA..BE
    U target_id = at<U>(node, 0x38);                                                // [esp+10]
    U item_id = at<U>(node, 0x3c);                                                  // [esp+20]
    U p40 = at<U>(node, 0x40);                                                      // [esp+34]
    U p44 = at<U>(node, 0x44);                                                      // [esp+18]
    void* target = kotor_fastcall_004ae770(server(), nullptr, target_id);           // 00517AE9..F2 (EBP, [esp+70])
    if (target == nullptr) { at<U>(self, 0x978) = 0; return k_failed_00745dc4; }    // 00517AF6..09
    void* item = recovered_item_lookup_004ae760(server(), nullptr, item_id);        // 00517B0E..22 ([esp+48])
    if (item == nullptr) { at<U>(self, 0x978) = 0; return k_failed_00745dc4; }      // 00517B26
    Vector use_point = {0.0f, 0.0f, 0.0f};                                          // [esp+54] 00517B31..41
    if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, target_id, 0.0f, 0) == 0 && p44 != 0) { // 00517B28..5A
        U one_a = 1, one_b = 1;                                                     // [esp+30], [esp+44]
        p44 = 0;                                                                    // [esp+18] 00517B81
        float range;                                                                // [esp+1C]
        w5_creature_get_use_range_004ee440(vcall<void*>(self, 0x30), nullptr, target_id, &use_point, &range, 0); // 00517B60..8A
        {
            const U q[26] = {3, addr(&target_id), 3, addr(&item_id), 1, addr(&p40), 1, addr(&p44)};
            front(self, 0x38, group_of(node), q);                                   // 00517B8F..C9
        }
        {
            const U q[26] = {3, addr(&target_id)};
            front(self, 0x13, group_of(node), q);                                   // 00517BCE..F8
        }
        {
            const U q[26] = {3, addr(&target_id), 1, addr(&one_a), 2, addr(&range), 2, addr(&range), 1, addr(&one_b)};
            front(self, 0x11, group_of(node), q);                                   // 00517BFD..3F
        }
        Vector zero = {0.0f, 0.0f, 0.0f};                                           // [esp+38] 00517C77..87
        w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &use_point, at<U>(self, 0x8c),
            target_id, one_a, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);        // 00517C44..95
        return k_complete_00745dc0;                                                 // 00517C9A -> 005181EA
    }
    void* stats = at<void*>(self, 0xa74);                                           // 00517CA2 (EBX)
    void* target_object = vcall<void*>(target, 0x10);                               // 00517CA8..AC
    I rank = static_cast<signed char>(static_cast<U8>(
        w1x_creature_skill_rank_005aa570(stats, nullptr, 7, target_object, 0)));    // 00517CAF..BC ([esp+28])
    if (at<U>(self, 0x978) == 0) {                                                  // 00517CC0..C8
        U one = 1;                                                                  // [esp+70] 00517D22
        U anim = k_anim_heal;                                                       // [esp+28] 00517D2C (over rank)
        U speed = k_bits_one;                                                       // [esp+2C]
        U duration = k_bits_two;                                                    // [esp+38]
        at<U>(self, 0x978) = 1;                                                     // 00517D0D
        {
            const U q[26] = {3, addr(&target_id), 3, addr(&item_id), 1, addr(&p40), 1, addr(&p44)};
            front(self, 0x38, group_of(node), q);                                   // 00517CCE..4D
        }
        {
            const U q[26] = {1, addr(&anim), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
            front(self, 6, group_of(node), q);                                      // 00517D52..A7
        }
        return k_complete_00745dc0;                                                 // 00517DAC
    }
    U roll = static_cast<U16>(recovered_rules_roll_dice_00550b60(kotor_cell_rules_007a3a28, nullptr, 1, 0x14)); // 00517DB1..C4 ([esp+24])
    U take20;                                                                       // [esp+2C]
    if (at<U>(self, 0x4e0) == 1) take20 = 0;                                        // 00517DC7..DB
    else { take20 = 1; roll = 0x14; }                                               // 00517DDD..E1
    const I count = at<I>(target, 0x128);                                           // 00517DE9
    void* effect = nullptr;                                                         // EDI
    for (I i = 0; i < count; ++i) {                                                 // 00517DF9..44 (signed)
        void* e = at<void**>(target, 0x124)[i];
        const U type = at<U16>(e, 8);
        if (static_cast<I>(type) > static_cast<I>(k_effect_poison_00746708)) break;  // 00517E1A..1C (sorted list)
        if (type == k_effect_poison_00746708 || type == k_effect_disease_00746690) { effect = e; break; } // 00517E21..BC
    }
    I dc = 0;                                                                       // [esp+14] 00517E4C
    U total;                                                                        // EBX after 00518017
    if (effect != nullptr) {                                                        // 00517E4A..54
        const U type = at<U16>(effect, 8);
        if (type == k_effect_poison_00746708) {                                     // 00517E5A..63
            const U row = recovered_effect_integer_00503690(effect, nullptr, 0);    // 00517E65..69
            table_int(at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x90), row, k_save_dc_00747d3c, &dc); // 00517E6E..1C
            if (dc < 1) dc = 1;                                                     // 00517F21..2F
        } else if (type == k_effect_disease_00746690) {                             // 00517EBE..C9
            const U row = recovered_effect_integer_00503690(effect, nullptr, 0);    // 00517ECB..CF
            table_int(at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x94), row, k_subs_save_007470bc, &dc); // 00517ED4..1C
            if (dc < 1) dc = 1;
        } else {
            dc = 1;                                                                 // 00517F2A..2F
        }
        if (static_cast<I>(roll) + rank >= dc)                                      // 00517F33..3F (signed)
            recovered_object_remove_effect_004d06c0(target, nullptr, at<U>(effect, 0), at<U>(effect, 4)); // 00517F41..4A
    } else {
        recovered_creature_feedback_004ede10(self, nullptr, 0x37, nullptr);         // 00517F74..7A
        const std::int16_t current = static_cast<std::int16_t>(vcall<U>(target, 0x9c, 0u));          // 00517F7F..8C
        const std::int16_t maximum = static_cast<std::int16_t>(vcall<U>(target, 0x98, 1u));          // 00517F8F..96
        if (!(current < maximum)) {                                                 // 00517F9C..9F (signed 16-bit)
            recovered_creature_feedback_004ede10(self, nullptr, 0x38, nullptr);     // 00517FA1..A7
            return k_failed_00745dc4;
        }
    }
    void* data = new_message_data();                                                // 00517F4F..72 (EDI)
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 00517FB8..C8
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x14a);                      // 00517FCD..D6
    recovered_event_integer_004d76b0(data, nullptr, 1, roll);                       // 00517FDB..E4
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(rank));       // 00517FE9..F2
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));         // 00517FF7..00
    recovered_event_integer_004d76b0(data, nullptr, 4, take20);                     // 00518005..0E
    total = roll + static_cast<U>(rank);                                            // 00518013..17
    const U outcome = (static_cast<I>(total) >= dc) ? 1u : (take20 != 0 ? 3u : 0u); // 00518019..2F
    recovered_event_integer_004d76b0(data, nullptr, 5, outcome);                    // 00518031..35
    recovered_event_integer_004d76b0(data, nullptr, 6, 0x14a);                      // 0051803A..43
    kotor_fastcall_004ec700(self, nullptr, data);                                   // 00518048..4B
    if (recovered_item_in_repository_00555f40(recovered_creature_inventory_004ef770(self, nullptr, 1), nullptr, item, 1) == 1) { // 00518050..6A
        const I stack = at<I>(item, 0x28c);                                         // 00518070
        if (stack > 1) {                                                            // 00518076..79 (signed)
            at<I>(item, 0x28c) = stack - 1;                                         // 0051807B..7C
        } else if (recovered_creature_remove_item_00510ef0(self, nullptr, item, 1, 1, 0) != 0) { // 00518084..94
            void* player = kotor_fastcall_004ae910(server(), nullptr, at<U>(self, 4)); // 00518096..A3
            if (player != nullptr)                                                  // 005180A8..AA
                recovered_item_update_notify_00569fc0(sws_message(), nullptr, player, at<U>(item, 0x14)); // 005180AC..C1
            recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(self, 4), at<U>(item, 0x14),
                                             k_ai_event_destroy_007441e0, nullptr);  // 005180C6..EB
        }
    }
    void* heal = new_effect();                                                      // 005180F0..18
    {
        const U16 sub = at<U16>(heal, 0xa);                                         // 00518124
        at<U16>(heal, 8) = k_effect_heal_00746718;                                  // 0051811C..28
        at<U16>(heal, 0xa) = static_cast<U16>((sub & 0xfff8u) | k_subtype_magical_007473b8); // 00518134..41
    }
    recovered_effect_creator_00503a00(heal, nullptr, at<U>(self, 4));               // 00518145..4B
    recovered_effect_integer_005036a0(heal, nullptr, 0, total);                     // 00518150..55
    recovered_effect_apply_004d14f0(target, nullptr, heal, 0, 0);                   // 0051815A..61
    void* visual = new_effect();                                                    // 00518166..8E
    {
        const U16 sub = at<U16>(visual, 0xa);                                       // 0051819B
        at<U16>(visual, 8) = k_effect_visual_007466f4;                              // 00518192..9F
        at<U16>(visual, 0xa) = static_cast<U16>((sub & 0xfff8u) | k_subtype_magical_007473b8); // 005181AB..B7
    }
    recovered_effect_creator_00503a00(visual, nullptr, at<U>(self, 4));             // 005181BB..C1
    recovered_effect_integer_005036a0(visual, nullptr, 0, 0x3e9);                   // 005181C6..CF
    recovered_effect_apply_004d14f0(target, nullptr, visual, 0, 0);                 // 005181D4..DB
    at<U>(self, 0x978) = 0;                                                         // 005181E0
    return k_complete_00745dc0;                                                     // 005181EA
}

// =========================================================================================================
// 0051B420 CSWSCreature::AIActionSurrenderToEnemies(node)
U __fastcall w5_action_surrender_to_enemies_0051b420(void* self, void*, void*) {
    if (at<U>(self, 0x9d4) == 1) return k_failed_00745dc4;                          // 0051B420..2E
    kotor_fastcall_00518990(self, nullptr, 0);                                      // 0051B431..33
    return k_complete_00745dc0;                                                     // 0051B438
}

// =========================================================================================================
// Missing callees of the bodies above (no implementation in any lane). Named kotor_fastcall_<addr>.
// =========================================================================================================

// 004AE7F0 CServerExoApp::GetEncounterByGameObjectID(id): ECX = [this+4]; JMP 004B1A10
void* __fastcall kotor_fastcall_004ae7f0(void* server_app_object, void*, U id) {
    return kotor_fastcall_004b1a10(at<void*>(server_app_object, 4), nullptr, id);
}

// 004B1A10 CServerExoAppInternal::GetEncounterByGameObjectID(id): RET 4
void* __fastcall kotor_fastcall_004b1a10(void* internal, void*, U id) {
    void* object = nullptr;                                                         // [esp] 004B1A21
    if (static_cast<U8>(kotor_fastcall_004d8230(at<void*>(internal, 0x1005c), nullptr, id, &object)) != k_lookup_found_0074666c)
        return nullptr;                                                             // 004B1A15..34
    if (object == nullptr) return nullptr;                                          // 004B1A36..3C
    if (at<U8>(object, 8) != k_type_encounter_0074663c) return nullptr;             // 004B1A3E..47
    return vcall<void*>(object, 0x5c);                                              // 004B1A49..4B AsSWSEncounter
}

// 005919E0 CSWSEncounter::CalculatePointsFromCR(float cr): RET 4, result in ST0
double __fastcall kotor_fastcall_005919e0(void* encounter, void*, float cr) {
    const double c = cr;
    if (c > static_cast<double>(k_zero_0073d700) && c < static_cast<double>(k_one_0073d764)) { // 005919E0..04
        const float* thresholds = at<float*>(at<void*>(encounter, 0x310), 0);       // 00591A0A..14
        if (c >= static_cast<double>(thresholds[0])) return k_one_0073d764;       // 00591A10..25
        if (c >= static_cast<double>(thresholds[1])) return x87_pow(k_points_base_00749d78, k_minus1_0073d768); // 00591A28..47
        if (c >= static_cast<double>(thresholds[2])) return x87_pow(k_points_base_00749d78, k_minus2_00749d70); // 00591A4A..69
        if (c >= static_cast<double>(thresholds[3])) return x87_pow(k_points_base_00749d78, k_minus3_00749d68); // 00591A6C..8B
        return x87_pow(k_points_base_00749d78, k_minus4_00749d60);                   // 00591A8E..99
    }
    if (!(c < static_cast<double>(k_zero_0073d700)) && c <= static_cast<double>(k_fifty_00740cc0)) // 00591A9C..BC (NaN -> pow)
        return w5_cell_encounter_points_table_00832860[static_cast<I>(cr)];         // 00591ABE..CD (__ftol2 of the float)
    return x87_pow(k_points_base_00749d78, c);                                      // 00591AD3..E2
}

// 00594310 CSWSEncounter::RemoveFromActiveCreatureCount(float cr, flag): RET 8
void __fastcall kotor_fastcall_00594310(void* encounter, void*, float cr, U flag) {
    const U active = at<U>(encounter, 0x264);                                       // 0059432B
    if (active == 0) return;                                                        // 00594331..35
    const double left = static_cast<double>(at<float>(encounter, 0x2dc)) -
                        kotor_fastcall_005919e0(encounter, nullptr, cr);            // 0059433B..45 (fsubr)
    const U remaining = active - 1;                                                 // 0059434B..4E
    at<U>(encounter, 0x264) = remaining;
    at<float>(encounter, 0x2dc) = static_cast<float>(left);                         // 00594354 (fst)
    if (left < static_cast<double>(k_zero_0073d700)) at<U>(encounter, 0x2dc) = 0;    // 0059435A..67 (unrounded value)
    if (flag == 1) {                                                                // 0059436D..72
        at<float>(encounter, 0x2d8) = static_cast<float>(kotor_fastcall_005919e0(encounter, nullptr, cr) +
                                                         static_cast<double>(at<float>(encounter, 0x2d8))); // 00594374..82
        return;
    }
    if (remaining != 0) return;                                                     // 0059438D..8F
    void* event = new_script_event();                                               // 00594395..B6
    at<U16>(event, 0) = 0x15;                                                       // 005943B8 (OnExhausted)
    recovered_event_object_004d7780(event, nullptr, 0, at<U>(encounter, 4));        // 005943BD..CC
    recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(encounter, 4), at<U>(encounter, 4),
                                     k_ai_event_script_007441dc, event);            // 005943D1..F0
    if (at<U>(encounter, 0x23c) == 1)                                               // 005943F5..FC
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr,
                                    &at<U>(encounter, 0x270), &at<U>(encounter, 0x274)); // 005943FE..1B
    for (I i = 0; i < at<I>(encounter, 0x2b4); ++i) {                               // 00594420..50 (signed, re-read)
        at<U>(at<unsigned char*>(encounter, 0x2b0) + i * 0x24, 0x1c) = 0;
        at<U>(at<unsigned char*>(encounter, 0x2b0) + i * 0x24, 0x18) = 0;
    }
    at<U>(encounter, 0x238) = 0;                                                    // 00594452
    at<U>(encounter, 0x278) = 0;                                                    // 00594458
}

// 004D35C0 CSWSCombatRound::GetSpellActionPending(): RET 0
U __fastcall kotor_fastcall_004d35c0(void* round, void*) {
    void* list = at<void*>(round, 0x9b0);                                           // 004D35C4
    if (list == nullptr) return 0;                                                  // 004D35CA..CC
    void* position = at<void*>(at<void*>(list, 0), 0);                              // 004D35CE..D4 ([esp+4])
    if (position == nullptr) return 0;                                              // 004D35D8
    void* data = unresolved_list_at_pos_005e9b60(at<void*>(list, 0), nullptr, position); // 004D35DA..DD
    while (position != nullptr) {                                                   // 004D35E2..E8, 004D3616..1C
        if (data != nullptr && at<I>(round, 0x944) >= at<I>(data, 0) && at<U8>(data, 0x10) == 2) // 004D35F0..02
            return 1;
        data = ptr<void>(unresolved_list_next_005e9b90(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, &position)); // 004D3604..11
    }
    return 0;
}

// 004EC700 CSWSCreature::BroadcastSkillData(CSWCCMessageData*): RET 4; the data block is always destroyed and freed
void __fastcall kotor_fastcall_004ec700(void* creature, void*, void* data) {
    void* player = kotor_fastcall_004ae910(server(), nullptr, at<U>(creature, 4));  // 004EC700..0E
    if (player != nullptr)                                                          // 004EC713..19
        recovered_cc_message_00568570(sws_message(), nullptr, at<U>(player, 4), 9, data, 0); // 004EC71B..34
    if (data != nullptr) {                                                          // 004EC739..3B
        recovered_message_data_destroy_004d6a40(data, nullptr);                     // 004EC73D..3F
        live_free(data);                                                            // 004EC744..45
    }
}

// 004F3ED0 CSWSCreature::RemoveGold(amount, feedback): RET 8. Party members (+A88) use the party table gold (+FC).
void __fastcall kotor_fastcall_004f3ed0(void* creature, void*, U amount, U feedback) {
    auto gold = [&]() -> U& {
        return at<U>(creature, 0xa88) != 0 ? at<U>(lane_config_required::GetPartyTable_004aee70(server(), nullptr), 0xfc)
                                           : at<U>(creature, 0x9d0);
    };
    I left = static_cast<I>(gold() - amount);                                       // 004F3EEA..0F
    if (left < 0) left = 0;                                                         // 004F3F13..15 (jns)
    const U removed = gold() - static_cast<U>(left);                                // 004F3F17..45
    if (left > 999999999) left = 999999999;                                         // 004F3F3F..49 (signed)
    gold() = static_cast<U>(left);                                                  // 004F3F4E..6E
    if (feedback == 1 && removed != 0) {                                            // 004F3F74..7D
        void* data = new_message_data();                                            // 004F3F7F..A4
        recovered_event_integer_004d76b0(data, nullptr, 0, removed);                // 004F3FA6..B3
        recovered_creature_feedback_004ede10(creature, nullptr, 0x95, data);        // 004F3FB8..C0
    }
}

// 0056FB10 CSWSMessage::SendServerToPlayerInventory_Drop(player id, item id): RET 8 (minor 3)
// 0056FC50 CSWSMessage::SendServerToPlayerInventory_Pickup(player id, item id): RET 8 (minor 5)
}  // extern "C"
namespace {
U send_inventory(void* message, U player_id, U item_id, U minor) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                  // ..14..1C CreateWriteMessage
    kotor_fastcall_00578570(message, nullptr, item_id);                             // ..21..28 WriteOBJECTIDServer
    U buffer = 0, size = 0;                                                         // [esp+8], [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &buffer, &size) == 0) return 0;   // ..2D..40 GetWriteMessage
    return kotor_fastcall_00567e60(message, nullptr, player_id, 0xc, minor, buffer, size); // ..49..5E
}
}  // namespace
extern "C" {
U __fastcall kotor_fastcall_0056fb10(void* message, void*, U player_id, U item_id) { return send_inventory(message, player_id, item_id, 3); }
U __fastcall kotor_fastcall_0056fc50(void* message, void*, U player_id, U item_id) { return send_inventory(message, player_id, item_id, 5); }

// 00501DE0 CSWSCreature::RunEquip(item id, slot, value): RET 0Ch, EAX 1 equipped / 0 cancelled (EquipCancel sent)
U __fastcall kotor_fastcall_00501de0(void* self, void*, U item_id, U slot, U value) {
    U dropped = 0;                                                                  // [esp+20] 00501DF6
    void* player = kotor_fastcall_004ae910(server(), nullptr, at<U>(self, 4));      // 00501DE8..FE ([esp+18])
    void* message = sws_message();                                                  // 00501E03..12 ([esp+1C])
    void* repository = nullptr;                                                     // [esp+14]
    void* item = recovered_item_lookup_004ae760(server(), nullptr, item_id);        // 00501E17..2A (EDI)
    auto cancel = [&]() -> U {                                                      // 00502044..6A / 00501FFE..2A
        if (message != nullptr && player != nullptr)
            recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(player, 4), item_id, slot);
        return 0;
    };
    if (item == nullptr) return cancel();                                           // 00501E31..33
    const U possessor = at<U>(item, 0x268);                                         // 00501E39
    if (possessor == at<U>(self, 4)) {                                              // 00501E3F..42
        repository = recovered_creature_inventory_004ef770(self, nullptr, 1);       // 00501E44..4D
    } else {
        void* container = recovered_item_lookup_004ae760(server(), nullptr, possessor); // 00501E56..60
        if (container != nullptr && at<U>(container, 0x268) == at<U>(self, 4)) {    // 00501E65..72
            repository = at<void*>(container, 0x26c);                               // 00501E74..7A
        } else {
            void* client = client_app();                                            // 00501E8B (EBP)
            void* party_table = lane_config_required::GetPartyTable_004aee70(server(), nullptr); // 00501E83..95
            void* party = kotor_fastcall_005ed8b0(client, nullptr);                 // 00501E93..9E (EBX)
            if (recovered_party_member_00634620(party, nullptr, kotor_fastcall_005eda50(client, nullptr, at<U>(self, 4))) == 0 && // 00501EA0..B5
                recovered_party_is_member_00563790(party_table, nullptr, at<U>(self, 4)) == 0) // 00501EB7..C6
                return cancel();                                                    // -> 00501FFE
            if (recovered_party_member_00634620(party, nullptr, kotor_fastcall_005eda50(client, nullptr, at<U>(item, 0x268))) != 0 || // 00501ECC..E4
                recovered_party_is_member_00563790(party_table, nullptr, at<U>(item, 0x268)) != 0) { // 00501EEA..FE
                repository = recovered_creature_inventory_004ef770(self, nullptr, 1); // 00501FEC..F5
            } else {
                void* owner = recovered_item_lookup_004ae760(server(), nullptr, at<U>(item, 0x268)); // 00501F04..19
                if (owner == nullptr) return cancel();                              // 00501F1B..1D
                if (recovered_party_is_member_00563790(party_table, nullptr, at<U>(owner, 0x268)) == 0) return cancel(); // 00501F23..33
                repository = at<void*>(owner, 0x26c);                               // 00501F39..3F
            }
        }
    }
    const U8 verdict = static_cast<U8>(recovered_item_can_equip_0051aa60(self, nullptr, item, &slot, 1, 0, 1)); // 00501F4B..5E
    auto take_and_equip = [&](U equip_value) {                                      // 00501F66..C2 / 0050227F..D8
        if (at<I>(item, 0x28c) > 1) item = kotor_fastcall_0055f280(item, nullptr, 1); // SplitItem
        if (recovered_item_in_repository_00555f40(repository, nullptr, item, 0) == 1)
            recovered_remove_item_00555fd0(repository, nullptr, item);
        if (at<U>(item, 0x268) != at<U>(self, 4))
            recovered_item_owner_00553210(item, nullptr, at<U>(self, 4), 0, 0, k_object_invalid);
        recovered_creature_equip_004feb90(self, nullptr, slot, item, 1, 0, equip_value);
    };
    // unequip `held` back into the repository, or drop it at the feet (z + 0.2) when the repository refuses it
    auto stow = [&](void*& held) -> bool {                                          // e.g. 005020AC..72
        recovered_creature_unequip_item_004faa70(self, nullptr, held);
        if (recovered_inventory_insert_0055d330(recovered_creature_inventory_004ef770(self, nullptr, 1), nullptr, &held, 1, 0, 0) != 0)
            return true;
        recovered_creature_remove_item_00510ef0(self, nullptr, held, 0, 1, 0);
        const float z = static_cast<float>(static_cast<double>(at<float>(self, 0x98)) + k_drop_lift_0073e46c);
        recovered_item_add_to_area_00555720(held, nullptr, recovered_object_area_004cb120(self, nullptr),
                                            at<float>(self, 0x90), at<float>(self, 0x94), z, 1);
        return false;
    };
    if (verdict == 1) {                                                             // 00501F5E..60
        take_and_equip(value);                                                      // 00501F66..C2
        const U equipped_slot = recovered_inventory_slot_from_item_005a4980(at<void*>(self, 0xa2c), nullptr, item); // 00501FC7..CE
        if (equipped_slot == 0x10) at<U>(self, 0xaac) = value;                      // 00501FD3..D6, 0050202D
        else if (equipped_slot == 0x20) at<U>(self, 0xaa8) = value;                 // 00501FD8..E1
    } else {
        if (verdict == 2) {                                                         // 00502038..3A
            void* held = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, slot); // 0050206D..7F ([esp+10])
            if (held == nullptr) return cancel();                                   // 00502083
            void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                            at<U>(item, 0xc)); // 00502085..92
            if (at<U16>(base, 0x24) > 1) {                                          // 00502097..A0 (unsigned)
                if (recovered_compare_item_00553cf0(held, nullptr, item) != 0) return cancel(); // 005020A2..AA
                stow(held);                                                         // 005020AC..72
            } else {
                stow(held);                                                         // 00502110..72
            }
        } else if (verdict == 3) {                                                  // 0050203C..3E
            void* held = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, 0x10); // 00502170..7F
            if (held != nullptr) {                                                  // 00502183
                stow(held);                                                         // 00502185..E8
                dropped = 1;                                                        // 005021ED
            }
            void* off = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, 0x20); // 005021F5..04
            if (off != nullptr) stow(off);                                          // 0050220A..72
            else if (dropped != 1) return cancel();                                 // 00502274..79
        } else {
            return cancel();                                                        // 00502044
        }
        take_and_equip(0);                                                          // 0050227F..D8
        const U equipped_slot = recovered_inventory_slot_from_item_005a4980(at<void*>(self, 0xa2c), nullptr, item); // 005022DD..E9
        recovered_creature_set_last_instant_equip_004efe00(self, nullptr, equipped_slot, value); // 005022EE..F1
    }
    void* weapon = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, 0x10); // 005022F6..FE
    void* offhand = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, 0x20); // 00502303..0D
    const U8 mode = at<U8>(self, 0x4d2);                                            // 00502312
    bool reset = false;
    if (mode == 5) {                                                                // 00502318..25
        reset = weapon != nullptr || offhand != nullptr;
    } else if (mode == 6) {                                                         // 00502327..49
        reset = weapon == nullptr ||
                at<U8>(kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                         at<U>(weapon, 0xc)), 0x1a) == 0;
    } else if (mode == 2 || mode == 3 || mode == 1) {                               // 0050234B..58
        reset = weapon != nullptr &&
                at<U8>(kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                         at<U>(weapon, 0xc)), 0x1a) != 0; // 0050235A..75
    }
    if (reset) w5_creature_set_combat_mode_0050ee80(self, nullptr, 0, 1);           // 00502377..7D
    return 1;                                                                       // 00502385
}

// 0055F280 CSWSCreature::SplitItem (ECX = the item stack): RET 4, EAX new CSWSItem of `count` or 0
void* __fastcall kotor_fastcall_0055f280(void* item, void*, I count) {
    if (count <= 0 || count >= at<I>(item, 0x28c)) return nullptr;                  // 0055F29A..A8 (signed)
    void* copy = construct(0x29c, [](void* m) { return recovered_item_construct_005530a0(m, nullptr, k_object_invalid); }); // 0055F2AA..D7
    kotor_fastcall_0055d950(copy, nullptr, item);                                   // 0055F2D9..E4 (copy not checked)
    if (copy != nullptr) {                                                          // 0055F2E9..EB
        at<I>(item, 0x28c) -= count;                                                // 0055F2ED
        at<I>(copy, 0x28c) = count;                                                 // 0055F2F3
    }
    return copy;
}

// 0055D950 CSWSItem::CopyItem: single owner is claude_equip_items2.cpp (reviewed ACCEPT); declared above.

// 0055D800 CSWSItem::CloseInventory(creature id, take): RET 8. With `take`, the container's items move into the
// closing creature's repository; then OnClosed (0x17) is queued and the open flag (+288 bit 1) cleared.
void __fastcall kotor_fastcall_0055d800(void* item, void*, U id, U take) {
    if ((at<U8>(item, 0x288) & 2) == 0) return;                                     // 0055D819..21
    if (take != 0) {                                                                // 0055D827..31
        void* player = kotor_fastcall_004ae910(server(), nullptr, id);              // 0055D837..40
        if (player != nullptr) {                                                    // 0055D845..47
            void* creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30); // 0055D84D..5B (EDI)
            if (at<U>(at<void*>(item, 0x26c), 0x10) != 0) {                         // 0055D85D..68 (unsigned)
                do {
                    void* repository = at<void*>(item, 0x26c);                      // 0055D870
                    const U count = at<U>(repository, 0x10);
                    const I last = static_cast<I>(count - 1);
                    void* moved = (last >= 0 && last < static_cast<I>(count))       // 0055D879..82 (signed)
                        ? recovered_item_lookup_004ae760(server(), nullptr, at<U*>(repository, 0xc)[last]) : nullptr; // 0055D884..9B
                    recovered_remove_item_00555fd0(at<void*>(item, 0x26c), nullptr, moved); // 0055D89D..A8
                    recovered_inventory_insert_0055d330(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr,
                                                        &moved, 1, 0, 1);           // 0055D8AD..C3 ([esp+24] = take slot)
                } while (at<U>(at<void*>(item, 0x26c), 0x10) != 0);                 // 0055D8C8..D3
            }
        }
    }
    void* event = new_script_event();                                               // 0055D8D5..F8
    at<U16>(event, 0) = 0x17;                                                       // 0055D8FB (OnClosed)
    recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, id, at<U>(item, 0x14), k_ai_event_script_007441dc, event); // 0055D900..27
    at<U>(item, 0x288) &= 0xfffffffdu;                                              // 0055D92C
}

// 00518990 CSWSCreature::SurrenderToEnemies(flag): RET 4. Stops combat for self and every non-hostile creature within
// 250 (by the area's x-sorted object index), then joins faction slot 0 of the faction manager.
void __fastcall kotor_fastcall_00518990(void* self, void*, U keep_own) {
    recovered_creature_cancel_combat_004fdaa0(self, nullptr, 0);                    // 00518996..9A
    kotor_fastcall_004d0940(self, nullptr, keep_own);                               // 0051899F..A6
    Vector pos; copy_vector(&pos, static_cast<unsigned char*>(self) + 0x90);        // [esp+14] 005189AB..D5
    const float range = k_surrender_range_00747440;                                 // [esp+24] (over the argument slot)
    const float max_x = static_cast<float>(static_cast<double>(pos.x) + range);     // [esp+10] 005189BD..D9
    void* area = recovered_object_area_004cb120(self, nullptr);                     // 005189DD..E2 (EBX)
    if (area != nullptr) {                                                          // 005189E4..E6
        I index;                                                                    // [esp+C]
        const float min_x = static_cast<float>(static_cast<double>(pos.x) - range); // 005189EC..FB
        if (recovered_area_index_00506f20(area, nullptr, &index, min_x) == 1 && index < at<I>(area, 0x194)) { // 005189FF..17
            do {
                void* other = kotor_fastcall_004ae770(server(), nullptr, at<U*>(area, 0x190)[index]); // 00518A20..38
                if (other != nullptr && at<U>(self, 4) != at<U>(other, 4)) {        // 00518A3A..48
                    if (at<float>(other, 0x90) > max_x) break;                      // 00518A4E..5D (test ah,41h; je)
                    const double dx = static_cast<double>(at<float>(other, 0x90)) - pos.x; // 00518A63..69
                    const double dy = static_cast<double>(at<float>(other, 0x94)) - pos.y; // 00518A6D..73
                    const double dz = static_cast<double>(at<float>(other, 0x98)) - pos.z; // 00518A77..7D
                    const double sum = (dz * dz + dy * dy) + dx * dx;               // 00518A81..8F
                    const double reach = static_cast<double>(range) * range;        // 00518A91..95
                    if (!(reach < sum || reach != reach || sum != sum) &&           // 00518A99..A6 (test ah,1: C0)
                        recovered_object_get_reputation_0057cb80(self, nullptr, at<U>(other, 4), 0) <= 10) { // 00518AA8..B5 (signed)
                        recovered_creature_cancel_combat_004fdaa0(other, nullptr, 0); // 00518AB7..BB
                        kotor_fastcall_004d0940(other, nullptr, keep_own);          // 00518AC0..C3
                    }
                }
                ++index;                                                            // 00518AC8..D5
            } while (index < at<I>(area, 0x194));                                   // 00518AD9 (signed, re-read)
        }
    }
    if (kotor_fastcall_004aefc0(server(), nullptr) != nullptr &&                    // 00518AE0..F0
        at<void*>(kotor_fastcall_004aefc0(server(), nullptr), 0) != nullptr) {      // 00518AF2..03
        const U id = at<U>(self, 4);                                                // 00518B0D
        w2x_faction_add_member_005bfa70(at<void*>(kotor_fastcall_004aefc0(server(), nullptr), 0), nullptr, id, 0); // 00518B10..1A
    }
}

// 004AEFC0 CServerExoApp::GetFactionManager(): [[this+4]+10054h]
void* __fastcall kotor_fastcall_004aefc0(void* server_app_object, void*) {
    return at<void*>(at<void*>(server_app_object, 4), 0x10054);
}

// 004D0940 CSWSObject::ClearAllEffects(keep own): RET 4. From the last effect down, removes every effect except
// innate / permanent durations, type 9, and (with the flag) the ones this object created itself; +E8 = 1.
void __fastcall kotor_fastcall_004d0940(void* object, void*, U keep_own) {
    I i = at<I>(object, 0x128);                                                     // 004D0943
    if (i == 0) return;                                                             // 004D0949..4B
    --i;                                                                            // 004D094D
    if (i >= 0) {                                                                   // 004D0950..52
        do {
            void* effect = at<void**>(object, 0x124)[i];                            // 004D0960..66
            if (effect != nullptr) {                                                // 004D0969..6B
                const U16 duration = static_cast<U16>(at<U16>(effect, 0xa) & 7);    // 004D096D..71
                if (duration != k_duration_innate_007473c8 && duration != k_duration_permanent_007473bc && // 004D0975..85
                    static_cast<U>(at<U16>(effect, 8)) != k_effect_type_keep_007466a0 && // 004D0987..91
                    !(keep_own != 0 && at<U>(effect, 0x18) == at<U>(object, 4))) {  // 004D0993..9D
                    recovered_object_remove_effect_004d05c0(object, nullptr, effect); // 004D099F..A2
                    i = at<I>(object, 0x128);                                       // 004D09A7 (restart below the new count)
                }
            }
            --i;                                                                    // 004D09AD
        } while (i >= 0);                                                           // 004D09AE (jns)
    }
    at<U>(object, 0xe8) = 1;                                                        // 004D09B1
}

// .bss 00832860: the CR -> encounter points lookup that CSWSEncounter::CSWSEncounter 00593C70 builds on first use
// (and staticDisposePowerLookup 0073BED0 frees). No encounter constructor is reconstructed yet; this is its storage.
float* w5_cell_encounter_points_table_00832860 = nullptr;

}  // extern "C"
