// W2 tier 4 G07 player last-update object. See w2_g07_playerlast.hpp. Comments give original addresses.
#include "4002af1e06-w2_g07_playerlast.hpp"
#include <cstring>
#include <initializer_list>
namespace w2::g07 {
namespace {
using I = std::int32_t;
constexpr Word kInvalid = 0x7f000000u;
Word w16(Memory& m, Word a) { return m.read16(a) & 0xffffu; }
Word b8(Memory& m, Word a) { return m.read8(a) & 0xffu; }
// CExoArrayList<DWORD> append with the inline growth used throughout (0 -> 10h, then doubled)
void append_dword(Memory& m, PlayerLastPorts& x, Word list, Word value) {
    if (m.read(list + 4) == m.read(list + 8)) {
        const Word cap = m.read(list + 8) != 0 ? m.read(list + 8) * 2 : 0x10;
        const Word old = m.read(list);
        m.write(list + 8, cap);
        const Word fresh = x.allocate_006fa7e6(cap * 4);
        m.write(list, fresh);
        for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(list + 4)); ++k) m.write(fresh + 4 * k, m.read(old + 4 * k));
        if (old != 0) x.free_006fa390(old);
    }
    m.write(m.read(list) + 4 * m.read(list + 4), value);
    m.write(list + 4, m.read(list + 4) + 1);
}
// remove index `at` from a CExoArrayList<DWORD> (count decremented first, then shift down)
void remove_at(Memory& m, Word list, Word at) {
    m.write(list + 4, m.read(list + 4) - 1);
    for (Word k = at; static_cast<I>(k) < static_cast<I>(m.read(list + 4)); ++k) m.write(m.read(list) + 4 * k, m.read(m.read(list) + 4 * k + 4));
}
}

// ===================================================================== small unowned helpers
Word action_node_construct_004eadd0(Memory& m, Word n) {
    m.write(n + 0x0c, 0); m.write(n + 0x10, 0); m.write(n + 0x14, 0);
    m.write16(n, 0xffff); m.write16(n + 2, 0xffff); m.write16(n + 4, 0xffff);
    m.write(n + 0x0c, 0); m.write(n + 0x10, 0); m.write(n + 0x14, 0);
    m.write(n + 0x18, kInvalid); m.write(n + 8, 0xffffffffu);
    return n;
}
Word stats_known_spell_005a6750(Memory& m, Word stats, Word cls, Word flag, Word index) {
    if ((cls & 0xffu) >= b8(m, stats + 0x89)) return 0xffffffffu;
    if ((flag & 0xffu) >= 1) return 0xffffffffu;
    const Word list = stats + (cls & 0xffu) * 0x28 + 0x8c + (flag & 0xffu) * 0x0c;
    const Word i = index & 0xffu;
    if (static_cast<I>(i) >= static_cast<I>(m.read(list + 4))) return 0xffffffffu;
    return m.read(m.read(list) + 4 * i);
}
Word stats_number_known_spells_005a67a0(Memory& m, Word stats, Word cls, Word flag) {
    if ((cls & 0xffu) >= b8(m, stats + 0x89)) return 0;
    return w16(m, stats + (cls & 0xffu) * 0x28 + ((flag & 0xffu) + 0x0c) * 0x0c);
}
// 006475C0 CSWSpellLikeAbility ctor is W3's (t6b ruling): PlayerLastPorts::sla_construct_006475c0.
void spell_like_allocate_0056b710(Memory& m, PlayerLastPorts& x, Word list, Word count) {
    const Word old = m.read(list);
    m.write(list + 8, count);
    Word fresh = x.allocate_006fa7e6(count * 0x0c);
    if (fresh != 0) for (Word k = 0; static_cast<I>(k) < static_cast<I>(count); ++k) (void)x.sla_construct_006475c0(fresh + 0x0c * k);
    m.write(list, fresh);
    for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(list + 4)); ++k)
        for (Word d = 0; d < 0x0c; d += 4) m.write(m.read(list) + 0x0c * k + d, m.read(old + 0x0c * k + d));
    if (old != 0) x.free_006fa390(old);
}

