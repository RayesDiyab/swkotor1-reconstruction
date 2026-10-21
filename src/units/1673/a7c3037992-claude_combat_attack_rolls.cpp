// combat-attack lane, part D1 "rolls": attack roll / defensive / sneak / special-attack / saving-throw leaves of the
// AIActionPhysicalAttacks 005BBBF0 cascade.
//   005BACA0 CSWSCreature::ResolveAttackRoll            005B8820 CSWSCreature::ResolveSneakAttack
//   005B8E50 CSWSCreature::ResolveDefensiveEffects      005B78E0 CSWSCreature::IsProjectileParryable
//   005BA540 CSWSCreature::ResolveRangedSpecialAttack   005BA8E0 CSWSCreature::ResolveMeleeSpecialAttack
//   005B5510 CSWSCreature::FUN_005b5510                 005B92B0 CSWSCreature::SavingThrowRoll
//   004EC760 CSWSCreature::BroadcastSavingThrowData     005AB810 CSWSCreatureStats::GetFortSavingThrow
//   005AB880 CSWSCreatureStats::GetWillSavingThrow      005A6A90 CSWSCreatureStats::GetEffectLinkImmunity
//   005A5AA0 EffectTypeToGameEffect (static, stdcall)
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// Every callee binds to an existing selected provider of the c07b candidate map, to a sibling combat-attack part
// (kotor_fastcall_<addr>, see out/agent-combat-attack/PLAN.md) or is reconstructed below. No traps.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_sound_math { double __cdecl Magnitude004ab100(const float* v); }   // vector_magnitude 004AB100 [ST0]

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);             // GetAttack [W1 b7]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);              // RollDice (AX) [W1 b7]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                         // HasFeat [0963]
U __fastcall kt_feat_remaining_uses_005a6680(void* stats, void*, U feat);                           // GetFeatRemainingUses (AL)
unsigned char __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U arg);                 // GetLevel [0963]
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other);        // GetEffectImmunity [1094]
U __fastcall recovered_critical_threat_005a5130(void* stats, void*, U offhand);                     // GetCriticalHitRoll [1053]
unsigned __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);               // GetDeadTemp
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);          // GetVisibleListElement
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);                // GetClientCreature
U __fastcall kotor_animation_sound_damage_effect_00616890(void* client_creature, void*);            // CSWCCreature::GetForceShield
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                         // CSWItem::GetBaseItem [0820]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);            // CSWSpellArray::GetSpell
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                      // CSWCCMessageData()
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                         // ~CSWCCMessageData
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);              // SetInteger
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                  // CGameEffect(1) [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                       // SetCreator
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);           // CGameEffect::SetInteger
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                       // CGameEffect::GetInteger
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);        // ApplyEffect
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                             // ~CGameEffect
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                          // GetServerAIMaster
void* __cdecl combat_reuse_message_004aedb0();                                                      // GetSWSMessage
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
U __fastcall kotor_fastcall_004f3fe0(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7,
    U a8, U a9, U a10);                                                                             // GetTotalEffectBonus [effectbonus part; supersedes the partial W2 providers]
U __fastcall recovered_stats_base_fort_save_005aa1b0(void* stats, void*);                           // GetBaseFortSavingThrow (AL)
U __fastcall recovered_stats_base_will_save_005aa2f0(void* stats, void*);                           // GetBaseWillSavingThrow (AL)
U __fastcall recovered_stats_reflex_save_005ab8f0(void* stats, void*, U flag);                      // GetReflexSavingThrow (AL)
I __fastcall c2da_current_get_int_entry_00413510(void* table, void*, I row, I column, I* out);       // C2DA::GetINTEntry(int,int) [0390]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);                // CExoString(char*)

// ---------------- sibling combat-attack parts (PLAN.md partition) ----------------
U __fastcall kotor_fastcall_004d2ec0(void* threat, void*);                                         // [round] SubAttackThreat::Clear
U __fastcall kotor_fastcall_004d2a00(void* round, void*);                                          // [round] GetOffHandAttack
U __fastcall kotor_fastcall_004d3da0(void* round, void*);                                          // [round] GetWeaponAttackType
U __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* other);                              // [round] CSWCCMessageData::CopyTo
U __fastcall kotor_fastcall_005a7ea0(void* stats, void*, void* versus);                            // [attackstats] GetAttackModifierVersus
U __fastcall kotor_fastcall_005ad0e0(void* stats, void*, void* versus, U flag);                    // [attackstats] GetArmorClassVersus (AX)
U __fastcall kotor_fastcall_005a52b0(void* stats, void*, U offhand);                               // [attackstats] GetCriticalHitMultiplier
U __fastcall kotor_fastcall_005a60d0(void* stats, void*);                                          // [attackstats] GetBaseAttackBonus
U __fastcall kotor_fastcall_005b5690(void* creature, void*);                                       // [main] IsLightsaberEquipped

// ---------------- exports of this part ----------------
U __stdcall kotor_stdcall_005a5aa0(U type, U subtype);
U __fastcall kotor_fastcall_005a6a90(void* stats, void*, void* effect);
U __fastcall kotor_fastcall_005ab810(void* stats, void*, U flag);
U __fastcall kotor_fastcall_005ab880(void* stats, void*, U flag);
void __fastcall kotor_fastcall_004ec760(void* creature, void*, void* data);
U __fastcall kotor_fastcall_005b5510(void* creature, void*, U result);
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U type, U dc, U subtype, U versus_id, U a5, U a6, U store);
U __fastcall kotor_fastcall_005b78e0(void* creature, void*, void* target, U a2, U a3, U* force_shield, U hand);
void __fastcall kotor_fastcall_005b8820(void* creature, void*, void* target);
U __fastcall kotor_fastcall_005b8e50(void* creature, void*, void* target, U hit);
void __fastcall kotor_fastcall_005ba540(void* creature, void*, void* target, U flag);
void __fastcall kotor_fastcall_005ba8e0(void* creature, void*, U flag, U a2, void* target, U a4);
void __fastcall kotor_fastcall_005baca0(void* creature, void*, void* target, U preset, U preset_result);

// 00832874: .bss dword read only by ResolveAttackRoll (no writer in the image; loader zero) - roll override
volatile U w5_cell_attack_roll_override_00832874 = 0;
}

