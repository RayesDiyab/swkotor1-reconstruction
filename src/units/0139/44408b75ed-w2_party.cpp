// W2 tier 3 logic: CreateParty / PositionParty and callees, sub-area predicates. See w2_party.hpp.
// Comments give the original instruction address where a branch or quirk is decided.
#include "669c8a7e66-w2_party.hpp"
#include <cmath>
#include "28478d5d50-w2_ftol.hpp"
#include <cstring>
#include <initializer_list>
namespace w2::party {
namespace {
using I = std::int32_t;
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
Word bits(float v) { Word b; std::memcpy(&b, &v, 4); return b; }
double fd(Memory& m, Word a) { return static_cast<double>(f32(m.read(a))); }
float fs(double v) { return static_cast<float>(v); }                                   // FSTP m32
void store(Memory& m, Word a, double v) { m.write(a, bits(fs(v))); }
void copy3(Memory& m, Word d, Word s) { m.write(d, m.read(s)); m.write(d + 4, m.read(s + 4)); m.write(d + 8, m.read(s + 8)); }
void zero3(Memory& m, Word d) { m.write(d, 0); m.write(d + 4, 0); m.write(d + 8, 0); }
Word ftol2_low(double v) { return w2::ftol2_006fae8c(v); }   // __ftol2 006FAE8C (existing provider natively)
constexpr Word kInvalid = 0x7f000000u;
constexpr Word kRadius10 = 0x41200000u, kRadius5 = 0x40a00000u, kRadius20 = 0x41a00000u, kOne = 0x3f800000u;
constexpr float kPercentScale = 0.0099999998f;   // .rdata 0073DFEC
constexpr float kHundred = 100.0f;               // .rdata 00741838
constexpr float kFour = 4.0f;                    // .rdata 0073F404
}

// ===================================================================== repositories / lists
void clear_items_0055f330(Memory& m, Word r) { m.write(r + 0x10, 0); m.write(r + 4, 0); }

Word find_item_with_tag_00555ed0(Memory& m, Ports& p, Word r, Word tag) {
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(r + 0x10)); ++i) {
        const Word item = p.item_by_id_004ae760(server_of(m, p), m.read(m.read(r + 0x0c) + 4 * i));
        if (item == 0) continue;
        if (p.string_equal_005e5200(item + 0x28, tag) != 0) return m.read(item + 0x14);
        if (m.read(item + 0x26c) != 0) {
            const Word id = find_item_with_tag_00555ed0(m, p, m.read(item + 0x26c), tag);
            if (id != kInvalid) return id;
        }
    }
    return kInvalid;
}

Word item_in_repository_00555f40(Memory& m, Ports& p, Word r, Word item, Word recurse) {
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(r + 0x10)); ++i) {
        const Word it = p.item_by_id_004ae760(server_of(m, p), m.read(m.read(r + 0x0c) + 4 * i));
        if (item == it) return 1;
        if (recurse == 0 || it == 0) continue;
        const Word base = p.base_item_005b31a0(m.read(p.rules_007a3a28() + 0x34), m.read(it + 0x0c));
        if (base == 0 || m.read(base + 0x70) == 0 || m.read(it + 0x26c) == 0) continue;
        if (item_in_repository_00555f40(m, p, m.read(it + 0x26c), item, recurse) != 0) return 1;
    }
    return 0;
}

Word remove_item_00555fd0(Memory& m, Word r, Word item) {
    if (item == 0 || static_cast<I>(m.read(r + 0x10)) <= 0) return 0;
    const Word id = m.read(item + 0x14);
    Word at = 0;
    while (m.read(m.read(r + 0x0c) + 4 * at) != id) {
        ++at;
        if (static_cast<I>(at) >= static_cast<I>(m.read(r + 0x10))) return 0;
    }
    m.write(r + 0x10, m.read(r + 0x10) - 1);
    for (; static_cast<I>(at) < static_cast<I>(m.read(r + 0x10)); ++at)
        m.write(m.read(r + 0x0c) + 4 * at, m.read(m.read(r + 0x0c) + 4 * at + 4));
    if ((m.read8(item + 0x288) & 0x80u) != 0) {                      // 00556028
        m.write(r + 4, m.read(r + 4) - 1);
        m.write(item + 0x288, m.read(item + 0x288) & 0xffffff7fu);
    }
    return 1;
}

// 0066DCC0 CExoArrayList<int>::remove is W1-owned (recovered_int_list_remove_0066dcc0, lane-w1 tier4): Ports.

// ===================================================================== server thunks
Word load_from_save_game_004af050(Memory& m, Word server) { return m.read(m.read(server + 4) + 0x100c4); }
Word object_array_004aed70(Memory& m, Word server) { return m.read(m.read(server + 4) + 0x1005c); }

