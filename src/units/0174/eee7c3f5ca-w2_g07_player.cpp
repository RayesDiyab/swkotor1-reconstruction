// W2 tier 4: WriteGameObjUpdate_PlayerUpdate 00572DC0 and small callees. See w2_g07_player.hpp.
// Comments give original instruction addresses.
#include "5ada6db8dd-w2_g07_player.hpp"
#include "74be47154e-module_message.hpp"
#include <initializer_list>
namespace w2::g07 {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
using I = std::int32_t;
constexpr Word kOne = 0x3f800000u;
void wbyte(Memory& m, w2::entry::Ports& p, Word self, Word v) { write_byte_004d62c0(m, p, self, v, 8); }
void wword(Memory& m, w2::entry::Ports& p, Word self, Word v) { write_word16_004d6300(m, p, self, v, 0x10); }
void wdword(Memory& m, w2::entry::Ports& p, Word self, Word v) { sm::message_word_004d6340(m, p, self, v, 0x20); }
void wbool(Memory& m, w2::entry::Ports& p, Word self, Word v) { write_bool_004d68e0(m, p, self, v); }
void wfloat(Memory& m, w2::entry::Ports& p, Word self, Word v) { write_float_bits_004d6920(m, p, self, v, kOne, 0x20); }
void wid(Memory& m, w2::entry::Ports& p, Word self, Word id) { write_object_id_00578570(m, p, self, id); }
void put8(w2::entry::Ports& p, Word a, Word v) { p.write_byte(a, v & 0xffu); }

// Inline CExoArrayList<T>::Add as compiled in 00572DC0: grow 0 -> 10h, then x2; copy `count` elements;
// free the old block when non-null; store; count++. list = {data, count, capacity} in the frame.
void list_add(Memory& m, w2::entry::Ports& p, Word list, Word value, Word size) {
    const Word count = m.read(list + 4);
    if (count == m.read(list + 8)) {
        const Word cap = m.read(list + 8) != 0 ? m.read(list + 8) * 2 : 0x10;
        const Word old = m.read(list);
        m.write(list + 8, cap);
        const Word fresh = p.allocate(cap * size);
        m.write(list, fresh);
        for (Word k = 0; static_cast<I>(k) < static_cast<I>(count); ++k) {
            if (size == 1) put8(p, fresh + k, m.read8(old + k));
            else if (size == 2) m.write16(fresh + 2 * k, m.read16(old + 2 * k));
            else m.write(fresh + 4 * k, m.read(old + 4 * k));
        }
        if (old != 0) p.free(old);
    }
    const Word at = m.read(list) + size * m.read(list + 4);
    if (size == 1) put8(p, at, value); else if (size == 2) m.write16(at, value & 0xffffu); else m.write(at, value);
    m.write(list + 4, m.read(list + 4) + 1);
}
void zero_lists(Memory& m, Word f) { for (Word o = 0x20; o < 0x38; o += 4) m.write(f + o, 0); }
}

