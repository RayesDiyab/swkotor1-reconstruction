// combat-attack lane, part C1 (attackstats): the to-hit and armor-class side of CSWSCreature::ResolveAttackRoll.
//   005A7EA0 CSWSCreatureStats::GetAttackModifierVersus      005AD0E0 CSWSCreatureStats::GetArmorClassVersus
//   005A60D0 CSWSCreatureStats::GetBaseAttackBonus           005A5330 CSWSCreatureStats::GetDEXMod
//   004ED190 CSWSCreature::GetRelativeWeaponSize             00553CC0 CSWSItem::ComputeArmorClass
//   005A4EC0 CSWSCreatureStats::ResolveSpecialAttackAttackBonus 005A9F50 CSWSCreatureStats::GetTotalACFeatMod
//   005A54F0 CSWSCreatureStats::GetWeaponFinesse             005A52B0 CSWSCreatureStats::GetCriticalHitMultiplier
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates; live_operator_new / live_free only (none needed here). CSWSCreatureStats +24 = owner creature.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

namespace kotor_reconstruction { struct StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs);   // CExoString::operator+ [0093]
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* element, void*); } // CExoString() [0603]

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
extern unsigned recovered_debug_cell_008327e4;                                                // .bss combat-debug text flag [W1 b7]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, unsigned index);  // CSWSCombatRound::GetAttack [W1 b7]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // CSWBaseItemArray::GetBaseItem [0820]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                   // CSWItem::GetBaseItem [0820]
unsigned char __fastcall kotor_m15b_class_get_attack_bonus_005bccb0(void* cls, void*, unsigned level); // CSWClass::GetAttackBonus [0820]
unsigned char __fastcall kotor_m15b_class_get_armor_bonus_005be770(void* cls, void*, unsigned level);  // CSWClass::GetArmorBonus [0820]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                    // CSWSCreatureStats::HasFeat [clear9]
U __fastcall kotor_m15closure_00555690(void* item, void*);                                     // CSWSItem::IsLightsaber [0831]
unsigned __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                       // return_zero [0832]
unsigned __fastcall recovered_creature_invisible_00501950(void* self, void*, void* viewer);    // CSWSCreature::GetInvisible [W1 b7]
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);      // GetVisibleListElement [W1 b7]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                  // CGameEffect::GetInteger [W1 b7]
// CSWSCreature::GetTotalEffectBonus [W2 g07 native, single 004F3FE0 entry]. NOTE: its kind-1 (attack) arm is an
// explicit W2 trap (004F410D) in the c07b candidate: the attack-bonus call below reaches it until W2 completes it.
unsigned __fastcall kotor_fastcall_004f3fe0(void* c, void*, unsigned a1, unsigned a2,
    unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7, unsigned a8, unsigned a9, unsigned a10);
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                           // CExoString(const CExoString&) [0078]
// CExoString::operator= 005E5C50: assign_string_005e5c50 (w5_action_queue.hpp inline -> async_assign_string_005e5c50 [0199])
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                     // CExoString::Format [0125]

// ---------------- sibling parts (combat-attack lane) ----------------
void* __fastcall kotor_fastcall_004d4ff0(void* round, void*, U hand);   // CSWSCombatRound::GetCurrentAttackWeapon [part B]
U __fastcall kotor_fastcall_004ed5e0(void* creature, void*);            // CSWSCreature::GetDamageFlags [part C2]

// ---------------- exports ----------------
I __fastcall kotor_fastcall_004ed190(void* creature, void*, void* item);
U __fastcall kotor_fastcall_00553cc0(void* item, void*);
U __fastcall kotor_fastcall_005a60d0(void* stats, void*);
U __fastcall kotor_fastcall_005a5330(void* stats, void*, U flag);
U __fastcall kotor_fastcall_005a9f50(void* stats, void*);
U __fastcall kotor_fastcall_005a54f0(void* stats, void*, void* item);
U __fastcall kotor_fastcall_005a52b0(void* stats, void*, U offhand);
U __fastcall kotor_fastcall_005a4ec0(void* stats, void*, U unused);
U __fastcall kotor_fastcall_005a7ea0(void* stats, void*, void* target);
U __fastcall kotor_fastcall_005ad0e0(void* stats, void*, void* attacker, U flag);
}

