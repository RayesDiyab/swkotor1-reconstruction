// Lane endar-actions, group "movement": NWScript engine-command bodies that queue movement / combat / talent
// actions (CSWVirtualMachineCommands::ExecuteCommand*), plus the callees that had no implementation anywhere.
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// Original handler ABI: int __thiscall ExecuteCommandX(int routine, int argc), RET 8 (verified for every body
// below) -> extern "C" int __fastcall recovered_vm_<name>_<addr>(self, edx, routine, argc) = @...@16.
// CSWVirtualMachineCommands fields used: +4 "script runs on an object" flag (== 1), +8 OBJECT_SELF id.
// .rdata constants used as proven literals: [0074666C] (byte) 0 = CGameObjectArray "found" status,
// [00746644] 0x7F000000 OBJECT_INVALID, [00746633] (byte) 4 = object type area, [00746B10] 40.0f.
#include "061872cb05-claude_vm_actions_common.hpp"
#include <initializer_list>

namespace kotor_combat_round_cells { extern volatile std::int32_t word_008327dc; }   // [008327DC] [round_queue]

extern "C" {
// ---- existing providers (c17 map, object in brackets) not already declared by the W5 headers ----
U __fastcall kotor_fastcall_004fde40(void* creature, void*, U target, U mode, U round_arg, U state, U clear, U front,
    U immediate, U p8, U p9, U p10, U p11, U p12, U p13, U p14, U p15);                    // AddAttackActions [attack_producer]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);  // CSWSObject::ClearAllActions [1206-07 w5_drive_control]
void* __fastcall kotor_fastcall_004cc630(void* object, void*, U type);                     // CSWSObject::GetNodeById [producer_helpers]
void __fastcall kotor_fastcall_004d1d50(void* action, void*);                              // CSWSCombatRoundAction::ClearData [round_queue]
void __fastcall kotor_fastcall_004d3660(void* round, void*, void* action, I index);        // CSWSCombatRound::AddAction [round_queue]
U __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const void* point, U area_id,
    U target_id, U run, U range, U a6, U a7, U a8, U a9, U a10, U a11, U a12, const void* a13, U a14, U a15, U a16);  // [vm_actions_t5]

U __fastcall kotor_fastcall_004fbe40(void* creature, void*, U skill, U a2, U target, const void* position, U area_id,
    U a6, U a7);                                                                            // CSWSCreature::UseSkill [skill_producer]
U __fastcall kotor_fastcall_004f9460(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7, U a8, U a9,
    U a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18);                       // AddCastSpellActions [claude_w5_combat_actions]
U __fastcall kotor_fastcall_004f8c70(void* creature, void*, U b1, U b2, U b3, U b4, U b5, U b6, U b7, U b8, U b9,
    U b10, U b11);                                                                          // AddItemCastSpellActions [claude_w5_combat_actions]
U __fastcall recovered_creature_can_use_item_0051b440(void* creature, void*, void* item, U flag);   // CanUseItem [w2_g07_native]
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, U index);               // CSWSItem::GetActiveProperty [menu_active_property]
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);          // GetVisibleListElement [w1_native_b7]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U id);       // CSWBaseItemArray::GetBaseItem [0820]
U __fastcall kotor_fastcall_005089d0(void* area, void*);                                            // CSWSArea::GetPlayerCanSetMines [trap_limit]
U __fastcall recovered_stats_can_use_skill_005af880(void* stats, void*, U skill);                   // GetCanUseSkill [w2_g07_native]
U __fastcall recovered_item_property_has_00555c90(void* item, void*, U type, U subtype);           // GetPropertyByTypeExists [1025]
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                     // CItemRepository::ItemListGetItem [0886]
unsigned __fastcall kt_get_class_005a4e90(void* stats, void*, unsigned index);                      // CSWSCreatureStats::GetClass (AL) [provider_adapters]
unsigned __fastcall kt_is_jedi_005be4a0(void*, void*, unsigned class_id);                           // CSWClass::IsJedi (cdecl original) [provider_adapters]
unsigned char __fastcall recovered_stats_spell_uses_left_005a5550(void* stats, void*, U a, U b, U c, U d); // GetSpellUsesLeft (AL) [talent_lowers]
int __fastcall recovered_stats_in_spell_like_005a68e0(void* stats, void*, U id);                    // GetIsInSpellLikeAbilityList [talent_lowers]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);            // CSWSpellArray::GetSpell [0820]
U __fastcall kotor_fastcall_0059b650(void* spell, void*, U cls);                                    // CSWSpell::GetSpellLevel (AL) [claude_combat_spell]
void* __fastcall w5_server_get_door_by_id_004ae7c0(void* server, void*, U id);                      // GetDoorByGameObjectID [pf_w5_drive]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);                    // GetPlaceableByGameObjectID [0886]
unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, U skill, U versus, U base_only); // GetSkillRank (AL) [0886]
extern void* kotor_cell_rules_007a3a28;                                                              // g_pRules 007A3A28 [0421]

// ---- reconstructed here (no implementation in any lane) ----
U __fastcall kotor_fastcall_004d3810(void* round, void*, U target, U animation, U result, U damage);
U __fastcall kotor_fastcall_004f9fb0(void* creature, void*, U target, U animation, U result, U damage);
U __fastcall kotor_fastcall_00500ca0(void* creature, void*, U type, U id, U target, U a4, U a5, U a6, U a7, U a8);
U __fastcall kotor_fastcall_004fd070(void* creature, void*, U type, U id, const void* position, U a4, U a5, U a6, U a7, U a8);
U __fastcall kotor_fastcall_004fc210(void* creature, void*, U item_id, U property, U a3, U target, const void* position, U area_id, U a7);
U __fastcall kotor_fastcall_005a74a0(void* stats, void*, U spell, U* item_out, U* property_out);
U __fastcall kotor_fastcall_005a6e30(void* stats, void*, U spell);
U __fastcall kotor_fastcall_00584db0(void* placeable, void*, U spell, U a2, const void* position, U target, U a5, U a6);
}

