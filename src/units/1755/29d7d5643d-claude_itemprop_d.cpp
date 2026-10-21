// Lane effects-itemprop, part D: CSWSItemPropertyHandler::Remove* callbacks 004E97A0..004EA9E0 and the shared
// saving-throw removal helper 004E9390, reconstructed from the original swkotor.exe (read-only, capstone linear
// sweep).  Addresses in the comments are original instructions.  Conventions / ABI: claude_itemprop_common.hpp.
//  * Remove callbacks: thiscall(item, property, creature, slot) RET 10h -> @kotor_fastcall_<addr>@24, EAX = 0.
//  * 004E9390 (called directly with ECX = handler): thiscall(U value, item, property, creature) RET 10h -> @24.
//  * Creature effect list: +124 data, +128 count (re-read every iteration as the original does).  Several loops
//    start at a per-type first index kept in the creature stats ([creature+A74] WORD +12E/+130/+132/+134/+144/+158)
//    and stop at the first effect whose type is greater than the wanted one (the list is sorted by type).
//  * Every loop removes only the first matching effect, then returns.
// Original quirks kept (see report):
//  * 004E97A0 passes GetIPRPCostTable the DWORD argument slot of `property` whose low byte was overwritten with
//    property BYTE +4 (only the low byte is used by 005C49A0), and its GetINTEntry out cell is that same slot, so a
//    failed lookup compares against (property & FFFFFF00h) | byte4.
//  * 004E9CF0 / 004E9E30 use the `property` argument slot as the out cell (unwritten on a bad row -> the pointer).
//  * 004E9990 / 004E9BB0 / 004E9F60 / 004EA130 / 004EA290 / 004EA890 / 004EA940 never initialise their out cell.
//  * 004E9390 requires GetInteger(1) == 0 and then GetInteger(1) in {1,2,3}: the removal can never happen.
//  * 004E9F60 removes a matching effect without the GetInteger(0) check when the property subtype is > 5.
#include "9bb88ebc0b-claude_itemprop_common.hpp"

