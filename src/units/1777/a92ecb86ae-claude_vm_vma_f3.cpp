// Lane vmA, sub-group "f3" (creature / object queries): NWScript engine-command handlers
// (CSWVirtualMachineCommands::ExecuteCommand*) reconstructed from the original swkotor.exe (read-only, capstone).
// Every handler: original int __thiscall (int routine, int argc), RET 8 -> extern "C" int __fastcall
// kotor_fastcall_<addr>(void* self, void* edx, int routine, unsigned argc) (@16).  Conventions: claude_vm_vmA_common.hpp.
// Addresses in comments are original instruction ranges.
// .rdata facts used as literals: [00746634] 5 creature, [00746636] 7 trigger, [00746638] 9 placeable, [00746639] 10 door
// (object type bytes), [00745E5C] float 25.0.
#include "520143efd3-claude_vm_vma_common.hpp"

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules* [0421]
// ---- existing providers (c31 map; object in brackets; arity checked against the original RET) ----
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);                     // GetItemByGameObjectID RET 4 [1025]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);                // GetPlaceableByGameObjectID RET 4 [0886]
int __fastcall kotor_fastcall_004ae950(void* server, void*, U id, U* out);                      // GetFactionOfObject RET 8 [claude_vm_actions_world]
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                      // CFactionManager::GetFaction RET 4 [claude_vm_actions_world]
U __fastcall kotor_fastcall_0057c870(void* object, void*, U item_id, U target_id, I count, U a4); // AddGiveItemActions RET 10h [claude_p2s_a_inv]
U __fastcall kotor_fastcall_004fbe40(void* creature, void*, U skill, U a2, U target, const void* position, U area_id,
    U a6, U a7);                                                                                // CSWSCreature::UseSkill RET 1Ch [skill_producer]
U __fastcall kotor_fastcall_004edb20(void* self, void*, U target_id, U a2);                     // GetIsWeaponEffective RET 8 [claude_combat_attack_damage]
void* __fastcall kotor_fastcall_00553990(void* item, void*, I index);                           // CSWSItem::GetPassiveProperty RET 4 [claude_combat_attack_damage]
U __fastcall recovered_stats_reflex_save_005ab8f0(void* stats, void*, U flag);                  // GetReflexSavingThrow (AL) RET 4 [w2_g07_native]
U __fastcall recovered_creature_armor_class_004ed1d0(void* creature, void*);                    // CSWSCreature::GetArmorClass (AX) [w2_g07_native]
U __fastcall kotor_fastcall_005a6e30(void* stats, void*, U spell);                              // GetSpellLikeAbilityCasterLevel (AL) RET 4 [claude_vm_actions_movement]
U __fastcall recovered_stats_class_level_005a5090(void* stats, void*, U index, U flag);         // GetClassLevel (AL) RET 8 [w2_g07_native]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, U id);          // CSWSpellArray::GetSpell RET 4 [0820]
U __fastcall kotor_fastcall_005ad0e0(void* stats, void*, void* versus, U flag);                 // GetArmorClassVersus (AX) RET 8 [claude_combat_attack_attackstats]
U __fastcall kt_get_class_005a4e90(void* stats, void*, U index);                                // CSWSCreatureStats::GetClass (AL) RET 4 [provider_adapters]
int __fastcall area_clear_line_of_sight_0050c330(void* area, void*, void* a, void* b, void* c, unsigned* d,
    unsigned caller, unsigned target, int flag, int e);                                         // CSWSArea::ClearLineOfSight RET 20h [0526]
Vector* __cdecl kotor_cdecl_00423bc0(Vector* out, const Vector* a, const Vector* b);            // Vector operator- [0457]
U __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const void* point, U area_id,
    U target_id, U run, U range, U a6, U a7, U a8, U a9, U a10, U a11, U a12, const void* a13, U a14, U a15, U a16); // RET 44h [vm_actions_t5]
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);                                                                   // CSWSObject::AddAction RET 70h [w1_native_b7]
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U slot);             // CSWInventory::GetItemInSlot RET 4 [1025]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, I id);   // CSWBaseItemArray::GetBaseItem RET 4 [0820]
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                 // CItemRepository::ItemListGetItem RET 4 [0886]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                     // CSWItem::GetBaseItem [0820]
unsigned char __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, U* slot, U a3, U a4, U a5); // CanEquipItem RET 14h [1025]
U __fastcall recovered_inventory_has_item_005a4950(void* inventory, void*, void* item);         // CSWInventory::GetItemInInventory RET 4 [w1_native_b7]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                    // CServerExoApp::GetSWSMessage [creature_destructor_r2]
U __fastcall recovered_inventory_slot_message_0056fbd0(void* message, void*, U recipient, U object_id, U slot); // RET 0Ch [clear9_cleanup]
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data); // SendFeedbackMessage RET 8 [w1_native_b7]
U __fastcall recovered_action_node_clear_004f0310(void* creature, void*, void* item, U slot, U a2, U a3); // CSWSCreature::EquipItem RET 10h [clear9_cleanup]
void __fastcall recovered_combat_round_equip_004d3c30(void* round, void*, U item_id, U slot, U a2, U a3); // CSWSCombatRound::AddEquipAction RET 10h [clear9_cleanup]
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_005bf6e0(void* faction, void*, I start, U is_pc);
U __fastcall kotor_fastcall_005bf340(void* faction, void*, U visible_to, U must_be_visible);
I __fastcall kotor_fastcall_005befe0(void* faction, void*);
U __fastcall kotor_fastcall_005bf1f0(void* faction, void*);
U __fastcall kotor_fastcall_0057c980(void* object, void*, U item_id, U from_id, U a3);
U __fastcall kotor_fastcall_004f3a60(void* creature, void*);
}