namespace {
// shared head of 004B4D70 / 004B4DF0 / 004B1970: lookup, status == [0074666C] (0), type byte compare
Word typed_object(Memory& m, Ports& p, Word internal, Word id, Word slot, Word type) {
    m.write(slot, 0);
    if ((p.object_lookup_004d8230(m.read(internal + 0x1005c), id, slot) & 0xffu) != 0) return 0;
    const Word object = m.read(slot);
    if (object == 0 || m.read8(object + 8) != type) return 0;
    return object;
}
}
Word test_safe_location_004b4d70(Memory& m, Ports& p, Word internal, Word id, Word position, Word slot) {
    const Word object = typed_object(m, p, internal, id, slot, 5);   // .rdata 00746634 = 5 (creature)
    if (object == 0) return 0;
    const Word creature = p.vcall0(object, 0x30);
    if (creature == 0) return 0;
    const Word area = p.area_by_id_internal_004b17e0(internal, m.read(creature + 0x8c));
    return p.area_safe_point_004be5e0(area, position, m.read(creature + 0x340));
}
Word compute_safe_location_004b4df0(Memory& m, Ports& p, Word internal, Word position, Word radius, Word id, Word flag, Word out, Word slot) {
    const Word object = typed_object(m, p, internal, id, slot, 5);
    if (object == 0) return 0;
    const Word creature = p.vcall0(object, 0x30);
    if (creature == 0) return 0;
    const Word area = p.area_by_id_internal_004b17e0(internal, m.read(creature + 0x8c));
    return p.area_safe_location_004be860(area, position, radius, m.read(creature + 0x340), flag, out);
}
Word waypoint_by_id_004b19c0(Memory& m, Ports& p, Word internal, Word id, Word slot) {
    const Word object = typed_object(m, p, internal, id, slot, 0x0c);   // .rdata 0074663B = 0Ch (waypoint)
    return object == 0 ? 0 : p.vcall0(object, 0x58);
}
Word aoe_by_id_004b1970(Memory& m, Ports& p, Word internal, Word id, Word slot) {
    const Word object = typed_object(m, p, internal, id, slot, 0x0b);   // .rdata 0074663A = 0Bh
    return object == 0 ? 0 : p.vcall0(object, 0x50);
}

void aoe_jump_to_point_00596360(Memory& m, Ports& p, Word aoe, Word area, Word position, Word scratch) {
    (void)scratch;
    const Word creature = p.creature_by_id_004ae770(server_of(m, p), m.read(aoe + 0x24c));
    if (creature == 0) return;
    if (p.object_area_004cb120(aoe) != area) return;
    p.int_list_remove_0066dcc0(creature + 0x330, m.read(aoe + 4));
    p.aoe_remove_from_subareas_00595b60(aoe, 0);
    p.set_position_004cd1a0(aoe, position, 1, 1, 0);
    p.list_add_unique_006449d0(creature + 0x330, m.read(aoe + 4));
    p.aoe_update_subareas_00595640(aoe, 0);
}

// ===================================================================== client party
// 0060EF50 CSWCCreature::SetInParty and 00634CC0 GetPathData are W4-owned (t3 review): reached through Ports.

void party_reset_00637630(Memory& m, Ports& p, Word cparty, Word position, Word yaw, Word flag) {
    (void)m;
    p.follow_array_reset_00634b60(cparty + 0x2dc, position, yaw);
    if (flag != 0) p.party_reset_formation_0063765e(cparty, position, yaw);   // 00637652..0063787B (closeA)
}

void reset_all_follow_data_00637890(Memory& m, Ports& p, Word cp, Word pos, Word yaw, Word flag, Word s) {
    party_reset_00637630(m, p, cp, pos, yaw, flag);
    for (Word base : {0x00u, 0x88u, 0x110u}) {                      // three CSWPartyMember follow blocks
        m.write(cp + base + 0x28, 0xffffffffu);
        copy3(m, cp + base + 0x3c, pos); m.write(cp + base + 0x48, 0);
        copy3(m, cp + base + 0x4c, pos); m.write(cp + base + 0x58, 0);
    }
    for (Word k = 0; k < 3; ++k) {
        const Word server = server_of(m, p);
        const Word sid = p.client_to_server_id_004aea30(server, m.read(cp + 0x24 + 0x88 * k));
        const Word c = p.creature_by_id_004ae770(server, sid);
        if (c == 0) continue;
        m.write(m.read(c + 0x340) + 0x210, 0);
        m.write(m.read(c + 0x340) + 0x34, kInvalid);
        if (test_safe_location_004b4d70(m, p, m.read(server_of(m, p) + 4), m.read(c + 4), c + 0x90, s + 0x10) != 0) continue;
        zero3(m, s);
        (void)compute_safe_location_004b4df0(m, p, m.read(server_of(m, p) + 4), c + 0x90, kRadius5, m.read(c + 4), 1, s, s + 0x10);
        p.set_position_004cd1a0(c, s, 1, 1, 0);
    }
}


