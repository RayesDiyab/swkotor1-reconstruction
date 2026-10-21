// Lane effects-itemprop, part C: CSWSItemPropertyHandler Remove* callbacks 004E7F40..004E9110, the Apply callbacks
// 004E9230 / 004E92E0 (changed saving throw) and their direct helper 004E67D0, reconstructed from the original
// swkotor.exe (read-only, capstone linear sweep).  Addresses in the comments are original instructions.
// ABI and data layout: see claude_itemprop_common.hpp.  Each callback's RET and stack offsets were re-derived:
//   Remove*  thiscall(item, property, creature, slot) RET 10h            -> @24
//   Apply*   thiscall(item, property, creature, slot, load_game) RET 14h -> @28
//   004E67D0 thiscall(item, value, property, creature, load_game) RET 14h -> @28 (called directly, not via a table)
// Creature fields used by the Remove scans: +A74 CSWSCreatureStats* whose WORDs +12E/+130/+136/+142/+144 hold the
// first applied-effect index to scan per effect family; +124/+128 applied-effect list data/count (signed loop).
// Quirks kept:
//  * the "Value" cost cell is never initialised before C2DA::GetINTEntry; a bad row leaves it indeterminate;
//  * 004E7F40 / 004E83A0: after removing an attack-family match the scan restarts at the family start index without
//    incrementing (several matches can be removed); a damage-family match removes one effect and stops;
//  * 004E86F0 shifts 1 by the (hardware-masked, & 31) param/subtype;
//  * 004E67D0 never checks operator new (null effect dereference) and applies with (load_game, 0).
#include "9bb88ebc0b-claude_itemprop_common.hpp"