namespace {
// .rdata constants (literal proven values)
constexpr U k_effect_type_skip_0074671c = 0x28;    // .rdata 0074671C
constexpr U k_effect_type_skip_007466f4 = 0x1e;    // .rdata 007466F4
constexpr I k_immunity_count_00746978 = 0x21;      // .rdata 00746978
constexpr U k_imm_00746900 = 1, k_imm_00746904 = 2, k_imm_00746908 = 3, k_imm_0074690c = 4, k_imm_00746910 = 5,
            k_imm_00746914 = 6, k_imm_00746918 = 7, k_imm_0074691c = 8, k_imm_00746920 = 9, k_imm_00746924 = 0xa,
            k_imm_00746928 = 0xb, k_imm_0074692c = 0xc, k_imm_00746930 = 0xd, k_imm_00746934 = 0x10,
            k_imm_00746938 = 0x11, k_imm_0074693c = 0x12, k_imm_00746964 = 0x1c, k_imm_00746968 = 0x1d,
            k_imm_0074696c = 0x1e, k_imm_00746970 = 0x1f, k_imm_00746974 = 0x20;
constexpr U k_effect_type_5c_007467e8 = 0x5c;      // .rdata 007467E8
constexpr U k_effect_type_5d_007467ec = 0x5d;      // .rdata 007467EC
constexpr U16 k_effect_id_00746740 = 0x31;         // .rdata 00746740 (word)
constexpr U16 k_effect_id_0074669c = 0x08;         // .rdata 0074669C (word)
constexpr U16 k_subtype_007473c0 = 0x1;            // .rdata 007473C0 (word)
constexpr U k_int0_007468b0 = 0;                   // .rdata 007468B0
constexpr U k_int5_0074688c = 0x4007;              // .rdata 0074688C
constexpr U k_float_3_0 = 0x40400000u;             // 3.0 immediate
constexpr U k_float_6_0 = 0x40c00000u;             // 6.0 immediate
const float k_sneak_range_007455c8 = 10.0f;        // .rdata 007455C8
const double k_behind_007469b8 = 0.707;            // .rdata 007469B8 (qword)
const float k_zero_0073d700 = 0.0f;                // .rdata 0073D700
const char k_shieldhit_0074b2f0[] = "ShieldHit";   // .rdata 0074B2F0

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }          // CSWSCreature +9C8 CSWSCombatRound*
inline void* stats_of(void* c) { return at<void*>(c, 0xa74); }          // CSWSCreature +A74 CSWSCreatureStats*
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* current_attack(void* round) {                              // GetAttack(round, round+96C byte)
    return recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c));
}
inline U d20() { return recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, 0x14) & 0xffffu; } // RollDice(1,20) movzx ax
inline void list_add(void* list, void* value) {                         // CExoArrayList::Add 00671C00
    gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, addr(value));
}
inline void* new_message_data() {                                       // operator new(34h) + CSWCCMessageData()
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
inline void* new_effect() {                                             // operator new(8Ch) + CGameEffect(1)
    void* memory = live_operator_new(0x8c);
    return memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void delete_effect(void* effect) {                               // ~CGameEffect + _free
    recovered_effect_destroy_00503590(effect, nullptr);
    live_free(effect);
}
// the shared "special attack" self effect (005BA5E9..95 / 005BA69A..748 / 005BA971..A19 / 005BAB77..C20):
// id 31h, subtype bits = 1, +C = 3.0, creator = self, ints {0: 0, 1: value, 2: rules+AA, 5: 4007h}, applied to self
void apply_special_effect(void* self, U value) {
    void* e = new_effect();
    at<U16>(e, 8) = k_effect_id_00746740;
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_subtype_007473c0);
    at<U>(e, 0xc) = k_float_3_0;
    recovered_effect_creator_00503a00(e, nullptr, at<U>(self, 4));
    recovered_effect_integer_005036a0(e, nullptr, 0, k_int0_007468b0);
    recovered_effect_integer_005036a0(e, nullptr, 1, value);
    recovered_effect_integer_005036a0(e, nullptr, 2, at<U8>(rules(), 0xaa));
    recovered_effect_integer_005036a0(e, nullptr, 5, k_int5_0074688c);
    recovered_effect_apply_004d14f0(self, nullptr, e, 0, 0);
}
// the shared "stun" effect on the target (005BA772..85B / 005BAA42..B4E): id 8 with int0 = 4, +C = 6.0, creator =
// self; DC = level(0) + signed stats byte (+F0 ranged / +EA melee); dropped when the target is link-immune or saves
// (Fortitude, type 1); otherwise appended to the attack's +88 effect list with +12D = 4. +13C = 1 either way.
void apply_stun(void* self, void* target_object, void* attack, U stats_byte_offset) {
    void* e = new_effect();
    at<U16>(e, 8) = k_effect_id_0074669c;
    recovered_effect_integer_005036a0(e, nullptr, 0, 4);
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_subtype_007473c0);
    at<U>(e, 0xc) = k_float_6_0;
    recovered_effect_creator_00503a00(e, nullptr, at<U>(self, 4));
    // ranged order: byte, level, AsSWSCreature; melee order: AsSWSCreature, byte, level (same values)
    void* target = nullptr;
    if (stats_byte_offset == 0xea) target = as_creature(target_object);            // 005BAAB5..BA
    const signed char bonus = static_cast<signed char>(at<U8>(stats_of(self), stats_byte_offset)); // 005BA7E6 / 005BAAC5
    const U dc = static_cast<U>(recovered_stats_level_005a5fd0(stats_of(self), nullptr, 0)) + static_cast<U>(static_cast<I>(bonus));
    if (stats_byte_offset != 0xea) target = as_creature(target_object);            // 005BA801
    if (kotor_fastcall_005a6a90(stats_of(target), nullptr, e) == 0 &&              // 005BA80D / 005BAAE7
        static_cast<U8>(kotor_fastcall_005b92b0(target, nullptr, 1, dc, 0, at<U>(self, 4), 1, 0, 1)) == 0) {
        list_add(static_cast<unsigned char*>(attack) + 0x88, e);                    // 005BA82E..39 / 005BAB08..13
        at<U8>(attack, 0x12d) = 4;
    } else {
        delete_effect(e);                                                           // 005BA847..54 / 005BAB30..3D
    }
    at<U>(attack, 0x13c) = 1;                                                       // 005BA857..5B / 005BAB21 / 005BAB44
}
}  // namespace

