// W2 tier 4 G07 object updates. See w2_g07_object.hpp. Comments give original instruction addresses.
#include "aa81aaf111-w2_g07_object.hpp"
#include "74be47154e-module_message.hpp"
#include <cmath>
#include "28478d5d50-w2_ftol.hpp"
#include <cstring>
namespace w2::g07 {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
using I = std::int32_t;
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
Word bits(float v) { Word b; std::memcpy(&b, &v, 4); return b; }
double fd(Memory& m, Word a) { return static_cast<double>(f32(m.read(a))); }
I s16(Word v) { return static_cast<I>(static_cast<std::int16_t>(v & 0xffffu)); }
Word ftol2_low(double v) { return w2::ftol2_006fae8c(v); }   // __ftol2 006FAE8C (existing provider natively)
constexpr Word kInvalid = 0x7f000000u, kOne = 0x3f800000u;
Word srv(Memory& m, w2::entry::Ports& p) { return m.read(p.app_007a39fc() + 8); }
Word type_of(Memory& m, Word object) { return m.read8(object + 8); }
// type bytes (.rdata 00746634..0074663E): 5 creature, 6 item, 7 trigger, 9 placeable, 0Ah door, 0Bh AOE, 0Ch, 0Eh
void wbyte(Memory& m, w2::entry::Ports& p, Word self, Word v, Word bits_ = 8) { write_byte_004d62c0(m, p, self, v, bits_); }
void wword(Memory& m, w2::entry::Ports& p, Word self, Word v, Word bits_ = 0x10) { write_word16_004d6300(m, p, self, v, bits_); }
void wdword(Memory& m, w2::entry::Ports& p, Word self, Word v, Word bits_ = 0x20) { sm::message_word_004d6340(m, p, self, v, bits_); }
void wbool(Memory& m, w2::entry::Ports& p, Word self, Word v) { write_bool_004d68e0(m, p, self, v); }
void wfloat(Memory& m, w2::entry::Ports& p, Word self, Word v) { write_float_bits_004d6920(m, p, self, v, kOne, 0x20); }
void wid(Memory& m, w2::entry::Ports& p, Word self, Word id) { write_object_id_00578570(m, p, self, id); }
}

// ===================================================================== small unowned callees
Word is_beam_type_004ca960(Word type) {
    switch (type & 0xffffu) {
    case 0x7f5: case 0x7f6: case 0x1770: case 0x7ea: case 0x7eb: case 0x7ec: case 0x801: case 0x802: case 0x803:
    case 0x804: case 0x805: case 0xfc5: case 0x7ed: case 0x80d: case 0x811: case 0x812: return 1;
    default: return 0;
    }
}
Word is_beam_004ca9d0(Memory& m, Word effect) { return is_beam_type_004ca960(m.read16(effect)); }

void add_unique_large_004e3b70(Memory& m, ObjectPorts& x, Word list, Word lo, Word hi) {
    const Word count = m.read(list + 4);
    if (static_cast<I>(count) > 0) {
        Word hits = 0;
        for (Word k = 0; k < count; ++k)
            if (m.read(m.read(list) + 8 * k) == lo && m.read(m.read(list) + 8 * k + 4) == hi) ++hits;
        if (hits != 0) return;
    }
    if (count == m.read(list + 8)) x.list_allocate8_004c4f50(list, m.read(list + 8) != 0 ? m.read(list + 8) * 2 : 0x10);
    const Word at = m.read(list) + 8 * m.read(list + 4);
    m.write(at, lo); m.write(at + 4, hi);
    m.write(list + 4, m.read(list + 4) + 1);
}

Word class_level_005a5090(Memory& m, Word stats, Word index, Word flag) {
    if ((index & 0xffu) >= m.read8(stats + 0x89)) return 0;
    const Word e = stats + (index & 0xffu) * 40;
    const I diff = static_cast<I>(m.read8(e + 0xa8)) - (flag != 0 ? static_cast<I>(m.read8(e + 0xa9)) : 0);
    return diff < 0 ? 0u : static_cast<Word>(diff);              // callers use the low byte only (005A50C7 SETS/DEC/AND)
}

Word total_damage_004d2180(Memory& m, Word attack) {
    Word sum = 0;
    for (Word block = 0; block < 2; ++block)
        for (Word k = 0; k < 7; ++k) {
            const I v = s16(m.read16(attack + 0x1c + block * 0x0e + 2 * k));
            if (v > -1) sum += static_cast<Word>(v);
        }
    return sum;
}