namespace {
using namespace vma;
constexpr U k_type_creature_00746634 = 5;
constexpr U k_type_trigger_00746636 = 7;
constexpr U k_type_placeable_00746638 = 9;
constexpr U k_type_door_00746639 = 10;
const float k_run_distance2_00745e5c = 25.0f;                                                    // .rdata 00745E5C
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); }
inline void* item_by_id(U id) { return recovered_item_lookup_004ae760(server_app(), nullptr, id); }
inline bool found(U id, void** out) {
    return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)) == 0;
}
inline void* stats_of(void* creature) { return at<void*>(creature, 0xa74); }
// [[[7A39FC]+8]+4] CServerExoAppInternal, +10054h CFactionManager*
inline void* server_internal() { return at<void*>(server_app(), 4); }
}  // namespace

extern "C" {

// ================================================================== 00536AD0 GetCasterLevel(object) [routine 84]
// Creature: +1C0 spell id; else placeable; else vtbl+50h object (area-of-effect): spell = its vtbl+B8h(), creator
// +248h -> creature / placeable.  Creature: +964h nonzero -> +968h; class byte +1D4h: FFh -> spell row level*2-1
// (minimum 10), FEh -> GetSpellLikeAbilityCasterLevel(spell), else GetClassLevel(class, 1).
int __fastcall kotor_fastcall_00536ad0(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 00536ADF ebp
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00536ADA..F3
    void* object = nullptr;
    if (found(id, &object)) {                                                          // 00536AF6..1B
        void* creature = as_creature(object);                                          // 00536B21..30
        void* placeable = as_placeable(object);                                        // 00536B2C..34
        U spell = 0;
        bool spell_row = false;
        if (creature) {                                                                // 00536B37..39
            spell = at<U>(creature, 0x1c0);                                            // 00536B3B
        } else if (placeable) {
            spell_row = true;                                                          // 00536B43..45 -> 00536C03
        } else if (vcall<void*>(object, 0x50) != nullptr) {                            // 00536B4B..56
            spell = vcall<U>(vcall<void*>(object, 0x50), 0xb8);                        // 00536B5C..74
            const U creator = at<U>(vcall<void*>(object, 0x50), 0x248);                // 00536B70..7B
            creature = creature_by_id(creator);                                        // 00536B81..9A
            placeable = recovered_placeable_lookup_004ae7a0(server_app(), nullptr, creator); // 00536B90..9C
            if (!creature) spell_row = placeable != nullptr;                           // 00536BA2 / 00536BFF..01
        }
        if (creature) {
            if (at<U>(creature, 0x964) != 0) {                                         // 00536BA6..AE
                result = at<I>(creature, 0x968);                                       // 00536BB0
            } else {
                const U cls = at<U8>(creature, 0x1d4);                                 // 00536BB8
                if (cls == 0xff) {                                                     // 00536BBF..C4
                    placeable = creature; spell_row = true;                            // 00536BC6 (spell from +1C0)
                } else if (cls == 0xfe) {                                              // 00536BDA..E5
                    result = static_cast<U8>(kotor_fastcall_005a6e30(stats_of(creature), nullptr, spell)); // 00536BE7..ED
                } else {
                    result = static_cast<U8>(recovered_stats_class_level_005a5090(stats_of(creature), nullptr, cls, 1)); // 00536BF2..FA
                }
            }
        }
        if (spell_row) {                                                               // 00536C03..2C
            void* row = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
                                                                   at<U>(placeable, 0x1c0));
            if (row) {
                result = static_cast<I>(at<U8>(row, 0x3f)) * 2 - 1;                    // 00536C1F..23
                if (result < 10) result = 10;                                          // 00536C27..2C (jge)
            }
        }
    }
    return push_int(result);                                                           // 00536C33..51
}

// ================================================================== 0052C8B0 ActionGiveItem / ActionTakeItem [routines 135, 136]
int __fastcall kotor_fastcall_0052c8b0(void* self, void*, int routine, unsigned) {
    U item_id = 0, other_id = 0;
    if (!pop_object(&item_id)) return k_pop_failed;                                    // 0052C8B4..C8
    if (!pop_object(&other_id)) return k_pop_failed;                                   // 0052C8CA..DC
    void* caller = game_object(self_id(self));                                         // 0052C8EA..FC
    if (caller && item_by_id(item_id)) {                                               // 0052C8FE..17
        if (routine == 0x87)                                                           // 0052C923 (cmp 87h)
            kotor_fastcall_0057c870(as_object(caller), nullptr, item_id, other_id, -1, 1); // 0052C930..3B
        else
            kotor_fastcall_0057c980(as_object(caller), nullptr, item_id, other_id, 1); // 0052C949..52
    }
    return 0;
}

