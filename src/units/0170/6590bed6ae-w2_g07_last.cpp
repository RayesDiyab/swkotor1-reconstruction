// W2 tier 4 (fork B): 00577000 / 00575C80 / 00568480. See w2_g07_last.hpp.
//
// CSWSPlayerLastUpdateObject (0x394 bytes) fields touched here. Constructor 005CAA50 (W2) zeroes +30..+38,
// +40..+48, +54..+5C, +A8/+AC/+B0 (list), +D4/+D8/+DC (list), +2A word; sets +4C = 2711h, +50 = 1.0f,
// +112 = +114 = FFFFh (words); constructs CResRef +7C, CExoLocString +90 / +98, CExoString +108.
// Destructor 005CA6A0 (W2): ~CExoString +108, free [+D4] (then 0), free [+A8] (then 0), ~CExoLocString +98, +90.
//   off   size  meaning (as used)                                  written at
//   +000  4     appearance dword (creature +A50)                   005770E3, 005684D9
//   +004  4     appearance dword (creature +A54)                   005770EB, 005684E1
//   +008  4     appearance dword (creature +A58)                   005770D1, 005684EA
//   +00C  4     appearance dword (creature +A5C)                   005770DA, 005684F3
//   +010  2     appearance word  (creature +A60)                   00577076, 005684FD
//   +012..+01A  9 appearance bytes (creature +A62..+A6A)           0057709B.., 00568507..
//   +01C  4     appearance dword (creature +A6C)                   005770F4, 00568558
//   +020  1     appearance byte  (creature +A70)                   005770FD, 00568561
//   +000..+023  zeroed (9 dwords) for non-creatures                00577150
//   +024  4     = 1 (seen this frame)                              00577044
//   +028  1     object type byte (object +8)                       00575C9F
//   +02A  2     creature +9D8                                      00577107
//   +02C  4     object id                                          0057704E
//   +030  12    position (object +90)                              00575CCA
//   +03C  4     area id ([area+11C+4]) or 7F000000h                00575CF2 / 00575CF7
//   +040  12    orientation (object +9C)                           00575D10
//   +04C  4     animation (object +D4, dword)                      00575CB7
//   +050  4     animation speed (object +D8, FLD/FSTP)             00575CAA
//   +054  12    path point x (raw), y (FLD/FSTP), 0                00576028..00576045
//   +060  1     ai +28 (creature [+340]+28)                        00575F4B
//   +062  2     creature +9F0      +064 1 creature +9F2            00575F66 / 00575F70
//   +066  2     creature +9FC      +068 4 creature +4E0            00575F7A / 00575F8D
//   +06C  1     creature +4D1      +070 4 creature +9F4            00575F96 / 00575F84
//   +074  4     creature vtable C4h result                         00575D3A
//   +078  4     creature +504 (animation 2719h / 277Dh)            00575D5E
//   +07C  16    CResRef (object vtable C8h)                        00575D91
//   +08C  2     object vtable D0h result (word)                    00575D76
//   +090  8     CExoLocString (stats +34)                          005684B9
//   +098  8     CExoLocString (stats +3C)                          005684CE
//   +0A0  4     walk rate (ST0 -> m32)                             00576058
//   +0A4  4     run rate  (ST0 -> m32)                             00576065
//   +0A8  ?     visual effect lists (00570530 dst)                 00575DAF
//   +0B4  1     associate state                                    0057623D
//   +0B5  1     damage level                                       0057607B
//   +0BA  2     vtable 9Ch(1)   +0BC 2 creature +E0   +0BE 2 creature +E4   +0C0 2 vtable 98h(1)   00576096..005760C5
//   +0C4  4     (creature +9F0 & 40h) != 0                         00576146
//   +0C8  4     faction leader == creature id                     00576178
//   +0CC  4     faction singleton party                            0057615A
//   +0D0  4     creature +930                                      00576184
//   +0D4  ?     door/placeable creature lists (005706F0 dst)       00575DE2 / 00575E59
//   +0E0  4     door +2E8 / placeable +278                         00575DF4 / 00575E6B
//   +0E4  4     door byte +2F4 / placeable byte +28C / trigger +2C8   00575E08 / 00575E7F / 00575EE6
//   +0E8  4     trigger +2B8                                       00575ED3
//   +0EC  4     door +2C4 / placeable +260                         00575E1B / 00575E92
//   +0F0  4     door +F8 / placeable +F8                           00575E2E / 00575EA5
//   +0F4  1     AI-state reputation toward the observer, or 2      00575FD1 / 00575FD9
//   +0F8  4     visible element bit 0    +0FC 4 bit 1    +100 4 bit 4 (or module list count)   005761F2.. / 005761D2
//   +104  2     stats +DC   +108 8 CExoString (stats +E0)   +110 1 stats +E8   +111 1 stats +64   0057625D..00576297
//   +112+i 1    class i (overlaps the level bytes; original)      005762F6
//   +114+i 1    class level i                                      00576305
//   +118  2     stats +17E   +11C 4 creature +F4                   005762AA / 005762B7
//   +120  4     creature +9D4                                      0057612F
//   +130  2     stats +124   +132 2 stats +122   +134 2 stats +126   +136 2 max force points   005760E3..00576119
//   +148  4     +204   +14C 4 +208   +150 4 +20C (placeable or creature)   00575F08.. / 00576329..
//   +154  0x24 x 16-byte slots: +0 7F000000h, +4 byte 0, +8 FFh, +C word 0 (creatures)   00577120..
#include "d5bc14c6f5-w2_g07_last.hpp"
#include <cstdint>
#include <initializer_list>
namespace w2::g07 {
namespace {
namespace msg = w2::msg;
namespace sm = kotor::server_mainloop;
constexpr Word kInvalid = 0x7f000000u;   // [00746644]
constexpr Word kOne = 0x3f800000u;
enum : Word { kCreature = 5, kItem = 6, kTrigger = 7, kPlaceable = 9, kDoor = 0x0a, kAoe = 0x0b };
void w8(LastPorts& p, Word a, Word v) { p.msg().write_byte(a, v & 0xffu); }   // byte store
void copy3(Memory& m, Word d, Word s) { m.write(d, m.read(s)); m.write(d + 4, m.read(s + 4)); m.write(d + 8, m.read(s + 8)); }
void byte_out(Memory& m, LastPorts& p, Word self, Word v) { msg::write_byte_004d62c0(m, p.msg(), self, v, 8); }
void word_out(Memory& m, LastPorts& p, Word self, Word v) { msg::write_word16_004d6300(m, p.msg(), self, v, 0x10); }
void bool_out(Memory& m, LastPorts& p, Word self, Word v) { msg::write_bool_004d68e0(m, p.msg(), self, v); }
void float_out(Memory& m, LastPorts& p, Word self, Word bits) { msg::write_float_bits_004d6920(m, p.msg(), self, bits, kOne, 0x20); }
// "hostile" BOOL shared by the door / trigger / placeable branches: reputation < 5Ah and the object's
// faction differs from the observer creature's stats +78.
Word hostile(Memory& m, LastPorts& p, Word player, Word object, Word as_slot, Word faction_offset) {
    const Word go = p.game_object_00561080(player);
    if (static_cast<std::int32_t>(p.reputation_0057cb80(object, m.read(go + 4), 0)) >= 0x5a) return 0;
    const Word go2 = p.game_object_00561080(player);
    const Word faction = m.read(p.vcall0(object, as_slot) + faction_offset);
    const Word c = p.vcall0(go2, 0x30);
    return faction != m.read(m.read(c + 0xa74) + 0x78) ? 1u : 0u;
}
}

// 005E58A0 CExoString::IsEmpty is an existing owner (@frame_LogIsEmpty_005e58a0@8): port.

Word get_locstring_server_00578870(Memory& m, LastPorts& p, Word player, Word loc, Word loc2, Word out, Word gender, Word s) {
    const Word a = s, b = s + 8, c = s + 0x10, d = s + 0x18, e = s + 0x20;
    p.str_empty_005b3190(a);
    p.str_empty_005b3190(b);
    p.str_assign_text_005e5140(out, p.msg().empty_text_0073d71c());
    const Word server = m.read(p.app_007a39fc() + 8);
    if (server == 0) {
        p.str_destroy_005e5c20(b);
        p.str_destroy_005e5c20(a);
        return 0;
    }
    const Word language = w2::msg::player_language_004b4050(m, p.loc_ports(), m.read(server + 4), player, s + 0x28);   // 004AE810 thunk
    (void)p.loc_get_string_005ea130(loc, language, a, gender);
    (void)p.loc_get_string_005ea130(loc2, language, b, gender);
    if (p.str_is_empty_005e58a0(b) != 0 || p.str_is_empty_005e58a0(a) != 0) {
        p.str_assign_005e5c50(out, p.str_concat_005e5d10(a, d, b));
        p.str_destroy_005e5c20(d);
    } else {
        p.msg().str_literal_005e5a90(c, p.space_text_0073d5d8());
        const Word joined = p.str_concat_005e5d10(a, d, c);
        p.str_assign_005e5c50(out, p.str_concat_005e5d10(joined, e, b));
        p.str_destroy_005e5c20(e);
        p.str_destroy_005e5c20(d);
        p.str_destroy_005e5c20(c);
    }
    p.str_destroy_005e5c20(b);
    p.str_destroy_005e5c20(a);
    return 1;
}

void add_door_appearance_00566da0(Memory& m, LastPorts& p, Word self, Word door) {
    bool_out(m, p, self, m.read(door + 0x3c0));
    byte_out(m, p, self, m.read8(door + 0x2a0));
    if (m.read8(door + 0x2a0) == 0) byte_out(m, p, self, m.read8(door + 0x2a1));
}

void add_trigger_geometry_00567c20(Memory& m, LastPorts& p, Word self, Word trigger) {
    byte_out(m, p, self, m.read8(trigger + 0x284));
    for (Word i = 0, off = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(trigger + 0x284)); ++i, off += 0x0c) {
        float_out(m, p, self, m.read(m.read(trigger + 0x288) + off));
        float_out(m, p, self, m.read(m.read(trigger + 0x288) + off + 4));
        float_out(m, p, self, m.read(m.read(trigger + 0x288) + off + 8));
    }
}