Word damage_level_004cb020(Memory& m, w2::entry::Ports& p, Word object) {
    (void)m;
    double ratio;
    const Word c = p.vcall0(object, 0x30);
    if (c != 0) {
        const Word a = p.vcall0(object, 0x30), b = p.vcall0(object, 0x30);
        const float hp = static_cast<float>(s16(p.vcall1(a, 0x9c, 0)));          // FILD / FSTP m32
        const I max = s16(p.vcall1(b, 0x98, 1));
        ratio = static_cast<double>(hp) / static_cast<double>(max);
    } else {
        const float hp = static_cast<float>(s16(p.vcall1(object, 0x9c, 0)));
        const I max = s16(p.vcall1(object, 0x98, 1));
        ratio = static_cast<double>(hp) / static_cast<double>(max);
    }
    if (ratio >= static_cast<double>(0.95f)) return 0;           // [00745E50]
    if (ratio >= 0.75) return 1;
    if (ratio >= 0.5) return 2;
    if (ratio >= 0.25) return 3;
    return (ratio > 0.0 || std::isnan(ratio)) ? 4u : 5u;         // 004CB10F: unordered -> 4
}

Word singleton_party_005bf690(Memory& m, w2::entry::Ports& p, Word faction) {
    Word flagged = 0;
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(faction + 4)); ++i) {
        const Word c = p.creature_by_id_004ae770(srv(m, p), m.read(m.read(faction) + 4 * i));
        if (c != 0 && m.read(c + 0x9d4) != 0) ++flagged;
        if (static_cast<I>(flagged) > 1) return 0;
    }
    return 1;
}

Word leader_005bf5c0(Memory& m, w2::entry::Ports& p, NpcFactionPort& nf, Word faction) {
    Word leader = kInvalid;
    const Word internal = m.read(srv(m, p) + 4);
    if (internal == 0 || m.read(internal + 0x10054) == 0) return leader;
    if (nf.is_npc_faction_0052b280(m.read(internal + 0x10054), m.read(faction + 0x0c)) != 0) {   // 005BF5EA call 0052B280 (W1)
        float best = 0.0f;                                        // [esp+0C] = 0
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(faction + 4)); ++i) {
            const Word c = p.creature_by_id_004ae770(srv(m, p), m.read(m.read(faction) + 4 * i));
            if (c == 0) continue;
            const float v = f32(m.read(m.read(c + 0xa74) + 0x84));
            if (v > best) { best = v; leader = m.read(m.read(faction) + 4 * i); }   // 005BF62D: only strictly greater
        }
        return leader;
    }
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(faction + 4)); ++i) {
        const Word c = p.creature_by_id_004ae770(srv(m, p), m.read(m.read(faction) + 4 * i));
        if (c != 0 && m.read(c + 0x9d4) != 0) return m.read(c + 4);
    }
    return leader;
}