// ===================================================================== small callees
Word known_spell_uses_005ca070(Memory& m, Word plast, Word cls, Word level) {
    if ((cls & 0xffu) >= 2 || (level & 0xffu) >= 1) return 0;
    return m.read8(plast + (cls & 0xffu) + (level & 0xffu) + 0x1ac);
}
void set_known_spell_uses_005ca0a0(Memory& m, Word plast, Word cls, Word level, Word value) {
    if ((cls & 0xffu) >= 2 || (level & 0xffu) >= 1) return;
    const Word a = plast + (cls & 0xffu) + (level & 0xffu) + 0x1ac;
    m.write(a, (m.read(a) & 0xffffff00u) | (value & 0xffu));
}
Word spells_per_day_left_005a5650(Memory& m, Word stats, Word cls, Word level) {
    if ((cls & 0xffu) >= m.read8(stats + 0x89) || (level & 0xffu) >= 1) return 0;
    return m.read8(stats + (level & 0xffu) + (cls & 0xffu) * 40 + 0xa5);
}
Word can_level_up_005a6810(Memory& m, w2::entry::Ports& p, PlayerPorts& x, Word stats) {
    const Word server = m.read(p.app_007a39fc() + 8);
    const Word level = x.stats_level_005a5fd0(stats, 0) & 0xffu;
    if (static_cast<I>(level) >= static_cast<I>(m.read(x.server_info_004aec90(server) + 0x94))) return 0;
    const Word level2 = x.stats_level_005a5fd0(stats, 0) & 0xffu;
    if (m.read(stats + 0x68) < m.read(p.rules_007a3a28() + level2 * 4 + 0x38)) return 0;   // 005A684E: unsigned JB
    if (p.vcall0(m.read(stats + 0x24), 0x94) != 0) return 0;
    if (x.dead_temp_004ef890(m.read(stats + 0x24)) != 0) return 0;
    return 1;
}
Word gui_skills_00567430(Memory& m, PlayerPorts& x, Word c) {
    const Word st = m.read(c + 0xa74);
    Word f = 0;
    if (x.can_use_skill_005af880(st, 1) != 0) f = 2;
    if (x.can_use_skill_005af880(st, 2) != 0) f |= 0x10;
    if (x.can_use_skill_005af880(st, 2) != 0) f |= 0x10;        // 00567460: skill 2 asked twice (original)
    if (x.can_use_skill_005af880(st, 6) != 0) f |= 0x20;
    return f;
}
Word player_state_00567600(Memory& m, PlayerPorts& x, Word c) {
    Word f = 0;
    if ((x.detect_mode_004edae0(c) & 0xffu) == 1) f = 1;
    if (m.read8(c + 0x4d1) == 1) f |= 2;
    const Word mode = m.read8(c + 0x4d2);
    if (mode == 1) f |= 4;
    if (mode == 2) f |= 8;
    if (mode == 3) f |= 0x10;
    if (mode == 5) f |= 0x20;
    if (mode == 6) f |= 0x40;
    if (m.read(c + 0x8e8) == 1) f |= 0x80;
    if (m.read8(c + 0xa10) != 0) f |= 0x100;
    if (mode == 4) f |= 0x200;
    if (m.read(c + 0xa48) != 0) f |= 0x400;
    if (m.read(c + 0xe8) != 0) f |= 0x800;
    if (m.read(c + 0x384) != 0) f |= 0x1000;                     // 0056769E: JBE (unsigned != 0)
    return f;
}

// 0056C4D0: WORD Compare(…) then, when the counts differ, 'S' + WORD new count; then one 'U' record per
// creature entry (12 bytes: +0 WORD, +4 DWORD, +8 BYTE) that is past the old count or differs from it.
// Old list = plast+1B0 (pointer to {data, count}); new list = [stats+128] {data, count}.
void spell_like_ability_0056c4d0(Memory& m, w2::entry::Ports& p, PlayerPorts& x, Word self, Word c, Word plast) {
    wword(m, p, self, x.compare_spell_like_0056c430(self, c, plast));
    const Word old = m.read(plast + 0x1b0);
    const Word now = m.read(m.read(c + 0xa74) + 0x128);
    const Word n = m.read(now + 4);
    const Word old_n = old != 0 ? m.read(old + 4) : 0;
    if (old_n != n) { wbyte(m, p, self, 0x53); wword(m, p, self, n); }
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(n); ++i) {
        const Word e = m.read(now) + 0x0c * i;
        if (static_cast<I>(i) < static_cast<I>(old_n)) {          // 0056C543: signed compare
            const Word o = m.read(old) + 0x0c * i;
            if (m.read(e + 4) == m.read(o + 4) && m.read8(e + 8) == m.read8(o + 8) && m.read(e) == m.read(o)) continue;
        }
        wbyte(m, p, self, 0x55); wword(m, p, self, i);
        wbool(m, p, self, m.read(e + 4)); wbyte(m, p, self, m.read8(e + 8)); wword(m, p, self, m.read16(e));
    }
}