// ===================================================================== party table
// 00563A40 (labelled CSWPartyTable::GetIsLeader) (id) RET 4: +EC := -1, then := slot index of id (9 slots at +0C);
// EAX is the slot index, 9 when absent. For id 7F000000h the original jumps 00563A55 -> 00563A77 and returns
// WITHOUT setting EAX (EAX keeps the caller's value). The only caller (006357D7) never reads EAX after the call
// (next: mov ecx,[esp+18h]; push esi; call 00561C40), so W2 returns 9 there as a placeholder; +EC is FFFFFFFFh.
Word party_leader_slot_00563a40(Memory& m, Word party, Word id) {
    m.write(party + 0xec, 0xffffffffu);
    if (id == 0x7f000000u) return 9;
    for (Word k = 0; k < 9; ++k) if (m.read(party + 0x0c + 4 * k) == id) { m.write(party + 0xec, k); return k; }
    return 9;
}
// 00563790 CSWPartyTable::GetIsAvailable(id) RET 4: 1 when id is in the nine slots at +0C..+2C.
Word party_is_member_00563790(Memory& m, Word party, Word id) {
    for (Word k = 0; k < 9; ++k) if (m.read(party + 0x0c + 4 * k) == id) return 1;
    return 0;
}
void unstealth_party_00563c60(Memory& m, Ports& p, Word party, Word extra) {
    const Word server = server_of(m, p);
    const Word client_object = p.client_object_by_id_004ae910(server, p.player_creature_id_004aea40(server));
    if (client_object != 0) {
        const Word c = p.creature_by_id_004ae770(server_of(m, p), m.read(client_object + 0x38));
        if (c != 0) p.set_activity_004f00a0(c, 1, 0, extra);
    }
    if (static_cast<I>(m.read(party)) <= 0) return;
    std::uint8_t i = 0;                                               // BL
    do {
        const Word npc = m.read(party + 4 + 4u * i);
        const Word c = p.creature_by_id_004ae770(server_of(m, p), m.read(party + 0x0c + 4 * npc));
        if (c != 0) p.set_activity_004f00a0(c, 1, 0, extra);
        ++i;
    } while (static_cast<I>(i) < static_cast<I>(m.read(party)));
}

void transfer_inventory_005641e0(Memory& m, Ports& p, Word party, Word c, Word slot) {
    if (c == 0) return;
    const Word saved = m.read(c + 0xa88);
    p.set_in_party_004fdb20(c, 0, 0);
    m.write(party + 0xfc, m.read(party + 0xfc) + p.gold_004edd60(c));
    const Word repo = p.item_repository_004ef770(c, 1);
    p.set_in_party_004fdb20(c, saved, 0);
    if (m.read(party + 0x118) == 0) {
        const Word mem = p.allocate_006fa7e6(0x18);
        m.write(party + 0x118, mem != 0 ? p.repository_construct_0055d290(mem, p.player_creature_id_004aea40(server_of(m, p)), 1) : 0);
    }
    for (Word i = 0; i < m.read(repo + 0x10); ++i) {                 // unsigned (JB)
        const Word item = p.repository_item_005560b0(repo, i);
        m.write(slot, item);
        if (item == 0) continue;
        p.set_possessor_00553210(item, 0x7fffffffu, 0, 0, kInvalid);  // [0074664C]
        (void)p.repository_add_0055d330(m.read(party + 0x118), slot, 1, 0, 1);
    }
    clear_items_0055f330(m, repo);
}

Word filename_00563620(Memory& m, Ports& p, Word party, Word out, Word index, Word s) {
    (void)party; (void)m;
    // CExoString::Format("AVAILNPC%d", index) into a temporary, copy-constructed into out (the temporary's
    // allocation is not modelled: out receives the same text through the char* constructor).
    char text[24];
    std::memcpy(text, "AVAILNPC", 8);
    const std::int64_t v = static_cast<I>(index);
    std::uint64_t u = v < 0 ? static_cast<std::uint64_t>(-v) : static_cast<std::uint64_t>(v);
    char digits[12]; int n = 0;
    do { digits[n++] = static_cast<char>('0' + u % 10); u /= 10; } while (u != 0);
    int at = 8;
    if (v < 0) text[at++] = '-';
    while (n > 0) text[at++] = digits[--n];
    text[at++] = 0;
    for (int k = 0; k < at; ++k) p.write8(s + static_cast<Word>(k), static_cast<unsigned char>(text[k]));
    p.str_literal_005e5a90(out, s);
    return out;
}

