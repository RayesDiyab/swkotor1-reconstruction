// W2 tier 4 (G07, fork I). See w2_g07_party_update.hpp. Comments give original instruction addresses.
#include "28b3146bfb-w2_g07_party_update.hpp"
#include "aa81aaf111-w2_g07_object.hpp"
#include "74be47154e-module_message.hpp"
#include <cstring>
namespace w2::g07 {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
using I = std::int32_t;
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
double fd(Memory& m, Word a) { return static_cast<double>(f32(m.read(a))); }
I s16(Word v) { return static_cast<I>(static_cast<std::int16_t>(v & 0xffffu)); }
Word srv(Memory& m, w2::entry::Ports& p) { return m.read(p.app_007a39fc() + 8); }
Word luo_list(Memory& m, Word luo, Word which) {               // 0 -> +98h, 1 -> +90h, 2 -> +9Ch, else none
    switch (which & 0xffu) { case 0: return m.read(luo + 0x98); case 1: return m.read(luo + 0x90); case 2: return m.read(luo + 0x9c); default: return 0; }
}
bool luo_valid(Word which) { return (which & 0xffu) <= 2; }
void record(Memory& m, w2::entry::Ports& p, Word self, Word letter, Word op) {
    write_byte_004d62c0(m, p, self, 0x47, 8); write_byte_004d62c0(m, p, self, letter, 8); write_byte_004d62c0(m, p, self, op, 8);
}
Word need(PartyUpdatePorts& x, Word v, Word at, const char* what) { if (v == 0) x.trap(at, what); return v; }
}

// ===================================================================== small callees
Word item_list_object_id_005560e0(Memory& m, Word repository, Word index) {
    if (static_cast<I>(index) < 0 || static_cast<I>(index) >= static_cast<I>(m.read(repository + 0x10))) return 0x7f000000u;
    return m.read(m.read(repository + 0x0c) + 4 * index);
}
Word luo_update_item_005ca4d0(Memory& m, PartyUpdatePorts& x, Word luo, Word pos, Word which) {
    if (!luo_valid(which)) return 0;
    return x.list_at_pos_005e9b60(luo_list(m, luo, which), pos);
}
void luo_set_end_005ca520(Memory& m, Word luo, Word out, Word which) {
    if (!luo_valid(which)) return;
    m.write(out, m.read(luo_list(m, luo, which) + 4));               // list tail
}
void luo_get_prev_005ca570(Memory& m, PartyUpdatePorts& x, Word luo, Word slot, Word which) {
    if (!luo_valid(which)) return;
    x.list_prev_005e9bd0(luo_list(m, luo, which), slot);
}
void luo_remove_005ca5c0(Memory& m, PartyUpdatePorts& x, Word luo, Word pos, Word which) {
    if (!luo_valid(which)) return;
    x.free_006fa390(x.list_remove_005e9c10(luo_list(m, luo, which), pos));
}
void luo_add_head_005ca630(Memory& m, PartyUpdatePorts& x, Word luo, Word id, Word which, Word stack, Word cost) {
    const Word node = x.allocate_006fa7e6(0x0c);
    if (node == 0) x.trap(0x005ca642, "W2: original writes a null LUO inventory node (allocation failed)");
    m.write(node, id); m.write(node + 8, cost); m.write(node + 4, stack);
    if (!luo_valid(which)) { x.free_006fa390(node); return; }       // 005CA691
    x.list_add_head_005e9d70(luo_list(m, luo, which), node);
}
// 0062FEA0 GetCurrentEquipScreenNPC is W3's (t6b ruling); W2 calls W3's export through PanelPorts.