// ================================================================== 0052D9B0 ActionUseSkill(skill, target, subskill=0, item=INVALID) [routine 288]
int __fastcall kotor_fastcall_0052d9b0(void* self, void*, int, unsigned argc) {
    I skill = 0; U target = 0;
    if (!pop_int(&skill)) return k_pop_failed;                                         // 0052D9B4..C8
    if (!pop_object(&target)) return k_pop_failed;                                     // 0052D9CA..DC
    I subskill = 0;                                                                    // 0052D9F2
    if (static_cast<I>(argc) >= 3 && !pop_int(&subskill)) return k_pop_failed;         // 0052D9EF..0E (jl: signed)
    U item = k_invalid;                                                                // 0052DA20
    if (static_cast<I>(argc) >= 4 && !pop_object(&item)) return k_pop_failed;          // 0052DA1D..3C
    void* object = nullptr;
    if (found(self_id(self), &object) && as_creature(object) &&                        // 0052DA3E..6F
        recovered_object_area_004cb120(as_creature(object), nullptr)) {                // 0052DA71..83
        void* area = recovered_object_area_004cb120(as_creature(object), nullptr);     // 0052DA85..90
        Vector position = {0.0f, 0.0f, 0.0f};                                          // 0052DABC..CC
        kotor_fastcall_004fbe40(as_creature(object), nullptr, static_cast<U>(skill), static_cast<U>(subskill), target,
                                &position, at<U>(area, 0x120), item, 0);               // 0052DA95..DC
    }
    return 0;
}

// ================================================================== 005392D0 GetHasInventory(object) [routine 570]
int __fastcall kotor_fastcall_005392d0(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 005392DD
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005392D8..EF
    void* object = game_object(id);                                                    // 005392F2..01
    if (object) {
        if (as_creature(object) || vcall<void*>(object, 0x64)) {                       // 0053930C..20
            result = 1;                                                                // 00539356
        } else if (as_placeable(object)) {                                             // 00539322..2D
            result = at<I>(as_placeable(object), 0x324);                               // 0053932F..34
        } else if (vcall<void*>(object, 0x38) && at<U>(vcall<void*>(object, 0x38), 0x26c) != 0) { // 0053933C..54
            result = 1;
        }
    }
    return push_int(result);                                                           // 0053935B..78
}

// ================================================================== 0053B960 GetLocked(object) [routine 325]
int __fastcall kotor_fastcall_0053b960(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 0053B96F
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053B96A..83
    void* object = nullptr;
    if (found(id, &object)) {                                                          // 0053B986..AB
        if (as_door(object)) result = at<I>(as_door(object), 0x2c4);                   // 0053B9AD..C3
        else if (as_placeable(object)) result = at<I>(as_placeable(object), 0x260);    // 0053B9CB..DD
    }
    return push_int(result);                                                           // 0053B9E3..01
}

// ================================================================== 00545CA0 SetNPCAIStyle(creature, style) [routine 707]
int __fastcall kotor_fastcall_00545ca0(void*, void*, int, unsigned) {
    U id = 0; I style = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00545CA9..BD
    if (!pop_int(&style)) return k_pop_failed;                                         // 00545CBF..D1
    void* object = nullptr;
    if (found(id, &object) && as_creature(object))                                     // 00545CDE..10
        at<U16>(stats_of(as_creature(object)), 0x13a) = static_cast<U16>(style);       // 00545D12..26 (word store)
    return 0;
}

// ================================================================== 0053BD80 GetMetaMagicFeat() [routine 105]
// Quirk (original): when the caller is not a CSWSObject the byte is read from the uninitialised `push ecx` slot,
// which holds this (the commands object): byte [this+1D5h].  Reproduced.
int __fastcall kotor_fastcall_0053bd80(void* self, void*, int, unsigned) {
    U8 result = 0xff;                                                                  // 0053BD92 or bl,FFh
    void* object = game_object(self_id(self));                                         // 0053BD82..95
    void* source = (object && as_object(object)) ? as_object(object) : self;           // 0053BD9C..B4
    if (source) result = at<U8>(source, 0x1d5);                                        // 0053BDB8..BC
    return push_int(static_cast<I>(result));                                           // 0053BDC2..E2 movzx
}

// ================================================================== 005396F0 GetCurrentForcePoints(object) [routine 55]
int __fastcall kotor_fastcall_005396f0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005396F1..0B
    void* creature = creature_by_id(id);                                               // 0053970E..1C
    I result = 0;
    if (creature) {                                                                    // 00539723..25 (stats not null-checked)
        void* stats = stats_of(creature);
        result = static_cast<I>(static_cast<short>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124))); // 0053972D..3B
    }
    return push_int(result);                                                           // 0053973E..59
}