void add_placeable_appearance_00566df0(Memory& m, LastPorts& p, Word self, Word pl, Word s) {
    byte_out(m, p, self, m.read8(pl + 0x230));
    word_out(m, p, self, m.read(pl + 0xd4));
    for (Word off : {0xf8u, 0x328u, 0x280u, 0x330u, 0x260u, 0x32cu}) bool_out(m, p, self, m.read(pl + off));
    const Word str = s, loc = s + 8;
    p.str_empty_005b3190(str);
    p.loc_construct_005e9e10(loc);
    (void)get_locstring_server_00578870(m, p, 0xffffffffu, pl + 0x234, loc, str, 0, s + 0x10);
    p.loc_destroy_005ea000(loc);
    bool_out(m, p, self, p.str_is_empty_005e58a0(str) == 0 ? 1u : 0u);
    p.str_destroy_005e5c20(str);
}

namespace {
// Inlined CExoArrayList<T*>::Add growth: capacity 0 -> 10h, else doubled; old storage copied, then freed if non-null.
void list_append(Memory& m, LastPorts& p, Word list, Word value) {
    if (m.read(list + 4) == m.read(list + 8)) {
        const Word cap = m.read(list + 8) != 0 ? m.read(list + 8) * 2 : 0x10;
        const Word old = m.read(list);
        m.write(list + 8, cap);
        m.write(list, p.allocate_006fa7e6(cap * 4));
        for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(list + 4)); ++i)
            m.write(m.read(list) + 4 * i, m.read(old + 4 * i));
        if (old != 0) p.free_006fa390(old);
    }
    m.write(m.read(list) + 4 * m.read(list + 4), value);
    m.write(list + 4, m.read(list + 4) + 1);
}
}