namespace {
const char k_fmt_bab_0074a94c[] = " + %d (Base Attack Bonus)";
const char k_fmt_offhand_0074a92c[] = " + %d (Offhand Attack Penalty)";
const char k_fmt_special_0074a904[] = " + %d (Special Attack To Hit Modifier)";
const char k_fmt_dueling_0074a8e4[] = " + %d (Dueling To Hit Modifier)";
const char k_fmt_good_aim_0074a8d0[] = " + %d (Good Aim)";
const char k_fmt_training_0074a8a4[] = " + %d (Offensive Training Bonus (vs Race))";
const char k_fmt_dex_0074a88c[] = " + %d (Dex Modifier)";
const char k_fmt_str_0074a874[] = " + %d (Str Modifier)";
const char k_fmt_focus_0074a858[] = " + %d (Weapon Focus Bonus)";
const char k_fmt_effect_ab_0074a83c[] = " + %d (Effect Attack Bonus)";
const char k_fmt_natural_0074ac50[] = " + %d (Natural AC)";
const char k_fmt_armor_0074ac3c[] = " + %d (Armor AC)";
const char k_fmt_shield_0074ac28[] = " + %d (Shield AC)";
const char k_fmt_ac_dex_0074ac18[] = " + %d (Dex Mod)";
const char k_fmt_ac_dodge_0074ac04[] = " + %d (Dodge Mod)";
const char k_fmt_eff_dodge_0074abe4[] = " + %d (Effect AC Dodge Bonus)";
const char k_fmt_eff_natural_0074abc4[] = " + %d (Effect AC Natural Bonus)";
const char k_fmt_eff_armor_0074aba4[] = " + %d (Effect AC Armor Bonus)";
const char k_fmt_eff_shield_0074ab84[] = " + %d (Effect AC Shield Bonus)";
const char k_fmt_eff_deflection_0074ab60[] = " + %d (Effect AC Deflection Bonus)";
const float k_good_aim_range2_00745e5c = 25.0f;   // .rdata 00745E5C
constexpr U k_effect_ac_increase_0074673c = 48;   // .rdata 0074673C (effect type)
constexpr U k_effect_ac_decrease_00746740 = 49;   // .rdata 00746740
constexpr U k_damage_all_0074688c = 0x4007;       // .rdata 0074688C
constexpr U k_damage_flag_1_00746850 = 1;         // .rdata 00746850
constexpr U k_damage_flag_2_00746854 = 2;         // .rdata 00746854
constexpr U k_damage_flag_4_00746858 = 4;         // .rdata 00746858
constexpr U k_ac_dodge_007468b0 = 0;              // .rdata 007468B0 (effect integer 0 = AC type)
constexpr U k_ac_natural_007468b4 = 1;            // .rdata 007468B4
constexpr U k_ac_armor_007468b8 = 2;              // .rdata 007468B8
constexpr U k_ac_shield_007468bc = 3;             // .rdata 007468BC
constexpr U k_ac_deflection_007468c0 = 4;         // .rdata 007468C0

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline U addr32(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* creature_of(void* stats) { return at<void*>(stats, 0x24); }
inline void* round_of(void* creature) { return at<void*>(creature, 0x9c8); }
inline void* inventory_of(void* creature) { return at<void*>(creature, 0xa2c); }
inline void* item_in_slot(void* creature, U slot) { return recovered_equipment_item_005a4c20(inventory_of(creature), nullptr, slot); }
inline void* base_item_by_id(void* item) {                    // rules +34 CSWBaseItemArray, item +0C base item id
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr, at<int>(item, 0xc));
}
inline void* get_attack(void* creature, U index) { return recovered_combat_round_attack_004d2b50(round_of(creature), nullptr, index); }
inline I s8(U8 v) { return static_cast<signed char>(v); }
// IsDebilitated ? min(+EC, 0) : +EC (the setge/dec/and idiom), as a sign-extended byte
inline I capped_dex(void* stats) {
    const I dex = s8(at<U8>(stats, 0xec));
    if (w5x_creature_is_debilitated_005b4880(creature_of(stats), nullptr) != 0) return dex >= 0 ? 0 : dex;
    return dex;
}

// CExoString scratch (8 bytes) as the original stack slots
struct ExoString { alignas(4) unsigned char b[8]; };
inline kotor_reconstruction::StartupString* ss(ExoString& s) { return reinterpret_cast<kotor_reconstruction::StartupString*>(s.b); }

// The combat-debug text pattern (only while the .bss flag 008327E4 is set; no writer in the image):
//   copy = GetAttack(index)->+78; tmp = CExoString(); for each term: tmp.Format(fmt, v); sum = copy + tmp;
//   GetAttack(index)->+78 = sum; ~sum; [copy = GetAttack(index)->+78]; finally ~tmp, ~copy.
// The creature (and its round +9C8) is re-read for every GetAttack as the original does.
struct DebugText {
    void* creature; U index; ExoString copy, tmp;
    DebugText(void* c, U i) : creature(c), index(i) {
        WindowCopy005e5b00(copy.b, nullptr, static_cast<unsigned char*>(get_attack(creature, index)) + 0x78);
        kotor_movie::string_element_construct_005b3190(tmp.b, nullptr);
    }
    void add(const char* format, I value, bool refresh) {
        frame_LogFormat_005e5680(tmp.b, format, value);
        ExoString sum;
        GuiConcat005e5d10(ss(copy), nullptr, ss(sum), ss(tmp));
        assign_string_005e5c50(static_cast<unsigned char*>(get_attack(creature, index)) + 0x78, nullptr, sum.b);
        frame_LogStringDtor_005e5c20(sum.b, nullptr);
        if (refresh) assign_string_005e5c50(copy.b, nullptr, static_cast<unsigned char*>(get_attack(creature, index)) + 0x78);
    }
    void close() { frame_LogStringDtor_005e5c20(tmp.b, nullptr); frame_LogStringDtor_005e5c20(copy.b, nullptr); }
};
// the single-term blocks of GetAttackModifierVersus: copy, tmp, Format, sum, assign, ~sum, ~tmp, ~copy
void debug_term(void* creature, U index, const char* format, I value) {
    DebugText t(creature, index);
    t.add(format, value, false);
    t.close();
}
}  // namespace

extern "C" {
// ---------------------------------------------------------------------------------------------------------
// 004ED190 CSWSCreature::GetRelativeWeaponSize(CSWSItem*): RET 4. base item +1B (weapon size) - creature +4F8
// (creature size); outside -2..1 -> -10.
I __fastcall kotor_fastcall_004ed190(void* creature, void*, void* item) {
    const I size = static_cast<I>(at<U>(creature, 0x4f8));                          // 004ED198
    const I rel = static_cast<I>(at<U8>(base_item_by_id(item), 0x1b)) - size;       // 004ED190..B1
    if (rel < -2 || rel > 1) return -10;                                            // 004ED1B3..BE (jl / jle signed)
    return rel;
}

// 00553CC0 CSWSItem::ComputeArmorClass: RET 0. Base item +18 == 1 (armor) -> base item +8A, else 0.
U __fastcall kotor_fastcall_00553cc0(void* item, void*) {
    if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x18) != 1) return 0; // 00553CC3..D1
    return at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x8a);     // 00553CD2..E1 (second lookup)
}