// ================================================================== 005386F0 GetFirst/NextFactionMember(member, pc_only) [routines 380, 381]
int __fastcall kotor_fastcall_005386f0(void*, void*, int routine, unsigned) {
    U result = k_invalid;                                                              // 005386FE edi
    U id = k_invalid; I pc_only = 1;                                                   // 00538704..08
    if (!pop_object(&id)) return k_pop_failed;                                         // 005386FA..22
    if (!pop_int(&pc_only)) return k_pop_failed;                                       // 00538725..37
    void* object = nullptr;
    if (found(id, &object) && server_internal()) {                                     // 00538739..6E
        void* manager = at<void*>(server_internal(), 0x10054);                         // 00538771
        U faction_id = 0;
        if (manager && kotor_fastcall_004ae950(server_app(), nullptr, id, &faction_id) == 1) { // 00538777..90
            void* faction = kotor_fastcall_0052b3b0(manager, nullptr, static_cast<I>(faction_id)); // 00538792..9E
            if (faction) {                                                             // 005387A0..A2
                if (routine == 0x17c)                                                  // 005387A8..AD
                    result = kotor_fastcall_005bf6e0(faction, nullptr, 0, static_cast<U>(pc_only)); // 005387AF..CD
                else if (routine == 0x17d)                                             // 005387B8..BD
                    result = kotor_fastcall_005bf6e0(faction, nullptr, at<I>(faction, 0x14), static_cast<U>(pc_only)); // 005387BF..CD
            }
        }
    }
    return push_object(result);                                                        // 005387D0..EE
}

// ================================================================== 00538B10 GetFactionWorstAC(member=SELF, visible=TRUE) [routine 192]
// Quirks (original): the optional argument is popped only when argc == 2 exactly; the faction pointer from
// GetFaction is not null-checked; the creature's stats are dereferenced before the server-internal test.
int __fastcall kotor_fastcall_00538b10(void*, void*, int, unsigned argc) {
    U result = k_invalid;                                                              // 00538B1E esi
    U id = k_invalid;                                                                  // 00538B24
    if (!pop_object(&id)) return k_pop_failed;                                         // 00538B1A..3A
    I visible = 1;                                                                     // 00538B42
    if (argc == 2 && !pop_int(&visible)) return k_pop_failed;                          // 00538B3D..5E (jne)
    void* creature = creature_by_id(id);                                               // 00538B60..73
    if (creature) {
        const U faction_id = at<U>(stats_of(creature), 0x78);                          // 00538B77..8B
        if (server_internal() && at<void*>(server_internal(), 0x10054)) {              // 00538B83..98
            void* faction = kotor_fastcall_0052b3b0(at<void*>(server_internal(), 0x10054), nullptr,
                                                    static_cast<I>(faction_id));       // 00538BA4..A7
            result = kotor_fastcall_005bf340(faction, nullptr, id, static_cast<U>(visible)); // 00538B9A..AE
        }
    }
    return push_object(result);                                                        // 00538BB5..D3
}

// ================================================================== 00538500 GetFactionGold(member) [routine 185]
int __fastcall kotor_fastcall_00538500(void*, void*, int, unsigned) {
    I result = -1;                                                                     // 0053850D
    U id = k_invalid;                                                                  // 00538510
    if (!pop_object(&id)) return k_pop_failed;                                         // 00538508..28
    void* creature = creature_by_id(id);                                               // 0053852B..39
    if (creature) {
        const U faction_id = at<U>(stats_of(creature), 0x78);                          // 00538542..48
        if (server_internal() && at<void*>(server_internal(), 0x10054))                // 0053854B..63
            result = kotor_fastcall_005befe0(kotor_fastcall_0052b3b0(at<void*>(server_internal(), 0x10054), nullptr,
                                                                     static_cast<I>(faction_id)), nullptr); // 00538565..74
    }
    return push_int(result);                                                           // 00538576..92
}

// ================================================================== 005388D0 GetFactionMostFrequentClass(member) [routine 191]
int __fastcall kotor_fastcall_005388d0(void*, void*, int, unsigned) {
    I result = -1;                                                                     // 005388DD
    U id = k_invalid;                                                                  // 005388E0
    if (!pop_object(&id)) return k_pop_failed;                                         // 005388D8..F8
    void* creature = creature_by_id(id);                                               // 005388FB..09
    if (creature) {
        const U faction_id = at<U>(stats_of(creature), 0x78);                          // 00538912..18
        if (server_internal() && at<void*>(server_internal(), 0x10054))                // 0053891B..33
            result = static_cast<I>(kotor_fastcall_005bf1f0(kotor_fastcall_0052b3b0(at<void*>(server_internal(), 0x10054),
                                                                     nullptr, static_cast<I>(faction_id)), nullptr)); // 00538935..44
    }
    return push_int(result);                                                           // 00538946..62
}

// ================================================================== 0052C880 ActionEquipMostEffectiveArmor() [routine 404]
int __fastcall kotor_fastcall_0052c880(void* self, void*, int, unsigned) {
    void* creature = creature_by_id(self_id(self));                                    // 0052C880..8F
    if (creature) kotor_fastcall_004f3a60(creature, nullptr);                          // 0052C894..9A
    return 0;
}

