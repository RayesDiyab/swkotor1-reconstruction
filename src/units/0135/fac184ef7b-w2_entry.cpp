#include "e5bc96b646-w2_entry.hpp"
#include "28478d5d50-w2_ftol.hpp"
#include "2f0774e610-client_loadbar.hpp"
#include <cstring>
// Instruction-ordered bodies. Comments cite original addresses.
namespace w2::entry {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
Word server_mode(Memory& m, Ports& p) { return m.read16(m.read(server_of(m, p) + 4) + 0x10008); } // 004AEDD0
Word server_module(Memory& m, Ports& p) { return p.internal_module_004b14f0(m.read(server_of(m, p) + 4)); } // 004AE6B0
Word sws_message(Memory& m, Ports& p) { return m.read(m.read(server_of(m, p) + 4) + 0x10010); } // 004AEDB0
Word party_table(Memory& m, Word server) { return m.read(server + 4) + 0x1b770; }              // 004AEE70
Word player_list(Memory& m, Word server) { return m.read(m.read(server + 4) + 0x10064); }      // 004AED90
Word ai_master(Memory& m, Word server) { return m.read(m.read(server + 4) + 0x10044); }      // 004AED80
Word loadscreens(Memory& m, Ports& p) { return m.read(m.read(p.rules_007a3a28() + 0xb8) + 0x18); }
Word player_info_name(Ports& p, Word server) {
    // [[server]+10h]() = GetNetLayer; 005D5170 GetPlayerInfo(id) = net+20014h (id ignored); +4 = name
    return p.network_getter(server) + 0x20014 + 4;
}
Word signed_mod(Ports& p, Word dividend, Word divisor, Word pc) {
    if (divisor == 0) { p.fault(pc, "integer divide by zero (#DE) in original"); return 0; }
    const auto a = static_cast<std::int32_t>(dividend), b = static_cast<std::int32_t>(divisor);
    if (a == INT32_MIN && b == -1) { p.fault(pc, "integer overflow (#DE) in original"); return 0; }
    return static_cast<Word>(a % b);
}
void copy16(Memory& m, Word dst, Word src) { for (Word i = 0; i < 16; i += 4) m.write(dst + i, m.read(src + i)); }
}

Word ftol2_low(Word bits) {
    float f; std::memcpy(&f, &bits, 4);
    return w2::ftol2_006fae8c(static_cast<double>(f));   // __ftol2 006FAE8C (existing provider natively)
}

void set_area_transition_bmp_00560cd0(Memory& m, Ports& p, Word player, Word value, Word str) {
    if (value == 1) p.str_assign_005e5c50(player + 0x18, str);
    m.write(player + 0x14, value);
}

