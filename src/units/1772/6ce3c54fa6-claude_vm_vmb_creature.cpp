// Lane vmB, file "creature": creature / faction query NWScript engine-command handlers of package vmB
// (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only, capstone).
// Every handler: original int __thiscall (int routine, int argc), RET 8 (verified) ->
//   extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc) @16.
// Addresses in comments are original instruction ranges.  Conventions: claude_vm_vmB_common.hpp.
// .rdata literals: [00746644] 7F000000h OBJECT_INVALID; [00746634] byte 5 creature, [00746638] byte 9 placeable,
// [00746639] byte 0Ah door (object type bytes); [00746878] 400h damage-flag bound; [0074666C] byte 0 "found".
// Original quirks kept (see each handler):
//  * GetDamageDealtByType / GetLastLocked / GetLastDisturbed: when OBJECT_SELF is not a CSWSObject the original
//    reuses the stack slot holding `this` (the CSWVirtualMachineCommands*) as the object pointer.
//  * GetMinOneHP: an invalid object pushes the popped object id itself as the integer result.
//  * GetAttacker: the result register is never assigned from Get{First,Next}Attacker, so OBJECT_INVALID is always
//    pushed; the attacker iteration (and its list rebuild) still runs.
//  * SetMaxHitPoints / GetGoingToBeAttackedBy: a pop failure returns -2000; GetGoingToBeAttackedBy's push failure
//    returns -2001 (and 7D1h / F82Fh literals).
//  * GetLastAttackType reads the attack type word from the creature (+16Ch) after testing +4E0h for null.
#include "54db204e3b-claude_vm_vmb_common.hpp"

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); } // CExoArrayList::Allocate [0098]

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
U __fastcall kotor_fastcall_004cafe0(void* object, void*, U flags);                       // CSWSObject::GetLastDamageAmountByFlags RET 4 [claude_vm_actions_queries]
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);                     // CSWSObject::SetCurrentHitPoints RET 4 [0886]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                     // CSWSCreature::GetFaction [w1_native_b7]
void __fastcall recovered_faction_remove_005bf920(void* faction, void*, U id);             // CSWSFaction::RemoveMember RET 4 [creature_destructor_r2]
U __fastcall w1x_creature_max_fp_004fd490(void* creature, void*);                          // CSWSCreature::GetMaxForcePoints (AX) [0886]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                 // CServerExoApp::GetPlayerCreatureId [0595]
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                 // CFactionManager::GetFaction RET 4 [claude_vm_actions_world]
U __fastcall kotor_fastcall_005ab810(void* stats, void*, U flag);                          // CSWSCreatureStats::GetFortSavingThrow RET 4 (AL) [claude_combat_attack_rolls]
U __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                   // CSWSCreatureStats::GetLevel RET 4 (AL) [0963]
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_005ba400(void* creature, void*);                               // CSWSCreature::GetFirstAttacker
U __fastcall kotor_fastcall_005b7fe0(void* creature, void*);                               // CSWSCreature::GetNextAttacker
U __fastcall kotor_fastcall_005bed00(void* faction, void*, U target, U check_los);         // CSWSFaction::GetMostDamagedMember
I __fastcall kotor_fastcall_005bf140(void* faction, void*);                                // CSWSFaction::GetAverageLevel
}

using namespace vmb;

namespace {
inline void* stats_of(void* creature) { return at<void*>(creature, 0xa74); }
// [[[7A39FC]+8]+4]: CServerExoAppInternal; +10054h CFactionManager*
inline void* server_internal() { return at<void*>(server_app(), 4); }
}  // namespace

