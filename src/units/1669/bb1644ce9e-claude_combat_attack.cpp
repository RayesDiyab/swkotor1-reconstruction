// W5 queued-action body, lane worker combat-attack (part A, main):
//   005BBBF0 CSWSCreature::AIActionPhysicalAttacks (export w5_action_physical_attacks_005bbbf0)
//   + its direct callees that have no implementation anywhere (c07b candidate map):
//   005BBA80 ResolveAttack, 005B48F0 HasTargetBeenDetected, 004D2B70 ResolveChoreographedState,
//   004D2C30 ResolveEngagedState, 004D2D60 ResolveMasterState, 004FB0F0 MaxAttackRange, 004FFAD0 MaxCleaveRange,
//   004F1310 DesiredAttackRange, 004EBB30 RangeWeaponEquipped, 005B4970 InitializeNewAttackTarget,
//   004D3E50 UpdateAttackTargetForAllAction, 005B48B0 CancelAllCombatModes, 005B7B30 ResolveMoveToForceJump,
//   005B5690 IsLightsaberEquipped, 005B47C0 IsCreature.
// The rest of the attack engine (combat round, attack/damage rolls, stats, animations) is in the sibling
// claude_combat_attack_<part>.cpp files and is declared below.
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; ST0 returns are double;
// live_operator_new / live_free only. No traps are introduced by this file.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
struct Node;
void* __fastcall next_005e9b90(kotor_list::Internal* list, void*, Node** position);  // CExoLinkedListInternal::GetNext
}
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); } // CExoString() [0603]
namespace kotor_reconstruction { struct StartupString; }
namespace directory_list { char __fastcall At005e5600(kotor_reconstruction::StartupString* s, void*, int index); } // CExoString::operator[] [0288]
namespace menu_resources { struct Ref { Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                               // return_zero [0832]
U __fastcall frame_GetPauseState_004ae980(void* server, void*, U mask);                          // [0457]
void __fastcall kotor_fastcall_006345e0(void* member, void*, U message);                         // SetCombatMessage
// recovered_party_index_006349c0 (CSWParty::GetIndex) is declared by w5_foreign.hpp (unsigned; read as signed)
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);       // GetVisibleListElement [W1 b7]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                      // CSWItem::GetBaseItem
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id);  // CSWBaseItemArray::GetBaseItem [0820]
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, int index);                     // CSWParty::GetCharacter
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);       // CSWCCreature::GetServerCreature
U __fastcall w5x_creature_is_debilitated_005b4880(void* creature, void*);                        // IsDebilitated
I __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, int row, U column, float* out); // C2DA::GetFLOATEntry
extern unsigned kotor_rules_cell32_007a210c;                                                     // .data 007A210C [0431]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0379]
// unresolved_list_at_pos_005e9b60 (CExoLinkedListInternal::GetAtPos) is declared by w5_t2.hpp
void __fastcall w5_creature_set_combat_mode_0050ee80(void* creature, void*, U mode, U check);    // SetCombatMode [W5 t3]
void __fastcall w5_combat_clear_all_special_attacks_004d4f60(void* round, void*);                // ClearAllSpecialAttacks [W5 t3]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                      // CSWSCreatureStats::HasFeat
U __fastcall kotor_m15closure_00555690(void* item, void*);                                       // CSWSItem::IsLightsaber
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U arg);                // CGameEffect::CGameEffect
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id);     // CGameEffect::SetObjectID
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);        // CGameEffect::SetInteger
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);    // CSWSObject::ApplyEffect
U __fastcall recovered_party_table_leader_00563a00(void* table, void*, U id);                    // CSWPartyTable::GetIsLeader
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);            // CSWSCombatRound::GetAttack [W1 b7]
U __cdecl aurora_vector_equal_004aa980(const float* left, const float* right);                   // operator== (Vector) [0446]
double __cdecl vector_magnitude_004ab100(const Vector* v);                                       // Vector magnitude [0464]
Vector* __fastcall w3_placeable_nearest_action_point_00584c20(void* placeable, void*, Vector* out, const Vector* from); // [W3]
U __fastcall kotor_fastcall_004f2de0(void* creature, void*, float range, U target, U exclude);   // GetNearestEnemy [sol-nearest-enemy]
// claude_w5_combat_actions.cpp (CSWSCombatRound pause members)
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                     // SetRoundPaused
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                     // SetPauseTimer

// ---------------- sibling parts of this lane (claude_combat_attack_<part>.cpp) ----------------
U __fastcall kotor_fastcall_004d5f70(void* round, void*, U target, U engaged, U master, U a4, U a5); // StartCombatRound [round]
U __fastcall kotor_fastcall_005bb590(void* creature, void*, void* target, U a2, U a3, U a4, U a5, U a6, U a7); // ResolveRangedAttack [anim]
U __fastcall kotor_fastcall_005bb890(void* creature, void*, void* target, U a2, U a3, U a4, U a5, U a6, U a7); // ResolveMeleeAttack [anim]

// ---------------- exports of this file ----------------
U __fastcall kotor_fastcall_005b48f0(void* creature, void*, U target, U flag);
U __fastcall kotor_fastcall_004d2b70(void* round, void*, U target);
U __fastcall kotor_fastcall_004d2c30(void* round, void*, U target);
U __fastcall kotor_fastcall_004d2d60(void* round, void*, U target, U* engaged);
double __fastcall kotor_fastcall_004fb0f0(void* creature, void*, U target, U a2, U ranged_only);
double __fastcall kotor_fastcall_004ffad0(void* creature, void*, U target, U a2, U a3);
double __fastcall kotor_fastcall_004f1310(void* creature, void*, U target, U flag);
U __fastcall kotor_fastcall_004ebb30(void* creature, void*);
void __fastcall kotor_fastcall_005b4970(void* creature, void*, U target, void* node);
void __fastcall kotor_fastcall_004d3e50(void* round, void*, U target);
void __fastcall kotor_fastcall_005b48b0(void* creature, void*, U unused);
U __fastcall kotor_fastcall_005b7b30(void* creature, void*, void* target, U16* feat);
U __fastcall kotor_fastcall_005b5690(void* creature, void*);
U __fastcall kotor_fastcall_005b47c0(void* creature, void*);
void __fastcall kotor_fastcall_005bba80(void* creature, void*, U target, U a2, U a3, U a4, U a5, U a6, U a7);
}