namespace {
using namespace vmx;
constexpr U k_found_0074666c = 0;            // [0074666C] byte
constexpr U k_type_area_00746633 = 4;        // [00746633] byte
const float k_move_away_range_00746b10 = 40.0f;   // [00746B10]

struct Pair { U type; void* value; };
// CSWSObject::AddAction(type, group, 13 (parameter type, parameter pointer) pairs); unused pairs are (0, 0).
U add_action(void* object, U type, U group, std::initializer_list<Pair> list) {
    Pair p[13] = {};
    U i = 0;
    for (const Pair& x : list) p[i++] = x;
    return recovered_object_add_action_004cea20(object, nullptr, type, group, p[0].type, p[0].value, p[1].type, p[1].value,
        p[2].type, p[2].value, p[3].type, p[3].value, p[4].type, p[4].value, p[5].type, p[5].value, p[6].type, p[6].value,
        p[7].type, p[7].value, p[8].type, p[8].value, p[9].type, p[9].value, p[10].type, p[10].value, p[11].type, p[11].value,
        p[12].type, p[12].value);
}
U add_action_to_front(void* object, U type, U group, std::initializer_list<Pair> list) {
    Pair p[13] = {};
    U i = 0;
    for (const Pair& x : list) p[i++] = x;
    return recovered_object_add_action_to_front_004ccb00(object, nullptr, type, group, p[0].type, p[0].value, p[1].type, p[1].value,
        p[2].type, p[2].value, p[3].type, p[3].value, p[4].type, p[4].value, p[5].type, p[5].value, p[6].type, p[6].value,
        p[7].type, p[7].value, p[8].type, p[8].value, p[9].type, p[9].value, p[10].type, p[10].value, p[11].type, p[11].value,
        p[12].type, p[12].value);
}
// OBJECT_SELF lookup exactly as the handlers do it: [self+4] == 1, then CGameObjectArray::GetGameObject([self+8])
bool self_object(void* self, void** out) {
    if (at<U>(self, 4) != 1) return false;
    return static_cast<U8>(kotor_fastcall_004d8230(object_array(), nullptr, at<U>(self, 8), out)) == k_found_0074666c;
}
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }     // vtbl+10 AsSWSObject
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }   // vtbl+30 AsSWSCreature
// CScriptLocation (0x18 bytes: position, orientation) popped as engine structure 2; released with its (empty,
// 004015A0 void_return) destructor and free (006FA390 -> live_free).
void free_location(void* location) { live_free(location); }
// a read the original performs through a possibly-null pointer (kept: faults exactly like the original)
inline U read_through(U address) { return *reinterpret_cast<volatile U*>(static_cast<std::uintptr_t>(address)); }
inline U8 read8_through(U address) { return *reinterpret_cast<volatile U8*>(static_cast<std::uintptr_t>(address)); }
}  // namespace

extern "C" {

// =========================================================================================================
// 00545800 ExecuteCommandActionWait (202 ActionWait(float seconds)): queue action 1Eh on OBJECT_SELF.
int __fastcall recovered_vm_action_wait_00545800(void* self, void*, int, unsigned) {
    float seconds;                                                                     // [esp+8]
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &seconds)) return k_pop_failed;   // 00545804..23
    void* object = nullptr;
    if (!self_object(self, &object)) return 0;                                         // 00545826..54
    if (as_object(object) == nullptr) return 0;                                        // 00545856..61
    if (at<U>(as_object(object), 0xe8) == 0) return 0;                                 // 00545863..74 (+E8 area)
    (void)add_action(as_object(object), 0x1e, 0xffff, {{2, &seconds}});                // 00545876..BF
    return 0;
}

// =========================================================================================================
// 0052E7F0 ExecuteCommandAttack (37 ActionAttack(object target, int passive = FALSE))
int __fastcall recovered_vm_action_attack_0052e7f0(void* self, void*, int, unsigned argc) {
    U target = 0, passive = 0;                                                         // [esp+8] / [esp+4] = 0
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed;    // 0052E7F4..10
    if (static_cast<I>(argc) >= 2 &&
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&passive))) return k_pop_failed; // 0052E812..2B
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 8));   // 0052E839..46
    if (creature == nullptr) return 0;                                                 // 0052E84B..4D
    // 0052E84F..7B: AddAttackActions(target, 0, 0, passive, 0, 0, 0, 1, 2719h, 5DCh, 1, 0, 0, 4, 0)
    (void)kotor_fastcall_004fde40(creature, nullptr, target, 0, 0, passive, 0, 0, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
    return 0;
}

// =========================================================================================================
// 0052E890 ExecuteCommandCutsceneAttack (503 CutsceneAttack(object target, int animation, int result, int damage))
int __fastcall recovered_vm_cutscene_attack_0052e890(void* self, void*, int, unsigned) {
    U target = k_invalid, animation = 0, result = 0, damage = 0;                       // 0052E8A1..B9
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target) ||
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&animation)) ||
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&result)) ||
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&damage))) return k_pop_failed; // 0052E8C1..0F
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 8));   // 0052E912..1F
    if (creature != nullptr) (void)kotor_fastcall_004f9fb0(creature, nullptr, target, animation, result, damage);   // 0052E924..3E
    return 0;
}

// =========================================================================================================
// 0052CDC0 ExecuteCommandActionJumpToPoint (214 ActionJumpToLocation(location) -> AddAction;
// 313 JumpToLocation(location) -> AddActionToFront). An optional second int (default 1) is popped when argc >= 2.
int __fastcall recovered_vm_action_jump_to_point_0052cdc0(void* self, void*, int routine, unsigned argc) {
    float position[3] = {-1.0f, -1.0f, -1.0f};                                         // [esp+14..1C] = BF800000
    float orientation[3] = {0.0f, 0.0f, 0.0f};                                         // [esp+20..28] = 0
    void* location = nullptr;                                                          // [esp+C]
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 2, &location)) return k_pop_failed; // 0052CDC4..15
    std::memcpy(position, location, 12);                                               // 0052CE18..2E
    std::memcpy(orientation, static_cast<unsigned char*>(location) + 0xc, 12);         // 0052CE32..46
    free_location(location);                                                           // 0052CE4A..52 (dtor 004015A0 is empty)
    U flag = 0;                                                                        // [esp+C] reused: 0 (0052CE61)
    if (static_cast<I>(argc) >= 2) {                                                   // 0052CE5E..6E
        if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&flag))) return k_pop_failed; // 0052CEEA..0C
    } else {
        flag = 1;                                                                      // 0052CE70
    }
    void* object = nullptr;
    if (!self_object(self, &object)) return 0;                                         // 0052CE74..A1
    if (as_creature(object) == nullptr) return 0;                                      // 0052CEA7..B2
    if (at<U>(as_creature(object), 0xe8) == 0) return 0;                               // 0052CEB8..C9
    void* area = recovered_object_area_004cb120(as_creature(object), nullptr);         // 0052CECF..DA
    const U base = area != nullptr ? word(area) + 0x11c : 0;                        // 0052CEDF..0F (CGameObject part of the area)
    U area_id = read_through(base + 4);                                                // 0052CF11..14 (null area faults, as the original)
    if (read8_through(base + 8) != k_type_area_00746633) return 0;                     // 0052CF18..21
    float speed = 20.0f;                                                               // 0052CF41: argc slot = 41A00000
    if (routine == 0xd6) {                                                             // 0052CF27 / 0052CF4B
        (void)add_action(object, 5, 0xffff, {{2, &position[0]}, {2, &position[1]}, {2, &position[2]}, {3, &area_id}, {1, &flag},
                                             {2, &speed}, {2, &orientation[0]}, {2, &orientation[1]}});   // 0052CF4D..8F
    } else {
        (void)add_action_to_front(object, 5, 0xffff, {{2, &position[0]}, {2, &position[1]}, {2, &position[2]}, {3, &area_id}, {1, &flag},
                                                      {2, &speed}, {2, &orientation[0]}, {2, &orientation[1]}});   // 0052CF9E..E0
    }
    return 0;
}