extern "C" {
unsigned __fastcall kotor_fastcall_004e67d0(void* self, void*, void* item, int value, void* property, void* creature,
                                            unsigned load_game);
unsigned __fastcall kotor_fastcall_004e7f40(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e83a0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e86f0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e8950(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e8bc0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e8db0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e8f70(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9110(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9230(void* self, void*, void* item, void* property, void* creature, unsigned slot,
                                            unsigned load_game);
unsigned __fastcall kotor_fastcall_004e92e0(void* self, void*, void* item, void* property, void* creature, unsigned slot,
                                            unsigned load_game);
}

using namespace ipx;

namespace {
// ---- .rdata effect type constants (DWORD compares / WORD stores) ----
constexpr I k_eff_attack_inc_007466a4 = 0x0a;
constexpr I k_eff_attack_dec_007466a8 = 0x0b;
constexpr I k_eff_damage_inc_007466b0 = 0x0d;
constexpr I k_eff_damage_dec_007466b4 = 0x0e;
constexpr U16 k_eff_save_inc_007466e4 = 0x1a;
constexpr U16 k_eff_save_dec_007466e8 = 0x1b;
constexpr I k_eff_force_resist_inc_00746700 = 0x21;
constexpr I k_eff_ability_inc_0074670c = 0x24;
constexpr I k_eff_ac_inc_0074673c = 0x30;
// ---- slot -> attack / damage hand constants: slots 10h, 20h, 4000h, 8000h, 10000h, 8 ----
constexpr I k_attack_hand_007468c8[6] = {1, 2, 3, 4, 5, 7};   // .rdata 007468C8 / CC / D0 / D4 / D8 / DC
constexpr I k_damage_hand_007468e4[6] = {1, 2, 3, 4, 5, 7};   // .rdata 007468E4 / E8 / EC / F0 / F4 / F8
// ---- ability constants 00746838..0074684C ----
constexpr I k_ability_00746838[6] = {0, 1, 2, 3, 4, 5};
// ---- IPRP cost table indices ----
constexpr U k_cost_bonus = 1, k_cost_force_resist = 0x0b, k_cost_penalty = 0x14;

inline I effect_count(void* creature) { return at<I>(creature, 0x128); }
inline void* effect_at(void* creature, I i) { return at<void**>(creature, 0x124)[i]; }
inline I start_index(void* creature, U offset) { return at<U16>(at<void*>(creature, 0xa74), offset); }
inline I eff_type(void* e) { return at<U16>(e, 8); }
inline I geti(void* e, U index) { return static_cast<I>(get_int(e, index)); }
// (byte [e+0A] & 7) == WORD [007473BC]; [e+18] == item+14
inline bool equipped_dur(void* e) { return duration_bits(e) == k_duration_equipped_007473bc; }
inline bool created_by(void* e, void* item) { return at<U>(e, 0x18) == at<U>(item, 0x14); }

// cmp slot,10h/20h/4000h/8000h/10000h/8 -> GetInteger(index) == table[k]; any other slot: no match
bool slot_match(void* e, U index, U slot, const I (&v)[6]) {
    int k;
    switch (slot) {
    case 0x10: k = 0; break;
    case 0x20: k = 1; break;
    case 0x4000: k = 2; break;
    case 0x8000: k = 3; break;
    case 0x10000: k = 4; break;
    case 8: k = 5; break;
    default: return false;
    }
    return geti(e, index) == v[k];
}

// Racial / alignment test shared by the bonus scans (e.g. 004E80E4..818B for a=5, b=6, c=7):
//   type a or b: GetInteger(2) must equal [[7A3A28]+AA]; type b then checks subtype 1 -> GetInteger(3)==1,
//   2 -> GetInteger(4)==2, 3 -> GetInteger(4)==3, other -> match; type c: GetInteger(2) == subtype; other: match.
bool race_match(void* e, void* property, U16 a, U16 b, U16 c) {
    U16 t = prop_type(property);
    if (t == a || t == b) {
        const I race = static_cast<I>(racial_all());
        if (geti(e, 2) != race) return false;
        t = prop_type(property);
        if (t == b) {
            const U16 s = prop_subtype(property);
            if (s == 1) return geti(e, 3) == 1;
            if (s == 2) return geti(e, 4) == 2;
            if (s == 3) return geti(e, 4) == 3;
            return true;
        }
    }
    if (t == c) {
        const I s = prop_subtype(property);
        return geti(e, 2) == s;
    }
    return true;
}

inline void* cost_table(U index) { return kotor_fastcall_005c49a0(two_dim(), nullptr, index); }
}  // namespace

extern "C" {

// ===================================================================================================== 004E67D0
// CSWSItemPropertyHandler::ApplyChangedSavingThrow(item, value, property, creature, load_game)
unsigned __fastcall kotor_fastcall_004e67d0(void*, void*, void* item, int value, void* property, void* creature,
                                            unsigned load_game) {
    void* e = new_effect(1);                                                              // 004E67E8..12
    I amount = value;                                                                     // 004E6814
    if (amount >= 0) {                                                                    // 004E6818..22 (jl)
        at<U16>(e, 8) = k_eff_save_inc_007466e4;                                          // 004E6824..2A
    } else {
        at<U16>(e, 8) = k_eff_save_dec_007466e8;                                          // 004E6830..37
        amount = -amount;                                                                 // 004E683B
    }
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_duration_equipped_007473bc); // 004E6841..54
    set_creator(e, item);                                                                 // 004E6858..5E
    switch (prop_subtype(property)) {                                                     // 004E6863..72
    case 1: set_int(e, 1, 1); break;                                                      // 004E687C..82
    case 2: set_int(e, 1, 2); break;                                                      // 004E6878
    case 3: set_int(e, 1, 3); break;                                                      // 004E6874
    default: break;
    }
    set_int(e, 0, static_cast<U>(amount));                                                // 004E6887..8C
    set_int(e, 2, 0);                                                                     // 004E6891..97
    set_int(e, 3, racial_all());                                                          // 004E689C..AD
    apply(creature, e, load_game, 0);                                                     // 004E68B2..BE
    return 0;
}

// ===================================================================================================== 004E7F40
// CSWSItemPropertyHandler::RemoveEnhancementBonus
unsigned __fastcall kotor_fastcall_004e7f40(void*, void*, void* item, void* property, void* creature, unsigned slot) {
    I value;                                                                              // [esp+8] uninitialised
    cost_int(at<void*>(two_dim(), 0x30), prop_cost(property), &value);                    // 004E7F5A..A6
    I i = start_index(creature, 0x130);                                                   // 004E7FAB..B5
    if (i >= effect_count(creature)) return 0;                                            // 004E7FBC..C2
    for (;;) {
        void* e = effect_at(creature, i);                                                 // 004E7FD0..D6
        if (eff_type(e) > k_eff_damage_inc_007466b0) break;                               // 004E7FD9..E3
        if (eff_type(e) == k_eff_attack_inc_007466a4 && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == value) {                                                        // 004E7FE9..802B
            if (slot_match(e, 1, slot, k_attack_hand_007468c8) && race_match(e, property, 5, 6, 7)) { // 004E8031..8185
                remove_by_id(creature, e);                                                // 004E818B..98
                i = start_index(creature, 0x130);                                         // 004E819D..AA (no increment)
                if (i >= effect_count(creature)) break;                                   // 004E835A..64
                continue;
            }
        } else if (eff_type(e) == k_eff_damage_inc_007466b0 && equipped_dur(e) && created_by(e, item) &&
                   geti(e, 0) == value && slot_match(e, 5, slot, k_damage_hand_007468e4)) { // 004E81AF..82A4
            const U flags = get_int(e, 1);                                                // 004E82AA..B5
            if ((kotor_fastcall_00554050(item, nullptr) & flags) != 0 &&                  // 004E82B9..C4
                race_match(e, property, 5, 6, 7)) {                                       // 004E82CA..8357
                remove_by_id(creature, e);                                                // 004E836C..77
                break;
            }
        }
        ++i;                                                                              // 004E8359
        if (i >= effect_count(creature)) break;                                           // 004E835A..64
    }
    return 0;
}

// ===================================================================================================== 004E83A0
// CSWSItemPropertyHandler::RemoveAttackPenalty (attack + damage penalty scan)
unsigned __fastcall kotor_fastcall_004e83a0(void*, void*, void* item, void* property, void* creature, unsigned slot) {
    I value;                                                                              // [esp+4] uninitialised
    cost_int(cost_table(k_cost_penalty), prop_cost(property), &value);                    // 004E83B9..040B
    value = -value;                                                                       // 004E8410..20
    I i = start_index(creature, 0x130);                                                   // 004E8414..24
    if (i >= effect_count(creature)) return 0;                                            // 004E842B..31
    for (;;) {
        void* e = effect_at(creature, i);                                                 // 004E8440..46
        if (eff_type(e) > k_eff_damage_dec_007466b4) break;                               // 004E8449..53
        if (eff_type(e) == k_eff_attack_dec_007466a8 && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == value && geti(e, 2) == static_cast<I>(racial_all())) {          // 004E8459..BA
            if (slot_match(e, 1, slot, k_attack_hand_007468c8)) {                         // 004E84C0..856C
                remove_by_id(creature, e);                                                // 004E8572..7F
                i = start_index(creature, 0x130);                                         // 004E8584..91 (no increment)
                if (i >= effect_count(creature)) break;                                   // 004E86B7..C1
                continue;
            }
        } else if (eff_type(e) == k_eff_damage_dec_007466b4 && equipped_dur(e) && created_by(e, item) &&
                   geti(e, 0) == value && geti(e, 2) == static_cast<I>(racial_all()) &&
                   slot_match(e, 5, slot, k_damage_hand_007468e4)) {                      // 004E8596..869C
            const U flags = get_int(e, 1);                                                // 004E869E..A7
            if ((kotor_fastcall_00554050(item, nullptr) & flags) != 0) {                  // 004E86A7..B4
                remove_by_id(creature, e);                                                // 004E86C9..D4
                break;
            }
        }
        ++i;                                                                              // 004E86B6
        if (i >= effect_count(creature)) break;                                           // 004E86B7..C1
    }
    return 0;
}

// ===================================================================================================== 004E86F0
// CSWSItemPropertyHandler::RemoveDamageBonus
unsigned __fastcall kotor_fastcall_004e86f0(void*, void*, void* item, void* property, void* creature, unsigned slot) {
    const I cost = prop_cost(property);                                                   // 004E86FD..01
    const U shift = prop_type(property) == 0xb ? prop_subtype(property)                   // 004E86F9..0F
                                               : static_cast<U>(at<U8>(property, 9));     // 004E8711..15
    I i = start_index(creature, 0x136);                                                   // 004E8719..23
    if (i >= effect_count(creature)) return 0;                                            // 004E872A..30
    do {
        void* e = effect_at(creature, i);                                                 // 004E8740..46
        if (eff_type(e) > k_eff_damage_inc_007466b0) break;                               // 004E8749..54
        if (eff_type(e) == k_eff_damage_inc_007466b0 && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == cost && slot_match(e, 5, slot, k_damage_hand_007468e4) &&       // 004E875A..884B
            geti(e, 1) == static_cast<I>(1u << (shift & 31)) &&                           // 004E8851..67
            race_match(e, property, 0xb, 0xc, 0xd)) {                                     // 004E886D..8920
            remove_by_id(creature, e);                                                    // 004E88C0..CB
            break;
        }
        ++i;                                                                              // 004E8926..30
    } while (i < effect_count(creature));                                                 // 004E8931..33
    return 0;
}

// ===================================================================================================== 004E8950
// CSWSItemPropertyHandler::RemoveAttackBonus
unsigned __fastcall kotor_fastcall_004e8950(void*, void*, void* item, void* property, void* creature, unsigned slot) {
    I value;                                                                              // [esp+8] uninitialised
    cost_int(at<void*>(two_dim(), 0x30), prop_cost(property), &value);                    // 004E896A..B6
    I i = start_index(creature, 0x130);                                                   // 004E89BB..C5
    if (i >= effect_count(creature)) return 0;                                            // 004E89CC..D2
    do {
        void* e = effect_at(creature, i);                                                 // 004E89E0..E6
        if (eff_type(e) > k_eff_attack_inc_007466a4) break;                               // 004E89E9..F4
        if (eff_type(e) == k_eff_attack_inc_007466a4 && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == value && slot_match(e, 1, slot, k_attack_hand_007468c8) &&      // 004E89FA..8AEB
            race_match(e, property, 0x26, 0x27, 0x28)) {                                  // 004E8AF1..8B7E
            remove_by_id(creature, e);                                                    // 004E8B95..A0
            break;
        }
        ++i;                                                                              // 004E8B80..8A
    } while (i < effect_count(creature));                                                 // 004E8B8B..8D
    return 0;
}

// ===================================================================================================== 004E8BC0
// CSWSItemPropertyHandler::RemoveAttackPenalty (attack penalty only)
unsigned __fastcall kotor_fastcall_004e8bc0(void*, void*, void* item, void* property, void* creature, unsigned slot) {
    I value;                                                                              // [esp+4] uninitialised
    cost_int(cost_table(k_cost_penalty), prop_cost(property), &value);                    // 004E8BD9..2B
    value = -value;                                                                       // 004E8C30..40
    I i = start_index(creature, 0x130);                                                   // 004E8C34..44
    if (i >= effect_count(creature)) return 0;                                            // 004E8C4B..51
    do {
        void* e = effect_at(creature, i);                                                 // 004E8C60..66
        if (eff_type(e) > k_eff_damage_dec_007466b4) break;                               // 004E8C69..73
        if (eff_type(e) == k_eff_attack_dec_007466a8 && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == value && geti(e, 2) == static_cast<I>(racial_all()) &&          // 004E8C79..DA
            slot_match(e, 1, slot, k_attack_hand_007468c8)) {                             // 004E8CE0..6D
            remove_by_id(creature, e);                                                    // 004E8D84..8F
            break;
        }
        ++i;                                                                              // 004E8D6F..79
    } while (i < effect_count(creature));                                                 // 004E8D7A..7C
    return 0;
}

// ===================================================================================================== 004E8DB0
// CSWSItemPropertyHandler::RemoveAbilityBonus
unsigned __fastcall kotor_fastcall_004e8db0(void*, void*, void* item, void* property, void* creature, unsigned) {
    I value;                                                                              // [esp+0C] uninitialised
    cost_int(cost_table(k_cost_bonus), prop_cost(property), &value);                      // 004E8DCB..1D
    I i = start_index(creature, 0x144);                                                   // 004E8E22..2C
    if (i >= effect_count(creature)) return 0;                                            // 004E8E33..39
    do {
        void* e = effect_at(creature, i);                                                 // 004E8E40..46
        if (eff_type(e) > k_eff_ability_inc_0074670c) break;                              // 004E8E49..54
        if (eff_type(e) == k_eff_ability_inc_0074670c && equipped_dur(e) && created_by(e, item) &&
            geti(e, 1) == value) {                                                        // 004E8E5A..98
            const U s = prop_subtype(property);                                           // 004E8E9A
            if (s > 5 || geti(e, 0) == k_ability_00746838[s]) {                           // 004E8E9E..8F14 (jump table 004E8F4C)
                remove_by_id(creature, e);                                                // 004E8F27..30
                break;
            }
        }
        ++i;                                                                              // 004E8F16..1C
    } while (i < effect_count(creature));                                                 // 004E8F1D..1F
    return 0;
}

// ===================================================================================================== 004E8F70
// CSWSItemPropertyHandler::RemoveACBonus
unsigned __fastcall kotor_fastcall_004e8f70(void*, void*, void* item, void* property, void* creature, unsigned) {
    I value;                                                                              // [esp+8] uninitialised
    cost_int(cost_table(k_cost_bonus), prop_cost(property), &value);                      // 004E8F8A..DC
    const I ac_type = at<U8>(base_item(item), 0x8b);                                      // 004E8FE1..FE (stored over arg 2)
    I i = start_index(creature, 0x12e);                                                   // 004E9002..0C
    if (i >= effect_count(creature)) return 0;                                            // 004E9013..19
    do {
        void* e = effect_at(creature, i);                                                 // 004E9021..27
        if (eff_type(e) > k_eff_ac_inc_0074673c) break;                                   // 004E902A..35
        if (eff_type(e) == k_eff_ac_inc_0074673c && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == ac_type && geti(e, 1) == value) {                               // 004E903B..80
            bool match;
            const U16 t = prop_type(property);                                            // 004E9082
            const I s = prop_subtype(property);
            if (t == 2) {                                                                 // 004E9086..9E
                match = (s <= 0 || s > 3) ? true : geti(e, 4) == s;
            } else if (t == 4) {                                                          // 004E90A0..B8
                match = geti(e, 2) == s;
            } else if (t == 3) {                                                          // 004E90BA..D2
                match = geti(e, 5) == s;
            } else {
                match = true;                                                             // 004E90BE -> 004E90E9
            }
            if (match) {
                remove_by_id(creature, e);                                                // 004E90E9..F4
                break;
            }
        }
        ++i;                                                                              // 004E90D4..DE
    } while (i < effect_count(creature));                                                 // 004E90DF..E1
    return 0;
}

// ===================================================================================================== 004E9110
// CSWSItemPropertyHandler::RemoveImprovedForceResistance
unsigned __fastcall kotor_fastcall_004e9110(void*, void*, void* item, void* property, void* creature, unsigned) {
    I value;                                                                              // [esp+8] uninitialised
    cost_int(cost_table(k_cost_force_resist), prop_cost(property), &value);               // 004E912A..7C
    I i = start_index(creature, 0x142);                                                   // 004E9181..8B
    if (i >= effect_count(creature)) return 0;                                            // 004E9192..98
    do {
        void* e = effect_at(creature, i);                                                 // 004E91B0..B6
        if (eff_type(e) > k_eff_force_resist_inc_00746700) break;                         // 004E91B9..BF
        if (eff_type(e) == k_eff_force_resist_inc_00746700 && equipped_dur(e) && created_by(e, item) &&
            geti(e, 0) == value) {                                                        // 004E91C1..EF
            remove_by_id(creature, e);                                                    // 004E9203..0C
            break;
        }
        ++i;                                                                              // 004E91F6..FC
    } while (i < effect_count(creature));                                                 // 004E91FD..FF
    return 0;
}

// ===================================================================================================== 004E9230
// CSWSItemPropertyHandler::ApplyImprovedSavingThrow: cost table 1 "Value"; nonzero -> ApplyChangedSavingThrow
unsigned __fastcall kotor_fastcall_004e9230(void* self, void*, void* item, void* property, void* creature, unsigned,
                                            unsigned load_game) {
    I value;                                                                              // [esp+8] uninitialised
    cost_int(cost_table(k_cost_bonus), prop_cost(property), &value);                      // 004E924C..9E
    if (value != 0)                                                                       // 004E92A3..A9
        kotor_fastcall_004e67d0(self, nullptr, item, value, property, creature, load_game); // 004E92AB..BE (ECX = this)
    return 0;
}

// ===================================================================================================== 004E92E0
// CSWSItemPropertyHandler::ApplyReducedSavingThrows: cost table 14h "Value" (not negated here; the helper picks
// the decrease type from the sign); nonzero -> ApplyChangedSavingThrow
unsigned __fastcall kotor_fastcall_004e92e0(void* self, void*, void* item, void* property, void* creature, unsigned,
                                            unsigned load_game) {
    I value;                                                                              // [esp+8] uninitialised
    cost_int(cost_table(k_cost_penalty), prop_cost(property), &value);                    // 004E92FC..4E
    if (value != 0)                                                                       // 004E9353..59
        kotor_fastcall_004e67d0(self, nullptr, item, value, property, creature, load_game); // 004E935B..6E (ECX = this)
    return 0;
}

}  // extern "C"