extern "C" {
unsigned __fastcall kotor_fastcall_004e9390(void* self, void*, unsigned value, void* item, void* property, void* creature);
unsigned __fastcall kotor_fastcall_004e97a0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9990(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9bb0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9cf0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9e30(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004e9f60(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea130(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea290(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea3f0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea580(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea650(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea6e0(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea770(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea800(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea890(void* self, void*, void* item, void* property, void* creature, unsigned slot);
unsigned __fastcall kotor_fastcall_004ea940(void* self, void*, void* item, void* property, void* creature, unsigned slot);
}

using namespace ipx;

namespace {
// ---- .rdata effect types ----
constexpr I k_effect_regenerate_00746698 = 0x07;       // .rdata 00746698
constexpr I k_effect_resistance_00746684 = 0x02;       // .rdata 00746684 (damage resistance)
constexpr I k_effect_reduction_007466ac = 0x0c;        // .rdata 007466AC (damage reduction)
constexpr I k_effect_damage_dec_007466b4 = 0x0e;       // .rdata 007466B4
constexpr I k_effect_dmg_immunity_007466bc = 0x10;     // .rdata 007466BC
constexpr I k_effect_dmg_vuln_007466c0 = 0x11;         // .rdata 007466C0 (damage immunity decrease)
constexpr I k_effect_immunity_007466d4 = 0x16;         // .rdata 007466D4
constexpr I k_effect_save_inc_007466e4 = 0x1a;         // .rdata 007466E4
constexpr I k_effect_save_dec_007466e8 = 0x1b;         // .rdata 007466E8
constexpr I k_effect_ability_dec_00746710 = 0x25;      // .rdata 00746710
constexpr I k_effect_ac_dec_00746740 = 0x31;           // .rdata 00746740
constexpr I k_effect_skill_inc_00746758 = 0x37;        // .rdata 00746758
constexpr I k_effect_skill_dec_0074675c = 0x38;        // .rdata 0074675C
constexpr I k_effect_disguise_00746774 = 0x3e;         // .rdata 00746774
constexpr I k_effect_deflect_inc_007467e8 = 0x5c;      // .rdata 007467E8
constexpr I k_effect_deflect_dec_007467ec = 0x5d;      // .rdata 007467EC
// ---- .rdata tables ----
// 004E9849 jump table 004E994C: damage-type bit for subtype 0..13 (.rdata 00746850..00746884)
constexpr U k_damage_bits_00746850[14] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000};
// 004EA067 jump table 004EA10C: ability for subtype 0..5 (.rdata 00746838..0074684C)
constexpr U k_ability_00746838[6] = {0, 1, 2, 3, 4, 5};
// weapon-slot -> effect integer 5 (.rdata 007468E4..007468F8)
constexpr U k_slot_007468e4 = 1, k_slot_007468e8 = 2, k_slot_007468ec = 3, k_slot_007468f0 = 4, k_slot_007468f4 = 5,
            k_slot_007468f8 = 7;
// 004EA41D jump table 004EA558: immunity per subtype 0..9 (.rdata 0074696C, 00746968+00746940, 00746900..)
constexpr U k_immunity_0074696c = 0x1e, k_immunity_00746968 = 0x1d, k_immunity_00746940 = 0x13,
            k_immunity_00746900 = 1, k_immunity_00746904 = 2, k_immunity_00746908 = 3, k_immunity_0074690c = 4,
            k_immunity_00746964 = 0x1c, k_immunity_00746914 = 6, k_immunity_00746970 = 0x1f, k_immunity_00746974 = 0x20;
const char k_amount_00746a00[] = "Amount";             // .rdata 00746A00

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline I count(void* creature) { return at<I>(creature, 0x128); }
inline void* effect_at(void* creature, I i) { return at<void**>(creature, 0x124)[i]; }
inline I type_of(void* e) { return at<U16>(e, 8); }                                        // movzx word [e+8]
inline bool equipped(void* e) { return duration_bits(e) == k_duration_equipped_007473bc; }
inline bool from_item(void* e, void* item) { return at<U>(e, 0x18) == at<U>(item, 0x14); }
inline I first_index(void* creature, U offset) { return at<U16>(at<void*>(creature, 0xa74), offset); }
// CExoString s(column); table->GetINTEntry(row, s, out); ~s
inline void cost_int_named(void* table, const char* column, U row, I* out) {
    ExoString s;
    frame_LogStringCtor_005e5a90(&s, nullptr, column);
    c2da_current_get_int_00413660(table, nullptr, row, &s, out);
    frame_LogStringDtor_005e5c20(&s, nullptr);
}
}  // namespace

extern "C" {

// ===================================================================================================== 004E9390
// CSWSItemPropertyHandler::(Remove)ChangedSavingThrow helper(value, item, property, creature) RET 10h.
unsigned __fastcall kotor_fastcall_004e9390(void*, void*, unsigned value, void* item, void* property, void* creature) {
    const U subtype = prop_subtype(property);                                              // 004E9390..9394, 93AA
    for (I i = 0; i < count(creature); ++i) {                                              // 004E939E..93AE, 9469..9472
        void* e = effect_at(creature, i);                                                  // 004E93C0..93C6
        const I t = type_of(e);                                                            // 004E93C9
        if (t != k_effect_save_inc_007466e4 && t != k_effect_save_dec_007466e8) continue;  // 004E93CD..93DB
        if (!equipped(e)) continue;                                                        // 004E93E1..93F0
        if (!from_item(e, item)) continue;                                                 // 004E93F2..93FC
        if (get_int(e, 0) != value) continue;                                              // 004E93FE..940B
        if (get_int(e, 1) != 0) continue;                                                  // 004E940D..9418
        const U racial = racial_all();                                                     // 004E941A..941F
        if (get_int(e, 3) != racial) continue;                                             // 004E9425..9433
        const U which = get_int(effect_at(creature, i), 1);                                // 004E9435..9440
        bool match;
        if (which == 1) match = subtype == 1;                                              // 004E9445..944F
        else if (which == 3) match = subtype == 3;                                         // 004E9451..945B
        else if (which == 2) match = subtype == 2;                                         // 004E945D..9462
        else match = false;                                                                // 004E9460 -> 9469
        if (match) {                                                                       // 004E9467
            remove_by_id(creature, effect_at(creature, i));                                // 004E9481..9493
            return 0;                                                                      // 004E9498..949E
        }
    }
    return 0;                                                                              // 004E9478..947E
}

// ===================================================================================================== 004E97A0
// CSWSItemPropertyHandler::RemoveDamageImmunity
unsigned __fastcall kotor_fastcall_004e97a0(void*, void*, void* item, void* property, void* creature, unsigned) {
    const U subtype = prop_subtype(property);                                              // 004E97BF..97C3
    if (addr(creature) + 0x124u == 0) return 0;                                            // 004E97C7..97CF
    // mov al,[property+4]; mov byte [esp+28h],al; mov edx,[esp+28h]: the property argument slot, low byte replaced
    I slot_cell = static_cast<I>((addr(property) & 0xffffff00u) | at<U8>(property, 4));    // 004E97D5..97E2
    void* table = kotor_fastcall_005c49a0(two_dim(), nullptr, static_cast<U>(slot_cell));  // 004E97D8..97EE
    if (table == nullptr) return 0;                                                        // 004E97F3..97F7
    cost_int_named(table, k_value_00744bfc, prop_cost(property), &slot_cell);              // 004E97FD..9838 (out = slot)
    if (subtype > 0xd) return 0;                                                           // 004E983D..9843
    const U bit = k_damage_bits_00746850[subtype];                                         // 004E9849..98B8
    for (I i = 0; i < count(creature); ++i) {                                              // 004E98BE..98C5, 991B..9921
        void* e = effect_at(creature, i);                                                  // 004E98D0..98D3
        if (type_of(e) != k_effect_dmg_immunity_007466bc) continue;                        // 004E98D6..98E0
        if (get_int(e, 0) != bit) continue;                                                // 004E98E2..98ED
        if (get_int(e, 1) != static_cast<U>(slot_cell)) continue;                          // 004E98EF..98FC
        if (!equipped(e)) continue;                                                        // 004E98FE..990D
        if (!from_item(e, item)) continue;                                                 // 004E990F..9919
        remove_by_id(creature, e);                                                         // 004E9925..9930
        return 0;
    }
    return 0;                                                                              // 004E9935..9949
}

// ===================================================================================================== 004E9990
// CSWSItemPropertyHandler::RemoveDamagePenalty
unsigned __fastcall kotor_fastcall_004e9990(void*, void*, void* item, void* property, void* creature, unsigned slot) {
    I amount;                                                                              // [esp+4]: never initialised
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x14), prop_cost(property), &amount); // 004E99A9..99FB
    amount = -amount;                                                                      // 004E9A00..9A10
    for (I i = first_index(creature, 0x130); i < count(creature); ++i) {                  // 004E9A08..9A21, 9B66..9B73
        void* e = effect_at(creature, i);                                                  // 004E9A30..9A36
        const I t = type_of(e);                                                            // 004E9A39..9A3D
        if (t > k_effect_damage_dec_007466b4) return 0;                                    // 004E9A42..9A44 (jg)
        if (t != k_effect_damage_dec_007466b4) continue;                                   // 004E9A4A..9A50
        if (!equipped(e)) continue;                                                        // 004E9A56..9A65
        if (!from_item(e, item)) continue;                                                 // 004E9A6B..9A75
        if (get_int(e, 0) != static_cast<U>(amount)) continue;                             // 004E9A7B..9A88
        const U racial = racial_all();                                                     // 004E9A8E..9A93
        if (get_int(e, 2) != racial) continue;                                             // 004E9A99..9AA7
        U want;
        if (slot == 0x10) want = k_slot_007468e4;                                          // 004E9AAD..9AC7
        else if (slot == 0x20) want = k_slot_007468e8;                                     // 004E9ACC..9AE2
        else if (slot == 0x4000) want = k_slot_007468ec;                                   // 004E9AE7..9B00
        else if (slot == 0x8000) want = k_slot_007468f0;                                   // 004E9B02..9B1B
        else if (slot == 0x10000) want = k_slot_007468f4;                                  // 004E9B1D..9B36
        else if (slot == 8) want = k_slot_007468f8;                                        // 004E9B38..9B4C
        else continue;                                                                     // 004E9B3B -> 9B66
        if (get_int(e, 5) != want) continue;                                               // GetInteger(5) per branch
        const U flags = get_int(e, 1);                                                     // 004E9B4E..9B5B
        if ((kotor_fastcall_00554050(item, nullptr) & flags) == 0) continue;               // 004E9B57..9B64
        remove_by_id(creature, e);                                                         // 004E9B7B..9B86
        return 0;
    }
    return 0;                                                                              // 004E9B8B..9B9F
}

// ===================================================================================================== 004E9BB0
// CSWSItemPropertyHandler::RemoveDamageReduction
unsigned __fastcall kotor_fastcall_004e9bb0(void*, void*, void* item, void* property, void* creature, unsigned) {
    I amount;                                                                              // [esp+8]: never initialised
    cost_int_named(kotor_fastcall_005c49a0(two_dim(), nullptr, 6), k_amount_00746a00, prop_cost(property), &amount); // 004E9BCA..9C1C
    for (I i = first_index(creature, 0x132); i < count(creature); ++i) {                  // 004E9C21..9C38, 9CB3..9CBC
        void* e = effect_at(creature, i);                                                  // 004E9C50..9C56
        const I t = type_of(e);
        if (t > k_effect_reduction_007466ac) return 0;                                     // 004E9C59..9C5F (jg)
        if (t != k_effect_reduction_007466ac) continue;                                    // 004E9C61..9C67
        if (!equipped(e)) continue;                                                        // 004E9C69..9C78
        if (!from_item(e, item)) continue;                                                 // 004E9C7A..9C84
        if (get_int(e, 0) != static_cast<U>(amount)) continue;                             // 004E9C86..9C93
        const U subtype = prop_subtype(property);                                          // 004E9C95..9C99
        if (get_int(e, 1) != subtype + 1u) continue;                                       // 004E9C9D..9CAC
        remove_by_id(creature, e);                                                         // 004E9CC0..9CC9
        return 0;
    }
    return 0;                                                                              // 004E9CCE..9CE2
}

// ===================================================================================================== 004E9CF0
// CSWSItemPropertyHandler::RemoveDamageResistance
unsigned __fastcall kotor_fastcall_004e9cf0(void*, void*, void* item, void* property, void* creature, unsigned) {
    const U subtype = prop_subtype(property);                                              // 004E9D0B..9D0F
    I amount = static_cast<I>(addr(property));                                             // out cell = property slot [esp+2Ch]
    cost_int_named(kotor_fastcall_005c49a0(two_dim(), nullptr, 7), k_amount_00746a00, prop_cost(property), &amount); // 004E9D14..9D61
    for (I i = first_index(creature, 0x134); i < count(creature); ++i) {                  // 004E9D66..9D7D, 9DF0..9DF9
        void* e = effect_at(creature, i);                                                  // 004E9D90..9D96
        const I t = type_of(e);
        if (t > k_effect_resistance_00746684) return 0;                                    // 004E9D99..9D9F (jg)
        if (t != k_effect_resistance_00746684) continue;                                   // 004E9DA1..9DA7
        if (!equipped(e)) continue;                                                        // 004E9DA9..9DB8
        if (!from_item(e, item)) continue;                                                 // 004E9DBA..9DC4
        if (get_int(e, 0) != (1u << (subtype & 31u))) continue;                            // 004E9DC6..9DDA (shl edx,cl)
        if (get_int(e, 1) != static_cast<U>(amount)) continue;                             // 004E9DDC..9DE9
        remove_by_id(creature, e);                                                         // 004E9DFD..9E06
        return 0;
    }
    return 0;                                                                              // 004E9E0B..9E1F
}

// ===================================================================================================== 004E9E30
// CSWSItemPropertyHandler::RemoveDamageVulnerability
unsigned __fastcall kotor_fastcall_004e9e30(void*, void*, void* item, void* property, void* creature, unsigned) {
    const U subtype = prop_subtype(property);                                              // 004E9E4B..9E4F
    I amount = static_cast<I>(addr(property));                                             // out cell = property slot [esp+2Ch]
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x16), prop_cost(property), &amount); // 004E9E54..9E9F
    if (count(creature) <= 0) return 0;                                                    // 004E9EA4..9EB0
    for (I i = 0;; ) {
        void* e = effect_at(creature, i);                                                  // 004E9EC0..9EC6
        const I t = type_of(e);
        if (t > k_effect_dmg_vuln_007466c0) return 0;                                      // 004E9EC9..9ECF (jg)
        if (t == k_effect_dmg_vuln_007466c0 && equipped(e) && from_item(e, item) &&        // 004E9ED1..9EF4
            get_int(e, 0) == (1u << (subtype & 31u)) &&                                    // 004E9EF6..9F0A
            get_int(e, 1) == static_cast<U>(amount)) {                                     // 004E9F0C..9F19
            remove_by_id(creature, e);                                                     // 004E9F2D..9F36
            return 0;
        }
        if (++i >= count(creature)) return 0;                                              // 004E9F20..9F29
    }
}

// ===================================================================================================== 004E9F60
// CSWSItemPropertyHandler::RemoveDecreasedAbilityScore
unsigned __fastcall kotor_fastcall_004e9f60(void*, void*, void* item, void* property, void* creature, unsigned) {
    I amount;                                                                              // [esp+0Ch]: never initialised
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x15), prop_cost(property), &amount); // 004E9F7B..9FCD
    amount = -amount;                                                                      // 004E9FD2..9FE8
    for (I i = first_index(creature, 0x144); i < count(creature); ++i) {                  // 004E9FD6..9FF5, A0D6..A0DF
        void* e = effect_at(creature, i);                                                  // 004EA000..A006
        const I t = type_of(e);
        if (t > k_effect_ability_dec_00746710) return 0;                                   // 004EA009..A014 (jg)
        if (t != k_effect_ability_dec_00746710) continue;                                  // 004EA01A..A020
        if (!equipped(e)) continue;                                                        // 004EA026..A035
        if (!from_item(e, item)) continue;                                                 // 004EA03B..A045
        if (get_int(e, 1) != static_cast<U>(amount)) continue;                             // 004EA04B..A058
        const U subtype = prop_subtype(property);                                          // 004EA05A
        if (subtype <= 5 && get_int(e, 0) != k_ability_00746838[subtype]) continue;        // 004EA05E..A0D4 (ja -> remove)
        remove_by_id(creature, e);                                                         // 004EA0E7..A0F0
        return 0;
    }
    return 0;                                                                              // 004EA0F5..A109
}

// ===================================================================================================== 004EA130
// CSWSItemPropertyHandler::RemoveDecreasedAC
unsigned __fastcall kotor_fastcall_004ea130(void*, void*, void* item, void* property, void* creature, unsigned) {
    I amount;                                                                              // [esp+8]: never initialised
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x14), prop_cost(property), &amount); // 004EA14A..A19C
    amount = -amount;                                                                      // 004EA1A1..A1B7
    for (I i = first_index(creature, 0x12e); i < count(creature); ++i) {                  // 004EA1A5..A1C4, A257..A260
        void* e = effect_at(creature, i);                                                  // 004EA1D1..A1D7
        const I t = type_of(e);
        if (t > k_effect_ac_dec_00746740) return 0;                                        // 004EA1DA..A1E0 (jg)
        if (t != k_effect_ac_dec_00746740) continue;                                       // 004EA1E6..A1EC
        if (!equipped(e)) continue;                                                        // 004EA1EE..A1FD
        if (!from_item(e, item)) continue;                                                 // 004EA1FF..A209
        const U subtype = prop_subtype(property);                                          // 004EA20B..A20F
        if (get_int(e, 0) != subtype) continue;                                            // 004EA213..A221
        if (get_int(e, 1) != static_cast<U>(amount)) continue;                             // 004EA223..A230
        const U racial = racial_all();                                                     // 004EA232..A238
        if (get_int(e, 2) != racial) continue;                                             // 004EA23E..A24C
        remove_by_id(creature, e);                                                         // 004EA268..A273
        return 0;
    }
    return 0;                                                                              // 004EA278..A28C
}

// ===================================================================================================== 004EA290
// CSWSItemPropertyHandler::RemoveDecreasedSkillModifier
unsigned __fastcall kotor_fastcall_004ea290(void*, void*, void* item, void* property, void* creature, unsigned) {
    I amount;                                                                              // [esp+8]: never initialised
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x15), prop_cost(property), &amount); // 004EA2AA..A2FC
    amount = -amount;                                                                      // 004EA301..A317
    for (I i = first_index(creature, 0x158); i < count(creature); ++i) {                  // 004EA305..A324, A3B7..A3C0
        void* e = effect_at(creature, i);                                                  // 004EA331..A337
        const I t = type_of(e);
        if (t > k_effect_skill_dec_0074675c) return 0;                                     // 004EA33A..A340 (jg)
        if (t != k_effect_skill_dec_0074675c) continue;                                    // 004EA346..A34C
        if (!equipped(e)) continue;                                                        // 004EA34E..A35D
        if (!from_item(e, item)) continue;                                                 // 004EA35F..A369
        const U subtype = prop_subtype(property);                                          // 004EA36B..A36F
        if (get_int(e, 0) != subtype) continue;                                            // 004EA373..A381
        if (get_int(e, 1) != static_cast<U>(amount)) continue;                             // 004EA383..A390
        const U racial = racial_all();                                                     // 004EA392..A398
        if (get_int(e, 2) != racial) continue;                                             // 004EA39E..A3AC
        remove_by_id(creature, e);                                                         // 004EA3C8..A3D3
        return 0;
    }
    return 0;                                                                              // 004EA3D8..A3EC
}