// ===================================================================== CSWSPlayerLastUpdateObject
void clear_action_queue_005ca0d0(Memory& m, Word self) {
    for (Word k = 0; k < 10; ++k) {
        const Word n = self + 0x40 + 0x1c * k;
        m.write(n + 0x0c, 0); m.write(n + 0x10, 0); m.write(n + 0x14, 0);
        m.write16(n, 0xffff); m.write16(n + 2, 0xffff); m.write16(n + 4, 0xffff);
        m.write(n + 0x18, kInvalid); m.write(n + 8, 0xffffffffu);
    }
}
void clear_known_spells_005cab20(Memory& m, PlayerLastPorts& x, Word self) {
    for (Word k = 0; k < 2; ++k) {
        const Word l = self + 0x164 + 0x0c * k;
        if (m.read(l) != 0) { x.free_006fa390(m.read(l)); m.write(l, 0); m.write(l + 8, 0); }
        m.write(l + 4, 0);
    }
}
void clear_known_spell_uses_005ca060(Memory& m, Word self) { m.write16(self + 0x1ac, 0); }
void clear_spell_add_delete_005cac30(Memory& m, PlayerLastPorts& x, Word self) {
    for (Word k = 0; k < 2; ++k) {
        const Word a = self + 0x17c + 0x0c * k, b = self + 0x194 + 0x0c * k;
        if (m.read(a) != 0) { x.free_006fa390(m.read(a)); m.write(a, 0); m.write(a + 8, 0); }
        m.write(a + 4, 0);
        if (m.read(b) != 0) { x.free_006fa390(m.read(b)); m.write(b, 0); m.write(b + 8, 0); }
        m.write(b + 4, 0);
    }
}
void clear_feats_005cab60(Memory& m, PlayerLastPorts& x, Word self) {
    for (Word l : {self + 0x18, self + 0x24}) {
        if (m.read(l) != 0) { x.free_006fa390(m.read(l)); m.write(l, 0); m.write(l + 8, 0); }
        m.write(l + 4, 0);
    }
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(self + 0x34)); ++i) {
        const Word e = m.read(m.read(self + 0x30) + 4 * i);
        if (e != 0) { x.free_006fa390(e); m.write(m.read(self + 0x30) + 4 * i, 0); }
    }
    if (m.read(self + 0x30) != 0) { x.free_006fa390(m.read(self + 0x30)); m.write(self + 0x30, 0); m.write(self + 0x38, 0); }
    m.write(self + 0x34, 0);
}
void clear_visibility_list_005ca780(Memory& m, PlayerLastPorts& x, Word self) {
    if (static_cast<I>(m.read(self + 0x15c)) <= 0) return;
    for (I i = static_cast<I>(m.read(self + 0x15c)) - 1; i >= 0; --i) {
        const Word e = m.read(m.read(self + 0x158) + 4 * static_cast<Word>(i));
        remove_at(m, self + 0x158, static_cast<Word>(i));
        if (e != 0) x.free_006fa390(e);
    }
}
Word player_last_construct_005cac80(Memory& m, PlayerLastPorts& x, Word self) {
    m.write(self + 0x1c, 0); m.write(self + 0x20, 0); m.write(self + 0x18, 0);
    m.write(self + 0x28, 0); m.write(self + 0x2c, 0); m.write(self + 0x24, 0);
    m.write(self + 0x34, 0); m.write(self + 0x38, 0); m.write(self + 0x30, 0);
    for (Word k = 0; k < 10; ++k) (void)action_node_construct_004eadd0(m, self + 0x40 + 0x1c * k);
    m.write(self + 0x15c, 0); m.write(self + 0x160, 0); m.write(self + 0x158, 0);
    for (Word base : {0x164u, 0x17cu, 0x194u})
        for (Word k = 0; k < 2; ++k) x.array_construct_005a76d0(self + base + 0x0c * k);
    m.write(self + 8, 0);
    m.write(self + 0x1b0, 0);
    return self;
}
void player_last_destroy_005cad70(Memory& m, PlayerLastPorts& x, Word self) {
    clear_known_spells_005cab20(m, x, self);
    clear_visibility_list_005ca780(m, x, self);
    const Word sla = m.read(self + 0x1b0);
    if (sla != 0) { x.free_006fa390(m.read(sla)); m.write(sla, 0); x.free_006fa390(sla); }
    m.write(self + 0x1b0, 0);
    for (Word base : {0x194u, 0x17cu, 0x164u})                    // eh_vector_destructor_iterator: last element first
        for (I k = 1; k >= 0; --k) x.array_destruct_0056b6f0(self + base + 0x0c * static_cast<Word>(k));
    x.free_006fa390(m.read(self + 0x158)); m.write(self + 0x158, 0);
    x.free_006fa390(m.read(self + 0x30)); m.write(self + 0x30, 0);
    x.free_006fa390(m.read(self + 0x24)); m.write(self + 0x24, 0);
    x.free_006fa390(m.read(self + 0x18)); m.write(self + 0x18, 0);
}
void add_known_spell_005cabe0(Memory& m, PlayerLastPorts& x, Word self, Word cls, Word flag, Word spell) {
    const Word l = self + 0x164 + ((cls & 0xffu) + (flag & 0xffu)) * 0x0c;
    if (m.read(l + 4) == m.read(l + 8)) x.list_allocate_005e03c0(l, m.read(l + 8) != 0 ? m.read(l + 8) * 2 : 0x10);
    m.write(m.read(l) + 4 * m.read(l + 4), spell);
    m.write(l + 4, m.read(l + 4) + 1);
}
Word plast_known_spell_005ca740(Memory& m, Word self, Word cls, Word flag, Word index) {
    const Word n = (cls & 0xffu) + (flag & 0xffu), i = index & 0xffu;
    if (static_cast<I>(i) >= static_cast<I>(m.read(self + 0x168 + 0x0c * n))) return 0xffffffffu;
    return m.read(m.read(self + 0x164 + 0x0c * n) + 4 * i);
}