// =========================================================================================================
// 0053F990 ExecuteCommandActionMoveAwayFromObject (23 ActionMoveAwayFromObject(object, int run = FALSE, float range = 40.0))
int __fastcall recovered_vm_action_move_away_from_object_0053f990(void* self, void*, int, unsigned argc) {
    U target = 0, run = 0;                                                             // [esp+10] / [esp+C] = 0
    float range = k_move_away_range_00746b10;                                          // [esp+8] = [00746B10]
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed;   // 0053F99B..C4
    if (static_cast<I>(argc) >= 2 &&
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&run))) return k_pop_failed;   // 0053F9C8..EF
    if (static_cast<I>(argc) >= 3 &&
        !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &range)) return k_pop_failed;  // 0053F9F2..09
    void* object = nullptr;
    if (!self_object(self, &object)) return 0;                                         // 0053FA0B..39
    if (as_creature(object) == nullptr) return 0;                                      // 0053FA3F..4A
    if (at<U>(as_creature(object), 0xe8) == 0) return 0;                               // 0053FA50..61
    void* target_object = nullptr;                                                     // [esp+18] (only the status is used)
    if (static_cast<U8>(kotor_fastcall_004d8230(object_array(), nullptr, target, &target_object)) != k_found_0074666c) return 0; // 0053FA67..8C
    U ten = 0xa;                                                                       // 0053FAD9: argc slot overwritten with 0Ah
    (void)add_action(object, 3, 0xfffe, {{3, &target}, {1, &run}, {2, &range}, {1, &ten}});   // 0053FA8E..E4
    return 0;
}

// =========================================================================================================
// 0053FE00 ExecuteCommandMoveToPoint (21 ActionMoveToLocation(location, int run = FALSE);
// 382 ActionForceMoveToLocation(location, int run = FALSE, float timeout = 30.0))
// Quirks kept: the popped CScriptLocation is freed only when OBJECT_SELF is not a creature in an area; it leaks
// on success, on an integer/float pop failure and for any other routine id.
int __fastcall recovered_vm_move_to_point_0053fe00(void* self, void*, int routine, unsigned argc) {
    U run = 0;                                                                         // [esp+8]
    float timeout = 30.0f;                                                             // [esp+10] = 41F00000
    void* location = nullptr;                                                          // [esp+C]
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 2, &location)) return k_pop_failed; // 0053FE04..35
    if (static_cast<I>(argc) > 1) {                                                    // 0053FE42..45 (signed)
        if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&run))) return k_pop_failed; // 0053FE47..66
        if (routine == 0x17e && static_cast<I>(argc) > 2 &&
            !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &timeout)) return k_pop_failed;   // 0053FE69..88
    }
    void* object = nullptr;
    void* area = nullptr;
    if (self_object(self, &object) && as_creature(object) != nullptr &&
        (area = recovered_object_area_004cb120(as_creature(object), nullptr)) != nullptr) {   // 0053FE8A..E0
        U timeout_bits;
        if (routine == 0x15) timeout_bits = 0;                                          // 0053FEE6..0A
        else if (routine == 0x17e) std::memcpy(&timeout_bits, &timeout, 4);             // 0053FF0C..33
        else return 0;                                                                 // 0053FF12 -> 0053FF98 (location leaks)
        float zero[3];
        zero[2] = 0.0f; zero[1] = 0.0f; zero[0] = 0.0f;                                // 0053FF4C..5C
        // 0053FF34..70: AsSWSCreature()->AddMoveToPointAction(FFFFh, &location.position, area id, OBJECT_INVALID,
        //               run, 0.0, a6 = timeout (382) / 0, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0)
        (void)recovered_creature_add_move_to_point_action_004f8b60(as_creature(object), nullptr, 0xffff, location,
            at<U>(area, 0x120), k_invalid, run, 0, timeout_bits, 0, 0, 0, 0, 0, 0, zero, 0, 0, 0);
        return 0;
    }
    if (location != nullptr) free_location(location);                                 // 0053FF80..95
    return 0;
}

// =========================================================================================================
// 0052CCE0 ExecuteCommandActionJumpToObject (196 ActionJumpToObject(object, int walk_straight = TRUE) -> AddAction;
// 385 JumpToObject(object, int walk_straight = TRUE) -> AddActionToFront). Action 30h.
int __fastcall recovered_vm_action_jump_to_object_0052cce0(void* self, void*, int routine, unsigned argc) {
    U target = 0, straight = 1;                                                        // [esp+8] / [esp+4] = 1
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed;   // 0052CCE4..00
    if (static_cast<I>(argc) > 1 &&
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&straight))) return k_pop_failed; // 0052CD02..26
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 8));   // 0052CD29..36
    if (creature == nullptr || at<U>(creature, 0xe8) == 0) return 0;                   // 0052CD3B..49
    if (routine == 0xc4) (void)add_action(creature, 0x30, 0xffff, {{3, &target}, {1, &straight}});        // 0052CD82..97
    else (void)add_action_to_front(creature, 0x30, 0xffff, {{3, &target}, {1, &straight}});               // 0052CDA5
    return 0;
}

// =========================================================================================================
// 0052D440 ExecuteCommandActionRandomWalk (20 ActionRandomWalk()): action 2Dh at OBJECT_SELF's position.
int __fastcall recovered_vm_action_random_walk_0052d440(void* self, void*, int, unsigned) {
    void* object = nullptr;                                                            // [esp+4]
    if (!self_object(self, &object)) return 0;                                         // 0052D440..75
    float position[3];
    std::memcpy(position, static_cast<unsigned char*>(object) + 0x90, 12);             // 0052D47B..95
    void* area = recovered_object_area_004cb120(object, nullptr);                      // 0052D499
    alignas(4) unsigned char resref[16];
    (void)c2da_default_ref_00405ed0(resref, nullptr);                                  // 0052D49E..A4 (constructed, unused)
    if (area == nullptr) return 0;                                                     // 0052D4A9
    if (at<U>(object, 0xe8) == 0) return 0;                                            // 0052D4AD..B9
    U area_id = at<U>(area, 0x120);                                                    // 0052D4CF / 0052D4EC
    (void)add_action(object, 0x2d, 0xffff, {{2, &position[0]}, {2, &position[1]}, {2, &position[2]}, {3, &area_id}}); // 0052D4BB..0C
    return 0;
}