extern "C" {
// =========================================================================================================
// 005A5AA0 EffectTypeToGameEffect(type, subtype): static __stdcall, RET 8 (jump table 005A5BB4, index bytes 005A5C04
// for type 8..64h). Maps an effect type to its immunity-table row (0 = none).
U __stdcall kotor_stdcall_005a5aa0(U type, U subtype) {
    switch (type) {                                                                 // 005A5AA0..B7
    case 0x08:                                                                      // 005A5B36: by subtype
        if (subtype == 8) return 0xd;
        if (subtype == 7) return 0xf;
        if (subtype == 5) return 0x12;
        if (subtype == 4) return 0x13;
        if (subtype == 3) return 0x15;
        if (subtype == 1) return 0x16;
        if (subtype == 6) return 0x17;
        if (subtype == 2) return 0x18;
        return 0;                                                                   // 005A5BAE (EAX = 0)
    case 0x12: return 1;   case 0x23: return 2;   case 0x25: return 3;   case 0x0b: return 4;
    case 0x0e: return 5;   case 0x11: return 6;   case 0x31: return 7;   case 0x1d: return 8;
    case 0x1b: return 9;   case 0x22: return 0xa; case 0x38: return 0xb; case 0x5d: return 0xc;
    case 0x5e: return 0xd; case 0x5f: return 0xe; case 0x61: return 0xf; case 0x63: return 0x10;
    case 0x64: return 0x11; case 0x13: return 0x14;
    default: return 0;                                                              // 005A5BAC
    }
}

// ---------------------------------------------------------------------------------------------------------
// 005A6A90 CSWSCreatureStats::GetEffectLinkImmunity(CGameEffect*): RET 4. Immunity of the stats' creature to an
// effect or to any effect linked to it (+28 / +2C); on a direct immunity a feedback message is sent to the owner
// (and a copy to the effect's creator).
U __fastcall kotor_fastcall_005a6a90(void* stats, void*, void* effect) {
    U result = 0;                                                                   // [esp+10] (005A6AC5)
    const U16 type = at<U16>(effect, 8);                                            // 005A6AB0
    I index = 0;                                                                    // ESI
    bool immune = false;
    if (type != k_effect_type_skip_0074671c && type != k_effect_type_skip_007466f4) { // 005A6ABC..D9
        const U sub = recovered_effect_integer_00503690(effect, nullptr, 0);        // 005A6ADB..DF
        const U row = kotor_stdcall_005a5aa0(type, sub);                            // 005A6AE4..EB
        if (row != 0) {                                                             // 005A6AF2..F4
            for (index = 0; index < k_immunity_count_00746978; ) {                  // 005A6AF6..B46
                I value;   // [esp+18]: quirk - never initialised by the original; GetINTEntry writes it for every
                           // in-range row/column (the only reads here)
                c2da_current_get_int_entry_00413510(at<void*>(at<void*>(rules(), 0xb8), 0xdc), nullptr,
                                                    static_cast<I>(row), index + 1, &value); // 005A6B01..1E
                if (value != 0) {                                                   // 005A6B23..29
                    result = recovered_stats_effect_query_005a6960(stats, nullptr, static_cast<U>(index), nullptr); // 005A6B2B..30
                    if (result != 0) { immune = true; break; }                      // 005A6B35..3B
                }
                index = index + 1;                                                  // 005A6B42
            }
        }
    }
    if (!immune) {
        void* link1 = at<void*>(effect, 0x28);                                      // 005A6B4C
        if (link1 != nullptr) result = kotor_fastcall_005a6a90(stats, nullptr, link1); // 005A6B53..5B
        void* link2 = at<void*>(effect, 0x2c);                                      // 005A6B5F
        if (link2 != nullptr && result == 0) result = kotor_fastcall_005a6a90(stats, nullptr, link2); // 005A6B62..76
        return result;                                                              // 005A6B7A
    }
    U message;                                                                      // EDI (005A6B93..CCB)
    const U i = static_cast<U>(index);
    if (i == k_imm_00746900) message = 0x84;
    else if (i == k_imm_00746904) message = 0x85;
    else if (i == k_imm_00746908) message = 0x7d;
    else if (i == k_imm_0074690c) message = 0x80;
    else if (i == k_imm_00746914) message = 0x82;
    else if (i == k_imm_00746918) message = 0x8b;
    else if (i == k_imm_0074691c) message = 0x8c;
    else if (i == k_imm_00746920) message = 0x92;
    else if (i == k_imm_00746924) message = 0x90;
    else if (i == k_imm_00746928) message = 0x91;
    else if (i == k_imm_0074692c) message = 0x8a;
    else if (i == k_imm_00746930) message = 0x87;
    else if (i == k_imm_00746934) message = 0x89;
    else if (i == k_imm_00746938) message = 0x8d;
    else if (i == k_imm_0074693c) message = 0x88;
    else if (i == k_imm_00746964) message = 0x81;
    else if (i == k_imm_00746968) message = 0x83;
    else if (i == k_imm_00746970) message = 0x7e;
    else if (i == k_imm_00746974) message = 0x7f;
    else return result;                                                             // 005A6CC5 -> 005A6B7A (no link check)
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18)); // 005A6CD0..E1 (EBP)
    void* data = new_message_data();                                                // 005A6CE6..09 (ESI)
    void* owner = at<void*>(stats, 0x24);                                           // CSWSCreatureStats +24 creature
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(owner, 4));             // 005A6D0D..23
    if (creator != nullptr) {                                                       // 005A6D28..2A
        void* copy = new_message_data();                                            // 005A6D2C..4D (EBX, 0 on failure)
        kotor_fastcall_004d6ee0(data, nullptr, copy);                               // 005A6D4F..5A
        recovered_creature_feedback_004ede10(creator, nullptr, message, copy);      // 005A6D5F..63
    }
    recovered_creature_feedback_004ede10(owner, nullptr, message, data);            // 005A6D68..71
    return result;                                                                  // 005A6D76 -> 005A6B7A
}

// ---------------------------------------------------------------------------------------------------------
// 005AB810 / 005AB880 CSWSCreatureStats::GetFortSavingThrow / GetWillSavingThrow(flag): RET 4, AL used by callers.
// flag 0: effect bonus GetTotalEffectBonus(3, 0, &slot, 0, 0, save, 0, FF, FF, 0) is added.
// Quirk: the argument slot is reused - its low byte receives the misc bonus (+1A0 / +1A1) and the whole dword is
// added, so a nonzero flag's upper three bytes are part of the sum (callers pass 1).
U __fastcall kotor_fastcall_005ab810(void* stats, void*, U flag) {
    U slot = flag;                                                                  // [esp+10] = the argument
    U effect = 0;                                                                   // EDI
    if (flag == 0) {                                                                // 005AB817..1D
        slot = 0;                                                                   // 005AB83A
        effect = kotor_fastcall_004f3fe0(at<void*>(stats, 0x24), nullptr, 3, 0, addr(&slot), 0, 0, 1, 0,
                                                      0xff, 0xff, 0);              // 005AB81F..43
    }
    const U8 misc = at<U8>(stats, 0x1a0);                                           // 005AB845
    const U8 base_bonus = at<U8>(stats, 0xee);                                      // 005AB84B (BL)
    slot = (slot & 0xffffff00u) | misc;                                             // 005AB851
    const U base = recovered_stats_base_fort_save_005aa1b0(stats, nullptr) & 0xffu; // 005AB855..62
    return static_cast<U>(base_bonus) + (base + effect + slot);                     // 005AB864..6E
}