void assign_visual_effect_lists_00570530(Memory& m, LastPorts& p, Word, Word dst, Word src) {
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(dst + 4)); ++i) {
        p.free_006fa390(m.read(m.read(dst) + 4 * i));
        m.write(m.read(dst) + 4 * i, 0);
    }
    if (m.read(dst) != 0) { p.free_006fa390(m.read(dst)); m.write(dst, 0); m.write(dst + 8, 0); }
    m.write(dst + 4, 0);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(src + 4)); ++i) {
        bool found = false;
        for (Word j = 0; static_cast<std::int32_t>(j) < static_cast<std::int32_t>(m.read(dst + 4)); ++j) {
            const Word e = m.read(m.read(src) + 4 * i), d = m.read(m.read(dst) + 4 * j);
            if (m.read16(e) != m.read16(d)) continue;
            if (p.is_beam_004ca9d0(e) == 0) { found = true; continue; }
            if (m.read(e + 4) == m.read(d + 4) && m.read8(e + 8) == m.read8(d + 8)) found = true;
        }
        if (found) continue;
        const Word copy = p.allocate_006fa7e6(0x0c);                    // only +0 word, +4, +8 byte are written
        m.write16(copy, m.read16(m.read(m.read(src) + 4 * i)));
        m.write(copy + 4, m.read(m.read(m.read(src) + 4 * i) + 4));
        w8(p, copy + 8, m.read8(m.read(m.read(src) + 4 * i) + 8));
        Word dup = 0;                                                    // inlined AddUnique (fresh pointer)
        for (Word j = 0; static_cast<std::int32_t>(j) < static_cast<std::int32_t>(m.read(dst + 4)); ++j)
            if (m.read(m.read(dst) + 4 * j) == copy) ++dup;
        if (dup != 0) continue;
        list_append(m, p, dst, copy);
    }
}

