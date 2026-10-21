// W5 combat-attack lane, part C2 (damage statistics) of the AIActionPhysicalAttacks 005BBBF0 cascade:
//   005A9050 CSWSCreatureStats::GetDamageRoll            005A8BE0 CSWSCreatureStats::GetDamageBonus
//   005A4FB0 CSWSCreatureStats::ResolveSpecialAttackDamageBonus
//   005A4D40 / 005A4D60 / 005A4D90 CSWSCreatureStats::GetUnarmedDamageDice / Die / Roll
//   00550D10 CSWRules::GetWeightedDamageAmount           004FD6F0 CSWSCreature::CalculateDamagePower
//   004ED5E0 CSWSCreature::GetDamageFlags (-> tail 00554050 CSWSItem::GetDamageFlags)
//   004AE9F0 CServerExoApp::GetDifficultyOption (-> tail 004B1DF0 CServerExoAppInternal::GetDifficultyOption
//            -> 00550C30 CSWRules::GetDifficultyOption)
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); no x87 in this part;
// game heap only through the selected CExoString providers. No traps are introduced by this file.
// Stack-frame names: E = ESP after the last prologue push; Lxx = [E+xx] (locals), argN = the Nth stack argument.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_reconstruction { struct StartupString; }
// 005E5D10 CExoString::operator+(const CExoString&) -> hidden return [0093-ini-string-priority]
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs);

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);             // CSWSCombatRound::GetAttack [w1 b7]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                       // CSWItem::GetBaseItem [0820]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id);   // CSWBaseItemArray::GetBaseItem [0820]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);            // CSWRules::RollDice [w1 b7]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                       // CSWSCreatureStats::HasFeat [0963]
U __fastcall recovered_item_property_has_00555c90(void* item, void*, U type, U subtype);          // GetPropertyByTypeExists [1025]
U __fastcall recovered_item_property_query_005539c0(void* item, void*, void** out, U type, U subtype); // GetPropertyByType [0886]
U __fastcall kotor_fastcall_004f3fe0(void* creature, void*, U, U, U, U, U, U, U, U, U, U); // GetTotalEffectBonus [effectbonus part; supersedes the partial W2 providers]
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                                 // return_zero [0832]
int __cdecl crt_rand_006fc447();                                                                  // _rand [0479]
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                              // CExoString(const CExoString&) [0078]
void __cdecl frame_LogFormat_005e5680(void* string, const char* format, ...);                      // CExoString::Format [0125]
// 005E5C50 CExoString::operator= : async_assign_string_005e5c50 (declared in w5_action_queue.hpp) [0199]
extern unsigned recovered_debug_cell_008327e4;                                                    // .bss 008327E4 debug-combat flag [w1 b7]

// ---------------- sibling parts of this lane (claude_combat_attack_*.cpp) ----------------
void* __fastcall kotor_fastcall_004d4ff0(void* round, void*, U flag);                 // CSWSCombatRound::GetCurrentAttackWeapon [round]
U __fastcall kotor_fastcall_004d3da0(void* round, void*);                             // CSWSCombatRound::GetWeaponAttackType [round]
void __fastcall kotor_fastcall_004d20d0(void* attack, void*, U flags, U amount, U a3);  // CSWSCombatAttackData::AddDamage [round]
U __fastcall kotor_fastcall_005a52b0(void* stats, void*, U offhand);                  // CSWSCreatureStats::GetCriticalHitMultiplier [attackstats]

// ---------------- exports of this part ----------------
U __fastcall kotor_fastcall_00550c30(void* rules, void*, U option, U level);
U __fastcall kotor_fastcall_004b1df0(void* internal, void*, U option);
U __fastcall kotor_fastcall_004ae9f0(void* server, void*, U option);
U __fastcall kotor_fastcall_00550d10(void* rules, void*, U roll, U max, U attacker_pc, U defender_pc, U percent);
U __fastcall kotor_fastcall_00554050(void* item, void*);
U __fastcall kotor_fastcall_004ed5e0(void* creature, void*);
U __fastcall kotor_fastcall_004fd6f0(void* creature, void*, U target);
U __fastcall kotor_fastcall_005a4d40(void* stats, void*);
U __fastcall kotor_fastcall_005a4d60(void* stats, void*);
U __fastcall kotor_fastcall_005a4d90(void* stats, void*, void* target);
U __fastcall kotor_fastcall_005a4fb0(void* stats, void*, U unused);
U __fastcall kotor_fastcall_005a8be0(void* stats, void*, void* target, U offhand, U unused);
U __fastcall kotor_fastcall_005a9050(void* stats, void*, void* target, U offhand, U critical, U sneak, U maximum, U record);
}