// ===================================================================== 0056E850
void test_party_differences_0056e850(Memory& m, w2::entry::Ports& p, PartyUpdatePorts& x, Word self, Word player, Word c,
                                     Word last_out, Word flags_out, Word s) {
    (void)self;
    const Word pgo = need(x, p.game_object_00561080(player), 0x0056e877, "W2: original calls through a null player game object (TestPartyObjectUpdateDifferences)");
    const Word pc = p.vcall0(pgo, 0x30);
    const Word pos = s, tmp = s + 4;
    m.write(flags_out, 0);
    m.write(pos, m.read(m.read(m.read(player + 0x10))));
    while (m.read(pos) != 0) {
        const Word e = x.list_at_pos_005e9b60(m.read(m.read(player + 0x10)), m.read(pos));
        m.write(last_out, e);
        if (m.read(e + 0x18) == m.read(c + 4)) {
            m.write(e + 0x14, 1);
            m.write(flags_out, 0);
            x.compute_ai_state_004f1460(c);
            const Word rep = pc != 0 ? (x.ai_state_reputation_0057cd90(pc, m.read(c + 4)) & 0xffu) : 2u;
            Word f = 0;
            if (m.read16(e + 0x1c) != m.read16(c + 0x9f0) || m.read8(e + 0x1e) != m.read8(c + 0x9f2) || m.read16(e + 0x20) != m.read16(c + 0x9fc)
                || m.read(e + 0x24) != m.read(c + 0x9f4) || m.read8(e + 0x30) != rep || m.read(e + 0x34) != m.read(c + 0x4e0)
                || m.read8(e + 0x38) != m.read8(c + 0x4d1))
                f |= 0x40;
            if (m.read(c + 0x8c) != m.read(e + 0x3c)) f |= 0x40200;
            if (pc == 0) x.trap(0x0056e998, "W2: original reads the area of a null player creature (TestPartyObjectUpdateDifferences)");
            if (m.read(c + 0x8c) == m.read(pc + 0x8c)) {          // 0056E9A0: |1| + |0| <= 3 always holds
                const double dx = fd(m, e + 0x40) - fd(m, c + 0x90), dy = fd(m, e + 0x44) - fd(m, c + 0x94), dz = fd(m, e + 0x48) - fd(m, c + 0x98);
                const double sum = (dz * dz + dy * dy) + dx * dx;
                if (sum >= 4.0) f |= 0x200;                            // [0073F404] = 4.0; NaN -> no flag
            }
            m.write(flags_out, m.read(flags_out) | f);
            f = 0;
            const I hp = s16(p.vcall1(c, 0x9c, 1));
            if (static_cast<I>(m.read16(e + 0x28)) != hp || static_cast<I>(m.read16(e + 0x2a)) != s16(m.read16(c + 0xe0))
                || static_cast<I>(m.read16(e + 0x2c)) != s16(m.read16(c + 0xe4)) || static_cast<I>(m.read16(e + 0x2e)) != s16(p.vcall1(c, 0x98, 1)))
                f |= 0x400;                                            // MOVZX vs MOVSX (original)
            const Word st = m.read(c + 0xa74);
            if (static_cast<I>(m.read16(e + 0x68)) != s16(m.read16(st + 0x124)) || static_cast<I>(m.read16(e + 0x6a)) != s16(m.read16(st + 0x122))
                || static_cast<I>(m.read16(e + 0x6c)) != s16(m.read16(st + 0x126)) || static_cast<I>(m.read16(e + 0x6e)) != s16(x.max_force_points_004fd490(c)))
                f |= 0x80000;
            bool g = true;
            if (m.read(p.vcall0(c, 0x30) + 0x9d4) != m.read(e + 0x4c)) g = false;
            else if (((m.read8(p.vcall0(c, 0x30) + 0x9f0) & 0x40u) != 0 ? 1u : 0u) != m.read(e + 0x50)) g = false;
            else {
                const Word id = m.read(c + 4);
                const Word leader = leader_005bf5c0(m, p, x, x.faction_00513fc0(p.vcall0(c, 0x30)));
                if ((leader == id ? 1u : 0u) != m.read(e + 0x54)) g = false;
                else if (singleton_party_005bf690(m, p, x.faction_00513fc0(c)) != m.read(e + 0x58)) g = false;
                else if (m.read(p.vcall0(c, 0x30) + 0x930) != m.read(e + 0x5c)) g = false;
                else if (m.read(c + 0x9e0) != m.read(e + 0x60) || m.read(c + 0x9e4) != m.read(e + 0x64)) g = false;
            }
            if (!g) f |= 0x4000;
            if ((p.vcall0(c, 0xd0) & 0xffffu) != m.read16(e + 0x10)) f |= 0x20;
            else if (x.resref_ne_00406dc0(e, p.vcall1(c, 0xc8, tmp)) != 0) f |= 0x20;
            m.write(flags_out, m.read(flags_out) | f);
            return;
        }
        x.list_next_005e9b90(m.read(m.read(player + 0x0c)), pos);    // 0056E8B6: walks with the +0C list (original)
    }
    const Word e = x.allocate_006fa7e6(0x70);
    if (e == 0) x.trap(0x0056eb96, "W2: original writes a null party last-update object (allocation failed)");
    x.ref_default_00405ed0(e);
    m.write(e + 0x40, 0); m.write(e + 0x44, 0); m.write(e + 0x48, 0);
    m.write(last_out, e);
    m.write(e + 0x14, 1);
    m.write(m.read(last_out) + 0x18, m.read(c + 4));
    m.write(flags_out, 0x56660);
    x.list_add_head_005e9d70(m.read(m.read(player + 0x10)), m.read(last_out));
}