extern "C" {

// ===================================================================================================== 00537250
// GetDamageDealtByType(int nDamageType) [routine 344] on OBJECT_SELF
int __fastcall kotor_fastcall_00537250(void* self, void*, int, unsigned) {
    U result = 0;                                                                        // 00537262 edi = 0
    void* object = game_object(self_id(self));                                           // 00537251..64
    void* target;
    if (object != nullptr && as_object(object) != nullptr) target = as_object(object);   // 0053726B..81 (second call)
    else target = self;                                                                  // 00537285 quirk: [esp+8] = this
    I flags = 0;
    if (!pop_int(&flags)) return k_pop_failed;                                           // 00537289..A5
    if (target != nullptr && flags > 0 && flags < 0x400) {                               // 005372A8..BA (signed)
        const U8 type = static_cast<U8>(type_of(target));                                // 005372BC
        if (type == 5 || type == 9 || type == 10)                                        // 005372BF..C9
            result = kotor_fastcall_004cafe0(target, nullptr, static_cast<U>(flags));    // 005372CB..D3
    }
    return push_int(static_cast<I>(result));                                             // 005372D5..F2
}

// ===================================================================================================== 00545F10
// GetMinOneHP(object oObject) [routine 715]: CSWSObject +200h
int __fastcall kotor_fastcall_00545f10(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00545F11..2B
    void* object = game_object(id);                                                      // 00545F2E..42
    I value;
    if (object != nullptr && as_object(object) != nullptr)                               // 00545F44..51
        value = at<I>(as_object(object), 0x200);                                         // 00545F53..5A
    else
        value = static_cast<I>(id);                                                      // 00545F62 quirk: the popped id slot
    return push_int(value);                                                              // 00545F66..82
}

// ===================================================================================================== 00547D40
// SetMaxHitPoints(object oObject, int nMaxHP) [routine 758]: +E0h = nMaxHP, then SetCurrentHitPoints(nMaxHP)
int __fastcall kotor_fastcall_00547d40(void*, void*, int, unsigned) {
    U id = 0; I hp = 0;
    if (!pop_object(&id)) return k_push_failed;                                          // 00547D40..55 quirk: -2000
    if (!pop_int(&hp)) return k_push_failed;                                             // 00547D57..73 quirk: -2000
    void* object = game_object(id);                                                      // 00547D76..89
    if (object != nullptr) {
        void* o = as_object(object);                                                     // 00547D8D..96
        if (o != nullptr) {
            at<I>(o, 0xe0) = hp;                                                         // 00547D98..9C
            w1x_object_set_hp_004cc280(o, nullptr, static_cast<U>(hp));                  // 00547DA2..A9
        }
    }
    return 0;                                                                            // 00547DAE
}

// ===================================================================================================== 00541BA0
// RemoveFromParty(object oPC) [routine 573]
int __fastcall kotor_fastcall_00541ba0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00541BA1..BB
    void* creature = creature_by_id(id);                                                 // 00541BBE..D1
    if (creature != nullptr && at<U>(creature, 0x9d4) != 0) {                            // 00541BD3..DD
        void* faction = combat_reuse_faction_00513fc0(creature, nullptr);                // 00541BDF..E1
        if (faction != nullptr) recovered_faction_remove_005bf920(faction, nullptr, id); // 00541BE6..F1
    }
    return 0;                                                                            // 00541BF6
}

// ===================================================================================================== 00539760
// GetMaxForcePoints(object oObject) [routine 56]
int __fastcall kotor_fastcall_00539760(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00539761..7B
    void* creature = creature_by_id(id);                                                 // 0053977E..8C
    I value = 0;                                                                         // 00539791
    if (creature != nullptr)
        value = static_cast<short>(static_cast<U16>(w1x_creature_max_fp_004fd490(creature, nullptr))); // 00539797..9E movsx
    return push_int(value);                                                              // 005397A1..BC
}

// ===================================================================================================== 00546BD0
// GetCurrentStealthXP() [routine 474]: area of the player creature, +2B8h
int __fastcall kotor_fastcall_00546bd0(void*, void*, int, unsigned) {
    void* server = server_app();                                                         // 00546BD0..D6
    void* creature = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00546BD9..E3
    I value = 0;                                                                         // 00546BE8
    if (creature != nullptr) {
        void* area = recovered_object_area_004cb120(creature, nullptr);                  // 00546BEE..F0
        if (area != nullptr) value = at<I>(area, 0x2b8);                                 // 00546BF5..F9
    }
    return push_int(value);                                                              // 00546BFF..1A
}

