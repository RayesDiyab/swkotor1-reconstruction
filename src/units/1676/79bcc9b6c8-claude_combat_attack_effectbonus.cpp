// W5 combat-attack lane, part F (effectbonus):
//   004F3FE0 CSWSCreature::GetTotalEffectBonus(kind, target, int* adjust, a4, a5, a6, a7, a8, a9, a10): RET 28h
//     kind 1 attack (types 0Ah/0Bh), 2 damage (0Dh/0Eh), 3 saving throw (1Ah/1Bh), 4 ability (24h/25h),
//     5 skill (37h/38h); any other kind returns 0. Jump table 004F5F54 = {004F410D, 004F4491, 004F5124, 004F5639,
//     004F5A78}.
//   + 005A5110 CSWSCreatureStats::GetSimpleAlignmentGoodEvil (no callable provider in c07b; only a port body).
// The complete function, replacing the partial W2 providers (@recovered_creature_total_effect_bonus_004f3fe0@48
// traps on kinds 1/2, @w2x_creature_effect_ability_004f3fe0@48 implements only the ability arm).
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// The creature keeps six 36-entry scratch arrays: +568 bonus values, +5F8 penalty values, +688 / +718 bonus /
// penalty effect subtypes (CGameEffect +1C), +7A8 / +838 bonus / penalty item ids (CSWSItem +14).
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c07b candidate map) ----------------
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                    // CGameEffect::GetInteger [W1 b7]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);            // CSWSCombatRound::GetAttack [W1 b7]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);           // CSWRules::RollDice [W1 b7]
extern U recovered_effect_rank_scratch_008327e0;                                                 // .bss 008327E0 fistp scratch [0886]
// ---------------- sibling parts of this lane ----------------
U __fastcall kotor_fastcall_005a52b0(void* stats, void*, U offhand);                             // GetCriticalHitMultiplier [attackstats]
void __fastcall kotor_fastcall_004d20d0(void* data, void*, U type, U value, U base);             // CSWSCombatAttackData::AddDamage [round]
// ---------------- exports ----------------
U __fastcall kotor_fastcall_005a5110(void* stats, void*);
U __fastcall kotor_fastcall_004f3fe0(void* creature, void*, U kind, U target, U adjust, U a4, U a5, U a6, U a7,
                                     U a8, U a9, U a10);
}