// =========================================================================================================
// 004D3810 CSWSCombatRound::AddCutsceneAttackAction(target, animation, result, damage): thiscall RET 10h.
// new(88h) CSWSCombatRoundAction (inline ctor: +38/+3C/+40 = 0, ClearData); a failed allocation writes through
// null exactly like the original.
U __fastcall kotor_fastcall_004d3810(void* round, void*, U target, U animation, U result, U damage) {
    void* memory = live_operator_new(0x88);                                            // 004D3812..19
    unsigned char* action = static_cast<unsigned char*>(memory);
    if (memory != nullptr) {                                                           // 004D381E..27
        at<U>(action, 0x38) = 0; at<U>(action, 0x3c) = 0; at<U>(action, 0x40) = 0;     // 004D3829..2F
        kotor_fastcall_004d1d50(action, nullptr);                                      // 004D3832
    } else {
        action = nullptr;                                                              // 004D3839
    }
    *reinterpret_cast<volatile U8*>(action + 0x10) = 1;                                // 004D383F
    at<U>(action, 0x14) = target;                                                      // 004D3843
    at<U16>(action, 4) = 0x2719;                                                       // 004D3846
    const U wrapped = static_cast<U>(kotor_combat_round_cells::word_008327dc) + 0xbb8u; // 004D384C..51
    I value; std::memcpy(&value, &wrapped, 4);
    at<I>(action, 8) = value / 2;                                                      // 004D3856..5F (cdq/sub/sar: signed /2)
    at<U>(action, 0x78) = animation;                                                   // 004D3862..66
    at<U>(action, 0x74) = 1;                                                           // 004D386F
    at<U>(action, 0x7c) = result;                                                      // 004D3876
    at<U>(action, 0x80) = damage;                                                      // 004D3879
    kotor_fastcall_004d3660(round, nullptr, action, -1);                               // 004D386D..82
    void* creature = at<void*>(round, 0x9b4);                                          // 004D3887
    if (at<U>(creature, 0x50c) == k_invalid) at<U>(creature, 0x50c) = target;          // 004D388D..99
    return word(creature);                                                          // EAX = [esi+9B4] (callers ignore it)
}

// 004F9FB0 CSWSCreature::AddCutsceneAttackActions(target, animation, result, damage): thiscall RET 10h, EAX 1.
U __fastcall kotor_fastcall_004f9fb0(void* creature, void*, U target, U animation, U result, U damage) {
    (void)kotor_fastcall_004d3810(at<void*>(creature, 0x9c8), nullptr, target, animation, result, damage);  // 004F9FB0..CD
    if (at<U>(creature, 0x4e0) == 0 && kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) != nullptr) { // 004F9FD2..F0
        recovered_creature_compute_ai_state_004f1460(creature, nullptr);              // 004F9FF2..F4
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                    // 004F9FF9..FD
    }
    if (kotor_fastcall_004cc630(creature, nullptr, 0x3f) == nullptr) {                 // 004FA002..0D
        U one = 1;                                                                     // 004FA037: the damage argument slot := 1
        (void)add_action(creature, 0x3f, 0xffff, {{1, &one}});                         // 004FA00F..42
    }
    return 1;                                                                          // 004FA047
}

// =========================================================================================================
// 0052DAF0 ExecuteCommandActionUseTalent (309 ActionUseTalentOnObject(talent, object);
// 310 ActionUseTalentAtLocation(talent, location)). The talent (engine structure 3: +0 type, +4 id, +8 byte,
// +C item id, +10 property, +14 byte, +15 byte) is freed on every path except a failed second pop (leak, original).
// The three byte fields are stored with byte moves into dword stack slots and pushed as dwords: the
// original's upper 24 bits are uninitialised stack; here they are zero (the callees read the low byte only).
int __fastcall recovered_vm_action_use_talent_0052daf0(void* self, void*, int routine, unsigned) {
    void* talent = nullptr;                                                            // [esp+4]
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 3, &talent)) return k_pop_failed; // 0052DAF4..15
    const U type = at<U>(talent, 0);                                                   // 0052DB18..24
    if (static_cast<I>(type) > -1) {                                                   // 0052DB1E / 0052DB28 (jle)
        const U b8 = at<U8>(talent, 8), b14 = at<U8>(talent, 0x14), b15 = at<U8>(talent, 0x15); // 0052DB2E..51
        const U id = at<U>(talent, 4), item = at<U>(talent, 0xc), property = at<U>(talent, 0x10);
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 8));   // 0052DB44..5E
        if (creature != nullptr) {
            if (routine == 0x135) {                                                    // 0052DB68..71
                U target = k_invalid;                                                  // routine slot := 7F000000
                if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed;  // 0052DB73..9B (talent leaks)
                (void)kotor_fastcall_00500ca0(creature, nullptr, type, id, target, b8, item, property, b14, b15); // 0052DB9E..BC
            } else if (routine == 0x136) {                                             // 0052DBC3..C8
                void* location = nullptr;
                if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 2, &location)) return k_pop_failed; // 0052DBCA..DE
                (void)kotor_fastcall_004fd070(creature, nullptr, type, id, location, b8, item, property, b14, b15); // 0052DBE0..FE
                if (location != nullptr) live_free(location);                          // 0052DC03..18
            }
        }
    }
    if (talent != nullptr) live_free(talent);                                          // 0052DC1B..30 (dtor 004015A0 empty)
    return 0;
}

