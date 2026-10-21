#include "5915c8b3cd-w2_npc_items.hpp"
#include "28478d5d50-w2_ftol.hpp"
#include "e18fc12cb6-effect_insert.hpp"
#include <cstdint>
#include <cstring>
namespace w2::npc_items {
namespace {
using I = std::int32_t;
constexpr Word kInvalid = 0x7f000000u;
constexpr Word kItemType = 6;                  // [00746635]
constexpr float kPercentScale = 0.0099999998f; // [0073DFEC]
Word t5_server(Memory& m, w2::party::Ports& p) { return m.read(p.app_007a39fc() + 8); }
struct Grow final : kotor::server_mainloop::EffectInsertPorts {
    Ports& x; explicit Grow(Ports& v) : x(v) {}
    void grow(Word list, Word capacity) override { x.array_grow_005e03c0(list, capacity); }
};
}

// 005636B0: (unsigned)npc < 9 -> [party+30h+4*npc] (full dword), else 0.
Word npc_available_005636b0(Memory& m, Word party, Word npc) {
    if (static_cast<I>(npc) < 0 || static_cast<I>(npc) >= 9) return 0;   // 005636B6 jl / 005636BB jge
    return m.read(party + 0x30 + 4 * npc);
}

// 005637C0: available -> AL = BYTE [party+54h+npc] (upper EAX bytes are npc's, 0 for 0..8);
// otherwise OR AL,0FFh on EAX = npc (005637DC).
Word npc_selectable_005637c0(Memory& m, Word party, Word npc) {
    if (static_cast<I>(npc) >= 0 && static_cast<I>(npc) < 9 && m.read(party + 0x30 + 4 * npc) != 0)
        return (npc & 0xffffff00u) | m.read8(party + 0x54 + npc);       // 005637D5 mov al, [eax+ecx+54h]
    return npc | 0xffu;
}

// 00564300 AddNPC(npc, creature). Stack locals of the original live at s: +0 SaveCreature top struct (orig 1Ch),
// +8 result string (orig 20h), +10h filename / "UTC " string (orig 28h), +18h "V2.0" / write-name string (orig 14h),
// +20h C2DA out slot (orig: the creature argument slot, so it starts as the creature pointer), +28h "PercentXP".
Word add_npc_00564300(Memory& m, w2::party::Ports& p, Ports& x, Word party, Word npc, Word creature, Word s) {
    if (static_cast<I>(npc) < 0 || static_cast<I>(npc) >= 9 || creature == 0) return 0;
    if (m.read(party + 0x30 + 4 * npc) != 0) return 0;                  // 00564348
    m.write(party + 0x30 + 4 * npc, 1);                                   // 00564350
    w2::party::transfer_inventory_005641e0(m, p, party, creature, s + 0x30);   // 00564358
    const Word top = s, result = s + 8, name = s + 0x10, ver = s + 0x18, out = s + 0x20, col = s + 0x28;
    p.str_literal_005e5a90(col, p.literal(0x00746f08));                 // "PercentXP"
    m.write(out, creature);                                               // out slot = the creature argument slot
    const Word table = m.read(m.read(p.rules_007a3a28() + 0xb8) + 0xf0);
    const Word found = p.c2da_int_00413660(table, npc, col, out);        // 00564397
    p.str_destroy_005e5c20(col);
    const I pct = static_cast<I>(m.read(out));
    const double mult = (found != 0 && pct != 0) ? static_cast<double>(pct) * static_cast<double>(kPercentScale) : 1.0;
    const I party_xp = static_cast<I>(m.read(party + 0xf8));
    const I target = static_cast<I>(w2::ftol2_006fae8c(static_cast<double>(party_xp) * mult));   // 005643DB
    // 005643F3..00564421: GetLevel is re-read each pass; edi += (pass-1)*1000 while pass+1 <= level (at least one pass)
    I sum = 0;
    if ((x.stats_level_005a5fd0(m.read(creature + 0xa74), 0) & 0xffu) >= 1) {
        I counter = 1, step = 1000;
        do {
            sum += step - 1000;
            ++counter; step += 1000;
        } while (counter <= static_cast<I>(x.stats_level_005a5fd0(m.read(creature + 0xa74), 0) & 0xffu));
    }
    m.write(creature + 0x22c, sum > target ? static_cast<Word>(sum - target) : 0u);   // 0056442D signed JG
    const Word server = t5_server(m, p);
    const Word pc = p.creature_by_id_004ae770(server, p.player_creature_id_004aea40(server));
    if (pc != 0 && x.creature_faction_00513fc0(pc) != 0)
        x.faction_add_member_005bfa70(x.creature_faction_00513fc0(pc), m.read(creature + 4), 0);   // 0056447B
    x.stats_movement_rate_005a5680(m.read(creature + 0xa74), 0);
    x.str_default_005b3190(result);
    (void)w2::party::filename_00563620(m, p, party, name, npc, s + 0x40);
    x.str_format_game_in_progress_005e5680(result, name);              // "GAMEINPROGRESS:%s"
    p.str_destroy_005e5c20(name);
    const Word mem = p.allocate_006fa7e6(0xa0);
    const Word gff = mem != 0 ? x.gff_construct_004105a0(mem) : 0;
    if (gff == 0) x.trap(0x00564538, "W2: AddNPC CResGFF allocation failed (original calls CreateGFFFile on null)");
    p.str_literal_005e5a90(ver, p.literal(0x00745338));                 // "V2.0"
    p.str_literal_005e5a90(name, p.literal(0x00747368));                // "UTC "
    (void)x.gff_create_00411260(gff, top, name, ver);                    // 0056453F
    p.str_destroy_005e5c20(name);
    p.str_destroy_005e5c20(ver);
    (void)x.save_creature_00500610(creature, gff, top);                  // 00564567
    p.str_literal_005e5a90(ver, x.str_cstr_005e5670(result));
    const Word written = x.gff_write_00413030(gff, ver, 0x7eb);          // 00564590 (the CResGFF is never freed: original)
    p.str_destroy_005e5c20(ver);
    p.str_destroy_005e5c20(result);
    return written;
}

// 005645F0 AddNPC(npc, CExoString* template)
Word add_npc_template_005645f0(Memory& m, w2::party::Ports& p, Ports& x, Word party, Word npc, Word template_str, Word s) {
    if (static_cast<I>(npc) < 0 || static_cast<I>(npc) >= 9) return 0;
    if (m.read(party + 0x30 + 4 * npc) != 0) return 0;                  // 00564620
    const Word mem = p.allocate_006fa7e6(0xac8);
    const Word c = mem != 0 ? p.creature_construct_004f7a10(mem, kInvalid, 0) : 0;
    p.ref_from_string_00406d60(s, template_str);                         // CResRef at s (16 bytes)
    if (c == 0) x.trap(0x00564672, "W2: AddNPC creature allocation failed (original calls LoadFromTemplate on null)");
    if (p.creature_template_005026d0(c, s, 0) == 0) {                    // 00564672
        p.vdelete(c);                                                     // [[c]](c, 1)
        return 0;
    }
    const Word r = add_npc_00564300(m, p, x, party, npc, c, s + 0x40);
    p.vdelete(c);
    return r;
}

// 00554170: [item+288h] bit 0 := flag bit 0
void set_identified_00554170(Memory& m, Word item, Word flag) {
    const Word v = m.read(item + 0x288);
    m.write(item + 0x288, v ^ ((v ^ flag) & 1u));
}

// 00510EF0 CSWSCreature::RemoveItem(item, a2, a3, CSWSItem** out) RET 10h
Word creature_remove_item_00510ef0(Memory& m, w2::party::Ports& p, Ports& x, Word c, Word item, Word a2, Word a3, Word out) {
    if (item == 0) return 0;
    if (x.inventory_has_item_005a4950(m.read(c + 0xa2c), item) == 1) {   // 00510F0C (equipped)
        x.run_unequip_005023a0(c, m.read(item + 0x14), kInvalid, a2, out, 0);
        if (out != 0) item = m.read(out);                                 // 00510F32
    }
    if (m.read(item + 0x268) == m.read(c + 4)) {                          // possessed by this creature
        if (w2::party::item_in_repository_00555f40(m, p, p.item_repository_004ef770(c, 1), item, 0) == 1)
            (void)w2::party::remove_item_00555fd0(m, p.item_repository_004ef770(c, 1), item);
    } else {
        const Word o = x.game_object_004ae750(t5_server(m, p), m.read(item + 0x268));
        if (o == 0 || m.read8(o + 8) != kItemType) return 0;             // 00510F81
        const Word container = p.vcall0(o, 0x38);
        if (m.read(container + 0x268) != m.read(c + 4)) return 0;        // 00510F93
        (void)w2::party::remove_item_00555fd0(m, m.read(container + 0x26c), item);
    }
    p.set_possessor_00553210(item, kInvalid, 1, a3, kInvalid);           // 00510FB4
    return 1;
}

// 00584AC0 CSWSPlaceable::RemoveItem(item) RET 4
Word placeable_remove_item_00584ac0(Memory& m, w2::party::Ports& p, Word pl, Word item) {
    const Word repo = m.read(pl + 0x36c);
    if (w2::party::item_in_repository_00555f40(m, p, repo, item, 0) != 1) return 0;
    (void)w2::party::remove_item_00555fd0(m, m.read(pl + 0x36c), item);
    p.set_possessor_00553210(item, kInvalid, 1, 1, kInvalid);            // 00584AF7
    return 1;
}

// 0055D2C0 CItemRepository::InsertItem(item, position) RET 8
void repository_insert_item_0055d2c0(Memory& m, Ports& x, Word r, Word item, Word position) {
    if ((m.read8(r + 8) & 1u) != 0) return;                               // 0055D2C4
    const Word list = r + 0x0c, id = m.read(item + 0x14);
    if (static_cast<I>(position) >= 0 && static_cast<I>(position) < static_cast<I>(m.read(r + 0x10))) {
        Grow g(x);
        kotor::server_mainloop::effect_insert_00616940(m, g, list, id, position);   // 0055D2E5 tail jump
        return;
    }
    if (m.read(list + 4) == m.read(list + 8)) {                           // append (0055D2FC)
        const Word cap = m.read(list + 8);
        x.array_grow_005e03c0(list, cap != 0 ? cap * 2 : 0x10);
    }
    m.write(m.read(list) + 4 * m.read(list + 4), id);
    m.write(list + 4, m.read(list + 4) + 1);
}

// 005C70C0 CSWSStore::AddItemToInventory(CSWSItem** cell) RET 4: keeps the store list sorted by descending cost.
Word store_add_item_005c70c0(Memory& m, w2::party::Ports& p, Ports& x, Word store, Word cell) {
    const Word cost = x.item_cost_00554000(m.read(cell));
    const Word first = p.repository_item_005560b0(m.read(store + 0x230), 0);
    if (first == 0 || x.item_cost_00554000(first) <= cost) {             // 005C70EC unsigned JA
        (void)p.repository_add_0055d330(m.read(store + 0x230), cell, 1, 0, 1);
        return 1;
    }
    Word at = m.read(m.read(store + 0x230) + 0x10);
    for (;;) {
        --at;
        if (static_cast<I>(at) < 0) x.trap(0x005c711b, "W2: store insert walked below index 0 (original loops forever)");
        const Word it = p.repository_item_005560b0(m.read(store + 0x230), at);
        if (it == 0) continue;
        if (m.read(it + 0x14) == m.read(m.read(cell) + 0x14)) return 0; // 005C712E: already listed
        if (x.item_cost_00554000(it) <= cost) continue;                   // 005C7139 unsigned JBE
        repository_insert_item_0055d2c0(m, x, m.read(store + 0x230), m.read(cell), at);
        return 1;
    }
}
}