// ===================================================================== 0056DC60
Word test_object_visible_0056dc60(Memory& m, w2::entry::Ports& p, ObjectPorts& x, Word, Word object, Word observer) {
    const double ox = fd(m, object + 0x90), oy = fd(m, object + 0x94);
    double vx = fd(m, observer + 0x90), vy = fd(m, observer + 0x94);
    bool has_area = true;                                         // EBP
    if (p.object_area_004cb120(observer) == 0 && p.vcall0(observer, 0x30) != 0) {
        has_area = false;
        const Word c = p.vcall0(observer, 0x30);
        vx = fd(m, c + 0x314); vy = fd(m, c + 0x318);
    }
    const double dx = ox - vx, dy = oy - vy;
    const float distance2 = static_cast<float>(dy * dy + dx * dx);      // 0056DD08: FSTP m32
    float range = 250.0f;                                         // 0x437A0000
    if (p.vcall0(observer, 0x30) != 0) {
        const float spot = f32(x.spot_bits_004efc70(p.vcall0(observer, 0x30)));
        const float listen = f32(x.listen_bits_004efd20(p.vcall0(observer, 0x30)));
        range = (listen <= spot) ? f32(x.spot_bits_004efc70(p.vcall0(observer, 0x30)))
                                 : f32(x.listen_bits_004efd20(p.vcall0(observer, 0x30)));   // 0056DD42: greater or unordered -> listen
    }
    const Word type = type_of(m, object);
    if (type == 0x0c) return 0;
    if (p.vcall0(object, 0x5c) != 0) return 0;
    if (type == 0x0e) return 0;
    if (p.vcall0(object, 0x68) != 0) return 0;
    const bool out_of_range = !(static_cast<double>(range) * range >= static_cast<double>(distance2));   // C0 (less or unordered)
    if (type == 5) {
        (void)p.object_area_004cb120(object);                    // 0056DDAF: result unused
        const Word oc = p.vcall0(observer, 0x30);
        if (oc == 0) return 0;
        if (m.read(object + 4) == m.read(observer + 4)) return 1;
        if (m.read(p.vcall0(object, 0x30) + 0xa88) != 0) return 1;
        if (has_area) {
            const Word e = x.visible_element_00517a20(p.vcall0(observer, 0x30), m.read(object + 4));
            return (e != 0 && (m.read8(e + 4) & 3u) != 0) ? 1u : 0u;
        }
        if (out_of_range) return 0;
        const Word obs_c = p.vcall0(observer, 0x30);
        return x.invisible_00501950(p.vcall0(object, 0x30), obs_c) != 0 ? 0u : 1u;
    }
    if (out_of_range) return 0;
    if (type != 7) return 1;
    // trigger (0056DE88): every path below returns 1; the calls are kept for fidelity
    (void)x.client_object_by_id_004ae910(srv(m, p), m.read(observer + 4));
    const Word trig = p.vcall0(object, 0x40);
    if (m.read(trig + 0x2bc) != 0) {
        if (m.read(p.vcall0(object, 0x40) + 0x2c8) != 0) return 1;
        const Word faction = m.read(p.vcall0(object, 0x40) + 0x2b8);
        const Word oc = p.vcall0(observer, 0x30);
        if (oc == 0) x.trap(0x0056ded5, "W2: original reads stats of a null observer creature (TestObjectVisible trap trigger)");
        if (faction == m.read(m.read(oc + 0xa74) + 0x78)) return 1;
        if (static_cast<I>(x.reputation_0057cb80(object, m.read(observer + 4), 0)) >= 0x5a) return 1;
        (void)x.array_count_004af240(p.vcall0(object, 0x40) + 0x2a8, m.read(observer + 4));
        return 1;                                                 // 0056DF0A: both outcomes return EBX = 1
    }
    return 1;                                                     // 0056DF1D..: +2B4 / +2FC tests, both outcomes 1
}

// ===================================================================== 0056E190
void delete_for_object_0056e190(Memory& m, w2::entry::Ports& p, ObjectPorts& x, Word self, Word player, Word id, Word s) {
    const Word pgo = p.game_object_00561080(player);
    const Word array = m.read(m.read(srv(m, p) + 4) + 0x1005c);   // 004AED70
    if (array == 0) return;
    const Word list = m.read(m.read(player + 0x0c));
    const Word pos_slot = s;
    m.write(pos_slot, m.read(list));                              // head position
    if (m.read(pos_slot) == 0) return;
    for (;;) {
        const Word entry = x.list_at_pos_005e9b60(m.read(m.read(player + 0x0c)), m.read(pos_slot));
        const Word eid = m.read(entry + 0x2c);
        if (eid == id && eid != m.read(pgo + 4)) {
            wbyte(m, p, self, 0x44);
            const Word type = m.read8(entry + 0x28);
            wbyte(m, p, self, type);
            wid(m, p, self, m.read(entry + 0x2c));
            if (type == 6 || type == 5 || type == 9) {
                Word different = 1;
                const Word out = s + 4;
                m.write(out, 0);
                if ((x.object_lookup_004d8230(array, m.read(entry + 0x2c), out) & 0xffu) == 0 && p.vcall0(m.read(out), 0x10) != 0) {
                    const Word a = p.object_area_004cb120(p.vcall0(m.read(out), 0x10));
                    different = (a == p.object_area_004cb120(p.game_object_00561080(player))) ? 0u : 1u;
                }
                wbool(m, p, self, different);
                if (type == 5 || type == 9) {
                    wbool(m, p, self, m.read(entry + 0x148));
                    wdword(m, p, self, m.read(entry + 0x150));
                    if (type == 5) wbool(m, p, self, m.read(entry + 0x14c));
                }
            } else if (type == 7) {
                wbyte(m, p, self, 3);
            }
            const Word at = m.read(pos_slot);
            x.list_next_005e9b90(m.read(m.read(player + 0x0c)), pos_slot);
            x.list_remove_005e9c10(m.read(m.read(player + 0x0c)), at);
            x.last_update_destroy_005ca6a0(entry);
            x.free_006fa390(entry);
            return;
        }
        m.write(entry + 0x24, 0);                                 // 0056E1F5
        x.list_next_005e9b90(m.read(m.read(player + 0x0c)), pos_slot);
        if (m.read(pos_slot) == 0) return;
    }
}