// =========================================================================================================
// 0052EE50 ExecuteCommandCastSpell (48 ActionCastSpellAtObject, 234 ActionCastSpellAtLocation,
// 501 ActionCastFakeSpellAtObject, 502 ActionCastFakeSpellAtLocation).
int __fastcall recovered_vm_cast_spell_0052ee50(void* self, void*, int routine, unsigned argc) {
    U spell = 0, has_location = 0, instant = 0, target = 0, metamagic = 0, cheat = 0, domain = 0, projectile = 0;
    unsigned char location_local[0x18];                                                // [esp+38] CScriptLocation
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&spell))) return k_pop_failed; // 0052EE6C..8C
    target = k_invalid;                                                                // 0052EE96
    std::memset(location_local, 0, sizeof(location_local));                           // 0052EE9E 004CA7A0 (six zero dwords)
    U fake = 0;                                                                        // ebp
    if (routine != 0x30 && routine != 0x1f5) {                                         // 0052EEA7..B6
        void* location = nullptr;
        if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 2, &location)) return k_pop_failed; // 0052EEB8..CC
        std::memcpy(location_local, location, 0x18);                                   // 0052EED2..E1
        has_location = 1;                                                              // 0052EEE5
        live_free(location);                                                           // 0052EEE3..FA
    } else if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) {
        return k_pop_failed;                                                           // 0052EEFF..11
    }
    if (routine == 0x1f5 || routine == 0x1f6) {                                        // 0052EF13..21
        fake = 1;                                                                      // 0052EF89
    } else {
        if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&metamagic)) ||
            !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&cheat))) return k_pop_failed; // 0052EF23..49
        if (routine == 0x30) {                                                         // 0052EF4B..4E
            if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&domain))) return k_pop_failed; // 0052EF50..62
        } else {
            domain = 0;                                                                // 0052EF7F
        }
    }
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&projectile))) return k_pop_failed; // 0052EF8E..A0
    if ((routine == 0x30 || routine == 0xea) && static_cast<I>(argc) >= 7 &&
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&instant))) return k_pop_failed; // 0052EFA2..C8
    const I count = at<I>(at<void*>(kotor_cell_rules_007a3a28, 0x8c), 0);              // 0052EFCA..D6 spell array count
    if (static_cast<I>(spell) > count || static_cast<I>(spell) < 0) return 0;          // 0052EFD8..E6 (jg: spell == count passes)
    switch (projectile) {                                                              // 0052EFEC..F9 jump table 0052F2D0
    case 0: projectile = 0; break;
    case 1: projectile = 1; break;
    case 2: projectile = 2; break;
    case 3: projectile = 3; break;
    case 4: projectile = 5; break;
    default: return 0;                                                                 // ja 0052F2A8
    }
    void* caster = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(self, 8));     // 0052F030..46
    if (at<U>(self, 4) != 1 || caster == nullptr) return 0;                            // 0052F048..54
    if ((routine == 0x30 || routine == 0x1f5) &&
        kotor_fastcall_004ae750(server_app(), nullptr, target) == nullptr) return 0;   // 0052F05A..7B
    void* creature = as_creature(caster);                                              // 0052F081..85
    void* placeable = vcall<void*>(caster, 0x48);                                      // 0052F088..8E AsSWSPlaceable
    U x, y, z;
    std::memcpy(&x, location_local, 4); std::memcpy(&y, location_local + 4, 4); std::memcpy(&z, location_local + 8, 4);
    if (fake != 0) {                                                                   // 0052F091
        if (creature != nullptr) {                                                     // 0052F09C
            (void)kotor_fastcall_004f9460(creature, nullptr, spell, 0, 0, 0, 0, x, y, z, target, 0, has_location, 0, 1,
                                          projectile, instant, 0xffffffffu, 0xff, 0);  // 0052F0A0..EA
        } else if (placeable != nullptr) {                                             // 0052F0F8..FA
            (void)kotor_fastcall_00584db0(placeable, nullptr, spell, 0, location_local, target, 1, projectile); // 0052F0FC..16
        }
        return 0;                                                                      // 0052F0EF / 0052F11B -> 0052F2B0
    }
    if (creature == nullptr) {                                                         // 0052F124..26
        if (placeable != nullptr && at<U>(placeable, 0xe8) != 0)                       // 0052F278..84
            (void)kotor_fastcall_00584db0(placeable, nullptr, spell, metamagic, location_local, target, 0, projectile); // 0052F286..A3
        return 0;
    }
    if (at<U>(creature, 0xe8) == 0) return 0;                                          // 0052F12C..39
    U level_slot = (static_cast<U>(routine) & 0xffffff00u) | 0xffu;                    // 0052F134: byte [routine slot] := FF
    U cls = 0;                                                                         // bl
    bool use_class = false;
    if (cheat != 0) {                                                                  // 0052F13F..48
        cls = 0xff; use_class = true;                                                  // 0052F14A
    } else {
        U index = 0;                                                                   // ebx
        bool found = false;
        if (at<U8>(at<void*>(creature, 0xa74), 0x89) != 0) {                          // 0052F152..62
            do {
                const U klass = kt_get_class_005a4e90(at<void*>(creature, 0xa74), nullptr, index) & 0xffu;  // 0052F168..6E
                if (kt_is_jedi_005be4a0(nullptr, nullptr, klass) != 0 &&                // 0052F171..7C
                    recovered_stats_spell_uses_left_005a5550(at<void*>(creature, 0xa74), nullptr, spell, index, 0, 0) != 0) { // 0052F17E..95
                    void* spell_object = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c),
                                                                                  nullptr, static_cast<int>(spell));  // 0052F197..AD
                    if (spell_object != nullptr) {
                        const U k2 = kt_get_class_005a4e90(at<void*>(creature, 0xa74), nullptr, index);   // 0052F1B3..BA
                        if ((kotor_fastcall_0059b650(spell_object, nullptr, k2) & 0xffu) < 0xffu) { found = true; break; } // 0052F1BF..C9
                    }
                }
                ++index;                                                               // 0052F1D8
            } while (static_cast<I>(index) < static_cast<I>(at<U8>(at<void*>(creature, 0xa74), 0x89)));  // 0052F1CB..DB
        }
        cls = index & 0xffu;
        if (found && cls != 2) use_class = true;                                       // 0052F1DF..E2 (class slot 2 falls through, original)
        if (!use_class) {
            if (recovered_stats_in_spell_like_005a68e0(at<void*>(creature, 0xa74), nullptr, spell) != 1) return 0; // 0052F1E4..F7
            const U level = kotor_fastcall_005a6e30(at<void*>(creature, 0xa74), nullptr, spell);   // 0052F1F9..04
            // 0052F209..0E: `movzx ecx,al; cmp ecx,ebp(-1); je` can never be taken (original quirk): no early exit
            cls = 0xfe;                                                                // 0052F210
            level_slot = (level_slot & 0xffffff00u) | (level & 0xffu);                 // 0052F212
        }
    }
    // 0052F216..6D: AddCastSpellActions(spell, class, domain, 0, 0, location, target, 0, has_location, 0, 0, projectile,
    //               instant, -1, level slot, 0); the metamagic slot is cleared afterwards (dead store, 0052F265)
    (void)kotor_fastcall_004f9460(creature, nullptr, spell, cls & 0xffu, domain, 0, 0, x, y, z, target, 0, has_location, 0, 0,
                                  projectile, instant, 0xffffffffu, level_slot, 0);
    return 0;
}

// =========================================================================================================
// 00530360 ExecuteCommandDoDoorAction (338 DoDoorAction(door, action) / 547 DoPlaceableObjectAction(placeable, action)).
int __fastcall recovered_vm_do_door_action_00530360(void* self, void*, int routine, unsigned) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 8));   // 00530366..80 (before the pops)
    U target = 0, action = 0;                                                          // [esp+C] / [esp+10]
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target) ||
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&action))) return k_pop_failed; // 00530382..AF
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);             // 005303B2..C5
    if (object == nullptr || as_object(object) == nullptr || creature == nullptr) return 0;   // 005303C7..E0
    void* target_object = as_object(object);                                           // 005303E6..ED
    const U open_type = routine == 0x152 ? 0x14u : 0x28u;                              // 00530419 / 0053043B
    switch (action) {                                                                  // 005303EF..FC jump table 00530640
    case 0:                                                                            // 00530403..7A
        (void)add_action_to_front(creature, open_type, 0xffff, {{3, &target}});
        return 0;
    case 1:                                                                            // 0053047D..29
        (void)add_action_to_front(creature, open_type, 0xffff, {{3, &target}});
        (void)add_action_to_front(creature, 0x26, 0xfffe, {{3, &target}});
        return 0;
    case 2:                                                                            // 0053052C..62
        (void)kotor_fastcall_004fde40(creature, nullptr, target, 0, 0, 0, 0, 1, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
        return 0;
    case 3:                                                                            // 00530565..8B
        if (routine == 0x152) at<U>(at<void*>(creature, 0x340), 0xbc) = target;
        return 0;
    case 4: {                                                                          // 0053058E..33
        if (at<U>(target_object, 0xf8) != 0) return 0;
        const U slot = static_cast<U>(routine) & 0xffffff00u;                          // byte [routine slot] := bl = 0
        U bl = 0;
        if (at<U8>(at<void*>(creature, 0xa74), 0x89) == 0) return 0;                   // 005305A2..B0
        for (;;) {
            // 005305B6..C8: GetSpellUsesLeft(93, routine slot dword (low byte = class index), 0, 0); quirk kept:
            // the routine id's upper bytes ride along in the class argument
            if (recovered_stats_spell_uses_left_005a5550(at<void*>(creature, 0xa74), nullptr, 0x5d, slot | bl, 0, 0) > 0) break;
            bl = (bl + 1) & 0xffu;                                                     // 005305D6
            if (!(bl < at<U8>(at<void*>(creature, 0xa74), 0x89))) return 0;            // 005305CA..E8
        }
        U px, py, pz;
        std::memcpy(&px, static_cast<unsigned char*>(target_object) + 0x90, 4);        // 0053060A..2B
        std::memcpy(&py, static_cast<unsigned char*>(target_object) + 0x94, 4);
        std::memcpy(&pz, static_cast<unsigned char*>(target_object) + 0x98, 4);
        (void)kotor_fastcall_004f9460(creature, nullptr, 0x5d, bl, 0, 0, 0, px, py, pz, target, 0, 0, 1, 0, 0, 0,
                                      0xffffffffu, 0xff, 0);                           // 005305EB..2E
        return 0;
    }
    default:
        return 0;                                                                      // ja 00530633
    }
}