// ===================================================================================================== 00546EE0
// GetStealthXPDecrement() [routine 498]: area of the player creature, +2BCh
int __fastcall kotor_fastcall_00546ee0(void*, void*, int, unsigned) {
    void* server = server_app();                                                         // 00546EE0..E6
    void* creature = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00546EE9..F3
    I value = 0;                                                                         // 00546EF8
    if (creature != nullptr) {
        void* area = recovered_object_area_004cb120(creature, nullptr);                  // 00546EFE..00
        if (area != nullptr) value = at<I>(area, 0x2bc);                                 // 00546F05..09
    }
    return push_int(value);                                                              // 00546F0F..2A
}

// ===================================================================================================== 00546190
// GetFirstAttacker(object oCreature=OBJECT_SELF) [routine 727] / GetNextAttacker(...) [routine 728]
int __fastcall kotor_fastcall_00546190(void* self, void*, int routine, unsigned argc) {
    U id = self_id(self);                                                                // 00546191..94
    const U result = k_invalid;                                                          // 0054619E esi = [746644]
    if (static_cast<I>(argc) >= 1 && !pop_object(&id)) return k_pop_failed;              // 00546198..C1 (jl)
    void* creature = creature_by_id(id);                                                 // 005461C4..D2
    if (creature != nullptr) {                                                           // 005461D7..DB
        if (routine == 0x2d7) (void)kotor_fastcall_005ba400(creature, nullptr);           // 005461DD..ED
        else if (routine == 0x2d8) (void)kotor_fastcall_005b7fe0(creature, nullptr);      // 005461EF..F6
        // 005461FB..02: only an OBJECT_INVALID return re-loads esi (with OBJECT_INVALID): result never changes
    }
    return push_object(result);                                                          // 00546208..24
}

// ===================================================================================================== 00538800
// GetFactionMostDamagedMember(object oFactionMember=OBJECT_SELF, int bMustBeVisible=TRUE) [routine 183]
int __fastcall kotor_fastcall_00538800(void*, void*, int, unsigned argc) {
    U id = k_invalid; I visible = 1;                                                     // 00538814 / 00538832
    U result = k_invalid;                                                                // 0053880E esi
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053880A..2A
    if (argc == 2 && !pop_int(&visible)) return k_pop_failed;                            // 0053882D..4E
    void* creature = creature_by_id(id);                                                 // 00538850..63
    if (creature != nullptr) {
        const I faction_id = at<I>(stats_of(creature), 0x78);                            // 00538867 / 0053887B
        void* internal = server_internal();                                              // 0053886D..79
        if (internal != nullptr) {
            void* manager = at<void*>(internal, 0x10054);                                // 00538880..88
            if (manager != nullptr)
                result = kotor_fastcall_005bed00(kotor_fastcall_0052b3b0(manager, nullptr, faction_id), nullptr,
                                                 id, static_cast<U>(visible));           // 0053888A..A3
        }
    }
    return push_object(result);                                                          // 005388A5..C3
}

// ===================================================================================================== 00538190
// GetFactionAverageLevel(object oFactionMember) [routine 189]
int __fastcall kotor_fastcall_00538190(void*, void*, int, unsigned) {
    U id = k_invalid;                                                                    // 005381A0
    I result = -1;                                                                       // 0053819D esi = -1
    if (!pop_object(&id)) return k_pop_failed;                                           // 00538198..B8
    void* creature = creature_by_id(id);                                                 // 005381BB..CE
    if (creature != nullptr) {
        const I faction_id = at<I>(stats_of(creature), 0x78);                            // 005381D2..D8
        void* internal = server_internal();                                              // 005381DB..E9
        if (internal != nullptr) {
            void* manager = at<void*>(internal, 0x10054);                                // 005381EB..F3
            if (manager != nullptr)
                result = kotor_fastcall_005bf140(kotor_fastcall_0052b3b0(manager, nullptr, faction_id), nullptr); // 005381F5..04
        }
    }
    return push_int(result);                                                             // 00538206..22
}

