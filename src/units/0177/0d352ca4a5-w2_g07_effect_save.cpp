// W2 tier 4: GetTotalEffectBonus 004F3FE0, case 3 (saving throws). See w2_g07_effect_save.hpp.
//
// Creature fields used (offsets from the creature `self`):
//   +124 / +128   effect list {data, count}; data[i] -> CGameEffect*
//   +9C8          combat round; byte [round+96C] = current attack index (GetAttack is called, result byte +3A unused)
//   +A74          stats; word [stats+15A] = first index of the saving-throw effects in the sorted list
//   +568 [36]     positive values  (reset to -1)       +5F8 [36]  negative values (reset to -1)
//   +688 [36]     positive tags    (reset to -1)       +718 [36]  negative tags   (reset to -1)
//   +7A8 [36]     positive item ids (reset 7F000000h)  +838 [36]  negative item ids (reset 7F000000h)
// CGameEffect fields: word +08 type (1Ah [007466E4] bonus, 1Bh [007466E8] penalty; the list is sorted by type and the
// scan stops at the first type > 1Bh), +18 creator item id, +1C tag (-1 none); integers 0 value, 1 save type, 2 vs type,
// 3 race, 5 alignment.
#include "6e34a1e9b9-w2_g07_effect_save.hpp"
#include <cstdint>
namespace w2::g07 {
namespace {
using I = std::int32_t;
constexpr Word kBonus = 0x1a, kPenalty = 0x1b, kNone = 0xffffffffu, kInvalid = 0x7f000000u;

// One bonus / penalty effect into its slot set (004F51F4..004F52D2 for +568, 004F5393..004F5511 for +5F8).
void place(Memory& m, EffectSavePorts& p, Word self, Word e, Word values, Word server) {
    const Word item = p.item_by_id_004ae760(server, m.read(e + 0x18));
    Word ids, key; bool search;
    if (item != 0) { ids = values + 0x240; key = m.read(item + 0x14); search = true; }
    else { ids = values + 0x120; key = m.read(e + 0x1c); search = key != kNone; }   // 004F52DB: tag -1 skips the search
    if (search) {
        bool found = false;
        for (Word k = 0; k < 36; ++k) {
            if (m.read(self + ids + 4 * k) != key) continue;
            found = true;
            const Word v = p.effect_integer_00503690(e, 0);
            if (static_cast<I>(m.read(self + values + 4 * k)) < static_cast<I>(v)) {      // JL: keep the larger
                m.write(self + values + 4 * k, p.effect_integer_00503690(e, 0));
                return;
            }
        }
        if (found) return;
    }
    for (Word k = 0; k < 36; ++k) {                                       // first free value slot
        if (m.read(self + values + 4 * k) != kNone) continue;
        m.write(self + ids + 4 * k, key);
        m.write(self + values + 4 * k, p.effect_integer_00503690(e, 0));
        return;
    }
}
}

Word effect_save_004f3fe0(Memory& m, EffectSavePorts& p, Word self, Word a1, Word target, Word out, Word a4, Word a5,
                          Word a6, Word a7, Word a8, Word a9, Word a10) {
    (void)a4; (void)a5; (void)a8; (void)a9; (void)a10;
    m.write(out, 0);                                                      // 004F400B
    const Word round = m.read(self + 0x9c8);
    const Word attack = p.combat_attack_004d2b50(round, m.read8(round + 0x96c));
    (void)m.read8(attack + 0x3a);                                         // 004F4034: attack type, unused by case 3
    if (target != 0) p.trap(0x004f405e, "W2: GetTotalEffectBonus with a target (race / alignment lookup) not reconstructed");
    const Word race = kNone, alignment = kNone;                          // 004F409C: no target
    for (Word k = 0; k < 36; ++k) {                                       // 004F40A4 reset
        m.write(self + 0x568 + 4 * k, kNone); m.write(self + 0x5f8 + 4 * k, kNone);
        m.write(self + 0x688 + 4 * k, kNone); m.write(self + 0x718 + 4 * k, kNone);
        m.write(self + 0x7a8 + 4 * k, kInvalid); m.write(self + 0x838 + 4 * k, kInvalid);
    }
    const Word kind = a1 & 0xffu;                                         // 004F40F8 movzx eax, byte [ebp+8]
    if (kind == 0 || kind > 5) return 0;                                  // 004F4100 ja 004F5F38: xor eax,eax; ret 28h
    if (kind == 1) p.trap(0x004f410d, "W2: GetTotalEffectBonus case 1 (attack) not reconstructed");
    if (kind == 2) p.trap(0x004f4491, "W2: GetTotalEffectBonus case 2 (damage) not reconstructed");
    if (kind == 4) p.trap(0x004f5639, "W2: GetTotalEffectBonus case 4 here (ability is the 0886 route of the 004F3FE0 entry)");
    if (kind == 5) p.trap(0x004f5a78, "W2: GetTotalEffectBonus case 5 here (skill is the 0886 route of the 004F3FE0 entry)");
    const Word list = self + 0x124;
    if (static_cast<I>(m.read(list + 4)) > 0) {
        Word i = m.read16(m.read(self + 0xa74) + 0x15a);
        if (static_cast<I>(i) < static_cast<I>(m.read(list + 4))) {
            do {
                const Word e = m.read(m.read(list) + 4 * i);
                const Word type = m.read16(e + 8);
                if (static_cast<I>(type) > static_cast<I>(kPenalty)) break;   // 004F5165
                (void)p.effect_integer_00503690(e, 0);
                const Word save = p.effect_integer_00503690(e, 1);
                const Word versus = p.effect_integer_00503690(e, 2);
                const Word race_value = p.effect_integer_00503690(e, 3);
                const Word align = p.effect_integer_00503690(e, 5);
                const bool keep = (save == 0 || save == (a6 & 0xffu))
                               && (versus == 0 || versus == (a7 & 0xffu))
                               && (race_value == m.read8(p.rules_007a3a28() + 0xaa) || race_value == race)
                               && (align == 0 || align == alignment);
                if (keep) {
                    const Word server_now = m.read(p.app_007a39fc() + 8);   // [[007A39FC]+8] per lookup
                    if (type == kBonus) place(m, p, self, e, 0x568, server_now);
                    else if (type == kPenalty) place(m, p, self, e, 0x5f8, server_now);
                }
                ++i;
            } while (static_cast<I>(i) < static_cast<I>(m.read(list + 4)));   // count re-read (004F5520)
        }
    }
    I positive = 0, negative = 0;
    for (Word k = 0; k < 36; ++k) {
        const Word a = m.read(self + 0x568 + 4 * k), b = m.read(self + 0x5f8 + 4 * k);
        if (a != kNone) positive = static_cast<I>(static_cast<Word>(positive) + a);
        if (b != kNone) negative = static_cast<I>(static_cast<Word>(negative) + b);
    }
    if (positive > 20) positive = 20;                                      // 004F560C (signed)
    if (negative > 20) negative = 20;
    return static_cast<Word>(positive) - static_cast<Word>(negative);
}
}
