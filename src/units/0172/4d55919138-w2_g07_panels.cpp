// W2 tier 4 G07: MajorGUIPanels 00577670 / PartyAIState 005779F0. See w2_g07_panels.hpp.
#include "c94ea11535-w2_g07_panels.hpp"
#include "74be47154e-module_message.hpp"
namespace w2::g07 {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
using I = std::int32_t;
constexpr Word kInvalid = 0x7f000000u;                                    // [00746644]
Word srv(Memory& m, w2::entry::Ports& p) { return m.read(p.app_007a39fc() + 8); }
Word cli(Memory& m, w2::entry::Ports& p) { return m.read(p.app_007a39fc() + 4); }
// guards shared by 00567240 / 005683D0: player, game object, creature, inventory; returns the inventory or 0
Word inventory_of(Memory& m, w2::entry::Ports& p, Word player) {
    if (player == 0 || p.game_object_00561080(player) == 0) return 0;
    const Word go = p.game_object_00561080(player);
    if (p.vcall0(go, 0x30) == 0) return 0;
    if (m.read(p.vcall0(go, 0x30) + 0xa2c) == 0) return 0;
    return m.read(p.vcall0(go, 0x30) + 0xa2c);
}
}

// 004AEC90 CServerExoApp::GetServerInfo is W1-owned (@recovered_server_info_004aec90@8): PanelPorts::server_info_004aec90.
Word member_list_005be780(Memory& m, Word faction, Word out) { m.write(out, m.read(faction)); return m.read(faction + 4); }

Word create_new_player_last_00561100(Memory& m, PanelPorts& x, Word player) {
    const Word old = m.read(player + 0x5c);
    if (old != 0) {
        x.player_last_destroy_005cad70(old);
        x.free_006fa390(old);
        m.write(player + 0x5c, 0);
    }
    const Word mem = x.allocate_006fa7e6(0x1b4);
    const Word obj = mem != 0 ? x.player_last_construct_005cac80(mem) : 0;
    m.write(player + 0x5c, obj);
    x.clear_spell_uses_005ca060(obj);                                     // called even when obj == 0 (original)
    return m.read(player + 0x5c);
}

Word compute_inventory_update_00567240(Memory& m, w2::entry::Ports& p, PanelPorts& x, Word, Word player, Word slot) {
    const Word inv = inventory_of(m, p, player);
    if (inv == 0) return 0;
    const Word item = x.item_in_slot_005a4c20(inv, 1u << (slot & 31u));
    const Word arr = m.read(m.read(player + 0x54));
    if (item != 0) {
        if (m.read(arr + 4 * slot) != m.read(item + 0x14)) return 1;
        return m.read(item + 0x28c) != m.read(arr + 4 * slot + 0x48) ? 1u : 0u;
    }
    return m.read(arr + 4 * slot) == kInvalid ? 0u : 1u;
}

void update_last_inventory_005683d0(Memory& m, w2::entry::Ports& p, PanelPorts& x, Word, Word player, Word slot) {
    const Word inv = inventory_of(m, p, player);
    if (inv == 0) return;
    const Word item = x.item_in_slot_005a4c20(inv, 1u << (slot & 31u));
    if (item != 0) {
        m.write(m.read(m.read(player + 0x54)) + 4 * slot, m.read(item + 0x14));
        m.write(m.read(m.read(player + 0x54)) + 4 * slot + 0x48, m.read(item + 0x28c));
    } else {
        m.write(m.read(m.read(player + 0x54)) + 4 * slot, kInvalid);
        m.write(m.read(m.read(player + 0x54)) + 4 * slot + 0x48, 0xffffffffu);
    }
}