// =========================================================================================================
// 00539940 ExecuteCommandGetIsDoorActionPossible (337 GetIsDoorActionPossible / 546 GetIsPlaceableObjectActionPossible)
int __fastcall recovered_vm_get_is_door_action_possible_00539940(void* self, void*, int routine, unsigned) {
    U result = 0;                                                                      // edi
    void* door = nullptr;                                                              // esi
    void* placeable = nullptr;                                                         // ebx
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 8));   // 00539940..6B (before the pops)
    U target = 0, action = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target) ||
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, reinterpret_cast<I*>(&action))) return k_pop_failed; // 00539960..97
    if (routine == 0x151) door = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, target);       // 00539999..B8
    else placeable = recovered_placeable_lookup_004ae7a0(server_app(), nullptr, target);                // 005399BA..C7
    bool usable = false;
    if (creature != nullptr) {                                                         // 005399C9
        if (door != nullptr && at<U>(door, 0x3c0) == 0) usable = true;                 // 005399D1..DD
        else if (placeable != nullptr && at<U>(placeable, 0x398) == 0) usable = true;  // 005399DF..EF
    }
    if (usable) {
        switch (action) {                                                              // 005399F5..02 jump table 00539BBC
        case 0:                                                                        // 00539A09..6A
            if (routine == 0x151 && door != nullptr) {
                if (at<U>(door, 0xd4) == 0x2726 && at<U>(door, 0x2c4) == 0) result = 1;
            } else if (placeable != nullptr && at<U>(placeable, 0xd4) == 0x275c && at<U>(placeable, 0x260) == 0) {
                result = 1;
            }
            break;
        case 1: {                                                                      // 00539A6F..06
            const unsigned rank = w1x_creature_skill_rank_005aa570(at<void*>(creature, 0xa74), nullptr, 6, 0, 0);  // 00539A6F..7D
            const I signed_rank = static_cast<signed char>(static_cast<U8>(rank));
            U difficulty = 0; bool check = false;
            if (routine == 0x151 && door != nullptr) {                                 // 00539A82..8E
                if (at<U>(door, 0x2c4) != 0 && at<U>(door, 0x2d8) == 0 && signed_rank > 0) { difficulty = at<U8>(door, 0x2bf); check = true; }
            } else if (placeable != nullptr && at<U>(placeable, 0x260) != 0 && at<U>(placeable, 0x26c) == 0 && signed_rank > 0) {
                difficulty = at<U8>(placeable, 0x274); check = true;                    // 00539AD3..FF
            }
            if (check && !(signed_rank + 0x14 < static_cast<I>(difficulty))) result = 1;    // 00539ABB..C9
            break;
        }
        case 2:                                                                        // 00539B08..29
            if ((door != nullptr && at<U>(door, 0xf8) == 0) || (placeable != nullptr && at<U>(placeable, 0xf8) == 0)) result = 1;
            break;
        case 3:                                                                        // 00539AC9
            result = 1;
            break;
        case 4: {                                                                      // 00539B2B..95
            const bool door_ok = door != nullptr && at<U>(door, 0xf8) == 0 && at<U>(door, 0x2d8) == 0;
            const bool placeable_ok = placeable != nullptr && at<U>(placeable, 0xf8) == 0 && at<U>(placeable, 0x26c) == 0;
            if (door_ok || placeable_ok) {
                const U slot = static_cast<U>(routine) & 0xffffff00u;                  // byte [routine slot] := bl
                U bl = 0;
                do {
                    // 00539B61..7D: class argument = routine slot dword with the low byte replaced (quirk kept)
                    const U8 left = recovered_stats_spell_uses_left_005a5550(at<void*>(creature, 0xa74), nullptr, 0x5d, slot | bl, 0, 0);
                    result = 0u < left ? 1u : 0u;
                    bl = (bl + 1) & 0xffu;                                             // 00539B7F
                    if (result != 0) break;                                            // 00539B81..87
                } while (bl <= at<U8>(at<void*>(creature, 0xa74), 0x89));              // 00539B89..95 (jbe: count + 1 passes)
            }
            break;
        }
        default:
            break;
        }
    }
    return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result)) ? 0 : k_push_failed; // 00539B97..B8
}

