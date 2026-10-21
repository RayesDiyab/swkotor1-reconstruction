// W2 tier 4 (fork C): 00572070 WriteGameObjUpdate_UpdateAppearance and small callees. See header.
// Comments give original instruction addresses.
#include "51971a08a7-w2_g07_appearance.hpp"
#include "28478d5d50-w2_ftol.hpp"
#include <cstring>
namespace w2::g07 {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
constexpr Word kInvalid = 0x7f000000u;   // [00746644]
constexpr Word kCreature = 5;            // [00746634]
void dword(Memory& m, w2::entry::Ports& p, Word self, Word v) { sm::message_word_004d6340(m, p, self, v, 0x20); }
// 'D' (44h) unequip record: BYTE 44h, OBJECTID 7F000000h, DWORD slot
void remove_record(Memory& m, w2::entry::Ports& p, Word self, Word slot) {
    write_byte_004d62c0(m, p, self, 0x44, 8);
    write_object_id_00578570(m, p, self, kInvalid);
    dword(m, p, self, slot);
}
// 'A' (41h) equip record for slots 1 / 2 (no instant-equip BOOL): 00572362..00572392 / 005723A7..005723D7
void add_record(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word self, Word creature, Word item, Word slot, bool instant, Word scratch) {
    write_byte_004d62c0(m, p, self, 0x41, 8);
    write_object_id_00578570(m, p, self, m.read(item + 0x14));
    dword(m, p, self, slot);
    if (instant) write_bool_004d68e0(m, p, self, last_instant_equip_004efdd0(m, creature, slot));
    item_appearance_00567090(m, p, x, self, item);
    active_item_properties_00566f20(m, p, x, self, item, creature, scratch);
}
}

Word last_instant_equip_004efdd0(Memory& m, Word c, Word slot) {
    if (slot == 0x10) return m.read(c + 0xaac);
    if (slot == 0x20) return m.read(c + 0xaa8);
    return 0;
}
// 004EFE00 SetLastInstantEquip is W1 tier7 (ruling): AppearancePorts::set_last_instant_equip_004efe00.

Word ftol2_low_006fae8c(double v) { return w2::ftol2_006fae8c(v); }   // existing provider natively (t4b)

namespace {
// fld [base+C4h]; fimul [item+260h]; __ftol2 (one rounding at PC=53)
Word cost_product(Memory& m, AppearancePorts& x, Word item, Word at) {
    const Word base = x.item_base_item_005b4790(item);
    if (base == 0) x.trap(at, "W2: original reads base+C4h of a null base item (item cost)");
    float f; const Word bits = m.read(base + 0xc4); std::memcpy(&f, &bits, 4);
    return ftol2_low_006fae8c(static_cast<double>(f) * static_cast<double>(static_cast<std::int32_t>(m.read(item + 0x260))));
}
// GetCost body after the [item+108h] test: min 1 (unsigned JBE), else recomputed
Word cost_body(Memory& m, AppearancePorts& x, Word item, Word at1, Word at2) {
    if (cost_product(m, x, item, at1) <= 1) return 1;
    return cost_product(m, x, item, at2);
}
// Shared passive-property filter of the restriction checks: [p+11h] == FFh, or bit [p+11h] of [item+294h] set.
Word filtered_passive(Memory& m, AppearancePorts& x, Word item, Word i, Word at) {
    const Word p = passive_property_00553990(m, item, i);
    if (p == 0) x.trap(at, "W2: passive property index out of range");   // unreachable within [item+24Ch]
    const Word cl = m.read8(p + 0x11);
    if (cl != 0xff && (m.read(item + 0x294) & (1u << (cl & 31))) == 0) return 0;   // SHL uses CL mod 32
    return p;
}
}

Word item_cost_00554000(Memory& m, AppearancePorts& x, Word item) {
    if (m.read(item + 0x108) == 1) return 0;                              // 00554003
    return cost_body(m, x, item, 0x00554017, 0x00554034);
}

Word passive_property_00553990(Memory& m, Word item, Word index) {
    if (static_cast<std::int32_t>(index) < 0 || static_cast<std::int32_t>(index) >= static_cast<std::int32_t>(m.read(item + 0x24c))) return 0;
    return m.read(item + 0x254) + index * 0x1c;
}

Word simple_alignment_good_evil_005a5110(Memory& m, Word stats) {
    const std::int16_t v = static_cast<std::int16_t>(m.read16(stats + 0x17e));
    if (v <= 0x28) return 3;                                              // signed JG
    return v >= 0x3c ? 2 : 1;
}

