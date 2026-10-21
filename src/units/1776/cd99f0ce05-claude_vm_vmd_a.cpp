// Lane vmD (sub-worker A): NWScript engine-command handlers CSWVirtualMachineCommands::ExecuteCommand*,
// reconstructed from the original swkotor.exe (read-only, capstone linear sweep). Comments name original instructions.
//  * Original: int __thiscall ExecuteCommandX(int routine, int argc), RET 8 (verified for every handler below)
//    -> extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc) (@16).
//  * Returns: 0 success, -2001 (0xFFFFF82F) pop failure, -2000 (0xFFFFF830) push failure (original literals).
//  * CSWVirtualMachineCommands: +4 "run-script object valid" flag, +8 OBJECT_SELF id of the running script.
//  * CGameEffect: operator new(8Ch) + CGameEffect(1); +8 WORD type, +0A WORD flags (bits 0..2 duration type,
//    bits 3/4 subtype).  Effect type constants are the proven .rdata words (address in the name).
//  * Original quirks kept: a failed operator new is not checked before the effect field stores (null write);
//    a failed StackPushEngineStructure leaks the new effect / event; found objects whose pointer is used for a
//    virtual call are not null-checked where the original does not check them.
//  * 006FA390 is the CRT free -> live_free; 004015A0 (CScriptLocation dtor) is a bare `ret`.
#include "061872cb05-claude_vm_actions_common.hpp"

extern "C" {
// ---- CGameEffect providers (decorated arity checked against the original RET) ----
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);         // CGameEffect(int) RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);   // SetInteger RET 8 [0886-cohort-patched]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);              // SetCreator RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                    // ~CGameEffect [w1_native_b7_0056cbf0]
void __fastcall kotor_fastcall_00504240(void* effect, void*);                              // UpdateLinked [claude_combat_attack_damage]
// ---- rules / stats / combat ----
extern void* kotor_cell_rules_007a3a28;                                                     // g_pRules [007A3A28] [0421]
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7); // CSWSCreature::SavingThrowRoll RET 1Ch (AL) [claude_combat_attack_rolls]
U __fastcall recovered_attack_melee_005a7770(void* stats, void*, U a1, U a2, U a3, U a4);  // CSWSCreatureStats::GetMeleeAttackBonus RET 10h [1051]
U __fastcall recovered_attack_ranged_005a7b60(void* stats, void*, U a1, U a2, U a3);       // CSWSCreatureStats::GetRangedAttackBonus RET 0Ch [1051]
U __fastcall kotor_fastcall_005ad0e0(void* stats, void*, void* versus, U flag);            // CSWSCreatureStats::GetArmorClassVersus RET 8 (AX) [claude_combat_attack_attackstats]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);     // CSWRules::RollDice RET 8 (AX) [w1_native_b7]
U __fastcall kotor_fastcall_005b8e50(void* creature, void*, void* target, U hit);          // CSWSCreature::ResolveDefensiveEffects RET 8 [claude_combat_attack_rolls]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);      // CSWSCombatRound::GetAttack RET 4 [w1_native_b7]
// ---- CSWCCMessageData / CScriptEvent (shared SetObjectID/SetInteger/SetFloat bodies) ----
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);             // CSWCCMessageData() [w1_native_b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                // ~CSWCCMessageData [w1_native_b7]
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                   // CScriptEvent() [1017]
void __fastcall recovered_event_destroy_004d7590(void* event, void*);                      // ~CScriptEvent [0886-cohort-patched]
void __fastcall recovered_event_object_004d7780(void* data, void*, U index, U id);         // SetObjectID RET 8 [1017]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);     // SetInteger RET 8 [1017]
void __fastcall kotor_fastcall_004d7720(void* data, void*, I index, float value);          // SetFloat RET 8 [claude_combat_attack_damage]
// ---- server / client app ----
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                       // GetClientObjectByObjectId RET 4 [0595]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);               // GetSWSMessage [creature_destructor_r2]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage RET 10h
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                             // CServerExoApp::GetWorldTimer [0457-successor]
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                             // CClientExoApp::GetSWParty [0707]
}

using namespace vmx;

