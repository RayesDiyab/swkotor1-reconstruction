// W2 tier 4: ComputeUpdateRequired 0056B950 / ComputeAppearanceUpdateRequired 00567100. See header.
//
// CSWSPlayerLastUpdateObject fields read (L = last-update object; reading instruction in brackets):
//   +30/+34/+38 float position          [0056BC49.., 0056C05B..]
//   +3C  area id (cmp area+120h)        [0056BCA4, 0056C122]
//   +40/+44/+48 float orientation        [0056BC1C.., 0056BCA9.., 0056C09E..]
//   +4C  animation id                    [0056BA61, 0056C04B]
//   +50  float animation speed           [0056BA57, 0056C03C]
//   +54/+58/+5C float last path point    [0056BBBC, 0056BBC3, 0056BBC6]
//   +60  byte AI mode (vs ai+28h)        [0056BFCB]
//   +62  word, +64 byte, +66 word, +68 dword, +6C byte, +70 dword  (0x40 group) [0056B9DB..0056BA21]
//   +74  dword (vs vtbl+C4h)             [0056BAA6]
//   +78  dword (vs creature+504h)        [0056BA92]
//   +7C  CResRef (portrait)              [0056BFB2, 0056C0F4];  +8C word (vs vtbl+D0h) [0056BF98, 0056C0DA]
//   +A0/+A4 float walk / run rate        [0056BCE1, 0056BCFD]
//   +A8  visual-effect list              [0056BA3C, 0056C137]
//   +B5  byte damage level               [0056BF7D]
//   +BA/+BC/+BE/+C0 words (hit points)   [0056BD4C..0056BD8D]
//   +C4/+C8/+CC/+D0 dwords (0x4000 group)[0056BE17..0056BE82]
//   +D4  creature list (door/placeable)  [0056C23F]
//   +E0/+E4/+E8/+EC/+F0 dwords (door/placeable/trigger state) [0056C167..0056C2F2]
//   +F4  byte AI-state reputation        [0056BA0B]
//   +F8/+FC/+100 dwords (visibility bits; +100 also party count) [0056BF09, 0056BF1D..0056BF66]
//   +104 word, +108 CExoString, +110/+111 bytes, +112+i class bytes, +114+i level bytes, +118 word, +11C dword [0056BAD6..0056BB6D]
//   +120 dword                           [0056BE17]
//   +130/+132/+134/+136 words (force points) [0056BDA4..0056BDDB]
//   +148/+14C/+150 dwords                [0056BFDD..0056BFFF]
// 00567100 reads: +00..+0C (4 dwords), +10 word, +12..+1A bytes, +1C dword, +20 byte, +2A word.
#include "d34a525424-w2_g07_compute.hpp"
#include <cmath>
#include <cstring>
namespace w2::g07 {
namespace {
using I = std::int32_t;
double fd(Memory& m, Word a) { Word b = m.read(a); float v; std::memcpy(&v, &b, 4); return v; }
double fb(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
float fs(double v) { return static_cast<float>(v); }
Word w16(Memory& m, Word a) { return m.read16(a) & 0xffffu; }
Word b8(Memory& m, Word a) { return m.read8(a) & 0xffu; }
constexpr double kNear = static_cast<double>(0.0099999998f);   // .rdata 0073DFEC
constexpr double kFacing = static_cast<double>(0.95f);         // .rdata 00745E50
constexpr double kRate = 10.0;                                 // .rdata 00749358 (qword)
constexpr Word kInvalid = 0x7f000000u;
// x87 "FCOMP; TEST AH,1; JE" family: C0 is set for "less" and for unordered.
bool c0(double a, double b) { return !(a >= b); }
}

Word creature_in_party_005be800(Memory& m, Word faction, Word id) {
    const Word data = m.read(faction), count = m.read(faction + 4);
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(count); ++i) if (m.read(data + 4 * i) == id) return 1;
    return 0;
}

// Each effect of `outer` needs an effect in `inner` with the same type word (+0) and, when the outer
// effect is a beam, the same target id (+4). An empty `inner` fails any non-empty `outer`.
static bool all_matched(Memory& m, ComputePorts& p, Word outer, Word inner) {
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(outer + 4)); ++i) {
        const Word e = m.read(m.read(outer) + 4 * i);
        const Word type = w16(m, e);
        const Word beam = p.is_beam_004ca9d0(e);
        bool found = false;
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(inner + 4)) && !found; ++j) {
            const Word o = m.read(m.read(inner) + 4 * j);
            if (w16(m, o) == type && (beam == 0 || m.read(o + 4) == m.read(e + 4))) found = true;
        }
        if (!found) return false;
    }
    return true;
}