namespace {
constexpr U k_in_progress_00745dbc = 1;   // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;      // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;        // .rdata 00745DC4
constexpr U8 k_type_creature_00746634 = 5;  // .rdata 00746634 (byte)
constexpr U8 k_type_placeable_00746638 = 9; // .rdata 00746638 (byte)
constexpr U8 k_type_door_00746639 = 10;     // .rdata 00746639 (byte)
constexpr U16 k_effect_type_00746810 = 0x66; // .rdata 00746810 (word): CGameEffect type for the force jump
const float k_eye_height_0073f414 = 1.5f;   // .rdata 0073F414
const float k_los_bias_0073dfec = 0.01f;    // .rdata 0073DFEC (0x3C23D70A)
const float k_range_slack_0073e46c = 0.2f;  // .rdata 0073E46C
const float k_quarter_0073edf0 = 0.25f;     // .rdata 0073EDF0
const float k_cleave_extra_00741be0 = 2.0f; // .rdata 00741BE0
const float k_ranged_only_range_007455d8 = 20.0f; // .rdata 007455D8
const float k_default_ranged_range_007455dc = 30.0f; // .rdata 007455DC
const float k_zero_0073d700 = 0.0f;         // .rdata 0073D700
const float k_melee_extra_0073e9ac = 0.5f;  // .rdata 0073E9AC
const float k_personal_slack_00746f4c = 0.7f; // .rdata 00746F4C
const float k_size_slack_00746f48 = 1.6f;   // .rdata 00746F48
const float k_jump_min_squared_00741838 = 100.0f; // .rdata 00741838
const char k_empty_0073d71c[] = "";         // .rdata 0073D71C
const char k_modeltype_00747c2c[] = "MODELTYPE"; // .rdata 00747C2C

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // vtbl+48 AsSWSPlaceable
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }          // CSWSCreature +9C8 CSWSCombatRound*
inline void* inventory_of(void* c) { return at<void*>(c, 0xa2c); }      // CSWSCreature +A2C CSWInventory*
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* party_table() { return static_cast<unsigned char*>(at<void*>(server_app(), 4)) + 0x1b770; } // inline view of 004AEE70
inline Vector vec_of_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline const Vector* position_of(void* o) { return reinterpret_cast<const Vector*>(static_cast<unsigned char*>(o) + 0x90); }
void add_action_front(void* o, U type, U group, const U* pairs) { w5_add_action_to_front_body(o, type, group, pairs); }
// base item of the item in inventory slot 10h (main hand) through the rules base item array (+34)
void* rules_base_item_of(void* item) {
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr, at<int>(item, 0xc));
}
// CSWPartyMemberData::SetCombatMessage for the creature's client party slot (005BBCC7..D06 / 005BC9D5..A13)
void party_combat_message(void* self, U message) {
    void* client = client_app();                                                    // [[7A39FC]+4]
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(self, 4));
    const I index = static_cast<I>(recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, client_id));
    if (index < 0) return;                                                          // jl
    void* party = kotor_fastcall_005ed8b0(client, nullptr);
    kotor_fastcall_006345e0(static_cast<unsigned char*>(party) + index * 0x88 + 0x24, nullptr, message);
}
// the pointer arithmetic of 005BC27B..81 (GetArea() + 10Ch) without a null-pointer UB in C++
inline void* area_offset(void* area, U offset) { return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(area) + offset); }
}  // namespace

extern "C" {
// =========================================================================================================
// 004EBB30 CSWSCreature::RangeWeaponEquipped: RET 0, EAX 0/1 (main-hand base item +1A ranged flag)
U __fastcall kotor_fastcall_004ebb30(void* self, void*) {
    void* item = recovered_equipment_item_005a4c20(inventory_of(self), nullptr, 0x10); // 004EBB30..38
    if (item == nullptr) return 0;                                                  // 004EBB3D..3F
    const U ranged = at<U8>(rules_base_item_of(item), 0x1a) != 0 ? 1u : 0u;         // 004EBB41..5D (0 < byte: sbb/neg)
    return static_cast<I>(ranged) > 0 ? 1u : 0u;                                    // 004EBB5F..63 (setg)
}

// 005B5690 CSWSCreature::IsLightsaberEquipped: RET 0, EAX 0/1 (slot 10h or slot 20h holds a lightsaber)
U __fastcall kotor_fastcall_005b5690(void* self, void*) {
    U result = 0;                                                                   // EDI (005B569C)
    void* item = recovered_equipment_item_005a4c20(inventory_of(self), nullptr, 0x10); // 005B5694..9E
    if (item != nullptr && kotor_m15closure_00555690(item, nullptr) != 0) result = 1; // 005B56A3..B2
    item = recovered_equipment_item_005a4c20(inventory_of(self), nullptr, 0x20);    // 005B56B7..BF
    if (item != nullptr && kotor_m15closure_00555690(item, nullptr) != 0) return 1; // 005B56C4..D6
    return result;                                                                  // 005B56D8
}

// 005B47C0 CSWSCreature::IsCreature: RET 0, EAX 0/1. appearance 2DA (rules +B8 -> +C) row = +A60, column
// MODELTYPE; the first character S/L/s/l (large/small creature models). The GetString status is ignored.
U __fastcall kotor_fastcall_005b47c0(void* self, void*) {
    U result = 0;                                                                   // EDI (005B47E0)
    alignas(4) unsigned char value[8];                                              // CExoString [esp+8]
    kotor_movie::string_element_construct_005b3190(value, nullptr);                 // 005B47DC..E2
    alignas(4) unsigned char column[8];                                             // CExoString [esp+10]
    frame_LogStringCtor_005e5a90(column, nullptr, k_modeltype_00747c2c);            // 005B47E7..F4
    c2da_current_get_string_00413270(at<void*>(at<void*>(rules(), 0xb8), 0xc), nullptr,
                                     at<U16>(self, 0xa60), column, value);          // 005B47F9..20
    const char first = directory_list::At005e5600(reinterpret_cast<kotor_reconstruction::StartupString*>(value),
                                                  nullptr, 0);                      // 005B4825..2A
    if (first == 'S' || first == 'L' || first == 's' || first == 'l') result = 1;  // 005B482F..3F
    frame_LogStringDtor_005e5c20(column, nullptr);                                  // 005B4844..4D (column first)
    frame_LogStringDtor_005e5c20(value, nullptr);                                   // 005B4852..5E
    return result;
}

// 005B48F0 CSWSCreature::HasTargetBeenDetected(target, flag): RET 8, EAX 0/1
U __fastcall kotor_fastcall_005b48f0(void* self, void*, U target, U flag) {
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target);               // 005B48F0..08
    if (o != nullptr) {                                                             // 005B490F..11
        if (as_door(o) != nullptr) return 1;                                        // 005B4913..1C
        if (as_placeable(o) != nullptr) return 1;                                   // 005B491E..27
    }
    if (at<U>(self, 0xa88) != 0) return 1;                                          // 005B4929..31
    void* element = recovered_creature_visible_element_00517a20(self, nullptr, target); // 005B4933..36
    if (element == nullptr) return 0;                                               // 005B493B..3D
    if (flag != 0 && (at<U8>(element, 4) & 0x0c) == 4) return 0;                    // 005B493F..50
    const U8 bits4 = at<U8>(element, 4);                                            // 005B4952
    if ((bits4 & 0x10) != 0) return 0;                                              // 005B4955..57
    return (bits4 & 1) != 0 ? 1u : 0u;                                              // 005B4959..5B
}

