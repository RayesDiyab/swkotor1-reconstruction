#include "056127edc0-w2_ifo.hpp"
#include <initializer_list>
// Instruction-ordered bodies (original addresses in comments). Base for
// 00561E30: donor ssh/work/mac-character-load mac-module-leaves/load-character-ifo,
// re-verified instruction by instruction against swkotor.exe.
namespace w2::ifo {
namespace {
Word party_table(Memory& m, Word server) { return m.read(server + 4) + 0x1b770; }   // 004AEE70
Word ai_master(Memory& m, Word server) { return m.read(m.read(server + 4) + 0x10044); } // 004AED80
}

// ---------------------------------------------------------------- 00561E30
// scratch offsets = original ESP offsets after the prologue:
//  +10 status, +14 new-resource flag / string, +1C top struct, +20 string/ref/allocation then area id,
//  +30..+38 position, +3C..+44 orientation, +48 resource ref, +58 list, +7C index then element struct,
//  +80 flag argument, +84 post-process argument.
Word load_character_00561e30(Memory& m, Ports& p, Word self, Word s) {
    Word index = m.read(s + 0x7c);
    const bool special = index == 0xffffffffu;                             // 00561E52
    for (Word o = 0x30; o <= 0x44; o += 4) m.write(s + o, 0);
    m.write(s + 0x14, 0);
    Word resource;
    if (special) {
        p.str_literal_005e5a90(s + 0x20, p.literal(0x00745310));           // "TEMP:"
        p.directory_add_00408800(p.resman_007a39e8(), s + 0x20);
        p.str_destroy_005e5c20(s + 0x20);
        index = m.read(self + 0x80);
        p.ref_construct_00406d80(s + 0x48, p.literal(0x00745318));         // "pifo"
        resource = p.resource_lookup_004074d0(p.resman_007a39e8(), s + 0x48, 0x7de);
        if (resource == 0) {
            const Word storage = p.allocate_006fa7e6(0xa0);
            m.write(s + 0x20, storage);
            resource = storage != 0 ? p.gff_construct_004105a0(storage) : 0;
            p.ref_construct_00406d80(s + 0x20, p.literal(0x00745318));
            p.resource_register_00407680(p.resman_007a39e8(), s + 0x20, 0x7de, resource);
            p.strncpy_006fab80(resource + 0x8d, p.literal(0x00745330), 4);   // "IFO "
            m.write(s + 0x14, 1);
        }
    } else {
        p.ref_construct_00406d80(s + 0x48, p.literal(0x007442a8));         // "Module"
        resource = p.resource_lookup_004074d0(p.resman_007a39e8(), s + 0x48, 0x7de);
    }
    if (resource == 0) return 0;                                           // 00561F91 -> 005620B7
    (void)p.demand_00409b10(resource);
    p.top_00411240(resource, s + 0x1c);
    if (p.list_004118c0(resource, s + 0x58, s + 0x1c, p.literal(0x00745320)) == 0   // "Mod_PlayerList"
        || p.element_00411990(resource, s + 0x7c, s + 0x58, index) == 0) {
        (void)p.release_00409b80(resource);
        (void)p.manager_release_00409cf0(p.resman_007a39e8(), resource);
        return 0;
    }
    const Word id = p.dword_00411c20(resource, s + 0x7c, p.literal(0x00744c24), s + 0x10, 0x7f000000u);  // "ObjectId"
    const Word storage = p.allocate_006fa7e6(0xac8);
    m.write(s + 0x20, storage);
    const Word creature = storage != 0 ? p.creature_construct_004f7a10(storage, id, 1) : 0;
    p.set_game_object_00561c40(self, creature);
    m.write(self + 0x38, m.read(creature + 4));                            // 00562054 (domain: allocation succeeded)
    m.write(s + 0x20, p.dword_00411c20(resource, s + 0x7c, p.literal(0x00746d10), s + 0x10, 0));      // "AreaId"
    set_in_party_004fdb20(m, p, creature, 1, 1);
    if (p.stats_read_005afce0(m.read(creature + 0xa74), resource, s + 0x7c, creature + 0xa50) != 0) {
        (void)p.manager_release_00409cf0(p.resman_007a39e8(), resource);  // no CRes::Release on this path
        p.vcall1(creature, 0x00, 1);                                       // scalar deleting destructor
        return 0;
    }
    load_journal_004f17d0(m, p, creature, resource, s + 0x7c, s + 0x88);
    (void)p.byte_00411a60(resource, s + 0x7c, p.literal(0x007472dc), s + 0x10, 0);    // "DetectMode" (value ignored)
    set_activity_004f00a0(m, p, creature, 2, 1, 0);
    const Word stealth = p.byte_00411a60(resource, s + 0x7c, p.literal(0x007472d0), s + 0x10, 0) & 0xffu;   // "StealthMode"
    set_activity_004f00a0(m, p, creature, 1, stealth, 0);
    m.write(creature + 0x4f8, p.int_00411c90(resource, s + 0x7c, p.literal(0x007472c0), s + 0x10, 3));       // "CreatureSize"
    p.items_read_004ffda0(creature, resource, s + 0x7c, special ? 0u : 1u, 1, 0);
    p.object_state_004d1cf0(creature, resource, s + 0x7c);
    p.spells_read_005aeb30(m.read(creature + 0xa74), resource, s + 0x7c);
    const Word disguised = p.byte_00411a60(resource, s + 0x7c, p.literal(0x00747260), s + 0x10, 0) & 0xffu; // "PM_IsDisguised"
    m.write(creature + 0xa48, disguised);
    if (disguised != 0)
        m.write16(creature + 0xa4c, p.word_00411b40(resource, s + 0x7c, p.literal(0x00747250), s + 0x10, 0)); // "PM_Appearance"
    p.scripts_read_004ebf20(creature, resource, s + 0x7c);
    const Word labels[6] = {0x00745d80, 0x00745d74, 0x00745d68, 0x00745d38, 0x00745d48, 0x00745d58};      // X/Y/ZPosition, X/Y/ZOrientation
    for (Word k = 0; k < 6; ++k) m.write(s + 0x30 + 4 * k, p.float_bits_00411d00(resource, s + 0x7c, p.literal(labels[k]), s + 0x10, 0));
    p.set_orientation_004cd170(creature, s + 0x3c);
    m.write(self + 0x40, (!special && m.read(s + 0x80) != 0) ? 1u : 0u);
    if (m.read(s + 0x84) != 0) p.post_process_004f1c40(creature);
    (void)p.release_00409b80(resource);
    (void)p.manager_release_00409cf0(p.resman_007a39e8(), resource);
    if (m.read(s + 0x14) == 1 && resource != 0) p.vcall1(resource, 0x00, 1);
    if (special) {
        p.str_literal_005e5a90(s + 0x14, p.literal(0x00745310));
        p.directory_remove_004088d0(p.resman_007a39e8(), s + 0x14);
        p.str_destroy_005e5c20(s + 0x14);
    } else {
        m.write(creature + 0x314, m.read(s + 0x30));
        m.write(creature + 0x310, m.read(s + 0x20));
        m.write(creature + 0x318, m.read(s + 0x34));
        m.write(creature + 0x31c, m.read(s + 0x38));
        m.write(creature + 0x320, 0);
    }
    return 1;
}

// ---------------------------------------------------------------- 004FDB20 and party-follow info
Word follow_info_construct_004f79e0(Memory& m, Word self) {
    for (Word o = 4; o <= 0x24; o += 4) m.write(self + o, 0);
    follow_info_clear_004f0020(m, self);
    return self;
}
void follow_info_clear_004f0020(Memory& m, Word self) {
    for (Word base : {0x04u, 0x10u, 0x1cu}) for (Word k = 0; k < 12; k += 4) m.write(self + base + k, 0);
    m.write(self, 0x7f000000u);
    m.write(self + 0x28, 0); m.write(self + 0x2c, 0);
    m.write(self + 0x30, 2);                                               // [00745DC0]
    m.write(self + 0x34, 0); m.write(self + 0x38, 1);
}
void set_in_party_004fdb20(Memory& m, Ports& p, Word creature, Word in_party, Word update) {
    m.write(creature + 0xa88, in_party);
    if (update == 0) return;
    if (in_party != 0) {
        if (m.read(creature + 0x4c0) == 0) {
            const Word storage = p.allocate_006fa7e6(0x3c);
            m.write(creature + 0x4c0, storage != 0 ? follow_info_construct_004f79e0(m, storage) : 0);
        }
        if (m.read(creature + 0x78) == 4) return;
        p.set_ai_level_004b08a0(ai_master(m, server_of(m, p)), creature, 4);
    } else {
        if (m.read(creature + 0x4c0) != 0) { p.free_006fa390(m.read(creature + 0x4c0)); m.write(creature + 0x4c0, 0); }
        if (m.read(creature + 0x78) == 2) return;
        p.set_ai_level_004b08a0(ai_master(m, server_of(m, p)), creature, 2);
    }
}

// ---------------------------------------------------------------- journal
Word party_journal_005633c0(Memory& m, Ports& p, Word party) {
    if (m.read(party + 0x11c) == 0) {
        const Word storage = p.allocate_006fa7e6(0x0c);
        m.write(party + 0x11c, storage != 0 ? p.array_list_construct_005c5a30(storage) : 0);
    }
    return m.read(party + 0x11c);
}
// scratch: +0C status, +10 element struct, +14 plot string, +1C default string, +24 result string, +2C list
void load_journal_004f17d0(Memory& m, Ports& p, Word creature, Word resource, Word structure, Word s) {
    (void)creature;
    const Word journal = party_journal_005633c0(m, p, party_table(m, server_of(m, p)));
    if (journal == 0 || m.read(journal + 4) != 0) return;
    if (p.list_004118c0(resource, s + 0x2c, structure, p.literal(0x00746f90)) == 0) return;   // "JNL_Entries"
    const Word cell = p.journal_sort_order_cell_00833a90();
    m.write(cell, p.int_00411c90(resource, structure, p.literal(0x00746f80), s + 0x0c, m.read(cell)));  // "JNL_SortOrder"
    p.str_empty_005b3190(s + 0x14);
    const Word count = p.list_count_00411940(resource, s + 0x2c);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(count); ++i) {
        (void)p.element_00411990(resource, s + 0x10, s + 0x2c, i);
        p.str_literal_005e5a90(s + 0x1c, p.literal(0x0073d71c));
        (void)p.string_00411ec0(resource, s + 0x24, s + 0x10, p.literal(0x00746f74), s + 0x0c, s + 0x1c);   // "JNL_PlotID"
        p.str_assign_005e5c50(s + 0x14, s + 0x24);
        p.str_destroy_005e5c20(s + 0x24);
        p.str_destroy_005e5c20(s + 0x1c);
        const Word state = p.int_00411c90(resource, s + 0x10, p.literal(0x00746f68), s + 0x0c, 0);           // "JNL_State"
        p.journal_set_state_005c5a40(party_journal_005633c0(m, p, party_table(m, server_of(m, p))), s + 0x14, state, 1);
        const Word date = p.dword_00411c20(resource, s + 0x10, p.literal(0x00746f5c), s + 0x0c, 0);          // "JNL_Date"
        p.journal_set_date_005c63f0(party_journal_005633c0(m, p, party_table(m, server_of(m, p))), s + 0x14, date, 1);
        const Word time = p.dword_00411c20(resource, s + 0x10, p.literal(0x00746f50), s + 0x0c, 0);          // "JNL_Time"
        p.journal_set_time_005c6650(party_journal_005633c0(m, p, party_table(m, server_of(m, p))), s + 0x14, time, 1);
    }
    p.str_destroy_005e5c20(s + 0x14);
}