Word npc_object_00564700(Memory& m, Ports& p, Word party, Word index, Word create, Word apply, Word s) {
    if (static_cast<I>(index) < 0 || static_cast<I>(index) >= 9) return kInvalid;
    if (m.read(party + 0x30 + 4 * index) == 0) return kInvalid;
    const Word existing = m.read(party + 0x0c + 4 * index);
    if (existing != kInvalid) return existing;
    if (create == 0) return kInvalid;
    const Word mem = p.allocate_006fa7e6(0xac8);
    const Word c = mem != 0 ? p.creature_construct_004f7a10(mem, kInvalid, 0) : 0;
    p.add_game_in_progress_005638d0(party);
    const Word name = s + 0x10, ref = s + 0x18;
    (void)filename_00563620(m, p, party, name, index, s + 0x30);
    p.ref_from_string_00406d60(ref, name);
    if (c == 0) p.trap(0x005647c2, "W2: original calls LoadFromTemplate on a null creature (allocation failed)");
    const bool failed = p.creature_template_005026d0(c, ref, 0) == 0;
    p.str_destroy_005e5c20(name);
    if (failed) {                                                     // 005647E3: game-in-progress left mounted (original)
        p.vdelete(c);
        return kInvalid;
    }
    p.remove_game_in_progress_00563950(party);
    if (apply != 0 && static_cast<I>(p.vcall1_short(c, 0x9c, 0)) <= 0) {   // 00564825: TEST AX,AX / JG
        m.write(c + 0xf0, 1);
        const Word emem = p.allocate_006fa7e6(0x8c);
        const Word e = emem != 0 ? p.effect_construct_00503e40(emem, 1) : 0;
        if (e == 0) p.trap(0x0056485e, "W2: original writes the effect type through a null effect (allocation failed)");
        m.write16(e + 8, 4);                                          // [0074668C]
        m.write16(e + 0x0a, (m.read16(e + 0x0a) & 0xfff8u) | 0);      // [007473B8] = 0
        p.apply_effect_004d14f0(c, e, 0, 0);
    }
    m.write(party + 0x0c + 4 * index, m.read(c + 4));
    return m.read(c + 4);
}

Word spawn_npc_00565130(Memory& m, Ports& p, Word party, Word index, Word use_given, Word position, Word orientation, Word flag, Word s) {
    const Word server = server_of(m, p);
    const Word id = npc_object_00564700(m, p, party, index, 1, flag, s + 0x60);
    const Word c = p.creature_by_id_004ae770(server, id);
    if (c == 0) return id;
    const Word saved = m.read(c + 0xe8);
    m.write(c + 0xe8, 1);
    p.clear_all_actions_004ccd80(c, 1);
    m.write(c + 0xe8, saved);
    const Word area = p.module_area_004c30f0(p.module_004ae6b0(server));
    const Word pos = s + 0x1c, out = s + 0x28, orient = s + 0x34, str = s + 0x14, percent = s + 0x50;
    zero3(m, out);
    if (use_given != 0) { copy3(m, pos, position); copy3(m, orient, orientation); }
    else { copy3(m, pos, c + 0x90); copy3(m, orient, c + 0x9c); }
    if (p.area_room_004bb600(area, pos, 0) != 0)
        (void)p.area_safe_location_004be860(area, pos, kRadius20, m.read(c + 0x340), 1, out);
    p.add_to_area_004fa100(c, area, m.read(out), m.read(out + 4), m.read(out + 8), 1, 0);   // (0,0,0) when no room (original)
    p.set_orientation_004cd170(c, orient);
    p.str_literal_005e5a90(str, p.literal(0x00746f08));               // "PercentXP"
    m.write(percent, index);                                          // out slot is the index argument slot (original)
    const Word table = m.read(m.read(p.rules_007a3a28() + 0xb8) + 0xf0);
    const Word found = p.c2da_int_00413660(table, index, str, percent);
    p.str_destroy_005e5c20(str);
    const I pct = static_cast<I>(m.read(percent));
    const double mult = (found != 0 && pct != 0) ? static_cast<double>(pct) * static_cast<double>(kPercentScale) : 1.0;
    const I party_xp = static_cast<I>(m.read(party + 0xf8));
    const I diff = static_cast<I>(m.read(m.read(c + 0xa74) + 0x68) - m.read(c + 0x22c));
    const Word target = ftol2_low(static_cast<double>(party_xp) * mult);
    if (diff >= static_cast<I>(target)) return id;                    // 0056532F (signed JGE)
    // FIDIV by the raw slot; a zero divisor gives +-inf/NaN under masked x87 exceptions -> __ftol2 low dword 0
    const Word q = pct == 0 ? 0u : ftol2_low(static_cast<double>(diff) / static_cast<double>(pct) * static_cast<double>(kHundred));
    p.receive_experience_004ef930(c, static_cast<Word>(party_xp) - q);
    if ((m.read8(p.client_options_005ed700(client_of(m, p)) + 8) & 1u) == 0) return id;
    if (p.can_level_up_005a6810(m.read(c + 0xa74)) == 0) return id;
    p.auto_levelup_005b27e0(m.read(c + 0xa74), 1);
    return id;
}