// ===================================================================== CSWSMessage compute / store helpers
Word gui_known_spells_00570930(Memory& m, PlayerLastPorts& x, Word, Word creature, Word plast) {
    const Word stats = m.read(creature + 0xa74);
    for (Word k = 0; k < 2; ++k) {
        const Word count = w16(m, plast + 0x168 + 0x0c * k);
        Word matched = 0;                                          // byte at the `plast` argument slot
        if (count != 0) {
            if (count > 0xff) x.trap(0x00570a67, "W2: GuiKnownSpells byte index never reaches a count above 255 (original loops forever)");
            for (Word j = 0; j < count; j = (j + 1) & 0xffu) {
                const Word a = plast_known_spell_005ca740(m, plast, k, 0, j);
                if (a == stats_known_spell_005a6750(m, stats, k, 0, matched)) { matched = (matched + 1) & 0xffu; continue; }
                append_dword(m, x, plast + 0x17c + 0x0c * k, a);   // known by the last update, not matched: delete list
            }
        }
        Word i = matched;
        Word known = stats_number_known_spells_005a67a0(m, stats, k, 0);
        if (known > 0xff && i < known) x.trap(0x00570b41, "W2: GuiKnownSpells byte index never reaches a count above 255 (original loops forever)");
        while ((i & 0xffu) < known) {
            append_dword(m, x, plast + 0x194 + 0x0c * k, stats_known_spell_005a6750(m, stats, k, 0, i));   // add list
            i = (i + 1) & 0xffu;
            known = stats_number_known_spells_005a67a0(m, stats, k, 0);
        }
    }
    for (Word c = 0; c < 2; ++c)
        if (m.read(plast + 0x198 + 0x0c * c) != 0 || m.read(plast + 0x180 + 0x0c * c) != 0) return 1;
    return 0;
}
Word gui_known_spell_uses_00567490(Memory& m, PlayerLastPorts& x, Word, Word creature, Word plast) {
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(b8(m, m.read(creature + 0xa74) + 0x89)); ++i) {
        const Word cls = x.get_class_005a4e90(m.read(creature + 0xa74), i) & 0xffu;
        if (x.is_jedi_005be4a0(cls) == 0) continue;
        const Word a = x.known_spell_uses_005ca070(plast, i, 0) & 0xffu;
        if (a != (x.spells_per_day_left_005a5650(m.read(creature + 0xa74), i, 0) & 0xffu)) return 1;
    }
    return 0;
}
Word action_queue_changed_00567590(Memory& m, PlayerLastPorts& x, Word, Word creature, Word plast) {
    for (Word k = 0; k < 10; ++k) {
        const Word c = creature + 0x3a8 + 0x1c * k, l = plast + 0x40 + 0x1c * k;
        if (w16(m, c + 2) != w16(m, l + 2) || w16(m, c) != w16(m, l) || w16(m, c + 4) != w16(m, l + 4)) return 1;
        if (m.read(c + 0x18) != m.read(l + 0x18)) return 1;
        if (x.vector_equal_004aa980(c + 0x0c, l + 0x0c) == 0) return 1;
    }
    return 0;
}
Word visibility_changed_0056b880(Memory& m, Word, Word creature, Word plast) {
    const Word mine = m.read(plast + 0x15c), theirs = m.read(creature + 0x924);
    if (theirs != mine) return 1;
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(theirs); ++i) {
        const Word e = m.read(m.read(creature + 0x920) + 4 * i);
        if (static_cast<I>(mine) <= 0) return 1;
        // 0056B8F0: the inner loop runs `mine` times but always compares element i (original)
        const Word q = m.read(m.read(plast + 0x158) + 4 * i);
        if (m.read(e) != m.read(q)) return 1;
        const Word d = b8(m, e + 4) ^ b8(m, q + 4);
        if ((d & 0x13u) != 0) return 1;
    }
    return 0;
}
Word gui_feats_0056c310(Memory& m, PlayerLastPorts& x, Word, Word creature, Word plast) {
    const Word s = m.read(creature + 0xa74);
    if (w16(m, s + 4) != m.read(plast + 0x1c) || w16(m, s + 0x1c) != m.read(plast + 0x28) || m.read(s + 0x10) != m.read(plast + 0x34)) return 1;
    for (Word i = 0; (i & 0xffffu) < w16(m, m.read(creature + 0xa74) + 4); ++i) {
        const Word f = x.get_feat_005a6920(m.read(creature + 0xa74), i) & 0xffffu;
        bool found = false;
        for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(plast + 0x1c)); ++k) if (w16(m, m.read(plast + 0x18) + 2 * k) == f) found = true;
        if (!found) return 1;
    }
    for (Word i = 0; (i & 0xffffu) < w16(m, m.read(creature + 0xa74) + 0x1c); ++i) {
        const Word f = x.bonus_feat_005a6940(m.read(creature + 0xa74), i) & 0xffffu;
        bool found = false;
        for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(plast + 0x28)); ++k) if (w16(m, m.read(plast + 0x24) + 2 * k) == f) found = true;
        if (!found) return 1;
    }
    const Word st = m.read(creature + 0xa74);
    const I n = static_cast<I>(m.read(st + 0x10));
    for (Word i = 0; static_cast<I>(i & 0xffffu) < n; ++i) {
        const Word a = m.read(m.read(st + 0x0c) + 4 * (i & 0xffffu)), b = m.read(m.read(plast + 0x30) + 4 * (i & 0xffffu));
        if (w16(m, a) != w16(m, b) || b8(m, a + 2) != b8(m, b + 2)) return 1;
    }
    return 0;
}
Word compare_spell_like_0056c430(Memory& m, Word, Word creature, Word plast) {
    const Word mine = m.read(plast + 0x1b0), theirs = m.read(m.read(creature + 0xa74) + 0x128);
    const Word count = m.read(theirs + 4);
    const Word my_count = mine != 0 ? m.read(mine + 4) : 0;
    Word diff = my_count != count ? 1u : 0u;
    if (static_cast<I>(count) <= 0) return diff;
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(theirs + 4)); ++i) {
        if (static_cast<I>(i) >= static_cast<I>(my_count)) { ++diff; continue; }
        const Word a = m.read(theirs) + 0x0c * i, b = m.read(mine) + 0x0c * i;
        if (m.read(a + 4) != m.read(b + 4) || b8(m, a + 8) != b8(m, b + 8) || m.read(a) != m.read(b)) ++diff;
    }
    return diff;
}
void update_action_queue_00567520(Memory& m, Word, Word creature, Word plast) {
    for (Word k = 0; k < 10; ++k) {
        const Word c = creature + 0x3a8 + 0x1c * k, l = plast + 0x40 + 0x1c * k;
        m.write16(l + 2, w16(m, c + 2)); m.write16(l, w16(m, c)); m.write16(l + 4, w16(m, c + 4));
        m.write(l + 0x18, m.read(c + 0x18));
        m.write(l + 0x0c, m.read(c + 0x0c)); m.write(l + 0x10, m.read(c + 0x10)); m.write(l + 0x14, m.read(c + 0x14));
        m.write(l + 8, m.read(c + 8));
    }
}
void update_visibility_list_005707a0(Memory& m, PlayerLastPorts& x, Word, Word creature, Word plast) {
    clear_visibility_list_005ca780(m, x, plast);                  // inlined copy at 005707A6..0057080A
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(creature + 0x924)); ++i) {
        const Word src = m.read(m.read(creature + 0x920) + 4 * i);
        const Word e = x.allocate_006fa7e6(8);
        if (e == 0) x.trap(0x00570850, "W2: original writes through a null visibility element (allocation failed)");
        m.write(e, kInvalid);
        x.write8(e + 4, b8(m, e + 4) & 0xe0u);                    // upper three bits keep the allocator's contents (original)
        m.write(e, m.read(src));
        Word bits = b8(m, e + 4);
        bits ^= (b8(m, src + 4) ^ bits) & 2u; bits ^= (b8(m, src + 4) ^ bits) & 1u; bits ^= (b8(m, src + 4) ^ bits) & 0x10u;
        x.write8(e + 4, bits);
        append_dword(m, x, plast + 0x158, e);
    }
}
void store_spell_like_00570ba0(Memory& m, PlayerLastPorts& x, Word, Word creature, Word plast) {
    const Word src = m.read(m.read(creature + 0xa74) + 0x128);
    const Word n = m.read(src + 4);
    Word list = m.read(plast + 0x1b0);
    if (list == 0) {
        list = x.allocate_006fa7e6(0x0c);
        if (list != 0) { m.write(list + 4, 0); m.write(list + 8, 0); m.write(list, 0); }
        m.write(plast + 0x1b0, list);
    }
    if (n == 0) return;
    if (list == 0) x.trap(0x00570c0d, "W2: original allocates into a null spell-like-ability list (allocation failed)");
    spell_like_allocate_0056b710(m, x, list, n);
    m.write(list + 4, n);
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(n); ++i) {
        const Word a = m.read(src) + 0x0c * i, b = m.read(list) + 0x0c * i;
        m.write(b + 4, m.read(a + 4)); x.write8(b + 8, b8(m, a + 8)); m.write(b, m.read(a));
    }
}