// ================================================================== 0053CE10 GetReflexSavingThrow(object) [routine 493]
int __fastcall kotor_fastcall_0053ce10(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 0053CE44
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053CE11..2B
    void* object = game_object(id);                                                    // 0053CE2E..42
    if (object) {
        const U type = type_of(object);                                                // 0053CE4A
        if (type == k_type_creature_00746634) {                                        // 0053CE4D..53
            void* creature = as_creature(object);
            if (creature && stats_of(creature))                                        // 0053CE55..66
                result = static_cast<signed char>(recovered_stats_reflex_save_005ab8f0(stats_of(creature), nullptr, 0)); // 0053CE68..6E
        } else if (type == k_type_door_00746639) {                                     // 0053CE73..79
            if (as_door(object)) result = at<U8>(as_door(object), 0x2bd);              // 0053CE7B..84
        } else if (type == k_type_placeable_00746638) {                                // 0053CE8D..93
            if (as_placeable(object)) result = at<U8>(as_placeable(object), 0x316);    // 0053CE95..9E
        }
    }
    return push_int(result);                                                           // 0053CEA5..C1
}

// ================================================================== 00536410 GetAC(object, future=0) [routine 116]
// Object types 5..10 through the table at 005364D8: 5 -> creature AC, 6/9/10 -> 0, 7/8 -> -1; others -1.
int __fastcall kotor_fastcall_00536410(void*, void*, int, unsigned argc) {
    I result = -1;                                                                     // 0053641F
    U id = 0; I future = 0;                                                            // 00536422
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053641A..3C
    if (static_cast<I>(argc) >= 2 && !pop_int(&future)) return k_pop_failed;           // 0053643F..58 (jl)
    void* object = nullptr;
    if (found(id, &object) && type_of(object) > k_type_area_00746633) {                // 0053645A..8E (jbe: unsigned)
        switch (type_of(object)) {                                                     // 00536490..9B
        case 5: result = static_cast<short>(recovered_creature_armor_class_004ed1d0(as_creature(object), nullptr)); break; // 005364A2..AE
        case 6: case 9: case 10: result = 0; break;                                    // 005364B3
        default: break;                                                                // 7, 8, >10
        }
    }
    return push_int(result);                                                           // 005364B5..D3
}

// ================================================================== 0053A130 GetItemACValue(item) [routine 401]
int __fastcall kotor_fastcall_0053a130(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053A131..4B
    I result = 0;                                                                      // 0053A15E
    void* item = item_by_id(id);                                                       // 0053A14E..60
    if (item) {
        result = at<I>(item, 0x244);                                                   // 0053A171 base AC
        for (I i = 0; i < at<I>(item, 0x24c); ++i) {                                   // 0053A16B..A1 (re-read count)
            void* property = kotor_fastcall_00553990(item, nullptr, i);                // 0053A180..83
            if (property && at<U16>(property, 0) == 1) result += at<U16>(property, 6); // 0053A188..96
        }
    }
    return push_int(result);                                                           // 0053A1A4..C1
}

// ================================================================== 0053A000 GetIsTrapped(object) [routine 551]
int __fastcall kotor_fastcall_0053a000(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053A001..1B
    I result = 0;                                                                      // 0053A02D
    void* object = game_object(id);                                                    // 0053A01E..2F
    if (object) {
        const U type = type_of(object);                                                // 0053A03A
        if (type == k_type_trigger_00746636) result = at<I>(as_trigger(object), 0x2bc);          // 0053A03D..4A
        else if (type == k_type_placeable_00746638) result = at<I>(as_placeable(object), 0x278); // 0053A052..5F
        else if (type == k_type_door_00746639) result = at<I>(as_door(object), 0x2e8);           // 0053A067..74
    }
    return push_int(result);                                                           // 0053A07A..96
}

// ================================================================== 0053A0A0 GetIsWeaponEffective(versus=INVALID, offhand=FALSE) [routine 422]
int __fastcall kotor_fastcall_0053a0a0(void* self, void*, int, unsigned) {
    I result = 0;                                                                      // 0053A0B2
    U versus = 0; I offhand = 0;
    if (!pop_object(&versus)) return k_pop_failed;                                     // 0053A0A5..C7
    if (!pop_int(&offhand)) return k_pop_failed;                                       // 0053A0CA..DC
    void* creature = creature_by_id(self_id(self));                                    // 0053A0DE..EB
    if (creature) result = static_cast<I>(kotor_fastcall_004edb20(creature, nullptr, versus, static_cast<U>(offhand))); // 0053A0F0..05
    return push_int(result);                                                           // 0053A107..26
}

// ================================================================== 00536C60 GetChallengeRating(creature) [routine 494]
int __fastcall kotor_fastcall_00536c60(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00536C60..7F
    void* creature = creature_by_id(id);                                               // 00536C82..90
    U bits_out = 0;                                                                    // 00536C97
    if (creature && stats_of(creature)) bits_out = at<U>(stats_of(creature), 0x84);    // 00536C9F..B1 (dword copy)
    return push_float(flt(bits_out));                                                  // 00536CB5..D6
}