Word required_feat_005b30c0(Memory& m, Word base, Word index) {
    const Word i = index & 0xffu;
    if (i >= m.read8(base + 0x80)) return 0xffff;                          // AX only
    return m.read16(m.read(base + 0x7c) + 2 * i);
}

Word check_item_alignment_005145a0(Memory& m, AppearancePorts& x, Word c, Word item) {
    if (x.item_property_exists_00555c90(item, 0x2b, 0) == 0) return 1;
    if (x.stats_has_feat_005a6630(m.read(c + 0xa74), 0x5d) != 0) return 1;
    const Word ge = simple_alignment_good_evil_005a5110(m, m.read(c + 0xa74)) & 0xffu;   // 005145DE MOVZX BX, AL
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(item + 0x24c)); ++i) {
        const Word p = filtered_passive(m, x, item, i, 0x005145f8);
        if (p == 0 || m.read16(p) != 0x2b) continue;
        if (m.read16(p + 2) != ge) return 0;                              // 0051462C
    }
    return 1;
}

Word check_item_feat_00514650(Memory& m, AppearancePorts& x, Word c, Word item) {
    if (x.item_property_exists_00555c90(item, 0x39, 0) == 0) return 1;
    if (x.stats_has_feat_005a6630(m.read(c + 0xa74), 0x5d) != 0) return 1;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(item + 0x24c)); ++i) {
        const Word p = filtered_passive(m, x, item, i, 0x00514698);
        if (p == 0 || m.read16(p) != 0x39) continue;
        if ((feat_remaining_uses_005a6680(m, x, m.read(c + 0xa74), m.read16(p + 2)) & 0xffu) == 0) return 0;   // 005146D5
    }
    return 1;
}

Word check_item_class_00518210(Memory& m, AppearancePorts& x, Word c, Word item) {
    if (x.item_property_exists_00555c90(item, 0x2c, 0) == 0) return 1;
    if (x.stats_has_feat_005a6630(m.read(c + 0xa74), 0x5d) != 0) return 1;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(item + 0x24c)); ++i) {
        const Word p = filtered_passive(m, x, item, i, 0x00518260);
        if (p == 0 || m.read16(p) != 0x2c) continue;
        const Word want = m.read16(p + 2);
        for (Word k = 0; k < m.read8(m.read(c + 0xa74) + 0x89); ++k) {    // class count re-read each pass
            if ((x.stats_class_005a4e90(m.read(c + 0xa74), k) & 0xffu) == want) return 1;   // 005182B1
        }
    }
    return 0;
}

Word check_item_race_005182f0(Memory& m, AppearancePorts& x, Word c, Word item, Word out) {
    Word allow = 1;
    if (x.item_property_exists_00555c90(item, 0x0a, 0) != 0) {            // 00518304
        m.write(out, 0);
        x.item_property_by_type_005539c0(item, out, 0x0a, 0x81);
        if (m.read(out) != 0) allow = 0;                                  // 00518330
    }
    const Word base = x.item_base_item_005b4790(item);
    if (base == 0) x.trap(0x00518339, "W2: original reads base+B4h of a null base item (CheckItemRaceRestrictions)");
    const Word kind = m.read8(base + 0xb4);
    if (kind != 0 && allow != 0) {
        const Word race = m.read16(m.read(c + 0xa74) + 0xdc);
        if (kind == 1) { if (race != 6) return 0; }                       // 0051837B: [0074BF14] (.rdata) = 6
        else if (kind == 2) { if (race != 5) return 0; }                  // 0051835D: [0074BF10] (.rdata) = 5
        else return 0;
    }
    const Word subrace = m.read8(m.read(c + 0xa74) + 0xe8);
    const Word base2 = x.item_base_item_005b4790(item);
    if (base2 == 0) x.trap(0x005183a9, "W2: original reads base+B8h of a null base item (CheckItemRaceRestrictions)");
    if ((m.read(base2 + 0xb8) & (1u << (subrace & 31))) != 0) return 0;  // 005183A9
    if (x.item_property_exists_00555c90(item, 0x2d, 0) == 0) return 1;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(item + 0x24c)); ++i) {
        const Word p = filtered_passive(m, x, item, i, 0x005183e8);
        if (p == 0 || m.read16(p) != 0x2d) continue;
        if (m.read16(m.read(c + 0xa74) + 0xdc) == m.read16(p + 2)) return 1;   // 0051842A
    }
    return 0;
}