Word load_item_00560970(Memory& m, Ports& p, Word item, Word gff, Word st, Word s) {
    const Word status = s + 0x0c, fallback = s + 0x10, ref1 = s + 0x20, ref2 = s + 0x30, status2 = s + 0x48;
    p.ref_construct_00406d80(fallback, p.literal(0x0073d71c));
    m.write(status, 0);
    (void)p.gff_resref_00411e10(gff, ref1, st, p.literal(0x00747228), status, fallback);   // "EquippedRes"
    Word ref = ref1;
    if (m.read(status) == 0) {
        p.ref_construct_00406d80(fallback, p.literal(0x0073d71c));
        m.write(status, 0);
        (void)p.gff_resref_00411e10(gff, ref2, st, p.literal(0x00747200), status, fallback);  // "InventoryRes"
        if (m.read(status) == 0) {                                    // 00560B16
            p.item_fields_0055fcd0(item, gff, st);
            return 1;
        }
        ref = ref2;
    }
    m.write(status, p.item_template_005608b0(item, ref));            // result kept in the status slot
    const Word drop = p.gff_byte_00411a60(gff, st, p.literal(0x0074721c), status2, 0) & 0xffu;   // "Dropable"
    if (m.read(status2) != 0) m.write(item + 0x288, m.read(item + 0x288) ^ (((drop << 3) ^ m.read(item + 0x288)) & 8u));
    const Word pick = p.gff_byte_00411a60(gff, st, p.literal(0x00748fbc), status2, 0) & 0xffu;   // "Pickpocketable"
    if (m.read(status2) != 0) m.write(item + 0x288, m.read(item + 0x288) ^ (((pick << 4) ^ m.read(item + 0x288)) & 0x10u));
    return m.read(status);
}

void force_equip_clothing_00501c40(Memory& m, Ports& p, Word c, Word s) {
    if (m.read(c + 0xab8) == 0) return;
    const Word repo = p.item_repository_004ef770(c, 1);
    const Word str = s, ref = s + 0x10;
    p.str_literal_005e5a90(str, p.literal(0x00747324));               // "g_a_clothes01"
    const Word id = find_item_with_tag_00555ed0(m, p, repo, str);
    p.str_destroy_005e5c20(str);
    Word item = p.item_by_id_004ae760(server_of(m, p), id);
    if (item == 0) {
        const Word mem = p.allocate_006fa7e6(0x29c);
        item = mem != 0 ? p.item_construct_005530a0(mem, kInvalid) : 0;
        if (item == 0) { m.write(c + 0xab8, 0); return; }
        p.ref_construct_00406d80(ref, p.literal(0x00747324));
        if (p.item_template_005608b0(item, ref) == 0) { p.vdelete(item); m.write(c + 0xab8, 0); return; }
        m.write(item + 0x288, m.read(item + 0x288) | 8u);
        p.set_possessor_00553210(item, m.read(c + 4), 0, 0, kInvalid);
    }
    if (item_in_repository_00555f40(m, p, repo, item, 0) != 0) (void)remove_item_00555fd0(m, repo, item);
    p.equip_item_004feb90(c, 2, item, 1, 0, 0);
    m.write(c + 0xab8, 0);
}

void create_party_00565760(Memory& m, Ports& p, Word party, Word s) {
    if (m.read(party + 0x110) != 0) return;
    p.add_game_in_progress_005638d0(party);
    const Word client = client_of(m, p), server = server_of(m, p);
    (void)p.sw_party_005ed8b0(client);                                // result unused (original)
    if (m.read(party + 0xf0) == 0xffffffffu) {
        if (m.read(party) != 0) {                                     // JBE: unsigned
            zero3(m, s + 0x24); zero3(m, s + 0x30);
            for (Word i = 0; i < m.read(party); ++i) {                // JB: unsigned
                const Word not_loaded = load_from_save_game_004af050(m, server_of(m, p)) == 0 ? 1u : 0u;
                const Word id = spawn_npc_00565130(m, p, party, m.read(party + 4 + 4 * i), 0, s + 0x30, s + 0x24, not_loaded, s + 0x60);
                const Word cc = p.client_creature_005ed5b0(client, p.server_to_client_id_005eda50(client, id));
                const Word sc = p.creature_by_id_004ae770(server, id);
                if (cc != 0) (void)p.client_set_in_party_0060ef50(cc, 1);
                if (sc != 0) p.set_in_party_004fdb20(sc, 1, 1);
            }
        }
    } else {
        m.write(party + 0x0c + 4 * m.read(party + 0xf0), p.player_creature_id_004aea40(server));
    }
    const Word mem = p.allocate_006fa7e6(0xa0);
    Word gff = 0;
    if (mem != 0) {
        p.ref_construct_00406d80(s + 0x30, p.literal(0x00749320));    // "INVENTORY"
        gff = p.gff_construct_00410630(mem, 0, p.literal(0x007490dc), s + 0x30);   // "INV "
    }
    if (gff == 0) p.trap(0x005658de, "W2: original reads a property of a null CResGFF (allocation failed)");
    if (p.gff_loaded_004064a0(gff) != 0) {
        if (m.read(party + 0x118) == 0) {
            const Word rmem = p.allocate_006fa7e6(0x18);
            m.write(party + 0x118, rmem != 0 ? p.repository_construct_0055d290(rmem, p.player_creature_id_004aea40(server), 1) : 0);
        } else {
            clear_items_0055f330(m, m.read(party + 0x118));
            m.write(party + 0x110, 0);
        }
        p.gff_top_00411240(gff, s + 0x1c);
        (void)p.gff_list_004118c0(gff, s + 0x40, s + 0x1c, p.literal(0x00747210));   // "ItemList"
        for (Word i = 0; i < p.gff_list_count_00411940(gff, s + 0x40); ++i) {        // count re-read (original)
            (void)p.gff_element_00411990(gff, s + 0x20, s + 0x40, i);
            m.write(s + 0x14, 0);
            const Word imem = p.allocate_006fa7e6(0x29c);
            const Word item = imem != 0 ? p.item_construct_005530a0(imem, kInvalid) : 0;
            m.write(s + 0x14, item);
            if (item == 0) continue;
            if (load_item_00560970(m, p, item, gff, s + 0x20, s + 0x60) == 0) {
                if (m.read(s + 0x14) != 0) p.vdelete(m.read(s + 0x14));
                continue;
            }
            if (p.repository_add_0055d330(m.read(party + 0x118), s + 0x14, 1, 0, 0) == 0) continue;
            if (m.read(s + 0x14) == 0) continue;
            p.set_possessor_00553210(m.read(s + 0x14), p.player_creature_id_004aea40(server), 0, 0, kInvalid);
        }
    }
    p.vdelete(gff);
    p.remove_game_in_progress_00563950(party);
    const Word pc = p.creature_by_id_004ae770(server, p.player_creature_id_004aea40(server));
    if (pc != 0) transfer_inventory_005641e0(m, p, party, pc, s + 0x5c);
    for (Word k = 0; k < 9; ++k) {
        const Word c = p.creature_by_id_004ae770(server, m.read(party + 0x0c + 4 * k));
        if (c != 0) transfer_inventory_005641e0(m, p, party, c, s + 0x5c);
    }
    const Word pc2 = p.creature_by_id_004ae770(server, p.player_creature_id_004aea40(server));
    if (pc2 != 0) force_equip_clothing_00501c40(m, p, pc2, s + 0x60);
    for (Word k = 0; k < 9; ++k) {
        const Word c = p.creature_by_id_004ae770(server, m.read(party + 0x0c + 4 * k));
        if (c != 0) force_equip_clothing_00501c40(m, p, c, s + 0x60);
    }
    m.write(party + 0x110, 1);
}