// ===================================================================================================== 004EA3F0
// CSWSItemPropertyHandler::RemoveImmunity
unsigned __fastcall kotor_fastcall_004ea3f0(void*, void*, void* item, void* property, void* creature, unsigned) {
    U second_enabled = 0;                                                                  // [esp+8] (push ecx slot)
    U second = addr(property);                                                             // property slot, reused
    if (addr(creature) + 0x124u == 0) return 0;                                            // 004EA3F3..A405
    const U subtype = prop_subtype(property);                                              // 004EA40B..A40F
    U immunity;
    switch (subtype) {                                                                     // 004EA413..A41D (JT 004EA558)
    case 0: immunity = k_immunity_0074696c; break;                                         // 004EA424
    case 1:                                                                                // 004EA42C..A444
        second = k_immunity_00746940;
        immunity = k_immunity_00746968;
        second_enabled = 1;
        break;
    case 2: immunity = k_immunity_00746900; break;                                         // 004EA446
    case 3: immunity = k_immunity_00746904; break;                                         // 004EA44E
    case 4: immunity = k_immunity_00746908; break;                                         // 004EA456
    case 5: immunity = k_immunity_0074690c; break;                                         // 004EA45E
    case 6: immunity = k_immunity_00746964; break;                                         // 004EA466
    case 7: immunity = k_immunity_00746914; break;                                         // 004EA46E
    case 8: immunity = k_immunity_00746970; break;                                         // 004EA476
    case 9: immunity = k_immunity_00746974; break;                                         // 004EA47E
    default: return 0;                                                                     // 004EA417 (ja)
    }
    for (I i = 0; i < count(creature); ++i) {                                              // 004EA484..A487, A50C..A512
        void* e = effect_at(creature, i);                                                  // 004EA490..A493
        if (type_of(e) == k_effect_immunity_007466d4 && get_int(e, 0) == immunity &&       // 004EA496..A4AD
            equipped(e) && from_item(e, item)) {                                           // 004EA4AF..A4CA
            remove_by_id(creature, e);                                                     // 004EA522..A52D
            return 0;
        }
        if (second_enabled != 0 && type_of(e) == k_effect_immunity_007466d4 &&             // 004EA4CC..A4DE
            get_int(e, 0) == second && equipped(e) && from_item(e, item)) {                // 004EA4E0..A50A
            remove_by_id(creature, e);                                                     // 004EA53C..A547
            return 0;
        }
    }
    return 0;                                                                              // 004EA518..A51F / A54D..A553
}