namespace {
// .rdata constants (proven values)
constexpr U16 k_effect_invalid_0074667c = 0;
constexpr U16 k_effect_damage_resistance_00746684 = 2;
constexpr U16 k_effect_choke_0074669c = 8;
constexpr U16 k_effect_damage_increase_007466b0 = 0x0d;
constexpr U16 k_effect_damage_decrease_007466b4 = 0x0e;
constexpr U16 k_effect_temporary_hitpoints_007466b8 = 0x0f;
constexpr U16 k_effect_movement_speed_decrease_007466f0 = 0x1d;
constexpr U16 k_effect_force_resistance_decrease_00746704 = 0x22;
constexpr U16 k_effect_ability_decrease_00746710 = 0x25;
constexpr U16 k_effect_link_0074671c = 0x28;
constexpr U16 k_effect_ac_increase_0074673c = 0x30;
constexpr U16 k_effect_ac_decrease_00746740 = 0x31;
constexpr U16 k_effect_spell_immunity_00746744 = 0x32;
constexpr U16 k_effect_true_seeing_0074679c = 0x48;
constexpr U16 k_effect_concealment_007467ac = 0x4c;
constexpr U16 k_effect_temporary_force_points_007467e4 = 0x5b;
constexpr U16 k_effect_force_shield_00746824 = 0x6b;
constexpr I k_ability_default_00746838 = 0;
constexpr I k_ability_max_0074684c = 5;
constexpr I k_damage_type_fallback_0074685c = 8;
constexpr I k_damage_type_max_00746884 = 0x2000;
constexpr I k_ac_damage_type_default_0074688c = 0x4007;
constexpr I k_ac_modify_type_max_007468c0 = 4;
constexpr U8 k_type_placeable_00746638 = 9;
constexpr U8 k_type_door_00746639 = 10;
constexpr U8 k_type_creature = 5;                    // literal 5 at 00542283
constexpr float k_seconds_per_round_00740c84 = 3.0f;
constexpr float k_seconds_per_turn_007455dc = 30.0f;
constexpr float k_seconds_per_minute_00748734 = 60.0f;

inline U self_id(void* self) { return at<U>(self, 8); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline I push_status(U pushed) { return pushed ? 0 : k_push_failed; }      // neg/sbb/and 7D0h/add F830h
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline U pop_object(U* id) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, id); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }     // vtbl+10 AsSWSObject
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }   // vtbl+30 AsSWSCreature
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }  // vtbl+48 AsSWSPlaceable
inline void* stats_of(void* creature) { return at<void*>(creature, 0xa74); }
inline void* round_of(void* creature) { return at<void*>(creature, 0x9c8); }

// operator new(8Ch) + CGameEffect(1); a null allocation yields a null effect that the caller then writes through
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void set_type(void* e, U16 type) { at<U16>(e, 8) = type; }
// xor r,r; mov r16,[e+0A]; and r,FFEF; or r,8; mov [e+0A],r16
inline void set_subtype_magical(void* e) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xffefu) | 8u); }
inline void set_int(void* e, U index, U value) { recovered_effect_integer_005036a0(e, nullptr, index, value); }
inline U rules_aa() { return at<U8>(rules(), 0xaa); }                       // movzx [[007A3A28]+0AAh]
// GetGameObject(self id) found && object != null -> SetCreator(self id)
inline void creator_if_object(void* self, void* e) {
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr) recovered_effect_creator_00503a00(e, nullptr, self_id(self));
}
// GetGameObject(self id) found && object->AsSWSCreature() -> SetCreator(self id) (object pointer unchecked)
inline void creator_if_creature(void* self, void* e) {
    void* object = nullptr;
    if (lookup(self_id(self), &object) && as_creature(object) != nullptr) recovered_effect_creator_00503a00(e, nullptr, self_id(self));
}
// StackPushEngineStructure(0, e): failure -> -2000 (effect leaked); success -> ~CGameEffect + free, 0
inline int push_effect(void* e) {
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 0, e)) return k_push_failed;
    recovered_effect_destroy_00503590(e, nullptr);
    live_free(e);
    return 0;
}

// EffectACIncrease / EffectACDecrease share one instruction-for-instruction body (00530B20 / 00530960):
// pop value; pop modify type; damage type = [0074688C] unless argc >= 3 (signed jl) pops it.
int ac_effect(void* self, unsigned argc, U16 type) {
    I value = 0, modify = 0, damage = 0;                                                // locals +C / +4 / +8
    if (!pop_int(&value)) return k_pop_failed;                                          // ..0979..98D
    if (!pop_int(&modify)) return k_pop_failed;                                         // ..098F..9A1
    damage = k_ac_damage_type_default_0074688c;                                         // ..09A8..9AE
    if (static_cast<I>(argc) >= 3 && !pop_int(&damage)) return k_pop_failed;           // ..09A3/9B2..9C6
    if (modify < 0 || modify > k_ac_modify_type_max_007468c0) modify = 0;               // ..09DF..9EF
    void* e = new_effect();                                                             // ..09F8..A22
    set_type(e, type);                                                                  // ..0A24..A31
    set_subtype_magical(e);                                                             // ..0A2B..A48
    set_int(e, 0, static_cast<U>(modify));                                              // ..0A4C..A53
    set_int(e, 1, static_cast<U>(value));                                               // ..0A58..A61
    set_int(e, 2, rules_aa());                                                          // ..0A66..A78
    set_int(e, 5, static_cast<U>(damage));                                              // ..0A7D..A86
    creator_if_object(self, e);                                                         // ..0A8B..ABF
    return push_effect(e);                                                              // ..0AC4..B10
}