namespace {
constexpr U k_log_scale_bits_00745ff8 = 0x40549a78u;  // .rdata 00745FF8 (3.321928f)
constexpr I k_type_attack_inc_007466a4 = 0x0a;        // .rdata 007466A4
constexpr I k_type_attack_dec_007466a8 = 0x0b;        // .rdata 007466A8
constexpr I k_type_damage_inc_007466b0 = 0x0d;        // .rdata 007466B0
constexpr I k_type_damage_dec_007466b4 = 0x0e;        // .rdata 007466B4
constexpr I k_type_save_inc_007466e4 = 0x1a;          // .rdata 007466E4
constexpr I k_type_save_dec_007466e8 = 0x1b;          // .rdata 007466E8
constexpr I k_type_ability_inc_0074670c = 0x24;       // .rdata 0074670C
constexpr I k_type_ability_dec_00746710 = 0x25;       // .rdata 00746710
constexpr I k_type_skill_inc_00746758 = 0x37;         // .rdata 00746758
constexpr I k_type_skill_dec_0074675c = 0x38;         // .rdata 0074675C
constexpr I k_hand_any_007468c4 = 0;                  // .rdata 007468C4 (attack arm)
constexpr I k_hand_a_007468c8 = 1;                    // .rdata 007468C8
constexpr I k_hand_b_007468d0 = 3;                    // .rdata 007468D0
constexpr I k_hand_c_007468dc = 7;                    // .rdata 007468DC
constexpr I k_hand_any_007468e0 = 0;                  // .rdata 007468E0 (damage arm)
constexpr I k_hand_a_007468e4 = 1;                    // .rdata 007468E4
constexpr I k_hand_b_007468ec = 3;                    // .rdata 007468EC
constexpr I k_hand_c_007468f8 = 7;                    // .rdata 007468F8
const char k_die_00747034[] = "Die";                  // .rdata 00747034
const char k_num_dice_00747038[] = "NumDice";         // .rdata 00747038
const char k_rank_00747040[] = "Rank";                // .rdata 00747040
constexpr U k_slots = 0x24;                           // 36 entries per scratch array

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline I* arr(void* self, U offset) { return &at<I>(self, offset); }
inline I eff_int(void* effect, U index) { return static_cast<I>(recovered_effect_integer_00503690(effect, nullptr, index)); }
inline I eff_type(void* effect) { return at<U16>(effect, 8); }                         // movzx word [effect+8]
inline I eff_subtype(void* effect) { return at<I>(effect, 0x1c); }
inline I racial_all() { return at<U8>(rules(), 0xaa); }                                // [[7A3A28]+AA] byte

// fldlg2; fild [x]; fyl2x; fmul dword [00745FF8]; fstp dword; fld; frndint; fistp dword [008327E0]
// (004F4655..79 / 004F4C46..7C / 004F4EF0..1F): round(log2(x)) through the float; 0 -> integer indefinite.
I log2_index(I x) {
    I value = x;
    float scaled;
    const float scale = flt(k_log_scale_bits_00745ff8);
    __asm {
        fldlg2
        fild dword ptr [value]
        fyl2x
        fmul dword ptr [scale]
        fstp dword ptr [scaled]
        fld dword ptr [scaled]
        frndint
        fistp dword ptr [recovered_effect_rank_scratch_008327e0]
    }
    return static_cast<I>(recovered_effect_rank_scratch_008327e0);
}

// The replace-if-larger stacking shared by the attack, save, ability-increase and skill arms: an entry whose key
// (effect subtype or item id) matches keeps the larger value (GetInteger(value_index) re-read for the write);
// without a match the first free (-1) value slot takes key and value. skip_search: the original jumps straight to
// the free-slot search (subtype -1).
void stack_larger(void* self, void* effect, U values, U keys, I key, U value_index, bool skip_search) {
    I* v = arr(self, values);
    I* k = arr(self, keys);
    if (!skip_search) {
        bool found = false;                                                         // [esp+1C] / EBX
        for (U i = 0; i < k_slots; ++i) {
            if (k[i] != key) continue;
            found = true;
            if (v[i] < eff_int(effect, value_index)) {                              // jl (signed)
                v[i] = eff_int(effect, value_index);
                return;
            }
        }
        if (found) return;
    }
    for (U j = 0; j < k_slots; ++j) {
        if (v[j] == -1) {
            k[j] = key;
            v[j] = eff_int(effect, value_index);
            return;
        }
    }
}
// The ability-decrease variant (004F5853..593B): every matching entry accumulates GetInteger(1), no early exit.
void stack_add(void* self, void* effect, U values, U keys, I key, bool skip_search) {
    I* v = arr(self, values);
    I* k = arr(self, keys);
    if (!skip_search) {
        bool found = false;
        for (U i = 0; i < k_slots; ++i) {
            if (k[i] != key) continue;
            found = true;
            v[i] += eff_int(effect, 1);                                             // 004F587F..84 / 004F58FB..00
        }
        if (found) return;
    }
    for (U j = 0; j < k_slots; ++j) {
        if (v[j] == -1) {
            k[j] = key;
            v[j] = eff_int(effect, 1);
            return;
        }
    }
}
// item (+18 creator is an item: key = item +14, item key arrays) or subtype (+1C, subtype key arrays) stacking
void stack_item_or_subtype(void* self, void* effect, U values, U item_keys, U subtype_keys, U value_index, bool add) {
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, at<U>(effect, 0x18)); // GetItemByGameObjectID
    if (item != nullptr) {
        if (add) stack_add(self, effect, values, item_keys, at<I>(item, 0x14), false);
        else stack_larger(self, effect, values, item_keys, at<I>(item, 0x14), value_index, false);
        return;
    }
    const bool skip = eff_subtype(effect) == -1;
    if (add) stack_add(self, effect, values, subtype_keys, eff_subtype(effect), skip);
    else stack_larger(self, effect, values, subtype_keys, eff_subtype(effect), value_index, skip);
}
// the unrolled 4 x 9 sum of the value arrays (-1 entries skipped)
void sum_arrays(void* self, I& bonus, I& penalty) {
    const I* v = arr(self, 0x568);
    const I* p = arr(self, 0x5f8);
    for (U i = 0; i < k_slots; ++i) {
        if (v[i] != -1) bonus += v[i];
        if (p[i] != -1) penalty += p[i];
    }
}

struct Context {
    void* self; void* stats; void* list; void* attack; void* target; I* adjust;
    I hand;        // [esp+58]
    I racial;      // [esp+48] target racial type or -1
    I alignment;   // [esp+34] target good/evil or -1
    U a4, a5, a6, a7, a8, a9, a10;
};
inline I list_count(const Context& c) { return at<I>(c.list, 4); }
inline void* list_effect(const Context& c, I i) { return at<void**>(c.list, 0)[i]; }
inline bool racial_ok(const Context& c, I r) { return r == racial_all() || r == c.racial; }
inline bool alignment_ok(const Context& c, I a) { return a == 0 || a == c.alignment; }