U __fastcall kotor_fastcall_005ab880(void* stats, void*, U flag) {
    U slot = flag;
    U effect = 0;
    if (flag == 0) {                                                                // 005AB887..8D
        slot = 0;                                                                   // 005AB8AA
        effect = kotor_fastcall_004f3fe0(at<void*>(stats, 0x24), nullptr, 3, 0, addr(&slot), 0, 0, 3, 0,
                                                      0xff, 0xff, 0);              // 005AB88F..B3
    }
    const U8 misc = at<U8>(stats, 0x1a1);                                           // 005AB8B5
    const U8 base_bonus = at<U8>(stats, 0xf2);                                      // 005AB8BB
    slot = (slot & 0xffffff00u) | misc;                                             // 005AB8C1
    const U base = recovered_stats_base_will_save_005aa2f0(stats, nullptr) & 0xffu; // 005AB8C5..D2
    return static_cast<U>(base_bonus) + (base + effect + slot);                     // 005AB8D4..DE
}

// ---------------------------------------------------------------------------------------------------------
// 004EC760 CSWSCreature::BroadcastSavingThrowData(CSWCCMessageData*): RET 4. CC message 7 to the creature's player.
void __fastcall kotor_fastcall_004ec760(void* creature, void*, void* data) {
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)); // 004EC760..6D
    if (client == nullptr) return;                                                  // 004EC772..74
    const U player = at<U>(client, 4);                                              // 004EC783
    recovered_cc_message_00568570(combat_reuse_message_004aedb0(), nullptr, player, 7, data, 0); // 004EC776..93
}

// ---------------------------------------------------------------------------------------------------------
// 005B5510 CSWSCreature::FUN_005b5510(result): RET 4. -1: +558 == 0; otherwise 1 <= result <= 3 (signed).
U __fastcall kotor_fastcall_005b5510(void* creature, void*, U result) {
    if (result == 0xffffffffu) return at<U>(creature, 0x558) == 0 ? 1u : 0u;      // 005B5516..2A
    const I r = static_cast<I>(result);
    return (r > 0 && r <= 3) ? 1u : 0u;                                             // 005B552D..3B (jle / jg)
}

// ---------------------------------------------------------------------------------------------------------
// 005B92B0 CSWSCreature::SavingThrowRoll(type, dc, subtype, versus id, a5, a6, store): RET 1Ch, AL 0 fail /
// 1 saved / 2 immune. type 1 Fortitude, 2 Reflex, 3 Will (others: 0 at once). The CC message data (0..6 = type,
// subtype, roll, bonus, save, dc, self) is broadcast and freed (store == 0) or appended to the versus creature's
// current attack +A0 list (store != 0; the versus creature is dereferenced unchecked, as in the original).
// a5 / a6 are not read.
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U type, U dc, U subtype, U versus_id, U, U, U store) {
    void* server = server_app();                                                    // [esp+20] (005B92C8..E3)
    const U type8 = type & 0xff;                                                    // [esp+1C]
    I save;                                                                         // [esp+0C]
    if (type8 == 1) save = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005ab810(stats_of(creature), nullptr, 1))); // 005B9335..45
    else if (type8 == 2) save = static_cast<signed char>(static_cast<U8>(recovered_stats_reflex_save_005ab8f0(stats_of(creature), nullptr, 1))); // 005B931F..2F
    else if (type8 == 3) save = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005ab880(stats_of(creature), nullptr, 1))); // 005B9309..19
    else return 0;                                                                  // 005B92F3..306
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, versus_id);          // 005B9349..58
    U slot = 0;                                                                     // [esp+34] (005B937B)
    void* versus = nullptr;                                                         // EBP / [esp+18]
    I bonus;                                                                        // EBX
    if (obj != nullptr) {                                                           // 005B9373..84
        void* o = as_object(obj);                                                   // 005B9386..8A
        bonus = static_cast<I>(kotor_fastcall_004f3fe0(creature, nullptr, 3, addr(o), addr(&slot), 0, 0,
                                                                    type, subtype, 0xff, 0xff, 0)); // 005B938D..92
        if (as_creature(obj) != nullptr) versus = as_creature(obj);                 // 005B9397..AF
    } else {
        bonus = static_cast<I>(kotor_fastcall_004f3fe0(creature, nullptr, 3, 0, addr(&slot), 0, 0,
                                                                    type, subtype, 0xff, 0xff, 0)); // 005B93B3..BE
    }
    if (bonus > 0x14) bonus = 0x14;                                                 // 005B93C0..C5 (jle)
    const U roll = d20();                                                           // 005B93CA..D9 -> [esp+14], [esp+28]
    void* data = new_message_data();                                                // 005B93DC..403 (ESI)
    at<U8>(data, 0) = 1;                                                            // 005B9416
    recovered_event_integer_004d76b0(data, nullptr, 0, type8);                      // 005B9405..19
    recovered_event_integer_004d76b0(data, nullptr, 1, subtype & 0xff);             // 005B941E..28
    recovered_event_integer_004d76b0(data, nullptr, 2, roll);                       // 005B942D..36
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(bonus));      // 005B943B..40
    recovered_event_integer_004d76b0(data, nullptr, 4, static_cast<U>(save));       // 005B9445..4E
    recovered_event_integer_004d76b0(data, nullptr, 5, dc & 0xffff);                // 005B9453..5D
    recovered_event_integer_004d76b0(data, nullptr, 6, at<U>(creature, 4));         // 005B9462..6A
    if (store == 0) {                                                               // 005B946F..75
        if (kotor_fastcall_004ae910(server, nullptr, at<U>(creature, 4)) != nullptr) // 005B9477..86
            kotor_fastcall_004ec760(creature, nullptr, data);                       // 005B9488..8B
        if (at<U>(creature, 4) != versus_id && versus != nullptr)                   // 005B9490..9B
            kotor_fastcall_004ec760(versus, nullptr, data);                         // 005B949D..A0
        recovered_message_data_destroy_004d6a40(data, nullptr);                     // 005B94A5..A7
        live_free(data);                                                            // 005B94AC..B2
    } else {
        // 005B94B7..503: inline CExoArrayList<CSWCCMessageData*>::Add on the versus creature's attack +A0
        unsigned char* list = static_cast<unsigned char*>(current_attack(round_of(versus))) + 0xa0;
        const U alloc = at<U>(list, 8);
        if (at<U>(list, 4) == alloc)                                                // 005B94CC..E0
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                                alloc != 0 ? alloc + alloc : 0x10); // 005B94E2..F2
        at<void**>(list, 0)[at<U>(list, 4)] = data;                                 // 005B94F7..FD
        at<U>(list, 4) += 1;                                                        // 005B9500
    }
    const I total = static_cast<I>(roll) + (bonus + save);                          // 005B9507..18 -> [esp+14]
    void* attack = nullptr;                                                         // ESI
    U8 sub = static_cast<U8>(subtype);                                              // AL
    if (versus != nullptr) {                                                        // 005B9516..1C
        attack = current_attack(round_of(versus));                                  // 005B951E..39
        at<U8>(attack, 0x12c) = static_cast<U8>(type);                              // 005B953B..3F
        at<U8>(attack, 0x12f) = static_cast<U8>(roll);                              // 005B9549
        at<U8>(attack, 0x131) = static_cast<U8>(save);                              // 005B9553
        at<U8>(attack, 0x12e) = sub;                                                // 005B955D
        at<U8>(attack, 0x130) = static_cast<U8>(bonus);                             // 005B9563
        at<U8>(attack, 0x132) = static_cast<U8>(total);                             // 005B9569
        at<U8>(attack, 0x133) = static_cast<U8>(dc);                                // 005B956F
    }
    if (total >= static_cast<I>(dc & 0xffff)) {                                     // 005B957B..84 (jl: signed)
        if (attack != nullptr) at<U8>(attack, 0x134) = 1;                           // 005B9586..8A
        return 1;                                                                   // 005B9591
    }
    U immunity;
    bool test = true;
    if (sub == 0x0a) immunity = k_imm_00746900;                                     // 005B9598..9E
    else if (sub == 0x0c) immunity = k_imm_00746904;                                // 005B95CC..D2
    else if (sub == 5) immunity = k_imm_00746908;                                   // 005B95DC..E2
    else if (sub == 8) immunity = k_imm_0074690c;                                   // 005B95EC..F2
    else if (sub == 4) {                                                            // 005B95F9..636
        immunity = k_imm_00746974;
        const U spell_id = at<U>(creature, 0x14c);
        if (spell_id == 0xffffffffu) test = false;
        else {
            void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(rules(), 0x8c), nullptr, static_cast<int>(spell_id));
            if (spell == nullptr || (at<U8>(spell, 0x140) != 1 && at<U8>(spell, 0x140) != 2)) test = false;
        }
    } else if (sub == 0x0e) immunity = k_imm_00746910;                              // 005B963B..41
    else { immunity = 0; test = false; }
    if (test && recovered_stats_effect_query_005a6960(stats_of(creature), nullptr, immunity & 0xff, versus) != 0) { // 005B95A3..B4
        if (attack != nullptr) at<U8>(attack, 0x134) = 2;                           // 005B95BA..BE
        return 2;                                                                   // 005B95C5
    }
    if (attack != nullptr) at<U8>(attack, 0x134) = 0;                               // 005B964B..4F
    return 0;                                                                       // 005B9656
}