void assign_creature_lists_005706f0(Memory& m, LastPorts& p, Word, Word src, Word dst) {
    if (m.read(dst) != 0) { p.free_006fa390(m.read(dst)); m.write(dst, 0); m.write(dst + 8, 0); }
    m.write(dst + 4, 0);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(src + 4)); ++i)
        list_append(m, p, dst, m.read(m.read(src) + 4 * i));
}

Word associate_state_005676b0(Memory& m, LastPorts& p, Word, Word c) {
    Word bits = 0;
    const Word mode = p.detect_mode_004edae0(c) & 0xffu;
    if (mode == 1) bits = mode;
    if (m.read8(c + 0x4d1) == 1) bits |= 2;
    switch (m.read8(c + 0x4d2)) {
    case 1: bits |= 4; break;
    case 2: bits |= 8; break;
    case 3: bits |= 0x10; break;
    case 5: bits |= 0x20; break;
    case 6: bits |= 0x40; break;
    case 4: bits |= 0x80; break;
    default: break;
    }
    return bits;
}

Word last_construct_005caa50(Memory& m, LastPorts& p, Word self) {
    for (Word off : {0x30u, 0x34u, 0x38u, 0x40u, 0x44u, 0x48u, 0x54u, 0x58u, 0x5cu}) m.write(self + off, 0);
    p.ref_default_00405ed0(self + 0x7c);
    p.loc_construct_005e9e10(self + 0x90);
    p.loc_construct_005e9e10(self + 0x98);
    m.write(self + 0xac, 0); m.write(self + 0xb0, 0); m.write(self + 0xa8, 0);
    m.write(self + 0xd8, 0); m.write(self + 0xdc, 0); m.write(self + 0xd4, 0);
    p.str_empty_005b3190(self + 0x108);
    m.write16(self + 0x2a, 0);
    m.write(self + 0x50, kOne);
    m.write(self + 0x4c, 0x2711);
    m.write16(self + 0x112, 0xffff);
    m.write16(self + 0x114, 0xffff);
    return self;
}

void last_update_destroy_005ca6a0(Memory& m, LastPorts& p, Word self) {
    p.str_destroy_005e5c20(self + 0x108);
    p.free_006fa390(m.read(self + 0xd4));
    m.write(self + 0xd4, 0);
    p.free_006fa390(m.read(self + 0xa8));
    m.write(self + 0xa8, 0);
    p.loc_destroy_005ea000(self + 0x98);
    p.loc_destroy_005ea000(self + 0x90);
}