void position_party_00565b00(Memory& m, Ports& p, Word party, Word s) {
    const Word cp = p.sw_party_005ed8b0(client_of(m, p));
    const Word server = server_of(m, p);
    m.write(s + 0x1c, server);
    const Word pc = p.creature_by_id_004ae770(server, p.player_creature_id_004aea40(server));
    if (pc == 0) p.trap(0x00565b36, "W2: original dereferences a null player creature in PositionParty");
    copy3(m, s + 0x50, pc + 0x9c);
    const Word yaw = p.yaw_bits_004aa0f0(s + 0x50);
    reset_all_follow_data_00637890(m, p, cp, pc + 0x90, yaw, 0, s + 0xe0);
    if (m.read(party + 0x10c) != 0) { m.write(party + 0x10c, 0); return; }   // 00565B74
    zero3(m, s + 0x20);
    if (static_cast<I>(m.read(party)) > 0) {
        const Word follow = cp + 0x2dc;
        m.write(s + 0x14, follow);
        m.write(s + 0x2c, 0); m.write(s + 0x30, 0); m.write(s + 0x34, kOne);   // axis (0,0,1)
        Word e = cp + 0xd4, npc = party + 4;
        for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(party)); ++i, e += 0x88, npc += 4) {
            copy3(m, s + 0x5c, e + 0x10);
            copy3(m, s + 0x38, e);
            const Word t = m.read(p.path_data_00634cc0(follow, m.read(e + 0x0c)) + 0x0c);
            m.write(s + 0x18, t);
            p.rotate_about_axis(s + 0x68, s + 0x2c, p.deg_to_rad_bits_004a96d0(t), s + 0x5c);
            store(m, s + 0x44, fd(m, s + 0x38) + fd(m, s + 0x68));
            store(m, s + 0x48, fd(m, s + 0x3c) + fd(m, s + 0x6c));
            store(m, s + 0x4c, fd(m, s + 0x40) + fd(m, s + 0x70));
            m.write(s + 0x20, m.read(s + 0x44)); m.write(s + 0x24, m.read(s + 0x48)); m.write(s + 0x28, m.read(s + 0x4c));
            const Word id = npc_object_00564700(m, p, party, m.read(npc), 0, 1, s + 0x80);
            (void)compute_safe_location_004b4df0(m, p, m.read(server_of(m, p) + 4), s + 0x20, kRadius10, id, 0, s + 0x20, s + 0xd8);
            const Word c = p.creature_by_id_004ae770(m.read(s + 0x1c), id);
            if (c == 0) p.trap(0x00565cbb, "W2: original calls SetPosition on a null NPC creature in PositionParty");
            p.set_position_004cd1a0(c, s + 0x20, 1, 1, 0);
            copy3(m, e + 0x34, s + 0x20);
            p.set_orientation_004cd170(c, s + 0x50);
        }
    }
    unstealth_party_00563c60(m, p, party, 0);
    m.write(party + 0x190, 0);
    m.write(party + 0x10c, 0);
}