// ---------------------------------------------------------------------------------------------------------
// 005B78E0 CSWSCreature::IsProjectileParryable(target, a2, a3, int* force_shield, hand): RET 14h, EAX 0/1.
// Only for a ranged current attack against a creature. A deflecting creature (feat 37h / 1 / 18h, alive, +8ED = 0)
// with a client creature, a lightsaber and +A04 = 0 parries when its client facing (+30) . attacker orientation
// (+9C) <= 0. Otherwise (a2 and a3 nonzero, a force shield up) the attacker's weapon (hand 0: slot 10h, 1: 20h,
// else slot 0) is looked up in the "ShieldHit" column; *force_shield receives the shield.
// Quirk: GetINTEntry's output is the argument slot of `target` (pre-filled with the target pointer), so a failed
// lookup counts as parryable.
U __fastcall kotor_fastcall_005b78e0(void* creature, void*, void* target, U a2, U a3, U* force_shield, U hand) {
    U result = 0;                                                                   // [esp+0C] (005B790B)
    void* attack = current_attack(round_of(creature));                              // 005B78FD..13 (EBX)
    if (as_creature(target) == nullptr) return result;                              // 005B7918..27
    if (at<U>(attack, 0x40) == 0) return result;                                    // 005B792D..32
    void* t = as_creature(target);                                                  // 005B7938..3F (ESI)
    const bool deflects = recovered_stats_has_feat_005a6630(stats_of(t), nullptr, 0x37) != 0 ||
                          recovered_stats_has_feat_005a6630(stats_of(t), nullptr, 1) != 0 ||
                          recovered_stats_has_feat_005a6630(stats_of(t), nullptr, 0x18) != 0; // 005B7941..72
    if (deflects && at<U8>(t, 0x8ed) == 0 && recovered_creature_incapacitated_004ef890(t, nullptr) == 0) { // 005B7978..8F
        if (recovered_creature_client_creature_004efb20(t, nullptr) == nullptr) return result; // 005B7995..9E
        if (kotor_fastcall_005b5690(t, nullptr) == 0) return result;                // 005B79A4..AD
        if (at<U>(t, 0xa04) != 0) return result;                                    // 005B79B3..BB
        unsigned char* facing = static_cast<unsigned char*>(recovered_creature_client_creature_004efb20(t, nullptr)) + 0x30; // 005B79C1..C8
        const float fx = at<float>(facing, 0), fy = at<float>(facing, 4), fz = at<float>(facing, 8); // [esp+10..18]
        const double dot = (static_cast<double>(fy) * at<float>(creature, 0xa0) + static_cast<double>(fz) * at<float>(creature, 0xa4)) +
                           static_cast<double>(fx) * at<float>(creature, 0x9c);     // 005B79D7..FF
        if (!(dot <= static_cast<double>(k_zero_0073d700))) return result;          // 005B7A01..0C (test ah,41h; jp)
        return 1;                                                                   // 005B7A12
    }
    if (a3 == 0) return result;                                                     // 005B7A1C..22
    if (a2 == 0) return result;                                                     // 005B7A28..2E
    if (recovered_creature_client_creature_004efb20(t, nullptr) == nullptr) return result; // 005B7A34..3D
    if (kotor_animation_sound_damage_effect_00616890(recovered_creature_client_creature_004efb20(t, nullptr), nullptr) == 0)
        return result;                                                              // 005B7A43..53
    const U slot = hand == 0 ? 0x10u : (hand == 1 ? 0x20u : 0u);                    // 005B7A59..6F
    void* item = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, slot); // 005B7A74..7D
    U parry = result;
    if (item != nullptr) {                                                          // 005B7A82..84
        const U8 base_item = at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x8e); // 005B7A86..8D
        alignas(4) unsigned char column[8];                                         // CExoString [esp+10]
        frame_LogStringCtor_005e5a90(column, nullptr, k_shieldhit_0074b2f0);        // 005B7A93..A4
        I value = 0;                                                                // 005B7A9C zeroes the output slot before GetINTEntry (review F4)
        c2da_current_get_int_00413660(at<void*>(at<void*>(rules(), 0xb8), 0x64), nullptr, base_item, column, &value); // 005B7AA9..D0
        frame_LogStringDtor_005e5c20(column, nullptr);                              // 005B7AD5..E1
        parry = value != 0 ? 1u : result;                                           // 005B7AE6..F3
    }
    *force_shield = kotor_animation_sound_damage_effect_00616890(recovered_creature_client_creature_004efb20(t, nullptr), nullptr); // 005B7AF7..09
    return parry;                                                                   // 005B7B11
}