namespace {
constexpr U k_default_damage_flags_00746850 = 1;   // .rdata 00746850
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* creature_of(void* stats) { return at<void*>(stats, 0x24); }          // CSWSCreatureStats +24
inline void* round_of(void* creature) { return at<void*>(creature, 0x9c8); }      // CSWSCreature +9C8
inline void* attack_at(void* creature, U index) {                                  // GetAttack(index) on the creature's round
    return recovered_combat_round_attack_004d2b50(round_of(creature), nullptr, index);
}
inline void* base_item_of(U id) {                                                  // [rules+34] CSWBaseItemArray
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr, static_cast<int>(id));
}
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }               // vtbl+30 AsSWSCreature
// CExoString local (8 bytes: text pointer, length)
struct alignas(4) Str { unsigned char b[8]; void* p() { return b; } };
static_assert(sizeof(Str) == 8, "CExoString is 8 bytes");
inline kotor_reconstruction::StartupString* ss(void* p) { return static_cast<kotor_reconstruction::StartupString*>(p); }

// The debug-combat string append used by both functions: copy = attack+80 (CExoString(const&)), text = CExoString(),
// <format into text>, attack+80 = copy + text, then destroy (sum, text, copy) in that order.
template<class F> void append_debug(void* creature, U index, F format) {
    Str copy, text, sum;
    WindowCopy005e5b00(copy.p(), nullptr, static_cast<unsigned char*>(attack_at(creature, index)) + 0x80);
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(text.p()), nullptr);
    format(text.p());
    void* result = GuiConcat005e5d10(ss(copy.p()), nullptr, ss(sum.p()), ss(text.p()));
    async_assign_string_005e5c50(static_cast<unsigned char*>(attack_at(creature, index)) + 0x80, nullptr, result);
    frame_LogStringDtor_005e5c20(sum.p(), nullptr);
    frame_LogStringDtor_005e5c20(text.p(), nullptr);
    frame_LogStringDtor_005e5c20(copy.p(), nullptr);
}
// C2DA::GetINTEntry(row, CExoString(column), &out) on [[rules+B8]+table], column destroyed after the lookup
void table_int(U table, U row, const char* column_text, I* out) {
    Str column;
    frame_LogStringCtor_005e5a90(column.p(), nullptr, column_text);
    c2da_current_get_int_00413660(at<void*>(at<void*>(rules(), 0xb8), table), nullptr, row, column.p(), out);
    frame_LogStringDtor_005e5c20(column.p(), nullptr);
}
const char k_numdice_00747038[] = "NumDice";   // .rdata 00747038
const char k_die_00747034[] = "Die";           // .rdata 00747034
}  // namespace