// ===================================================================================================== 004EA580
// CSWSItemPropertyHandler::RemoveSkillBonus
unsigned __fastcall kotor_fastcall_004ea580(void*, void*, void* item, void* property, void* creature, unsigned) {
    for (I i = first_index(creature, 0x130); i < count(creature); ++i) {                  // 004EA582..A599, A617..A620
        void* e = effect_at(creature, i);                                                  // 004EA5B0..A5B6
        const I t = type_of(e);
        if (t > k_effect_skill_inc_00746758) return 0;                                     // 004EA5B9..A5BF (jg)
        if (t != k_effect_skill_inc_00746758) continue;                                    // 004EA5C1..A5C7
        if (!equipped(e)) continue;                                                        // 004EA5C9..A5D8
        if (!from_item(e, item)) continue;                                                 // 004EA5DA..A5E4
        const U subtype = prop_subtype(property);                                          // 004EA5E6
        if (get_int(e, 0) != subtype) continue;                                            // 004EA5EA..A5F8
        const U cost = prop_cost(property);                                                // 004EA5FA
        if (get_int(e, 1) != cost) continue;                                               // 004EA5FE..A60C
        remove_by_id(creature, e);                                                         // 004EA62B..A636
        return 0;
    }
    return 0;                                                                              // 004EA622..A628 / A63B..A641
}
}  // extern "C"

