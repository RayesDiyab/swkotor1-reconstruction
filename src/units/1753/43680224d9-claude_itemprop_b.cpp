// Lane effects-itemprop, part B: CSWSItemPropertyHandler Apply/Remove callbacks
//   004E6D50 ApplyBonusFeat            004E6E20 ApplyDamageImmunity        004E6F60 ApplyDamagePenalty
//   004E7190 ApplyDamageReduction      004E72B0 ApplyDamageResistance      004E73D0 ApplyDamageVulnerability
//   004E74F0 ApplyDecreasedAbilityScore 004E7670 ApplyDecreasedAC          004E77C0 ApplyDecreasedSkillModifier
//   004E7900 ApplyImmunity             004E7B10 ApplySkillBonus            004E7C00 ApplyRegeneration
//   004E7CE0 ApplyBlasterBoltDeflectionIncrease  004E7DB0 ApplyBlasterBoltDeflectionDecrease  004E7E80 ApplyDisguise
//   004E94B0 RemoveSpecialWalk         004E95A0 RemoveTrueSeeing           004E9620 RemoveFreedomofMovement
//   004E9700 RemoveBonusFeat
// Transcribed from the original swkotor.exe (read-only, capstone); addresses in comments are original instructions.
// ABI and data layout: see claude_itemprop_common.hpp.  Apply: RET 14h (@28); Remove: RET 10h (@24); EAX = 0.
// Quirks kept (all original):
//  * operator new results are not null-checked before use, except where the original tests the effect
//    (004E6D50, 004E6E20, 004E7900 skip silently on a null effect).
//  * an effect allocated before an early exit is leaked (004E6F60 / 004E7C00 when the value is zero).
//  * 004E6F60 calls GetBaseItem once at entry and discards the result.
//  * C2DA::GetINTEntry out cells: 004E6E20 uses the property argument slot whose low byte was overwritten by
//    property+4 (so a failed lookup yields that pointer-derived value); 004E6F60 uses the item argument slot
//    (a failed lookup yields the item pointer); the others use an uninitialised local.
//  * 004E6D50 / 004E6E20 / 004E7900 set the flags word with (flags & FFFB) | 3 instead of (flags & FFF8) | [007473BC].
//  * ApplyEffect(effect, a, b) pairs are passed exactly as the original pushes them ((load,load) or (load,0)).
//  * 004E94B0 / 004E9620 keep scanning after a RemoveEffectById, so the element that slides into the removed
//    slot is skipped (list index advanced over a shrunken list).
#include "9bb88ebc0b-claude_itemprop_common.hpp"

using namespace ipx;