// ===================================================================== 00572DC0
void player_update_00572dc0(Memory& m, w2::entry::Ports& p, PlayerPorts& x, Word self, Word player, Word plast, Word last, Word flags, Word F) {
    // emulated frame: [esp+N] == F+N after PUSH EDI. Argument slots +4C..+58, saved EBP (this) at +18.
    m.write(F + 0x18, self); m.write(F + 0x4c, player); m.write(F + 0x50, plast); m.write(F + 0x54, last); m.write(F + 0x58, flags);
    const Word go = p.game_object_00561080(player);
    if (go == 0) x.trap(0x00572deb, "W2: original calls a vtable on a null player game object (PlayerUpdate)");
    const Word c = p.vcall0(go, 0x30);
    m.write(F + 0x10, c);
    if (c == 0) return;
    wbyte(m, p, self, 0x49);
    wid(m, p, self, m.read(c + 4));
    wdword(m, p, self, flags);
    if ((flags & 1u) != 0) {
        wword(m, p, self, x.armor_class_004ed1d0(c));
        wdword(m, p, self, x.gold_004edd60(c));
        wdword(m, p, self, m.read(c + 0xa44));
        wbool(m, p, self, can_level_up_005a6810(m, p, x, m.read(c + 0xa74)));
        wdword(m, p, self, m.read(m.read(c + 0xa74) + 0x68));
        if (can_level_up_005a6810(m, p, x, m.read(c + 0xa74)) == 1) {
            wword(m, p, self, m.read16(m.read(c + 0xa74) + 0x164));
            Word i = 0;
            put8(p, F + 0x4c, 0);
            while ((i & 0xffu) < m.read8(p.rules_007a3a28() + 0xab)) {
                wbyte(m, p, self, x.skill_rank_005aa570(m.read(c + 0xa74), m.read(F + 0x4c), 0, 1));
                i = (i + 1) & 0xffu; put8(p, F + 0x4c, i);
            }
            put8(p, F + 0x4c, m.read8(m.read(c + 0xa74) + 0x88));
            wbyte(m, p, self, m.read(F + 0x4c));
            wbyte(m, p, self, (m.read8(m.read(c + 0xa74) + 0x89) - 1) & 0xffu);
        }
    }
    if ((flags & 2u) != 0) wword(m, p, self, gui_skills_00567430(m, x, c));
    if ((flags & 8u) != 0) wword(m, p, self, player_state_00567600(m, x, c));
    if ((flags & 0x80u) != 0) {                                   // 00572F6C: stale-array class block
        const Word st = m.read(c + 0xa74);
        m.write(F + 0x14, 0); m.write16(F + 0x4c, 0); put8(p, F + 0x1c, 0);
        Word count = 0;
        const Word n = m.read8(st + 0x89);
        for (Word i = 0; i < n; ++i)
            if (m.read16(F + 0x14 + 2 * i) != 0) { put8(p, F + 0x4c + i, 1); count = (count + 1) & 0xffu; }
        if (n != 0) put8(p, F + 0x1c, count);
        wbyte(m, p, self, m.read(F + 0x1c));
        for (Word i = 0; i < m.read8(m.read(c + 0xa74) + 0x89); ++i)
            if (m.read8(F + 0x4c + i) != 0) { wbyte(m, p, self, i); wword(m, p, self, m.read16(F + 0x14 + 2 * i)); }
    }
    if ((flags & 0x10u) != 0) {                                   // 00573012: two id-list pairs at plast+17C
        for (Word k = 0; k < 2; ++k) {
            const Word e = plast + 0x180 + 0x0c * k;
            // the loop counter is a BYTE compared (zero-extended) with the signed dword count: a count above
            // 0FFh never terminates in the original
            if (static_cast<I>(m.read(e)) > 0xff || static_cast<I>(m.read(e + 0x18)) > 0xff)
                x.trap(0x0057305f, "W2: original PlayerUpdate 0x10 list loop never terminates (count > 255)");
            wword(m, p, self, m.read(e));
            for (Word b = 0; static_cast<I>(b) < static_cast<I>(m.read(e)); ++b) wdword(m, p, self, m.read(m.read(e - 4) + 4 * b));
            wword(m, p, self, m.read(e + 0x18));
            for (Word b = 0; static_cast<I>(b) < static_cast<I>(m.read(e + 0x18)); ++b) wdword(m, p, self, m.read(m.read(e + 0x14) + 4 * b));
        }
    }
    if ((flags & 0x20u) != 0) for (Word k = 0; k < 2; ++k) { wword(m, p, self, 0); wword(m, p, self, 0); }
    if ((flags & 0x40u) != 0) {                                   // 005730D1: Jedi spell uses (stale arrays)
        m.write(F + 0x14, 0); m.write16(F + 0x4c, 0); put8(p, F + 0x1c, 0);
        for (Word i = 0; i < m.read8(m.read(c + 0xa74) + 0x89); ++i) {
            const Word st = m.read(c + 0xa74);
            if (x.is_jedi_005be4a0(x.get_class_005a4e90(st, i) & 0xffu) == 0) continue;
            const Word u = known_spell_uses_005ca070(m, plast, i, 0) & 0xffu;
            const Word v = spells_per_day_left_005a5650(m, m.read(c + 0xa74), i, 0) & 0xffu;
            if (u != v) put8(p, F + 0x14 + 2 * i, m.read8(F + 0x14 + 2 * i) | 1u);
            if (m.read16(F + 0x14 + 2 * i) != 0) { put8(p, F + 0x1c, m.read8(F + 0x1c) + 1); put8(p, F + 0x4c + i, 1); }
        }
        wbyte(m, p, self, m.read(F + 0x1c));
        for (Word i = 0; i < m.read8(m.read(c + 0xa74) + 0x89); ++i) {
            if (m.read8(F + 0x4c + i) == 0) continue;
            wbyte(m, p, self, i);
            const Word w = m.read16(F + 0x14 + 2 * i);
            wword(m, p, self, w);
            if ((w & 1u) != 0) wbyte(m, p, self, spells_per_day_left_005a5650(m, m.read(c + 0xa74), i, 0));
        }
        for (Word i = 0; i < m.read8(m.read(c + 0xa74) + 0x89); ++i) {
            if (x.is_jedi_005be4a0(x.get_class_005a4e90(m.read(c + 0xa74), i) & 0xffu) == 0) continue;
            put8(p, F + 0x4c, spells_per_day_left_005a5650(m, m.read(c + 0xa74), i, 0));
            set_known_spell_uses_005ca0a0(m, plast, i, 0, m.read(F + 0x4c));
        }
    }
    if ((flags & 4u) != 0) {                                      // 00573239: feats 'A'/'D', bonus feats 'B'/'X'
        const Word B = F + 0x20, W = F + 0x2c;
        zero_lists(m, F);
        const Word st = m.read(c + 0xa74);
        for (Word i = 0; i < m.read16(m.read(c + 0xa74) + 4); i = (i + 1) & 0xffffu) {
            const Word f = x.get_feat_005a6920(m.read(c + 0xa74), i) & 0xffffu;
            m.write(F + 0x14, f);
            bool found = false;
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(plast + 0x1c)); ++k) if (m.read16(m.read(plast + 0x18) + 2 * k) == f) found = true;
            if (found) continue;
            list_add(m, p, B, 0x41, 1); list_add(m, p, W, m.read16(F + 0x14), 2);
        }
        (void)st;
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(plast + 0x1c)); ++j) {
            const Word f = m.read16(m.read(plast + 0x18) + 2 * j);
            m.write16(F + 0x14, f);
            bool found = false;
            for (Word i = 0; i < m.read16(m.read(c + 0xa74) + 4); i = (i + 1) & 0xffffu)
                if ((x.get_feat_005a6920(m.read(c + 0xa74), i) & 0xffffu) == f) found = true;
            if (found) continue;
            list_add(m, p, B, 0x44, 1); list_add(m, p, W, m.read16(F + 0x14), 2);
        }
        for (Word i = 0; i < m.read16(m.read(c + 0xa74) + 0x1c); i = (i + 1) & 0xffffu) {
            const Word f = x.bonus_feat_005a6940(m.read(c + 0xa74), i) & 0xffffu;
            m.write(F + 0x14, f);
            bool found = false;
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(plast + 0x28)); ++k) if (m.read16(m.read(plast + 0x24) + 2 * k) == f) found = true;
            if (found) continue;
            list_add(m, p, B, 0x42, 1); list_add(m, p, W, m.read16(F + 0x14), 2);
        }
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(plast + 0x28)); ++j) {
            const Word f = m.read16(m.read(plast + 0x24) + 2 * j);
            bool found = false;
            for (Word i = 0; i < m.read16(m.read(c + 0xa74) + 0x1c); i = (i + 1) & 0xffffu)
                if ((x.bonus_feat_005a6940(m.read(c + 0xa74), i) & 0xffffu) == f) found = true;
            if (found) continue;
            list_add(m, p, B, 0x58, 1); list_add(m, p, W, f, 2);
        }
        const Word n = m.read(B + 4);
        wword(m, p, self, n);
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(n); ++i) {
            wbyte(m, p, self, m.read8(m.read(B) + i));
            wword(m, p, self, m.read16(m.read(W) + 2 * i));
        }
        const Word st2 = m.read(c + 0xa74);
        wword(m, p, self, m.read(st2 + 0x10));
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(st2 + 0x10)); ++i) {
            wword(m, p, self, m.read16(m.read(m.read(st2 + 0x0c) + 4 * i)));
            wbyte(m, p, self, x.feat_remaining_uses_005a6680(m.read(c + 0xa74), m.read16(m.read(m.read(st2 + 0x0c) + 4 * i))));
        }
        p.free(m.read(B));
        p.free(m.read(W));
    }
    if ((flags & 0x100u) != 0) {                                  // 00573872: 10 x 1Ch slots, plast+40 vs creature+3A8
        const Word B = F + 0x20, C = F + 0x2c;
        zero_lists(m, F);
        for (Word i = 0; i < 10; ++i) {
            const Word e = plast + 0x40 + 0x1c * i;
            const Word a = m.read16(e);
            if (a == 0xffffu) continue;
            const Word b = m.read16(e + 2);
            bool found = false;
            for (Word k = 0; k < 10 && !found; ++k) {
                const Word q = c + 0x3a8 + 0x1c * k;
                if (m.read16(q + 2) == b && m.read16(q) == a && m.read16(q + 4) == m.read16(e + 4)) found = true;
            }
            if (found) continue;
            list_add(m, p, B, 0x44, 1); list_add(m, p, C, i, 1);
        }
        for (Word i = 0; i < 10; ++i) {
            const Word e = c + 0x3a8 + 0x1c * i;
            const Word a = m.read16(e);
            if (a == 0xffffu) continue;
            const Word d = m.read16(e + 2);
            bool found = false;
            for (Word k = 0; k < 10 && !found; ++k) {
                const Word q = plast + 0x40 + 0x1c * k;
                if (m.read16(q + 2) == d && m.read16(q) == a && m.read16(q + 4) == m.read16(e + 4)) found = true;
            }
            if (found) continue;
            const Word op = (d == 0xffffu && m.read16(e + 4) != 0xffffu) ? 0x53u : 0x41u;   // 00573A47
            list_add(m, p, B, op, 1); list_add(m, p, C, i, 1);
        }
        const Word n = m.read(B + 4);
        wbyte(m, p, self, n);
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(n); ++i) {
            const Word op = m.read8(m.read(B) + i), idx = m.read8(m.read(C) + i);
            wbyte(m, p, self, op);
            if (op == 0x44) {
                const Word e = plast + 0x40 + idx * 0x1c;
                if (m.read16(e + 2) == 0xffffu) { wword(m, p, self, m.read16(e + 4)); wword(m, p, self, m.read16(e)); }
                else { wword(m, p, self, m.read16(e + 2)); wword(m, p, self, m.read16(e)); }
            } else if (op == 0x53) {
                const Word e = c + 0x3a8 + idx * 0x1c;
                wword(m, p, self, m.read16(e + 4)); wword(m, p, self, m.read16(e)); wid(m, p, self, m.read(e + 0x18));
            } else if (op == 0x41) {
                const Word e = c + 0x3a8 + idx * 0x1c;
                wword(m, p, self, m.read16(e + 2)); wword(m, p, self, m.read16(e));
                const Word t = m.read16(e + 2);
                if (t != 0 && t != 2) wid(m, p, self, m.read(e + 0x18));
                if (t == 0 || t == 4 || t == 0x13 || t == 2 || t == 0x0c) {
                    wfloat(m, p, self, m.read(e + 0x0c)); wfloat(m, p, self, m.read(e + 0x10)); wfloat(m, p, self, m.read(e + 0x14));
                }
                if (t == 4 || t == 0x13) wdword(m, p, self, m.read(e + 8));
            }
        }
        p.free(m.read(C));
        p.free(m.read(B));
    }
    if ((flags & 0x2000u) != 0) {                                 // 00573DE0: perception list diff
        const Word D = F + 0x20, E = F + 0x2c;
        zero_lists(m, F);
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(plast + 0x15c)); ++j) {
            const Word pe = m.read(m.read(plast + 0x158) + 4 * j);
            bool found = false;
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(c + 0x924)) && !found; ++k)
                if (m.read(pe) == m.read(m.read(m.read(c + 0x920) + 4 * k))) found = true;
            if (found) continue;
            list_add(m, p, D, pe, 4);
        }
        wdword(m, p, self, m.read(D + 4));
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(D + 4)); ++i) wid(m, p, self, m.read(m.read(m.read(D) + 4 * i)));
        for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(c + 0x924)); ++k) {
            const Word q = m.read(m.read(c + 0x920) + 4 * k);
            bool same = false;
            for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(plast + 0x15c)) && !same; ++j) {
                const Word pe = m.read(m.read(plast + 0x158) + 4 * j);
                if (m.read(q) == m.read(pe) && ((m.read8(q + 4) ^ m.read8(pe + 4)) & 0x13u) == 0) same = true;
            }
            if (same) continue;
            list_add(m, p, E, q, 4);
        }
        wdword(m, p, self, m.read(E + 4));
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(E + 4)); ++i) {
            const Word q = m.read(m.read(E) + 4 * i);
            wid(m, p, self, m.read(q));
            const Word v = m.read8(q + 4);
            wbool(m, p, self, (v >> 1) & 1u); wbool(m, p, self, v & 1u); wbool(m, p, self, (v >> 4) & 1u);
        }
        p.free(m.read(D));
        p.free(m.read(E));
    }
    if ((flags & 0x800u) != 0) {
        if (last != 0 && m.read(c + 0xa74) != 0) {
            wbool(m, p, self, 1);
            for (Word off : {0xe9u, 0xebu, 0xedu, 0xefu, 0xf1u, 0xf3u}) wbyte(m, p, self, m.read8(m.read(c + 0xa74) + off));
            wbyte(m, p, self, x.str_stat_005a6190(m.read(c + 0xa74)));
            wbyte(m, p, self, x.dex_stat_005a6550(m.read(c + 0xa74)));
            wbyte(m, p, self, x.con_stat_005a6250(m.read(c + 0xa74)));
            wbyte(m, p, self, x.int_stat_005a6310(m.read(c + 0xa74)));
            wbyte(m, p, self, x.wis_stat_005a63d0(m.read(c + 0xa74)));
            wbyte(m, p, self, x.cha_stat_005a6490(m.read(c + 0xa74)));
            wbyte(m, p, self, x.will_save_005ab880(m.read(c + 0xa74), 0));
            wbyte(m, p, self, x.reflex_save_005ab8f0(m.read(c + 0xa74), 0));
            wbyte(m, p, self, x.fort_save_005ab810(m.read(c + 0xa74), 0));
        } else {
            wbool(m, p, self, 0);
        }
    }
    if ((flags & 0x8000u) != 0 && last != 0 && m.read(c + 0xa74) != 0) {
        for (Word i = 0; i < m.read8(p.rules_007a3a28() + 0xab); ++i) {
            wbyte(m, p, self, x.skill_rank_005aa570(m.read(c + 0xa74), i, 0, 1));
            wbyte(m, p, self, x.skill_rank_005aa570(m.read(c + 0xa74), i, 0, 0));
        }
    }
    if ((flags & 0x10000u) != 0) {
        wword(m, p, self, p.vcall0(c, 0xd0));
        if ((p.vcall0(c, 0xd0) & 0xffffu) >= 0xfffe) {
            const Word ref = p.vcall1(c, 0xc8, F + 0x2c);          // hidden CResRef result at [esp+2Ch]
            const Word text = x.resref_str_00405fe0(ref);
            const Word copy = F + 0x60;                           // by-value CResRef argument built from the text
            p.ref_assign_text_00406290(copy, F + 0x70, text);
            write_resref_004d6380(m, p, self, copy, 0x10);
        }
    }
    if ((flags & 0x200u) != 0) {
        wword(m, p, self, m.read16(c + 0xe0));
        wword(m, p, self, p.vcall1(c, 0x9c, 0));
        wword(m, p, self, m.read16(c + 0xe4));
        wword(m, p, self, p.vcall1(c, 0x98, 1));
    }
    if ((flags & 0x20000u) != 0) {
        const Word st = m.read(c + 0xa74);
        wword(m, p, self, m.read16(st + 0x122));
        wword(m, p, self, (m.read16(m.read(c + 0xa74) + 0x126) + m.read16(m.read(c + 0xa74) + 0x124)) & 0xffffu);
        wword(m, p, self, m.read16(m.read(c + 0xa74) + 0x126));
        wword(m, p, self, x.max_force_points_004fd490(c));
    }
    if ((flags & 0x4000u) != 0) spell_like_ability_0056c4d0(m, p, x, self, c, plast);
    if (plast != 0) p.clear_spell_add_delete_005cac30(plast);
}
}