// =========================================================================================================
// CSWSCombatRound (+9B0 action list, +9B4 owner creature)
// 004D2B70 CSWSCombatRound::ResolveChoreographedState(target): RET 4, EAX 0/1. Both owner and target wield a
// melee (base item +1A == 0) main-hand weapon whose base item +8 is neither 1 nor 8.
U __fastcall kotor_fastcall_004d2b70(void* round, void*, U target) {
    void* item = recovered_equipment_item_005a4c20(inventory_of(at<void*>(round, 0x9b4)), nullptr, 0x10); // 004D2B70..82
    if (item == nullptr) return 0;                                                  // 004D2B87..8B
    U8 kind = at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 8);     // 004D2B91..99 (BL)
    if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1a) != 0) return 0; // 004D2B9C..A8 (ja)
    if (kind == 1 || kind == 8) return 0;                                           // 004D2BAA..B2
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target);               // 004D2BB4..C2
    if (o == nullptr || as_creature(o) == nullptr) return 0;                        // 004D2BC7..D6
    item = recovered_equipment_item_005a4c20(inventory_of(as_creature(o)), nullptr, 0x10); // 004D2BD8..E9
    if (item == nullptr) return 0;                                                  // 004D2BEE..F2
    kind = at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 8);        // 004D2BF4..FB
    if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1a) != 0) return 0; // 004D2BFE..0A
    if (kind == 1 || kind == 8) return 0;                                           // 004D2C0C..14
    return 1;                                                                       // 004D2C16
}

// 004D2C30 CSWSCombatRound::ResolveEngagedState(target): RET 4, EAX 0/1.
// Target creature +50C attack target, +524 / +510 other targets, its round +934 round-active flag.
U __fastcall kotor_fastcall_004d2c30(void* round, void*, U target) {
    U engaged = 0;                                                                  // EDI (004D2C43)
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target);               // 004D2C30..45
    if (o == nullptr || as_creature(o) == nullptr) return 0;                        // 004D2C4A..5D
    void* c = as_creature(o);                                                       // 004D2C63..6A (ESI)
    const U active = at<U>(round_of(c), 0x934);                                     // 004D2C6C..72 (ECX)
    if (active == 0 && at<U>(c, 0x50c) == k_object_invalid) engaged = 1;            // 004D2C78..88
    const U owner_id = at<U>(at<void*>(round, 0x9b4), 4);                           // 004D2C8D..93 (EAX)
    const U attack_target = at<U>(c, 0x50c);                                        // 004D2C96 (EDX)
    if (attack_target == owner_id || at<U>(c, 0x524) == owner_id) engaged = 1;      // 004D2C9C..A8
    if (active != 0 && attack_target == k_object_invalid) engaged = 1;              // 004D2CAD..B9
    void* pc = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 004D2CBE..CF
    if (pc != nullptr &&                                                            // 004D2CD4..D6
        target == at<U>(kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr), 4)) { // 004D2CD8..E2
        const U t50c = at<U>(c, 0x50c);                                             // 004D2CE4
        bool cleared = false;
        if (t50c == k_object_invalid) {                                             // 004D2CEA..EF
            if (at<U>(c, 0x524) == t50c) engaged = 0;                               // 004D2CF1..F9
        } else if (t50c != at<U>(at<void*>(round, 0x9b4), 4)) {                     // 004D2D02..0B
            engaged = 0; cleared = true;                                            // -> 004D2D3D
        }
        if (!cleared) {
            const U t524 = at<U>(c, 0x524);                                         // 004D2D0D
            if (t524 != k_object_invalid && t524 != at<U>(at<void*>(round, 0x9b4), 4)) { // 004D2D13..23
                engaged = 0;
            } else {
                const U t510 = at<U>(c, 0x510);                                     // 004D2D25
                if (t510 != k_object_invalid && t510 != at<U>(at<void*>(round, 0x9b4), 4)) engaged = 0; // 004D2D2B..3D
            }
        }
    }
    if (w5x_creature_is_debilitated_005b4880(c, nullptr) != 0) return 0;            // 004D2D3F..50
    return engaged;                                                                 // 004D2D53
}

// 004D2D60 CSWSCombatRound::ResolveMasterState(target, U* engaged): RET 8, EAX 0/1
U __fastcall kotor_fastcall_004d2d60(void* round, void*, U target, U* engaged) {
    U master = 0;                                                                   // EDI (004D2D6B)
    if (*engaged == 0) return 0;                                                    // 004D2D60..6F
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target);               // 004D2D75..83
    if (o == nullptr || as_creature(o) == nullptr) return 0;                        // 004D2D88..97
    void* c = as_creature(o);                                                       // 004D2D99..9D (EAX)
    const U t50c = at<U>(c, 0x50c);                                                 // 004D2DA0
    if (t50c == k_object_invalid) master = 1;                                       // 004D2DA6..AE
    void* owner = at<void*>(round, 0x9b4);                                          // 004D2DB3 (EDX)
    if (t50c == at<U>(owner, 4) && at<U>(round_of(c), 0x9bc) == 0) master = 1;      // 004D2DB9..CE
    if (at<U>(owner, 0xa88) != 0) return 1;                                         // 004D2DD3..E5
    void* pc = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 004D2DE8..FA
    if (pc != nullptr) (void)kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr); // 004D2DFF..05 (unused)
    return master;
}

// 004D3E50 CSWSCombatRound::UpdateAttackTargetForAllAction(target): RET 4. Every queued round action of type 1
// (attack) with +18 == 1 gets +14 = target.
void __fastcall kotor_fastcall_004d3e50(void* round, void*, U target) {
    void* list = at<void*>(round, 0x9b0);                                           // 004D3E54..5C
    if (list == nullptr) return;
    base_ini_reconstruction::Node* position = at<base_ini_reconstruction::Node*>(at<void*>(list, 0), 0);      // 004D3E5E..68 ([esp+4])
    if (position == nullptr) return;
    do {
        void* action = unresolved_list_at_pos_005e9b60(at<void*>(at<void*>(round, 0x9b0), 0), nullptr, position); // 004D3E75..7E
        if (at<U8>(action, 0x10) == 1 && at<U>(action, 0x18) == 1) at<U>(action, 0x14) = target; // 004D3E83..8D
        base_ini_reconstruction::next_005e9b90(at<kotor_list::Internal*>(at<void*>(round, 0x9b0), 0), nullptr,
                                               &position);                          // 004D3E90..9D
    } while (position != nullptr);                                                  // 004D3EA2..A8
}