extern "C" {
unsigned __fastcall kotor_fastcall_004e6d50(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e6e20(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e6f60(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7190(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e72b0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e73d0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e74f0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7670(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e77c0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7900(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7b10(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7c00(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7ce0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7db0(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7e80(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e94b0(void*, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e95a0(void*, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9620(void*, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9700(void*, void*, void* item, void* property, void* creature, unsigned slot);
}

namespace {
// ---- .rdata effect types (WORD/DWORD loads) ----
constexpr U k_eff_damage_resistance_00746684 = 0x02;  // .rdata 00746684
constexpr U k_eff_regenerate_00746698 = 0x07;         // .rdata 00746698
constexpr U k_eff_damage_reduction_007466ac = 0x0c;   // .rdata 007466AC
constexpr U k_eff_damage_decrease_007466b4 = 0x0e;    // .rdata 007466B4
constexpr U k_eff_damage_immunity_inc_007466bc = 0x10; // .rdata 007466BC
constexpr U k_eff_damage_immunity_dec_007466c0 = 0x11; // .rdata 007466C0
constexpr U k_eff_immunity_007466d4 = 0x16;           // .rdata 007466D4
constexpr U k_eff_ability_decrease_00746710 = 0x25;   // .rdata 00746710
constexpr U k_eff_ac_decrease_00746740 = 0x31;        // .rdata 00746740
constexpr U k_eff_skill_increase_00746758 = 0x37;     // .rdata 00746758
constexpr U k_eff_skill_decrease_0074675c = 0x38;     // .rdata 0074675C
constexpr I k_eff_special_walk_a_00746764 = 0x3a;     // .rdata 00746764
constexpr I k_eff_special_walk_b_00746768 = 0x3b;     // .rdata 00746768
constexpr U k_eff_disguise_00746774 = 0x3e;           // .rdata 00746774
constexpr I k_eff_true_seeing_0074679c = 0x48;        // .rdata 0074679C
constexpr U k_eff_bonus_feat_007467c8 = 0x53;         // .rdata 007467C8
constexpr U k_eff_deflect_inc_007467e8 = 0x5c;        // .rdata 007467E8
constexpr U k_eff_deflect_dec_007467ec = 0x5d;        // .rdata 007467EC
// ---- .rdata integer values ----
constexpr U k_ac_vs_all_0074688c = 0x4007;            // .rdata 0074688C
constexpr U k_damage_slot_secondary_007468e8 = 2;     // .rdata 007468E8
// ability index jump table 004E7654: values [00746838..0074684C] = 0..5
constexpr U k_ability_00746838[6] = {0, 1, 2, 3, 4, 5};
// immunity jump table 004E7AE4 (subtype 0..9)
constexpr U k_imm_0074696c = 0x1e, k_imm_00746940 = 0x13, k_imm_00746968 = 0x1d, k_imm_00746900 = 1,
            k_imm_00746904 = 2, k_imm_00746908 = 3, k_imm_0074690c = 4, k_imm_00746964 = 0x1c,
            k_imm_00746914 = 6, k_imm_00746970 = 0x1f, k_imm_00746974 = 0x20;
// freedom-of-movement immunity values
constexpr U k_imm_paralysis_00746914 = 6, k_imm_entangle_00746920 = 9, k_imm_slow_00746924 = 0xa,
            k_imm_movement_speed_00746954 = 0x18;
const char k_amount_00746a00[] = "Amount";            // .rdata 00746A00

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
// mov [e+8],type; xor r,r; mov r16,[e+0A]; and r,FFFB; or r,3; mov [e+0A],r16
inline void set_type_fffb(void* e, U type) {
    at<U16>(e, 8) = static_cast<U16>(type);
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfffbu) | 3u);
}
// CExoString s(column); table->GetINTEntry(row, s, out); ~s
inline void cost_int_col(void* table, U row, const char* column, I* out) {
    ExoString s;
    frame_LogStringCtor_005e5a90(&s, nullptr, column);
    c2da_current_get_int_00413660(table, nullptr, row, &s, out);
    frame_LogStringDtor_005e5c20(&s, nullptr);
}
inline void* cost_table(U index) { return kotor_fastcall_005c49a0(two_dim(), nullptr, index); }
// slot -> damage-hand integer 5 ([007468E4..007468F8] = 1,2,3,4,5,7); other slots store nothing
inline void set_damage_hand(void* e, U slot) {
    U v;
    if (slot == 0x10) v = 1;            // [007468E4]
    else if (slot == 0x20) v = 2;       // [007468E8]
    else if (slot == 0x4000) v = 3;     // [007468EC]
    else if (slot == 0x8000) v = 4;     // [007468F0]
    else if (slot == 0x10000) v = 5;    // [007468F4]
    else if (slot == 8) v = 7;          // [007468F8]
    else return;
    set_int(e, 5, v);
}
inline void** effects(void* creature) { return at<void**>(creature, 0x124); }
inline I effect_count(void* creature) { return at<I>(creature, 0x128); }
inline U eff_type(void* e) { return at<U16>(e, 8); }
inline bool creator_is(void* e, void* item) { return at<U>(e, 0x18) == at<U>(item, 0x14); }
inline bool equipped(void* e) { return static_cast<U16>(duration_bits(e)) == k_duration_equipped_007473bc; }
}  // namespace

extern "C" {

// ===================================================================================================== 004E6D50
// ApplyBonusFeat
unsigned __fastcall kotor_fastcall_004e6d50(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    if (at<U>(creature, 0xa74) == 0) return 0;                                       // 004E6D67..73
    void* e = new_effect(1);                                                         // 004E6D79..A3
    if (e == nullptr) return 0;                                                      // 004E6DA5..AF
    set_type_fffb(e, k_eff_bonus_feat_007467c8);                                     // 004E6DB1..CE
    set_int(e, 0, prop_subtype(property));                                           // 004E6DD2..DB
    set_creator(e, item);                                                            // 004E6DE0..EA
    apply(creature, e, load_game, load_game);                                        // 004E6DEF..F8
    return 0;
}

// ===================================================================================================== 004E6E20
// ApplyDamageImmunity
unsigned __fastcall kotor_fastcall_004e6e20(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    // 004E6E43..4A: mov al,[prop+4]; mov byte [arg2],al; mov edx,[arg2] -> argument slot with its low byte replaced
    I slot_cell = static_cast<I>((addr(property) & 0xffffff00u) | at<U8>(property, 4));
    void* table = cost_table(static_cast<U>(slot_cell));                             // 004E6E4E..56 (BYTE index)
    if (table == nullptr) return 0;                                                  // 004E6E5B..5F
    const U16 subtype = prop_subtype(property);                                      // 004E6E66
    const U16 cost = prop_cost(property);                                            // 004E6E78
    cost_int_col(table, cost, k_value_00744bfc, &slot_cell);                         // 004E6E6A..A5 (out = arg slot)
    const U bit = 1u << (subtype & 31u);                                             // 004E6EAA..B6
    void* e = new_effect(1);                                                         // 004E6EB1..DE
    if (e == nullptr) return 0;                                                      // 004E6EE0..EA
    set_type_fffb(e, k_eff_damage_immunity_inc_007466bc);                            // 004E6EEC..08
    set_int(e, 0, bit);                                                              // 004E6EF8..0E
    set_int(e, 1, static_cast<U>(slot_cell));                                        // 004E6F13..1C
    set_creator(e, item);                                                            // 004E6F21..2B
    apply(creature, e, load_game, load_game);                                        // 004E6F30..3B
    return 0;
}

// ===================================================================================================== 004E6F60
// ApplyDamagePenalty
unsigned __fastcall kotor_fastcall_004e6f60(void*, void*, void* item, void* property, void* creature, unsigned slot, unsigned load_game) {
    (void)base_item(item);                                                           // 004E6F7F..8B (result unused)
    void* e = new_effect(1);                                                         // 004E6F90..BA
    I value = static_cast<I>(addr(item));                                            // out cell = item argument slot
    cost_int(cost_table(0x14), prop_cost(property), &value);                         // 004E6FBF..11
    value = static_cast<I>(0u - static_cast<U>(value));                                                                // 004E7016..1C neg
    if (value == 0) return 0;                                                        // 004E7020 (effect leaked)
    set_type_equipped(e, k_eff_damage_decrease_007466b4);                            // 004E7026..45
    set_creator(e, item);                                                            // 004E7049..4D
    set_damage_hand(e, slot);                                                        // 004E7052..AF
    set_int(e, 0, static_cast<U>(value));                                            // 004E70B4..BE
    set_int(e, 1, kotor_fastcall_00554050(item, nullptr));                           // 004E70C3..CF GetDamageFlags
    set_int(e, 2, racial_all());                                                     // 004E70D4..E6
    if (at<U8>(base_item(item), 8) == 3) {                                           // 004E70EB..09
        void* second = new_effect(0);                                                // 004E710B..35
        recovered_effect_copy_00504090(second, nullptr, e, 0);                       // 004E7137..44
        set_int(second, 5, k_damage_slot_secondary_007468e8);                        // 004E7149..54
        apply(creature, second, load_game, 0);                                       // 004E7159..5F
    }
    apply(creature, e, load_game, 0);                                                // 004E7164..6A
    return 0;
}

// ===================================================================================================== 004E7190
// ApplyDamageReduction
unsigned __fastcall kotor_fastcall_004e7190(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    void* e = new_effect(1);                                                         // 004E71AA..D4
    set_type_equipped(e, k_eff_damage_reduction_007466ac);                           // 004E71D6..FF
    set_creator(e, item);                                                            // 004E7203..09
    I value;                                                                         // [esp+8] (uninitialised)
    cost_int_col(cost_table(6), prop_cost(property), k_amount_00746a00, &value);     // 004E720E..60
    set_int(e, 0, static_cast<U>(value));                                            // 004E7265..6E
    set_int(e, 1, prop_subtype(property) + 1u);                                      // 004E7273..7D
    apply(creature, e, load_game, 0);                                                // 004E7282..8E
    return 0;
}

// ===================================================================================================== 004E72B0
// ApplyDamageResistance
unsigned __fastcall kotor_fastcall_004e72b0(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    void* e = new_effect(1);                                                         // 004E72CA..F4
    set_type_equipped(e, k_eff_damage_resistance_00746684);                          // 004E72F6..1F
    set_creator(e, item);                                                            // 004E7323..29
    I value;                                                                         // [esp+8] (uninitialised)
    cost_int_col(cost_table(7), prop_cost(property), k_amount_00746a00, &value);     // 004E732E..80
    set_int(e, 0, 1u << (at<U8>(property, 2) & 31u));                                // 004E7385..94
    set_int(e, 1, static_cast<U>(value));                                            // 004E7399..A2
    apply(creature, e, load_game, 0);                                                // 004E73A7..B3
    return 0;
}

// ===================================================================================================== 004E73D0
// ApplyDamageVulnerability
unsigned __fastcall kotor_fastcall_004e73d0(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    void* e = new_effect(1);                                                         // 004E73EB..11
    const U16 subtype = prop_subtype(property);                                      // 004E7417..1B
    set_type_equipped(e, k_eff_damage_immunity_dec_007466c0);                        // 004E7411..42
    set_creator(e, item);                                                            // 004E7446..4C
    I value;                                                                         // [esp+0C] (uninitialised)
    cost_int(cost_table(0x16), prop_cost(property), &value);                         // 004E7451..9E
    set_int(e, 0, 1u << (subtype & 31u));                                            // 004E74A3..B1
    set_int(e, 1, static_cast<U>(value));                                            // 004E74B6..BF
    apply(creature, e, load_game, 0);                                                // 004E74C4..D0
    return 0;
}

// ===================================================================================================== 004E74F0
// ApplyDecreasedAbilityScore
unsigned __fastcall kotor_fastcall_004e74f0(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    I value;                                                                         // [esp+4] (uninitialised)
    cost_int(cost_table(0x15), prop_cost(property), &value);                         // 004E7509..5B
    value = static_cast<I>(0u - static_cast<U>(value));                                                                // 004E7560..66 neg
    if (value == 0) return 0;                                                        // 004E756A
    void* e = new_effect(1);                                                         // 004E7571..9B
    set_type_equipped(e, k_eff_ability_decrease_00746710);                           // 004E759D..C6
    set_creator(e, item);                                                            // 004E75CA..D0
    const U subtype = prop_subtype(property);                                        // 004E75D5
    if (subtype <= 5) set_int(e, 0, k_ability_00746838[subtype]);                    // 004E75D9..1A (ja / jump table)
    set_int(e, 1, static_cast<U>(value));                                            // 004E761F..28
    apply(creature, e, load_game, 0);                                                // 004E762D..39
    return 0;
}

// ===================================================================================================== 004E7670
// ApplyDecreasedAC
unsigned __fastcall kotor_fastcall_004e7670(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    I value;                                                                         // [esp+4] (uninitialised)
    cost_int(cost_table(0x14), prop_cost(property), &value);                         // 004E7689..DB
    value = static_cast<I>(0u - static_cast<U>(value));                                                                // 004E76E0..E6 neg
    if (value == 0) return 0;                                                        // 004E76EA
    void* e = new_effect(1);                                                         // 004E76F1..1B
    set_type_equipped(e, k_eff_ac_decrease_00746740);                                // 004E771D..46
    set_creator(e, item);                                                            // 004E774A..50
    set_int(e, 0, prop_subtype(property));                                           // 004E7755..5E
    set_int(e, 1, static_cast<U>(value));                                            // 004E7763..6C
    set_int(e, 2, racial_all());                                                     // 004E7771..83
    set_int(e, 5, k_ac_vs_all_0074688c);                                             // 004E7788..93
    apply(creature, e, load_game, 0);                                                // 004E7798..A4
    return 0;
}

// ===================================================================================================== 004E77C0
// ApplyDecreasedSkillModifier
unsigned __fastcall kotor_fastcall_004e77c0(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    I value;                                                                         // [esp+4] (uninitialised)
    cost_int(cost_table(0x15), prop_cost(property), &value);                         // 004E77D9..2B
    value = static_cast<I>(0u - static_cast<U>(value));                                                                // 004E7830..36 neg
    if (value == 0) return 0;                                                        // 004E783A
    void* e = new_effect(1);                                                         // 004E7841..6B
    set_type_equipped(e, k_eff_skill_decrease_0074675c);                             // 004E786D..96
    set_creator(e, item);                                                            // 004E789A..A0
    set_int(e, 0, prop_subtype(property));                                           // 004E78A5..AE
    set_int(e, 1, static_cast<U>(value));                                            // 004E78B3..BC
    set_int(e, 2, racial_all());                                                     // 004E78C1..D3
    apply(creature, e, load_game, 0);                                                // 004E78D8..E4
    return 0;
}

// ===================================================================================================== 004E7900
// ApplyImmunity
unsigned __fastcall kotor_fastcall_004e7900(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    U second = 0;                                                                    // [esp+10]
    U second_value = 0;                                                              // stored in the property argument slot
    U immunity;                                                                      // edi
    switch (prop_subtype(property)) {                                                // 004E7918..33 (ja 004E7ACA)
    case 0: immunity = k_imm_0074696c; break;                                        // 004E793A
    case 1: second_value = k_imm_00746940; immunity = k_imm_00746968; second = 1; break; // 004E7942..5A
    case 2: immunity = k_imm_00746900; break;                                        // 004E795C
    case 3: immunity = k_imm_00746904; break;                                        // 004E7964
    case 4: immunity = k_imm_00746908; break;                                        // 004E796C
    case 5: immunity = k_imm_0074690c; break;                                        // 004E7974
    case 6: immunity = k_imm_00746964; break;                                        // 004E797C
    case 7: immunity = k_imm_00746914; break;                                        // 004E7984
    case 8: immunity = k_imm_00746970; break;                                        // 004E798C
    case 9: immunity = k_imm_00746974; break;                                        // 004E7994
    default: return 0;
    }
    void* e = new_effect(1);                                                         // 004E799A..C0
    if (e == nullptr) return 0;                                                      // 004E79C2..CE
    set_type_fffb(e, k_eff_immunity_007466d4);                                       // 004E79D4..EF
    set_int(e, 0, immunity);                                                         // 004E79DF..F3
    set_int(e, 1, racial_all());                                                     // 004E79F8..0A
    set_creator(e, item);                                                            // 004E7A0F..19
    apply(creature, e, load_game, load_game);                                        // 004E7A1E..2B
    if (second == 0) return 0;                                                       // 004E7A30..36
    e = new_effect(1);                                                               // 004E7A3C..66
    if (e == nullptr) return 0;                                                      // 004E7A68..72
    set_type_fffb(e, k_eff_immunity_007466d4);                                       // 004E7A74..94
    set_int(e, 0, second_value);                                                     // 004E7A74..9A
    set_int(e, 1, racial_all());                                                     // 004E7A9F..B0
    set_creator(e, item);                                                            // 004E7AB5..BB
    apply(creature, e, load_game, load_game);                                        // 004E7AC0..C5
    return 0;
}

// ===================================================================================================== 004E7B10
// ApplySkillBonus
unsigned __fastcall kotor_fastcall_004e7b10(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    const I amount = prop_cost(property);                                            // 004E7B2C
    if (amount <= 0) return 0;                                                       // 004E7B30..32
    void* e = new_effect(1);                                                         // 004E7B38..62
    set_type_equipped(e, k_eff_skill_increase_00746758);                             // 004E7B64..8D
    set_creator(e, item);                                                            // 004E7B91..97
    set_int(e, 0, prop_subtype(property));                                           // 004E7B9C..A5
    set_int(e, 1, static_cast<U>(amount));                                           // 004E7BAA..AF
    set_int(e, 2, racial_all());                                                     // 004E7BB4..C6
    apply(creature, e, load_game, 0);                                                // 004E7BCB..D7
    return 0;
}

// ===================================================================================================== 004E7C00
// ApplyRegeneration
unsigned __fastcall kotor_fastcall_004e7c00(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    void* e = new_effect(1);                                                         // 004E7C19..43
    const I amount = prop_cost(property);                                            // 004E7C45..49
    if (amount <= 0) return 0;                                                       // 004E7C4D..57 (effect leaked)
    set_creator(e, item);                                                            // 004E7C59..63
    set_type_equipped(e, k_eff_regenerate_00746698);                                 // 004E7C68..8A
    set_int(e, 0, static_cast<U>(amount));                                           // 004E7C79..8E
    set_int(e, 1, 0x1770);                                                           // 004E7C93..9C (6000 ms)
    set_int(e, 4, prop_type(property));                                              // 004E7CA1..A9
    apply(creature, e, load_game, 0);                                                // 004E7CAE..BA
    return 0;
}

// ===================================================================================================== 004E7CE0
// ApplyBlasterBoltDeflectionIncrease
unsigned __fastcall kotor_fastcall_004e7ce0(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    const I amount = prop_cost(property);                                            // 004E7CFC
    if (amount <= 0) return 0;                                                       // 004E7D00..02
    void* e = new_effect(1);                                                         // 004E7D08..32
    set_type_equipped(e, k_eff_deflect_inc_007467e8);                                // 004E7D34..5D
    set_creator(e, item);                                                            // 004E7D61..67
    set_int(e, 0, prop_subtype(property));                                           // 004E7D6C..75
    set_int(e, 1, static_cast<U>(amount));                                           // 004E7D7A..7F
    apply(creature, e, load_game, 0);                                                // 004E7D84..90
    return 0;
}

// ===================================================================================================== 004E7DB0
// ApplyBlasterBoltDeflectionDecrease
unsigned __fastcall kotor_fastcall_004e7db0(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    const I amount = prop_cost(property);                                            // 004E7DCC
    if (amount <= 0) return 0;                                                       // 004E7DD0..D2
    void* e = new_effect(1);                                                         // 004E7DD8..02
    set_type_equipped(e, k_eff_deflect_dec_007467ec);                                // 004E7E04..2D
    set_creator(e, item);                                                            // 004E7E31..37
    set_int(e, 0, prop_subtype(property));                                           // 004E7E3C..45
    set_int(e, 1, static_cast<U>(amount));                                           // 004E7E4A..4F
    apply(creature, e, load_game, 0);                                                // 004E7E54..60
    return 0;
}

// ===================================================================================================== 004E7E80
// ApplyDisguise
unsigned __fastcall kotor_fastcall_004e7e80(void*, void*, void* item, void* property, void* creature, unsigned, unsigned load_game) {
    const U subtype = prop_subtype(property);                                        // 004E7E8E..9B
    void* e = new_effect(1);                                                         // 004E7E9F..C9
    set_type_equipped(e, k_eff_disguise_00746774);                                   // 004E7ECB..F5
    set_creator(e, item);                                                            // 004E7EF9..FF
    set_int(e, 0, subtype);                                                          // 004E7F04..09
    set_int(e, 1, 0);                                                                // 004E7F0E..14
    apply(creature, e, load_game, 0);                                                // 004E7F19..25
    return 0;
}

// ===================================================================================================== 004E94B0
// RemoveSpecialWalk: scan from the creature's sorted-effect start index for type 3Ah (matching subtype) / 3Bh.
unsigned __fastcall kotor_fastcall_004e94b0(void*, void*, void* item, void* property, void* creature, unsigned) {
    I i = at<U16>(at<void*>(creature, 0xa74), 0x150);                                // 004E94B1..BC
    if (i >= effect_count(creature)) return 0;                                       // 004E94C3..C9 (jge)
    do {
        void* e = effects(creature)[i];                                              // 004E94D1..D7
        if (static_cast<I>(eff_type(e)) > k_eff_special_walk_b_00746768) return 0;   // 004E94DA..E4 (jg)
        if (static_cast<I>(eff_type(e)) == k_eff_special_walk_a_00746764 && creator_is(e, item) &&
            get_int(e, 0) == prop_subtype(property) && equipped(e)) {                // 004E94EA..29
            remove_by_id(creature, e);                                               // 004E952B..34
        } else if (static_cast<I>(eff_type(e)) == k_eff_special_walk_b_00746768 && creator_is(e, item) &&
                   equipped(e)) {                                                    // 004E953B..62
            remove_by_id(creature, e);                                               // 004E957C..85
            return 0;
        }
        ++i;                                                                         // 004E9564..6D
    } while (i < effect_count(creature));
    return 0;
}

// ===================================================================================================== 004E95A0
// RemoveTrueSeeing: count and data pointer are read once before the scan.
unsigned __fastcall kotor_fastcall_004e95a0(void*, void*, void* item, void*, void* creature, unsigned) {
    const I count = effect_count(creature);                                          // 004E95A8
    if (count <= 0) return 0;                                                        // 004E95B0..B2
    void** cursor = effects(creature);                                               // 004E95B4
    for (I i = 0; i < count; ++i, ++cursor) {                                        // 004E95E7..ED
        void* e = *cursor;                                                           // 004E95C0
        if (static_cast<I>(eff_type(e)) == k_eff_true_seeing_0074679c && creator_is(e, item) && equipped(e)) { // 004E95C2..E5
            remove_by_id(creature, e);                                               // 004E95F8..03
            return 0;
        }
    }
    return 0;
}

// ===================================================================================================== 004E9620
// RemoveFreedomofMovement: removes every equipped immunity (16h) of this item against paralysis/entangle/slow/
// movement speed, scanning from the sorted-effect start index.
unsigned __fastcall kotor_fastcall_004e9620(void*, void*, void* item, void*, void* creature, unsigned) {
    I i = at<U16>(at<void*>(creature, 0xa74), 0x15e);                                // 004E9622..2C
    if (i >= effect_count(creature)) return 0;                                       // 004E9633..39 (jge)
    do {
        void* e = effects(creature)[i];                                              // 004E9650..56
        const I type = eff_type(e);                                                  // 004E9659..5D
        if (type > static_cast<I>(k_eff_immunity_007466d4)) return 0;                // 004E9662..64 (jg)
        if (type == static_cast<I>(k_eff_immunity_007466d4) && equipped(e) && creator_is(e, item)) { // 004E966A..89
            if (get_int(e, 0) == k_imm_paralysis_00746914 || get_int(e, 0) == k_imm_entangle_00746920 ||
                get_int(e, 0) == k_imm_slow_00746924 || get_int(e, 0) == k_imm_movement_speed_00746954) // 004E968B..CD
                remove_by_id(creature, e);                                           // 004E96CF..D8
        }
        ++i;                                                                         // 004E96DD..E6
    } while (i < effect_count(creature));
    return 0;
}

// ===================================================================================================== 004E9700
// RemoveBonusFeat
unsigned __fastcall kotor_fastcall_004e9700(void*, void*, void* item, void* property, void* creature, unsigned) {
    if (at<U>(creature, 0xa74) == 0) return 0;                                       // 004E9701..0D
    // 004E9713..1B: list = creature+124 (the compiler's non-null test of the member address is always true)
    if (effect_count(creature) <= 0) return 0;                                       // 004E971D..25
    I i = 0;
    do {
        void* e = effects(creature)[i];                                              // 004E9730..32
        if (eff_type(e) == k_eff_bonus_feat_007467c8 && creator_is(e, item) &&
            get_int(e, 0) == prop_subtype(property) && equipped(e)) {                // 004E9735..74
            remove_by_id(creature, e);                                               // 004E9787..92
            return 0;
        }
        ++i;                                                                         // 004E9776..7C
    } while (i < effect_count(creature));
    return 0;
}

}  // extern "C"