// ===================================================================== 00572A20
void test_player_differences_00572a20(Memory& m, w2::entry::Ports& p, PlayerLastPorts& x, Word self, Word player, Word plast, Word last, Word fo, Word s) {
    const Word go = p.game_object_00561080(player);
    if (go == 0) x.trap(0x00572a34, "W2: original calls through a null player game object (TestPlayerUpdateDifferences)");
    const Word c = p.vcall0(go, 0x30);
    if (c == 0) return;
    auto set = [&](Word bit) { m.write(fo, m.read(fo) | bit); };
    if (w16(m, plast + 0x14) != (x.gui_skills_00567430(c) & 0xffffu)) set(2);
    if (w16(m, plast + 0x3c) != (x.player_state_00567600(c) & 0xffffu)) set(8);
    if (gui_known_spells_00570930(m, x, self, c, plast) != 0) set(0x10);
    if (gui_known_spell_uses_00567490(m, x, self, c, plast) != 0) set(0x40);
    if (action_queue_changed_00567590(m, x, self, c, plast) != 0) set(0x100);
    if (visibility_changed_0056b880(m, self, c, plast) != 0) set(0x2000);
    if (gui_feats_0056c310(m, x, self, c, plast) != 0) set(4);
    bool changed = x.gold_004edd60(c) != m.read(plast);
    if (!changed) changed = w16(m, plast + 4) != (x.armor_class_004ed1d0(c) & 0xffffu);
    if (!changed) changed = static_cast<I>(static_cast<std::int16_t>(w16(m, plast + 6))) != static_cast<I>(m.read(c + 0xa44));
    if (!changed) changed = m.read(c + 0xa40) == 1;
    if (!changed) changed = m.read(plast + 0x10) != m.read(m.read(c + 0xa74) + 0x68);
    if (!changed) changed = m.read(plast + 8) != x.can_level_up_005a6810(m.read(c + 0xa74));
    if (!changed) changed = m.read(plast + 0x0c) != (x.stats_level_005a5fd0(m.read(c + 0xa74), 0) & 0xffu);
    if (changed) set(1);
    if (last != 0 && m.read(c + 0xa74) != 0) {
        const Word st = m.read(c + 0xa74);
        bool abil = false;
        for (Word k = 0; k < 6 && !abil; ++k) if (b8(m, st + 0xe9 + 2 * k) != b8(m, last + 0x124 + k)) abil = true;
        if (abil) set(0x800);
        // 00572BAC: the stat getters run regardless of the base-ability result
        if ((x.str_stat_005a6190(m.read(c + 0xa74)) & 0xffu) != b8(m, last + 0x12a)
            || (x.dex_stat_005a6550(m.read(c + 0xa74)) & 0xffu) != b8(m, last + 0x12b)
            || (x.con_stat_005a6250(m.read(c + 0xa74)) & 0xffu) != b8(m, last + 0x12c)
            || (x.int_stat_005a6310(m.read(c + 0xa74)) & 0xffu) != b8(m, last + 0x12d)
            || (x.wis_stat_005a63d0(m.read(c + 0xa74)) & 0xffu) != b8(m, last + 0x12e)
            || (x.cha_stat_005a6490(m.read(c + 0xa74)) & 0xffu) != b8(m, last + 0x12f))
            set(0x800);
        for (Word i = 0; i < b8(m, x.rules_007a3a28() + 0xab); ++i) {
            if ((x.skill_rank_005aa570(m.read(c + 0xa74), i, 0, 1) & 0xffu) != b8(m, last + 0x138 + i)) set(0x8000);
            if ((x.skill_rank_005aa570(m.read(c + 0xa74), i, 0, 0) & 0xffu) != b8(m, last + 0x140 + i)) set(0x8000);
        }
        const Word ref = p.vcall1(c, 0xc8, s);                    // hidden CResRef result
        if (x.resref_ne_00406dc0(ref, last + 0x7c) != 0 || (p.vcall0(c, 0xd0) & 0xffffu) != w16(m, last + 0x8c)) set(0x10000);
        if (w16(m, last + 0xbc) != w16(m, c + 0xe0) || w16(m, last + 0xba) != (p.vcall1(c, 0x9c, 0) & 0xffffu)
            || w16(m, last + 0xbe) != w16(m, c + 0xe4) || w16(m, last + 0xc0) != (p.vcall1(c, 0x98, 1) & 0xffffu))
            set(0x200);
        const Word st2 = m.read(c + 0xa74);
        if (w16(m, last + 0x132) != w16(m, st2 + 0x122) || w16(m, last + 0x130) != ((w16(m, st2 + 0x126) + w16(m, st2 + 0x124)) & 0xffffu)
            || w16(m, last + 0x134) != w16(m, st2 + 0x126) || w16(m, last + 0x136) != (x.max_force_points_004fd490(c) & 0xffffu))
            set(0x20000);
    }
    if (compare_spell_like_0056c430(m, self, c, plast) != 0) set(0x4000);
}