// 005A60D0 CSWSCreatureStats::GetBaseAttackBonus: RET 0. Sum of CSWClass::GetAttackBonus over the class list
// (+89 count, 28h-byte entries at +A7: class id, +A8: level); +102 nonzero overrides the sum.
U __fastcall kotor_fastcall_005a60d0(void* stats, void*) {
    U total = 0;                                                                    // EDI
    U8 count = at<U8>(stats, 0x89);                                                 // 005A60D5
    if (count != 0) {                                                               // 005A60E0..E2 (jbe)
        for (U i = 0; ; ) {                                                         // EBX
            const U8 index = static_cast<U8>(i);
            U8 level;                                                               // [esp+0C] (low byte only written)
            if (index < count) {                                                    // 005A60E4..E6 (byte compare)
                const I l = at<U8>(stats, index * 0x28 + 0xa8);                     // 005A60EF..F5 (movzx; never < 0)
                level = static_cast<U8>(l < 0 ? 0 : l);                             // 005A60FD..06
            } else {
                level = 0;                                                          // 005A60E8
            }
            const U8 cls = index < count ? at<U8>(stats, index * 0x28 + 0xa7) : 0xff; // 005A610A..18
            const U cp = cls * 0x1a0u + at<U>(rules(), 0xac);                     // 005A611F..34
            if (cp != 0)                                                            // 005A6136 (je on the sum)
                total += kotor_m15b_class_get_attack_bonus_005bccb0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(cp)),
                                                                    nullptr, level); // 005A6138..45 (movzx al)
            count = at<U8>(stats, 0x89);                                            // 005A6147 (re-read)
            ++i;
            if (static_cast<I>(i) >= static_cast<I>(count)) break;                  // 005A6150..53 (jl)
        }
    }
    const U8 override_value = at<U8>(stats, 0x102);                                 // 005A6155
    if (override_value == 0) return total;                                          // 005A615B..65
    return override_value;                                                          // 005A6166..6D
}

// 005A5330 CSWSCreatureStats::GetDEXMod(flag): RET 4, AL = signed byte (callers movsx).
//   flag 1 and not debilitated: capped by the worn armor's base item +AD (max dex) when it has armor class.
U __fastcall kotor_fastcall_005a5330(void* stats, void*, U flag) {
    I cap = 0;                                                                      // EBX
    bool capped = false;
    if (flag == 1 && w5x_creature_is_debilitated_005b4880(creature_of(stats), nullptr) == 0) { // 005A5338..4A
        void* armor = item_in_slot(creature_of(stats), 2);                         // 005A534C..5C
        if (armor != nullptr && static_cast<I>(kotor_fastcall_00553cc0(armor, nullptr)) > 0) { // 005A535E..6B (jle)
            cap = at<U8>(kotor_m15b_item_get_base_item_005b4790(armor, nullptr), 0xad); // 005A536D..74
            if (cap >= 0) capped = true;                                            // 005A537B..7D (always)
        }
    }
    if (!capped && w5x_creature_is_debilitated_005b4880(creature_of(stats), nullptr) != 0) capped = true; // 005A537F..89
    if (!capped) return static_cast<U>(s8(at<U8>(stats, 0xec)));                    // 005A539E..A8
    const I dex = s8(at<U8>(stats, 0xec));                                          // 005A538B
    if (dex < cap) return static_cast<U>(dex);                                      // 005A5392..94 -> 005A53A5
    return static_cast<U>(cap);                                                     // 005A5396..9B (mov al, bl)
}

// 005A9F50 CSWSCreatureStats::GetTotalACFeatMod: RET 0, AL. One-handed melee (main hand set, off hand empty,
// base item type 2 or 4): Dueling feats 73h/72h/71h -> 3/2/1.
U __fastcall kotor_fastcall_005a9f50(void* stats, void*) {
    void* main = item_in_slot(creature_of(stats), 0x10);                            // 005A9F54..62
    void* off = item_in_slot(creature_of(stats), 0x20);                             // 005A9F67..74
    if (main == nullptr || off != nullptr) return 0;                                // 005A9F79..7F
    void* base = kotor_m15b_item_get_base_item_005b4790(main, nullptr);             // 005A9F81..83
    if (base == nullptr) return 0;                                                  // 005A9F88..8A
    const U8 type = at<U8>(base, 8);
    if (type != 2 && type != 4) return 0;                                           // 005A9F8C..95
    if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x73) != 0) return 3;     // 005A9F97..A9
    if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x72) != 0) return 2;     // 005A9FAA..BC
    if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x71) != 0) return 1;     // 005A9FBD..CA
    return 0;                                                                       // 005A9FCC
}

// 005A54F0 CSWSCreatureStats::GetWeaponFinesse(CSWSItem*): RET 4. Lightsaber and (capped) DEX mod > STR mod.
U __fastcall kotor_fastcall_005a54f0(void* stats, void*, void* item) {
    if (item == nullptr) return 0;                                                  // 005A54F3..F9
    if (kotor_m15closure_00555690(item, nullptr) != 1) return 0;                    // 005A54FB..03
    const I dex = capped_dex(stats);                                                // 005A5505..2B
    if (static_cast<signed char>(dex) <= s8(at<U8>(stats, 0xea))) return 0;         // 005A552B..31 (byte jle)
    return 1;
}

// 005A52B0 CSWSCreatureStats::GetCriticalHitMultiplier(offhand): RET 4. Default 2; the weapon's base item +1F.
//   offhand: main-hand item when its base item type is 3 (double weapon), else the off-hand item.
U __fastcall kotor_fastcall_005a52b0(void* stats, void*, U offhand) {
    void* item;
    if (offhand != 0) {                                                             // 005A52B9..CB
        item = item_in_slot(creature_of(stats), 0x10);                              // 005A52CD
        if (item == nullptr) return 2;                                              // 005A52D4..D6
        if (at<U8>(base_item_by_id(item), 8) != 3)                                  // 005A52D8..EE
            item = item_in_slot(creature_of(stats), 0x20);                          // 005A52F0..00
    } else {
        item = item_in_slot(creature_of(stats), 0x10);                              // 005A52C9 / 005A52FB
    }
    if (item == nullptr) return 2;                                                  // 005A5302..04
    return at<U8>(base_item_by_id(item), 0x1f);                                     // 005A5306..18
}