Word area_client_area_0056cd20(Memory& m, Ports& p, Word self, Word player, Word area, Word x, Word y, Word z,
                               Word orientation, Word flag, Word scratch) {
    const Word tileset = scratch, value = scratch + 8, column = scratch + 0x10, out_data = scratch + 0x18,
               out_size = scratch + 0x1c, text = scratch + 0x20;
    const Word module = server_module(m, p);                               // 0056CD4B
    const Word mode = server_mode(m, p) & 0xffffu;                         // 0056CD5B
    if (mode != 2 || module == 0) return 0;
    const Word entry = m.read(module + 0x58);                              // 0056CD72
    if (p.game_object_00561080(player) == 0) return 0;
    if (area == 0) return 0;
    sm::message_begin_004d68a0(m, p, self, 0x10000, 0xffffffffu, 1);       // 0056CDA1
    const Word current = m.read(player + 0x14);
    if (current == 0xffffffffu || current == 1) {                          // 0056CDA9..B1
        p.str_literal_005e5a90(text, p.literal(0x0073d71c));
        set_area_transition_bmp_00560cd0(m, p, player, m.read16(area + 0x228), text);
        p.str_destroy_005e5c20(text);
    }
    if (static_cast<std::int32_t>(m.read(player + 0x14)) >= static_cast<std::int32_t>(m.read(loadscreens(m, p) + 0x24))) { // 0056CE06 JL
        p.str_literal_005e5a90(text, p.literal(0x0073d71c));
        set_area_transition_bmp_00560cd0(m, p, player, 0, text);
        p.str_destroy_005e5c20(text);
    }
    const Word screen = m.read(player + 0x14);
    if (screen != 0) {
        write_byte_004d62c0(m, p, self, screen, 8);                        // 0056D01B
    } else {
        p.str_empty_005b3190(tileset);                                     // 0056CE4F: never assigned (quirk)
        const Word rows = m.read(loadscreens(m, p) + 0x24);
        Word chosen = 0;
        p.str_empty_005b3190(value);
        Word count = 0;
        bool picked = false;
        if (static_cast<std::int32_t>(rows) > 2) {
            for (Word row = 2; static_cast<std::int32_t>(row) < static_cast<std::int32_t>(rows); ++row) {
                p.str_literal_005e5a90(column, p.literal(0x00749368));     // "TileSet"
                const bool ok = p.c2da_string_00413270(loadscreens(m, p), row, column, value) == 1;
                p.str_destroy_005e5c20(column);
                if (ok && p.compare_no_case_005e6450(tileset, value) == 1) ++count;
            }
            if (static_cast<std::int32_t>(count) > 0) {
                const Word pick = signed_mod(p, p.rand_006fc447(), count, 0x0056cf21);
                Word ordinal = 0;
                for (Word row = 2; static_cast<std::int32_t>(row) < static_cast<std::int32_t>(rows); ++row) {
                    p.str_literal_005e5a90(column, p.literal(0x00749360)); // "TILESET"
                    const bool ok = p.c2da_string_00413270(loadscreens(m, p), row, column, value) == 1;
                    p.str_destroy_005e5c20(column);
                    if (ok && p.compare_no_case_005e6450(tileset, value) == 1) {
                        if (ordinal == pick) chosen = row;
                        ++ordinal;
                    }
                }
                picked = chosen != 0;                                      // 0056CFBD
            }
        }
        if (!picked) chosen = signed_mod(p, p.rand_006fc447(), rows - 2, 0x0056cfd5) + 2; // 0056CFC8..D7
        write_byte_004d62c0(m, p, self, chosen, 8);                        // 0056CFEC
        p.str_destroy_005e5c20(value);
        p.str_destroy_005e5c20(tileset);
    }
    p.str_literal_005e5a90(text, p.literal(0x0073d71c));                   // 0056D029
    set_area_transition_bmp_00560cd0(m, p, player, 0xffffffffu, text);
    p.str_destroy_005e5c20(text);
    const Word facing = p.atan2_bits(m.read(orientation + 4), m.read(orientation)); // 0056D055..6C
    write_float_bits_004d6920(m, p, self, x, 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, self, y, 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, self, z, 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, self, facing, 0x3f800000, 0x20);
    write_bool_004d68e0(m, p, self, flag);
    const Word c = ftol2_low(m.read(entry + 0x18)), b = ftol2_low(m.read(entry + 0x14)), a = ftol2_low(m.read(entry + 0x10));
    if (p.pack_area_0050aaa0(area, a, b, c, player) != 1) return 0;        // 0056D0D6
    if (sm::message_finish_004d6440(m, self, out_data, out_size) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, m.read(player + 4), 4, 1, m.read(out_data), m.read(out_size));
}

Word custom_token_list_0056d2d0(Memory& m, Ports& p, Word self, Word recipient, Word scratch8) {
    const Word out_size = scratch8, out_data = scratch8 + 4;
    const Word tlk = p.tlk();
    const Word n = m.read(tlk + 0x4c);
    Word count = n;
    if (n != 0) {
        Word e = m.read(tlk + 0x48);
        for (Word k = n; k != 0; --k, e += 0x0c)
            if (static_cast<std::int32_t>(m.read(e)) <= 9) --count;        // [0073ECCC]=9, JG
    }
    if (count == 0) return 1;
    sm::message_begin_004d68a0(m, p, self, count * 8 + 4, recipient, 1);
    sm::message_word_004d6340(m, p, self, count, 0x20);
    for (Word k = n, offset = 0; k != 0; --k, offset += 0x0c) {
        const Word number = m.read(m.read(p.tlk() + 0x48) + offset);
        if (static_cast<std::int32_t>(number) > 9) {
            sm::message_word_004d6340(m, p, self, number, 0x20);
            write_string_004d6960(m, p, self, m.read(p.tlk() + 0x48) + offset + 4, 0x20);
        }
    }
    if (sm::message_finish_004d6440(m, self, out_data, out_size) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, recipient, 0x32, 2, m.read(out_data), m.read(out_size));
}