// ===================================================================== 00574400
void store_player_values_00574400(Memory& m, w2::entry::Ports& p, PlayerLastPorts& x, Word self, Word player, Word plast, Word last, Word flags, Word s) {
    const Word go = p.game_object_00561080(player);
    if (go == 0) x.trap(0x00574414, "W2: original calls through a null player game object (StoreValuesInLastPlayerUpdateObject)");
    const Word c = p.vcall0(go, 0x30);
    if (c == 0) return;
    if ((flags & 1u) != 0) {
        m.write16(plast + 4, x.armor_class_004ed1d0(c));
        m.write(plast, x.gold_004edd60(c));
        m.write16(plast + 6, w16(m, c + 0xa44));
        m.write(plast + 8, x.can_level_up_005a6810(m.read(c + 0xa74)));
        m.write(plast + 0x0c, x.stats_level_005a5fd0(m.read(c + 0xa74), 0) & 0xffu);
        m.write(plast + 0x10, m.read(m.read(c + 0xa74) + 0x68));
        m.write(c + 0xa40, 0);
    }
    if ((flags & 2u) != 0) m.write16(plast + 0x14, x.gui_skills_00567430(c));
    if ((flags & 8u) != 0) m.write16(plast + 0x3c, x.player_state_00567600(c));
    if ((flags & 4u) != 0) {
        m.write(plast + 0x1c, 0);
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(w16(m, m.read(c + 0xa74) + 4)); ++i) {
            const Word f = x.get_feat_005a6920(m.read(c + 0xa74), i);
            const Word l = plast + 0x18;
            if (m.read(l + 4) == m.read(l + 8)) x.list_u16_allocate_004d2fa0(l, m.read(l + 8) != 0 ? m.read(l + 8) * 2 : 0x10);
            m.write16(m.read(l) + 2 * m.read(l + 4), f);
            m.write(l + 4, m.read(l + 4) + 1);
        }
        m.write(plast + 0x28, 0);
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(w16(m, m.read(c + 0xa74) + 0x1c)); ++i) {
            const Word f = x.bonus_feat_005a6940(m.read(c + 0xa74), i);
            const Word l = plast + 0x24;
            if (m.read(l + 4) == m.read(l + 8)) {                    // 00574556: inline WORD growth
                const Word cap = m.read(l + 8) != 0 ? m.read(l + 8) * 2 : 0x10;
                const Word old = m.read(l);
                m.write(l + 8, cap);
                const Word fresh = x.allocate_006fa7e6(cap * 2);
                m.write(l, fresh);
                for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(l + 4)); ++k) m.write16(fresh + 2 * k, w16(m, old + 2 * k));
                if (old != 0) x.free_006fa390(old);
            }
            m.write16(m.read(l) + 2 * m.read(l + 4), f);
            m.write(l + 4, m.read(l + 4) + 1);
        }
        for (I k = static_cast<I>(m.read(plast + 0x34)) - 1; k >= 0; --k) {   // 005745D6: free and remove from the end
            x.free_006fa390(m.read(m.read(plast + 0x30) + 4 * static_cast<Word>(k)));
            m.write(m.read(plast + 0x30) + 4 * static_cast<Word>(k), 0);
            remove_at(m, plast + 0x30, static_cast<Word>(k));
        }
        const Word st = m.read(c + 0xa74);
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(st + 0x10)); ++i) {
            const Word e = x.allocate_006fa7e6(4);
            if (e == 0) x.trap(0x00574649, "W2: original writes through a null feat-use element (allocation failed)");
            const Word src = m.read(m.read(st + 0x0c) + 4 * i);
            m.write16(e, w16(m, src));
            x.write8(e + 2, b8(m, src + 2));
            append_dword(m, x, plast + 0x30, e);
        }
    }
    if ((flags & 0x10u) != 0) {
        clear_known_spells_005cab20(m, x, plast);
        for (Word k = 0; k < 2; ++k) {
            const Word stats = m.read(c + 0xa74);
            Word known = stats_number_known_spells_005a67a0(m, stats, k, 0);
            if (known > 0xff) x.trap(0x00574759, "W2: StoreValues byte spell index never reaches a count above 255 (original loops forever)");
            for (Word j = 0; (j & 0xffu) < known; ) {
                add_known_spell_005cabe0(m, x, plast, k, 0, stats_known_spell_005a6750(m, m.read(c + 0xa74), k, 0, j));
                j = (j + 1) & 0xffu;
                known = stats_number_known_spells_005a67a0(m, m.read(c + 0xa74), k, 0);
                if (known > 0xff) x.trap(0x00574759, "W2: StoreValues byte spell index never reaches a count above 255 (original loops forever)");
            }
        }
    }
    if ((flags & 0x100u) != 0) update_action_queue_00567520(m, self, c, plast);
    if ((flags & 0x2000u) != 0) update_visibility_list_005707a0(m, x, self, c, plast);
    if ((flags & 0x800u) != 0 && last != 0 && m.read(c + 0xa74) != 0) {
        for (Word k = 0; k < 6; ++k) x.write8(last + 0x124 + k, b8(m, m.read(c + 0xa74) + 0xe9 + 2 * k));
        x.write8(last + 0x12a, x.str_stat_005a6190(m.read(c + 0xa74)) & 0xffu);
        x.write8(last + 0x12b, x.dex_stat_005a6550(m.read(c + 0xa74)) & 0xffu);
        x.write8(last + 0x12c, x.con_stat_005a6250(m.read(c + 0xa74)) & 0xffu);
        x.write8(last + 0x12d, x.int_stat_005a6310(m.read(c + 0xa74)) & 0xffu);
        x.write8(last + 0x12e, x.wis_stat_005a63d0(m.read(c + 0xa74)) & 0xffu);
        x.write8(last + 0x12f, x.cha_stat_005a6490(m.read(c + 0xa74)) & 0xffu);
    }
    if ((flags & 0x8000u) != 0 && last != 0 && m.read(c + 0xa74) != 0) {
        for (Word i = 0; i < b8(m, x.rules_007a3a28() + 0xab); ++i) {
            x.write8(last + 0x138 + i, x.skill_rank_005aa570(m.read(c + 0xa74), i, 0, 1) & 0xffu);
            x.write8(last + 0x140 + i, x.skill_rank_005aa570(m.read(c + 0xa74), i, 0, 0) & 0xffu);
        }
    }
    if ((flags & 0x10000u) != 0) {
        if (last == 0) x.trap(0x00574928, "W2: original stores the portrait into a null last-update object");
        m.write16(last + 0x8c, p.vcall0(c, 0xd0));
        const Word ref = p.vcall1(c, 0xc8, s);
        x.resref_assign_00406120(last + 0x7c, s + 0x10, ref);
    }
    if ((flags & 0x200u) != 0) {
        if (last == 0) x.trap(0x00574954, "W2: original stores hit points into a null last-update object");
        m.write16(last + 0xbc, w16(m, c + 0xe0));
        m.write16(last + 0xba, p.vcall1(c, 0x9c, 0));
        m.write16(last + 0xbe, w16(m, c + 0xe4));
        m.write16(last + 0xc0, p.vcall1(c, 0x98, 1));
    }
    if ((flags & 0x20000u) != 0) {
        if (last == 0) x.trap(0x005749a4, "W2: original stores force points into a null last-update object");
        const Word st = m.read(c + 0xa74);
        m.write16(last + 0x132, w16(m, st + 0x122));
        m.write16(last + 0x130, (w16(m, st + 0x126) + w16(m, st + 0x124)) & 0xffffu);
        m.write16(last + 0x134, w16(m, st + 0x126));
        m.write16(last + 0x136, x.max_force_points_004fd490(c));
    }
    if ((flags & 0x4000u) != 0) store_spell_like_00570ba0(m, x, self, c, plast);
}
}