// ===================================================================== 00577FA0 / 00576FC0
void test_update_differences_00577fa0(Memory& m, w2::entry::Ports& p, ObjectPorts& x, Word self, Word player, Word object, Word last_out, Word flags_out, Word aflags_out, Word s) {
    const Word pgo = p.game_object_00561080(player);
    m.write(s, m.read(m.read(m.read(player + 0x0c))));
    while (m.read(s) != 0) {
        const Word e = x.list_at_pos_005e9b60(m.read(m.read(player + 0x0c)), m.read(s));
        m.write(last_out, e);                                     // 00577FDB: written for every visited entry
        if (m.read(e + 0x2c) == m.read(object + 4)) {
            m.write(e + 0x24, 1);
            const Word same = m.read(object + 4) == m.read(pgo + 4) ? 1u : 0u;
            m.write(flags_out, x.compute_update_0056b950(self, player, object, m.read(last_out), same));
            m.write(aflags_out, x.compute_appearance_00567100(self, object, m.read(last_out)));
            return;
        }
        x.list_next_005e9b90(m.read(m.read(player + 0x0c)), s);
    }
    m.write(last_out, x.create_new_last_00577000(self, player, object, flags_out, aflags_out));
}
void store_values_00576fc0(ObjectPorts& x, Word self, Word player, Word last, Word object, Word flags, Word aflags) {
    x.update_last_00575c80(self, player, object, last, flags);
    x.update_last_appearance_00568480(object, last, aflags);
}