// 005A4EC0 CSWSCreatureStats::ResolveSpecialAttackAttackBonus(unused): RET 4. Current attack +64 (special attack
// feat) -> to-hit modifier (jump table 005A4F28, index bytes 005A4F48 for feat-0Bh in 0..5Ch).
U __fastcall kotor_fastcall_005a4ec0(void* stats, void*, U) {
    void* creature = creature_of(stats);
    void* attack = get_attack(creature, at<U8>(round_of(creature), 0x96c));        // 005A4EC0..D1
    switch (at<U16>(attack, 0x64)) {                                                // 005A4ED6..ED
    case 0x0b: case 0x1e: return static_cast<U>(-4);                                // 005A4EFC
    case 0x11: case 0x12: case 0x1c: case 0x1d: case 0x52: case 0x53: return static_cast<U>(-3); // 005A4EF4
    case 0x5b: case 0x5c: return static_cast<U>(-2);                                // 005A4F04
    case 0x1a: case 0x35: return static_cast<U>(-1);                                // 005A4F0C
    case 0x65: return 0;                                                            // 005A4F12
    case 0x66: return 2;                                                            // 005A4F17
    case 0x67: return 4;                                                            // 005A4F1F
    default: return 0;                                                              // 005A4EDF / 005A4F24
    }
}