// =========================================================================================================
// 004F1310 CSWSCreature::DesiredAttackRange(target, flag): RET 8, ST0
double __fastcall kotor_fastcall_004f1310(void* self, void*, U target, U flag) {
    float range = 0.0f;                                                             // [esp+0C] (004F1325)
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target);               // 004F1319..2D
    if (o == nullptr) return range;                                                 // 004F1334..36 -> 004F143F
    if (as_creature(o) != nullptr) {                                                // 004F133C..47
        void* c = as_creature(o);                                                   // 004F134D..52
        if (flag != 0)                                                              // 004F1354..5A
            return (static_cast<double>(at<float>(at<void*>(c, 0x340), 8)) + at<float>(at<void*>(self, 0x340), 8)) +
                   k_personal_slack_00746f4c;                                       // 004F135C..7A
        void* table = at<void*>(at<void*>(rules(), 0xb8), 0xc);                     // appearance 2DA
        float value = 0.0f;                                                         // the target argument slot (004F1395)
        kotor_m15b_c2da_get_float_entry_00413350(table, nullptr, at<U16>(self, 0xa60), kotor_rules_cell32_007a210c,
                                                 &value);                           // 004F137D..A7
        range = static_cast<float>(static_cast<double>(value) + k_size_slack_00746f48); // 004F13AC..CD fstp [esp+0C]
        kotor_m15b_c2da_get_float_entry_00413350(at<void*>(at<void*>(rules(), 0xb8), 0xc), nullptr, at<U16>(c, 0xa60),
                                                 kotor_rules_cell32_007a210c, &value); // 004F13C2..DC (not re-zeroed)
        return static_cast<double>(value) + range;                                  // 004F13E2..E7
    }
    const Vector* p;
    if (as_door(o) != nullptr) p = position_of(as_door(o));                         // 004F13F2..02
    else if (as_placeable(o) != nullptr) p = position_of(as_placeable(o));          // 004F1404..0F
    else return k_eye_height_0073f414;                                              // 004F144C (1.5)
    Vector position = *p;                                                           // 004F1412..36 ([esp+10..18])
    w5_creature_get_use_range_004ee440(self, nullptr, target, &position, &range, 0); // 004F1420..3A
    return range;                                                                   // 004F143F
}

// 004FB0F0 CSWSCreature::MaxAttackRange(target, a2, ranged_only): RET 0Ch, ST0
double __fastcall kotor_fastcall_004fb0f0(void* self, void*, U target, U a2, U ranged_only) {
    void* item = recovered_equipment_item_005a4c20(inventory_of(self), nullptr, 0x10); // 004FB0F4..FC
    if (item != nullptr) {
        const U ranged = at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1a) != 0 ? 1u : 0u; // 004FB107..19
        if (static_cast<I>(ranged) > 0) {                                           // 004FB11B..1D (jle)
            if (ranged_only != 0) return k_ranged_only_range_007455d8;              // 004FB11F..2F
            const float range = at<float>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x34); // 004FB132..39
            // 004FB13C..47 fcom 0.0; test ah,44h; jp: equal -> 30.0, otherwise (also unordered) the item range
            if (range == k_zero_0073d700) return k_default_ranged_range_007455dc;
            return range;
        }
    }
    return kotor_fastcall_004f1310(self, nullptr, target, a2) + k_melee_extra_0073e9ac; // 004FB156..67
}

// 004FFAD0 CSWSCreature::MaxCleaveRange(a1, a2, a3): RET 0Ch, ST0 = MaxAttackRange(a1, a2, a3) + 2.0
double __fastcall kotor_fastcall_004ffad0(void* self, void*, U target, U a2, U a3) {
    return kotor_fastcall_004fb0f0(self, nullptr, target, a2, a3) + k_cleave_extra_00741be0; // 004FFAD0..E4
}

// =========================================================================================================
// 004D3E50 caller: 005B4970 CSWSCreature::InitializeNewAttackTarget(target, node): RET 8
void __fastcall kotor_fastcall_005b4970(void* self, void*, U target, void* node) {
    if (node != nullptr) at<U>(node, 0x38) = target;                                // 005B4970..80
    w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, target, 0);  // 005B4983..88
    at<U>(self, 0x84) = target;                                                     // 005B4994
    at<U>(self, 0x50c) = target;                                                    // 005B499A
    kotor_fastcall_004d3e50(round_of(self), nullptr, target);                       // 005B498D..A0
    at<U>(self, 0x4e8) = 1;                                                         // 005B49A6
}

// 005B48B0 CSWSCreature::CancelAllCombatModes(unused): RET 4
void __fastcall kotor_fastcall_005b48b0(void* self, void*, U) {
    w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, k_object_invalid, 0); // 005B48B1..BA
    vcall<U>(self, 0x7c, 0x2710u);                                                  // 005B48BF..C8
    w5_creature_set_combat_mode_0050ee80(self, nullptr, 0, 1);                      // 005B48CB..D1
    w5_combat_clear_all_special_attacks_004d4f60(round_of(self), nullptr);          // 005B48D6..DC
}

// 005B7B30 CSWSCreature::ResolveMoveToForceJump(target object, WORD* feat): RET 8, EAX 0/1.
// With the Force Jump feat (65h) and a lightsaber, a creature target at least 10 m away (squared 100) with a
// direct line applies the force-jump effect (type 66h, object 0 = target) and selects feat 67h / 66h / 65h.
U __fastcall kotor_fastcall_005b7b30(void* self, void*, void* target, U16* feat) {
    if (target == nullptr) return 0;                                                // 005B7B50..56
    if (as_creature(target) == nullptr) return 0;                                   // 005B7B5C..65
    if (*feat != 0) return 0;                                                       // 005B7B6B..72
    if (recovered_stats_has_feat_005a6630(at<void*>(self, 0xa74), nullptr, 0x65) == 0) return 0; // 005B7B78..87
    if (kotor_fastcall_005b5690(self, nullptr) == 0) return 0;                      // 005B7B8D..96
    Vector d;                                                                       // [esp+14]
    kotor_cdecl_00423bc0(&d, position_of(self), position_of(target));               // 005B7B9C..AF (self - target)
    const double d2 = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x; // 005B7BB4..D1
    if (d2 < k_jump_min_squared_00741838 || d2 != d2) return 0;                     // 005B7BD3..DE (test ah,1: below or unordered)
    at<U>(at<void*>(self, 0x340), 0x30) = at<U>(target, 4);                         // 005B7BE4..ED
    float radius = at<float>(at<void*>(self, 0x340), 4);                            // 005B7BF0..02 ([esp+10])
    alignas(4) unsigned char resref[16];                                            // CResRef [esp+14]
    reinterpret_cast<menu_resources::Ref*>(resref)->construct_00406d80(k_empty_0073d71c); // 005B7BF9..06
    const float band = at<float>(at<void*>(self, 0x340), 0x14);                     // 005B7C0B..21 (the target argument slot)
    if (w5_server_test_direct_line_004aee80(server_app(), nullptr, at<U>(self, 4), resref, position_of(self),
            position_of(target), &radius, band, 0, nullptr) != 1) return 0;          // 005B7C14..42
    void* memory = live_operator_new(0x8c);                                         // 005B7C48..52
    void* effect = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 005B7C59..72
    at<U16>(effect, 8) = k_effect_type_00746810;                                    // 005B7C74..7A (unchecked)
    recovered_effect_set_object_id_005036f0(effect, nullptr, 0, at<U>(target, 4));  // 005B7C7E..8E
    recovered_effect_integer_005036a0(effect, nullptr, 0, 0);                       // 005B7C93..99
    recovered_effect_apply_004d14f0(self, nullptr, effect, 0, 0);                   // 005B7C9E..A5
    if (recovered_stats_has_feat_005a6630(at<void*>(self, 0xa74), nullptr, 0x67) != 0) { // 005B7CAA..B9
        *feat = 0x67;                                                               // 005B7CBB..BF
        return 1;
    }
    *feat = static_cast<U16>((recovered_stats_has_feat_005a6630(at<void*>(self, 0xa74), nullptr, 0x66) != 0 ? 1 : 0) + 0x65); // 005B7CCB..E5
    return 1;
}

