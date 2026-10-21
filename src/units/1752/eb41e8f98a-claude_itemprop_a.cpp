// Lane effects-itemprop, part A: CSWSItemPropertyHandler Apply callbacks 004E5490..004E6D4C
// (enhancement bonus, attack penalty, attack bonus, light, ability bonus, AC bonus, improved force resistance,
// special walk, true seeing, freedom of movement), reconstructed from the original swkotor.exe (read-only).
// ABI and data layout: see claude_itemprop_common.hpp.  Every function here is
//   thiscall(item, property, creature, slot, load_game) RET 14h, EAX = 0  ->  @kotor_fastcall_<addr>@28.
// Addresses in the comments are original instructions.
// Original quirks kept:
//  * operator new results are never checked before the effect is written (null effect -> fault);
//  * when the cost-table value is 0 the already constructed effects are leaked (004E5490, 004E58C0, 004E5E80,
//    004E6510);
//  * 004E5490 / 004E58C0 start with a CSWBaseItemArray::GetBaseItem call whose result is discarded;
//  * the GetINTEntry out cell is never initialised (left indeterminate when the row is invalid);
//  * the ApplyEffect argument pairs are exactly as pushed: (load_game, 0) for most, (load_game, load_game) for
//    004E62D0, 004E68E0, 004E6AD0.
#include "9bb88ebc0b-claude_itemprop_common.hpp"

using namespace ipx;