Word item_min_equip_level_00554080(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word item, Word s) {
    const Word table = m.read(m.read(x.rules_007a3a28() + 0xb8) + 0x114);
    const Word str = s, out = s + 8;
    Word row = 0;
    if (static_cast<std::int32_t>(m.read(table + 0x24)) > 0) {
        for (;;) {
            p.str_literal_005e5a90(str, p.literal(0x00748da4));            // "MAXSINGLEITEMVALUE"
            p.c2da_int_00413660(table, row, str, out);
            p.str_destroy_005e5c20(str);
            if (m.read(item + 0x108) == 1) break;                          // 005540FA
            if (cost_body(m, x, item, 0x0055410a, 0x00554127) <= m.read(out)) break;   // 0055413F: unsigned JBE
            ++row;
            if (static_cast<std::int32_t>(row) >= static_cast<std::int32_t>(m.read(table + 0x24))) break;
        }
    }
    return (row + 1) & 0xffu;                                              // 00554156: AL = BL + 1
}

Word feat_remaining_uses_005a6680(Memory& m, AppearancePorts& x, Word stats, Word feat) {
    const Word info = x.rules_feat_00550c00(x.rules_007a3a28(), feat);
    if (info == 0) return 0;                                               // 005A669C: XOR AL, AL with EAX = 0
    if (x.stats_has_feat_005a6630(stats, feat) == 0) return 0;            // 005A66A8
    const Word count = m.read(stats + 0x10);
    Word entry = 0, index = 0, found = 0;
    for (;;) {                                                            // 005A66B3: DX index (16-bit)
        const Word i = index & 0xffffu;
        if (static_cast<std::int32_t>(i) >= static_cast<std::int32_t>(count)) {
            if (found == 0) return (i & 0xffffff00u) | 0x64;               // 005A6705: MOV AL, 64h over EAX = index
            break;
        }
        entry = m.read(m.read(stats + 0x0c) + 4 * i);
        if (m.read16(entry) == (feat & 0xffffu)) found = 1;               // 005A66C0
        ++index;
        if (found != 0) break;
        if (index == 0x10000u) x.trap(0x005a66b3, "W2: original loops forever (16-bit index, count > FFFFh, feat absent)");
    }
    if (entry == 0) return 0x64;                                           // 005A66D7 (not reachable after a match)
    const Word diff = (m.read8(info + 0x33) - m.read8(entry + 2)) & 0xffu;   // 005A66E0: SUB AL
    if (diff == 0) return 0;
    return diff < 0x64 ? diff : 0x64;
}

Word check_proficiencies_00510e30(Memory& m, AppearancePorts& x, Word c, Word item, Word) {
    auto base = [&](Word at) {
        const Word b = x.base_item_005b31a0(m.read(x.rules_007a3a28() + 0x34), m.read(item + 0x0c));
        if (b == 0) x.trap(at, "W2: original dereferences a null base item (CheckProficiencies)");
        return b;
    };
    if (m.read(base(0x00510e49) + 4) == 0) return 0;                      // 00510E4C
    if (x.stats_has_feat_005a6630(m.read(c + 0xa74), 0x5d) != 0) return 1;
    for (Word i = 0; i < m.read8(base(0x00510e87) + 0x80); ++i) {        // count re-read via GetBaseItem each pass
        const Word feat = required_feat_005b30c0(m, base(0x00510ea6), i);
        if (x.stats_has_feat_005a6630(m.read(c + 0xa74), feat) == 0) return 0;
    }
    return 1;
}