void pack_module_004c3170(Memory& m, Ports& p, Word module, Word scratch) {
    const Word name = scratch, ref = scratch + 8, loc_scratch = scratch + 0x18;
    const Word message = sws_message(m, p);
    const Word colon = p.str_chr_006fb5e0(p.str_cstr_005e5670(module + 0x5c), 0x3a);
    p.str_literal_005e5a90(name, colon + 1);                               // 004C31B4 (domain: ':' present)
    write_string_004d6960(m, p, message, name, 0x20);
    p.str_destroy_005e5c20(name);
    write_locstring_005785c0(m, p, message, module + 0x64, 0, loc_scratch);
    write_byte_004d62c0(m, p, message, m.read8(module + 0x1a0), 8);
    copy16(m, ref, module + 0x6c); write_resref_004d6380(m, p, message, ref, 0x10);
    copy16(m, ref, module + 0x7c); write_resref_004d6380(m, p, message, ref, 0x10);
}

Word module_info_00567ef0(Memory& m, Ports& p, Word self, Word recipient, Word scratch) {
    const Word module = server_module(m, p);
    if ((server_mode(m, p) & 0xffffu) != 1 && (server_mode(m, p) & 0xffffu) != 2) return 0;
    if (module == 0) return 0;
    sm::message_begin_004d68a0(m, p, self, 0x80, 0xffffffffu, 1);
    pack_module_004c3170(m, p, module, scratch + 8);
    if (sm::message_finish_004d6440(m, self, scratch + 4, scratch) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, recipient, 3, 1, m.read(scratch + 4), m.read(scratch));
}

namespace {
Word empty_message(Memory& m, Ports& p, Word self, Word recipient, Word major, Word minor) {
    const Word buffer = p.allocate(3);
    m.write(self + 0x4c, 0xffffffffu);
    if (buffer == 0) return 0;
    p.write_byte(buffer, 0x50); p.write_byte(buffer + 1, major); p.write_byte(buffer + 2, minor);
    const Word net = p.network_getter(server_of(m, p));
    const Word result = p.send_005d4cb0(net, recipient, buffer, 3, 0);
    p.free(buffer);
    return result;
}
}
Word login_confirm_0056fde0(Memory& m, Ports& p, Word self, Word recipient) { return empty_message(m, p, self, recipient, 2, 5); }
Word start_start_new_module_0056cbf0(Memory& m, Ports& p, Word self) { return empty_message(m, p, self, 0xfffffff7u, 3, 0x0b); } // [0074C52C]
Word end_start_new_module_0056cc50(Memory& m, Ports& p, Word self, Word flag, Word s) {
    sm::message_begin_004d68a0(m, p, self, 1, 0xffffffffu, 1);
    write_bool_004d68e0(m, p, self, flag);
    if (sm::message_finish_004d6440(m, self, s + 4, s) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, 0xfffffff7u, 3, 0x0c, m.read(s + 4), m.read(s));   // [0074C52C]
}
// 0056A5F0 SendServerToPlayerModuleUpdate_Time(player, flags, dword10, byte8, byte4, byte2, kind, dword_kind) RET 20h.
// P/3/3: BYTE flags; [&1: BYTE kind; kind 3|4: DWORD]; [&2: BYTE]; [&4: BYTE]; [&8: BYTE]; [&10h: DWORD]. scratch8.
Word module_update_time_0056a5f0(Memory& m, Ports& p, Word self, Word player, Word flags, Word a3, Word a4, Word a5, Word a6, Word kind, Word value, Word s) {
    sm::message_begin_004d68a0(m, p, self, 0x0d, 0xffffffffu, 1);
    write_byte_004d62c0(m, p, self, flags, 8);
    if ((flags & 1u) != 0) {
        write_byte_004d62c0(m, p, self, kind, 8);
        if ((kind & 0xffu) == 3 || (kind & 0xffu) == 4) sm::message_word_004d6340(m, p, self, value, 0x20);   // 0056A627: AL compare
    }
    if ((flags & 2u) != 0) write_byte_004d62c0(m, p, self, a6, 8);
    if ((flags & 4u) != 0) write_byte_004d62c0(m, p, self, a5, 8);
    if ((flags & 8u) != 0) write_byte_004d62c0(m, p, self, a4, 8);
    if ((flags & 0x10u) != 0) sm::message_word_004d6340(m, p, self, a3, 0x20);
    if (sm::message_finish_004d6440(m, self, s + 4, s) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, m.read(player + 4), 3, 3, m.read(s + 4), m.read(s));
}
// 0056D1E0 SendServerToPlayerArea_Weather(player, byte, bool) RET 0Ch. P/4/4: BYTE, BOOL. scratch8.
Word area_weather_0056d1e0(Memory& m, Ports& p, Word self, Word player, Word weather, Word flag, Word s) {
    sm::message_begin_004d68a0(m, p, self, 5, 0xffffffffu, 1);
    write_byte_004d62c0(m, p, self, weather, 8);
    write_bool_004d68e0(m, p, self, flag);
    if (sm::message_finish_004d6440(m, self, s + 4, s) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, m.read(player + 4), 4, 4, m.read(s + 4), m.read(s));
}
Word module_loading_0056cb10(Memory& m, Ports& p, Word self, Word player) { const Word id = m.read(player + 4); return empty_message(m, p, self, id, 3, 6); }