// ================================================================== 0053D250 GetSpellCastItem() [routine 438]
int __fastcall kotor_fastcall_0053d250(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                              // 0053D261
    void* object = game_object(self_id(self));                                         // 0053D250..66
    if (object && as_creature(object)) result = at<U>(as_creature(object), 0x95c);     // 0053D26D..83
    return push_object(result);                                                        // 0053D289..A5
}

// ================================================================== 005397C0 GetIdentified(item) [routine 332]
// Quirk (original): pushes TRUE whenever the id is an item; the identified flag is not read.
int __fastcall kotor_fastcall_005397c0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005397C1..DB
    return push_int(item_by_id(id) != nullptr ? 1 : 0);                                // 005397DE..19
}

// ================================================================== 00543740 SetForcePowerUnsuccessful(result, creature=SELF) [routine 731]
// Quirk (original): both arguments are popped with StackPopObject (5D10C0), including the int nResult.
int __fastcall kotor_fastcall_00543740(void*, void*, int, unsigned) {
    U value = 0, id = k_invalid;                                                       // 0054374E..56
    if (!pop_object(&value)) return k_pop_failed;                                      // 00543749..65
    if (!pop_object(&id)) return k_pop_failed;                                         // 00543767..79
    void* creature = creature_by_id(id);                                               // 0054377B..89
    if (creature) at<U>(creature, 0x564) = value;                                      // 0054378E..96
    return 0;
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 005BF6E0 CSWSFaction::GetFactionMember(int start, int is_pc): thiscall RET 8.  +0 member id array, +4 count,
// +14h next index.  Returns the first member from `start` whose stats +6Ch (is PC) equals is_pc.
U __fastcall kotor_fastcall_005bf6e0(void* faction, void*, I start, U is_pc) {
    if (at<I>(faction, 4) >= 1) {                                                      // 005BF6E5..EB (jl)
        for (I i = start; i < at<I>(faction, 4); ++i) {                                // 005BF6ED..28 (signed)
            void* creature = creature_by_id(at<U*>(faction, 0)[i]);                    // 005BF700..0E
            if (creature && at<U>(stats_of(creature), 0x6c) == is_pc) {                // 005BF713..20
                at<I>(faction, 0x14) = i + 1;                                          // 005BF737..3A
                return at<U*>(faction, 0)[i];                                          // 005BF735..3D
            }
        }
    }
    return k_invalid;                                                                  // 005BF72C
}

// 005BF340 CSWSFaction::GetWorstAC(visible_to, must_be_visible): thiscall RET 8.  Member with the lowest
// GetArmorClassVersus(0,0) (word compare, signed, strictly lower than the running minimum starting at 999); when
// visibility is required, only members the observer has a clear line of sight to.
// Quirks (original): the running minimum is stored as the full dword but compared as a word; the LOS out dword is
// uninitialised scratch.
U __fastcall kotor_fastcall_005bf340(void* faction, void*, U visible_to, U must_be_visible) {
    U best_ac = 0x3e7;                                                                 // 005BF353
    U result = k_invalid;                                                              // 005BF35B
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                        // 005BF346..63 / 005BF44A..54
        void* creature = creature_by_id(at<U*>(faction, 0)[i]);                        // 005BF370..83
        if (!creature) continue;                                                       // 005BF385..87
        const U ac = kotor_fastcall_005ad0e0(stats_of(creature), nullptr, nullptr, 0); // 005BF38D..9C
        if (static_cast<short>(ac) >= static_cast<short>(best_ac)) continue;          // 005BF39E..A3 (jge, word)
        if (visible_to == k_invalid || must_be_visible == 0) {                         // 005BF3A9..BE -> 005BF468
            best_ac = ac; result = at<U*>(faction, 0)[i];                              // 005BF468..71
            continue;
        }
        Vector hit = {0.0f, 0.0f, 0.0f};                                               // 005BF3CA..DA
        unsigned scratch;                                                              // [esp+1C] uninitialised
        void* observer = creature_by_id(visible_to);                                   // 005BF3C4..EB
        if (observer && recovered_object_area_004cb120(creature, nullptr)) {           // 005BF3ED..FA
            if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(creature, nullptr), nullptr,
                    static_cast<unsigned char*>(creature) + 0x90, static_cast<unsigned char*>(observer) + 0x90,
                    &hit, &scratch, k_invalid, k_invalid, 0, 0) == 1) {               // 005BF3FC..33
                best_ac = ac; result = at<U*>(faction, 0)[i];                          // 005BF435..42
            }
        }
    }
    return result;                                                                     // 005BF45A
}

// 005BEFE0 CSWSFaction::GetGold(): thiscall RET.  Original (KotOR) body: walks the members through
// GetCreatureByGameObjectID, discards every result and returns 0.
I __fastcall kotor_fastcall_005befe0(void* faction, void*) {
    for (I i = 0; i < at<I>(faction, 4); ++i)                                          // 005BEFE4..0A
        (void)creature_by_id(at<U*>(faction, 0)[i]);                                   // 005BEFF0..FF
    return 0;                                                                          // 005BF00D
}