// ===================================================================================================== 00538BE0
// GetFortitudeSavingThrow(object oTarget) [routine 491]
int __fastcall kotor_fastcall_00538be0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00538BE1..FB
    void* object = game_object(id);                                                      // 00538BFE..12
    I result = 0;                                                                        // 00538C14
    if (object != nullptr) {
        const U8 type = static_cast<U8>(type_of(object));                                // 00538C1A
        if (type == 5) {                                                                 // 00538C1D [00746634]
            void* creature = as_creature(object);                                        // 00538C25..27
            if (creature != nullptr && stats_of(creature) != nullptr)                    // 00538C2A..36
                result = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005ab810(stats_of(creature), nullptr, 0))); // 00538C38..3E
        } else if (type == 0xa) {                                                        // 00538C43 [00746639]
            void* door = as_door(object);                                                // 00538C4B..4D
            if (door != nullptr) result = at<U8>(door, 0x2bc);                           // 00538C50..54
        } else if (type == 9) {                                                          // 00538C5D [00746638]
            void* placeable = as_placeable(object);                                      // 00538C65..67
            if (placeable != nullptr) result = at<U8>(placeable, 0x314);                 // 00538C6A..6E
        }
    }
    return push_int(result);                                                             // 00538C75..91
}

// ===================================================================================================== 0053B0E0
// GetLastLocked() [routine 349] on OBJECT_SELF: door +314h, placeable +364h
int __fastcall kotor_fastcall_0053b0e0(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                                // 0053B0F2
    void* object = game_object(self_id(self));                                           // 0053B0E1..F7
    void* target;
    if (object != nullptr && as_object(object) != nullptr) target = as_object(object);   // 0053B0FC..14
    else target = self;                                                                  // 0053B118 quirk: [esp+8] = this
    if (target != nullptr) {                                                             // 0053B11C
        const U8 type = static_cast<U8>(type_of(target));                                // 0053B120
        if (type == 0xa) result = at<U>(as_door(target), 0x314);                         // 0053B123..2C (unchecked)
        else if (type == 9) result = at<U>(as_placeable(target), 0x364);                 // 0053B134..3D (unchecked)
    }
    return push_object(result);                                                          // 0053B143..60
}

// ===================================================================================================== 0053ABB0
// GetLastAttackType(object oCreature=OBJECT_SELF) [routine 317]
int __fastcall kotor_fastcall_0053abb0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053ABB6..CF
    I result = 0;                                                                        // 0053ABE6
    void* object = nullptr;
    if (lookup(id, &object) && as_creature(object) != nullptr) {                         // 0053ABD2..07
        void* creature = as_creature(object);                                            // 0053AC09..0F
        if (at<void*>(creature, 0x4e0) != nullptr) {                                     // 0053AC12..1A
            const U kind = at<U16>(creature, 0x16c);                                     // 0053AC1C
            if (kind == 0xb) result = 9;                                                 // 0053AC23..26 / 0053AC38
            else result = kind == 0x1e ? 0xa : 0;                                        // 0053AC28..34 setne/dec/and 0Ah
        }
    }
    return push_int(result);                                                             // 0053AC3D..5B
}

// ===================================================================================================== 0053AE50
// GetLastDisturbed() [routine 348] on OBJECT_SELF: creature / placeable +164h
int __fastcall kotor_fastcall_0053ae50(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                                // 0053AE62
    void* object = game_object(self_id(self));                                           // 0053AE51..67
    void* target;
    if (object != nullptr && as_object(object) != nullptr) target = as_object(object);   // 0053AE6C..84
    else target = self;                                                                  // 0053AE88 quirk: [esp+8] = this
    if (target != nullptr) {                                                             // 0053AE8C
        const U8 type = static_cast<U8>(type_of(target));                                // 0053AE90
        if (type == 5) result = at<U>(as_creature(target), 0x164);                       // 0053AE93..A7 (unchecked)
        else if (type == 9) result = at<U>(as_placeable(target), 0x164);                 // 0053AE9E..A7 (unchecked)
    }
    return push_object(result);                                                          // 0053AEAD..CA
}