// EffectDamageIncrease / EffectDamageDecrease (005321B0 / 00531D50), same body, different type word
int damage_bonus_effect(void* self, U16 type) {
    I bonus = 0, damage = 0;                                                            // locals +4 / +8
    if (!pop_int(&bonus)) return k_pop_failed;                                          // ..21C9..21DD
    if (!pop_int(&damage)) return k_pop_failed;                                         // ..21DF..21F1
    if (bonus <= 0 || bonus > 10) bonus = 0;                                            // ..220A..2217 (jle / jle)
    if (damage < 0 || damage > k_damage_type_max_00746884) damage = k_damage_type_fallback_0074685c; // ..221F..2235
    void* e = new_effect();                                                             // ..2239..2264
    set_type(e, type);                                                                  // ..2266..2272
    set_subtype_magical(e);                                                             // ..226C..2287
    set_int(e, 0, static_cast<U>(bonus));                                               // ..228B..2294
    set_int(e, 1, static_cast<U>(damage));                                              // ..2299..22A2
    set_int(e, 2, rules_aa());                                                          // ..22A7..22B9
    creator_if_object(self, e);                                                         // ..22BE..22F1
    return push_effect(e);                                                              // ..22F6..2342
}

// EffectTemporaryHitpoints / EffectTemporaryForcePoints (00534490 / 00534340), same body, different type word
int temporary_points_effect(void* self, U16 type) {
    I amount = 0;
    if (!pop_int(&amount)) return k_pop_failed;                                         // ..44A9..44BD
    void* e = new_effect();                                                             // ..44D6..4501
    set_subtype_magical(e);                                                             // ..4503..451A
    if (amount > 0) {                                                                   // ..451E..4524 (jle)
        set_type(e, type);                                                              // ..4526..452D
        set_int(e, 0, static_cast<U>(amount));                                          // ..4531..453A
    } else {
        set_type(e, k_effect_invalid_0074667c);                                         // ..4541..4548
    }
    creator_if_creature(self, e);                                                       // ..454C..4585
    return push_effect(e);                                                              // ..458A..45D6
}

// EffectDroidStun / EffectWhirlWind (00534810 / 00534C20): choke-type effect with integer 0 = sub kind
int choke_effect(U kind) {
    void* e = new_effect();                                                             // ..4827..4851
    set_type(e, k_effect_choke_0074669c);                                               // ..4853..4867
    set_int(e, 0, kind);                                                                // ..4859..486B
    return push_effect(e);                                                              // ..4870..48BA
}
}  // namespace

extern "C" {

// ===================================================================================================== 0053D510
// GetSpellTargetObject() [routine 47]
int __fastcall kotor_fastcall_0053d510(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                               // 0053D518 edi = [00746644]
    void* object = nullptr;
    if (at<I>(self, 4) == 1 && lookup(self_id(self), &object)) {                        // 0053D513..44
        void* sws = as_object(object);                                                  // 0053D546..50 (object unchecked)
        if (sws != nullptr && (as_creature(sws) != nullptr || as_placeable(sws) != nullptr)) // 0053D552..6A
            result = at<U>(sws, 0x1bc);                                                 // 0053D56C
    }
    return push_status(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result)); // 0053D573..8F
}

// ===================================================================================================== 00534810
// EffectDroidStun() [routine 391]
int __fastcall kotor_fastcall_00534810(void*, void*, int, unsigned) { return choke_effect(3); } // 00534859 push 3