// ---------------------------------------------------------------- activity / movement
void movement_rate_004f6d70(Memory& m, Ports& p, Word c) {
    const Word stealth = m.read8(c + 0x4d1) == 1 ? 1u : 0u;
    Word i = m.read16(m.read(c + 0xa74) + 0x160);
    while (static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(c + 0x128))) {
        const Word e = m.read(m.read(c + 0x124) + i * 4);
        const Word type = m.read16(e + 8);
        if (static_cast<std::int32_t>(type) > 59) break;                   // [00746768] = 59
        if (type == 59 && ((m.read8(e + 0x0a) & 7) & 0xffffu) == 4 && p.effect_integer_00503690(e, 0) == 1) {   // [007473C8] = 4
            p.remove_effect_by_id_004d06c0(c, m.read(e), m.read(e + 4));
            i = m.read16(m.read(c + 0xa74) + 0x160);
        } else ++i;
    }
    Word j = m.read16(m.read(c + 0xa74) + 0x162);
    while (static_cast<std::int32_t>(j) < static_cast<std::int32_t>(m.read(c + 0x128))) {
        const Word e = m.read(m.read(c + 0x124) + j * 4);
        const Word type = m.read16(e + 8);
        if (static_cast<std::int32_t>(type) > 29) break;                   // [007466F0] = 29
        if (type == 29 && (m.read8(e + 0x0a) & 7) == 4 && m.read(e + 0x18) == m.read(c + 4)) {
            p.remove_effect_by_id_004d06c0(c, m.read(e), m.read(e + 4));
            j = m.read16(m.read(c + 0xa74) + 0x162);
        } else ++j;
    }
    if (stealth == 0) return;
    const Word storage = p.allocate_006fa7e6(0x8c);
    const Word e = storage != 0 ? p.effect_construct_00503e40(storage, 1) : 0;
    m.write16(e + 8, 59);
    const Word flags = (m.read16(e + 0x0a) & 0xfff8u) | 4u;
    m.write(e + 0x20, 0);
    m.write16(e + 0x0a, flags);
    p.effect_creator_00503a00(e, m.read(c + 4));
    p.effect_set_integer_005036a0(e, 0, 1);
    p.apply_effect_004d14f0(c, e, 0, 0);
}
void set_detect_mode_0050ee30(Memory& m, Ports& p, Word c, Word mode) {
    if ((mode & 0xffu) == m.read8(c + 0x4d0)) return;
    p.write8(c + 0x4d0, mode);
    movement_rate_004f6d70(m, p, c);
}
void set_stealth_mode_0050ee50(Memory& m, Ports& p, Word c, Word mode, Word value) {
    if ((mode & 0xffu) == m.read8(c + 0x4d1)) return;
    p.write8(c + 0x4d1, mode);
    m.write(c + 0xabc, value);
    movement_rate_004f6d70(m, p, c);
}
void set_activity_004f00a0(Memory& m, Ports& p, Word c, Word mask, Word on, Word extra) {
    const Word blocked_at_entry = m.read(c + 0xa00);
    if ((blocked_at_entry & mask) == mask) return;
    if (((mask & 0xf8) != 0 && on == 1) || (mask == 0xf8 && on == 0)) {
        if ((m.read8(c + 0x9fc) & 8) != 0 && (blocked_at_entry & 8) == 0) set_activity_004f00a0(m, p, c, 8, 0, 0);   // stale A00 (quirk)
        for (Word bit : {0x10u, 0x20u, 0x40u, 0x80u})
            if ((m.read8(c + 0x9fc) & bit) != 0 && (m.read8(c + 0xa00) & bit) == 0) set_activity_004f00a0(m, p, c, bit, 0, 0);
    }
    if (((mask & 0x3f00) != 0 && on == 1) || (mask == 0x3f00 && on == 0)) {
        for (Word bit : {0x100u, 0x200u, 0x400u, 0x800u, 0x1000u, 0x2000u})
            if ((m.read(c + 0x9fc) & bit) != 0 && (m.read(c + 0xa00) & bit) == 0) set_activity_004f00a0(m, p, c, bit, 0, 0);
    }
    const Word old = m.read(c + 0x9fc);
    Word changed;
    if (on == 1) { changed = (old & mask) == 0 ? 1u : 0u; m.write(c + 0x9fc, old | mask); }
    else { changed = old & mask; m.write(c + 0x9fc, old & ~mask); }
    if (changed == 0) return;
    if ((mask & 1) != 0) {
        if (on == 1 && (m.read8(c + 0x9fc) & 0x0c) == 0) set_stealth_mode_0050ee50(m, p, c, on, extra);
        else if (on == 0) set_stealth_mode_0050ee50(m, p, c, on, extra);
        else m.write(c + 0x9fc, m.read(c + 0x9fc) & ~mask);
    }
    if ((mask & 2) != 0) set_detect_mode_0050ee30(m, p, c, on != 0 ? 1u : 0u);
    if ((mask & 4) != 0 && on == 0) (void)p.stop_dialog_004cb4f0(c);
}