// ---------------------------------------------------------------------------------------------------------
// kind 1: attack bonus (004F410D..448C, sum 004F43AC, clamp 004F560C)
I attack_bonus(const Context& c) {
    I direct_bonus = 0, direct_penalty = 0;                                         // [esp+28], [esp+44]
    const I count = list_count(c);
    if (count > 0) {                                                                // 004F410D..1A
        for (I i = at<U16>(c.stats, 0x130); i < count; ++i) {                       // 004F4124..37 / 004F4394..A6
            void* e = list_effect(c, i);
            if (eff_type(e) > k_type_attack_dec_007466a8) break;                    // 004F414C..56 (jg)
            const I hand = eff_int(e, 1);                                           // EDI
            const I racial = eff_int(e, 2);                                         // EBX
            const I alignment = eff_int(e, 4);                                      // EAX
            U hand_ok = (hand == k_hand_any_007468c4 || hand == c.hand) ? 1u : 0u;  // 004F4183..97
            if (c.hand == 6) {                                                      // 004F419C..AF
                if (hand == k_hand_a_007468c8 || hand == k_hand_b_007468d0) hand_ok = 1;
            } else if (c.hand == 8) {                                               // 004F41B1..BE
                if (hand == k_hand_c_007468dc) hand_ok = 1;
            }
            if (!racial_ok(c, racial) || !alignment_ok(c, alignment) || hand_ok != 1) continue; // 004F41C5..F1
            const I type = eff_type(e);
            if (type == k_type_attack_inc_007466a4) {                               // 004F41F7..01
                if (hand != k_hand_any_007468c4) direct_bonus += eff_int(e, 0);     // 004F4207..18
                else stack_larger(c.self, e, 0x568, 0x688, eff_subtype(e), 0, eff_subtype(e) == -1); // 004F4221..CA
            } else if (type == k_type_attack_dec_007466a8) {                        // 004F42CF..D5
                if (hand != k_hand_any_007468c4) direct_penalty += eff_int(e, 0);   // 004F42DB..EC
                else stack_larger(c.self, e, 0x5f8, 0x718, eff_subtype(e), 0, eff_subtype(e) == -1); // 004F42F5..8D
            }
        }
    }
    I bonus = direct_bonus, penalty = direct_penalty;                               // 004F43AC..B4
    sum_arrays(c.self, bonus, penalty);                                             // 004F43B8..86
    if (bonus > 0x14) bonus = 0x14;                                                 // 004F560C..11
    if (penalty > 0x14) penalty = 0x14;                                             // 004F5616..1B
    return bonus - penalty;
}

// ---------------------------------------------------------------------------------------------------------
// kind 2: damage bonus (004F4491..5121). The arm's locals are kept in a frame image at the original [esp+N]
// offsets because the per-damage-type arrays are indexed by round(log2(type bits)) without a bound check.
struct DamageFrame {
    alignas(4) unsigned char bytes[0x250];
    I& at_offset(U offset) { return *reinterpret_cast<I*>(bytes + offset); }
    // [esp + index*4 + base] with the original 32-bit wrap of index*4
    I& indexed(U base, I index) {
        return *reinterpret_cast<I*>(reinterpret_cast<unsigned char*>(
            reinterpret_cast<std::uintptr_t>(bytes) + base + static_cast<U>(index) * 4u));
    }
};
constexpr U kf_num_dice_14 = 0x14, kf_die_18 = 0x18, kf_rank_old_2c = 0x2c, kf_rank_3c = 0x3c, kf_rank_old_ac = 0xac;
constexpr U kf_penalty_b0 = 0xb0, kf_bonus_ec = 0xec, kf_types_128 = 0x128, kf_types_1b8 = 0x1b8;

I rank_of(void* table, I row, I* out) {                                             // CExoString "Rank" + GetINTEntry
    alignas(4) unsigned char column[8];
    frame_LogStringCtor_005e5a90(column, nullptr, k_rank_00747040);
    const I r = static_cast<I>(c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), column, out));
    frame_LogStringDtor_005e5c20(column, nullptr);
    return r;
}
void dice_of(void* table, I row, I* num_dice, I* die) {                             // "NumDice" then "Die"
    alignas(4) unsigned char column[8];
    frame_LogStringCtor_005e5a90(column, nullptr, k_num_dice_00747038);
    c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), column, num_dice);
    frame_LogStringDtor_005e5c20(column, nullptr);
    alignas(4) unsigned char column2[8];
    frame_LogStringCtor_005e5a90(column2, nullptr, k_die_00747034);
    c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), column2, die);
    frame_LogStringDtor_005e5c20(column2, nullptr);
}
inline I roll(I num_dice, I die) {
    return static_cast<I>(recovered_rules_roll_dice_00550b60(rules(), nullptr, static_cast<U>(num_dice), static_cast<U>(die)) & 0xffffu);
}
inline I crit_multiplier(const Context& c) { return static_cast<I>(kotor_fastcall_005a52b0(c.stats, nullptr, c.a10)); }
inline bool critical(const Context& c) { return at<U8>(c.attack, 0x5c) == 2; }
// target vtable +AC / +A8 damage reductions (004F4B84..BA / 004F4DC7..01 / 004F4E8B..C5)
I reduce(const Context& c, I amount, U flags) {
    const U r = vcall<U>(c.target, 0xac, c.self, static_cast<U>(amount), flags, c.a5, 1u);
    return static_cast<I>(vcall<U>(c.target, 0xa8, c.self, r, flags, c.a5, 1u, 0u));
}