// ===================================================================== 00567CB0
void store_party_values_00567cb0(Memory& m, w2::entry::Ports& p, PartyUpdatePorts& x, Word self, Word pc, Word e, Word c, Word flags, Word s) {
    (void)self;
    if ((flags & 0x40u) != 0) {
        m.write16(e + 0x1c, m.read16(c + 0x9f0));
        p.write_byte(e + 0x1e, m.read8(c + 0x9f2));
        m.write16(e + 0x20, m.read16(c + 0x9fc));
        m.write(e + 0x24, m.read(c + 0x9f4));
        need(x, pc, 0x00567cf7, "W2: original calls GetAIStateReputation on a null player creature (StoreValuesInLastUpdatePartyObject)");
        p.write_byte(e + 0x30, x.ai_state_reputation_0057cd90(pc, m.read(c + 4)));
        m.write(e + 0x34, m.read(c + 0x4e0));
        p.write_byte(e + 0x38, m.read8(c + 0x4d1));
    }
    if ((flags & 0x40000u) != 0) m.write(e + 0x3c, m.read(c + 0x8c));
    if ((flags & 0x200u) != 0) { m.write(e + 0x40, m.read(c + 0x90)); m.write(e + 0x44, m.read(c + 0x94)); }   // z not stored (original)
    if ((flags & 0x400u) != 0) {
        m.write16(e + 0x2a, m.read16(c + 0xe0));
        m.write16(e + 0x28, p.vcall1(c, 0x9c, 1));
        m.write16(e + 0x2c, m.read16(c + 0xe4));
        m.write16(e + 0x2e, p.vcall1(c, 0x98, 1));
    }
    if ((flags & 0x80000u) != 0) {
        m.write16(e + 0x6a, m.read16(m.read(c + 0xa74) + 0x122));
        m.write16(e + 0x68, m.read16(m.read(c + 0xa74) + 0x124));
        m.write16(e + 0x6c, m.read16(m.read(c + 0xa74) + 0x126));
        m.write16(e + 0x6e, x.max_force_points_004fd490(c));
    }
    if ((flags & 0x4000u) != 0) {
        m.write(e + 0x4c, m.read(c + 0x9d4));
        m.write(e + 0x50, (m.read8(c + 0x9f0) & 0x40u) != 0 ? 1u : 0u);
        m.write(e + 0x58, singleton_party_005bf690(m, p, x.faction_00513fc0(c)));
        const Word id = m.read(c + 4);
        m.write(e + 0x54, leader_005bf5c0(m, p, x, x.faction_00513fc0(c)) == id ? 1u : 0u);
        m.write(e + 0x5c, m.read(c + 0x930));
        m.write(e + 0x64, m.read(c + 0x9e4));
        m.write(e + 0x60, m.read(c + 0x9e0));
    }
    if ((flags & 0x20u) != 0) {
        m.write16(e + 0x10, p.vcall0(c, 0xd0));
        const Word ref = p.vcall1(c, 0xc8, s);                         // hidden CResRef result at [esp+0C]
        (void)x.resref_assign_00406120(e, s + 0x10, ref);             // result slot [esp+1C]
    }
}