// ---------------------------------------------------------------------------------------------------------
// 005B8820 CSWSCreature::ResolveSneakAttack(CSWSCreature* target): RET 4. Needs one of the sneak-attack feats
// 3Ch..45h. Sneak attack (+44 = 1) when the target is flat-footed (dead-temp / +8ED / not seeing the attacker, a
// ranged attack also within 10 m) or the attacker is behind it (normalised (self - target) . target orientation
// < 0.707, ranged also within 10 m); an immune target (immunity 1Eh / 1Fh against the attacker) gets CC message
// data 86h on the attack's +A0 list instead.
void __fastcall kotor_fastcall_005b8820(void* creature, void*, void* target) {
    if (target == nullptr) return;                                                  // 005B883A..42
    void* stats = stats_of(creature);                                               // 005B884F (EBX)
    const U index = at<U8>(round_of(creature), 0x96c);                              // 005B8856 (EBP)
    const U ranged = at<U>(recovered_combat_round_attack_004d2b50(round_of(creature), nullptr, index), 0x40); // 005B885D..6A
    bool feat = false;
    for (U f = 0x3c; f <= 0x45 && !feat; ++f)                                       // 005B8866..EA (stops at the first)
        feat = recovered_stats_has_feat_005a6630(stats, nullptr, f) != 0;
    if (!feat) return;                                                              // 005B88EA -> 005B8A1A
    float distance = 0.0f;                                                          // [esp+50]: written only when ranged
    if (ranged != 0) {                                                              // 005B88F0..F6
        float v[3];                                                                 // [esp+10..18]
        v[0] = static_cast<float>(static_cast<double>(at<float>(creature, 0x90)) - at<float>(target, 0x90));
        v[1] = static_cast<float>(static_cast<double>(at<float>(creature, 0x94)) - at<float>(target, 0x94));
        v[2] = static_cast<float>(static_cast<double>(at<float>(creature, 0x98)) - at<float>(target, 0x98)); // 005B88F8..29
        distance = static_cast<float>(kotor_sound_math::Magnitude004ab100(v));     // 005B892D..32
    }
    bool behind_test;
    if (at<U8>(target, 0x8ed) != 0 || recovered_creature_incapacitated_004ef890(target, nullptr) != 0) { // 005B8939..4C
        behind_test = false;
    } else {
        void* element = recovered_creature_visible_element_00517a20(target, nullptr, at<U>(creature, 4)); // 005B894E..54
        if (element != nullptr && (at<U8>(element, 4) & 1) != 0) behind_test = true; // 005B8959..61
        else if (ranged == 1) behind_test = !(static_cast<double>(distance) <= k_sneak_range_007455c8); // 005B8967..7F
        else behind_test = ranged != 0;                                             // 005B8A2F..31
    }
    if (behind_test) {                                                              // 005B8A37
        Vector d;                                                                   // [esp+20..28]
        d.x = static_cast<float>(static_cast<double>(at<float>(creature, 0x90)) - at<float>(target, 0x90));
        d.y = static_cast<float>(static_cast<double>(at<float>(creature, 0x94)) - at<float>(target, 0x94));
        d.z = static_cast<float>(static_cast<double>(at<float>(creature, 0x98)) - at<float>(target, 0x98)); // 005B8A37..6D
        Vector n1;                                                                  // [esp+2C] -> copied to [esp+10..18]
        const Vector* r1 = normalize_004ab180(&n1, &d);                             // 005B8A71
        const Vector a = *r1;                                                       // 005B8A76..92
        Vector n2;                                                                  // [esp+3C]
        const Vector* r2 = normalize_004ab180(&n2, static_cast<const Vector*>(static_cast<void*>(
                                                       static_cast<unsigned char*>(target) + 0x9c))); // 005B8A82..96
        const double dot = (static_cast<double>(a.z) * r2->z + static_cast<double>(a.y) * r2->y) +
                           static_cast<double>(a.x) * r2->x;                        // 005B8A9B..B4
        if (!(dot < k_behind_007469b8)) return;                                     // 005B8AB6..C1 (test ah,5; jp)
        if (ranged != 0 && !(static_cast<double>(distance) <= k_sneak_range_007455c8)) return; // 005B8AC7..DA
    } else {
        if (recovered_stats_effect_query_005a6960(stats_of(target), nullptr, k_imm_0074696c, creature) != 0 || // 005B8985..9C
            recovered_stats_effect_query_005a6960(stats_of(target), nullptr, k_imm_00746970, creature) != 0) { // 005B899E..B4
            void* data = new_message_data();                                        // 005B89BA..D9 (EBX)
            recovered_event_object_004d7780(data, nullptr, 0, at<U>(target, 4));    // 005B89DB..EB
            recovered_event_integer_004d76b0(data, nullptr, 0, 0x86);               // 005B89F0..F9
            list_add(static_cast<unsigned char*>(recovered_combat_round_attack_004d2b50(round_of(creature), nullptr, index)) + 0xa0,
                     data);                                                         // 005B89FE..15
            return;
        }
    }
    at<U>(recovered_combat_round_attack_004d2b50(round_of(creature), nullptr, index), 0x44) = 1; // 005B8AE0..EC
}