// ===================================================================================================== 005421E0
// FortitudeSave / ReflexSave / WillSave(object oCreature, int nDC, int nSaveType=0, object oSaveVersus=OBJECT_SELF)
// [routines 108 / 109 / 110]
// Quirk (original): nSaveType (local +10h) is not initialised when argc <= 2 (written as 0 here; the compiler
// always pushes all four arguments); a routine outside 108..110 passes argc as the save kind (005422AD).
int __fastcall kotor_fastcall_005421e0(void* self, void*, int routine, unsigned argc) {
    U creature_id = k_invalid;                                                          // 005421F2 local +4
    U versus = self_id(self);                                                           // 005421E3/FA local +8
    I dc = 0, save_type = 0;                                                            // locals +14 / +10
    U result = 0;                                                                       // 005421FE esi = 0
    if (!pop_object(&creature_id)) return k_pop_failed;                                 // 00542200..12
    if (!pop_int(&dc)) return k_pop_failed;                                             // 00542215..27
    if (static_cast<I>(argc) > 2) {                                                     // 00542229..2E (jle)
        if (!pop_int(&save_type)) return k_pop_failed;                                  // 00542230..42
        if (!pop_object(&versus)) return k_pop_failed;                                  // 00542244..56
    }
    void* object = nullptr;
    if (lookup(creature_id, &object) && at<U8>(object, 8) == k_type_creature) {         // 00542258..87
        U kind;
        switch (routine) {                                                              // 00542289..96
        case 108: kind = 1; break;                                                      // 005422A6
        case 109: kind = 2; break;                                                      // 0054229F
        case 110: kind = 3; break;                                                      // 00542298
        default: kind = argc; break;                                                    // 005422AD [esp+20h] = argc
        }
        void* creature = as_creature(object);                                           // 005422C7..C9
        result = kotor_fastcall_005b92b0(creature, nullptr, kind, static_cast<U>(dc), static_cast<U>(save_type),
                                         versus, 1, 0, 0) & 0xffu;                      // 005422B1..D3 movzx esi, al
    }
    return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result))); // 005422D6..F4
}

// ===================================================================================================== 0053C8F0
// GetPartyMemberCount() [routine 126]
int __fastcall kotor_fastcall_0053c8f0(void*, void*, int, unsigned) {
    void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                       // 0053C8F0..F8
    return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, at<I>(party, 0))); // 0053C8FD..19
}

// ===================================================================================================== 00530960
// EffectACDecrease(int nValue, int nModifyType=AC_DODGE_BONUS, int nDamageType=AC_VS_DAMAGE_TYPE_ALL) [routine 450]
int __fastcall kotor_fastcall_00530960(void* self, void*, int, unsigned argc) {
    return ac_effect(self, argc, k_effect_ac_decrease_00746740);                        // 00530A24 type [00746740]
}

// ===================================================================================================== 00530680
// EffectAbilityDecrease(int nAbility, int nModifyBy) [routine 446]
int __fastcall kotor_fastcall_00530680(void* self, void*, int, unsigned) {
    I ability = 0, amount = 0;                                                          // locals +4 / +8
    if (!pop_int(&ability)) return k_pop_failed;                                        // 00530699..AD
    if (!pop_int(&amount)) return k_pop_failed;                                         // 005306AF..C1
    if (ability < 0 || ability > k_ability_max_0074684c) ability = k_ability_default_00746838; // 005306DA..F0
    void* e = new_effect();                                                             // 005306F4..1F
    set_type(e, k_effect_ability_decrease_00746710);                                    // 00530721..2D
    set_subtype_magical(e);                                                             // 00530727..42
    set_int(e, 0, static_cast<U>(ability));                                             // 00530746..4F
    set_int(e, 1, static_cast<U>(amount));                                              // 00530754..5D
    creator_if_object(self, e);                                                         // 00530762..96
    return push_effect(e);                                                              // 0053079B..E7
}

// ===================================================================================================== 005332E0
// EffectMovementSpeedDecrease(int nPercentChange) [routine 451].  argc must be exactly 1; the value is popped
// into the argc parameter slot itself ([esp+20h]).
int __fastcall kotor_fastcall_005332e0(void* self, void*, int, unsigned argc) {
    if (argc != 1) return k_pop_failed;                                                 // 005332EE..02 (jne)
    I percent = static_cast<I>(argc);
    if (!pop_int(&percent)) return k_pop_failed;                                        // 00533304..16
    void* e = new_effect();                                                             // 0053332F..5A
    set_type(e, k_effect_movement_speed_decrease_007466f0);                             // 0053335C..69
    set_subtype_magical(e);                                                             // 00533363..80
    set_int(e, 0, static_cast<U>(percent));                                             // 00533384..8B
    creator_if_object(self, e);                                                         // 00533390..C4
    return push_effect(e);                                                              // 005333C9..15
}