// ---------------------------------------------------------------------------------------------------------
// 005A7EA0 CSWSCreatureStats::GetAttackModifierVersus(CSWSCreature* target): RET 4, EAX total to-hit modifier.
// Records each term in the current CSWSCombatAttackData (+C3.. +D5) and, with the debug flag, in its text +78.
U __fastcall kotor_fastcall_005a7ea0(void* stats, void*, void* target) {
    void* creature = creature_of(stats);
    const U index = at<U8>(round_of(creature), 0x96c);                              // 005A7ECC (EDI)
    void* attack = recovered_combat_round_attack_004d2b50(round_of(creature), nullptr, index); // 005A7ED3..DB ([esp+10])
    const U ranged = at<U>(attack, 0x40);                                           // [esp+34]
    const U hand = at<U8>(attack, 0x3a);                                            // EBX / [esp+48]
    void* weapon = kotor_fastcall_004d4ff0(round_of(creature_of(stats)), nullptr, hand); // 005A7EE6..02 ([esp+24])
    if (weapon != nullptr) (void)base_item_by_id(weapon);                           // 005A7F0F..1C (result unused)
    I total = static_cast<I>(kotor_fastcall_005a60d0(stats, nullptr));               // 005A7F21..2B ([esp+0C]; zeroed first)
    if (hand == 2) at<U>(round_of(creature_of(stats)), 0x998) += 1;                 // 005A7F28..47
    at<U8>(attack, 0xc7) = static_cast<U8>(total);                                  // 005A7F4D..55
    if (recovered_debug_cell_008327e4 != 0)                                         // 005A7F5B..62
        debug_term(creature_of(stats), index, k_fmt_bab_0074a94c, total);           // 005A7F68..09
    // off-hand penalty (+CC..+CF) --------------------------------------------------------------------------
    I penalty = 0, light = 0, reduction = 0, feat = 0, applied = 0;                 // [2C] [3C] [14] [1C] [18]
    void* other = item_in_slot(creature_of(stats), 0x20);                           // 005A8024..36 (EBX)
    if (other == nullptr) {                                                         // 005A803A
        other = item_in_slot(creature_of(stats), 0x10);                             // 005A803C..4E
        if (other != nullptr) {                                                     // 005A8050..52
            void* base = base_item_by_id(other);                                    // 005A8054..61
            if (base != nullptr && at<U8>(base, 8) != 3) other = nullptr;           // 005A8066..70
        }
    }
    if (hand == 1) {                                                                // 005A8072..79
        at<U>(attack, 0xc8) = 0;                                                    // 005A8081..85
        if (other != nullptr && at<U8>(base_item_by_id(other), 9) != 0) {           // 005A807F..AB
            penalty = -6; applied = -6;                                             // 005A80B4..BE
            if (kotor_fastcall_004ed190(creature_of(stats), nullptr, other) == -1 ||  // 005A80B9..CA
                at<U8>(base_item_by_id(other), 8) == 3) {                           // 005A80CC..E2
                light = 2; applied = -4;                                            // 005A80E4..EC
            }
            if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x55) != 0) {     // 005A80F4..FF
                reduction = 4; applied += reduction; feat = 0x55;                   // 005A8101..1B
            } else {
                if (recovered_stats_has_feat_005a6630(stats, nullptr, 9) != 0) { reduction = 2; feat = 9; } // 005A8124..39
                applied += reduction;                                               // 005A8141..4B
            }
        }
    } else if (hand == 2) {                                                         // 005A8154..57
        at<U>(attack, 0xc8) = 1;                                                    // 005A815F..63
        if (other != nullptr) {                                                     // 005A816D
            penalty = -10;                                                          // 005A8173
            if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x55) != 0) { reduction = 8; feat = 0x55; }      // 005A816F..94
            else if (recovered_stats_has_feat_005a6630(stats, nullptr, 9) != 0) { reduction = 6; feat = 9; }       // 005A8196..B3
            else if (recovered_stats_has_feat_005a6630(stats, nullptr, 3) != 0) { reduction = 4; feat = 3; }       // 005A81B5..CA
            applied = reduction - 10;                                               // 005A81D2..D9
        }
    }
    total += applied;                                                               // 005A81DD..EF
    at<U8>(attack, 0xcc) = static_cast<U8>(penalty);                                // 005A81F7
    at<U8>(attack, 0xcd) = static_cast<U8>(light);                                  // 005A8201
    at<U8>(attack, 0xce) = static_cast<U8>(reduction);                              // 005A820B
    at<U8>(attack, 0xcf) = static_cast<U8>(feat);                                   // 005A8211
    if (recovered_debug_cell_008327e4 != 0 && applied != 0)                         // 005A8217..2A
        debug_term(creature_of(stats), index, k_fmt_offhand_0074a92c, applied);     // 005A8230..D5
    // special attack (+C6) ----------------------------------------------------------------------------------
    const I special = static_cast<I>(kotor_fastcall_005a4ec0(stats, nullptr, addr32(target))); // 005A82DA..E0 ([esp+48])
    at<U8>(attack, 0xc6) = static_cast<U8>(special);                                // 005A82EF
    total += special;                                                               // 005A82FA..02
    if (recovered_debug_cell_008327e4 != 0 && special != 0)                         // 005A82F5..0E
        debug_term(creature_of(stats), index, k_fmt_special_0074a904, special);     // 005A8314..B5
    // dueling (+D0/+D1): one-handed type 2/4 main-hand weapon and an empty off hand -------------------------
    I dueling = 0, dueling_feat = 0;                                                // EBX, [esp+1C]
    void* main = item_in_slot(creature_of(stats), 0x10);                            // 005A83BA..D2 ([esp+3C])
    void* off = item_in_slot(creature_of(stats), 0x20);                             // 005A83D6..E3
    if (main != nullptr && off == nullptr) {                                        // 005A83E8..F2
        void* base = kotor_m15b_item_get_base_item_005b4790(main, nullptr);         // 005A83F4
        if (base != nullptr && (at<U8>(base, 8) == 2 || at<U8>(base, 8) == 4)) {    // 005A83F9..06
            if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x73) != 0) { dueling = 3; dueling_feat = 0x73; }      // 005A8408..22
            else if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x72) != 0) { dueling = 2; dueling_feat = 0x72; } // 005A8424..3E
            else if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x71) != 0) { dueling = 1; dueling_feat = 0x71; } // 005A8440..52
        }
    }
    at<U8>(attack, 0xd0) = static_cast<U8>(dueling);                                // 005A8466
    at<U8>(attack, 0xd1) = static_cast<U8>(dueling_feat);                           // 005A846C
    total += dueling;                                                               // 005A8477..7B
    // quirk: the debug term is gated on the special-attack modifier ([esp+48]), not on the dueling value
    if (recovered_debug_cell_008327e4 != 0 && special != 0)                         // 005A8472..8B
        debug_term(creature_of(stats), index, k_fmt_dueling_0074a8e4, dueling);     // 005A8491..32
    // good aim (+D2): ranged attack within 5 m (squared distance <= 25) ---------------------------------------
    I good_aim = 0;                                                                 // EBX
    if (ranged != 0 && target != nullptr) {                                         // 005A853B..4A
        void* c = creature_of(stats);
        const double dz = static_cast<double>(at<float>(c, 0x98)) - at<float>(target, 0x98); // 005A854F..55
        const double dy = static_cast<double>(at<float>(c, 0x94)) - at<float>(target, 0x94); // 005A855B..61
        const double dx = static_cast<double>(at<float>(c, 0x90)) - at<float>(target, 0x90); // 005A8567..6D
        const double d2 = (dx * dx + dy * dy) + dz * dz;                            // 005A8573..87 (register only)
        if (d2 <= static_cast<double>(k_good_aim_range2_00745e5c)) good_aim = 10;   // 005A8589..96 (test ah,41h; jp: NaN skips)
    }
    at<U8>(attack, 0xd2) = static_cast<U8>(good_aim);                               // 005A859F
    total += good_aim;                                                              // 005A859B..AE
    if (recovered_debug_cell_008327e4 != 0 && good_aim != 0)                        // 005A85A5..BA
        debug_term(creature_of(stats), index, k_fmt_good_aim_0074a8d0, good_aim);   // 005A85C0..61
    // offensive training (+D3): melee attack, the target's main-hand base item +1A nonzero --------------------
    I training = 0;                                                                 // EBX
    if (ranged == 0 && target != nullptr) {                                         // 005A8666..75
        void* item = item_in_slot(target, 0x10);                                    // 005A8677..81
        if (item != nullptr && at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1a) != 0) // 005A8686..96 (jbe)
            training = 10;                                                          // 005A8698
    }
    at<U8>(attack, 0xd3) = static_cast<U8>(training);                               // 005A86A5
    total += training;                                                              // 005A86A1..B4
    if (recovered_debug_cell_008327e4 != 0 && training != 0)                        // 005A86AB..C0
        debug_term(creature_of(stats), index, k_fmt_training_0074a8a4, training);   // 005A86C6..67
    // ability modifier (+C3 STR, +C4 DEX) ---------------------------------------------------------------------
    I dex = 0, str = 0;                                                             // [esp+1C], [esp+18]
    if (ranged != 0) {                                                              // 005A876C..7C
        dex = static_cast<signed char>(capped_dex(stats));                          // 005A877E..AE
    } else if (kotor_fastcall_005a54f0(stats, nullptr, weapon) == 1) {              // 005A87B4..C3
        dex = static_cast<signed char>(capped_dex(stats));                          // 005A87C5..F5
    } else {
        str = s8(at<U8>(stats, 0xea));                                              // 005A87FB..02
    }
    total += str + dex;                                                             // 005A8806..19
    {
        void* round = round_of(creature_of(stats));                                 // 005A881D (EBX, read once)
        at<U8>(recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c)), 0xc3) = static_cast<U8>(str); // 005A8823..36
        at<U8>(recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c)), 0xc4) = static_cast<U8>(dex); // 005A883C..4F
    }
    if (recovered_debug_cell_008327e4 != 0) {                                       // 005A8855..5C
        DebugText t(creature_of(stats), index);                                     // 005A8862..93
        if (dex != 0) t.add(k_fmt_dex_0074a88c, dex, true);                          // 005A8891..906
        if (str != 0) t.add(k_fmt_str_0074a874, str, true);                          // 005A890B..7F
        t.close();                                                                  // 005A8984..9E
    }
    // weapon focus (+D4): the weapon base item's focus feat (+B2) ---------------------------------------------
    I focus = 0;                                                                    // EBX
    if (weapon != nullptr && kotor_m15b_item_get_base_item_005b4790(weapon, nullptr) != nullptr) { // 005A89A3..B6
        const U16 focus_feat = at<U16>(kotor_m15b_item_get_base_item_005b4790(weapon, nullptr), 0xb2); // 005A89B8..C1
        if (focus_feat != 0 && recovered_stats_has_feat_005a6630(stats, nullptr, focus_feat) != 0) focus = 1; // 005A89C8..D9
    }
    at<U8>(attack, 0xd4) = static_cast<U8>(focus);                                  // 005A89E6
    total += focus;                                                                 // 005A89E2..F5
    if (recovered_debug_cell_008327e4 != 0 && focus != 0)                           // 005A89EC..01
        debug_term(creature_of(stats), index, k_fmt_focus_0074a858, focus);         // 005A8A07..A8
    // effect attack bonus (+D5): GetTotalEffectBonus(1, target object, &zero, 0, 0, 0, 0, FFh, FFh, 0) ----------
    U zero = 0;                                                                     // [esp+44] (005A8ACA)
    const U object = target != nullptr ? addr32(vcall<void*>(target, 0x10)) : 0u;   // 005A8AD3..E5 (AsSWSObject)
    const I effect = static_cast<I>(kotor_fastcall_004f3fe0(creature_of(stats), nullptr, 1, object,
        addr32(&zero), 0, 0, 0, 0, 0xff, 0xff, 0));                                 // 005A8AAD..E9
    at<U8>(attack, 0xd5) = static_cast<U8>(effect);                                 // 005A8AF8
    total += effect;                                                                // 005A8AF4..08
    if (recovered_debug_cell_008327e4 != 0 && effect != 0)                          // 005A8AFE..14
        debug_term(creature_of(stats), index, k_fmt_effect_ab_0074a83c, effect);    // 005A8B1A..BB
    return static_cast<U>(total);                                                   // 005A8BC0..D5
}