I damage_bonus(const Context& c) {
    DamageFrame f;
    for (U o = 0; o < 0x3c; o += 4) { f.at_offset(kf_bonus_ec + o) = 0; f.at_offset(kf_penalty_b0 + o) = 0; } // 004F4491..BB
    for (U o = 0; o < 0x90; o += 4) { f.at_offset(kf_types_128 + o) = 0; f.at_offset(kf_types_1b8 + o) = 0; } // 004F44BD..E5
    f.at_offset(kf_rank_3c) = 0; f.at_offset(kf_rank_old_2c) = 0;                   // 004F44EC..F0 ([esp+3C], [esp+2C])
    f.at_offset(kf_num_dice_14) = 0; f.at_offset(kf_die_18) = 0;                    // 004F44F4..F8
    I total_bonus = 0, total_penalty = 0;                                           // [esp+4C], [esp+54]
    void* table = at<void*>(at<void*>(rules(), 0xb8), 0xb0);                        // 004F44E7..15 ([esp+44])
    I* bonus_types = &f.at_offset(kf_types_128);                                    // [esp+128] subtype stack damage types
    I* penalty_types = &f.at_offset(kf_types_1b8);                                  // [esp+1B8]
    I* values = arr(c.self, 0x568);
    I* penalties = arr(c.self, 0x5f8);
    const I count = list_count(c);
    if (count > 0) {                                                                // 004F4508..19
        for (I i = at<U16>(c.stats, 0x136); i < count; ++i) {                       // 004F451F..36 / 004F4B51..63
            void* e = list_effect(c, i);
            if (eff_type(e) > k_type_damage_dec_007466b4) break;                    // 004F454D..57
            U ok = 0;                                                               // EDI
            const I type_bits = eff_int(e, 1);                                      // EBX, [esp+28]
            const I racial = eff_int(e, 2);                                         // [esp+50]
            const I alignment = eff_int(e, 4);                                      // [esp+24]
            const I hand = eff_int(e, 5);                                           // [esp+30]
            if (hand == k_hand_any_007468e0) ok = 1;                                // 004F4595..A1
            if (hand == c.hand) ok = 1;                                             // 004F45A6..B0
            if (c.target != nullptr && vcall<U>(c.target, 0x38) != 0 &&             // 004F45B5..C3
                at<U>(c.target, 4) != at<U>(e, 0x18)) ok = 0;                       // 004F45C5..D0
            if (c.hand == 6) {                                                      // 004F45D2..EC
                if (hand == k_hand_a_007468e4 || hand == k_hand_b_007468ec) ok = 1;
            } else if (c.hand == 8) {                                               // 004F45EE..FA
                if (hand == k_hand_c_007468f8) ok = 1;
            }
            bool process = false;
            if (racial_ok(c, racial) && alignment_ok(c, alignment) && ok == 1) {    // 004F4601..30
                if (c.a4 == 0 && (type_bits & 7) != 0) process = true;              // 004F4632..39
                if ((static_cast<U>(type_bits) & 0xfffffff8u) != 0) process = true; // 004F463B..41
            }
            if (!process && c.a4 == 1 && (type_bits & 7) == 0) process = true;      // 004F4643..4F (review F1)
            if (!process) continue;
            const I index = log2_index(type_bits);                                  // 004F4655..7F -> [esp+30]
            const I type = eff_type(e);
            if (type == k_type_damage_inc_007466b0) {                               // 004F4679..8C
                if (hand != k_hand_any_007468e0) {                                  // 004F4692..98
                    rank_of(table, eff_int(e, 0), &f.at_offset(kf_rank_3c));        // 004F469E..E4
                    const I amount = eff_int(e, 0);                                 // 004F46E9..F7 ([esp+2C])
                    f.at_offset(kf_rank_old_2c) = amount;
                    I sum = 0;                                                      // ESI
                    if (amount > 5) {                                               // 004F46F4..FB (jle)
                        dice_of(table, amount, &f.at_offset(kf_num_dice_14), &f.at_offset(kf_die_18)); // 004F4701..7A
                        const I num_dice = f.at_offset(kf_num_dice_14), die = f.at_offset(kf_die_18);
                        if (c.a5 != 0) {                                            // 004F477F..84
                            sum = die * num_dice;                                   // 004F4786..91
                            *c.adjust += num_dice - sum;                            // 004F4794..A5
                            f.indexed(kf_bonus_ec, index) += sum;                   // 004F479A..A7
                            continue;
                        }
                        const I times = critical(c) ? crit_multiplier(c) : 1;       // 004F47AE..D4 / 004F4818
                        if (times > 0) {
                            for (I t = times; t != 0; --t) sum += roll(num_dice, die); // 004F47D8..FB
                        }
                    } else {
                        sum = amount;                                               // 004F481F
                    }
                    f.indexed(kf_bonus_ec, index) += sum;                           // 004F47FD..11 / 004F4821..35
                } else {                                                            // 004F483C: stacking by subtype
                    bool skip = eff_subtype(e) == -1;
                    if (!skip) {
                        bool found = false;                                         // [esp+1C]
                        bool done = false;
                        for (U k = 0; k < k_slots; ++k) {                           // 004F4846..08
                            if (at<I>(c.self, 0x688 + k * 4) != eff_subtype(e) || bonus_types[k] != type_bits) continue;
                            found = true;
                            rank_of(table, values[k], &f.at_offset(kf_rank_old_2c)); // 004F4897..F5
                            // quirk: [esp+3C] is the Rank of an earlier direct effect (0 if none), not of this one
                            if (f.at_offset(kf_rank_3c) > f.at_offset(kf_rank_old_2c)) { // 004F48FA..02
                                values[k] = eff_int(e, 0);                          // 004F4943..50
                                done = true;
                                break;
                            }
                        }
                        if (done || found) continue;                                // 004F490E..14
                    }
                    for (U j = 0; j < k_slots; ++j) {                               // 004F491E..3C
                        if (values[j] == -1) {
                            at<I>(c.self, 0x688 + j * 4) = eff_subtype(e);          // 004F495C..5F
                            values[j] = eff_int(e, 0);                              // 004F4966..73
                            bonus_types[j] = type_bits;                             // 004F497A
                            break;
                        }
                    }
                }
            } else if (type == k_type_damage_dec_007466b4) {                        // 004F4986..8F
                if (hand != k_hand_any_007468c4) {                                  // 004F4995..9B
                    I& slot = f.indexed(kf_penalty_b0, index);                      // EDI = &[esp+idx*4+B0]
                    rank_of(table, slot, &f.at_offset(kf_rank_old_ac));             // 004F49A1..FB
                    if (f.at_offset(kf_rank_3c) > f.at_offset(kf_rank_old_ac))      // 004F4A00..0B (stale [esp+3C])
                        slot = eff_int(e, 0);                                       // 004F4A11..1A
                } else {                                                            // 004F4A21: stacking by subtype
                    bool skip = eff_subtype(e) == -1;
                    if (!skip) {
                        bool found = false, done = false;
                        for (U k = 0; k < k_slots; ++k) {                           // 004F4A2B..E8
                            if (at<I>(c.self, 0x718 + k * 4) != eff_subtype(e) || penalty_types[k] != type_bits) continue;
                            found = true;
                            rank_of(table, penalties[k], &f.at_offset(kf_rank_old_2c)); // 004F4A77..D5
                            if (f.at_offset(kf_rank_3c) > f.at_offset(kf_rank_old_2c)) { // 004F4ADA..E2
                                penalties[k] = eff_int(e, 0);                       // 004F4B16..23
                                done = true;
                                break;
                            }
                        }
                        if (done || found) continue;                                // 004F4AEE..F4
                    }
                    for (U j = 0; j < k_slots; ++j) {                               // 004F4AFA..12
                        if (penalties[j] == -1) {
                            at<I>(c.self, 0x718 + j * 4) = eff_subtype(e);          // 004F4B2C..2F
                            penalties[j] = eff_int(e, 0);                           // 004F4B36..43
                            penalty_types[j] = type_bits;                           // 004F4B4A
                            break;
                        }
                    }
                }
            }
        }
    }
    // ---- 004F4B69: the 15 per-damage-type bonuses / penalties
    U flags = static_cast<U>(c.alignment);                                          // EDI = [esp+34] (quirk: stale flags)
    for (U t = 0; t < 0xf; ++t) {                                                   // 004F4B6D..C02
        I amount = f.indexed(kf_bonus_ec, static_cast<I>(t));                       // ESI
        if (amount > 0) {                                                           // 004F4B77..79 (jle)
            flags = 1u << t;                                                        // 004F4B7B..82
            if (c.target != nullptr) amount = reduce(c, amount, flags);             // 004F4B84..BA
            if (c.a5 == 0) kotor_fastcall_004d20d0(c.attack, nullptr, flags, static_cast<U>(amount), 1); // 004F4BBC..CB
            total_bonus += amount;                                                  // 004F4BD0
        }
        const I penalty = f.indexed(kf_penalty_b0, static_cast<I>(t));
        I taken = 0;
        if (penalty > 0) {                                                          // 004F4BD4..DF
            taken = penalty;
            kotor_fastcall_004d20d0(c.attack, nullptr, flags, static_cast<U>(-taken), 1); // 004F4BE1..EF
        }
        total_penalty += taken;                                                     // 004F4BF4..FE
    }
    const U stacked_flags = flags;                                                  // 004F4C08 [esp+34] = EDI
    // ---- 004F4C0C: the subtype-stacked entries
    for (U k = 0; k < k_slots; ++k) {                                               // 004F4C30..50E9
        I sum = 0;                                                                  // ESI (004F4C37)
        if (values[k] != -1) {                                                      // 004F4C34..3C
            const I index = log2_index(bonus_types[k]);                             // 004F4C42..7C -> [esp+30]
            const I v = values[k];                                                  // 004F4C70
            bool added_or_skipped = false;
            if (v > 5) {                                                            // 004F4C73..80
                dice_of(table, v, &f.at_offset(kf_num_dice_14), &f.at_offset(kf_die_18)); // 004F4C86..1A
                const I num_dice = f.at_offset(kf_num_dice_14), die = f.at_offset(kf_die_18);
                if (c.a5 == 1) {                                                    // 004F4D1F..23
                    sum = die * num_dice;                                           // 004F4D25..29
                } else if (critical(c)) {                                           // 004F4D30..38
                    I n = 0;                                                        // EBX
                    if (crit_multiplier(c) > 0) {                                   // 004F4D3A..51
                        do { sum += roll(num_dice, die); ++n; } while (n < crit_multiplier(c)); // 004F4D53..83 (re-read)
                    }
                } else {
                    sum = roll(num_dice, die);                                      // 004F4D87..9C
                }
                if (c.a4 != 1) {                                                    // 004F4D9F..A3
                    total_bonus += sum;                                             // 004F4ED6
                } else if (index != 0 && index != 1 && index != 2) {                // 004F4DA9..C1
                    if (c.target != nullptr) sum = reduce(c, sum, stacked_flags);   // 004F4DC7..01
                    if (c.a5 != 0) total_bonus += sum;                              // 004F4E03..08 -> 004F4ED6
                    else kotor_fastcall_004d20d0(c.attack, nullptr, static_cast<U>(bonus_types[k]) & 0xffffu,
                                                 static_cast<U>(sum), 1);           // 004F4E0E..20
                }
                added_or_skipped = true;
            }
            if (!added_or_skipped) {                                                // 004F4E2A: value used as the damage
                if (critical(c)) {                                                  // 004F4E2A..32
                    I n = 0;
                    if (crit_multiplier(c) > 0) {                                   // 004F4E34..47
                        do { sum += values[k]; ++n; } while (n < crit_multiplier(c)); // 004F4E50..6D
                    }
                } else {
                    sum = v;                                                        // 004F4E71
                }
                if (c.a4 != 1) {                                                    // 004F4E73..77
                    total_bonus += sum;                                             // 004F4ED6
                } else if (index != 0 && index != 1 && index != 2) {                // 004F4E79..89
                    if (c.target != nullptr) sum = reduce(c, sum, stacked_flags);   // 004F4E8B..C5
                    if (c.a5 == 0)
                        kotor_fastcall_004d20d0(c.attack, nullptr, static_cast<U>(bonus_types[k]) & 0xffffu,
                                                static_cast<U>(sum), 1);            // 004F4EC7..CC -> 004F4E0E
                    else
                        sum += sum;                                                 // 004F4ED2 (quirk: value dropped)
                }
            }
        }
        if (penalties[k] != -1) {                                                   // 004F4EDA..E6
            const I index = log2_index(penalty_types[k]);                           // 004F4EEC..1F -> [esp+30]
            const I v = penalties[k];                                               // 004F4F23
            if (v > 5) {                                                            // 004F4F2A..2D
                dice_of(table, v, &f.at_offset(kf_num_dice_14), &f.at_offset(kf_die_18)); // 004F4F33..D5
                const I num_dice = f.at_offset(kf_num_dice_14), die = f.at_offset(kf_die_18);
                if (c.a5 == 1) {                                                    // 004F4FDA..DE
                    sum = die * num_dice;                                           // 004F4FE0..E4
                } else if (critical(c)) {                                           // 004F4FEE..F6
                    I n = 0;                                                        // quirk: ESI keeps the bonus part's value
                    if (crit_multiplier(c) > 0) {                                   // 004F4FF8..0F
                        do { sum += roll(num_dice, die); ++n; } while (n < crit_multiplier(c)); // 004F5015..45
                    }
                } else {
                    sum = roll(num_dice, die);                                      // 004F5049..5E
                }
            } else if (critical(c)) {                                               // 004F5063..6B
                I n = 0;                                                            // quirk: ESI keeps the bonus part's value
                if (crit_multiplier(c) > 0) {                                       // 004F506D..80
                    do { sum += penalties[k]; ++n; } while (n < crit_multiplier(c)); // 004F5082..A3
                }
            } else {
                sum = v;                                                            // 004F50A7
            }
            if (c.a4 == 1) {                                                        // 004F50A9..AD
                if (index != 0 && index != 1 && index != 2)                         // 004F50AF..BF
                    kotor_fastcall_004d20d0(c.attack, nullptr, static_cast<U>(penalty_types[k]) & 0xffffu,
                                            static_cast<U>(-sum), 1);               // 004F50C1..D5
            } else {
                total_penalty += sum;                                               // 004F50DC
            }
        }
    }
    if (total_bonus > 0x24) total_bonus = 0x24;                                     // 004F50EF..F8
    if (total_penalty > 0x24) total_penalty = 0x24;                                 // 004F50FD..06
    return total_bonus - total_penalty;                                             // 004F510B
}