// ===================================================================================================== 00544E50
// RoundsToSeconds / HoursToSeconds / TurnsToSeconds(int n) [routines 121 / 122 / 123]
// Quirk (original): another routine id pushes the uninitialised float local (written as 0.0f here; the
// handler is only registered for 121..123).
int __fastcall kotor_fastcall_00544e50(void*, void*, int routine, unsigned) {
    I n = 0;                                                                            // 00544E5E
    float result = 0.0f;
    if (!pop_int(&n)) return k_pop_failed;                                              // 00544E59..77
    switch (routine) {                                                                  // 00544E7A..87
    case 121:                                                                           // 00544EBB fild n; fmul [00740C84]
        result = static_cast<float>(static_cast<double>(n) * static_cast<double>(k_seconds_per_round_00740c84));
        break;
    case 122: {                                                                         // 00544E95..B3
        const I minutes = at<U8>(kotor_fastcall_004aede0(server_app(), nullptr), 0x38);  // movzx [timer+38h]
        // fild minutes; fimul n (exact: |product| < 2^40); fmul 60.0f; fstp dword
        result = static_cast<float>((static_cast<double>(minutes) * static_cast<double>(n)) *
                                    static_cast<double>(k_seconds_per_minute_00748734));
        break;
    }
    case 123:                                                                           // 00544E89 fild n; fmul [007455DC]
        result = static_cast<float>(static_cast<double>(n) * static_cast<double>(k_seconds_per_turn_007455dc));
        break;
    default: break;                                                                     // 00544EC9 (local not written)
    }
    return push_status(recovered_vm_outer_push_float_005d1030(vm(), nullptr, result));  // 00544EC9..EA
}

// ===================================================================================================== 00530B20
// EffectACIncrease(int nValue, int nModifyType=AC_DODGE_BONUS, int nDamageType=AC_VS_DAMAGE_TYPE_ALL) [routine 115]
int __fastcall kotor_fastcall_00530b20(void* self, void*, int, unsigned argc) {
    return ac_effect(self, argc, k_effect_ac_increase_0074673c);                        // 00530BE4 type [0074673C]
}

// ===================================================================================================== 005321B0
// EffectDamageIncrease(int nBonus, int nDamageType=DAMAGE_TYPE_UNIVERSAL) [routine 120] (both always popped)
int __fastcall kotor_fastcall_005321b0(void* self, void*, int, unsigned) {
    return damage_bonus_effect(self, k_effect_damage_increase_007466b0);                // 00532266 type [007466B0]
}

// ===================================================================================================== 00532500
// EffectDamageResistance(int nDamageType, int nAmount, int nLimit=0) [routine 81] (all three always popped)
int __fastcall kotor_fastcall_00532500(void* self, void*, int, unsigned) {
    I damage = 0, amount = 0, limit = 0;                                                // locals +4 / +8 / +C
    if (!pop_int(&damage)) return k_pop_failed;                                         // 00532519..2D
    if (!pop_int(&amount)) return k_pop_failed;                                         // 0053252F..41
    if (!pop_int(&limit)) return k_pop_failed;                                          // 00532543..55
    if (damage < 0 || damage > k_damage_type_max_00746884) damage = k_damage_type_fallback_0074685c; // 0053256E..83
    void* e = new_effect();                                                             // 00532587..B2
    set_type(e, k_effect_damage_resistance_00746684);                                   // 005325B4..C1
    set_subtype_magical(e);                                                             // 005325BB..D8
    set_int(e, 0, static_cast<U>(damage));                                              // 005325DC..E3
    set_int(e, 1, static_cast<U>(amount));                                              // 005325E8..F1
    set_int(e, 2, static_cast<U>(limit));                                               // 005325F6..FF
    creator_if_object(self, e);                                                         // 00532604..37
    return push_effect(e);                                                              // 0053263C..88
}

// ===================================================================================================== 00534490
// EffectTemporaryHitpoints(int nHitPoints) [routine 314]
int __fastcall kotor_fastcall_00534490(void* self, void*, int, unsigned) {
    return temporary_points_effect(self, k_effect_temporary_hitpoints_007466b8);        // 00534526 type [007466B8]
}

// ===================================================================================================== 00532690
// EffectForceShield(int nShield) [routine 459]: creator set unconditionally, flags untouched
int __fastcall kotor_fastcall_00532690(void* self, void*, int, unsigned) {
    I shield = 0;                                                                       // 005326B6
    if (!pop_int(&shield)) return k_pop_failed;                                         // 005326A9..DB
    void* e = new_effect();                                                             // 005326DE..09
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 0053270B..19
    set_type(e, k_effect_force_shield_00746824);                                        // 0053271E..25
    set_int(e, 0, static_cast<U>(shield));                                              // 00532729..32
    return push_effect(e);                                                              // 00532737..83
}

// ===================================================================================================== 005345E0
// EffectTrueSeeing() [routine 465]
int __fastcall kotor_fastcall_005345e0(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                             // 005345F8..24
    set_type(e, k_effect_true_seeing_0074679c);                                         // 00534626..2C
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 00534630..3E
    set_subtype_magical(e);                                                             // 00534643..55
    return push_effect(e);                                                              // 00534649..A2
}

// ===================================================================================================== 00534C20
// EffectWhirlWind() [routine 703]
int __fastcall kotor_fastcall_00534c20(void*, void*, int, unsigned) { return choke_effect(0xa); } // 00534C69 push 0Ah