Word compare_vfx_lists_005672f0(Memory& m, ComputePorts& p, Word self, Word a, Word b) {
    (void)self;
    return all_matched(m, p, a, b) && all_matched(m, p, b, a) ? 1 : 0;
}

Word compare_creature_lists_0056b830(Memory& m, Word self, Word a, Word b) {
    (void)self;
    const Word n = m.read(a + 4);
    if (n != m.read(b + 4)) return 0;
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(n); ++i) if (m.read(m.read(a) + 4 * i) != m.read(m.read(b) + 4 * i)) return 0;
    return 1;
}

void update_appearance_equipped_004ed620(Memory& m, ComputePorts& p, Word c) {
    auto id = [&](Word slot) { const Word it = p.item_in_slot_005a4c20(m.read(c + 0xa2c), slot); return it != 0 ? m.read(it + 0x14) : kInvalid; };
    const Word a = id(0x20), b = id(0x10), d = id(2), e = id(1);
    if (a != m.read(c + 0xa50) || b != m.read(c + 0xa54) || d != m.read(c + 0xa58) || e != m.read(c + 0xa5c)) {
        m.write(c + 0xa50, a); m.write(c + 0xa54, b); m.write(c + 0xa58, d); m.write(c + 0xa5c, e);
    }
}

Word compute_appearance_update_required_00567100(Memory& m, ComputePorts& p, Word self, Word object, Word L) {
    (void)self;
    if (b8(m, object + 8) != 5) return 0;                            // [00746634]
    const Word c = p.vcall0(object, 0x30);
    update_appearance_equipped_004ed620(m, p, c);
    Word f = 0;
    if (w16(m, L + 0x10) != w16(m, c + 0xa60)) f = 1;
    if (b8(m, L + 0x12) != b8(m, c + 0xa62)) f |= 2;
    if (b8(m, L + 0x13) != b8(m, c + 0xa63)) f |= 4;
    if (b8(m, L + 0x14) != b8(m, c + 0xa64)) f |= 8;
    if (b8(m, L + 0x15) != b8(m, c + 0xa65)) f |= 0x10;
    if (b8(m, L + 0x16) != b8(m, c + 0xa66)) f |= 0x20;
    if (b8(m, L + 0x17) != b8(m, c + 0xa67)) f |= 0x40;
    if (b8(m, L + 0x18) != b8(m, c + 0xa68)) f |= 0x80;
    if (m.read(L + 0x1c) != m.read(c + 0xa6c)) f |= 0x4000;
    if (b8(m, L + 0x20) != b8(m, c + 0xa70)) f |= 0x8000;
    if (b8(m, L + 0x19) != b8(m, c + 0xa69)) f |= 0x800;
    if (b8(m, L + 0x1a) != b8(m, c + 0xa6a)) f |= 0x1000;
    if (m.read(L) != m.read(c + 0xa50) || m.read(L + 4) != m.read(c + 0xa54) || m.read(L + 8) != m.read(c + 0xa58) || m.read(L + 0x0c) != m.read(c + 0xa5c)) f |= 0x200;
    if (w16(m, L + 0x2a) != w16(m, c + 0x9d8)) f |= 0x2000;
    return f;
}