extern "C" {
unsigned __fastcall kotor_fastcall_004e5490(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e58c0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e5e80(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e62d0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e6390(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e6510(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e66b0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e68e0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e6a30(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e6ad0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
}

namespace {
// effect types (.rdata WORDs)
constexpr U k_type_attack_inc_007466a4 = 0x0a;
constexpr U k_type_attack_dec_007466a8 = 0x0b;
constexpr U k_type_damage_inc_007466b0 = 0x0d;
constexpr U k_type_damage_dec_007466b4 = 0x0e;
constexpr U k_type_immunity_007466d4 = 0x16;
constexpr U k_type_force_resist_00746700 = 0x21;
constexpr U k_type_ability_inc_0074670c = 0x24;
constexpr U k_type_ac_inc_0074673c = 0x30;
constexpr U k_type_light_00746754 = 0x36;
constexpr U k_type_walk_a_00746764 = 0x3a;
constexpr U k_type_walk_b_00746768 = 0x3b;
constexpr U k_type_true_seeing_0074679c = 0x48;
// attack-bonus weapon-hand values (.rdata 007468C8..DC) and damage-bonus values (.rdata 007468E4..F8)
constexpr U k_attack_hand_007468c8[6] = {1, 2, 3, 4, 5, 7};  // 007468C8 CC D0 D4 D8 DC
constexpr U k_damage_hand_007468e4[6] = {1, 2, 3, 4, 5, 7};  // 007468E4 E8 EC F0 F4 F8
constexpr U k_attack_offhand_007468cc = 2;                  // .rdata 007468CC
constexpr U k_damage_offhand_007468e8 = 2;                  // .rdata 007468E8
// ability index jump table 004E64F0 values (.rdata 00746838..0074684C)
constexpr U k_ability_00746838[6] = {0, 1, 2, 3, 4, 5};
constexpr U k_ac_kind_0074688c = 0x4007;                    // .rdata 0074688C
// immunity types (.rdata 00746914 / 00746920 / 00746924 / 00746954)
constexpr U k_immunity_00746914 = 6;
constexpr U k_immunity_00746920 = 9;
constexpr U k_immunity_00746924 = 0xa;
constexpr U k_immunity_00746954 = 0x18;
constexpr U k_base_item_two_handed = 3;                     // cmp byte [base+8],3

// slot compare chain: 10h, 20h, 4000h, 8000h, 10000h, 8 -> values[0..5]; any other slot -> no store
inline bool slot_value(U slot, const U (&values)[6], U* out) {
    switch (slot) {
    case 0x10: *out = values[0]; return true;
    case 0x20: *out = values[1]; return true;
    case 0x4000: *out = values[2]; return true;
    case 0x8000: *out = values[3]; return true;
    case 0x10000: *out = values[4]; return true;
    case 8: *out = values[5]; return true;
    default: return false;
    }
}
// alignment-group / racial arms: kind_a (subtype 1 -> int3=1, 2 -> int4=2, 3 -> int4=3), kind_b -> int2=subtype
inline void versus_arms(void* e, void* property, U16 kind_a, U16 kind_b) {
    const U16 kind = prop_type(property);
    if (kind == kind_a) {
        const U16 sub = prop_subtype(property);
        if (sub == 1) set_int(e, 3, 1);
        else if (sub == 2) set_int(e, 4, 2);
        else if (sub == 3) set_int(e, 4, 3);
    } else if (kind == kind_b) {
        set_int(e, 2, prop_subtype(property));
    }
}
// new CGameEffect(0); CopyEffect(source, 0); SetInteger(index, value); ApplyEffect(copy, load_game, 0)
inline void apply_offhand_copy(void* creature, void* source, U index, U value, U load_game) {
    void* copy = new_effect(0);
    recovered_effect_copy_00504090(copy, nullptr, source, 0);
    set_int(copy, index, value);
    apply(creature, copy, load_game, 0);
}
}  // namespace

extern "C" {

// ===================================================================================================== 004E5490
// ApplyEnhancementBonus: attack increase (0Ah) + damage increase (0Dh), value from [[rules+B8]+30] "Value".
unsigned __fastcall kotor_fastcall_004e5490(void*, void*, void* item, void* property, void* creature, unsigned slot,
                                            unsigned load_game) {
    (void)base_item(item);                                                               // 004E54A8..BD (discarded)
    void* attack = new_effect(1);                                                        // 004E54C2..EA
    void* damage = new_effect(1);                                                        // 004E54EF..19
    I value;                                                                             // [esp+10] (uninitialised)
    cost_int(at<void*>(two_dim(), 0x30), prop_cost(property), &value);                   // 004E551B..67
    if (value == 0) return 0;                                                            // 004E556C..72 (both leaked)
    set_type_equipped(attack, k_type_attack_inc_007466a4);                               // 004E5578..9C
    set_creator(attack, item);                                                           // 004E55A0..A4
    set_int(attack, 0, static_cast<U>(value));                                           // 004E55A9..B2
    U hand;
    if (slot_value(slot, k_attack_hand_007468c8, &hand)) set_int(attack, 1, hand);       // 004E55B7..17
    set_int(attack, 2, racial_all());                                                    // 004E561C..2E
    versus_arms(attack, property, 6, 7);                                                 // 004E5633..73
    if (at<U8>(base_item(item), 8) == k_base_item_two_handed)                            // 004E5678..91
        apply_offhand_copy(creature, attack, 1, k_attack_offhand_007468cc, load_game);    // 004E5693..EF
    apply(creature, attack, load_game, 0);                                               // 004E56FA..04
    set_type_equipped(damage, k_type_damage_inc_007466b0);                               // 004E5709..2D
    set_creator(damage, item);                                                           // 004E5731..35
    if (slot_value(slot, k_damage_hand_007468e4, &hand)) set_int(damage, 5, hand);       // 004E573A..96
    set_int(damage, 0, static_cast<U>(value));                                           // 004E579B..A4
    set_int(damage, 1, kotor_fastcall_00554050(item, nullptr));                          // 004E57A9..B5
    set_int(damage, 2, racial_all());                                                    // 004E57BA..CC
    versus_arms(damage, property, 6, 7);                                                 // 004E57D1..15
    if (at<U8>(base_item(item), 8) == k_base_item_two_handed)                            // 004E581A..30
        apply_offhand_copy(creature, damage, 5, k_damage_offhand_007468e8, load_game);    // 004E5832..8A
    apply(creature, damage, load_game, 0);                                               // 004E588F..99
    return 0;
}

// ===================================================================================================== 004E58C0
// ApplyAttackPenalty: attack decrease (0Bh) + damage decrease (0Eh), value = -GetIPRPCostTable(14h) "Value".
unsigned __fastcall kotor_fastcall_004e58c0(void*, void*, void* item, void* property, void* creature, unsigned slot,
                                            unsigned load_game) {
    (void)base_item(item);                                                               // 004E58DA..EC (discarded)
    void* attack = new_effect(1);                                                        // 004E58F1..19
    void* damage = new_effect(1);                                                        // 004E591E..48
    I value;                                                                             // [esp+10] (uninitialised)
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x14), prop_cost(property), &value); // 004E594A..9C
    value = static_cast<I>(0u - static_cast<U>(value));                                  // 004E59A1..A7 neg
    if (value == 0) return 0;                                                            // 004E59AB (both leaked)
    set_type_equipped(attack, k_type_attack_dec_007466a8);                               // 004E59B1..D0
    set_creator(attack, item);                                                           // 004E59D4..D8
    set_int(attack, 0, static_cast<U>(value));                                           // 004E59DD..E6
    set_int(attack, 2, racial_all());                                                    // 004E59EB..FD
    U hand;
    if (slot_value(slot, k_attack_hand_007468c8, &hand)) set_int(attack, 1, hand);       // 004E5A02..62
    if (at<U8>(base_item(item), 8) == k_base_item_two_handed)                            // 004E5A67..7E
        apply_offhand_copy(creature, attack, 1, k_attack_offhand_007468cc, load_game);    // 004E5A80..DC
    apply(creature, attack, load_game, 0);                                               // 004E5AE7..F1
    set_type_equipped(damage, k_type_damage_dec_007466b4);                               // 004E5AF6..19
    set_creator(damage, item);                                                           // 004E5B1D..21
    if (slot_value(slot, k_damage_hand_007468e4, &hand)) set_int(damage, 5, hand);       // 004E5B26..82
    set_int(damage, 0, static_cast<U>(value));                                           // 004E5B87..90
    set_int(damage, 1, kotor_fastcall_00554050(item, nullptr));                          // 004E5B95..A1
    set_int(damage, 2, racial_all());                                                    // 004E5BA6..B8
    if (at<U8>(base_item(item), 8) == k_base_item_two_handed)                            // 004E5BBD..D3
        apply_offhand_copy(creature, damage, 5, k_damage_offhand_007468e8, load_game);    // 004E5BD5..2D
    apply(creature, damage, load_game, 0);                                               // 004E5C32..3C
    return 0;
}

// ===================================================================================================== 004E5E80
// ApplyAttackBonus: attack increase (0Ah), value from [[rules+B8]+30] "Value"; versus arms on types 27h / 28h.
unsigned __fastcall kotor_fastcall_004e5e80(void*, void*, void* item, void* property, void* creature, unsigned slot,
                                            unsigned load_game) {
    void* attack = new_effect(1);                                                        // 004E5E9B..C5
    I value;                                                                             // [esp+0C] (uninitialised)
    cost_int(at<void*>(two_dim(), 0x30), prop_cost(property), &value);                   // 004E5ECA..16
    if (value == 0) return 0;                                                            // 004E5F1B..21 (leaked)
    set_type_equipped(attack, k_type_attack_inc_007466a4);                               // 004E5F27..4B
    set_creator(attack, item);                                                           // 004E5F4F..53
    set_int(attack, 0, static_cast<U>(value));                                           // 004E5F58..61
    U hand;
    if (slot_value(slot, k_attack_hand_007468c8, &hand)) set_int(attack, 1, hand);       // 004E5F66..C3
    set_int(attack, 2, racial_all());                                                    // 004E5FC8..DA
    versus_arms(attack, property, 0x27, 0x28);                                           // 004E5FDF..1F
    if (at<U8>(base_item(item), 8) == k_base_item_two_handed)                            // 004E6024..43
        apply_offhand_copy(creature, attack, 1, k_attack_offhand_007468cc, load_game);    // 004E6045..99
    apply(creature, attack, load_game, 0);                                               // 004E609E..A4
    return 0;
}

// ===================================================================================================== 004E62D0
// ApplyLight: only when the item has property 1Dh (subtype 0); effect 36h, int0 = 5000.
unsigned __fastcall kotor_fastcall_004e62d0(void*, void*, void* item, void*, void* creature, unsigned,
                                            unsigned load_game) {
    if (!recovered_item_property_has_00555c90(item, nullptr, 0x1d, 0)) return 0;         // 004E62E7..F8
    void* e = new_effect(1);                                                             // 004E62FA..24
    set_type_equipped(e, k_type_light_00746754);                                         // 004E6326..4B
    set_creator(e, item);                                                                // 004E634F..55
    set_int(e, 0, 0x1388);                                                               // 004E635A..63
    apply(creature, e, load_game, load_game);                                            // 004E6368..73
    return 0;
}

// ===================================================================================================== 004E6390
// ApplyAbilityBonus: GetIPRPCostTable(1) "Value"; effect 24h, int0 = ability (jump table 004E64F0), int1 = value.
unsigned __fastcall kotor_fastcall_004e6390(void*, void*, void* item, void* property, void* creature, unsigned,
                                            unsigned load_game) {
    I value;                                                                             // [esp+4] (uninitialised)
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 1), prop_cost(property), &value); // 004E63A9..FB
    if (value == 0) return 0;                                                            // 004E6400..06
    void* e = new_effect(1);                                                             // 004E640D..37
    set_type_equipped(e, k_type_ability_inc_0074670c);                                   // 004E6439..63
    set_creator(e, item);                                                                // 004E6467..6D
    const U sub = prop_subtype(property);                                                // 004E6472 movzx
    if (sub <= 5) set_int(e, 0, k_ability_00746838[sub]);                                // 004E6476..B7 (ja 004E64BC)
    set_int(e, 1, static_cast<U>(value));                                                // 004E64BC..C5
    apply(creature, e, load_game, 0);                                                    // 004E64CA..D6
    return 0;
}

// ===================================================================================================== 004E6510
// ApplyACBonus: GetIPRPCostTable(1) "Value"; effect 30h, int0 = base item AC type (byte +8B), int1 = value,
// int5 = 4007h, int2 = racial; property 2 (subtype 1..3) -> int4, 3 -> int5, 4 -> int2.
unsigned __fastcall kotor_fastcall_004e6510(void*, void*, void* item, void* property, void* creature, unsigned,
                                            unsigned load_game) {
    void* e = new_effect(1);                                                             // 004E652B..55
    I value;                                                                             // [esp+0C] (uninitialised)
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 1), prop_cost(property), &value); // 004E655A..AC
    if (value == 0) return 0;                                                            // 004E65B1..B7 (leaked)
    set_type_equipped(e, k_type_ac_inc_0074673c);                                        // 004E65BD..E1
    set_creator(e, item);                                                                // 004E65E5..E9
    set_int(e, 0, at<U8>(base_item(item), 0x8b));                                        // 004E65EE..0C
    set_int(e, 1, static_cast<U>(value));                                                // 004E6611..1A
    set_int(e, 5, k_ac_kind_0074688c);                                                   // 004E661F..2A
    set_int(e, 2, racial_all());                                                         // 004E662F..41
    const U16 kind = prop_type(property);                                                // 004E6646
    if (kind == 2) {
        const I sub = prop_subtype(property);                                            // 004E664F movzx
        if (sub > 0 && sub <= 3) set_int(e, 4, static_cast<U>(sub));                     // 004E6653..5F (jle/jg)
    } else if (kind == 3) {
        set_int(e, 5, prop_subtype(property));                                           // 004E6661..6E
    } else if (kind == 4) {
        set_int(e, 2, prop_subtype(property));                                           // 004E6670..7F
    }
    apply(creature, e, load_game, 0);                                                    // 004E6684..90
    return 0;
}