// 005BF1F0 CSWSFaction::GetMostFrequentClass(): thiscall RET.  For every class of every member, counts the
// faction's class slots equal to it; keeps the class whose count is strictly highest.
// Quirk (original): the occurrence counter is never reset between candidate classes (cumulative), so the last
// examined class with a positive contribution tends to win.  Returns 0 for an empty faction.
U __fastcall kotor_fastcall_005bf1f0(void* faction, void*) {
    U best_class = 0;                                                                  // 005BF200 [esp+1C]
    I best_count = 0;                                                                  // 005BF204 [esp+0C]
    I count = 0;                                                                       // 005BF208 [esp+08]
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                        // 005BF1F9..10 / 005BF31B..25
        void* creature = creature_by_id(at<U*>(faction, 0)[i]);                        // 005BF220..34
        if (!creature) continue;                                                       // 005BF236..3C
        for (U k = 0; k < at<U8>(stats_of(creature), 0x89); ++k) {                     // 005BF242..56 / 005BF2FD..11
            const U cls = static_cast<U8>(kt_get_class_005a4e90(stats_of(creature), nullptr, k)); // 005BF260..70
            for (I j = 0; j < at<I>(faction, 4); ++j) {                                // 005BF269..74 / 005BF2D7..DD
                void* other = creature_by_id(at<U*>(faction, 0)[j]);                   // 005BF280..95
                if (!other) continue;
                for (U m = 0; m < at<U8>(stats_of(other), 0x89); ++m)                  // 005BF29B..D5
                    if (static_cast<U8>(kt_get_class_005a4e90(stats_of(other), nullptr, m)) == cls) ++count; // 005BF2B0..C1
            }
            if (count > best_count) { best_count = count; best_class = cls; }          // 005BF2E7..F9 (jle: signed)
        }
    }
    return best_class;                                                                 // 005BF32B
}