// ===================================================================================================== 00534340
// EffectTemporaryForcePoints(int nTempForce) [routine 156]
int __fastcall kotor_fastcall_00534340(void* self, void*, int, unsigned) {
    return temporary_points_effect(self, k_effect_temporary_force_points_007467e4);     // 005343D6 type [007467E4]
}

// ===================================================================================================== 00531D50
// EffectDamageDecrease(int nPenalty, int nDamageType=DAMAGE_TYPE_UNIVERSAL) [routine 448]
int __fastcall kotor_fastcall_00531d50(void* self, void*, int, unsigned) {
    return damage_bonus_effect(self, k_effect_damage_decrease_007466b4);                // 00531E06 type [007466B4]
}

// ===================================================================================================== 005319A0
// EffectConcealment(int nPercentage) [routine 458]
int __fastcall kotor_fastcall_005319a0(void* self, void*, int, unsigned) {
    I percent = 0;                                                                      // local +4 (written by the pop)
    if (!pop_int(&percent)) return k_pop_failed;                                        // 005319B9..E3
    void* e = new_effect();                                                             // 005319E6..11
    if (percent >= 1 && percent <= 0x64) {                                              // 00531A13..27 (jl / jg)
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                   // 00531A29..2F
        set_subtype_magical(e);                                                         // 00531A34..43
        set_type(e, k_effect_concealment_007467ac);                                     // 00531A47..4D
        set_int(e, 0, static_cast<U>(percent));                                         // 00531A51..5A
        set_int(e, 1, rules_aa());                                                      // 00531A5F..71
    } else {
        set_type(e, k_effect_invalid_0074667c);                                         // 00531A78..7F
    }
    return push_effect(e);                                                              // 00531A83..CF
}

// ===================================================================================================== 00533E70
// EffectSpellImmunity(int nImmunityToSpell=FORCE_POWER_ALL_FORCE_POWERS) [routine 149]
int __fastcall kotor_fastcall_00533e70(void* self, void*, int, unsigned) {
    I spell = 0;
    if (!pop_int(&spell)) return k_pop_failed;                                          // 00533E89..B3
    void* e = new_effect();                                                             // 00533EB6..E1
    // -1 <= spell <= *[[007A3A28]+8Ch] (spell count; signed jl / jg)
    if (spell >= -1 && spell <= at<I>(at<void*>(rules(), 0x8c), 0)) {                   // 00533EE3..02
        set_type(e, k_effect_spell_immunity_00746744);                                  // 00533F04..10
        set_subtype_magical(e);                                                         // 00533F0A..1D
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                   // 00533F21..27
        set_int(e, 0, static_cast<U>(spell));                                           // 00533F2C..35
    } else {
        set_type(e, k_effect_invalid_0074667c);                                         // 00533F3C..43
    }
    return push_effect(e);                                                              // 00533F47..93
}

// ===================================================================================================== 00534120
// EffectForceResistanceDecrease(int nValue) [routine 454]
int __fastcall kotor_fastcall_00534120(void* self, void*, int, unsigned) {
    I value = 0;
    if (!pop_int(&value)) return k_pop_failed;                                          // 00534139..63
    void* e = new_effect();                                                             // 00534166..91
    set_type(e, k_effect_force_resistance_decrease_00746704);                           // 00534193..9A
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 0053419E..AC
    set_int(e, 0, static_cast<U>(value));                                               // 005341B1..BA
    set_subtype_magical(e);                                                             // 005341BF..D1
    return push_effect(e);                                                              // 005341C5..1E
}

// ===================================================================================================== 005435C0
// MagicalEffect / SupernaturalEffect / ExtraordinaryEffect(effect e) [routines 112 / 113 / 114]
// Quirks (original): a pop failure returns -2000; after a successful push the popped effect is freed with
// free() only (no ~CGameEffect); a failed push leaks it.
int __fastcall kotor_fastcall_005435c0(void*, void*, int routine, unsigned) {
    void* e = nullptr;                                                                  // local (written by the pop)
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 0, &e)) return k_push_failed; // 005435C1..D5 -> 0054364B
    switch (routine) {                                                                  // 005435D7..E4
    case 112: set_subtype_magical(e); break;                                            // 00543609..1C
    case 113: at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff7u) | 0x10u); break; // 005435F0..03
    case 114: at<U8>(e, 0xa) = static_cast<U8>(at<U8>(e, 0xa) | 0x18u); break;          // 005435E6..EA (byte or)
    default: break;
    }
    if (at<U16>(e, 8) == k_effect_link_0074671c) kotor_fastcall_00504240(e, nullptr);   // 00543620..30
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 0, e)) return k_push_failed; // 00543639..49
    live_free(e);                                                                       // 00543654..5E
    return 0;
}