// ===================================================================================================== 004E66B0
// ApplyImprovedForceResistance: GetIPRPCostTable(0Bh) "Value"; effect 21h, int0 = value.
unsigned __fastcall kotor_fastcall_004e66b0(void*, void*, void* item, void* property, void* creature, unsigned,
                                            unsigned load_game) {
    I value;                                                                             // [esp] (uninitialised)
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0xb), prop_cost(property), &value); // 004E66C8..1A
    if (value == 0) return 0;                                                            // 004E671F..25
    void* e = new_effect(1);                                                             // 004E672C..56
    set_type_equipped(e, k_type_force_resist_00746700);                                  // 004E6758..82
    set_creator(e, item);                                                                // 004E6786..8C
    set_int(e, 0, static_cast<U>(value));                                                // 004E6791..9A
    apply(creature, e, load_game, 0);                                                    // 004E679F..AB
    return 0;
}

// ===================================================================================================== 004E68E0
// ApplySpecialWalk: effect 3Ah (int0 = subtype) and effect 3Bh, both flagged magical (and FFEFh; or 8) first.
unsigned __fastcall kotor_fastcall_004e68e0(void*, void*, void* item, void* property, void* creature, unsigned,
                                            unsigned load_game) {
    void* e = new_effect(1);                                                             // 004E68FA..24
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xffefu) | 8u);                // 004E692A..42
    set_creator(e, item);                                                                // 004E6946..4A
    at<U16>(e, 8) = static_cast<U16>(k_type_walk_a_00746764);                            // 004E694F..5A
    set_int(e, 0, prop_subtype(property));                                               // 004E6956..67
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_duration_equipped_007473bc); // 004E6974..8A
    apply(creature, e, load_game, load_game);                                            // 004E696C..90
    void* f = new_effect(1);                                                             // 004E6995..BF
    at<U16>(f, 0xa) = static_cast<U16>((at<U16>(f, 0xa) & 0xffefu) | 8u);                // 004E69C1..DA
    set_creator(f, item);                                                                // 004E69DE..E2
    set_type_equipped(f, k_type_walk_b_00746768);                                        // 004E69E7..0A
    apply(creature, f, load_game, load_game);                                            // 004E69F4..0E
    return 0;
}