// ===================================================================== 00574A10
namespace {
bool beam_match(Memory& m, Word a, Word b) {                     // type, byte +8 and dword +4 equal
    return m.read16(a) == m.read16(b) && m.read8(a + 8) == m.read8(b + 8) && m.read(a + 4) == m.read(b + 4);
}
void effect_section(Memory& m, w2::entry::Ports& p, ObjectPorts& x, Word self, Word object, Word last, Word s) {
    const Word A = s, W = s + 0x0c;                               // CExoArrayList<LARGE_INTEGER>, CExoArrayList<WORD>
    for (Word k = 0; k < 0x18; k += 4) m.write(s + k, 0);
    const Word obj_count = m.read(object + 0x134), last_count = m.read(last + 0xac);
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(obj_count); ++i) {
        const Word e = m.read(m.read(object + 0x130) + 4 * i);
        const Word type = m.read16(e);
        bool skip = false;
        if (is_beam_004ca9d0(m, e) != 0) {
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(A + 4)) && !skip; ++k)
                if (beam_match(m, m.read(m.read(A) + 8 * k), e)) skip = true;
        } else {
            Word hits = 0;
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(W + 4)); ++k) if (m.read16(m.read(W) + 2 * k) == type) ++hits;
            if (static_cast<I>(hits) > 0) skip = true;
        }
        if (skip) continue;
        bool found = false;
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(last_count) && !found; ++j) {
            const Word q = m.read(m.read(last + 0xa8) + 4 * j);
            if (is_beam_004ca9d0(m, e) != 0) { if (beam_match(m, q, e)) found = true; }
            else if (m.read16(q) == type) found = true;
        }
        if (found) continue;
        add_unique_large_004e3b70(m, x, A, e, 0x41);
        if (m.read(W + 4) == m.read(W + 8)) {                    // 00575038: inline WORD-list growth
            const Word cap = m.read(W + 8) != 0 ? m.read(W + 8) * 2 : 0x10;
            const Word old = m.read(W);
            m.write(W + 8, cap);
            const Word fresh = x.allocate_006fa7e6(cap * 2);
            m.write(W, fresh);
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(W + 4)); ++k) m.write16(fresh + 2 * k, m.read16(old + 2 * k));
            if (old != 0) x.free_006fa390(old);
        }
        m.write16(m.read(W) + 2 * m.read(W + 4), type);
        m.write(W + 4, m.read(W + 4) + 1);
    }
    for (Word j = 0; static_cast<I>(j) < static_cast<I>(last_count); ++j) {
        const Word q = m.read(m.read(last + 0xa8) + 4 * j);
        const Word type = m.read16(q);
        bool found = false;
        for (Word k = 0; static_cast<I>(k) < static_cast<I>(obj_count) && !found; ++k) {
            const Word e = m.read(m.read(object + 0x130) + 4 * k);
            if (is_beam_004ca9d0(m, q) != 0) { if (beam_match(m, e, q)) found = true; }
            else if (m.read16(e) == type) found = true;
        }
        if (!found) add_unique_large_004e3b70(m, x, A, q, 0x44);
    }
    wword(m, p, self, m.read(A + 4));
    for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(A + 4)); ++k) {
        const Word q = m.read(m.read(A) + 8 * k);
        wbyte(m, p, self, m.read8(m.read(A) + 8 * k + 4));
        wword(m, p, self, m.read16(q));
        if (is_beam_004ca9d0(m, q) != 0) { wid(m, p, self, m.read(q + 4)); wbyte(m, p, self, m.read8(q + 8)); }
    }
    x.free_006fa390(m.read(W));
    x.free_006fa390(m.read(A));
}
// 0x10 group for doors / placeables (identical shapes, different fields)
struct Fields { Word faction, plot_byte, list, list_count, flag60, flag_ebp; };
void use_object_bools(Memory& m, w2::entry::Ports& p, ObjectPorts& x, Word self, Word player, Word object, Word slot, const Fields& f) {
    Word hostile = 0, b60 = 0;
    const Word d = p.vcall0(object, slot);
    (void)d;
    if (static_cast<I>(x.reputation_0057cb80(object, m.read(p.game_object_00561080(player) + 4), 0)) < 0x5a) {
        const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
        const Word faction = m.read(p.vcall0(object, slot) + f.faction);
        if (pc == 0) x.trap(0x005752e9, "W2: original reads stats of a null player creature ('U' 0x10 group)");
        if (faction != m.read(m.read(pc + 0xa74) + 0x78)) hostile = 1;
    }
    bool check60 = true;
    if (m.read8(p.vcall0(object, slot) + f.plot_byte) == 0
        && static_cast<I>(x.reputation_0057cb80(object, m.read(p.game_object_00561080(player) + 4), 0)) < 0x5a) {
        const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
        const Word faction = m.read(p.vcall0(object, slot) + f.faction);
        if (pc == 0) x.trap(0x00575350, "W2: original reads stats of a null player creature ('U' 0x10 group)");
        if (faction != m.read(m.read(pc + 0xa74) + 0x78)) {
            const Word pcid = m.read(p.vcall0(p.game_object_00561080(player), 0x30) + 4);
            const Word o = p.vcall0(object, slot);
            Word hits = 0;
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(o + f.list_count)); ++k) if (m.read(m.read(o + f.list) + 4 * k) == pcid) ++hits;
            if (hits == 0) check60 = false;
        }
    }
    if (check60 && m.read(p.vcall0(object, slot) + f.flag60) != 0) b60 = 1;
    const Word ebp = m.read(p.vcall0(object, slot) + f.flag_ebp) == 1 ? 1u : 0u;
    const Word edi = m.read(p.vcall0(object, slot) + 0xf8) == 1 ? 1u : 0u;
    wbool(m, p, self, hostile); wbool(m, p, self, b60); wbool(m, p, self, ebp); wbool(m, p, self, edi);
}
}