namespace {
// shared float tests
double dist2(Memory& m, Word L, Word o) {                         // ((dz*dz + dy*dy) + dx*dx), L - object
    const double dx = fd(m, L + 0x30) - fd(m, o + 0x90), dy = fd(m, L + 0x34) - fd(m, o + 0x94), dz = fd(m, L + 0x38) - fd(m, o + 0x98);
    return (dz * dz + dy * dy) + dx * dx;
}
double facing(Memory& m, Word L, Word o) {                        // (L48*o.A4 + L44*o.A0) + L40*o.9C
    return (fd(m, L + 0x48) * fd(m, o + 0xa4) + fd(m, L + 0x44) * fd(m, o + 0xa0)) + fd(m, L + 0x40) * fd(m, o + 0x9c);
}
bool area_moved(Memory& m, ComputePorts& p, Word o, Word L) {      // GetArea twice (original)
    if (p.object_area_004cb120(o) == 0) return false;
    return m.read(p.object_area_004cb120(o) + 0x120) != m.read(L + 0x3c);
}
bool portrait_changed(Memory& m, ComputePorts& p, Word o, Word L, Word scratch) {
    if ((p.vcall0(o, 0xd0) & 0xffffu) != w16(m, L + 0x8c)) return true;
    return p.resref_ne_00406dc0(L + 0x7c, p.vcall1(o, 0xc8, scratch)) != 0;
}
}