// ---------------------------------------------------------------------------------------------------------
// 005B8E50 CSWSCreature::ResolveDefensiveEffects(target, hit): RET 8, EAX 1 when the projectile is deflected
// (attack result +5C/+145 = 8, or 9 when deflected back / force-shielded) else 0. Deflection check: d20 + feat
// (18h: +6, 1: +3) + BAB + stats(+16C + +16D) + effect 5Ch/5Dh int1 sum against the attack roll (+C1 + +C2).
U __fastcall kotor_fastcall_005b8e50(void* creature, void*, void* target, U hit) {
    void* attack = current_attack(round_of(creature));                              // 005B8E55..66 (EDI)
    U slot = addr(target);                                                          // [esp+14]: the target argument slot
    if (kotor_fastcall_005b78e0(creature, nullptr, target, hit, 0, &slot, 0xffffffffu) == 0) return 0; // 005B8E6B..88
    if (as_creature(target) == nullptr) return 0;                                   // 005B8E8E..97
    void* t = as_creature(target);                                                  // 005B8E9D..A4 (ESI)
    if (at<U>(t, 0x8dc) != 0) {                                                     // 005B8EA6..AE
        const U8 r = at<U>(t, 0x8d8) != 0 ? 9 : 8;                                  // 005B8EB0..BD
        at<U8>(attack, 0x5c) = r; at<U8>(attack, 0x145) = r;
        return 1;
    }
    U roll = d20();                                                                 // 005B8ED5..E4 (EBX)
    at<U8>(attack, 0x140) = static_cast<U8>(roll);                                  // 005B8EE7
    I total = static_cast<I>(roll);
    if (recovered_stats_has_feat_005a6630(stats_of(t), nullptr, 0x18) != 0) {      // 005B8EED..FC
        total += 6; at<U8>(attack, 0x142) = 0x18; at<U8>(attack, 0x143) = 6;       // 005B8EFE..08
    } else if (recovered_stats_has_feat_005a6630(stats_of(t), nullptr, 1) != 0) {  // 005B8F11..20
        total += 3; at<U8>(attack, 0x142) = 1; at<U8>(attack, 0x143) = 3;          // 005B8F22..2C
    }
    total += static_cast<I>(kotor_fastcall_005a60d0(stats_of(t), nullptr));       // 005B8F33..44 (full EAX)
    at<U8>(attack, 0x146) = static_cast<U8>(kotor_fastcall_005a60d0(stats_of(t), nullptr)); // 005B8F3E..4B
    const U8 skill = static_cast<U8>(at<U8>(stats_of(t), 0x16d) + at<U8>(stats_of(t), 0x16c)); // 005B8F51..70
    total += static_cast<signed char>(skill);                                       // 005B8F65..6E
    at<U8>(attack, 0x147) = skill;                                                  // 005B8F7E
    unsigned char* effects = static_cast<unsigned char*>(t) + 0x124;                // 005B8F76 (CExoArrayList<CGameEffect*>)
    U sum = 0;                                                                      // [esp+14] (reused slot)
    for (I i = 0; i < at<I>(effects, 4); ++i) {                                     // 005B8F86..BF (count re-read)
        void* e = at<void**>(effects, 0)[i];
        const U type = at<U16>(e, 8);
        if (type == k_effect_type_5c_007467e8 || type == k_effect_type_5d_007467ec) // 005B8F9A..AC
            sum += recovered_effect_integer_00503690(e, nullptr, 1);                // 005B8FAE..B5
    }
    total += static_cast<I>(sum);                                                   // 005B8FC5
    at<U8>(attack, 0x144) = static_cast<U8>(sum);                                   // 005B8FC7
    I need = static_cast<I>(static_cast<signed char>(at<U8>(attack, 0xc2))) + at<U8>(attack, 0xc1); // 005B8FCD..DB
    at<U8>(attack, 0x148) = static_cast<U8>(total);                                 // 005B8FDF
    at<U8>(attack, 0x141) = static_cast<U8>(need);                                  // 005B8FE5
    if (total < need) return 0;                                                     // 005B8FEB (jl)
    need += 6;                                                                      // 005B8FED
    U8 r;
    if (total < need) r = 8;                                                        // 005B8FF0..F7
    else if (hit != 0) r = 9;                                                       // 005B8FFD..03
    else r = total < need ? 8 : 9;                                                  // 005B9005..07
    at<U8>(attack, 0x5c) = r; at<U8>(attack, 0x145) = r;                            // 005B8EC3..C8 / 005B900D..12
    return 1;
}

// ---------------------------------------------------------------------------------------------------------
// 005BA540 CSWSCreature::ResolveRangedSpecialAttack(target, flag): RET 8. Only when target != 0 and flag == 0.
// The special attack feat (+64, cleared when no uses remain) applies the self effect; 14h/1Fh/4Dh also try the
// stun effect on the target when the attack result passes FUN_005b5510.
void __fastcall kotor_fastcall_005ba540(void* creature, void*, void* target, U flag) {
    if (target == nullptr || flag != 0) return;                                     // 005BA556..6C
    void* attack = current_attack(round_of(creature));                              // 005BA572..8D (EBX, [esp+28])
    if (static_cast<U8>(kt_feat_remaining_uses_005a6680(stats_of(creature), nullptr, at<U16>(attack, 0x64))) == 0) // 005BA587..A1 (ja)
        at<U16>(attack, 0x64) = 0;                                                  // 005BA5A3
    const U feat = at<U16>(attack, 0x64);                                           // 005BA5A7
    switch (feat) {                                                                 // 005BA5AB..BE (table 005BA87C / 005BA888)
    case 0x1a: case 0x1e: case 0x5c: {                                              // 005BA5C5
        const U value = feat == 0x1a ? 1u : (feat == 0x1e ? 4u : 2u);               // 005BA5C5..E4
        apply_special_effect(creature, value);                                      // 005BA5E9..90
        return;
    }
    case 0x14: case 0x1f: case 0x4d:                                                // 005BA69A
        apply_special_effect(creature, 5);                                          // 005BA69A..748
        if (kotor_fastcall_005b5510(creature, nullptr, at<U8>(attack, 0x5c)) == 0) return; // 005BA74D..5B
        if (as_creature(target) == nullptr) return;                                 // 005BA761..6C
        apply_stun(creature, target, attack, 0xf0);                                 // 005BA772..85B
        return;
    default:
        return;                                                                     // 005BA865
    }
}

// ---------------------------------------------------------------------------------------------------------
// 005BA8E0 CSWSCreature::ResolveMeleeSpecialAttack(flag, a2, target, a4): RET 10h. Only when target != 0 and
// flag == 0 (a2 / a4 not read). Feats 08h/13h/51h: self effect + stun attempt; 0Bh/35h/5Bh: self effect only.
void __fastcall kotor_fastcall_005ba8e0(void* creature, void*, U flag, U, void* target, U) {
    if (target == nullptr || flag != 0) return;                                     // 005BA8F7..0D
    void* attack = current_attack(round_of(creature));                              // 005BA913..26 (EBX, [esp+20])
    (void)recovered_server_ai_master_004aed80(server_app(), nullptr);               // 005BA928..34 (result unused)
    if (static_cast<U8>(kt_feat_remaining_uses_005a6680(stats_of(creature), nullptr, at<U16>(attack, 0x64))) == 0) // 005BA939..4D
        at<U16>(attack, 0x64) = 0;                                                  // 005BA94F
    const U feat = at<U16>(attack, 0x64);                                           // 005BA953
    switch (feat) {                                                                 // 005BA957..6A (table 005BAC3C / 005BAC48)
    case 0x08: case 0x13: case 0x51:                                                // 005BA971
        apply_special_effect(creature, 5);                                          // 005BA971..A19
        if (kotor_fastcall_005b5510(creature, nullptr, at<U8>(attack, 0x5c)) == 0) return; // 005BAA1E..2C
        if (as_creature(target) == nullptr) return;                                 // 005BAA32..3C
        apply_stun(creature, target, attack, 0xea);                                 // 005BAA42..B4E
        return;
    case 0x0b: case 0x35: case 0x5b: {                                              // 005BAB53
        const U value = feat == 0x0b ? 4u : (feat == 0x35 ? 1u : 2u);               // 005BAB53..72
        apply_special_effect(creature, value);                                      // 005BAB77..C20
        return;
    }
    default:
        return;                                                                     // 005BAC25
    }
}