extern "C" {
// =========================================================================================================
// 00550C30 CSWRules::GetDifficultyOption(option, level): RET 8. byte [rules + BC + level*6 + option]
// (coordinator ruling: this lane owns 00550C30 / 004B1DF0 / 004AE9F0; combat-spell declares them)
U __fastcall kotor_fastcall_00550c30(void* rules_, void*, U option, U level) {
    return at<U8>(rules_, 0xbc + level * 6 + option);                              // 00550C30..46
}

// 004B1DF0 CServerExoAppInternal::GetDifficultyOption(option): RET 4. Level = [[internal+10004]+108].
U __fastcall kotor_fastcall_004b1df0(void* internal, void*, U option) {
    const U level = at<U>(at<void*>(internal, 0x10004), 0x108);                    // 004B1DF0..F6
    return kotor_fastcall_00550c30(rules(), nullptr, option, level);               // 004B1DFC..0D
}

// 004AE9F0 CServerExoApp::GetDifficultyOption(option): RET 4 (tail jump into the internal object)
U __fastcall kotor_fastcall_004ae9f0(void* server, void*, U option) {
    return kotor_fastcall_004b1df0(at<void*>(server, 4), nullptr, option);         // 004AE9F0..F3
}

// ---------------------------------------------------------------------------------------------------------
// 00550D10 CSWRules::GetWeightedDamageAmount(roll, max, attacker PC flag, defender PC flag, percent): RET 14h.
// Divisions by 100 are the original's signed magic-number divisions (truncate toward zero); products are 32-bit.
U __fastcall kotor_fastcall_00550d10(void*, void*, U roll, U max, U attacker_pc, U defender_pc, U percent) {
    const I r = static_cast<I>(roll), m = static_cast<I>(max), pct = static_cast<I>(percent);
    if (attacker_pc == 1) {                                                         // 00550D10..1D
        if (pct == 0) return roll;                                                  // 00550D1F..25 -> 00550DF2
        const I floor = static_cast<I>(static_cast<U>(m) * static_cast<U>(pct)) / 100; // 00550D2B..43
        if (floor == m) return max;                                                 // 00550D45..47 -> 00550D7A
        const I spread = crt_rand_006fc447() % (100 - pct);                         // 00550D49..56 (idiv)
        const I v = static_cast<I>(static_cast<U>(spread + pct) * static_cast<U>(m)) / 100 + r; // 00550D58..72
        if (v < m) return static_cast<U>(v);                                        // 00550D74..78 (jl)
        return max;                                                                 // 00550D7A..7E
    }
    if (defender_pc != 1) return roll;                                              // 00550D81..85
    if (pct == 0) return roll;                                                      // 00550D87..8D
    if (pct < 100) {                                                                // 00550D8F..92 (jge)
        const I v = r - static_cast<I>(static_cast<U>(100 - pct) * static_cast<U>(m)) / 100; // 00550D94..B5
        return v >= 1 ? static_cast<U>(v) : 1u;                                     // 00550DB7..BF
    }
    if (pct == 100) return roll;                                                    // 00550DC2 (je)
    const I spread = crt_rand_006fc447() % (pct - 100);                             // 00550DC4..CD
    return static_cast<U>(static_cast<I>(static_cast<U>(spread) * static_cast<U>(m)) / 100 + r); // 00550DCF..EF
}

// ---------------------------------------------------------------------------------------------------------
// 00554050 CSWSItem::GetDamageFlags: base item +C, 8 when the base item is missing or its flags are 0. RET 0.
U __fastcall kotor_fastcall_00554050(void* item, void*) {
    void* base = base_item_of(at<U>(item, 0xc));                                    // 00554050..5C
    if (base == nullptr) return 8;                                                  // 00554061..63 -> 0055406C
    const U flags = at<U>(base, 0xc);                                               // 00554065
    return flags != 0 ? flags : 8u;                                                 // 00554068..71
}

// 004ED5E0 CSWSCreature::GetDamageFlags: the current attack's weapon damage flags, else [00746850]. RET 0.
U __fastcall kotor_fastcall_004ed5e0(void* creature, void*) {
    void* attack = attack_at(creature, at<U8>(round_of(creature), 0x96c));          // 004ED5E3..F1
    void* weapon = kotor_fastcall_004d4ff0(round_of(creature), nullptr, at<U8>(attack, 0x3a)); // 004ED5F6..01
    if (weapon != nullptr) return kotor_fastcall_00554050(weapon, nullptr);        // 004ED606..0D (tail jump)
    return k_default_damage_flags_00746850;                                         // 004ED612
}

// 004FD6F0 CSWSCreature::CalculateDamagePower(target): RET 4.
//   GetTotalEffectBonus(1, target, &local, 0, 0, 0, 0, FF, FF, 0) with local = 0
U __fastcall kotor_fastcall_004fd6f0(void* creature, void*, U target) {
    U local = 0;                                                                    // [esp] (004FD711)
    return kotor_fastcall_004f3fe0(creature, nullptr, 1, target,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(&local)), 0, 0, 0, 0, 0xff, 0xff, 0); // 004FD6F1..19
}

// ---------------------------------------------------------------------------------------------------------
// 005A4D40 CSWSCreatureStats::GetUnarmedDamageDice: AL = 1. The creature +4F8 read and the return_zero call
// (with the creature as this) are kept; neither affects the result. RET 0.
U __fastcall kotor_fastcall_005a4d40(void* stats, void*) {
    volatile U level = at<U>(creature_of(stats), 0x4f8);                            // 005A4D41..4A ([esp])
    (void)level;
    (void)kotor_m15e_slot010_zero_0063e7f0(creature_of(stats), nullptr);            // 005A4D4E
    return 1;                                                                       // 005A4D53 (mov al,1: EAX = 1)
}

// 005A4D60 CSWSCreatureStats::GetUnarmedDamageDie: return_zero(creature) == 1 ? (+4F8 > 2 ? 6 : 4)
//   : (+4F8 <= 2 ? 2 : 1). RET 0.
U __fastcall kotor_fastcall_005a4d60(void* stats, void*) {
    const I level = at<I>(creature_of(stats), 0x4f8);                               // 005A4D60..64
    if (kotor_m15e_slot010_zero_0063e7f0(creature_of(stats), nullptr) == 1)         // 005A4D6A..72
        return level > 2 ? 6u : 4u;                                                 // 005A4D74..81 (setg, signed)
    return level <= 2 ? 2u : 1u;                                                    // 005A4D82..8C (setle, signed)
}