// ---------------------------------------------------------------------------------------------------------
// kind 3: saving throw (004F5124..552A, sum 004F5530, clamp 004F560C)
I save_bonus(const Context& c) {
    const I count = list_count(c);
    if (count > 0) {                                                                // 004F5124..29
        for (I i = at<U16>(c.stats, 0x15a); i < count; ++i) {                       // 004F512F..46 / 004F5518..2A
            void* e = list_effect(c, i);
            if (eff_type(e) > k_type_save_dec_007466e8) break;                      // 004F515B..65
            (void)eff_int(e, 0);                                                    // 004F516D..70 (unused)
            const I save = eff_int(e, 1);                                           // EDI
            const I save_type = eff_int(e, 2);                                      // [esp+24]
            const I racial = eff_int(e, 3);                                         // [esp+50]
            const I alignment = eff_int(e, 5);                                      // EAX
            if (save != 0 && save != static_cast<I>(c.a6 & 0xff)) continue;         // 004F51A3..AD
            if (save_type != 0 && save_type != static_cast<I>(c.a7 & 0xff)) continue; // 004F51B3..C1
            if (!racial_ok(c, racial) || !alignment_ok(c, alignment)) continue;     // 004F51C7..EE
            const I type = eff_type(e);
            if (type == k_type_save_inc_007466e4)                                   // 004F51F4..52D2
                stack_item_or_subtype(c.self, e, 0x568, 0x7a8, 0x688, 0, false);
            else if (type == k_type_save_dec_007466e8)                              // 004F5393..5511
                stack_item_or_subtype(c.self, e, 0x5f8, 0x838, 0x718, 0, false);
        }
    }
    I bonus = 0, penalty = 0;                                                       // 004F5534..36
    sum_arrays(c.self, bonus, penalty);                                             // 004F5538..606
    if (bonus > 0x14) bonus = 0x14;                                                 // 004F560C..11
    if (penalty > 0x14) penalty = 0x14;                                             // 004F5616..1B
    return bonus - penalty;
}