// ===================================================================== sub-area predicates
Word aoe_position_005946e0(Memory& m, Ports& p, Word aoe, Word out, Word slot) {
    const Word id = m.read(aoe + 0x24c);
    bool linked = id != kInvalid && m.read8(aoe + 0x230) == 0;       // [00746644] = 7F000000h
    if (linked) {
        m.write(slot, 0);
        linked = (p.object_lookup_004d8230(object_array_004aed70(m, server_of(m, p)), id, slot) & 0xffu) == 0 && m.read(slot) != 0;
    }
    if (!linked) p.trap(0x0059474e, "W2: original GetPosition calls itself without bound (unlinked or polygon AOE)");
    const Word c = p.vcall0(m.read(slot), 0x30);
    if (c == 0) p.trap(0x0059472c, "W2: original reads the position of a null linked object");
    copy3(m, out, c + 0x90);
    return out;
}

Word in_area_of_effect_00595450(Memory& m, Ports& p, Word aoe, Word x, Word y, Word z, Word s) {
    const Word pos = aoe_position_005946e0(m, p, aoe, s, s + 0x3c);
    const double px = f32(x), py = f32(y), pz = f32(z);
    const double dx = px - fd(m, pos), dy = py - fd(m, pos + 4), dz = pz - fd(m, pos + 8);
    const double r = fd(m, aoe + 0x238);
    const double sum = (dz * dz + dy * dy) + dx * dx;
    if (!(r * r >= sum)) return 0;                                    // 00595499: C0 (less or unordered) -> outside
    if (m.read8(aoe + 0x230) == 0) return 1;
    const Word a = s + 0x0c, b = s + 0x18, out = s + 0x24;
    for (Word i = 0; i < 4; ++i) {
        const Word j = i == 3 ? 0 : i + 1;
        const Word vi = m.read(aoe + 0x244) + 12 * i, vj = m.read(aoe + 0x244) + 12 * j;
        store(m, a, px - fd(m, vi)); store(m, a + 4, py - fd(m, vi + 4)); store(m, a + 8, pz - fd(m, vi + 8));
        store(m, b, fd(m, vj) - fd(m, vi)); store(m, b + 4, fd(m, vj + 4) - fd(m, vi + 4)); store(m, b + 8, fd(m, vj + 8) - fd(m, vi + 8));
        p.vector_cross_004ab210(out, b, a);
        if (fd(m, out + 8) > 0.0) return 0;                            // 00595534: [0073D700] = 0.0
    }
    return 1;
}

Word aoe_segment_005950c0(Memory& m, Ports& p, Word aoe, const Word (&a)[3], const Word (&b)[3], Word s) {
    const double ax = f32(a[0]), ay = f32(a[1]), bx = f32(b[0]), by = f32(b[1]);
    if (m.read8(aoe + 0x230) != 0) {                                 // 005953B8 polygon
        const Word area = p.object_area_004cb120(aoe);
        if (area == 0) return 0;
        for (Word i = 0; i < 4; ++i) {
            const Word j = i == 3 ? 0 : i + 1;
            const Word vi = m.read(aoe + 0x244) + 12 * i, vj = m.read(aoe + 0x244) + 12 * j;
            const Word xy[8] = {a[0], a[1], b[0], b[1], m.read(vi), m.read(vi + 4), m.read(vj), m.read(vj + 4)};
            if (p.intersect_segments_0058f840(area, xy, s + 0x24, s + 0x28) == 1) return 1;
        }
        return 0;
    }
    const Word slot = s + 0x5c;
    const double r = fd(m, aoe + 0x238);
    if (bx == ax) {                                                   // 005950E7: ordered equality
        const Word pv = s + 0x24;
        const float d = fs(fd(m, aoe_position_005946e0(m, p, aoe, pv, slot)) - bx);
        if (-r > d) return 0;
        if (static_cast<double>(d) > r) return 0;
        const double hi = ay > by ? ay : by;
        const double lo = ay < by ? ay : by;
        const double h = fs(std::sqrt(r * r - static_cast<double>(d) * static_cast<double>(d)));
        const double low = fd(m, aoe_position_005946e0(m, p, aoe, pv, slot) + 4) - h;
        if (low <= lo) {                                              // 005951A2
            const double low2 = fd(m, aoe_position_005946e0(m, p, aoe, pv, slot) + 4) - h;
            if (low2 >= hi) return 1;                                 // 005951C0
        }
        const double high = h + fd(m, aoe_position_005946e0(m, p, aoe, pv, slot) + 4);
        if (!(high <= lo)) return 0;                                  // 005951E2
        const double high2 = h + fd(m, aoe_position_005946e0(m, p, aoe, s + 0x48, slot) + 4);
        return high2 >= hi ? 1u : 0u;                                 // 00595204
    }
    const double slope = (by - ay) / (bx - ax);
    const float mf = fs(slope);
    const float c = fs(by - slope * bx);
    const float A = fs(static_cast<double>(mf) * mf + 1.0);
    const Word p1 = aoe_position_005946e0(m, p, aoe, s + 0x24, slot);
    const Word p2 = aoe_position_005946e0(m, p, aoe, s + 0x30, slot);
    const float B = fs(((static_cast<double>(c) - fd(m, p2 + 4)) * mf - fd(m, p1)) * 2.0);
    (void)p1;
    const double x3 = fd(m, aoe_position_005946e0(m, p, aoe, s + 0x30, slot));
    const double x4 = fd(m, aoe_position_005946e0(m, p, aoe, s + 0x24, slot));
    const float B2 = fs(static_cast<double>(B) * B);
    const float X2 = fs(x3 * x4);
    const float cy = fs(static_cast<double>(c) - fd(m, aoe_position_005946e0(m, p, aoe, s + 0x3c, slot) + 4));
    const double cy2 = (static_cast<double>(c) - fd(m, aoe_position_005946e0(m, p, aoe, s + 0x48, slot) + 4)) * cy;
    const double disc = B2 - ((((static_cast<double>(X2) - cy2) - r * r) * A) * static_cast<double>(kFour));
    if (disc < 0.0) return 0;                                         // 00595304 (NaN continues)
    const double root = std::sqrt(disc);
    const double twoA = static_cast<double>(A) + A;
    const float x1 = fs((-static_cast<double>(B) - root) / twoA);
    const float x2 = fs((root - static_cast<double>(B)) / twoA);
    const double MX = ax > bx ? ax : bx;
    const double MN = ax < bx ? ax : bx;
    if (MN >= x1 && MX <= x1) return 1;                               // 00595369 / 00595376
    if (!(MN >= x2)) return 0;                                        // 00595392
    return MX <= x2 ? 1u : 0u;                                        // 0059539D
}