// ===================================================================================================== 00538FC0
// GetGoingToBeAttackedBy(object oTarget) [routine 211]: creature +520h
int __fastcall kotor_fastcall_00538fc0(void*, void*, int, unsigned) {
    U id = 0;
    U result = k_invalid;                                                                // 00538FCD
    if (!pop_object(&id)) return k_push_failed;                                          // 00538FC8..E2 quirk: -2000
    void* creature = creature_by_id(id);                                                 // 00538FE5..F3
    if (creature != nullptr) result = at<U>(creature, 0x520);                            // 00538FF8..FC
    // 00539002..1E: neg/sbb/and 7D1h/add F82Fh -> push failure returns -2001
    return push_object_raw(result) ? 0 : k_pop_failed;
}

// ===================================================================================================== 0053C010
// GetMovementRate(object oCreature=OBJECT_SELF) [routine 496]: stats +194h
int __fastcall kotor_fastcall_0053c010(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053C011..2B
    void* creature = creature_by_id(id);                                                 // 0053C02E..3C
    I result = 0;                                                                        // 0053C041
    if (creature != nullptr && stats_of(creature) != nullptr)                            // 0053C043..4F
        result = at<I>(stats_of(creature), 0x194);                                       // 0053C051
    return push_int(result);                                                             // 0053C057..72
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 005BA400 CSWSCreature::GetFirstAttacker(): thiscall, plain RET.  Rebuilds the attacker list (+A80h, a heap
// CExoArrayList<ulong>) from the area's creature list (+190h data, +194h count): every creature whose attack
// target (+504h) is this creature's id.  Non-empty: iterator +A84h = 1 and returns the first id; else 7F000000h.
// Quirk: a failed operator new stores a null list that the loop then dereferences.
U __fastcall kotor_fastcall_005ba400(void* creature, void*) {
    void* area = recovered_object_area_004cb120(creature, nullptr);                      // 005BA419..1B
    if (area == nullptr) return k_invalid;                                               // 005BA424..2A / 005BA528
    U* old_list = at<U*>(creature, 0xa80);                                               // 005BA431
    if (old_list != nullptr) {                                                           // 005BA437..39
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old_list[0])));    // 005BA43B..3E
        old_list[0] = 0;                                                                 // 005BA444
        live_free(old_list);                                                             // 005BA443..46
    }
    U* list = static_cast<U*>(live_operator_new(0xc));                                   // 005BA44E..55
    if (list != nullptr) { list[1] = 0; list[2] = 0; list[0] = 0; }                      // 005BA458..62
    at<U*>(creature, 0xa80) = list;                                                      // 005BA468
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                         // 005BA46E..7C / 005BA4E5..F2 (signed)
        const U id = at<U*>(area, 0x190)[i];                                             // 005BA490..96
        void* other = creature_by_id(id);                                                // 005BA499..A3
        if (other != nullptr && at<U>(other, 0x504) == at<U>(creature, 4)) {             // 005BA4A8..B5
            U* l = at<U*>(creature, 0xa80);                                              // 005BA4B7
            if (l[1] == l[2])                                                            // 005BA4BD..C3
                menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(l),
                                                    nullptr, l[2] != 0 ? l[2] * 2 : 0x10); // 005BA4C5..D5
            reinterpret_cast<U*>(static_cast<std::uintptr_t>(l[0]))[l[1]] = id;          // 005BA4DA..DF
            ++l[1];                                                                      // 005BA4E2
        }
    }
    U* l = at<U*>(creature, 0xa80);                                                      // 005BA4F4
    if (static_cast<I>(l[1]) <= 0) return k_invalid;                                     // 005BA4FA..00 (jle)
    at<U>(creature, 0xa84) = 1;                                                          // 005BA502
    return reinterpret_cast<U*>(static_cast<std::uintptr_t>(l[0]))[0];                   // 005BA50C..0E
}

// 005B7FE0 CSWSCreature::GetNextAttacker(): thiscall, plain RET.
U __fastcall kotor_fastcall_005b7fe0(void* creature, void*) {
    U* list = at<U*>(creature, 0xa80);                                                   // 005B7FE1
    if (list == nullptr) return k_invalid;                                               // 005B7FE7..EE
    const I index = at<I>(creature, 0xa84);                                              // 005B7FF0
    if (index >= static_cast<I>(list[1])) return k_invalid;                              // 005B7FF6..F9 (jge signed)
    at<I>(creature, 0xa84) = index + 1;                                                  // 005B7FFB..FE
    return reinterpret_cast<U*>(static_cast<std::uintptr_t>(list[0]))[index];            // 005B8004..06
}