// kind 4: ability (004F5639..596D, sum 004F5973). The ability id (GetInteger 0) must equal the byte a9.
I ability_bonus(const Context& c) {
    const I count = list_count(c);
    if (count > 0) {                                                                // 004F5639..4C
        for (I i = at<U16>(c.stats, 0x144); i < count; ++i) {                       // 004F5652..65 / 004F5953..6D
            void* e = list_effect(c, i);
            if (eff_type(e) > k_type_ability_dec_00746710) break;                   // 004F567A..84
            const I ability = eff_int(e, 0);                                        // EDI
            (void)eff_int(e, 1);                                                    // 004F569B..A1 (unused)
            if (ability != static_cast<I>(c.a9 & 0xff)) continue;                   // 004F56A6..AC
            const I type = eff_type(e);
            if (type == k_type_ability_inc_0074670c)                                // 004F56B2..5820
                stack_item_or_subtype(c.self, e, 0x568, 0x7a8, 0x688, 1, false);
            else if (type == k_type_ability_dec_00746710)                           // 004F5825..5948 (accumulates)
                stack_item_or_subtype(c.self, e, 0x5f8, 0x838, 0x718, 1, true);
        }
    }
    I bonus = 0, penalty = 0;                                                       // EBX = [esp+50], EDI = [esp+34] (0)
    sum_arrays(c.self, bonus, penalty);                                             // 004F5973..A43
    if (bonus > 0x14) bonus = 0x14;                                                 // 004F5A49..4E
    if (penalty > 0x1e) penalty = 0x1e;                                             // 004F5A53..58
    return bonus - penalty;
}