Word can_use_item_0051b440(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word c, Word item, Word, Word s) {
    if (item == 0) return 0;                                              // 0051B458
    Word result = 1;
    if (check_item_alignment_005145a0(m, x, c, item) == 0 || check_item_class_00518210(m, x, c, item) == 0
        || check_item_race_005182f0(m, x, c, item, s + 0x0c) == 0 || check_item_feat_00514650(m, x, c, item) == 0) result = 0;   // 0051B494
    if (m.read(c + 0x9d4) == 1) {                                         // 0051B498
        const std::uint8_t level = static_cast<std::uint8_t>(x.stats_level_005a5fd0(m.read(c + 0xa74), 0));
        if (level < static_cast<std::uint8_t>(item_min_equip_level_00554080(m, p, x, item, s))) {
            const Word info = x.server_info_004aec90(m.read(p.app_007a39fc() + 8));   // 0051B4C1 call 004AEC90 GetServerInfo (W1)
            if (m.read(info + 0xc8) != 0) return 0;                       // 0051B4CE
        }
    }
    if (result != 1) return result;                                       // 0051B4D8
    const Word array = m.read(x.rules_007a3a28() + 0x34);
    const Word base = x.base_item_005b31a0(array, m.read(item + 0x0c));
    if (base != 0 && m.read8(base + 8) != 8 && m.read8(base + 9) != 0) {  // 0051B4F6 / 0051B4FC
        const Word level = m.read(c + 0x4f8);
        const Word again = x.base_item_005b31a0(m.read(x.rules_007a3a28() + 0x34), m.read(item + 0x0c));
        if (again == 0) x.trap(0x0051b51b, "W2: original reads base+1Bh of a null base item (CanUseItem)");
        if (static_cast<std::int32_t>(m.read8(again + 0x1b) - level) > 1) result = 0;   // 0051B521: signed JLE
    }
    if (base == 0) x.trap(0x0051b52e, "W2: original reads base+4 of a null base item (CanUseItem)");
    const Word mask = m.read(base + 4);
    for (Word bit = 0; bit < 0x12; ++bit) {                               // 0051B533: lowest proficiency bit
        if ((mask & (1u << bit)) != 0) return check_proficiencies_00510e30(m, x, c, item, 1u << bit) == 0 ? 0 : result;
    }
    return result;
}

Word active_property_00553960(Memory& m, Word item, Word index) {
    if (static_cast<std::int32_t>(index) < 0 || static_cast<std::int32_t>(index) >= static_cast<std::int32_t>(m.read(item + 0x248))) return 0;
    return m.read(item + 0x250) + index * 0x1c;
}

void active_item_properties_00566f20(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word self, Word item, Word creature, Word scratch) {
    const Word base = x.item_base_item_005b4790(item);
    if (base == 0) x.trap(0x00566f31, "W2: original reads base+ACh of a null base item (AddActiveItemProperties)");
    if (m.read8(base + 0xac) >= 0x1f) {                                   // 00566F31 / 00566F41: unsigned 1Fh..26h
        const Word again = x.item_base_item_005b4790(item);
        if (again == 0) x.trap(0x00566f41, "W2: original reads base+ACh of a null base item (AddActiveItemProperties)");
        if (m.read8(again + 0xac) <= 0x26) write_word16_004d6300(m, p, self, m.read16(item + 0x244), 0x10);
    }
    write_locstring_005785c0(m, p, self, item + 0x280, 0, scratch);      // 00566F68
    write_bool_004d68e0(m, p, self, 1);
    dword(m, p, self, item_cost_00554000(m, x, item));                        // 00566F82
    dword(m, p, self, m.read(item + 0x28c));
    write_bool_004d68e0(m, p, self, creature != 0 ? can_use_item_0051b440(m, p, x, creature, item, 1, scratch) : 0);   // 00566F9B
    write_bool_004d68e0(m, p, self, (m.read(item + 0x288) >> 5) & 1);
    const Word count = m.read(item + 0x248);
    write_byte_004d62c0(m, p, self, count, 8);                            // 00566FD2: low 8 bits
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(count); ++i) {
        const Word prop = active_property_00553960(m, item, i);           // non-null inside the loop bound
        write_word16_004d6300(m, p, self, m.read16(prop), 0x10);
        write_word16_004d6300(m, p, self, m.read16(prop + 2), 0x10);
        write_word16_004d6300(m, p, self, m.read16(prop + 6), 0x10);
        write_byte_004d62c0(m, p, self, m.read8(prop + 9), 8);
    }
    write_byte_004d62c0(m, p, self, m.read8(item + 0x238), 8);            // 00567043
    write_byte_004d62c0(m, p, self, 0xff, 8);
    for (Word k = 0; k < 8; ++k) write_byte_004d62c0(m, p, self, m.read8(item + 0x239 + k), 8);
}

void item_appearance_00567090(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word self, Word item) {
    const Word type = m.read(item + 0x0c);
    dword(m, p, self, type);                                              // 0056709F
    const Word base = x.base_item_005b31a0(m.read(x.rules_007a3a28() + 0x34), type);
    if (base == 0) return;
    write_byte_004d62c0(m, p, self, m.read8(item + 0x264), 8);            // 005670C5
    if (m.read8(base + 0x18) != 1) return;                                // 005670CA
    write_byte_004d62c0(m, p, self, m.read8(item + 0x265), 8);
    write_byte_004d62c0(m, p, self, m.read8(item + 0x266), 8);
}