Word compute_update_required_0056b950(Memory& m, ComputePorts& p, Word self, Word player, Word object, Word L, Word flag, Word scratch) {
    Word f = 0;
    const Word server = m.read(p.app_007a39fc() + 8);
    const Word type = b8(m, object + 8);
    if (type == 5) {                                                 // [00746634] creature
        const Word c = p.vcall0(object, 0x30);
        if (c == 0) return 0;
        p.compute_ai_state_004f1460(c);
        Word rep = 2;
        if (player != 0 && p.game_object_00561080(player) != 0 && p.vcall0(p.game_object_00561080(player), 0x30) != 0)
            rep = p.ai_state_reputation_0057cd90(p.vcall0(p.game_object_00561080(player), 0x30), m.read(c + 4)) & 0xffu;
        if (w16(m, L + 0x62) != w16(m, c + 0x9f0) || b8(m, L + 0x64) != b8(m, c + 0x9f2) || w16(m, L + 0x66) != w16(m, c + 0x9fc)
            || m.read(L + 0x70) != m.read(c + 0x9f4) || b8(m, L + 0xf4) != rep || m.read(L + 0x68) != m.read(c + 0x4e0)
            || b8(m, L + 0x6c) != b8(m, c + 0x4d1))
            f = 0x40;                                                // 0056BA29 (assignment)
        if (compare_vfx_lists_005672f0(m, p, self, L + 0xa8, c + 0x130) == 0) f |= 8;
        if (!(fd(m, object + 0xd8) == fd(m, L + 0x50)) || m.read(L + 0x4c) != m.read(c + 0xd4)) f |= 4;   // 0056BA5F
        const Word anim = m.read(c + 0xd4);
        if (anim == 0x2719 || anim == 0x277d) {
            const Word target = m.read(c + 0x504);
            if (target != kInvalid && target != m.read(L + 0x78)) f |= 6;
        }
        if (p.vcall0(c, 0xc4) != m.read(L + 0x74)) f |= 2;
        const Word ai = m.read(c + 0x340);
        bool sheet = false;
        for (Word i = 0; i < b8(m, m.read(c + 0xa74) + 0x89); ++i) {
            if ((p.get_class_005a4e90(m.read(c + 0xa74), i) & 0xffu) != b8(m, L + 0x112 + i)) { sheet = true; break; }
            if ((p.get_class_level_005a5090(m.read(c + 0xa74), i, 0) & 0xffu) != b8(m, L + 0x114 + i)) { sheet = true; break; }
        }
        if (!sheet) {
            const Word s = m.read(c + 0xa74);
            sheet = w16(m, s + 0xdc) != w16(m, L + 0x104) || p.string_ne_005e5310(s + 0xe0, L + 0x108) != 0
                 || b8(m, s + 0xe8) != b8(m, L + 0x110) || b8(m, s + 0x64) != b8(m, L + 0x111)
                 || w16(m, s + 0x17e) != w16(m, L + 0x118) || m.read(c + 0xf4) != m.read(L + 0x11c);
        }
        if (sheet) f |= 0x1000;
        if (p.animation_stationary_004cae60(c, m.read(c + 0xd4)) == 0) {
            const I n = static_cast<I>(m.read(ai + 0x8c));
            const Word half = static_cast<Word>(n / 2) & 0xffffu;    // CDQ/SUB/SAR 1, then AX
            if (half != 0) {
                const Word pt = m.read(ai + 0x90) + 8 * half;
                const float dx = fs(fd(m, pt - 8) - fd(m, L + 0x54));
                const double dy = fd(m, pt - 4) - fd(m, L + 0x58);
                const double nz = -fd(m, L + 0x5c);
                const double sum = (nz * nz + dy * dy) + static_cast<double>(dx) * dx;
                if (!c0(sum, kNear) || m.read(ai + 0x23c) != 0) {    // 0056BBEE / 0056BBF6
                    f |= 7;
                    m.write(ai + 0x23c, 0);
                }
            }
            if (m.read(c + 0xa14) == 1 && facing(m, L, c) < kFacing) f |= 3;   // 0056BCD3
        } else if ((f & 4) != 0) {
            f |= 3;                                                  // 0056BC43
        } else if (!c0(dist2(m, L, c), kNear)) {
            f |= 3;                                                  // 0056BC85
        } else if (area_moved(m, p, c, L)) {
            f |= 3;                                                  // 0056BCA7
        } else if (facing(m, L, c) < kFacing) {
            f |= 3;
        }
        if (std::fabs(fd(m, L + 0xa0) - fb(p.walk_rate_bits_004f1b20(c))) > kRate
            || std::fabs(fd(m, L + 0xa4) - fb(p.run_rate_bits_004f1be0(c))) > kRate)
            f |= 0x100;                                              // 0056BCF4 / 0056BD10
        bool vitals = flag != 0;
        if (!vitals) {
            const Word faction = p.faction_00513fc0(p.vcall0(p.game_object_00561080(player), 0x30));
            vitals = creature_in_party_005be800(m, faction, m.read(c + 4)) != 0;
        }
        if (vitals) {
            if (w16(m, L + 0xbc) != w16(m, c + 0xe0) || w16(m, L + 0xba) != (p.vcall1(c, 0x9c, 1) & 0xffffu)
                || w16(m, L + 0xbe) != w16(m, c + 0xe4) || w16(m, L + 0xc0) != (p.vcall1(c, 0x98, 1) & 0xffffu))
                f |= 0x400;
            const Word s = m.read(c + 0xa74);
            if (w16(m, L + 0x132) != w16(m, s + 0x122) || w16(m, L + 0x130) != w16(m, s + 0x124) || w16(m, L + 0x134) != w16(m, s + 0x126)
                || w16(m, L + 0x136) != (p.max_force_points_004fd490(c) & 0xffffu))
                f |= 0x80000;
        }
        if (p.client_object_004ae910(server, m.read(c + 4)) != 0 || flag != 0) {
            const bool differ =
                m.read(p.vcall0(c, 0x30) + 0x9d4) != m.read(L + 0x120)
                || ((b8(m, p.vcall0(c, 0x30) + 0x9f0) & 0x40u) == 0x40u ? 1u : 0u) != m.read(L + 0xc4)
                || p.singleton_party_005bf690(p.faction_00513fc0(c)) != m.read(L + 0xcc)
                || m.read(p.vcall0(c, 0x30) + 0x930) != m.read(L + 0xd0)
                || [&] { const Word id = m.read(c + 4); return (p.leader_005bf5c0(p.faction_00513fc0(p.vcall0(c, 0x30))) == id ? 1u : 0u) != m.read(L + 0xc8); }();
            if (differ) f |= 0x4000;
        }
        if (p.game_object_00561080(player) != 0) {
            const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
            const Word oc = p.vcall0(object, 0x30);
            if (pc != 0 && oc != 0) {
                bool vis = false;
                if (m.read(pc + 4) == m.read(oc + 4)) {
                    const Word module = p.module_004ae6b0(server);
                    if (module != 0) vis = p.array_count_004af240(module + 0x13c, m.read(oc + 4)) != m.read(L + 0x100);
                } else {
                    const Word el = p.visible_element_00517a20(oc, m.read(pc + 4));
                    if (el != 0) {
                        const Word b = b8(m, el + 4);
                        vis = ((b >> 1) & 1u) != m.read(L + 0xfc) || (b & 1u) != m.read(L + 0xf8) || ((b >> 4) & 1u) != m.read(L + 0x100);
                    } else {
                        vis = m.read(L + 0xfc) == 1 || m.read(L + 0xf8) == 1 || m.read(L + 0x100) == 1;
                    }
                }
                if (vis) f |= 0x8000;
            }
        }
        if ((p.damage_level_004cb020(c) & 0xffu) != b8(m, L + 0xb5)) f |= 0x800;
        if (portrait_changed(m, p, object, L, scratch)) f |= 0x20;
        if (flag != 0 && b8(m, L + 0x60) != m.read(ai + 0x28)) f |= 7;  // 0056BFD3
        if (m.read(L + 0x148) != m.read(c + 0x204) || m.read(L + 0x150) != m.read(c + 0x20c) || m.read(L + 0x14c) != m.read(c + 0x208))
            f |= 0x100000;
    }
    // 0056C00F: item 6, door 0Ah, area of effect 0Bh, placeable 9
    if (type == 6 || type == 0x0a || type == 0x0b || type == 9) {
        if (!(fd(m, object + 0xd8) == fd(m, L + 0x50))) f |= 4;
        if (m.read(L + 0x4c) != m.read(object + 0xd4)) f |= 4;
        if (!c0(dist2(m, L, object), kNear)) f |= 1;                  // 0056C097
        if (fd(m, object + 0xa4) * fd(m, L + 0x48) + fd(m, object + 0xa0) * fd(m, L + 0x44) + fd(m, object + 0x9c) * fd(m, L + 0x40) < kFacing)
            f |= 2;                                                  // 0056C0C8
        if (portrait_changed(m, p, object, L, scratch)) f |= 0x20;
        if (area_moved(m, p, object, L)) f |= 3;
        if (compare_vfx_lists_005672f0(m, p, self, L + 0xa8, object + 0x130) == 0) f |= 8;
        Word state = 0, list = 0;
        bool check = false;
        if (type == 0x0a) {                                          // door: vtbl+18h
            check = true;
            const Word d = p.vcall0(object, 0x18);
            state = m.read(d + 0x2c4) != m.read(L + 0xec) || m.read(p.vcall0(object, 0x18) + 0xf8) != m.read(L + 0xf0)
                 || m.read(p.vcall0(object, 0x18) + 0x2e8) != m.read(L + 0xe0) || b8(m, p.vcall0(object, 0x18) + 0x2f4) != m.read(L + 0xe4);
            if (state == 0) list = p.vcall0(object, 0x18) + 0x2dc;
        } else if (type == 9) {                                      // placeable: vtbl+48h
            check = true;
            const Word q = p.vcall0(object, 0x48);
            state = m.read(q + 0x260) != m.read(L + 0xec) || m.read(p.vcall0(object, 0x48) + 0xf8) != m.read(L + 0xf0)
                 || m.read(p.vcall0(object, 0x48) + 0x278) != m.read(L + 0xe0) || b8(m, p.vcall0(object, 0x48) + 0x28c) != m.read(L + 0xe4);
            if (state == 0) list = p.vcall0(object, 0x48) + 0x318;
        }
        if (check && (state != 0 || compare_creature_lists_0056b830(m, self, list, L + 0xd4) == 0)) f |= 0x10;
    }
    if (type == 7) {                                                 // [00746636] trigger: vtbl+40h
        if (m.read(p.vcall0(object, 0x40) + 0x2bc) != 0) {
            if (m.read(p.vcall0(object, 0x40) + 0x2c8) != m.read(L + 0xe4)) f |= 0x10;
            const Word cr = p.creature_by_id_004ae770(m.read(p.app_007a39fc() + 8), m.read(p.vcall0(object, 0x40) + 0x2e4));
            const Word t = p.vcall0(object, 0x40);
            Word faction;
            if (cr != 0) {
                const Word fac = m.read(m.read(cr + 0xa74) + 0x78);
                if (m.read(t + 0x2b8) != fac) m.write(p.vcall0(object, 0x40) + 0x2b8, fac);   // side effect (original)
                faction = m.read(m.read(cr + 0xa74) + 0x78);
            } else {
                faction = m.read(t + 0x2b8);
            }
            if (m.read(L + 0xe8) != faction) f |= 0x10;
        }
    }
    return f;
}
}