// 005BED00 CSWSFaction::GetMostDamagedMember(ulong target, int check_los): thiscall, RET 8.  Faction member list
// +0 data, +4 count.  Picks the live member (vtbl+94h == 0, GetDeadTemp == 0) with the largest
// (short)vtbl+98h(1) - (short)vtbl+9Ch(0) above the running best (start -1); with a valid target and check_los,
// a candidate also needs ClearLineOfSight(member area, member +90h, target +90h) == 1.
U __fastcall kotor_fastcall_005bed00(void* faction, void*, U target, U check_los) {
    I best = -1;                                                                         // 005BED13 [esp+0C]
    U result = k_invalid;                                                                // 005BED1B [esp+10]
    float hit[3];                                                                        // [esp+20] (zeroed per test)
    U unused_out;                                                                        // [esp+1C] (never initialised)
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                          // 005BED0A..23 / 005BEE3B..45 (signed)
        const U* members = at<U*>(faction, 0);
        void* member = creature_by_id(members[i]);                                       // 005BED30..3F
        if (member == nullptr) continue;                                                 // 005BED46..48
        if (vcall<U>(member, 0x94) != 0) continue;                                       // 005BED4E..5A
        if (recovered_creature_incapacitated_004ef890(member, nullptr) != 0) continue;   // 005BED60..69
        const I maximum = static_cast<short>(static_cast<U16>(vcall<U>(member, 0x98, 1u))); // 005BED6F..81
        const I current = static_cast<short>(static_cast<U16>(vcall<U>(member, 0x9c, 0u))); // 005BED7B..8A
        const I damage = maximum - current;                                              // 005BED8D
        if (damage <= best) continue;                                                    // 005BED8F..93 (jle)
        if (target == k_invalid || check_los == 0) {                                     // 005BED99..AE
            result = at<U*>(faction, 0)[i];                                              // 005BEE59..5F
            best = damage;                                                               // 005BEE63
            continue;
        }
        hit[0] = 0.0f; hit[1] = 0.0f; hit[2] = 0.0f;                                     // 005BEDBA..CA
        void* other = creature_by_id(target);                                            // 005BEDB4..DB
        if (other == nullptr) continue;                                                  // 005BEDDD..DF
        if (recovered_object_area_004cb120(member, nullptr) == nullptr) continue;        // 005BEDE1..EA
        void* area = recovered_object_area_004cb120(member, nullptr);                    // 005BEE12..14
        if (area_clear_line_of_sight_0050c330(area, nullptr, static_cast<unsigned char*>(member) + 0x90,
                static_cast<unsigned char*>(other) + 0x90, hit, &unused_out, k_invalid, k_invalid, 0, 0) == 1) { // 005BEDEC..23
            result = at<U*>(faction, 0)[i];                                              // 005BEE25..2F
            best = damage;                                                               // 005BEE33
        }
    }
    return result;                                                                       // 005BEE4B / 005BED0E
}

// 005BF140 CSWSFaction::GetAverageLevel(): thiscall, plain RET.  Sum of GetLevel(0) (AL, zero-extended) over the
// members that resolve to creatures, divided (idiv) by the full member count; 0 for an empty faction.
I __fastcall kotor_fastcall_005bf140(void* faction, void*) {
    I sum = 0;                                                                           // 005BF148
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                          // 005BF14A..4E / 005BF17B..81
        void* member = creature_by_id(at<U*>(faction, 0)[i]);                            // 005BF150..5E
        if (member != nullptr)                                                           // 005BF163..65
            sum += static_cast<U8>(recovered_stats_level_005a5fd0(stats_of(member), nullptr, 0)); // 005BF167..79
    }
    const I count = at<I>(faction, 4);                                                   // 005BF183
    if (count <= 0) return 0;                                                            // 005BF186..88 / 005BF195
    return sum / count;                                                                  // 005BF18A..8E cdq; idiv
}

}  // extern "C"