namespace {
// Shared player-list record (00568030 body 0056806B..0056820A, 0056FE40 body 0056FF10..005700BF).
// scratch: +00 name +08 loc1 +10 loc2 +18 ref +28 ref copy +38 ref result +48 loc scratch(0x14)
void player_record(Memory& m, Ports& p, Word self, Word player_id, Word go_id, Word player, Word scratch, bool begin) {
    const Word name = scratch, loc1 = scratch + 8, loc2 = scratch + 0x10, ref = scratch + 0x18,
               ref_copy = scratch + 0x28, ref_result = scratch + 0x38, loc_scratch = scratch + 0x48, arg = scratch + 0x60;
    p.str_copy_005e5b00(name, player_info_name(p, server_of(m, p)));
    Word creature_id = 0x7f000000u;
    const Word as_creature = p.vcall0(p.game_object_00561080(player), 0x30);
    if (as_creature != 0 && m.read(m.read(as_creature + 0xa74) + 0x6c) == 1) creature_id = m.read(as_creature + 4);
    p.loc_construct_005e9e10(loc1);
    p.loc_construct_005e9e10(loc2);
    p.ref_default_00405ed0(ref);
    Word has = 0;
    const Word creature = p.creature_by_id_004ae770(server_of(m, p), creature_id);
    if (creature != 0) {
        has = 1;
        p.loc_assign_005e9e70(loc1, m.read(creature + 0xa74) + 0x34);
        p.loc_assign_005e9e70(loc2, m.read(creature + 0xa74) + 0x3c);
        copy16(m, ref_copy, m.read(creature + 0xa74) + 0x16e);
        p.ref_assign_lower_00406120(ref, ref_result, ref_copy);
    }
    if (begin) sm::message_begin_004d68a0(m, p, self, 0x100, 0xffffffffu, 1);
    sm::message_word_004d6340(m, p, self, player_id, 0x20);
    write_object_id_00578570(m, p, self, go_id);
    write_string_004d6960(m, p, self, name, 0x20);
    write_bool_004d68e0(m, p, self, has);
    if (has == 1) {
        write_object_id_00578570(m, p, self, creature_id);
        write_locstring_005785c0(m, p, self, loc1, 0, loc_scratch);
        write_locstring_005785c0(m, p, self, loc2, 0, loc_scratch);
        const Word portrait = p.vcall0(creature, 0xd0);
        write_word16_004d6300(m, p, self, portrait, 0x10);
        if ((portrait & 0xffffu) >= 0xfffeu) { copy16(m, arg, ref); write_resref_004d6380(m, p, self, arg, 0x10); }
    }
}
void player_record_destroy(Ports& p, Word scratch) {
    p.loc_destroy_005ea000(scratch + 0x10);
    p.loc_destroy_005ea000(scratch + 8);
    p.str_destroy_005e5c20(scratch);
}
}

Word player_list_add_00568030(Memory& m, Ports& p, Word self, Word recipient, Word player, Word scratch) {
    const Word player_id = m.read(player + 4);
    const Word go_id = m.read(p.game_object_00561080(player) + 4);         // 00568065 (domain: player has an object)
    // The original creates the message after the record locals (0056815B); the
    // record helper performs that order with begin=true.
    player_record(m, p, self, player_id, go_id, player, scratch, true);
    const Word out_data = scratch + 0x74, out_size = scratch + 0x78;
    if (sm::message_finish_004d6440(m, self, out_data, out_size) == 0) { player_record_destroy(p, scratch); return 0; }
    const Word result = sm::message_send_00567e60(m, p, self, recipient, 0x0a, 2, m.read(out_data), m.read(out_size));
    player_record_destroy(p, scratch);
    return result;
}