// =========================================================================================================
// 00500CA0 CSWSCreature::AddUseTalentOnObjectActions(type, id, target, byte a4, item a5, property a6, a7, byte a8):
// thiscall RET 20h, EAX 0/1. The target must be a creature; type 0 spell / 1 feat / 2 skill; other types: 1.
U __fastcall kotor_fastcall_00500ca0(void* creature, void*, U type, U id, U target, U a4, U a5, U a6, U a7, U a8) {
    void* other = kotor_fastcall_004ae770(server_app(), nullptr, target);              // 00500CA0..BB
    if (other == nullptr || at<U>(creature, 0xe8) == 0) return 0;                      // 00500CBD..D2
    float position[3];
    std::memcpy(position, static_cast<unsigned char*>(other) + 0x90, 12);              // 00500CD5..EC
    void* area = recovered_object_area_004cb120(creature, nullptr);                    // 00500CEA..F0
    const U area_id = read_through(word(area) + 0x120);                                // 00500CF5 (null area faults, as the original)
    switch (type) {                                                                    // 00500D00..0D
    case 2:                                                                            // 00500D13..3B
        (void)kotor_fastcall_004fbe40(creature, nullptr, id, 0, target, position, area_id, k_invalid, 0);
        return 1;
    case 1:                                                                            // 00500D3E..77
        (void)kotor_fastcall_004fde40(creature, nullptr, target, id, 0, 0, 1, 0, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
        return 1;
    case 0: {
        if (a5 != k_invalid && a6 != 0xffffffffu) {                                    // 00500D7A..8D
            (void)kotor_fastcall_004fc210(creature, nullptr, a5, a6, 0, target, position, area_id, 0);   // 00500D8F..AF
            return 1;
        }
        U item = 0, property = target;                                                 // [esp+10] / the target slot is the out
        if (kotor_fastcall_005a74a0(at<void*>(creature, 0xa74), nullptr, id, &item, &property) == 1) {   // 00500DB2..D1
            (void)kotor_fastcall_004fc210(creature, nullptr, item, property, 0, target, position, area_id, 0); // 00500DD3..F9
            return 1;
        }
        U px, py, pz;
        std::memcpy(&px, &position[0], 4); std::memcpy(&py, &position[1], 4); std::memcpy(&pz, &position[2], 4);
        (void)kotor_fastcall_004f9460(creature, nullptr, id, a4 & 0xffu, 0, a8 & 0xffu, 0, px, py, pz, target,
                                      0, 0, 0, 0, 0, 0, 0xffffffffu, a7, 0);          // 00500DFC..3C
        return 1;
    }
    default:
        return 1;                                                                      // 00500E41..4D
    }
}

// 004FD070 CSWSCreature::AddUseTalentAtLocationActions(type, id, Vector* position, byte a4, item a5, property a6,
// a7, a8 = byte [esp+34h] at 004FD1AF): thiscall RET 20h, EAX 0/1. Only spells (type 0) queue anything.
U __fastcall kotor_fastcall_004fd070(void* creature, void*, U type, U id, const void* position, U a4, U a5, U a6, U a7, U a8) {
    if (at<U>(creature, 0xe8) == 0) return 0;                                          // 004FD077..80
    if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return 0;        // 004FD086..8D
    if (at<U>(recovered_object_area_004cb120(creature, nullptr), 0x120) == k_invalid) return 0;   // 004FD093..AB
    if (type != 0) return 1;                                                           // 004FD0B1..B7
    if (a5 != k_invalid && a6 != 0xffffffffu) {                                        // 004FD0BD..D0
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                    // 004FD0D2..D6
        const U area_id = at<U>(recovered_object_area_004cb120(creature, nullptr), 0x120);   // 004FD0DB..ED
        (void)kotor_fastcall_004fc210(creature, nullptr, a5, a6, 0, k_invalid, position, area_id, 0);   // 004FD0E2..FD
        return 1;
    }
    U item = 0, property = 0;                                                          // [esp+10] / [esp+C]
    const U found = kotor_fastcall_005a74a0(at<void*>(creature, 0xa74), nullptr, id, &item, &property);   // 004FD110..25
    if (found == 1) {                                                                  // 004FD12A..2D
        w5_object_clear_all_actions_004ccd80(creature, nullptr, found);                // 004FD12F..32
        const U area_id = at<U>(recovered_object_area_004cb120(creature, nullptr), 0x120);   // 004FD137..3E
        (void)kotor_fastcall_004fc210(creature, nullptr, item, property, 0, k_invalid, position, area_id, 0); // 004FD144..64
        return 1;
    }
    U px, py, pz;
    std::memcpy(&px, position, 4);
    std::memcpy(&py, static_cast<const unsigned char*>(position) + 4, 4);
    std::memcpy(&pz, static_cast<const unsigned char*>(position) + 8, 4);
    (void)kotor_fastcall_004f9460(creature, nullptr, id, a4 & 0xffu, 0, a8 & 0xffu, 0, px, py, pz, k_invalid,
                                  0, 1, 0, 0, 0, 0, 0xffffffffu, a7, 0);              // 004FD177..BD
    return 1;
}

// 004FC210 CSWSCreature::UseItem(item id, byte property, byte a3, target id, Vector* position, area id, a7):
// thiscall RET 1Ch. Property type 0Ah casts the item power, 25h uses skill 6, 2Eh sets a mine.
U __fastcall kotor_fastcall_004fc210(void* creature, void*, U item_id, U property_index, U a3, U target, const void* position,
                                     U area_id, U a7) {
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);       // 004FC211..2C
    void* target_object = kotor_fastcall_004ae750(server_app(), nullptr, target);      // 004FC231..40 (stored in the target slot)
    if (item == nullptr) return 0;                                                     // 004FC245..4D
    if (recovered_creature_can_use_item_0051b440(creature, nullptr, item, 0) == 0) return 0;   // 004FC253..5F
    void* property = kotor_m15_menu_active_property_00553960(item, nullptr, property_index & 0xffu);   // 004FC265..6D
    if (property == nullptr) return 0;                                                 // 004FC272..78
    const U bit = at<U8>(property, 0x11);                                              // 004FC27E..84
    bool charged = true;
    if (bit != 0xff && (at<U>(item, 0x294) & (1u << (bit & 31u))) == 0) charged = false;   // 004FC286..93
    const U16 kind = at<U16>(property, 0);
    if (charged && kind == 0xa) {                                                      // 004FC29D..A1
        if (at<U8>(property, 0xc) == 0) return 0;                                      // 004FC2A7..AC
        if (target_object != nullptr && as_creature(target_object) != nullptr) {      // 004FC2B2..C0
            void* visible = recovered_creature_visible_element_00517a20(creature, nullptr, target);   // 004FC2C2..C5
            if (visible != nullptr && (at<U8>(visible, 4) & 0xc) == 4) return 2;       // 004FC2CA..E1 ([00745DC0] = 2)
        }
        (void)kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                at<U>(item, 0xc));     // 004FC2E4..F1 (result unused)
        const U no_target = target == k_invalid ? 1u : 0u;                             // 004FC2F6..03 ([esp+10] = 0)
        const U queued = (at<U>(creature, 0x4e0) != 0 && kotor_fastcall_004cc630(creature, nullptr, 0xc) != nullptr) ? 1u : 0u; // 004FC307..25
        U px, py, pz;
        std::memcpy(&px, position, 4);
        std::memcpy(&py, static_cast<const unsigned char*>(position) + 4, 4);
        std::memcpy(&pz, static_cast<const unsigned char*>(position) + 8, 4);
        return kotor_fastcall_004f8c70(creature, nullptr, item_id, property_index & 0xffu, a3 & 0xffu, px, py, pz, target, a7,
                                       no_target, queued, 0);                          // 004FC327..5D
    }
    if (kind == 0x25)                                                                  // 004FC36A..71
        return kotor_fastcall_004fbe40(creature, nullptr, 6, 0, target, position, area_id, item_id, property_index & 0xffu); // 004FC373..8F
    if (kind != 0x2e) return 0;                                                        // 004FC39C..A0
    U target_slot = target;                                                            // 004FC3B1: a7 slot := target id
    U owner_slot = at<U>(item, 0x14);                                                  // 004FC3B5: property slot := item +14
    if (at<U>(creature, 0xe8) == 0) return 0;                                          // 004FC3A6..C2
    if (recovered_object_area_004cb120(creature, nullptr) != nullptr &&
        kotor_fastcall_005089d0(recovered_object_area_004cb120(creature, nullptr), nullptr) == 0) return 0;   // 004FC3C5..E0
    if (kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) != nullptr) {    // 004FC3E2..F6
        recovered_creature_compute_ai_state_004f1460(creature, nullptr);              // 004FC3F8..FA
        if (at<U8>(creature, 0x9f2) < 0xa) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);   // 004FC3FF..0C
    }
    if (kotor_fastcall_004ae750(server_app(), nullptr, owner_slot) == nullptr) return 0;    // 004FC411..26
    if (recovered_stats_can_use_skill_005af880(at<void*>(creature, 0xa74), nullptr, 1) == 0) return 0;   // 004FC428..37
    float px, py, pz;                                                                  // written into the a4 / a1 / a3 slots
    std::memcpy(&px, position, 4);
    std::memcpy(&py, static_cast<const unsigned char*>(position) + 4, 4);
    std::memcpy(&pz, static_cast<const unsigned char*>(position) + 8, 4);
    (void)add_action(creature, 0x1d, 0xffff, {{3, &owner_slot}, {3, &target_slot}, {2, &px}, {2, &py}, {2, &pz}});   // 004FC439..A0
    return 1;
}