// 005A4D90 CSWSCreatureStats::GetUnarmedDamageRoll(target object): RET 4.
//   RollDice(1, die) weighted by GetWeightedDamageAmount(roll, die, pc attacker vs npc, npc attacker vs pc, diff).
U __fastcall kotor_fastcall_005a4d90(void* stats, void*, void* target) {
    const I level = at<I>(creature_of(stats), 0x4f8);                               // 005A4D99..9C (ESI)
    U percent = 0x64;                                                               // [esp+10] (005A4DA2)
    U die;                                                                          // EBX
    if (kotor_m15e_slot010_zero_0063e7f0(creature_of(stats), nullptr) == 1)         // 005A4DAA..B2
        die = level > 2 ? 6u : 4u;                                                  // 005A4DB4..C0
    else
        die = level <= 2 ? 2u : 1u;                                                 // 005A4DC2..CA
    {
        volatile U level_again = at<U>(creature_of(stats), 0x4f8);                  // 005A4DCB..D7 ([esp+18], overwritten)
        (void)level_again;
    }
    (void)kotor_m15e_slot010_zero_0063e7f0(creature_of(stats), nullptr);            // 005A4DDB (result unused)
    const U roll = recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, die) & 0xffffu; // 005A4DE0..F2 (movzx ax)
    U defender_pc = 0;                                                              // EBP
    U attacker_pc = 0;                                                              // [esp+14]
    if (target != nullptr && as_creature(target) != nullptr) {                      // 005A4DF7..0C
        if (at<U>(stats, 0x6c) == 1 && at<U>(as_creature(target), 0x9d4) == 0) {   // 005A4E0E..23
            attacker_pc = 1;                                                        // 005A4E2E
            percent = kotor_fastcall_004ae9f0(server_app(), nullptr, 3);            // 005A4E25..65
        } else if (at<U>(stats, 0x6c) == 0 && at<U>(as_creature(target), 0x9d4) == 1) { // 005A4E3A..4F
            defender_pc = 1;                                                        // 005A4E59
            percent = kotor_fastcall_004ae9f0(server_app(), nullptr, 4);            // 005A4E51..65
        }
    }
    return kotor_fastcall_00550d10(rules(), nullptr, roll, die, attacker_pc, defender_pc, percent); // 005A4E69..80
}

// ---------------------------------------------------------------------------------------------------------
// 005A4FB0 CSWSCreatureStats::ResolveSpecialAttackDamageBonus(unused): RET 4. Table on the current attack's
// special-attack word (+64): index = word - 11h (unsigned bound 56h), bytes 005A5030 -> targets 005A5014.
U __fastcall kotor_fastcall_005a4fb0(void* stats, void*, U) {
    void* creature = creature_of(stats);
    void* attack = attack_at(creature, at<U8>(round_of(creature), 0x96c));          // 005A4FB0..C1
    const U index = static_cast<U>(at<U16>(attack, 0x64)) - 0x11u;                  // 005A4FC6..CC
    if (index > 0x56) return 0;                                                     // 005A4FCF..D4 (ja, EAX = 0)
    switch (index) {                                                                // 005A4FD6..DD
    case 0x00: case 0x01: return 8;                                                 // 11h,12h -> 005A4FEC
    case 0x0b: case 0x0c: return 5;                                                 // 1Ch,1Dh -> 005A4FE4
    case 0x41: case 0x42: return 0xa;                                               // 52h,53h -> 005A4FF4 (review F3, index table 005A5030)
    case 0x54: return 0;                                                            // 65h -> 005A4FFC
    case 0x55: return 2;                                                            // 66h -> 005A5001
    case 0x56: return 4;                                                            // 67h -> 005A5009
    default: return 0;                                                              // selector 6 -> 005A500E
    }
}