// ===================================================================== 0056E360
void repository_update_0056e360(Memory& m, w2::entry::Ports& p, PartyUpdatePorts& x, Word self, Word player, Word object, Word repository,
                                Word luo, Word which, Word letter, Word s) {
    (void)player;
    const Word pos = s;
    m.write(pos, 0);
    Word upd = 0, id = which;                                          // the id lives in the `which` argument slot
    I i = static_cast<I>(m.read(repository + 0x10)) - 1;
    luo_set_end_005ca520(m, luo, pos, which);
    for (;;) {
        if (i < 0 && m.read(pos) == 0) return;
        Word item = 0;
        if (i >= 0) {
            id = item_list_object_id_005560e0(m, repository, static_cast<Word>(i));
            item = x.item_by_id_004ae760(srv(m, p), id);
        }
        if (m.read(pos) != 0) upd = luo_update_item_005ca4d0(m, x, luo, m.read(pos), which);
        if (i < 0) {                                                   // 0056E59B: leftover entry -> delete
            record(m, p, self, letter, 0x44);
            write_object_id_00578570(m, p, self, m.read(need(x, upd, 0x0056e5c3, "W2: original reads a null LUO update item")));
            const Word old = m.read(pos);
            luo_get_prev_005ca570(m, x, luo, pos, which);
            luo_remove_005ca5c0(m, x, luo, old, which);
            continue;
        }
        if (m.read(pos) == 0) {                                        // 0056E519: new item -> add
            record(m, p, self, letter, 0x41);
            write_object_id_00578570(m, p, self, id);
            need(x, item, 0x0056e549, "W2: original reads a null item (WriteRepositoryUpdate add)");
            if (m.read8(item + 0x18) == 6) {
                x.item_appearance_00567090(self, item);
                x.active_item_properties_00566f20(self, item, p.vcall0(object, 0x30));
            }
            const Word stack = m.read(item + 0x28c);
            const Word cost = x.item_cost_00554000(item);
            luo_add_head_005ca630(m, x, luo, id, which, stack, cost);
            --i;
            continue;
        }
        need(x, upd, 0x0056e408, "W2: original reads a null LUO update item");
        if (id != m.read(upd)) {                                       // 0056E40A: different item at this position -> delete
            record(m, p, self, letter, 0x44);
            write_object_id_00578570(m, p, self, m.read(upd));
            const Word old = m.read(pos);
            luo_get_prev_005ca570(m, x, luo, pos, which);
            luo_remove_005ca5c0(m, x, luo, old, which);
            continue;
        }
        need(x, item, 0x0056e45d, "W2: original reads a null item (WriteRepositoryUpdate compare)");
        if (m.read(item + 0x28c) == m.read(upd + 4) && x.item_cost_00554000(item) == m.read(upd + 8)) {
            --i;
            luo_get_prev_005ca570(m, x, luo, pos, which);
            continue;
        }
        record(m, p, self, letter, 0x55);                              // 0056E493: changed stack / cost -> update
        write_object_id_00578570(m, p, self, m.read(item + 0x14));
        sm::message_word_004d6340(m, p, self, m.read(item + 0x28c), 0x20);
        sm::message_word_004d6340(m, p, self, x.item_cost_00554000(item), 0x20);
        m.write(upd + 4, m.read(item + 0x28c));
        const Word cost = x.item_cost_00554000(item);
        --i;
        m.write(upd + 8, cost);
        luo_get_prev_005ca570(m, x, luo, pos, which);
    }
}
}