void inventory_slot_add_00567710(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word self, Word creature, Word item, Word slot, Word scratch) {
    write_byte_004d62c0(m, p, self, 0x41, 8);
    if (item == 0) x.trap(0x00567723, "W2: original reads item+14h of a null item (WriteInventorySlotAdd)");
    write_object_id_00578570(m, p, self, m.read(item + 0x14));
    dword(m, p, self, slot);
    if (slot == 0x10 || slot == 0x20) write_bool_004d68e0(m, p, self, last_instant_equip_004efdd0(m, creature, slot));
    item_appearance_00567090(m, p, x, self, item);
    active_item_properties_00566f20(m, p, x, self, item, creature, scratch);
}

void update_appearance_00572070(Memory& m, w2::entry::Ports& p, AppearancePorts& x, Word self, Word c, Word last, Word flags, Word s) {
    if (m.read8(c + 8) != kCreature) return;                              // 0057209D
    const Word stats = m.read(c + 0xa74);
    write_byte_004d62c0(m, p, self, 0x50, 8);                             // 'P'
    write_byte_004d62c0(m, p, self, kCreature, 8);
    write_object_id_00578570(m, p, self, m.read(c + 4));
    write_word16_004d6300(m, p, self, flags, 0x10);                       // low 16 bits
    if ((flags & 0x400u) != 0) {
        write_locstring_005785c0(m, p, self, stats + 0x34, 0, s + 0x0c);  // first name
        write_locstring_005785c0(m, p, self, stats + 0x3c, 0, s + 0x0c);  // last name
    }
    if ((flags & 0x0001u) != 0) write_word16_004d6300(m, p, self, m.read16(c + 0xa60), 0x10);
    if ((flags & 0x0002u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa62), 8);
    if ((flags & 0x0004u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa63), 8);
    if ((flags & 0x0080u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa68), 8);
    if ((flags & 0x4000u) != 0) write_bool_004d68e0(m, p, self, m.read(c + 0xa6c));
    if ((flags & 0x8000u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa70), 8);
    if ((flags & 0x0800u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa69), 8);
    if ((flags & 0x1000u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa6a), 8);
    if ((flags & 0x0008u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa64), 8);
    if ((flags & 0x0010u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa65), 8);
    if ((flags & 0x0020u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa66), 8);
    if ((flags & 0x0040u) != 0) write_byte_004d62c0(m, p, self, m.read8(c + 0xa67), 8);
    if ((flags & 0x2000u) != 0) write_word16_004d6300(m, p, self, m.read16(c + 0x9d8), 0x10);
    const Word inv = c + 0xa2c;
    auto item = [&](Word slot) { return x.item_in_slot_005a4c20(m.read(inv), slot); };
    if (flags == 0xffffffffu) {                                           // 00572228: full equipment snapshot
        std::uint8_t count = 4;
        if (item(2) != 0) count = 5;                                      // 0057224D: MOV, not INC
        if (item(1) != 0) ++count;
        if (item(0x20) != 0) ++count;
        if (item(0x10) != 0) ++count;
        write_byte_004d62c0(m, p, self, count, 8);
        remove_record(m, p, self, 2);
        remove_record(m, p, self, 1);
        remove_record(m, p, self, 0x20); write_bool_004d68e0(m, p, self, last_instant_equip_004efdd0(m, c, 0x20));
        remove_record(m, p, self, 0x10); write_bool_004d68e0(m, p, self, last_instant_equip_004efdd0(m, c, 0x10));
        if (const Word it = item(2)) add_record(m, p, x, self, c, it, 2, false, s + 0x0c);
        if (const Word it = item(1)) add_record(m, p, x, self, c, it, 1, false, s + 0x0c);
        if (const Word it = item(0x20)) add_record(m, p, x, self, c, it, 0x20, true, s + 0x0c);
        if (const Word it = item(0x10)) add_record(m, p, x, self, c, it, 0x10, true, s + 0x0c);
        x.set_last_instant_equip_004efe00(c, 0x10, 0);
        x.set_last_instant_equip_004efe00(c, 0x20, 0);
        return;
    }
    if ((flags & 0x200u) == 0) return;                                    // 005724A3
    // 005724A9: incremental equipment diff against the last-update object
    const Word list = s;                                                  // {data, count, capacity}
    m.write(list, 0); m.write(list + 4, 0); m.write(list + 8, 0);
    std::uint8_t count = 0;
    auto bump = [&](Word now) { count = static_cast<std::uint8_t>(count + (now == kInvalid ? 1 : 2)); };
    if (m.read(c + 0xa50) != m.read(last)) bump(m.read(c + 0xa50));                  // 005724C1
    if (m.read(c + 0xa54) != m.read(last + 4)) bump(m.read(c + 0xa54));              // 005724DE
    const bool same_look = m.read16(c + 0xa60) == m.read16(last + 0x10)              // 005724FF..00572545
        && m.read8(c + 0xa63) == m.read8(last + 0x13) && m.read8(c + 0xa62) == m.read8(last + 0x12)
        && m.read8(c + 0xa64) == m.read8(last + 0x14) && m.read8(c + 0xa65) == m.read8(last + 0x15)
        && m.read8(c + 0xa66) == m.read8(last + 0x16) && m.read8(c + 0xa67) == m.read8(last + 0x17);
    if (same_look) {
        if (m.read(c + 0xa58) != m.read(last + 8)) bump(m.read(c + 0xa58));
        if (m.read(c + 0xa5c) != m.read(last + 0x0c)) bump(m.read(c + 0xa5c));
    } else {                                                                         // 00572581
        count = static_cast<std::uint8_t>(count + 2);
        if (item(1) != 0) ++count;
        if (item(2) != 0) ++count;
    }
    write_byte_004d62c0(m, p, self, count, 8);
    Word cleared_20 = 0, cleared_10 = 0;                                              // [ESP+30h] / [ESP+34h] (argument slots)
    if (m.read(c + 0xa50) != m.read(last)) {                                          // 005725D9
        remove_record(m, p, self, 0x20); write_bool_004d68e0(m, p, self, last_instant_equip_004efdd0(m, c, 0x20));
        if (m.read(c + 0xa50) != kInvalid) x.list_add_00671c00(list, 0x20);
        cleared_20 = 1;
    }
    if (m.read(c + 0xa54) != m.read(last + 4)) {                                      // 0057266C
        remove_record(m, p, self, 0x10); write_bool_004d68e0(m, p, self, last_instant_equip_004efdd0(m, c, 0x10));
        if (m.read(c + 0xa54) != kInvalid) x.list_add_00671c00(list, 0x10);
        cleared_10 = 1;
    }
    const bool same_look2 = m.read16(c + 0xa60) == m.read16(last + 0x10)             // 005726F8: re-read
        && m.read8(c + 0xa63) == m.read8(last + 0x13) && m.read8(c + 0xa62) == m.read8(last + 0x12)
        && m.read8(c + 0xa64) == m.read8(last + 0x14) && m.read8(c + 0xa65) == m.read8(last + 0x15)
        && m.read8(c + 0xa66) == m.read8(last + 0x16) && m.read8(c + 0xa67) == m.read8(last + 0x17);
    if (same_look2) {
        if (m.read(c + 0xa58) != m.read(last + 8)) {
            remove_record(m, p, self, 2);
            if (m.read(c + 0xa58) != kInvalid) x.list_add_00671c00(list, 2);
        }
        if (m.read(c + 0xa5c) != m.read(last + 0x0c)) {
            remove_record(m, p, self, 1);
            if (m.read(c + 0xa5c) != kInvalid) x.list_add_00671c00(list, 1);
        }
    } else {                                                                         // 00572837
        remove_record(m, p, self, 2);
        remove_record(m, p, self, 1);
        if (const Word it = item(1)) add_record(m, p, x, self, c, it, 1, false, s + 0x0c);
        if (const Word it = item(2)) add_record(m, p, x, self, c, it, 2, false, s + 0x0c);
    }
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(list + 4)); ++i) {   // 00572909
        const Word slot = m.read(m.read(list) + 4 * i);
        inventory_slot_add_00567710(m, p, x, self, c, item(slot), slot, s + 0x0c);
    }
    if (cleared_10 != 0) x.set_last_instant_equip_004efe00(c, 0x10, 0);
    if (cleared_20 != 0) x.set_last_instant_equip_004efe00(c, 0x20, 0);
    p.free(m.read(list));                                                             // 00572967 (free(NULL) when empty)
}
}