// ---------------------------------------------------------------------------------------------------------
// 005A8BE0 CSWSCreatureStats::GetDamageBonus(target creature, offhand, unused): RET 0Ch.
//   attack +102 = (attack +40 == 0) ? (signed char) stats +EA : 0     (ability damage modifier, first attack)
//   attack +103 = 2 when the weapon's specialization feat (base item +B0) is known, else 0
//   result      = power attack bonus (combat mode +4D2: 2 -> 5, 3 -> 10) + ResolveSpecialAttackDamageBonus
// Quirks: the specialization bonus is only recorded, not returned; the base item of the main-hand item is looked up
// and discarded; a class-count loop under `target != 0` has no effect; with the debug flag set and a nonzero
// special bonus only AL carries the result (callers use AL).
U __fastcall kotor_fastcall_005a8be0(void* stats, void*, void* target, U offhand, U) {
    void* creature = creature_of(stats);
    const U index = at<U8>(round_of(creature), 0x96c);                              // 005A8C02..0C (EDI)
    void* attack = attack_at(creature, index);                                      // 005A8C13..1B (EBX)
    const U first = at<U>(attack, 0x40);                                            // 005A8C1D..22 ([esp+14])
    const U critical = at<U8>(attack, 0x5c) == 2 ? 1u : 0u;                         // 005A8C26..32 ([esp+1C])
    void* weapon = kotor_fastcall_004d4ff0(round_of(creature), nullptr, at<U8>(attack, 0x3a)); // 005A8C2A..44 ([esp+20])
    void* main_hand = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 0x10); // 005A8C48..55
    if (main_hand != nullptr) (void)base_item_of(at<U>(main_hand, 0xc));            // 005A8C5A..6B (result unused)
    at<U8>(attack, 0x102) = first == 0 ? at<U8>(stats, 0xea) : 0;                   // 005A8C70..81
    // 005A8C87..A3: if (target) count up to the class count (+89); no effect
    I power = 0;                                                                    // [esp+0C]
    if (at<U8>(creature, 0x4d2) == 2) power = 5;                                    // 005A8CA5..B9
    if (at<U8>(creature, 0x4d2) == 3) power = 0xa;                                  // 005A8CC1..CC
    if (recovered_debug_cell_008327e4 != 0 && power != 0) {                         // 005A8CD4..E8
        append_debug(creature, index, [&](void* text) {                             // 005A8CEE..DCE
            if (critical != 0) {
                const U shown = kotor_fastcall_005a52b0(stats, nullptr, offhand);
                frame_LogFormat_005e5680(text, " + %d (Power/Improved Power Attack Damage Bonus) (Critical x%d)",
                    static_cast<I>(kotor_fastcall_005a52b0(stats, nullptr, offhand)) * power, shown); // .rdata 0074AA50
            } else {
                frame_LogFormat_005e5680(text, " + %d (Power/Improved Power Attack Damage Bonus)", power); // .rdata 0074AA18
            }
        });
    }
    U specialization = 0;                                                           // [esp+14]
    if (weapon != nullptr && kotor_m15b_item_get_base_item_005b4790(weapon, nullptr) != nullptr) { // 005A8DD3..EA
        const U feat = at<U16>(kotor_m15b_item_get_base_item_005b4790(weapon, nullptr), 0xb0); // 005A8DEC..F5
        if (feat != 0 && recovered_stats_has_feat_005a6630(stats, nullptr, feat) != 0) specialization = 2; // 005A8DFC..0D
    }
    at<U8>(attack, 0x103) = static_cast<U8>(specialization);                        // 005A8E15..19
    if (recovered_debug_cell_008327e4 != 0 && specialization != 0) {                // 005A8E1F..2F
        append_debug(creature, index, [&](void* text) {                             // 005A8E35..F18
            if (critical != 0) {
                const U shown = kotor_fastcall_005a52b0(stats, nullptr, offhand);
                frame_LogFormat_005e5680(text, " + %d (Weapon Specialization Bonus) (Critical x%d)",
                    kotor_fastcall_005a52b0(stats, nullptr, offhand) * specialization, shown); // .rdata 0074A9E4
            } else {
                frame_LogFormat_005e5680(text, " + %d (Weapon Specialization Bonus)", specialization); // .rdata 0074A9C0
            }
        });
    }
    const U special = kotor_fastcall_005a4fb0(stats, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(target))); // 005A8F1D..23 (EBX)
    const U sum = static_cast<U>(power) + special;                                  // 005A8F30..38 ([esp+20])
    if (recovered_debug_cell_008327e4 != 0 && special != 0) {                       // 005A8F28..44
        append_debug(creature, index, [&](void* text) {                             // 005A8F4A..027
            if (critical != 0) {
                const U shown = kotor_fastcall_005a52b0(stats, nullptr, offhand);
                frame_LogFormat_005e5680(text, " + %d (Special Attack Damage Bonus) (Critical x%d)",
                    kotor_fastcall_005a52b0(stats, nullptr, offhand) * special, shown); // .rdata 0074A98C
            } else {
                frame_LogFormat_005e5680(text, " + %d (Special Attack Damage Bonus)", special); // .rdata 0074A968
            }
        });
        return sum & 0xffu;                                                         // 005A902C (mov al, [esp+20])
    }
    return sum;                                                                     // 005A9030
}