Word player_list_all_0056fe40(Memory& m, Ports& p, Word self, Word player, Word scratch) {
    const Word slot = scratch + 0x7c, out_data = scratch + 0x74, out_size = scratch + 0x78;
    sm::message_begin_004d68a0(m, p, self, 0x200, 0xffffffffu, 1);
    const Word list = player_list(m, server_of(m, p));
    if (list == 0) return 0;
    Word count = 0;                                                         // byte counter at [esp+14]
    m.write(slot, m.read(m.read(list)));
    for (Word position = m.read(slot); position != 0; position = m.read(slot)) {
        if (p.game_object_00561080(p.list_at_005e9b60(m.read(list), position)) != 0) count = (count + 1) & 0xffu;
        p.list_next_005e9b90(m.read(list), slot);
    }
    write_byte_004d62c0(m, p, self, count, 8);
    m.write(slot, m.read(m.read(list)));
    for (Word position = m.read(slot); position != 0; position = m.read(slot)) {
        const Word other = p.list_at_005e9b60(m.read(list), position);
        const Word other_id = m.read(other + 4);
        const Word go = p.game_object_00561080(other);
        if (go != 0) {
            player_record(m, p, self, other_id, m.read(go + 4), other, scratch, false);
            player_record_destroy(p, scratch);
        }
        p.list_next_005e9b90(m.read(list), slot);
    }
    if (count == 0) return 0;                                               // 00570115: nothing sent
    if (sm::message_finish_004d6440(m, self, out_data, out_size) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, m.read(player + 4), 0x0a, 1, m.read(out_data), m.read(out_size));
}

void set_game_object_00561c40(Memory& m, Ports& p, Word player, Word object) {
    auto area_of = [&](Word server, Word id) -> Word {
        if (server == 0) return 0;
        const Word found = p.game_object_by_id_004ae750(server, id);
        if (found == 0) return 0;
        if (m.read8(found + 8) <= 4) return 0;                              // [00746633]=4, JBE
        return p.object_area_004cb120(found);
    };
    const Word old_server = server_of(m, p);
    const Word old_id = m.read(player + 0x28);
    const Word old_area = area_of(old_server, old_id);
    Word new_area = 0;
    if (object != 0) {
        const Word new_id = m.read(object + 4);
        m.write(player + 0x28, new_id);
        new_area = area_of(server_of(m, p), new_id);
        if (old_id != new_id) {
            m.write(m.read(player + 0x4c) + 0x10, 1);
            if (m.read(player + 0x5c) != 0) {
                p.clear_action_queue_005ca0d0(m.read(player + 0x5c));
                p.clear_known_spells_005cab20(m.read(player + 0x5c));
                p.clear_known_spell_uses_005ca060(m.read(player + 0x5c));
                p.clear_spell_add_delete_005cac30(m.read(player + 0x5c));
                p.clear_feats_005cab60(m.read(player + 0x5c));
            }
        }
        const Word first = server_of(m, p);
        if (first != 0 && party_table(m, first) != 0) {
            if (p.party_repository_00563340(party_table(m, server_of(m, p))) != 0) {
                const Word parent = m.read(player + 0x28);
                const Word repository = p.party_repository_00563340(party_table(m, server_of(m, p)));
                p.set_parent_id_00556150(repository, parent);
            }
        }
    } else {
        m.write(player + 0x28, 0x7f000000u);
    }
    if (old_area != new_area) {
        if (new_area != 0) p.increment_players_00508c20(new_area);
        if (old_area != 0) p.decrement_players_00508c40(old_area);
    }
}