// ===================================================================================================== 00537B20
// GetEffectDurationType(effect e) [routine 89]
int __fastcall kotor_fastcall_00537b20(void*, void*, int, unsigned) {
    void* e = nullptr;                                                                  // 00537B2E
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 0, &e)) return k_pop_failed; // 00537B21..45
    if (!recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(at<U8>(e, 0xa) & 7u)))
        return k_push_failed;                                                           // 00537B48..6A (effect leaked)
    if (e != nullptr) {                                                                 // 00537B6D..76
        recovered_effect_destroy_00503590(e, nullptr);                                  // 00537B78
        live_free(e);                                                                   // 00537B7D..83
    }
    return 0;
}

// ===================================================================================================== 00535710
// EventActivateItem(object oItem, location lTarget, object oTarget=OBJECT_INVALID) [routine 424]
// Quirks (original): the event is allocated before the pops and leaked on a pop or push failure (the popped
// location is leaked too); argc is not consulted, all three values are always popped.
int __fastcall kotor_fastcall_00535710(void* self, void*, int, unsigned) {
    void* memory = live_operator_new(0x34);                                             // 0053572B..34
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 00535737..52
    U possessor = k_invalid;                                                            // 00535767 edi
    U item_id = 0, target = 0;                                                          // locals +10 / +14 (written by the pops)
    void* location = nullptr;                                                           // 0053576C local +C = 0
    if (!pop_object(&item_id)) return k_pop_failed;                                     // 00535754..7B
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 2, &location)) return k_pop_failed; // 00535781..95
    if (!pop_object(&target)) return k_pop_failed;                                      // 0053579B..AD
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);        // 005357B3..C0
    if (item != nullptr) possessor = at<U>(item, 0x9c);                                 // 005357C5..C9
    at<U16>(event, 0) = 0x12;                                                           // 005357CF
    recovered_event_object_004d7780(event, nullptr, 0, item_id);                        // 005357D4..DD
    recovered_event_object_004d7780(event, nullptr, 1, self_id(self));                  // 005357E2..EA
    recovered_event_object_004d7780(event, nullptr, 2, possessor);                      // 005357EF..F4
    recovered_event_object_004d7780(event, nullptr, 3, target);                         // 005357F9..02
    kotor_fastcall_004d7720(event, nullptr, 0, at<float>(location, 0));                 // 00535807..12
    kotor_fastcall_004d7720(event, nullptr, 1, at<float>(location, 4));                 // 00535817..23
    kotor_fastcall_004d7720(event, nullptr, 2, at<float>(location, 8));                 // 00535828..34
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 1, event)) return k_push_failed; // 00535839..61
    recovered_event_destroy_004d7590(event, nullptr);                                   // 00535864..66
    live_free(event);                                                                   // 0053586B..71
    if (location != nullptr) live_free(location);                                       // 00535878..89 (dtor 004015A0 is a bare ret)
    return 0;
}