// ---------------------------------------------------------------------------------------------------------
// 005A9050 CSWSCreatureStats::GetDamageRoll(target object, offhand, critical, sneak, maximum, record): RET 18h.
//   AX = total damage (signed word) after the target's damage reduction/resistance/immunity virtuals (+AC/+A8/+A4).
U __fastcall kotor_fastcall_005a9050(void* stats, void*, void* target, U offhand, U critical, U sneak, U maximum,
    U record) {
    void* creature = creature_of(stats);
    U damage = 0;                                                                   // L10 (only the low word is consumed)
    I dice = 0, die = 0;                                                            // L14, L18
    const U index = at<U8>(round_of(creature), 0x96c);                              // L38 (005A907A..8E)
    U percent = 0x64;                                                               // EBP (005A9099)
    void* attack = attack_at(creature, index);                                      // L1C (005A9092..A3)
    void* weapon;                                                                   // EDI / L30
    if (offhand != 0) {                                                             // 005A90A7..AE
        weapon = recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 0x20); // 005A90B0..C3
        if (weapon == nullptr) weapon = kotor_fastcall_004d4ff0(round_of(creature), nullptr, 0); // 005A90C9..DC
    } else {
        weapon = kotor_fastcall_004d4ff0(round_of(creature), nullptr, 0);           // 005A90CC..DC
    }
    U attacker_pc = 0, defender_pc = 0;                                             // L28, L24 (005A90E4..EC)
    if (target != nullptr && as_creature(target) != nullptr) {                      // 005A90E6..FB
        if (at<U>(stats, 0x6c) == 1 && at<U>(as_creature(target), 0x9d4) == 0) {   // 005A90FD..12
            attacker_pc = 1;                                                        // 005A9114
            percent = kotor_fastcall_004ae9f0(server_app(), nullptr, 3);            // 005A911C..4E
        } else if (at<U>(stats, 0x6c) == 0 && at<U>(as_creature(target), 0x9d4) == 1) { // 005A9120..35
            defender_pc = 1;                                                        // 005A9137
            percent = kotor_fastcall_004ae9f0(server_app(), nullptr, 4);            // 005A913F..4E
        }
    }
    auto attack_type = [&]() { return kotor_fastcall_004d3da0(round_of(creature), nullptr); };
    // property 33h (damage dice override) -> NumDice/Die from [[rules+B8]+AC]; false when absent
    auto property_dice = [&](U state_dice, U state_die) -> bool {
        (void)state_dice; (void)state_die;                                          // EH states only
        if (recovered_item_property_has_00555c90(weapon, nullptr, 0x33, 0) == 0) return false;
        void* property = nullptr;   // the original leaves the slot unwritten when GetPropertyByType finds nothing
        recovered_item_property_query_005539c0(weapon, nullptr, &property, 0x33, 0);
        if (property == nullptr) return false;
        const U row = at<U16>(property, 6);
        if (row == 0) return false;
        table_int(0xac, row, k_numdice_00747038, &dice);
        table_int(0xac, row, k_die_00747034, &die);
        return true;
    };
    // one weighted roll (RollDice(dice, die) -> movsx ax; max = movsx (dice*die))
    auto weighted = [&](I max) -> U {
        const U roll = static_cast<U>(static_cast<I>(static_cast<short>(
            recovered_rules_roll_dice_00550b60(rules(), nullptr, static_cast<U>(dice), static_cast<U>(die)))));
        return kotor_fastcall_00550d10(rules(), nullptr, roll, static_cast<U>(static_cast<I>(static_cast<short>(max))),
                                       attacker_pc, defender_pc, percent);
    };
    // normal dice: critical -> one weighted roll per critical multiplier (re-read each iteration), else one roll
    auto roll_dice = [&]() {
        const I max = static_cast<I>(static_cast<U>(dice) * static_cast<U>(die));   // EDI (imul)
        if (critical != 0) {
            for (I i = 0; i < static_cast<I>(kotor_fastcall_005a52b0(stats, nullptr, offhand)); ++i)
                damage += weighted(max);                                            // add [esp+10], eax
        } else {
            damage = weighted(max);                                                 // 005A958A
        }
    };
    bool weapon_dice = weapon != nullptr;                                           // 005A9150..52
    if (weapon_dice) {
        const U8 kind = at<U8>(kotor_m15b_item_get_base_item_005b4790(weapon, nullptr), 0xac); // 005A9158..7A
        if (kind == 0x13) weapon_dice = false;
        else if (at<U8>(kotor_m15b_item_get_base_item_005b4790(weapon, nullptr), 0xac) == 0x14) weapon_dice = false;
    }
    if (weapon_dice) {
        if (maximum == 0) {                                                         // 005A9180..92
            if (attack_type() == 3 || attack_type() == 4 || attack_type() == 5) {   // 005A9198..CE
                if (property_dice(0, 1)) roll_dice();                               // 005A9282..3C6 / 005A93CB..F6
            } else {
                dice = at<U8>(base_item_of(at<U>(weapon, 0xc)), 0x1c);              // 005A91D4..EA
                die = at<U8>(base_item_of(at<U>(weapon, 0xc)), 0x1d);               // 005A91EE..12
                roll_dice();                                                        // 005A9203..76 / 005A927B..F6
            }
        } else {                                                                    // 005A93FB: maximum damage
            if (attack_type() == 3 || attack_type() == 4 || attack_type() == 5) {   // 005A93FB..31
                (void)property_dice(2, 3);                                          // 005A94B2..57C
            } else if (attack_type() == 7 || attack_type() == 8) {                  // 005A9433..57
                dice = static_cast<I>(kotor_fastcall_005a4d40(stats, nullptr) & 0xff); // 005A9491..9D (movzx al)
                die = static_cast<I>(kotor_fastcall_005a4d60(stats, nullptr) & 0xff);  // 005A94A1..A9
            } else {
                dice = at<U8>(base_item_of(at<U>(weapon, 0xc)), 0x1c);              // 005A9459..6F
                die = at<U8>(base_item_of(at<U>(weapon, 0xc)), 0x1d);               // 005A9473..88
            }
            damage = static_cast<U>(dice * die);                                    // 005A9581..8A
        }
        if (recovered_item_property_has_00555c90(weapon, nullptr, 0x1f, 0) != 0) damage = 0; // 005A958E..A7
    } else if (critical != 0) {                                                     // 005A95A9..B1: unarmed
        for (I i = 0; i < static_cast<I>(kotor_fastcall_005a52b0(stats, nullptr, offhand)); ++i) // 005A95B3..DA
            damage += kotor_fastcall_005a4d90(stats, nullptr, target);
    } else {
        damage = kotor_fastcall_005a4d90(stats, nullptr, target);                   // 005A95DE..E4
    }
    if (record != 0 && static_cast<short>(damage) > 0) {                            // 005A95E8..FA
        at<U8>(attack, 0xfe) = static_cast<U8>(damage);                             // 005A9605
        const U flags = kotor_fastcall_004ed5e0(creature, nullptr);                 // 005A960B..0F
        kotor_fastcall_004d20d0(attack, nullptr, flags,
                                static_cast<U>(static_cast<I>(static_cast<short>(damage))), 0); // 005A9600..17
    }
    if (recovered_debug_cell_008327e4 != 0) {                                       // 005A9622..29
        const short shown = static_cast<short>(damage);
        append_debug(creature, index, [&](void* text) {                             // 005A962F..709
            if (critical != 0)
                frame_LogFormat_005e5680(text, "Damage Roll: %d (Critical x%d)", static_cast<I>(shown),
                                         kotor_fastcall_005a52b0(stats, nullptr, offhand)); // .rdata 0074AB40
            else
                frame_LogFormat_005e5680(text, "Damage Roll: %d", static_cast<I>(shown)); // .rdata 0074AB30
        });
    }
    if (sneak != 0) {                                                               // 005A970E..17
        U count = 0;                                                                // EDI: highest sneak attack feat
        static const U feats[10] = {0x45, 0x44, 0x43, 0x42, 0x41, 0x40, 0x3f, 0x3e, 0x3d, 0x3c};
        for (U i = 0; i < 10; ++i) {                                                // 005A971D..E9
            if (recovered_stats_has_feat_005a6630(stats, nullptr, feats[i]) != 0) { count = 10 - i; break; }
        }
        const U extra = recovered_rules_roll_dice_00550b60(rules(), nullptr, count, 6) & 0xffffu; // 005A97EE..FC
        if (recovered_debug_cell_008327e4 != 0) {                                   // 005A97FF..06
            append_debug(creature, index, [&](void* text) {                         // 005A980C..8B6
                frame_LogFormat_005e5680(text, " + %d (Sneak Attack Damage)", extra); // .rdata 0074AB14
            });
        }
        damage += extra;                                                            // 005A98BB..C5
        at<U8>(attack, 0x101) = static_cast<U8>(extra);                             // 005A98C9
    }
    void* target_creature = nullptr;                                                // 005A98D1..F9
    if (target != nullptr && as_creature(target) != nullptr) target_creature = as_creature(target);
    const I bonus = static_cast<signed char>(static_cast<U8>(
        kotor_fastcall_005a8be0(stats, nullptr, target_creature, offhand, record))); // 005A990A..17 (movsx al) -> L28
    at<U8>(attack, 0xff) = static_cast<U8>(bonus);                                  // 005A991E
    at<U8>(attack, 0x100) = 0;                                                      // 005A9924
    U record_slot = record, sneak_slot = sneak;                                     // argument slots reused below
    if (critical != 0) {                                                            // 005A992B
        U spec_total = 0, bonus_total = 0;                                          // arg6 / arg4 slots (= 0)
        U8 ability_total = 0;                                                       // BL
        for (I i = 0; i < static_cast<I>(kotor_fastcall_005a52b0(stats, nullptr, offhand)); ++i) { // 005A9931..B4
            damage += static_cast<U>(bonus);                                        // 005A9960..78
            spec_total += at<U8>(attack, 0x103);                                    // 005A9968..8E
            ability_total = static_cast<U8>(ability_total + at<U8>(attack, 0x102)); // 005A997C..85
            bonus_total += at<U8>(attack, 0xff);                                    // 005A9995..9E
        }
        at<U8>(attack, 0x103) = static_cast<U8>(spec_total);                        // 005A99CC
        at<U8>(attack, 0xff) = static_cast<U8>(bonus_total);                        // 005A99D6
        at<U8>(attack, 0x102) = ability_total;                                      // 005A99E0
        // 005A99C8..EA: low word += movzx(spec byte) + movsx(ability byte); only the low word is consumed later
        damage += static_cast<U>(static_cast<U8>(spec_total)) + static_cast<U>(static_cast<I>(static_cast<signed char>(ability_total)));
        record_slot = spec_total; sneak_slot = bonus_total;
        // 005A99EE..C0E: property 31h (massive critical) on the weapon
        if (weapon != nullptr && recovered_item_property_has_00555c90(weapon, nullptr, 0x31, 0) != 0) {
            void* property = nullptr;   // the original leaves the slot unwritten when GetPropertyByType finds nothing
            recovered_item_property_query_005539c0(weapon, nullptr, &property, 0x31, 0); // 005A9A07..12
            if (property != nullptr && at<U16>(property, 6) != 0) {                // 005A9A17..2A
                const U row = at<U16>(property, 6);                                 // EDI
                I massive_dice = static_cast<I>(record_slot), massive_die = static_cast<I>(sneak_slot);
                table_int(0xb0, row, k_numdice_00747038, &massive_dice);            // 005A9A30..7C (into the arg6 slot)
                U extra;                                                            // EBX
                if (massive_dice == 0) {                                            // 005A9A81..8A
                    extra = row;                                                    // 005A9A8C (quirk: the row id is the damage)
                } else {
                    table_int(0xb0, row, k_die_00747034, &massive_die);             // 005A9A90..D8 (into the arg4 slot)
                    extra = recovered_rules_roll_dice_00550b60(rules(), nullptr, static_cast<U>(massive_dice),
                                                               static_cast<U>(massive_die)) & 0xffffu; // 005A9ADD..F8
                }
                at<U8>(attack, 0xff) = static_cast<U8>(at<U8>(attack, 0xff) + static_cast<U8>(extra)); // 005A9AFB..07
                damage += extra;                                                    // 005A9B01..16
                if (recovered_debug_cell_008327e4 != 0) {                           // 005A9B0D..1A
                    append_debug(creature, index, [&](void* text) {                 // 005A9B20..BEF
                        if (massive_dice == 0)
                            frame_LogFormat_005e5680(text, " + %d (Massive Critical) ", extra); // .rdata 0074AAF8
                        else
                            frame_LogFormat_005e5680(text, " + %d (%dd%d Massive Critical) ", extra, massive_dice,
                                                     massive_die);                  // .rdata 0074AAD8
                    });
                }
            }
        }
    } else {                                                                        // 005A9BF6..C0A
        damage += static_cast<U>(bonus) + static_cast<U>(static_cast<I>(static_cast<signed char>(at<U8>(attack, 0x102))))
                + at<U8>(attack, 0x103);
    }
    (void)record_slot; (void)sneak_slot;
    // 005A9C0E..3F: GetTotalEffectBonus(2, target, &local, 0, maximum, 0, 0, FF, FF, 0) with local = 0
    U local = 0;                                                                    // [E+48]
    const U effect = kotor_fastcall_004f3fe0(creature, nullptr, 2,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(target)), static_cast<U>(reinterpret_cast<std::uintptr_t>(&local)),
        0, maximum, 0, 0, 0xff, 0xff, 0);                                           // EBX
    at<U8>(attack, 0x109) = static_cast<U8>(effect);                                // 005A9C4A
    if (recovered_debug_cell_008327e4 != 0 && effect != 0) {                        // 005A9C50..5F
        append_debug(creature, index, [&](void* text) {                             // 005A9C65..D3C
            if (critical == 1)                                                      // 005A9C9A (cmp ..., 1)
                frame_LogFormat_005e5680(text, " + %d (Effect Damage Bonus) (Critical x%d)", effect,
                                         kotor_fastcall_005a52b0(stats, nullptr, offhand)); // .rdata 0074AAAC
            else
                frame_LogFormat_005e5680(text, " + %d (Effect Damage Bonus)", effect); // .rdata 0074AA90
        });
    }
    // quirk: the effect bonus is only recorded (+109), not added to the damage
    if (static_cast<short>(damage) < 1) {                                           // 005A9D41..47 (word compare)
        at<U8>(attack, 0xfe) = 1;                                                   // 005A9D4B
        damage = 1;                                                                 // 005A9D57 [esp+18] (review F2)
        const U flags = kotor_fastcall_004ed5e0(creature, nullptr);                 // 005A9D5F
        kotor_fastcall_004d20d0(attack, nullptr, flags, 1, 0);                      // 005A9D49..67
    }
    const U flags = kotor_fastcall_004ed5e0(creature, nullptr);                     // 005A9D6C..76 (EDI)
    if (target == nullptr) return static_cast<U>(static_cast<I>(static_cast<short>(damage))); // 005A9D78 -> 005A9DEA
    const U power = kotor_fastcall_004fd6f0(creature, nullptr,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(target)));                  // 005A9D7A..7E
    const U power_slot = (offhand & 0xffffff00u) | (power & 0xffu);                 // 005A9D96 byte store into the arg2 slot
    U value = vcall<U>(target, 0xac, creature, static_cast<U>(static_cast<I>(static_cast<short>(damage))), flags,
                       maximum, 1u);                                                // 005A9D83..A4 (DoDamageReduction-like)
    value = vcall<U>(target, 0xa8, creature, static_cast<U>(static_cast<I>(static_cast<short>(value))), flags, maximum,
                     1u, 1u);                                                       // 005A9DAA..BD
    value = vcall<U>(target, 0xa4, creature, static_cast<U>(static_cast<I>(static_cast<short>(value))), power_slot,
                     maximum, 1u);                                                  // 005A9DC3..D8
    short result = static_cast<short>(value);
    if (result < 0) result = 0;                                                     // 005A9DDE..E3
    return static_cast<U>(static_cast<I>(result));                                  // 005A9DE5
}
}  // extern "C"