// 0057C980 CSWSObject::AddTakeItemActions(item id, from id, a3): thiscall RET 0Ch, EAX 0/1.  A creature first
// moves to the source (run when the squared distance exceeds 25.0), then action 23h (take item) is queued.
U __fastcall kotor_fastcall_0057c980(void* object, void*, U item_id, U from_id, U a3) {
    I run = 0;                                                                         // 0057C994 ebp
    void* from = game_object(from_id);                                                 // 0057C980..96
    if (!from) return 0;                                                               // 0057C99B..A4
    void* source = as_object(from);                                                    // 0057C9A7..AF
    if (!source) return 0;                                                             // 0057C9B1..B3
    void* item = item_by_id(item_id);                                                  // 0057C9B9..C6
    if (!item) return 0;                                                               // 0057C9CB..CD
    if (at<U>(item, 0x268) == at<U>(object, 4)) return 0;                              // 0057C9D3..DC (already owner)
    if (at<U>(object, 0xe8) == 0) return 0;                                            // 0057C9E2..EA
    if (as_creature(object)) {                                                         // 0057C9F0..F9
        unsigned char* point = static_cast<unsigned char*>(source) + 0x90;             // 0057CA00
        Vector d;
        kotor_cdecl_00423bc0(&d, reinterpret_cast<Vector*>(static_cast<unsigned char*>(object) + 0x90),
                             reinterpret_cast<Vector*>(point));                        // 0057CA06..13 self - source
        const float zz = d.z, yy = d.y, xx = d.x, limit = k_run_distance2_00745e5c;
        unsigned short status;
        __asm {
            fld dword ptr [zz]                                                         // 0057CA18
            fmul dword ptr [zz]                                                        // 0057CA1C
            fld dword ptr [yy]                                                         // 0057CA23
            fmul dword ptr [yy]                                                        // 0057CA27
            faddp st(1), st                                                            // 0057CA2B
            fld dword ptr [xx]                                                         // 0057CA2D
            fmul dword ptr [xx]                                                        // 0057CA31
            faddp st(1), st                                                            // 0057CA35
            fcomp dword ptr [limit]                                                    // 0057CA37
            fnstsw ax                                                                  // 0057CA3D
            mov status, ax
        }
        if ((status & 0x4100) == 0) run = 1;                                           // 0057CA3F..44 (test ah,41h: > only)
        Vector zero = {0.0f, 0.0f, 0.0f};                                              // 0057CA4B..5B
        void* area = recovered_object_area_004cb120(source, nullptr);                  // 0057CA49..63 (not null-checked)
        recovered_creature_add_move_to_point_action_004f8b60(as_creature(object), nullptr, 0xffff, point,
            at<U>(area, 0x120), from_id, static_cast<U>(run), 0x3f800000u, 0, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0057CA68..A4
    }
    recovered_object_add_action_004cea20(object, nullptr, 0x23, 0xffff,                // 0057CAAA..F0
        3, &item_id, 3, &from_id, 1, &a3, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;                                                                          // 0057CAF7
}

// 004F3A60 CSWSCreature::EquipMostEffectiveArmor(): thiscall RET, EAX 0/1.  Scores the body-armour slot item and
// every repository item whose base-item type byte (+ACh) is in [1Fh, 26h] and that CanEquipItem accepts:
// score = base AC (+244h) + every AC property (type 1, value word +6) whose upgrade bit (+11h, FFh = always) is set
// in +294h.  The strictly better (unsigned) item is equipped (message only when +E8h is clear; feedback C1h for a
// party member in conversation; else EquipItem, then a combat-round equip or an action 8 queue).
// Quirk (original): the ebp register that held 0 for the first loop compare is reused as the score accumulator.
U __fastcall kotor_fastcall_004f3a60(void* creature, void*) {
    void* inventory = at<void*>(creature, 0xa2c);                                      // 004F3A67
    if (inventory == nullptr || at<void*>(creature, 0xa30) == nullptr) return 0;       // 004F3A6F..7F
    U best_item_ptr = 0;                                                               // [esp+14] 004F3A87
    U best_ac = 0;                                                                     // [esp+10] 004F3A8B
    void* current = recovered_equipment_item_005a4c20(inventory, nullptr, 2);          // 004F3A85..98
    auto score = [](void* item, U base) -> U {
        for (I i = 0; i < at<I>(item, 0x24c); ++i) {                                   // 004F3AB3..0A / 004F3B90..DA
            unsigned char* p = static_cast<unsigned char*>(kotor_fastcall_00553990(item, nullptr, i));
            if (p == nullptr) continue;
            const U8 bit = p[0x11];
            if (bit != 0xff && (at<U>(item, 0x294) & (1u << (bit & 31))) == 0) continue; // 004F3ADC..F1
            if (*reinterpret_cast<U16*>(p) == 1) base += *reinterpret_cast<U16*>(p + 6); // 004F3AF3..FD
        }
        return base;
    };
    if (current && kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                     at<I>(current, 0xc))) { // 004F3A9E..B1
        best_item_ptr = word(current);                                                 // 004F3AC3
        best_ac = score(current, at<U>(current, 0x244));                               // 004F3AB9..0A
    }
    U slot = 0;                                                                        // [esp+1C]
    for (U index = 0; index < at<U>(at<void*>(creature, 0xa30), 0x10); ++index) {     // 004F3B0C..1D / 004F3BEA..FE (jb)
        void* item = w2x_inventory_item_005560b0(at<void*>(creature, 0xa30), nullptr, index); // 004F3B23..2D
        if (item == nullptr) continue;
        slot = 2;                                                                      // 004F3B35
        if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) < 0x1f) continue; // 004F3B33..49 (jb)
        if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) > 0x26) continue; // 004F3B4F..5D (ja)
        if (recovered_item_can_equip_0051aa60(creature, nullptr, item, &slot, 0, 0, 1) == 0) continue; // 004F3B63..78
        if (kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                              at<I>(item, 0xc)) == nullptr) continue; // 004F3B7A..8E
        const U ac = score(item, at<U>(item, 0x244));                                  // 004F3B90..DA
        if (ac > best_ac) { best_item_ptr = word(item); best_ac = ac; }                // 004F3BDC..E6 (jbe: unsigned)
    }
    void* best = ptr(best_item_ptr);                                                   // 004F3C04
    if (best == nullptr) return 0;                                                     // 004F3C08..0A
    if (current == best) return 1;                                                     // 004F3C10..14
    U flag = 0;                                                                        // [esp+14] 004F3C22
    U equip_slot = 2;                                                                  // [esp+10] 004F3C2A
    if (at<void*>(creature, 0xa2c) != nullptr &&                                       // 004F3C1A..32
        recovered_inventory_has_item_005a4950(at<void*>(creature, 0xa2c), nullptr, best) != 0) return 1; // 004F3C34..3C
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)); // 004F3C42..4F
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);      // 004F3C54..5F
    U item_id = at<U>(best, 0x14);                                                     // 004F3C6E..71 [esp+18]
    if (at<U>(creature, 0xe8) == 0) {                                                  // 004F3C64..75
        if (client && message)                                                         // 004F3C77..81
            recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(client, 4), item_id, equip_slot); // 004F3C87..91
        return 1;
    }
    if (at<U>(creature, 0x4e0) == 1 && at<U8>(creature, 0xac0) == 1 && equip_slot == 2) { // 004F3CA3..C2
        if (client && message)                                                         // 004F3CC4..CA
            recovered_inventory_slot_message_0056fbd0(message, nullptr, at<U>(client, 4), item_id, equip_slot); // 004F3CCC..D2
        recovered_creature_feedback_004ede10(creature, nullptr, 0xc1, nullptr);        // 004F3CD7..E0
        return 1;
    }
    if (recovered_action_node_clear_004f0310(creature, nullptr, best, equip_slot, 0, flag) != 0) return 1; // 004F3CF2..04
    if (at<U>(creature, 0x4e0) == 1 && at<U8>(creature, 0xac0) == 1) {                 // 004F3D0A..19
        recovered_combat_round_equip_004d3c30(at<void*>(creature, 0x9c8), nullptr, item_id, equip_slot, flag, 0); // 004F3D1B..32
        return 1;
    }
    recovered_object_add_action_004cea20(creature, nullptr, 8, 0xffff,                  // 004F3D44..88
        3, &item_id, 1, &equip_slot, 1, &flag, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;
}

}  // extern "C"