// =========================================================================================================
// 005BBA80 CSWSCreature::ResolveAttack(target, a2..a7): RET 1Ch
void __fastcall kotor_fastcall_005bba80(void* self, void*, U target, U a2, U a3, U a4, U a5, U a6, U a7) {
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target);               // 005BBA80..92
    if (o == nullptr ||                                                             // 005BBA99..9B
        (as_creature(o) == nullptr && as_door(o) == nullptr && as_placeable(o) == nullptr)) { // 005BBAA1..C0
        kotor_fastcall_004d28f0(round_of(self), nullptr, 0, k_object_invalid);      // 005BBBB6..C3
        kotor_fastcall_004d2920(round_of(self), nullptr, 0, 0);                     // 005BBBC8..D2
        vcall<U>(self, 0x7c, 0x2711u);                                              // 005BBBD7..E0
        return;
    }
    void* t = as_object(o);                                                         // 005BBAC6..CA (EDI)
    at<U>(self, 0x504) = target;                                                    // 005BBAD3
    w5_creature_activity_manager_004f87d0(self, nullptr, 4);                        // 005BBACD..D9
    void* item = recovered_equipment_item_005a4c20(inventory_of(self), nullptr, 0x10); // 005BBADE..E6
    // 005BBB01..0C: the ranged flag is stored over the target argument slot ([esp+10]); nothing reads it again
    if (item != nullptr && at<U8>(rules_base_item_of(item), 0x1a) != 0)            // 005BBAEB..10
        kotor_fastcall_005bb590(self, nullptr, t, a2, a3, a4, a5, a6, a7);          // 005BBB12..33
    else
        kotor_fastcall_005bb890(self, nullptr, t, a2, a3, a4, a5, a6, a7);          // 005BBB3A..5B
    if (as_creature(t) == nullptr) return;                                          // 005BBB60..69
    void* c = as_creature(t);                                                       // 005BBB6B..6F
    if (at<U>(self, 0xaa4) == 0) return;                                            // 005BBB72..7A
    if (at<U>(at<void*>(c, 0xa74), 0x6c) == 0 &&                                    // 005BBB7C..87
        recovered_party_table_leader_00563a00(party_table(), nullptr, at<U>(c, 4)) == 0) return; // 005BBB89..A4
    at<U>(self, 0xaa4) = 0xffffffffu;                                               // 005BBBA7
}