namespace {
// 004EA650 / 004EA6E0 / 004EA770 / 004EA800 share one instruction sequence; only the effect type and the
// property key (WORD +6 cost value, or WORD +2 subtype for 004EA800) differ.
U remove_first_by_type_key(void* item, void* creature, I type, U key) {
    if (addr(creature) + 0x124u == 0) return 0;                                            // add edi,124h; test edi,edi
    for (I i = 0; i < count(creature); ++i) {                                              // mov eax,[edi+4]; test; jle / inc; cmp; jl
        void* e = effect_at(creature, i);
        if (type_of(e) != type) continue;                                                  // movzx; cmp [type]
        if (get_int(e, 0) != key) continue;                                                // GetInteger(0); cmp ebp
        if (!from_item(e, item)) continue;                                                 // [e+18] vs [item+14]
        if (!equipped(e)) continue;                                                        // flags & 7 == [007473BC]
        remove_by_id(creature, e);                                                         // RemoveEffectById
        return 0;
    }
    return 0;
}
}  // namespace

extern "C" {
// ===================================================================================================== 004EA650
// CSWSItemPropertyHandler::RemoveRegeneration (004EA650..A6D5)
unsigned __fastcall kotor_fastcall_004ea650(void*, void*, void* item, void* property, void* creature, unsigned) {
    return remove_first_by_type_key(item, creature, k_effect_regenerate_00746698, prop_cost(property));
}
// ===================================================================================================== 004EA6E0
// CSWSItemPropertyHandler::RemoveBlasterBoltDeflectionIncrease (004EA6E0..A765)
unsigned __fastcall kotor_fastcall_004ea6e0(void*, void*, void* item, void* property, void* creature, unsigned) {
    return remove_first_by_type_key(item, creature, k_effect_deflect_inc_007467e8, prop_cost(property));
}
// ===================================================================================================== 004EA770
// CSWSItemPropertyHandler::RemoveBlasterBoltDeflectionDecrease (004EA770..A7F5)
unsigned __fastcall kotor_fastcall_004ea770(void*, void*, void* item, void* property, void* creature, unsigned) {
    return remove_first_by_type_key(item, creature, k_effect_deflect_dec_007467ec, prop_cost(property));
}
// ===================================================================================================== 004EA800
// CSWSItemPropertyHandler::RemoveDisguise (004EA800..A885): key is the property subtype (WORD +2)
unsigned __fastcall kotor_fastcall_004ea800(void*, void*, void* item, void* property, void* creature, unsigned) {
    return remove_first_by_type_key(item, creature, k_effect_disguise_00746774, prop_subtype(property));
}

// ===================================================================================================== 004EA890
// CSWSItemPropertyHandler::RemoveImprovedSavingThrow
unsigned __fastcall kotor_fastcall_004ea890(void* self, void*, void* item, void* property, void* creature, unsigned) {
    I amount;                                                                              // [esp+8]: never initialised
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 1), prop_cost(property), &amount); // 004EA8AC..A8FE
    if (amount != 0)                                                                       // 004EA903..A909
        kotor_fastcall_004e9390(self, nullptr, static_cast<U>(amount), item, property, creature); // 004EA90B..A919
    return 0;                                                                              // 004EA91E..A930
}

// ===================================================================================================== 004EA940
// CSWSItemPropertyHandler::RemoveReducedSavingThrow
unsigned __fastcall kotor_fastcall_004ea940(void* self, void*, void* item, void* property, void* creature, unsigned) {
    I amount;                                                                              // [esp+8]: never initialised
    cost_int(kotor_fastcall_005c49a0(two_dim(), nullptr, 0x14), prop_cost(property), &amount); // 004EA95C..A9AE
    if (amount != 0)                                                                       // 004EA9B3..A9B9
        kotor_fastcall_004e9390(self, nullptr, static_cast<U>(amount), item, property, creature); // 004EA9BB..A9C9
    return 0;                                                                              // 004EA9CE..A9E0
}

}  // extern "C"