// 005A74A0 CSWSCreatureStats::GetItemDetailsForPower(spell, ulong* item id, int* property index): thiscall RET 0Ch.
// Searches the 18 equipment slots, then the item repository (items whose base item +4 is 0), for an active
// cast-spell property (type 0Ah) of that spell that is usable.
U __fastcall kotor_fastcall_005a74a0(void* stats, void*, U spell, U* item_out, U* property_out) {
    auto creature = [&]() { return at<void*>(stats, 0x24); };
    auto matches = [&](void* item, I index, bool repository) -> bool {
        if (at<U16>(kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<U>(index)), 0) != 0xa) return false;  // 005A7500..0C
        if (at<U8>(kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<U>(index)), 0xc) == 0) {        // 005A750E..1B
            if (!repository || !(at<I>(item, 0x258) > 0)) return false;                // 005A75EF..F7 (repository pass only)
        }
        const U bit = at<U8>(kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<U>(index)), 0x11);   // 005A751D..2B
        if (bit != 0xff && (at<U>(item, 0x294) & (1u << (bit & 31u))) == 0) return false;   // 005A752D..3A
        return at<U16>(kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<U>(index)), 2) == spell;   // 005A753C..4C
    };
    if (at<void*>(creature(), 0xa2c) != nullptr) {                                     // 005A74A3..B1
        for (U slot = 0; slot < 0x12; ++slot) {                                        // 005A74B7..61 (jb)
            void* item = recovered_equipment_item_005a4c20(at<void*>(creature(), 0xa2c), nullptr, 1u << slot);   // 005A74C0..DA
            if (item == nullptr || recovered_item_property_has_00555c90(item, nullptr, 0xa, 0) == 0) continue;   // 005A74DC..ED
            for (I index = 0; index < at<I>(item, 0x248); ++index) {                   // 005A74EF..5B (jle / jl)
                if (matches(item, index, false)) {
                    *item_out = at<U>(item, 0x14); *property_out = static_cast<U>(index);   // 005A764A..57
                    return 1;
                }
            }
        }
    }
    void* repository = recovered_creature_inventory_004ef770(creature(), nullptr, 1);  // 005A7567..71
    if (repository == nullptr) return 0;                                               // 005A7573..75
    for (U n = 0; n < at<U>(repository, 0x10); ++n) {                                  // 005A757B..3B (jbe / jb)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, n);              // 005A7588..90
        if (item == nullptr) continue;
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                       at<U>(item, 0xc));   // 005A759A..A7
        if (at<U>(base, 4) != 0) continue;                                             // 005A75AC..B1
        if (recovered_item_property_has_00555c90(item, nullptr, 0xa, 0) == 0) continue;     // 005A75B7..C4
        for (I index = 0; index < at<I>(item, 0x248); ++index) {                       // 005A75C6..33
            if (matches(item, index, true)) {
                *item_out = at<U>(item, 0x14); *property_out = static_cast<U>(index);
                return 1;
            }
        }
    }
    return 0;                                                                          // 005A7641..47
}

// 005A6E30 CSWSCreatureStats::GetSpellLikeAbilityCasterLevel(spell): thiscall RET 4. List at +128 {entries, count},
// 12-byte entries {+0 spell, +8 byte level}. Not found: EAX = (loop counter) | FF.
U __fastcall kotor_fastcall_005a6e30(void* stats, void*, U spell) {
    void* list = at<void*>(stats, 0x128);                                              // 005A6E30
    const I count = at<I>(list, 4);                                                    // 005A6E36
    U i = 0;
    if (count > 0) {                                                                   // 005A6E3C..3F
        unsigned char* entries = at<unsigned char*>(list, 0);
        for (; static_cast<I>(i) < count; ++i) {                                       // 005A6E50..5A
            if (at<U>(entries + i * 0xc, 0) == spell)
                return ((i * 3u) & 0xffffff00u) | at<U8>(entries, (i * 3u) * 4u + 8u); // 005A6E63..6C
        }
    }
    return i | 0xffu;                                                                  // 005A6E5E
}

// 00584DB0 CSWSPlaceable::AddCastSpellActions(spell, a2, Vector* position, target, a5, byte a6): thiscall RET 18h.
U __fastcall kotor_fastcall_00584db0(void* placeable, void*, U spell, U a2, const void* position, U target, U a5, U a6) {
    float point[3];
    std::memcpy(point, position, 12);                                                  // 00584DB3..D4
    if (target != k_invalid) {                                                         // 00584DC6..D8
        void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);         // 00584DDA..E4
        if (object == nullptr) return 0;                                               // 00584DE9..EB
        void* target_object = as_object(object);                                       // 00584DF1..F5
        if (target_object != nullptr) {                                                // 00584DF8..FC
            if (recovered_object_area_004cb120(target_object, nullptr) != recovered_object_area_004cb120(placeable, nullptr)) return 0; // 00584DFE..12
            std::memcpy(point, static_cast<unsigned char*>(target_object) + 0x90, 12);  // 00584E18..2E
        }
    }
    U flags = a6 & 0xffu;                                                              // 00584E32..37 (position slot reused)
    if (a5 != 0) flags |= 0x80000000u;                                                 // 00584E3B..43
    (void)add_action(placeable, 0xf, 0xffff, {{1, &spell}, {1, &a2}, {3, &target}, {2, &point[0]}, {2, &point[1]},
                                              {2, &point[2]}, {1, &flags}});            // 00584E4B..9D
    return 1;
}

}  // extern "C"