// ===================================================================================================== 00548CA0
// TouchAttackMelee / TouchAttackRanged(object oTarget, int bDisplayFeedback=TRUE) [routines 146 / 147]
// Quirks (original): when either object is not a creature the feedback integers 0 and 1 carry the routine id;
// a natural 20 that misses still rolls the critical confirmation (result 1 or 2); a "not found" lookup also
// clears bDisplayFeedback; the found objects are not null-checked before their virtual calls.
int __fastcall kotor_fastcall_00548ca0(void* self, void*, int routine, unsigned argc) {
    U target_id = 0;                                                                    // local +1C (written by the pop)
    I display = 1;                                                                      // 00548CCD local +14
    U8 deflect_a = 0, deflect_b = 0;                                                    // 00548CD5 / 00548CDA locals +0A / +0B
    U result = 0;                                                                       // 00548CCB edi
    if (!pop_object(&target_id)) return k_pop_failed;                                   // 00548CBC..FD
    if (static_cast<I>(argc) >= 2 && !pop_int(&display)) return k_pop_failed;          // 00548D00..19 (jl)
    void* target = nullptr;
    void* self_object = nullptr;
    if (!lookup(target_id, &target) || !lookup(self_id(self), &self_object)) {          // 00548D1B..5F
        // 00548F3E: [esp+18h] (bDisplayFeedback) = 0, dead store before the push
        return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result))); // 0054900E
    }
    U value0, value1;                                                                   // ebx / ebp
    if (as_creature(self_object) == nullptr) {                                          // 00548D65..71
        value0 = value1 = static_cast<U>(routine);                                      // 00548EBC..C0
    } else if (as_creature(target) == nullptr) {                                        // 00548D77..82
        const U8 type = at<U8>(target, 8);                                              // 00548EA0..A4
        if (type == k_type_door_00746639 || type == k_type_placeable_00746638) result = 1; // 00548EA7..B7
        value0 = value1 = static_cast<U>(routine);                                      // 00548EBC..C0
    } else {
        I bonus;
        if (routine == 0x92)                                                            // 00548D88..94
            bonus = static_cast<I>(recovered_attack_melee_005a7770(stats_of(as_creature(self_object)), nullptr,
                                                                   0, 1, 1, 0));        // 00548D96..A9
        else
            bonus = static_cast<I>(recovered_attack_ranged_005a7b60(stats_of(as_creature(self_object)), nullptr,
                                                                    0, 1, 1));          // 00548DB0..C2
        void* target_stats = stats_of(as_creature(target));                             // 00548DC7..D6
        const I ac = static_cast<std::int16_t>(kotor_fastcall_005ad0e0(target_stats, nullptr, as_creature(self_object), 1)); // 00548DD2..F5 movsx
        const I roll = static_cast<U16>(recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, 0x14)); // 00548DEB..05 movzx
        void* target_sws = as_object(target);                                           // 00548DFD..08
        if (kotor_fastcall_005b8e50(as_creature(self_object), nullptr, target_sws, 1) != 0) { // 00548E0B..1E
            result = 0;                                                                 // 00548E26
            void* round = round_of(as_creature(self_object));                           // 00548E20..2B
            const U index = at<U8>(round, 0x96c);                                       // 00548E35
            void* attack = recovered_combat_round_attack_004d2b50(round_of(as_creature(self_object)), nullptr, index); // 00548E31..4A
            deflect_a = at<U8>(attack, 0x5c);                                           // 00548E4F / 00548E55
            deflect_b = at<U8>(attack, 0x3c);                                           // 00548E52 / 00548E59
        } else if (roll == 1) {                                                         // 00548E5F..62
            result = 0;                                                                 // 00548E70
        } else if (bonus + roll >= ac || roll == 0x14) {                                // 00548E64..6E (jge / je)
            if (roll == 0x14) {                                                         // 00548E74..77
                const I confirm = static_cast<U16>(recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, 0x14)); // 00548E79..88
                result = (confirm + bonus >= ac) ? 2u : 1u;                             // 00548E8B..95 (setge; inc)
            } else {
                result = 1;                                                             // 00548E99
            }
        } else {
            result = 0;                                                                 // 00548E70
        }
        value0 = static_cast<U>(bonus);                                                 // ebx
        value1 = static_cast<U>(roll);                                                  // ebp
    }
    if (display == 1) {                                                                 // 00548EC4..C9
        void* memory = live_operator_new(0x34);                                         // 00548ECF..D6
        void* data = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 00548EDD..EE
        recovered_event_object_004d7780(data, nullptr, 0, self_id(self));               // 00548EF0..04
        recovered_event_object_004d7780(data, nullptr, 1, at<U>(target, 4));            // 00548F09..15
        recovered_event_integer_004d76b0(data, nullptr, 0, value0);                     // 00548F1A..1F
        recovered_event_integer_004d76b0(data, nullptr, 1, value1);                     // 00548F24..29
        if (result == 0) {                                                              // 00548F2E..30
            if (deflect_a == 0) {                                                       // 00548F32..38
                recovered_event_integer_004d76b0(data, nullptr, 2, 4);                  // 00548F3A / 00548F6C..70
            } else {
                recovered_event_integer_004d76b0(data, nullptr, 2, deflect_a);          // 00548F47..4F
                recovered_event_integer_004d76b0(data, nullptr, 4, deflect_b);          // 00548F54..5C / 00548F6E..70
            }
        } else if (result == 1 || result == 2) {                                        // 00548F5E..6B
            recovered_event_integer_004d76b0(data, nullptr, 2, result);                 // 00548F6C..70
        }
        if (routine == 0x93) recovered_event_integer_004d76b0(data, nullptr, 3, 1);     // 00548F75..85
        void* client = kotor_fastcall_004ae910(server_app(), nullptr, self_id(self));   // 00548F8A..9A
        if (client != nullptr)                                                          // 00548F9F..BC
            recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                          at<U>(client, 4), 0xd, data, 0);
        client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(target, 4));      // 00548FC1..D2
        if (client != nullptr)                                                          // 00548FD7..F4
            recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                          at<U>(client, 4), 0xd, data, 0);
        if (data != nullptr) {                                                          // 00548FF9..FB
            recovered_message_data_destroy_004d6a40(data, nullptr);                     // 00548FFD..FF
            live_free(data);                                                            // 00549004..0A
        }
    }
    return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result))); // 0054900E..39
}

}  // extern "C"