// ---------------------------------------------------------------------------------------------------------
// 005BACA0 CSWSCreature::ResolveAttackRoll(target, preset, preset result): RET 0Ch. Fills the current attack:
// +3A weapon attack type, +C1 roll, +C2 attack modifier, +F4 AC, +D8 natural 20, +DC natural 1, +E0..+F1 critical
// threat/confirm, +5C result (1 hit, 2 critical, 3 automatic, 4 miss; preset != 0 stores the preset result).
void __fastcall kotor_fastcall_005baca0(void* creature, void*, void* target, U preset, U preset_result) {
    if (target == nullptr) return;                                                  // 005BACB9..C2
    void* round = round_of(creature);                                               // 005BACC9 (EBP)
    void* attack = recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c)); // 005BACCF..DF (ESI)
    kotor_fastcall_004d2ec0(static_cast<unsigned char*>(attack) + 0xe0, nullptr);   // 005BACE1..E7
    const U offhand = kotor_fastcall_004d2a00(round, nullptr);                      // 005BACEC..F9 -> [esp+18]
    at<U8>(attack, 0x3a) = static_cast<U8>(kotor_fastcall_004d3da0(round_of(creature), nullptr)); // 005BACF3..04
    U roll = d20();                                                                 // 005BAD02..17 -> the target slot
    if (w5_cell_attack_roll_override_00832874 != 0) roll = w5_cell_attack_roll_override_00832874; // 005BAD1B..24
    void* stats = stats_of(creature);                                               // 005BAD2A
    const I modifier = static_cast<I>(kotor_fastcall_005a7ea0(stats, nullptr, as_creature(target))); // 005BAD28..3D -> [esp+10]
    I ac = 0;                                                                       // [esp+14]
    void* t = nullptr;                                                              // EBP
    if (as_creature(target) != nullptr) {                                           // 005BAD41..50
        t = as_creature(target);                                                    // 005BAD52..59
        ac = static_cast<short>(kotor_fastcall_005ad0e0(stats_of(t), nullptr, creature, 0)); // 005BAD5B..69 (movsx ax)
        at<U8>(attack, 0xf4) = static_cast<U8>(ac);                                 // 005BAD73
        kotor_fastcall_005b8820(creature, nullptr, t);                              // 005BAD6C..79
    }
    if (preset != 0) { at<U8>(attack, 0x5c) = static_cast<U8>(preset_result); return; } // 005BAD7E..8D
    if (at<U>(attack, 0x4c) != 0) {                                                 // 005BAD92..97
        at<U8>(attack, 0x5c) = 3; at<U8>(attack, 0xc1) = 0x14; at<U8>(attack, 0xc2) = static_cast<U8>(modifier); // 005BAD99..A8
        return;
    }
    if (at<U>(creature, 0x8d4) != 0) {                                              // 005BADB3..BB
        at<U8>(attack, 0x5c) = 1; at<U8>(attack, 0xc1) = static_cast<U8>(roll); at<U8>(attack, 0xc2) = static_cast<U8>(modifier);
        return;                                                                     // 005BADBD..D5
    }
    at<U8>(attack, 0xc1) = static_cast<U8>(roll);                                   // 005BADDA..E2
    at<U8>(attack, 0xc2) = static_cast<U8>(modifier);                               // 005BADEC
    const I total = modifier + static_cast<I>(roll);                                // 005BADE8..F6 -> the preset slot
    if (kotor_fastcall_005b8e50(creature, nullptr, target, total >= ac ? 1u : 0u) != 0) return; // 005BADF8..16 (jl)
    if (roll == 1) { at<U>(attack, 0xdc) = 1; at<U8>(attack, 0x5c) = 4; return; }  // 005BAE1C..27 / 005BAF7D..83
    at<U>(attack, 0xdc) = 0;                                                        // 005BAE37
    if (total < ac) {                                                               // 005BAE33..3D (jge)
        if (roll != 0x14) { at<U8>(attack, 0x5c) = 4; return; }                    // 005BAE3F..44 -> 005BAF83
        at<U>(attack, 0xd8) = 1;                                                    // 005BAE4E
    } else {
        at<U>(attack, 0xd8) = roll == 0x14 ? 1u : 0u;                               // 005BAE49..56
    }
    const I threat = static_cast<I>(recovered_critical_threat_005a5130(stats_of(creature), nullptr, offhand)); // 005BAE5C..67
    at<U8>(attack, 0xe0) = static_cast<U8>(threat);                                 // 005BAE70
    if (static_cast<I>(roll) < threat) {                                            // 005BAE6C..76 (jl)
        at<U>(attack, 0xe4) = 0; at<U>(attack, 0xec) = 0; at<U8>(attack, 0xe8) = 0; at<U8>(attack, 0xf1) = 0;
        at<U8>(attack, 0x5c) = 1;                                                   // 005BAF5D..77
        return;
    }
    at<U>(attack, 0xe4) = 1;                                                        // 005BAE7E
    const U confirm = d20();                                                        // 005BAE7C..99
    at<U8>(attack, 0xe8) = static_cast<U8>(confirm);                                // 005BAE9C
    if (static_cast<I>(confirm) + modifier < ac) {                                  // 005BAEA2..A8 (jl)
        at<U>(attack, 0xec) = 0; at<U8>(attack, 0xf1) = 0; at<U8>(attack, 0x5c) = 1; // 005BAF46..57
        return;
    }
    at<U>(attack, 0xec) = 1;                                                        // 005BAEAE
    at<U8>(attack, 0xf1) = static_cast<U8>(kotor_fastcall_005a52b0(stats_of(creature), nullptr, offhand)); // 005BAEB8..C6
    if (t != nullptr && recovered_stats_effect_query_005a6960(stats_of(t), nullptr, k_imm_00746970, creature) != 0) { // 005BAEC4..E5
        void* data = new_message_data();                                            // 005BAEE7..0C (EDI)
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(t, 4));             // 005BAF0E..1E
        recovered_event_integer_004d76b0(data, nullptr, 0, 0x7e);                   // 005BAF23..29
        list_add(static_cast<unsigned char*>(attack) + 0xa0, data);                 // 005BAF2E..35
        at<U8>(attack, 0x5c) = 1;                                                   // 005BAF3A
        return;
    }
    at<U8>(attack, 0x5c) = 2;                                                       // 005BAF40
}
}  // extern "C"