// ===================================================================================================== 004E6A30
// ApplyTrueSeeing: effect 48h.
unsigned __fastcall kotor_fastcall_004e6a30(void*, void*, void* item, void*, void* creature, unsigned,
                                            unsigned load_game) {
    void* e = new_effect(1);                                                             // 004E6A47..71
    set_type_equipped(e, k_type_true_seeing_0074679c);                                   // 004E6A73..9C
    set_creator(e, item);                                                                // 004E6AA0..A6
    apply(creature, e, load_game, 0);                                                    // 004E6AAB..B7
    return 0;
}

// ===================================================================================================== 004E6AD0
// ApplyFreedomofMovement: four immunity effects (16h) for immunity types 6, 9, 0Ah, 18h; int1 = racial.
unsigned __fastcall kotor_fastcall_004e6ad0(void*, void*, void* item, void*, void* creature, unsigned,
                                            unsigned load_game) {
    static const U kinds[4] = {k_immunity_00746914, k_immunity_00746920, k_immunity_00746924, k_immunity_00746954};
    for (U i = 0; i < 4; ++i) {                                                          // 004E6AEA / 6B86 / 6C17 / 6CA8 (unrolled)
        void* e = new_effect(1);
        set_type_equipped(e, k_type_immunity_007466d4);
        set_creator(e, item);
        set_int(e, 0, kinds[i]);
        set_int(e, 1, racial_all());
        apply(creature, e, load_game, load_game);
    }
    return 0;
}

}  // extern "C"