void initiate_module_004b3a90(Memory& m, Ports& p, Word internal, Word player, Word scratch) {
    const Word text = scratch, vector = scratch + 8, inner = scratch + 0x20;
    Word creature;
    if (p.game_object_00561080(player) == 0) {
        const Word memory = p.allocate(0xac8);                             // 004B3AC2
        creature = memory != 0 ? p.creature_construct_004f7a10(memory, 0x7f000000u, 1) : 0;
        m.write(m.read(creature + 0x340) + 0x2c, m.read(creature + 4));   // 004B3AEC..F5 (domain: allocation succeeds)
        set_game_object_00561c40(m, p, player, creature);
    } else {
        creature = p.vcall0(p.game_object_00561080(player), 0x30);
    }
    const Word fresh = m.read(player + 0x40) == 0 ? 1u : 0u;              // 004B3B1C..26
    Word area;
    if (fresh != 0) {
        area = p.module_area_004c30f0(p.internal_module_004b14f0(internal));
        m.write(creature + 0x310, m.read(area + 0x120));
        const Word z = m.read(m.read(p.internal_module_004b14f0(internal) + 0x58) + 0x18);
        const Word y = m.read(m.read(p.internal_module_004b14f0(internal) + 0x58) + 0x14);
        const Word x = m.read(m.read(p.internal_module_004b14f0(internal) + 0x58) + 0x10);
        m.write(creature + 0x314, x); m.write(creature + 0x318, y); m.write(creature + 0x31c, z);
        m.write(creature + 0x320, 0);
        m.write(vector, m.read(m.read(p.internal_module_004b14f0(internal) + 0x58) + 0x1c));
        m.write(vector + 4, m.read(m.read(p.internal_module_004b14f0(internal) + 0x58) + 0x20));
        m.write(vector + 8, 0);
        p.set_orientation_004cd170(creature, vector);
    } else {
        area = p.object_area_004cb120(creature);
        if (area != 0) {
            m.write(creature + 0x310, m.read(area + 0x120));
            m.write(creature + 0x314, m.read(creature + 0x90));
            m.write(creature + 0x318, m.read(creature + 0x94));
            m.write(creature + 0x31c, m.read(creature + 0x98));
            m.write(creature + 0x320, 0);
        } else {
            area = p.area_by_id_004ae780(server_of(m, p), m.read(creature + 0x310));
        }
    }
    if (area == 0) return;
    p.set_area_004cb160(creature, 0);
    const Word message = sws_message(m, p);
    if (message == 0) return;
    p.str_literal_005e5a90(text, p.literal(0x0073d71c));
    set_area_transition_bmp_00560cd0(m, p, player, m.read16(area + 0x228), text);
    p.str_destroy_005e5c20(text);
    (void)area_client_area_0056cd20(m, p, message, player, area, m.read(creature + 0x314), m.read(creature + 0x318),
                                    m.read(creature + 0x31c), creature + 0x9c, fresh, inner);
    (void)custom_token_list_0056d2d0(m, p, message, m.read(player + 4), inner + 0x40);
}

void move_player_to_area_004b3d10(Memory& m, Ports& p, Word internal, Word player, Word scratch) {
    const Word out = scratch, position = scratch + 0x0c;
    for (Word i = 0; i < 0x18; i += 4) m.write(scratch + i, 0);           // 004B3D1D..45 six zero dwords
    if (p.game_object_00561080(player) == 0) return;
    const Word creature = p.vcall0(p.game_object_00561080(player), 0x30);
    const Word tag = internal + 0x1008c;
    if ((p.str_char_005e5600(tag, 0) & 0xffu) != 0) {
        const Word id = p.waypoint_004c6e00(p.internal_module_004b14f0(internal), tag);
        if (id != 0x7f000000u) {
            const Word waypoint = p.waypoint_by_id_004ae7e0(server_of(m, p), id);
            if (waypoint != 0) {
                m.write(creature + 0x314, m.read(waypoint + 0x90));
                m.write(creature + 0x318, m.read(waypoint + 0x94));
                m.write(creature + 0x31c, m.read(waypoint + 0x98));
                p.set_orientation_004cd170(creature, waypoint + 0x9c);
            }
        }
        p.str_assign_text_005e5140(tag, p.literal(0x0073d71c));
    }
    m.write(position, m.read(creature + 0x314));
    m.write(position + 4, m.read(creature + 0x318));
    m.write(position + 8, m.read(creature + 0x31c));
    const Word area = p.module_area_004c30f0(p.internal_module_004b14f0(internal));
    m.write(m.read(creature + 0x340) + 0x2c, m.read(creature + 4));       // 004B3E06..13
    if (area != 0) {
        if (p.safe_location_004be860(area, position, 0x41a00000u, m.read(creature + 0x340), 1, out) == 0) {
            m.write(out, m.read(position)); m.write(out + 4, m.read(position + 4)); m.write(out + 8, m.read(position + 8));
        }
        const Word jump = m.read(internal + 0x100c4) == 0 ? 1u : 0u;
        p.update_subareas_0051b940(creature, out, m.read(area + 0x120), jump);
        p.add_to_area_004fa100(creature, area, m.read(out), m.read(out + 4), m.read(out + 8), 0, 0);
    }
    p.update_visible_0051b050(creature, 0);
    p.position_party_00565b00(internal + 0x1b770);
    m.write(internal + 0x100c4, 0);
}

Word primary_player_index_004c5d90(Memory& m, Ports& p, Word module, Word scratch4) {
    Word position = m.read(m.read(module + 0x48));
    m.write(scratch4, position);
    Word index = 0;
    while (position != 0) {
        const Word entry = p.list_at_005e9b60(m.read(module + 0x48), position);
        if (m.read(entry + 0x18) == 1) return index;
        p.list_next_005e9b90(m.read(module + 0x48), scratch4);
        position = m.read(scratch4);
        ++index;
    }
    return 0xffffffffu;
}