void update_object_00574a10(Memory& m, w2::entry::Ports& p, ObjectPorts& x, Word self, Word player, Word object, Word last, Word flags, Word aflags, Word s) {
    const Word pgo = p.game_object_00561080(player);
    if (pgo == 0) return;
    if (type_of(m, object) == 5 && last != 0 && aflags != 0) x.update_appearance_00572070(self, object, last, aflags);
    wbyte(m, p, self, 0x55);
    wbyte(m, p, self, type_of(m, object));
    wid(m, p, self, m.read(object + 4));
    wdword(m, p, self, flags);
    if ((flags & 1u) != 0) { wfloat(m, p, self, m.read(object + 0x90)); wfloat(m, p, self, m.read(object + 0x94)); wfloat(m, p, self, m.read(object + 0x98)); }
    if ((flags & 2u) != 0) {
        wfloat(m, p, self, m.read(object + 0x9c)); wfloat(m, p, self, m.read(object + 0xa0)); wfloat(m, p, self, m.read(object + 0xa4));
        if (p.vcall0(object, 0x30) != 0) {
            if (p.vcall0(p.vcall0(object, 0x30), 0xc4) != kInvalid) { wbyte(m, p, self, 1); wid(m, p, self, p.vcall0(p.vcall0(object, 0x30), 0xc4)); }
            else wbyte(m, p, self, 0);
        }
    }
    if ((flags & 0x20u) != 0) {
        const Word portrait = p.vcall0(object, 0xd0) & 0xffffu;
        wword(m, p, self, portrait);
        if (portrait >= 0xfffe) {
            const Word ref = p.vcall1(object, 0xc8, s + 0x40);    // hidden CResRef result
            write_resref_004d6380(m, p, self, ref, 0x10);
        }
    }
    const Word anim_flag = flags & 4u;
    Word anim = 0;
    if (anim_flag != 0) {
        wfloat(m, p, self, m.read(object + 0xd8));
        anim = m.read(object + 0xd4);
        wword(m, p, self, anim);
        const Word c = p.vcall0(object, 0x30);
        if (c != 0) {
            const Word a16 = anim & 0xffffu;
            if (a16 == 0x2719 || a16 == 0x277d) {
                const Word count = m.read8(m.read(c + 0x9c8) + 0x96c) + 1;
                wbyte(m, p, self, count, 2);
                for (Word i = 0; static_cast<I>(i) < static_cast<I>(count); ++i) {
                    wid(m, p, self, m.read(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x0c));
                    wword(m, p, self, m.read16(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x12));
                    wword(m, p, self, m.read16(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x14));
                    wword(m, p, self, m.read16(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 8));
                    wbyte(m, p, self, m.read8(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x5c), 4);
                    wword(m, p, self, m.read16(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x64));
                    wword(m, p, self, total_damage_004d2180(m, x.combat_attack_004d2b50(m.read(c + 0x9c8), i)), 9);
                    wbool(m, p, self, m.read(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x40));
                    wbool(m, p, self, m.read(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x48));
                    wbyte(m, p, self, m.read8(x.combat_attack_004d2b50(m.read(c + 0x9c8), i) + 0x3a), 4);
                }
            }
            if (a16 == 0x271f || a16 == 0x2720 || a16 == 0x2af8 || a16 == 0x2721 || a16 == 0x2722 || a16 == 0x2723 || a16 == 0x2724 || a16 == 0x274d || a16 == 0x28a1) {
                const Word v = m.read(c + 0x1c0);
                wdword(m, p, self, v);
                if (v == 0x31) {
                    const Word item = x.item_in_slot_005a4c20(m.read(c + 0xa2c), 0x10);
                    wid(m, p, self, item != 0 ? m.read(item + 0x14) : kInvalid);
                } else {
                    wid(m, p, self, m.read(c + 0x95c));
                }
            }
            if (a16 == 0x2721 || a16 == 0x2722 || a16 == 0x2723 || a16 == 0x2724 || a16 == 0x274d || a16 == 0x28a1) {
                if (m.read(c + 0x1bc) != kInvalid) { wbyte(m, p, self, 1); wid(m, p, self, m.read(c + 0x1bc)); }
                else { wbyte(m, p, self, 2); wfloat(m, p, self, m.read(c + 0x1b0)); wfloat(m, p, self, m.read(c + 0x1b4)); wfloat(m, p, self, m.read(c + 0x1b8)); }
                wdword(m, p, self, m.read(c + 0x1c8));
            }
        }
    }
    if ((flags & 8u) != 0) effect_section(m, p, x, self, object, last, s);
    if ((flags & 0x10u) != 0) {
        if (p.vcall0(object, 0x40) != 0) {                        // trigger
            if (m.read(p.vcall0(object, 0x40) + 0x2bc) != 0) {
                Word hostile = 0;
                if (static_cast<I>(x.reputation_0057cb80(object, m.read(p.game_object_00561080(player) + 4), 0)) < 0x5a) {
                    const Word faction = m.read(p.vcall0(object, 0x40) + 0x2b8);
                    const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
                    if (pc == 0) x.trap(0x00575280, "W2: original reads stats of a null player creature ('U' trigger)");
                    if (faction != m.read(m.read(pc + 0xa74) + 0x78)) hostile = 1;
                }
                wbool(m, p, self, hostile);
            }
        } else if (p.vcall0(object, 0x18) != 0) {                 // door
            use_object_bools(m, p, x, self, player, object, 0x18, Fields{0x2b8, 0x2f4, 0x2dc, 0x2e0, 0x2e8, 0x2c4});
        } else if (p.vcall0(object, 0x48) != 0) {                 // placeable
            use_object_bools(m, p, x, self, player, object, 0x48, Fields{0x23c, 0x28c, 0x318, 0x31c, 0x278, 0x260});
        }
    }
    if (type_of(m, object) != 5) return;                          // 00575558: creature-only tail
    if (anim_flag != 0) {
        const Word ai = m.read(object + 0x340);
        Word b = m.read8(ai + 0x28);
        Word slot_low16 = (b & 0xffu) | (last & 0xff00u);         // [esp+64] is the `last` argument slot with its low byte replaced
        if (m.read(object + 4) != m.read(pgo + 4)) b = 0;
        else if (b != 0 && m.read(object + 0xa18) == 1) { m.write(object + 0xa18, 0); b = 0; }
        slot_low16 = (b & 0xffu) | (last & 0xff00u);
        wbyte(m, p, self, b);
        if (x.animation_stationary_004cae60(object, anim) == 1) {
            wword(m, p, self, 0);
        } else {
            const Word path = m.read(object + 0x340);
            const I half = static_cast<I>(m.read(path + 0x8c)) / 2, start = static_cast<I>(m.read(path + 0x9c)) / 2;   // CDQ/SUB/SAR
            Word end16;
            Word hb = static_cast<Word>(half);
            if (static_cast<I>(hb & 0xffffu) >= start) {          // 005755FD: MOVZX vs signed start
                const Word sent = static_cast<Word>(half - start);
                wword(m, p, self, sent);
                if ((sent & 0xffffu) != 0) {
                    wbool(m, p, self, m.read(object + 0xa14));
                    if (m.read(object + 0xa14) != 0) wfloat(m, p, self, m.read(object + 0xa0c));
                }
                end16 = hb & 0xffffu;
            } else {
                wword(m, p, self, 0);
                end16 = slot_low16 & 0xffffu;                     // 00575656: stale slot (original)
            }
            if ((hb & 0xffffu) != 0) {
                const Word points = m.read(path + 0x90);
                for (I i = start; i < static_cast<I>(end16); ++i) {
                    wfloat(m, p, self, m.read(points + 8 * static_cast<Word>(i)));
                    wfloat(m, p, self, m.read(points + 8 * static_cast<Word>(i) + 4));
                    wfloat(m, p, self, 0);
                }
            }
        }
    }
    if ((flags & 0x40u) != 0) {
        const Word a4d1 = m.read8(object + 0x4d1), c9f2 = m.read8(object + 0x9f2), d4e0 = m.read(object + 0x4e0);
        const Word w9f0 = m.read16(object + 0x9f0), w9fc = m.read16(object + 0x9fc), eabc = m.read(object + 0xabc);
        Word rep = 2;
        if (player != 0 && p.game_object_00561080(player) != 0 && p.vcall0(p.game_object_00561080(player), 0x30) != 0)
            rep = x.ai_state_reputation_0057cd90(p.vcall0(p.game_object_00561080(player), 0x30), m.read(object + 4)) & 0xffu;
        wword(m, p, self, w9f0); wbyte(m, p, self, c9f2); wword(m, p, self, w9fc); wbyte(m, p, self, rep); wbyte(m, p, self, a4d1);
        wbool(m, p, self, eabc); wbool(m, p, self, d4e0);
        if (c9f2 == 2) wid(m, p, self, m.read(object + 0x9f4));
    }
    if ((flags & 0x100u) != 0) { wfloat(m, p, self, x.walk_rate_bits_004f1b20(object)); wfloat(m, p, self, x.run_rate_bits_004f1be0(object)); }
    if ((flags & 0x40000u) != 0) wid(m, p, self, m.read(object + 0x8c));
    if ((flags & 0x200u) != 0) {
        wdword(m, p, self, ftol2_low(fd(m, object + 0x90)), 0x0a);
        wdword(m, p, self, ftol2_low(fd(m, object + 0x94)), 0x0a);
    }
    if ((flags & 0x400u) != 0) {
        wword(m, p, self, p.vcall1(object, 0x9c, 1));
        wword(m, p, self, m.read16(object + 0xe0));
        wword(m, p, self, m.read16(object + 0xe4));
        wword(m, p, self, p.vcall1(object, 0x98, 1));
    }
    if ((flags & 0x80000u) != 0) {
        wword(m, p, self, m.read16(m.read(object + 0xa74) + 0x124));
        wword(m, p, self, m.read16(m.read(object + 0xa74) + 0x122));
        wword(m, p, self, m.read16(m.read(object + 0xa74) + 0x126));
        wword(m, p, self, x.max_force_points_004fd490(object));
    }
    if ((flags & 0x10000u) != 0) {
        write_locstring_005785c0(m, p, self, m.read(object + 0xa74) + 0x34, 0, s + 0x20);
        write_locstring_005785c0(m, p, self, m.read(object + 0xa74) + 0x3c, 0, s + 0x20);
    }
    if ((flags & 0x4000u) != 0) {
        wbool(m, p, self, m.read(object + 0x9d4));
        wbool(m, p, self, (m.read8(object + 0x9f0) & 0x40u) != 0 ? 1u : 0u);
        wbool(m, p, self, singleton_party_005bf690(m, p, x.faction_00513fc0(object)));
        wbool(m, p, self, leader_005bf5c0(m, p, x, x.faction_00513fc0(object)) == m.read(object + 4) ? 1u : 0u);
        wbool(m, p, self, m.read(object + 0x930));
        if (m.read(object + 0x930) != 0) {
            wid(m, p, self, m.read(object + 0x92c));
            const Word c = p.creature_by_id_004ae770(srv(m, p), m.read(object + 0x92c));
            if (c == 0) x.trap(0x005759c4, "W2: original reads the names of a null creature ('U' 0x4000 group)");
            write_locstring_005785c0(m, p, self, m.read(c + 0xa74) + 0x34, 0, s + 0x20);
            write_locstring_005785c0(m, p, self, m.read(c + 0xa74) + 0x3c, 0, s + 0x20);
        }
        wbool(m, p, self, m.read(object + 0x9e4));
        wbool(m, p, self, m.read(object + 0x9e0));
    }
    if ((flags & 0x8000u) != 0) {
        const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
        if (pc != 0) {
            if (m.read(object + 4) == m.read(pc + 4)) {
                wbool(m, p, self, 1); wbool(m, p, self, 1);
                const Word module = x.server_module_004ae6b0(srv(m, p));
                Word count = 0;
                if (module != 0)
                    for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(module + 0x140)); ++k)
                        if (m.read(m.read(module + 0x13c) + 4 * k) == m.read(object + 4)) ++count;
                wbool(m, p, self, count);                         // raw count (not normalised)
            } else {
                const Word e = x.visible_element_00517a20(object, m.read(pc + 4));
                if (e != 0) {
                    const Word v = m.read8(e + 4);
                    wbool(m, p, self, (v >> 1) & 1u); wbool(m, p, self, v & 1u); wbool(m, p, self, (v >> 4) & 1u);
                } else { wbool(m, p, self, 0); wbool(m, p, self, 0); wbool(m, p, self, 0); }
            }
        }
    }
    if ((flags & 0x20000u) != 0) {
        Word b = 0;
        if ((x.detect_mode_004edae0(object) & 0xffu) == 1) b = 1;
        if (m.read8(object + 0x4d1) == 1) b |= 2;
        switch (m.read8(object + 0x4d2)) {
        case 1: b |= 4; break; case 2: b |= 8; break; case 3: b |= 0x10; break;
        case 5: b |= 0x20; break; case 6: b |= 0x40; break; case 4: b |= 0x80; break; default: break;
        }
        wbyte(m, p, self, b);
    }
    if ((flags & 0x800u) != 0) wbyte(m, p, self, damage_level_004cb020(m, p, object));
    if ((flags & 0x1000u) != 0) {
        const Word st = m.read(object + 0xa74);
        wword(m, p, self, m.read16(st + 0xdc));
        write_string_004d6960(m, p, self, st + 0xe0, 0x20);
        wbyte(m, p, self, m.read8(m.read(object + 0xa74) + 0xe8));
        wbyte(m, p, self, m.read8(m.read(object + 0xa74) + 0x64));
        wword(m, p, self, m.read16(m.read(object + 0xa74) + 0x17e));
        wbool(m, p, self, m.read(object + 0xf4));
        wbyte(m, p, self, m.read8(m.read(object + 0xa74) + 0x89));
        for (Word i = 0; i < m.read8(m.read(object + 0xa74) + 0x89); ++i) {
            wbyte(m, p, self, x.get_class_005a4e90(m.read(object + 0xa74), i));
            wbyte(m, p, self, class_level_005a5090(m, m.read(object + 0xa74), i, 0));
        }
    }
}
}