// kind 5: skill (004F5A78..5E28, sum 004F5E2E). The skill id (GetInteger 0) must be FFh or the byte a8.
I skill_bonus(const Context& c) {
    const I count = list_count(c);
    if (count > 0) {                                                                // 004F5A78..89
        for (I i = at<U16>(c.stats, 0x158); i < count; ++i) {                       // 004F5A8F..A6 / 004F5E0E..28
            void* e = list_effect(c, i);
            if (eff_type(e) > k_type_skill_dec_0074675c) break;                     // 004F5ABB..C5
            const I skill = eff_int(e, 0);                                          // EDI
            (void)eff_int(e, 1);                                                    // 004F5ADC..E2 (unused)
            const I racial = eff_int(e, 2);                                         // EBX
            const I alignment = eff_int(e, 4);                                      // EAX
            if (skill != 0xff && skill != static_cast<I>(c.a8 & 0xff)) continue;    // 004F5AFB..09
            if (!racial_ok(c, racial) || !alignment_ok(c, alignment)) continue;     // 004F5B0F..32
            const I type = eff_type(e);
            if (type == k_type_skill_inc_00746758)                                  // 004F5B38..5CBA
                stack_item_or_subtype(c.self, e, 0x568, 0x7a8, 0x688, 1, false);
            else if (type == k_type_skill_dec_0074675c)                             // 004F5CBF..5E07
                stack_item_or_subtype(c.self, e, 0x5f8, 0x838, 0x718, 1, false);
        }
    }
    I bonus = 0, penalty = 0;                                                       // EDI = [esp+28], EDX = [esp+50] (0)
    sum_arrays(c.self, bonus, penalty);                                             // 004F5E2E..F03
    if (bonus > 0x1e) bonus = 0x1e;                                                 // 004F5F09..0E
    if (penalty > 0x1e) penalty = 0x1e;                                             // 004F5F13..18
    return bonus - penalty;
}
}  // namespace