Word create_new_last_00577000(Memory& m, LastPorts& p, Word self, Word player, Word object, Word flags_out, Word appearance_out, Word s) {
    const Word mem = p.allocate_006fa7e6(0x394);
    const Word last = mem != 0 ? last_construct_005caa50(m, p, mem) : 0;
    if (last == 0) p.trap(0x00577044, "W2: original writes through a null CSWSPlayerLastUpdateObject (allocation failed)");
    m.write(last + 0x24, 1);
    m.write(last + 0x2c, m.read(object + 4));
    if (m.read8(object + 8) == kCreature) {
        p.update_appearance_equipped_004ed620(object);
        m.write16(last + 0x10, m.read16(object + 0xa60));
        for (Word k : {0x13u, 0x15u, 0x18u, 0x12u, 0x14u, 0x19u, 0x16u, 0x17u, 0x1au}) w8(p, last + k, m.read8(object + 0xa50 + k));
        m.write(last + 8, m.read(object + 0xa58));
        m.write(last + 0x0c, m.read(object + 0xa5c));
        m.write(last, m.read(object + 0xa50));
        m.write(last + 4, m.read(object + 0xa54));
        m.write(last + 0x1c, m.read(object + 0xa6c));
        w8(p, last + 0x20, m.read8(object + 0xa70));
        m.write16(last + 0x2a, m.read16(object + 0x9d8));
        for (Word k = 0; k < 0x24; ++k) {
            const Word e = last + 0x154 + 0x10 * k;
            w8(p, e + 4, 0);
            m.write(e, kInvalid);
            m.write(e + 8, 0xff);
            m.write16(e + 0x0c, 0);
        }
        m.write(appearance_out, 0xffffffffu);
    } else {
        for (Word k = 0; k < 9; ++k) m.write(last + 4 * k, 0);         // REP STOSD
    }
    const Word type = m.read8(object + 8);
    switch (type) {
    case kDoor: m.write(flags_out, 0xfffffff7u); break;
    case kItem: m.write(flags_out, 0xfffffff3u); break;
    case kAoe: m.write(flags_out, 0xffffffffu); break;
    case kTrigger: m.write(flags_out, 0xfffffff3u); break;
    case kPlaceable: m.write(flags_out, 0xfffffff7u); break;
    default:
        m.write(flags_out, 0x103967u);
        if (player != 0) (void)p.game_object_00561080(player);         // 005771D1: result unused
        break;
    }
    update_last_00575c80(m, p, self, player, object, last, m.read(flags_out), s);
    byte_out(m, p, self, 0x41);
    byte_out(m, p, self, type);
    msg::write_object_id_00578570(m, p.msg(), self, m.read(object + 4));
    if (type == kCreature) {
        const Word c = p.vcall0(object, 0x30);
        byte_out(m, p, self, m.read(c + 0xa88) != 0 ? 1u : 0u);
        for (Word off = 0x90; off <= 0xa4; off += 4) float_out(m, p, self, m.read(object + off));
        word_out(m, p, self, m.read16(object + 0xd4));
    } else if (type == kItem) {
        p.add_item_appearance_00567090(self, p.vcall0(object, 0x38));
        const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
        p.add_active_item_properties_00566f20(self, p.vcall0(object, 0x38), pc);
    } else if (type == kDoor) {
        add_door_appearance_00566da0(m, p, self, p.vcall0(object, 0x18));
        const Word h = hostile(m, p, player, object, 0x18, 0x2b8);
        p.write_locstring_005785c0(self, p.vcall0(p.vcall0(object, 0x18), 0x8c), 0);
        bool_out(m, p, self, h);
        word_out(m, p, self, m.read16(p.vcall0(object, 0x18) + 0xd4));
        bool_out(m, p, self, m.read(p.vcall0(object, 0x18) + 0x2c4));
        bool_out(m, p, self, m.read(p.vcall0(object, 0x18) + 0x2c8));
        bool_out(m, p, self, m.read(object + 0xf8));
        bool_out(m, p, self, p.door_is_linked_005890d0(p.vcall0(object, 0x18)));
        p.write_locstring_005785c0(self, p.vcall0(object, 0x18) + 0x3c8, 0);
    } else if (type == kAoe) {
        const Word aoe = p.vcall0(object, 0x50);
        sm::message_word_004d6340(m, p.msg(), self, m.read(aoe + 0x22c), 0x20);
        msg::write_object_id_00578570(m, p.msg(), self, m.read(aoe + 0x24c));
    } else if (type == kTrigger) {
        p.write_locstring_005785c0(self, p.vcall0(p.vcall0(object, 0x40), 0x8c), 0);
        bool_out(m, p, self, m.read(p.vcall0(object, 0x40) + 0x2bc));
        bool_out(m, p, self, m.read(p.vcall0(object, 0x40) + 0x2b4));
        if (m.read(p.vcall0(object, 0x40) + 0x2bc) != 0) byte_out(m, p, self, 0x0a);
        else byte_out(m, p, self, m.read8(p.vcall0(object, 0x40) + 0x2fc));
        if (m.read(p.vcall0(object, 0x40) + 0x2bc) != 0) {
            bool_out(m, p, self, hostile(m, p, player, object, 0x40, 0x2b8));
            byte_out(m, p, self, m.read8(p.vcall0(object, 0x40) + 0x2d4));
        }
        float_out(m, p, self, msg::x87_single_roundtrip(m.read(p.vcall0(object, 0x40) + 0x2e0)));
        p.write_locstring_005785c0(self, p.vcall0(object, 0x40) + 0x30c, 0);
        add_trigger_geometry_00567c20(m, p, self, p.vcall0(object, 0x40));
    } else if (type == kPlaceable) {
        p.write_locstring_005785c0(self, p.vcall0(p.vcall0(object, 0x48), 0x8c), 0);
        bool_out(m, p, self, hostile(m, p, player, object, 0x48, 0x23c));
        add_placeable_appearance_00566df0(m, p, self, p.vcall0(object, 0x48), s);
    }
    p.list_add_head_005e9d70(m.read(m.read(player + 0x0c)), last);
    return last;
}