// =========================================================================================================
// 005BBBF0 CSWSCreature::AIActionPhysicalAttacks(node): RET 4.
// Node parameters: +38 a0 (int), +3C target, +40 a2, +44 a3 (animation), +48 a4 (round pause time), +4C a5,
// +50 a6 (word), and when a0 != 0 also +54/+58/+5C (otherwise 0 / 4 / 0). +6C group.
U __fastcall w5_action_physical_attacks_005bbbf0(void* self, void*, void* node) {
    at<U>(at<void*>(self, 0x340), 0xc) = 0;                                         // 005BBBFB..2E
    at<U>(at<void*>(self, 0x340), 0x38) = k_object_invalid;                         // 005BBC38..3E
    at<U>(self, 0x52c) = 0xbb8;                                                     // 005BBC45
    U target = at<U>(node, 0x3c);                                                   // [esp+10]
    U p94 = at<U>(node, 0x40);                                                      // [esp+94]
    U p5c = at<U>(node, 0x44);                                                      // [esp+5C] (animation)
    U p68 = at<U>(node, 0x48);                                                      // [esp+68]
    U p98 = at<U>(node, 0x4c);                                                      // [esp+98]
    U p50 = at<U16>(node, 0x50);                                                    // [esp+50] (zero-extended word)
    U p6c = at<U>(node, 0x38);                                                      // [esp+6C]
    U p7c = 0, p8c = 4, p90 = 0;                                                    // 005BBC84..93
    if (p6c != 0) { p7c = at<U>(node, 0x54); p8c = at<U>(node, 0x58); p90 = at<U>(node, 0x5c); } // 005BBC9A..B8
    U pac = 1;                                                                      // [esp+AC] (005BBC23)
    if (at<U>(self, 0xa88) != 0) party_combat_message(self, 0xbc50);                // 005BBCBF..D06
    // ---- the attacker must be able to act; otherwise the combat modes are cancelled inline (= 005B48B0)
    if (vcall<U>(self, 0x94) != 0 || kotor_m15e_slot010_zero_0063e7f0(self, nullptr) != 0 ||
        recovered_creature_incapacitated_004ef890(self, nullptr) != 0 ||            // 005BBD0B..35
        (at<U8>(self, 0x9f0) & 0x80) == 0 || (at<U8>(self, 0x9f0) & 4) == 0 ||      // 005BBD3B..4C
        target == at<U>(self, 4)) {                                                 // 005BBD52..59
        w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, k_object_invalid, 0); // 005BCA73..7B
        vcall<U>(self, 0x7c, 0x2710u);                                              // 005BCA80..89
        w5_creature_set_combat_mode_0050ee80(self, nullptr, 0, 1);                  // 005BCA8C..91
        w5_combat_clear_all_special_attacks_004d4f60(round_of(self), nullptr);      // 005BCA96..9C
        return k_failed_00745dc4;
    }
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, target);             // 005BBD5F..69 (EBX, [esp+60])
    void* t = nullptr;                                                              // EDI
    U detected = 0;                                                                 // EBP
    if (obj != nullptr && as_object(obj) != nullptr) {                              // 005BBD70..83
        t = as_object(obj);                                                         // 005BBD85..8C
        U dead = 0;                                                                 // EBX
        if (as_creature(t) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(t), nullptr); // 005BBD8E..A9
        if (vcall<U>(t, 0x94) == 0 && dead == 0)                                    // 005BBDAB..BB
            detected = kotor_fastcall_005b48f0(self, nullptr, target, 1);           // 005BBDBD..CB
    }
    U ranged_flag = 0;                                                              // [esp+4C]
    if (detected == 0) return k_complete_00745dc0;                                  // 005BBDCD..D7 -> 005BCA18
    Vector approach = *position_of(t);                                              // [esp+24..2C] (005BBDDD..F5)
    void* target_area = recovered_object_area_004cb120(t, nullptr);                 // 005BBDF9 -> [esp+18]
    const U choreographed = kotor_fastcall_004d2b70(round_of(self), nullptr, target); // 005BBDFE..12 (EBX)
    if (choreographed != 0) p5c = 0x277d;                                           // 005BBE14..18
    if (as_door(t) != nullptr) {                                                    // 005BBE20..29
        float door_range = 0.0f;                                                    // [esp+1C] (005BBE37)
        at<U>(at<void*>(self, 0x340), 0x38) = target;                               // 005BBE3F
        w5_creature_get_use_range_004ee440(self, nullptr, target, &approach, &door_range, 1); // 005BBE35..53
        at<float>(at<void*>(self, 0x340), 0xc) = door_range;                        // 005BBE58..62
    }
    const float max_range = static_cast<float>(kotor_fastcall_004fb0f0(self, nullptr, target, choreographed, 0)); // 005BBE65..74 [esp+3C]
    float desired = static_cast<float>(kotor_fastcall_004f1310(self, nullptr, target, choreographed)); // 005BBE78..85 [esp+14]
    const Vector* self_position = position_of(self);                                // EBP (005BBE89)
    Vector eye = *self_position;                                                    // [esp+9C..A4]
    eye.z = static_cast<float>(static_cast<double>(eye.z) + k_eye_height_0073f414); // 005BBEA0..BF
    U los = 0;                                                                      // [esp+20] (005BBEC6)
    U blocker = k_object_invalid;                                                   // [esp+A8] (005BBECE)
    Vector aim = vec_of_bits(0, 0, 0);                                              // [esp+70..78]
    const U8 type = at<U8>(t, 8);                                                   // 005BBEBA
    if (type == k_type_door_00746639 || type == k_type_placeable_00746638) {        // 005BBEBD..00
        float use_range;                                                            // [esp+58] (written by GetUseRange)
        w5_creature_get_use_range_004ee440(self, nullptr, at<U>(t, 4), &aim, &use_range, 0); // 005BBF1E..30
        approach = aim;                                                             // 005BBF35..49
    } else {
        aim = *position_of(t);                                                      // 005BBF02..1C
    }
    aim.z = static_cast<float>(static_cast<double>(aim.z) + k_eye_height_0073f414); // 005BBF4D..7A
    Vector hit_point = vec_of_bits(0, 0, 0);                                        // [esp+B0..B8]
    if (recovered_object_area_004cb120(self, nullptr) != nullptr) {                 // 005BBF7E..85
        eye.z = static_cast<float>(static_cast<double>(eye.z) + k_los_bias_0073dfec);   // 005BBF8B..9F
        aim.z = static_cast<float>(static_cast<double>(aim.z) - k_los_bias_0073dfec);   // 005BBFA6..B0
        const U self_id = at<U>(self, 4);                                           // 005BBF92 (EBX)
        const U rwe = kotor_fastcall_004ebb30(self, nullptr);                       // 005BBFB4
        los = static_cast<U>(area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(self, nullptr), nullptr,
            &eye, &aim, &hit_point, &blocker, self_id, target, static_cast<int>(rwe), 0)); // 005BBFB9..F0
        if (kotor_fastcall_004ebb30(self, nullptr) == 0 && los != 0)                // 005BBFEE..03
            (void)as_creature(t);                                                   // 005BC005..09 (result unused)
        if (los == 0) {                                                             // 005BC00C..12
            float* last = &at<float>(self, 0x514);                                  // EBX = self+514
            if (aurora_vector_equal_004aa980(last, reinterpret_cast<const float*>(self_position)) != 0 && // 005BC018..2A
                at<U>(self, 0x9d4) != 0) {                                          // 005BC030..38
                recovered_creature_feedback_004ede10(self, nullptr, 0xda, nullptr); // 005BC03E..47
                at<U>(self, 0x514) = 0; at<U>(self, 0x518) = 0; at<U>(self, 0x51c) = 0; // 005BC04C..89 (via [esp+40..48])
                at<U>(self, 0x504) = k_object_invalid;                              // 005BC08C
                const float enemy_range = static_cast<float>(kotor_fastcall_004fb0f0(self, nullptr, at<U>(self, 4), 0, 1)); // 005BC096..9E
                const U enemy = kotor_fastcall_004f2de0(self, nullptr, enemy_range, target, k_object_invalid); // 005BC0A1..A6
                if (enemy != k_object_invalid) {                                    // 005BC0A8..AE
                    void* e = kotor_fastcall_004ae750(server_app(), nullptr, enemy);    // 005BC0B0..BA
                    if (e != nullptr && at<U8>(e, 8) == k_type_creature_00746634) { // 005BC0BF..CC
                        kotor_fastcall_005b4970(self, nullptr, enemy, node);        // 005BC0CE..D9
                        return k_in_progress_00745dbc;                              // 005BC0DE
                    }
                }
                kotor_fastcall_005b48b0(self, nullptr, 1);                          // 005BC0F0..F4
                return k_complete_00745dc0;
            }
            std::memcpy(last, self_position, 12);                                   // 005BC10B..1D
        } else {
            at<U>(self, 0x514) = 0; at<U>(self, 0x518) = 0; at<U>(self, 0x51c) = 0; // 005BC11F..4E
        }
    }
    // ---- 005BC151: planar distance to the approach point; a back-off point when closer than the desired range
    Vector d;                                                                       // [esp+40]
    kotor_cdecl_00423bc0(&d, self_position, &approach);                             // 005BC151..5C (self - approach)
    const double dist2 = static_cast<double>(d.y) * d.y + static_cast<double>(d.x) * d.x; // 005BC161..95
    const float dist2f = static_cast<float>(dist2);                                 // 005BC197 fst [esp+64]
    Vector back_off = vec_of_bits(0, 0, 0);                                         // [esp+80..88] (005BC170..8A)
    if (dist2 < static_cast<double>(desired) * desired - k_range_slack_0073e46c) {  // 005BC19B..B2 (test ah,5; jp)
        Vector toward = vec_of_bits(0, 0, 0);                                       // [esp+30]
        toward.x = static_cast<float>(-static_cast<double>(d.x));                   // 005BC1B8..C3
        toward.y = static_cast<float>(-static_cast<double>(d.y));                   // 005BC1CB..DA
        toward.z = 0.0f;                                                            // 005BC1D2
        Vector unit;                                                                // [esp+40]
        normalize_004ab180(&unit, &toward);                                         // 005BC1DE
        vector_component_mult_00423bf0(&toward, &unit, desired);                    // 005BC1E3..F2
        Vector out;                                                                 // [esp+BC]
        kotor_cdecl_00423bc0(&out, position_of(t), &toward);                        // 005BC1F7..07
        back_off = out;                                                             // 005BC20C..2C
        WalkHit hit;                                                                // [esp+C8]
        w5_walk_hit_construct_004b14b0(&hit, nullptr);                              // 005BC225..33
        float radius = at<float>(at<void*>(self, 0x340), 4);                        // 005BC238..41 ([esp+1C])
        const float band = at<float>(at<void*>(self, 0x340), 0x14);                 // 005BC245..4A ([esp+58])
        void* area = recovered_object_area_004cb120(self, nullptr);                 // 005BC24E
        if (w5_server_test_direct_line_004aee80(server_app(), nullptr, at<U>(self, 4), area_offset(area, 0x10c),
                self_position, &back_off, &radius, band, 1, &hit) != 1) {            // 005BC253..8E
            back_off = hit.point_4;                                                 // 005BC290..CA
            Vector e;                                                               // [esp+BC]
            kotor_cdecl_00423bc0(&e, &back_off, position_of(t));                    // 005BC2B3..D1
            desired = static_cast<float>(vector_magnitude_004ab100(&e));            // 005BC2D6..DC fstp [esp+14]
        }
    }
    // ---- 005BC2E3: in range (same area, within the maximum range, not too close or a party member) with a line of
    // sight -> attack this round; otherwise queue the approach (or back-off) and re-queue the attack
    const double max2 = static_cast<double>(max_range) * max_range;
    bool attack = false;
    if (target_area == recovered_object_area_004cb120(self, nullptr) &&             // 005BC2E3..F0
        !(max2 < static_cast<double>(dist2f))) {                                    // 005BC2F2..03 (test ah,5; jnp)
        const double near2 = static_cast<double>(desired) * desired - k_range_slack_0073e46c; // 005BC305..0D
        const bool not_too_close = near2 <= static_cast<double>(dist2f) || near2 != near2; // 005BC313..1C (test ah,41h)
        if ((not_too_close || at<U>(self, 0xa88) != 0) && los != 0) attack = true;  // 005BC31E..2E
    }
    U jump_result = 0;                                                              // EBX on the attack path
    if (!attack) {
        ranged_flag = kotor_fastcall_004ebb30(self, nullptr);                       // 005BC334..43 -> [esp+4C]
        U too_close = 0;                                                            // [esp+18] (005BC347)
        {
            const double near2 = static_cast<double>(desired) * desired - k_range_slack_0073e46c; // 005BC33B..4F
            if (!(near2 <= static_cast<double>(dist2f) || near2 != near2)) too_close = 1; // 005BC355..60
        }
        if (at<U>(self, 0x4e8) != 0) {                                              // 005BC368..70
            kotor_fastcall_005b48b0(self, nullptr, 1);                              // 005BCA58..5C
            return k_failed_00745dc4;
        }
        U area_id;                                                                  // [esp+1C]
        if (target_area != nullptr) {                                               // 005BC376..78
            area_id = at<U>(target_area, 0x120);                                    // 005BC37A..80
        } else {
            if (as_creature(t) == nullptr || at<U>(self, 0xa24) != k_object_invalid) { // 005BC386..9F
                kotor_fastcall_005b48b0(self, nullptr, 1);                          // 005BCA58
                return k_failed_00745dc4;
            }
            area_id = at<U>(as_creature(t), 0x310);                                 // 005BC3A5..B4
            approach = *reinterpret_cast<const Vector*>(static_cast<unsigned char*>(as_creature(t)) + 0x314); // 005BC3B8..D2
        }
        bool jump = false;
        if (los != 0) {                                                             // 005BC3D6..DC
            jump_result = kotor_fastcall_005b7b30(self, nullptr, t, reinterpret_cast<U16*>(&p50)); // 005BC3DE..EB
            jump = jump_result != 0;                                                // 005BC3ED..EF -> 005BC479
        }
        if (!jump) {
            if (as_door(obj) != nullptr) {                                          // 005BC3F5..02
                float use_range;                                                    // [esp+20] (the line-of-sight slot)
                w5_creature_get_use_range_004ee440(self, nullptr, target, &approach, &use_range, 1); // 005BC408..1B
                desired = use_range;                                                // 005BC420..2D [esp+14]
                Vector point;                                                       // [esp+30]
                w5_door_get_nearest_action_point_00589240(as_door(obj), nullptr, &point, self_position, 0); // 005BC424..39
                approach = point;                                                   // 005BC43E..5C
                at<U>(at<void*>(self, 0x340), 0x38) = target;                       // 005BC460
                at<float>(at<void*>(self, 0x340), 0xc) = use_range;                 // 005BC463..6D
            } else if (as_placeable(obj) != nullptr) {                              // 005BC74F..58
                float use_range;                                                    // [esp+60] (over the object pointer slot)
                w5_creature_get_use_range_004ee440(self, nullptr, target, &approach, &use_range, 1); // 005BC75A..6D
                desired = use_range;                                                // 005BC772..80 [esp+14]
                Vector point;                                                       // [esp+30]
                w3_placeable_nearest_action_point_00584c20(as_placeable(obj), nullptr, &point, self_position); // 005BC776..89
                approach = point;                                                   // 005BC78E..A2
                too_close = 0;                                                      // 005BC7A6
            }
            // ---- 005BC7AE: re-queue this attack at the front (type 0Ch) with the same parameters
            const U group = at<U16>(node, 0x6c);                                    // 005BC811..13
            {
                const U q[26] = {1, addr(&p6c), 3, addr(&target), 1, addr(&p94), 1, addr(&p5c), 1, addr(&p68),
                                 1, addr(&p98), 1, addr(&p50), 1, addr(&p7c), 1, addr(&p8c), 1, addr(&p90)};
                add_action_front(self, 0x0c, group, q);                             // 005BC7AE..2D
            }
            U run = 1;                                                              // [esp+58] (005BC838)
            Vector zero = vec_of_bits(0, 0, 0);                                     // [esp+30..38] (005BC99C..AC)
            if (too_close != 0) {                                                   // 005BC832..40 -> 005BCA2A: back off
                w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, at<U16>(node, 0x6c), &back_off,
                    area_id, k_object_invalid, 0, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 005BCA2A..53, 005BC98E..B5
            } else {
                U zero60 = 0;                                                       // [esp+60] (005BC850)
                float range = ranged_flag != 0 ? max_range : desired;               // [esp+18] (005BC846..62)
                {
                    const U q[26] = {3, addr(&target), 1, addr(&run), 2, addr(&range), 2, addr(&max_range),
                                     1, addr(&pac), 1, addr(&zero60), 1, addr(&ranged_flag)};
                    add_action_front(self, 0x11, group, q);                         // 005BC866..C6 (CheckMoveToObject)
                }
                {
                    const U q[26] = {3, addr(&target)};
                    add_action_front(self, 0x13, at<U16>(node, 0x6c), q);           // 005BC8CB..0D (ChangeFacingObject)
                }
                range = ranged_flag != 0 ? max_range : desired;                     // 005BC912..26
                if (as_creature(t) != nullptr) {                                    // 005BC92A..33
                    at<float>(at<void*>(self, 0x340), 0xc) = static_cast<float>(
                        static_cast<double>(desired) - at<float>(at<void*>(as_creature(t), 0x340), 8)); // 005BC935..4F
                    at<U>(at<void*>(self, 0x340), 0x38) = target;                   // 005BC952..5C
                }
                w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, at<U16>(node, 0x6c), &approach,
                    area_id, target, run, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, ranged_flag); // 005BC95F..B5
            }
            if (at<U>(self, 0xa88) != 0 && static_cast<double>(dist2f) > k_quarter_0073edf0) // 005BC9BA..D3 (test ah,41h)
                party_combat_message(self, 0xa5ed);                                 // 005BC9D5..A13
            return k_complete_00745dc0;                                             // 005BCA18
        }
    } else {
        jump_result = ranged_flag;                                                  // 005BC475 (EBX = [esp+4C] = 0)
    }
    // ---- 005BC479: attack this round
    if (round_of(self) == nullptr) {                                                // 005BC479..81
        kotor_fastcall_005b48b0(self, nullptr, 1);                                  // 005BCA58
        return k_failed_00745dc4;
    }
    if (frame_GetPauseState_004ae980(server_app(), nullptr, 2) != 0) return k_in_progress_00745dbc; // 005BC487..99
    if (at<U>(round_of(self), 0x958) != 0) return k_in_progress_00745dbc;           // 005BC49F..AD
    U engaged = kotor_fastcall_004d2c30(round_of(self), nullptr, target);           // 005BC4B3..C7 -> [esp+54]
    const U master = kotor_fastcall_004d2d60(round_of(self), nullptr, target, &engaged); // 005BC4BD..D6 (EBP)
    if (engaged != 0) {                                                             // 005BC4D8..DE
        if (kotor_fastcall_005b47c0(self, nullptr) != 0 ||                          // 005BC4E4..ED
            (as_creature(t) != nullptr && kotor_fastcall_005b47c0(as_creature(t), nullptr) != 0)) // 005BC4EF..0A
            p5c = 0x277d;                                                           // 005BC50C
        if (master != 0) {                                                          // 005BC514..16
            kotor_fastcall_004d5f70(round_of(self), nullptr, target, engaged, master, p50, p6c); // 005BC51C..37
            void* tc = kotor_fastcall_004ae770(server_app(), nullptr, target);      // 005BC53C..4F
            if (tc != nullptr) {                                                    // 005BC551..53
                kotor_fastcall_004d5f70(round_of(tc), nullptr, at<U>(self, 4), engaged, 0, 0, p6c); // 005BC555..6D
                at<U>(tc, 0x4dc) = 1;                                               // 005BC572
            }
        }
    } else {
        p5c = 0x2719;                                                               // 005BC596
        kotor_fastcall_004d5f70(round_of(self), nullptr, target, 0, master, p50, p6c); // 005BC57E..9E
    }
    if (at<U>(round_of(self), 0x934) == 0) return k_in_progress_00745dbc;           // 005BC5A3..B1
    if ((p94 == 0x0b || jump_result != 0) && at<U16>(self, 0x8e0) == 0 &&           // 005BC5B7..CD
        recovered_equipment_item_005a4c20(inventory_of(self), nullptr, 0x10) != nullptr) { // 005BC5CF..DE
        void* attack = recovered_combat_round_attack_004d2b50(round_of(self), nullptr, at<U8>(round_of(self), 0x96c)); // 005BC5E0..EE
        at<U16>(attack, 0x64) = static_cast<U16>(p50);                              // 005BC5F3..F8
    }
    vcall<U>(self, 0x7c, p5c);                                                      // 005BC5FC..05
    at<U>(self, 0x50c) = target;                                                    // 005BC610
    kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));            // 005BC608..1E
    kotor_fastcall_004d2920(round_of(self), nullptr, p68, 0);                       // 005BC623..30
    at<I>(round_of(self), 0x968) += 1;                                              // 005BC635..3B
    if (engaged != 0) {                                                             // 005BC641..47
        if (master != 0) {                                                          // 005BC649..51
            void* c = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(round_of(self), 0x9c4)); // 005BC653..63
            if (c != nullptr) {                                                     // 005BC66A..6C
                kotor_fastcall_004d28f0(round_of(c), nullptr, 1, at<U>(round_of(c), 0x9c0)); // 005BC66E..7D
                kotor_fastcall_004d2920(round_of(c), nullptr, p68, 1);              // 005BC682..BF
            }
        } else {
            void* c = kotor_fastcall_004ae770(server_app(), nullptr, target);       // 005BC68B..93
            if (c != nullptr) {                                                     // 005BC69A..9C
                kotor_fastcall_004d28f0(round_of(c), nullptr, 1, at<U>(round_of(c), 0x9c4)); // 005BC69E..AD
                kotor_fastcall_004d2920(round_of(c), nullptr, p68, 0);              // 005BC6B2..BF
            }
        }
    }
    const U next = at<U>(round_of(self), 0x98c);                                    // 005BC6C4..CA (EDI)
    if (next != k_object_invalid) {                                                 // 005BC6D0..D6
        kotor_fastcall_005b4970(self, nullptr, next, node);                         // 005BC6D8..E3
        at<U>(round_of(self), 0x98c) = k_object_invalid;                            // 005BC6E8..EE
        target = next;                                                              // 005BC6F8
    }
    w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, target, 0);  // 005BC6FC..05
    kotor_fastcall_005bba80(self, nullptr, target, p98, p68, p6c, p7c, p8c, p90);   // 005BC70A..38
    return k_complete_00745dc0;                                                     // 005BC73D
}
}  // extern "C"