Word load_character_finish_004b5c50(Memory& m, Ports& p, Word internal, Word player, Word flag, Word scratch) {
    const Word text = scratch, value = scratch + 8, slot = scratch + 0x0c;
    const Word module = server_module(m, p);
    const Word first_memory = p.allocate(0x34);
    const Word first = first_memory != 0 ? p.event_construct_004d7540(first_memory) : 0;
    Word second = 0;
    if ((m.read8(player + 0x24) & 0xffu) != 1) {
        p.write_byte(player + 0x24, 1);
        (void)login_confirm_0056fde0(m, p, m.read(internal + 0x10010), m.read(player + 4));
        (void)module_info_00567ef0(m, p, m.read(internal + 0x10010), m.read(player + 4), scratch + 0x20);
    }
    if (flag != 0 && m.read(player + 0x44) != 0xffffffffu) {
        const Word old = p.game_object_00561080(player);
        if (old != 0) (void)p.vcall1(old, 0x00, 1);                       // scalar deleting destructor
        p.load_character_ifo_00561e30(player, m.read(player + 0x44), 1, 1);
    }
    m.write16(first, 0x0e);                                                // 004B5D19 (domain: allocation succeeded)
    if (p.game_object_00561080(player) == 0) {
        p.event_object_004d7780(first, 0, 0x7f000000u);                    // 004B5F32
    } else {
        const Word as_creature = p.vcall0(p.game_object_00561080(player), 0x30);
        if (p.vcall0(as_creature, 0x94) == 1 || p.dead_temp_004ef890(p.vcall0(p.game_object_00561080(player), 0x30)) == 1) {
            const Word second_memory = p.allocate(0x34);
            second = second_memory != 0 ? p.event_construct_004d7540(second_memory) : 0;
            m.write16(second, 0x0a);
        }
        p.event_object_004d7780(first, 0, m.read(p.game_object_00561080(player) + 4));
        const Word message = sws_message(m, p);
        const Word list = player_list(m, server_of(m, p));
        const Word creature = p.vcall0(p.game_object_00561080(player), 0x30);
        m.write(slot, m.read(m.read(list)));
        for (Word position = m.read(slot); position != 0; position = m.read(slot)) {
            const Word other = p.list_at_005e9b60(m.read(list), position);
            if (other != 0 && m.read(other + 0x48) != 0 && message != 0)
                p.character_list_response_0056b400(message, m.read(other + 4), m.read(creature + 4), 1);
            p.list_next_005e9b90(m.read(list), slot);
        }
    }
    if (module == 0) return 1;
    p.event_enqueue_004b08d0(ai_master(m, server_of(m, p)), 0, 0, m.read(player + 4), m.read(module + 0x20), 0x0a, first); // [007441DC]=0Ah
    if (second != 0)
        p.event_enqueue_004b08d0(ai_master(m, server_of(m, p)), 0, 0, m.read(p.game_object_00561080(player) + 4), m.read(module + 0x20), 0x0a, second);
    const Word creature = p.creature_by_id_004ae770(server_of(m, p), m.read(player + 0x38));
    if (creature != 0 && creature + 0xa50 != 0) {
        p.str_literal_005e5a90(text, p.literal(0x007454e4));               // "NORMALHEAD"
        (void)p.c2da_int_00413660(m.read(m.read(p.rules_007a3a28() + 0xb8) + 0x0c), m.read16(creature + 0xa60), text, value);
        p.str_destroy_005e5c20(text);
        p.enforce_no_pc_head_cloning_00508820(p.module_area_004c30f0(module), m.read(value));
    }
    return 1;
}

void no_pc_head_cloning_00508820(Memory& m, Ports& p, Word area, Word head, Word scratch) {
    const Word value = scratch, text = scratch + 4;                        // [esp+0C] out (not reset per object), [esp+10/18] string
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x194)); ++i) {
        const Word creature = p.creature_by_id_004ae770(server_of(m, p), m.read(m.read(area + 0x190) + 4 * i));
        if (creature == 0 || creature + 0xa50 == 0) continue;
        p.str_literal_005e5a90(text, p.literal(0x007454e4));               // "NORMALHEAD"
        const Word row = m.read(creature + 0xa48) != 0 ? m.read16(creature + 0xa4c) : m.read16(creature + 0xa60);
        (void)p.c2da_int_00413660(m.read(m.read(p.rules_007a3a28() + 0xb8) + 0x0c), row, text, value);
        p.str_destroy_005e5c20(text);
        if (m.read(value) == head) m.write(creature + 0xa6c, 1);
    }
}