void update_last_00575c80(Memory& m, LastPorts& p, Word self, Word player, Word object, Word last, Word flags, Word s) {
    const Word type = m.read8(object + 8);
    w8(p, last + 0x28, type);
    const bool anim = (flags & 4u) != 0;
    if (anim) m.write(last + 0x50, msg::x87_single_roundtrip(m.read(object + 0xd8)));
    if (anim) m.write(last + 0x4c, m.read(object + 0xd4));
    if ((flags & 1u) != 0) {
        copy3(m, last + 0x30, object + 0x90);
        if (p.object_area_004cb120(object) != 0) m.write(last + 0x3c, m.read(p.object_area_004cb120(object) + 0x11c + 4));
        else m.write(last + 0x3c, kInvalid);
    }
    if ((flags & 2u) != 0) {
        copy3(m, last + 0x40, object + 0x9c);
        if (m.read8(object + 8) == kCreature) m.write(last + 0x74, p.vcall0(p.vcall0(object, 0x30), 0xc4));
        const Word a = m.read(object + 0xd4);
        if (a == 0x2719 || a == 0x277d) {
            const Word c = p.vcall0(object, 0x30);
            if (c == 0) p.trap(0x00575d58, "W2: original reads +504 of a null creature (animation 2719h/277Dh on a non-creature)");
            m.write(last + 0x78, m.read(c + 0x504));
        }
    }
    if ((flags & 0x20u) != 0) {
        const Word w = p.vcall0(object, 0xd0);
        m.write16(last + 0x8c, w & 0xffffu);
        const Word ref = p.vcall1(object, 0xc8, s);
        p.resref_assign_00406120(last + 0x7c, s + 0x10, ref);
    }
    if ((flags & 8u) != 0) assign_visual_effect_lists_00570530(m, p, self, last + 0xa8, object + 0x130);
    if ((flags & 0x10u) != 0) {
        const Word t = m.read8(object + 8);
        if (t == kDoor) {
            assign_creature_lists_005706f0(m, p, self, p.vcall0(object, 0x18) + 0x2dc, last + 0xd4);
            m.write(last + 0xe0, m.read(p.vcall0(object, 0x18) + 0x2e8));
            m.write(last + 0xe4, m.read8(p.vcall0(object, 0x18) + 0x2f4));
            m.write(last + 0xec, m.read(p.vcall0(object, 0x18) + 0x2c4));
            m.write(last + 0xf0, m.read(p.vcall0(object, 0x18) + 0xf8));
        } else if (t == kPlaceable) {
            assign_creature_lists_005706f0(m, p, self, p.vcall0(object, 0x48) + 0x318, last + 0xd4);
            m.write(last + 0xe0, m.read(p.vcall0(object, 0x48) + 0x278));
            m.write(last + 0xe4, m.read8(p.vcall0(object, 0x48) + 0x28c));
            m.write(last + 0xec, m.read(p.vcall0(object, 0x48) + 0x260));
            m.write(last + 0xf0, m.read(p.vcall0(object, 0x48) + 0xf8));
        } else if (t == kTrigger) {
            if (m.read(p.vcall0(object, 0x40) + 0x2bc) != 0) {
                m.write(last + 0xe8, m.read(p.vcall0(object, 0x40) + 0x2b8));
                m.write(last + 0xe4, m.read(p.vcall0(object, 0x40) + 0x2c8));
            }
        }
    }
    if (m.read8(object + 8) == kPlaceable && (flags & 1u) != 0) {
        const Word pl = p.vcall0(object, 0x48);
        m.write(last + 0x148, m.read(pl + 0x204));
        m.write(last + 0x150, m.read(pl + 0x20c));
        m.write(last + 0x14c, m.read(pl + 0x208));
    }
    if (m.read8(object + 8) != kCreature) return;
    const Word c = p.vcall0(object, 0x30);
    if (anim) w8(p, last + 0x60, m.read8(m.read(c + 0x340) + 0x28));
    if ((flags & 0x40u) != 0) {
        m.write16(last + 0x62, m.read16(c + 0x9f0));
        w8(p, last + 0x64, m.read8(c + 0x9f2));
        m.write16(last + 0x66, m.read16(c + 0x9fc));
        m.write(last + 0x70, m.read(c + 0x9f4));
        m.write(last + 0x68, m.read(c + 0x4e0));
        w8(p, last + 0x6c, m.read8(c + 0x4d1));
        Word rep = 2;
        if (player != 0 && p.game_object_00561080(player) != 0 && p.vcall0(p.game_object_00561080(player), 0x30) != 0) {
            const Word go = p.game_object_00561080(player);
            const Word id = m.read(c + 4);
            rep = p.ai_state_reputation_0057cd90(p.vcall0(go, 0x30), id);
        }
        w8(p, last + 0xf4, rep);
    }
    if (anim && p.animation_stationary_004cae60(c, m.read16(last + 0x4c)) == 0) {
        const Word ai = m.read(c + 0x340);
        const std::int32_t n = static_cast<std::int32_t>(m.read(ai + 0x8c));
        const Word half = static_cast<Word>(n / 2);                    // CDQ / SUB / SAR: toward zero
        const Word points = m.read(ai + 0x90);
        if (points != 0 && (half & 0xffffu) != 0) {
            const Word at = points + (half & 0xffffu) * 8;
            m.write(last + 0x54, m.read(at - 8));
            m.write(last + 0x58, msg::x87_single_roundtrip(m.read(at - 4)));
            m.write(last + 0x5c, 0);
        }
    }
    if ((flags & 0x100u) != 0) {
        m.write(last + 0xa0, p.walk_rate_bits_004f1b20(c));
        m.write(last + 0xa4, p.run_rate_bits_004f1be0(c));
    }
    if ((flags & 0x800u) != 0) w8(p, last + 0xb5, p.damage_level_004cb020(c));
    if ((flags & 0x400u) != 0) {
        m.write16(last + 0xba, p.vcall1(c, 0x9c, 1) & 0xffffu);
        m.write16(last + 0xbc, m.read16(c + 0xe0));
        m.write16(last + 0xbe, m.read16(c + 0xe4));
        m.write16(last + 0xc0, p.vcall1(c, 0x98, 1) & 0xffffu);
    }
    if ((flags & 0x80000u) != 0) {
        m.write16(last + 0x130, m.read16(m.read(c + 0xa74) + 0x124));
        m.write16(last + 0x132, m.read16(m.read(c + 0xa74) + 0x122));
        m.write16(last + 0x134, m.read16(m.read(c + 0xa74) + 0x126));
        m.write16(last + 0x136, p.max_force_points_004fd490(c) & 0xffffu);
    }
    if ((flags & 0x4000u) != 0) {
        m.write(last + 0x120, m.read(c + 0x9d4));
        m.write(last + 0xc4, (m.read8(c + 0x9f0) & 0x40u) != 0 ? 1u : 0u);
        m.write(last + 0xcc, p.singleton_party_005bf690(p.faction_00513fc0(c)));
        const Word id = m.read(c + 4);
        m.write(last + 0xc8, p.faction_leader_005bf5c0(p.faction_00513fc0(c)) == id ? 1u : 0u);
        m.write(last + 0xd0, m.read(c + 0x930));
    }
    if ((flags & 0x8000u) != 0) {
        const Word pc = p.vcall0(p.game_object_00561080(player), 0x30);
        if (pc != 0) {
            if (m.read(pc + 4) == m.read(c + 4)) {
                const Word module = p.module_004ae6b0(m.read(p.app_007a39fc() + 8));
                if (module != 0) m.write(last + 0x100, p.array_count_004af240(module + 0x13c, m.read(c + 4)));
            } else {
                const Word e = p.visible_list_element_00517a20(c, m.read(pc + 4));
                if (e != 0) {
                    m.write(last + 0xfc, (m.read8(e + 4) >> 1) & 1u);
                    m.write(last + 0xf8, m.read8(e + 4) & 1u);
                    m.write(last + 0x100, (m.read8(e + 4) >> 4) & 1u);
                } else {
                    m.write(last + 0xfc, 0); m.write(last + 0xf8, 0); m.write(last + 0x100, 0);
                }
            }
        }
    }
    if ((flags & 0x20000u) != 0) w8(p, last + 0xb4, associate_state_005676b0(m, p, self, c));
    if ((flags & 0x1000u) != 0) {
        m.write16(last + 0x104, m.read16(m.read(c + 0xa74) + 0xdc));
        p.str_assign_005e5c50(last + 0x108, m.read(c + 0xa74) + 0xe0);
        w8(p, last + 0x110, m.read8(m.read(c + 0xa74) + 0xe8));
        w8(p, last + 0x111, m.read8(m.read(c + 0xa74) + 0x64));
        m.write16(last + 0x118, m.read16(m.read(c + 0xa74) + 0x17e));
        m.write(last + 0x11c, m.read(c + 0xf4));
        for (Word i = 0; i < m.read8(m.read(c + 0xa74) + 0x89); ++i) {
            w8(p, last + 0x112 + i, p.creature_class_005a4e90(m.read(c + 0xa74), i));   // overlaps level bytes (original)
            w8(p, last + 0x114 + i, p.class_level_005a5090(m.read(c + 0xa74), i, 0));
        }
    }
    if ((flags & 0x100000u) != 0) {
        m.write(last + 0x148, m.read(c + 0x204));
        m.write(last + 0x150, m.read(c + 0x20c));
        m.write(last + 0x14c, m.read(c + 0x208));
    }
}

void update_last_appearance_00568480(Memory& m, LastPorts& p, Word object, Word last, Word flags) {
    if (m.read8(object + 8) != kCreature) return;
    const Word c = p.vcall0(object, 0x30);
    if ((flags & 0x400u) != 0) {
        p.loc_assign_005e9e70(last + 0x90, m.read(c + 0xa74) + 0x34);
        p.loc_assign_005e9e70(last + 0x98, m.read(c + 0xa74) + 0x3c);
    }
    m.write(last, m.read(c + 0xa50));
    m.write(last + 4, m.read(c + 0xa54));
    m.write(last + 8, m.read(c + 0xa58));
    m.write(last + 0x0c, m.read(c + 0xa5c));
    m.write16(last + 0x10, m.read16(c + 0xa60));
    for (Word k = 0x12; k <= 0x1a; ++k) w8(p, last + k, m.read8(c + 0xa50 + k));
    m.write(last + 0x1c, m.read(c + 0xa6c));
    w8(p, last + 0x20, m.read8(c + 0xa70));
}
}