Word encounter_in_area_00590090(Memory& m, Word e, Word position) {
    const I count = static_cast<I>(m.read(e + 0x29c));
    Word crossings = 0;
    if (count > 0) {
        const Word index = m.read(e + 0x298), verts = m.read(e + 0x290);
        const double px = fd(m, position), py = fd(m, position + 4);
        for (I k = 0; k < count; ++k) {
            const Word an = m.read(index + 4 * static_cast<Word>(k));
            const Word bn = m.read(index + 4 * static_cast<Word>(k == count - 1 ? 0 : k + 1));
            const Word A = verts + 12 * an, B = verts + 12 * bn;
            const double Ax = fd(m, A), Ay = fd(m, A + 4), Bx = fd(m, B), By = fd(m, B + 4);
            if (!(Bx == Ax)) {                                        // 005900F0: equal x skips the crossing test
                const float dy = fs(By - Ay), dx = fs(Bx - Ax);
                const float yat = fs(Ay - (Ax - px) * (static_cast<double>(dy) / static_cast<double>(dx)));
                if (static_cast<double>(yat) > py) {                  // 0059012F
                    bool inside;
                    if (std::fabs(static_cast<double>(dx)) < std::fabs(static_cast<double>(dy))) {   // 0059014B: y range
                        if (By > Ay) inside = yat >= Ay && yat < By;
                        else inside = Ay > By && yat > By && yat <= Ay;
                    } else {                                          // x range
                        if (Bx > Ax) inside = px >= Ax && px < Bx;
                        else inside = Ax > Bx && px > Bx && px <= Ax;
                    }
                    if (inside) ++crossings;
                }
            }
            if (Bx == px && (By > py || Ay >= py)) ++crossings;       // 005901EC
        }
    }
    // 00590225: (count & 80000001h) == 1
    return (crossings & 0x80000001u) == 1u ? 1u : 0u;
}

Word encounter_segment_0058ffd0(Memory& m, Ports& p, Word e, Word a, Word b, Word s) {
    const Word area = p.object_area_004cb120(e);
    if (area == 0 || static_cast<I>(m.read(e + 0x29c)) <= 0) return 0;
    for (I k = 0; k < static_cast<I>(m.read(e + 0x29c)); ++k) {
        const I n = static_cast<I>(m.read(e + 0x29c));
        const Word an = m.read(m.read(e + 0x298) + 4 * static_cast<Word>(k));
        const Word bn = m.read(m.read(e + 0x298) + 4 * static_cast<Word>(k == n - 1 ? 0 : k + 1));
        const Word A = m.read(e + 0x290) + 12 * an, B = m.read(e + 0x290) + 12 * bn;
        const Word xy[8] = {m.read(a), m.read(a + 4), m.read(b), m.read(b + 4), m.read(A), m.read(A + 4), m.read(B), m.read(B + 4)};
        if (p.intersect_segments_0058f840(area, xy, s + 4, s) == 1) return 1;
    }
    return 0;
}
}