// ---------------------------------------------------------------- sub-areas
// 004BF470 CSWSArea::InSubAreas is W5-owned (coordinator ruling, t2 review); reached through Ports.
// scratch: +00 out object, +04 list {data,count,capacity}
Word update_subareas_0051b940(Memory& m, Ports& p, Word c, Word position, Word area_id, Word flag, Word s) {
    const Word out = s, list = s + 4;
    m.write(list, 0); m.write(list + 4, 0); m.write(list + 8, 0);
    const Word array = m.read(m.read(server_of(m, p) + 4) + 0x1005c);
    Word f = flag;
    if (m.read(p.sticky_subarea_cell_007a1b54()) != 0) f = 1;
    Word area = 0;
    if (area_id != 0x7f000000u) {
        (void)p.object_lookup_004d8230(array, area_id, out);             // status unchecked (original)
        area = p.vcall0(m.read(out), 0x28);
    }
    const Word had = static_cast<std::int32_t>(m.read(c + 0x334)) > 0 ? 1u : 0u;
    Word changed = 0;
    const Word inside = area != 0 ? p.in_subareas_004bf470(area, position, list) : 0;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(c + 0x328)); ++i) {
        const Word aoe = p.aoe_by_id_004ae7d0(server_of(m, p), m.read(m.read(c + 0x324) + 4 * i));
        if (aoe == 0) continue;
        p.aoe_jump_00596360(aoe, area, position);
        const Word id = m.read(m.read(c + 0x324) + 4 * i);
        const Word count = m.read(list + 4);
        Word found = 0;
        for (Word k = 0; static_cast<std::int32_t>(k) < static_cast<std::int32_t>(count); ++k) if (m.read(m.read(list) + 4 * k) == id) ++found;
        if (found != 0) continue;
        if (count == m.read(list + 8)) p.list_allocate_005e03c0(list, m.read(list + 8) != 0 ? m.read(list + 8) * 2 : 0x10);
        m.write(m.read(list) + 4 * m.read(list + 4), id);
        m.write(list + 4, m.read(list + 4) + 1);
    }
    if (had != 0 || inside != 0) {
        if (p.subarea_changes_00516020(c, list, 0, 0, f) == 1) {
            p.list_assign_0051b720(c + 0x330, list);
            changed = 1;
        }
    }
    p.free_006fa390(m.read(list));
    return changed;
}
}