// ---------------------------------------------------------------------------------------------------------
// 005AD0E0 CSWSCreatureStats::GetArmorClassVersus(CSWSCreature* attacker, flag): RET 8, AX = AC (callers movsx).
// Records the terms in the attacker's current CSWSCombatAttackData (+F5..+FC).
U __fastcall kotor_fastcall_005ad0e0(void* stats, void*, void* attacker, U flag) {
    const U index = at<U8>(round_of(attacker), 0x96c);                              // 005AD104..0B (EDI, [esp+70])
    void* attack = recovered_combat_round_attack_004d2b50(round_of(attacker), nullptr, index); // 005AD112..20 ([esp+34])
    // class armor bonus (+F8) -----------------------------------------------------------------------------------
    I class_ac = 0;                                                                 // [esp+88] (the attacker slot reused)
    U8 count = at<U8>(stats, 0x89);                                                 // 005AD124
    if (count != 0) {                                                               // 005AD12E..37 (jbe)
        for (U i = 0; ; ) {                                                         // EBX
            const U8 bi = static_cast<U8>(i);
            const U8 cls = bi < count ? at<U8>(stats, bi * 0x28 + 0xa7) : 0xff;     // 005AD140..4F
            unsigned char* cp = static_cast<unsigned char*>(at<void*>(rules(), 0xac)) + cls * 0x1a0u; // 005AD156..65
            U8 level = 0;                                                           // 005AD16F
            if (bi < count) {                                                       // 005AD16B..6D
                const I l = at<U8>(stats, bi * 0x28 + 0xa8);                        // 005AD173..79 (never < 0)
                level = static_cast<U8>(l < 0 ? 0 : l);                             // 005AD181..88
            }
            class_ac += static_cast<signed char>(kotor_m15b_class_get_armor_bonus_005be770(cp, nullptr, level)); // 005AD18A..9D
            count = at<U8>(stats, 0x89);                                            // 005AD19F (re-read)
            ++i;
            if (static_cast<I>(i) >= static_cast<I>(count)) break;                  // 005AD1AF..B2 (jl)
        }
    }
    at<U8>(attack, 0xf8) = static_cast<U8>(class_ac);                               // 005AD1BB..BF
    I total = class_ac + 10;                                                        // 005AD1C5..C8 ([esp+20])
    // natural / armor / shield (+F5 armor, +F9 natural) -----------------------------------------------------------
    I natural = 0, armor = 0, shield = 0;                                           // EBX, [esp+88], [esp+40]
    if (flag == 0) {                                                                // 005AD1CC..E4
        (void)kotor_m15e_slot010_zero_0063e7f0(creature_of(stats), nullptr);       // 005AD1E6..E9 (result unused)
        natural = static_cast<signed char>(static_cast<U8>(at<U8>(stats, 0xfe) - at<U8>(stats, 0xff) + at<U8>(stats, 0xf5))); // 005AD1EE..10
        armor = static_cast<signed char>(static_cast<U8>(at<U8>(stats, 0xf8) - at<U8>(stats, 0xf9) + at<U8>(stats, 0xf6)));   // 005AD200..23
        shield = static_cast<signed char>(static_cast<U8>(at<U8>(stats, 0xfc) - at<U8>(stats, 0xfd) + at<U8>(stats, 0xf7)));  // 005AD21D..45
    }
    total += shield + armor + natural;                                              // 005AD249..57
    at<U8>(attack, 0xf5) = static_cast<U8>(armor);                                  // 005AD25F
    at<U8>(attack, 0xf9) = static_cast<U8>(natural);                                // 005AD265
    if (recovered_debug_cell_008327e4 != 0) {                                       // 005AD26B..72
        DebugText t(attacker, index);                                               // 005AD278..AF
        if (natural != 0) t.add(k_fmt_natural_0074ac50, natural, true);              // 005AD2A5..1B
        if (armor != 0) t.add(k_fmt_armor_0074ac3c, armor, true);                    // 005AD320..95
        if (shield != 0) t.add(k_fmt_shield_0074ac28, shield, true);                 // 005AD39A..0C
        t.close();                                                                  // 005AD411..31
    }
    // dexterity / dodge (+F6 dex, +FA dodge, +F7 capped dex) ---------------------------------------------------
    I dex = 0, dodge = 0;                                                           // EBX, [esp+88]
    if (recovered_creature_invisible_00501950(attacker, nullptr, vcall<void*>(creature_of(stats), 0x10)) == 0) { // 005AD436..54
        void* element = recovered_creature_visible_element_00517a20(creature_of(stats), nullptr, at<U>(attacker, 4)); // 005AD45A..5E
        if (element != nullptr && (at<U8>(element, 4) & 1) != 0) {                  // 005AD463..6F
            I d = s8(at<U8>(stats, 0x100)) - s8(at<U8>(stats, 0x101));              // 005AD475..83
            if (d > 10) d = 10;                                                     // 005AD485..8A
            dodge = static_cast<signed char>(static_cast<U8>(d));                   // 005AD48F..96
            dex = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005a5330(stats, nullptr, 1))); // 005AD492..A2
            if (recovered_debug_cell_008327e4 != 0) {                               // 005AD4A5..AC
                // quirk: the debug text is constructed and destroyed without any term (005AD4B2..FF)
                DebugText t(attacker, index);
                t.close();
            }
        }
    } else {
        I cap = 0;                                                                  // [esp+44]
        bool capped = false;
        if (w5x_creature_is_debilitated_005b4880(creature_of(stats), nullptr) == 0) { // 005AD509..18
            void* worn = item_in_slot(creature_of(stats), 2);                       // 005AD51A..2C ([esp+48])
            if (worn != nullptr && static_cast<I>(kotor_fastcall_00553cc0(worn, nullptr)) > 0) { // 005AD52E..3D
                cap = at<U8>(kotor_m15b_item_get_base_item_005b4790(worn, nullptr), 0xad); // 005AD53F..51
                if (cap >= 0) capped = true;                                        // 005AD555 (always)
            }
        }
        if (!capped && w5x_creature_is_debilitated_005b4880(creature_of(stats), nullptr) != 0) capped = true; // 005AD557..61
        I value;
        if (capped) {
            value = s8(at<U8>(stats, 0xec));                                        // 005AD563
            if (value >= cap) value = cap;                                          // 005AD56A..72
        } else {
            value = s8(at<U8>(stats, 0xec));                                        // 005AD576
        }
        // quirk: only a negative (capped) DEX value takes GetDEXMod(1); otherwise the dex term stays 0
        if (static_cast<signed char>(static_cast<U8>(value)) < 0)                   // 005AD57D..7F (test al,al; jge)
            dex = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005a5330(stats, nullptr, 1))); // 005AD581..8A
    }
    const I shown_dex = static_cast<signed char>(static_cast<U8>(capped_dex(stats))); // 005AD58D..B7
    total += dodge + dex;                                                           // 005AD5B3..C9
    at<U8>(attack, 0xfa) = static_cast<U8>(dodge);                                  // 005AD5CD..D4
    at<U8>(attack, 0xf6) = static_cast<U8>(dex);                                    // 005AD5DA
    at<U8>(attack, 0xf7) = static_cast<U8>(shown_dex);                              // 005AD5E0
    if (recovered_debug_cell_008327e4 != 0) {                                       // 005AD5E6..F0
        DebugText t(attacker, index);                                               // 005AD5F6..2D
        if (dex != 0) t.add(k_fmt_ac_dex_0074ac18, dex, true);                       // 005AD623..99
        if (dodge != 0) t.add(k_fmt_ac_dodge_0074ac04, dodge, true);                 // 005AD69E..13
        t.close();                                                                  // 005AD718..38
    }
    // effect AC (effect types 30h increase / 31h decrease, sorted from the cached index stats +12E) -----------------
    // running maxima seeded from the stats bytes: [40] armor increase (+F8), [24] shield increase (+FC),
    // [30] deflection increase (+FA); [38]/[3C]/[2C] the decrease maxima; [10] dodge sum, [1C] natural sum
    I armor_inc = s8(at<U8>(stats, 0xf8));                                          // 005AD73F..4D
    I shield_inc = s8(at<U8>(stats, 0xfc));                                         // 005AD746..83
    I deflection_inc = s8(at<U8>(stats, 0xfa));                                     // 005AD751..6E
    I natural_bonus = 0, deflection_dec = 0, shield_dec = 0, armor_dec = 0, dodge_bonus = 0; // 005AD758..7F
    void* attacker_stats = at<void*>(attacker, 0xa74);                              // 005AD768
    const std::int16_t alignment = at<std::int16_t>(attacker_stats, 0x17e);                           // 005AD772 (signed word)
    const U align_group = alignment <= 0x28 ? 3u : (alignment >= 0x3c ? 2u : 1u);    // 005AD77B..94 ([esp+60])
    const U race = at<U16>(attacker_stats, 0xdc);                                   // 005AD796 ([esp+58])
    const U damage_flags = kotor_fastcall_004ed5e0(attacker, nullptr);              // 005AD7A0..B2 ([esp+4C])
    void* defender = creature_of(stats);                                            // 005AD7AF ([esp+14])
    if (at<I>(defender, 0x128) > 0) {                                               // 005AD7B6..C2 (jle)
        I i = at<U16>(stats, 0x12e);                                                // 005AD7C8..D5 ([esp+88])
        while (i < at<I>(defender, 0x128)) {                                        // 005AD7D3..DC / 005ADA4B..54 (jl)
            void* effect = at<void**>(defender, 0x124)[i];                          // 005AD7E2..F3
            const U type = at<U16>(effect, 8);                                      // 005AD7F6..0D
            if (type != k_effect_ac_increase_0074673c && type != k_effect_ac_decrease_00746740) break; // 005AD803..12
            const U ac_type = recovered_effect_integer_00503690(effect, nullptr, 0); // 005AD818..25 ([esp+68])
            const I amount = static_cast<I>(recovered_effect_integer_00503690(effect, nullptr, 1)); // 005AD821..32 ([esp+48])
            const U vs_race = recovered_effect_integer_00503690(effect, nullptr, 2); // 005AD82E..3F (EDI)
            const U vs_align = recovered_effect_integer_00503690(effect, nullptr, 4); // 005AD83B..4A (EBX)
            const U vs_damage = recovered_effect_integer_00503690(effect, nullptr, 5); // 005AD846..55 ([esp+44])
            (void)recovered_effect_integer_00503690(effect, nullptr, 6);            // 005AD851..59 (result unused)
            const U any_race = at<U8>(rules(), 0xaa);                               // 005AD85E..64
            if (!(vs_race == any_race && vs_align == 0 && vs_damage == k_damage_all_0074688c)) { // 005AD86E..81
                const bool race_ok = vs_race == any_race || vs_race == race;        // 005AD887..98
                const bool align_ok = vs_align == 0 || vs_align == align_group;     // 005AD89D..AB
                const bool damage_ok = vs_damage == k_damage_all_0074688c ||        // 005AD8B0..E2
                    (vs_damage == k_damage_flag_1_00746850 && damage_flags == 0) ||
                    (vs_damage == k_damage_flag_2_00746854 && damage_flags == 1) ||
                    (vs_damage == k_damage_flag_4_00746858 && damage_flags == 2);
                if (race_ok && align_ok && damage_ok) {                             // 005AD8E7..F9
                    auto increase = [&] { return at<U16>(at<void**>(defender, 0x124)[i], 8) == k_effect_ac_increase_0074673c; };
                    if (ac_type == k_ac_dodge_007468b0)                             // 005AD8FF..43
                        dodge_bonus = increase() ? dodge_bonus + amount : dodge_bonus - amount;
                    if (ac_type == k_ac_natural_007468b4)                           // 005AD94F..7D
                        natural_bonus = increase() ? natural_bonus + amount : natural_bonus - amount;
                    const bool inc = increase();                                    // 005AD981..9E (DI)
                    if (inc) {
                        if (flag == 0 && ac_type == k_ac_armor_007468b8 && amount > armor_inc) armor_inc = amount;   // 005AD9A7..B9
                    } else {
                        if (flag == 0 && ac_type == k_ac_armor_007468b8 && amount > armor_dec) armor_dec = amount;   // 005AD9BF..D1
                    }
                    if (inc) {
                        if (flag == 0 && ac_type == k_ac_shield_007468bc && amount > shield_inc) shield_inc = amount; // 005AD9D5..F0
                    } else {
                        if (flag == 0 && ac_type == k_ac_shield_007468bc && amount > shield_dec) shield_dec = amount; // 005AD9F6..04
                    }
                    if (inc) {
                        if (ac_type == k_ac_deflection_007468c0 && amount > deflection_inc) deflection_inc = amount; // 005ADA08..1D (no flag test)
                    } else {
                        // quirk: the decrease arm compares the AC type with 3 (shield) instead of 4 (deflection)
                        if (flag == 0 && ac_type == k_ac_shield_007468bc && amount > deflection_dec) deflection_dec = amount; // 005ADA23..31
                    }
                }
            }
            ++i;                                                                    // 005ADA35..4D
        }
    }
    if (recovered_debug_cell_008327e4 != 0) {                                       // 005ADA5A..61
        DebugText t(attacker, index);                                               // 005ADA67..9A
        if (dodge_bonus != 0) t.add(k_fmt_eff_dodge_0074abe4, dodge_bonus, true);    // 005ADA98..0E
        if (natural_bonus != 0) t.add(k_fmt_eff_natural_0074abc4, natural_bonus, true); // 005ADB13..85
        if (armor_inc != 0) t.add(k_fmt_eff_armor_0074aba4, armor_dec + armor_inc, true); // 005ADB8A..02 (gated on [40] only)
        if (shield_inc != 0) t.add(k_fmt_eff_shield_0074ab84, shield_dec + shield_inc, true); // 005ADC07..7F
        if (deflection_inc != 0) t.add(k_fmt_eff_deflection_0074ab60, deflection_dec + deflection_inc, true); // 005ADC84..FE
        t.close();                                                                  // 005ADD03..23
    }
    // quirk: the natural-AC effect sum ([1C]) is only reported, never added
    total += deflection_dec + deflection_inc + dodge_bonus;                         // 005ADD2E..56
    at<U8>(attack, 0xfa) = static_cast<U8>(at<U8>(attack, 0xfa) +
        static_cast<U8>(static_cast<U8>(dodge_bonus) + static_cast<U8>(deflection_inc) + static_cast<U8>(deflection_dec))); // 005ADD43..5A
    // quirk: the armor/shield maxima (gathered only when flag == 0) are added only when flag != 0
    if (flag != 0) total += shield_dec + armor_dec + shield_inc + armor_inc;        // 005ADD4D..74
    const I feats = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005a9f50(stats, nullptr))); // 005ADD76..81
    at<U8>(attack, 0xfb) = static_cast<U8>(feats);                                  // 005ADD84
    total += feats;                                                                 // 005ADD8D
    if (w5x_creature_is_debilitated_005b4880(creature_of(stats), nullptr) != 0) {   // 005ADD8A..96
        total -= 4;                                                                 // 005ADD98
        at<U8>(attack, 0xfc) = 0xfc;                                                // 005ADD9B
    }
    return static_cast<U>(total);                                                   // 005ADDA7 (mov ax, si)
}
}  // extern "C"