extern "C" {
// 005A5110 CSWSCreatureStats::GetSimpleAlignmentGoodEvil: RET 0, AL. +17E good/evil (signed word):
// <= 40 -> 3 (evil), >= 60 -> 2 (good), otherwise 1 (neutral).
U __fastcall kotor_fastcall_005a5110(void* stats, void*) {
    const short value = at<short>(stats, 0x17e);                                    // 005A5110
    if (value <= 0x28) return 3;                                                    // 005A5117..1F (jg)
    return value >= 0x3c ? 2u : 1u;                                                 // 005A5120..27
}

// 004F3FE0 CSWSCreature::GetTotalEffectBonus(kind byte, CSWSObject* target, int* adjust, a4, a5, a6 byte save,
// a7 byte save type, a8 byte skill, a9 byte ability, a10 off-hand): RET 28h, EAX signed total
U __fastcall kotor_fastcall_004f3fe0(void* creature, void*, U kind, U target, U adjust, U a4, U a5, U a6, U a7,
                                     U a8, U a9, U a10) {
    Context c;
    c.self = creature;
    c.stats = at<void*>(creature, 0xa74);
    c.adjust = reinterpret_cast<I*>(static_cast<std::uintptr_t>(adjust));
    c.target = reinterpret_cast<void*>(static_cast<std::uintptr_t>(target));
    c.a4 = a4; c.a5 = a5; c.a6 = a6; c.a7 = a7; c.a8 = a8; c.a9 = a9; c.a10 = a10;
    *c.adjust = 0;                                                                  // 004F4001..0B
    c.list = static_cast<unsigned char*>(creature) + 0x124;                         // 004F4018 ([esp+40])
    c.attack = recovered_combat_round_attack_004d2b50(at<void*>(creature, 0x9c8), nullptr,
                                                       at<U8>(at<void*>(creature, 0x9c8), 0x96c)); // 004F400D..30
    c.hand = at<U8>(c.attack, 0x3a);                                                // 004F4034..3A
    if (c.hand == 0 || a5 != 0) c.hand = (a10 != 0 ? 1 : 0) + 1;                    // 004F4038..50
    if (c.target != nullptr && vcall<void*>(c.target, 0x30) != nullptr) {           // 004F4054..65
        c.racial = at<U16>(at<void*>(vcall<void*>(c.target, 0x30), 0xa74), 0xdc);   // 004F4067..7C
        c.alignment = static_cast<U8>(kotor_fastcall_005a5110(at<void*>(vcall<void*>(c.target, 0x30), 0xa74), nullptr)); // 004F4080..96
    } else {
        c.racial = -1; c.alignment = -1;                                            // 004F409C..A0
    }
    for (U i = 0; i < k_slots; ++i) {                                               // 004F40A4..F6
        at<I>(creature, 0x568 + i * 4) = -1;
        at<I>(creature, 0x5f8 + i * 4) = -1;
        at<I>(creature, 0x688 + i * 4) = -1;
        at<I>(creature, 0x718 + i * 4) = -1;
        at<U>(creature, 0x7a8 + i * 4) = k_object_invalid;
        at<U>(creature, 0x838 + i * 4) = k_object_invalid;
    }
    switch (kind & 0xff) {                                                          // 004F40F8..106 (table 004F5F54)
    case 1: return static_cast<U>(attack_bonus(c));
    case 2: return static_cast<U>(damage_bonus(c));
    case 3: return static_cast<U>(save_bonus(c));
    case 4: return static_cast<U>(ability_bonus(c));
    case 5: return static_cast<U>(skill_bonus(c));
    default: return 0;                                                              // 004F5F38..4E
    }
}
}  // extern "C"