void major_gui_panels_00577670(Memory& m, w2::entry::Ports& p, PanelPorts& x, Word self, Word player, Word s) {
    const Word list = s, flags = s + 0x0c, lookup = s + 0x10;            // list {data,count,capacity}
    m.write(list, 0); m.write(list + 4, 0); m.write(list + 8, 0);
    Word go = p.game_object_00561080(player);
    if (go != 0 && p.vcall0(go, 0x30) != 0) {
        Word plast = m.read(player + 0x5c);
        const Word last = x.last_update_object_00561820(player, m.read(go + 4));
        if (last != 0) {
            m.write(flags, 0);
            bool write = true;
            if (plast == 0) {
                plast = create_new_player_last_00561100(m, x, player);
                x.gui_known_spells_00570930(self, p.vcall0(go, 0x30), plast);
                (void)p.vcall0(go, 0x30);                                 // 00577703: result unused
                m.write(flags, 0xffffffffu);
            } else {
                x.test_player_differences_00572a20(self, player, plast, last, flags);
                write = m.read(flags) != 0;
            }
            if (write) {
                x.player_update_00572dc0(self, player, plast, last, m.read(flags));
                x.store_player_values_00574400(self, player, plast, last, m.read(flags));
            }
        }
        if (m.read(m.read(player + 0x54) + 4) == 1) {
            const Word id = m.read(m.read(player + 0x58));
            if (id != kInvalid) {
                m.write(lookup, 0);
                const Word array = m.read(m.read(srv(m, p) + 4) + 0x1005c);   // 004AED70
                if ((x.object_lookup_004d8230(array, id, lookup) & 0xffu) == 0) {
                    const Word o = m.read(lookup);
                    Word repo = o;                                        // 005777AF: the object itself unless item / placeable
                    if (m.read8(o + 8) == 6) repo = m.read(p.vcall0(o, 0x38) + 0x26c);
                    if (m.read8(o + 8) == 9) repo = m.read(p.vcall0(o, 0x48) + 0x36c);
                    x.repository_update_0056e360(self, player, go, repo, m.read(m.read(player + 0x54)), 0, 0x43);
                }
            } else {
                Word swapped = 0;
                const Word saved_id = m.read(player + 0x28), saved_go = go;
                if ((m.read8(x.server_info_004aec90(srv(m, p)) + 0x104) & 1u) != 0
                    && x.party_access_panel_up_0062b4d0(x.in_game_gui_005ed690(cli(m, p)), 1) != 0) {
                    const Word npc = x.equip_screen_npc_0062fea0(x.in_game_gui_005ed690(cli(m, p)));
                    const Word nid = x.npc_object_00564700(x.party_table_004aee70(srv(m, p)), npc, 0, 1);
                    const Word c = p.creature_by_id_004ae770(srv(m, p), nid);
                    if (c != 0) { swapped = 1; m.write(player + 0x28, nid); go = c; }
                    else go = saved_go;
                }
                for (Word slot = 0; slot < 0x12; ++slot) {
                    if (compute_inventory_update_00567240(m, p, x, self, player, slot) != 1) continue;
                    update_last_inventory_005683d0(m, p, x, self, player, slot);
                    const Word mask = 1u << slot;
                    if (x.item_in_slot_005a4c20(m.read(p.vcall0(go, 0x30) + 0xa2c), mask) == 0) {
                        write_byte_004d62c0(m, p, self, 0x47, 8); write_byte_004d62c0(m, p, self, 0x49, 8);
                        write_byte_004d62c0(m, p, self, 0x44, 8); sm::message_word_004d6340(m, p, self, mask, 0x20);
                    } else {
                        x.list_add_00671c00(list, mask);
                    }
                }
                for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(list + 4)); ++k) {
                    write_byte_004d62c0(m, p, self, 0x47, 8); write_byte_004d62c0(m, p, self, 0x49, 8);
                    write_byte_004d62c0(m, p, self, 0x41, 8);
                    const Word mask = m.read(m.read(list) + 4 * k);
                    sm::message_word_004d6340(m, p, self, mask, 0x20);
                    const Word item = x.item_in_slot_005a4c20(m.read(p.vcall0(go, 0x30) + 0xa2c), mask);
                    if (item == 0) x.trap(0x0057795f, "W2: original reads the id of a null equipped item (MajorGUIPanels)");
                    write_object_id_00578570(m, p, self, m.read(item + 0x14));
                    x.item_appearance_00567090(self, item);
                    x.active_item_properties_00566f20(self, item, p.vcall0(go, 0x30));
                }
                if (swapped != 0) { go = saved_go; m.write(player + 0x28, saved_id); }
            }
            const Word repo = x.item_repository_004ef770(p.vcall0(go, 0x30), 1);
            x.repository_update_0056e360(self, player, go, repo, m.read(m.read(player + 0x54)), 1, 0x52);
        }
    }
    x.free_006fa390(m.read(list));                                        // free(NULL) when empty (original)
}

Word party_ai_state_005779f0(Memory& m, w2::entry::Ports& p, PanelPorts& x, Word self, Word player, Word s) {
    const Word data = s, out = s + 4, last = s + 8, flags = s + 0x0c;
    const Word array = m.read(m.read(srv(m, p) + 4) + 0x1005c);          // 004AED70
    const Word pgo = p.game_object_00561080(player);
    if (pgo == 0) x.trap(0x00577a17, "W2: original reads the vtable of a null player game object (PartyAIState)");
    const Word pc = p.vcall0(pgo, 0x30);
    const Word faction = x.faction_00513fc0(pc);
    if (faction != 0) {
        const Word count = member_list_005be780(m, faction, data);
        for (Word i = 0; i < count; ++i) {                                // JB: unsigned
            const Word id = m.read(m.read(data) + 4 * i);
            if (id == m.read(pgo + 4)) continue;
            m.write(out, 0);
            if ((x.object_lookup_004d8230(array, id, out) & 0xffu) != 0) continue;
            const Word c = p.vcall0(m.read(out), 0x30);
            if (c == 0) x.trap(0x00577a81, "W2: original reads +39C of a null member creature (PartyAIState)");
            if (m.read(c + 0x39c) == 1) continue;
            m.write(last, 0);                                             // the player argument slot in the original
            x.test_party_differences_0056e850(self, player, c, last, flags);
            const Word f = m.read(flags);
            if (f != 0) x.update_object_00574a10(self, player, c, 0, f, 0);
            x.store_party_values_00567cb0(self, pc, m.read(last), c, f);
        }
    }
    const Word pos = s + 0x10;                                            // L10 in the original
    m.write(pos, m.read(m.read(m.read(player + 0x10))));
    while (m.read(pos) != 0) {
        const Word e = x.list_at_pos_005e9b60(m.read(m.read(player + 0x10)), m.read(pos));
        if (m.read(e + 0x14) == 0) {
            const Word at = m.read(pos);
            x.list_next_005e9b90(m.read(m.read(player + 0x10)), pos);
            x.list_remove_005e9c10(m.read(m.read(player + 0x10)), at);
            x.free_006fa390(e);
        } else {
            m.write(e + 0x14, 0);
            x.list_next_005e9b90(m.read(m.read(player + 0x10)), pos);
        }
    }
    return 1;
}
}