Word load_primary_player_004b5f50(Memory& m, Ports& p, Word internal, Word player, Word scratch) {
    const Word module = server_module(m, p);
    const Word index = primary_player_index_004c5d90(m, p, module, scratch);
    if (index == 0xffffffffu) return 0;
    m.write(player + 0x44, index);
    (void)load_character_finish_004b5c50(m, p, internal, player, 1, scratch + 8);
    return 1;
}

Word module_message_00524800(Memory& m, Ports& p, Word self, Word player, Word minor, Word scratch) {
    const Word s10 = scratch, s20 = scratch + 8, s18 = scratch + 0x10, temp = scratch + 0x18, temp_scratch = scratch + 0x20;
    p.str_empty_005b3190(s10); p.str_empty_005b3190(s20); p.str_empty_005b3190(s18);
    auto cleanup = [&](Word result) { p.str_destroy_005e5c20(s18); p.str_destroy_005e5c20(s20); p.str_destroy_005e5c20(s10); return result; };
    auto read_into = [&](Word target) {
        read_string_004d67e0(m, p, self, temp, 0x20, temp_scratch);
        p.str_assign_005e5c50(target, temp);
        p.str_destroy_005e5c20(temp);
    };
    if (p.overflow_004d6230(self) != 0) return cleanup(0);
    const Word sub = minor & 0xffu;
    if (sub == 2) {
        Word result = 1;
        if ((m.read8(player + 0x24) & 0xffu) == 1 && (server_mode(m, p) & 0xffffu) == 2) {
            m.write(player + 0x7c, 1);
            initiate_module_004b3a90(m, p, m.read(server_of(m, p) + 4), player, scratch + 0x40);   // 004AE930 thunk
            result = player_list_add_00568030(m, p, self, 0xfffffff7u, player, scratch + 0x100) & 1u; // [0074C52C]
            result &= player_list_all_0056fe40(m, p, self, player, scratch + 0x100);
        }
        return cleanup(result);
    }
    if (sub == 4) {
        const Word id = sm::message_read_word_004d6780(m, self, 0x20);
        read_into(s10);
        read_into(s18);
        p.str_assign_005e5c50(m.read(server_of(m, p) + 4) + 0x1b93c, s18);  // 004AF0E0 SetSaveGameName
        if (p.underflow_004d6240(self) != 0) return cleanup(0);
        if (m.read(player + 0x3c) == 0) return cleanup(0);                  // 00560CC0 GetIsAllowedToSave
        return cleanup(p.save_game_004b58a0(m.read(server_of(m, p) + 4), id, s10, player, 0));
    }
    if (sub == 5) {
        const Word id = sm::message_read_word_004d6780(m, self, 0x20);
        read_into(s10);
        read_into(s20);
        if (p.underflow_004d6240(self) != 0) return cleanup(0);
        if (m.read(player + 0x3c) == 0) return cleanup(0);
        if (m.read(m.read(server_of(m, p) + 4) + 0x100b4) == 1) return cleanup(0);   // 004AECF0
        m.write(m.read(server_of(m, p) + 4) + 0x100b4, 1);                           // 004AED00
        (void)module_loading_0056cb10(m, p, self, player);
        return cleanup(p.load_game_004ba640(m.read(server_of(m, p) + 4), id, s10, s20, player));
    }
    if (sub == 7) {
        if (p.underflow_004d6240(self) != 0) return cleanup(0);
        (void)p.run_module_004b6270(m.read(server_of(m, p) + 4));
        return cleanup(1);
    }
    return cleanup(0);
}

Word area_message_00524b80(Memory& m, Ports& p, Word self, Word player, Word minor, Word scratch) {
    if (p.overflow_004d6230(self) != 0) return 0;
    if ((minor & 0xffu) != 3) return 0;
    if ((m.read8(player + 0x24) & 0xffu) == 1) p.write_byte(player + 0x24, 2);
    if ((server_mode(m, p) & 0xffffu) != 2) return 1;
    move_player_to_area_004b3d10(m, p, m.read(server_of(m, p) + 4), player, scratch);  // 004AE940 thunk
    Word result = player_list_add_00568030(m, p, self, 0xfffffff7u, player, scratch + 0x20) & 1u;
    result &= player_list_all_0056fe40(m, p, self, player, scratch + 0x20);
    if (p.pause_state_004ae980(server_of(m, p), 1) == 1) (void)p.set_pause_state_0056c9f0(self, 1, 1);
    if (p.pause_state_004ae980(server_of(m, p), 2) == 1) (void)p.set_pause_state_0056c9f0(self, 2, 1);
    return result;
}
}
