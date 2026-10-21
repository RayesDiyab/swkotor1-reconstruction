#include "72239a78a7-w2_entry.hpp"
#include <cmath>
#include <cstring>
// W2 G07: per-frame server->client game object update control layer.
// The heavy per-object writers (TestObjectVisible 0056DC60, TestObjectUpdateDifferences
// 00577FA0, UpdateObject 00574A10, StoreValuesInLastUpdateObject 00576FC0,
// DeleteLastUpdateObjectsForObject 0056E190, MajorGUIPanels 00577670,
// PartyAIState 005779F0, character sheet 00561260/0056F490) are ports.
// Floating point: the game runs the x87 with the MSVC CRT default precision
// control (53-bit), so FADD/FSUB/FMUL/FSQRT sequences are evaluated in double
// and rounded to float exactly where the original stores with FSTP m32.
namespace w2::entry {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;
namespace {
float f32(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
Word bits32(float f) { Word w; std::memcpy(&w, &f, 4); return w; }
Word server_module(Memory& m, Ports& p) { return p.internal_module_004b14f0(m.read(server_of(m, p) + 4)); }
Word object_array(Memory& m, Ports& p) { return m.read(m.read(server_of(m, p) + 4) + 0x1005c); }
Word write_length(Memory& m, Word self) { return m.read(self + 0x0c); }                 // 00405040
// x87 compare outcomes used by the original jcc sequences
bool less_ordered(double a, double b) { return a < b; }                                // C0=1, C2=0
}

void update_client_objects_004b3ec0(Memory& m, Ports& p, Word internal, Word player, Word force, Word time_lo, Word time_hi) {
    Word lo = time_lo, hi = time_hi;
    if (force == 1 && (lo | hi) == 0) {                                     // 004B3EC3..F6
        const std::uint64_t t = p.high_resolution_timer_005e9260();
        lo = static_cast<Word>(t); hi = static_cast<Word>(t >> 32);
    }
    const Word state = m.read8(player + 0x24);
    if (state != 2) {
        if (state != 1) return;
        if (m.read(player + 0x7c) != 1) return;
        if (p.game_object_00561080(player) == 0) return;
        const Word creature = p.vcall0(p.game_object_00561080(player), 0x30);
        if (creature == 0) return;
        if (m.read(creature + 0x310) == 0x7f000000u) return;
    }
    bool send = true;
    if (force != 1) {
        const std::uint64_t now = (static_cast<std::uint64_t>(hi) << 32) | lo;
        const std::uint64_t last = (static_cast<std::uint64_t>(m.read(player + 0x30)) << 32) | m.read(player + 0x2c);
        const std::uint64_t delta = now - last;                              // SUB/SBB
        if ((delta >> 32) == 0 && static_cast<Word>(delta) < 200000u) send = false;
    }
    const Word message = m.read(internal + 0x10010);
    alignas(4) Word local[0x10] = {};
    const Word scratch = static_cast<Word>(reinterpret_cast<std::uintptr_t>(local));
    if (send) {
        (void)game_object_update_00578520(m, p, message, player, scratch);
        if (p.game_object_00561080(player) != 0) {
            const Word id = m.read(p.game_object_00561080(player) + 4);
            if (m.read(player + 0x34) != id && p.object_area_004cb120(p.game_object_00561080(player)) != 0) {
                (void)object_control_0056f430(m, p, m.read(internal + 0x10010), m.read(player + 4), id, scratch);
                m.write(player + 0x34, id);
                m.write(player + 0x2c, lo); m.write(player + 0x30, hi);
            }
        }
        m.write(player + 0x2c, lo); m.write(player + 0x30, hi);
    }
    if (p.game_object_00561080(player) == 0) return;                        // 004B3FD2
    const Word creature = p.vcall0(p.game_object_00561080(player), 0x30);
    if (p.object_area_004cb120(creature) != 0) return;
    if (m.read(player + 0x34) == 0x7f000000u) return;
    m.write(player + 0x34, 0x7f000000u);
    (void)object_control_0056f430(m, p, m.read(internal + 0x10010), m.read(player + 4), 0x7f000000u, scratch);
    m.write(player + 0x2c, lo); m.write(player + 0x30, hi);
}

Word game_object_update_00578520(Memory& m, Ports& p, Word self, Word player, Word scratch) {
    return game_object_update_00578200(m, p, self, player, 0x7f000000u, scratch);
}

Word game_object_update_00578200(Memory& m, Ports& p, Word self, Word player, Word, Word scratch) {
    const Word out_data = scratch, out_size = scratch + 4, count = scratch + 8, inner = scratch + 0x10;
    (void)p.high_resolution_timer_005e9260();                              // 00578211 (result unused)
    const Word observer = p.game_object_00561080(player);
    const Word area = observer != 0 ? p.object_area_004cb120(observer) : 0;
    const Word array = object_array(m, p);
    if (array == 0) return 0;
    sm::message_begin_004d68a0(m, p, self, 0x400, m.read(player + 4), 1);
    bool overflow = false;
    delete_other_areas_0056df60(m, p, self, player, inner);
    (void)p.high_resolution_timer_005e9260();                              // 0057827C (result unused)
    Word stage = 0;
    if (write_length(m, self) >= 2000) overflow = true;
    else {
        stage = 1;
        if (area != 0 && observer != 0) compute_for_object_00578060(m, p, self, player, observer, array, m.read(observer + 4), inner);
        if (write_length(m, self) >= 2000) overflow = true;
        else {
            stage = 2;
            if (area != 0 && observer != 0) p.major_gui_panels_00577670(self, player);
            if (write_length(m, self) >= 2000) overflow = true;
        }
    }
    const Word list = sort_objects_00567850(m, p, self, player, observer, array, count, inner);
    auto category = [&](Word c) { return compute_for_category_00578130(m, p, self, c, 2000, player, observer, array, list, m.read(count), inner) != 0; };
    if (!overflow) {
        stage = 3;
        if (!category(1)) {
            stage = 4;
            minor_gui_panels_00572990(m, p, self, player);
            if (write_length(m, self) < 2000) {
                stage = 6;
                if (!category(3)) { stage = 7;
                if (!category(4)) { stage = 8;
                    p.party_ai_state_005779f0(self, player);
                    if (write_length(m, self) < 2000) { stage = 9;
                    if (!category(5)) { stage = 0x0a;
                    if (!category(6)) { stage = 0x0b;
                    if (!category(7)) { stage = 0x0c;
                    if (!category(8)) { stage = 0x0d;
                    if (!category(9)) stage = 0x0e; } } } } } } }
            }
        }
    }
    work_remaining_00567ba0(m, p, self, observer, area, stage, 0x0e);
    if (list != 0) p.free(list);
    if (sm::message_finish_004d6440(m, self, out_data, out_size) == 0) return 0;
    if (m.read(out_size) == 0) return 1;
    return sm::message_send_00567e60(m, p, self, m.read(player + 4), 5, 1, m.read(out_data), m.read(out_size));
}

Word object_control_0056f430(Memory& m, Ports& p, Word self, Word recipient, Word id, Word scratch8) {
    sm::message_begin_004d68a0(m, p, self, 4, 0xffffffffu, 1);
    write_object_id_00578570(m, p, self, id);
    if (sm::message_finish_004d6440(m, self, scratch8 + 4, scratch8) == 0) return 0;
    return sm::message_send_00567e60(m, p, self, recipient, 5, 2, m.read(scratch8 + 4), m.read(scratch8));
}

void compute_for_object_00578060(Memory& m, Ports& p, Word self, Word player, Word observer, Word array, Word id, Word scratch) {
    const Word out = scratch, last = scratch + 4, a = scratch + 8, b = scratch + 0x0c;
    if ((p.object_lookup_004d8230(array, id, out) & 0xffu) != 0) return;
    if (m.read(out) == 0) return;
    const Word object = p.vcall0(m.read(out), 0x10);
    const Word visible = p.test_object_visible_0056dc60(self, object, observer);
    m.write(object + 0x1f8, visible);
    if (visible == 1) {
        // The two flag outputs reuse the player/observer argument slots: they
        // start out holding those pointer values (preserved).
        m.write(a, player); m.write(b, observer); m.write(last, 0);
        p.test_update_differences_00577fa0(self, player, object, last, a, b);
        const Word bv = m.read(b);
        if (bv != 0 || m.read(a) != 0) p.update_object_00574a10(self, player, object, m.read(last), m.read(a), bv);
        p.store_values_00576fc0(self, player, m.read(last), object, m.read(a), bv);
    } else {
        p.delete_for_object_0056e190(self, player, m.read(object + 4));
    }
}

Word compute_for_category_00578130(Memory& m, Ports& p, Word self, Word category, Word limit, Word player, Word observer,
                                   Word array, Word list, Word count, Word scratch) {
    const Word out = scratch;
    m.write(out, 0);
    if (observer == 0 || p.object_area_004cb120(observer) == 0) {
        if (p.vcall0(observer, 0x30) == 0) return 0;                       // observer==0 faults in the original (domain)
        const Word module = server_module(m, p);
        if (module == 0) return 0;
        if (p.module_area_004c30f0(module) == 0) return 0;
    }
    if (static_cast<std::int32_t>(count) <= 0) return 0;
    Word entry = list;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(count); ++i, entry += 0x0c) {
        if (m.read(entry + 8) != category) continue;
        const Word id = m.read(entry);
        if ((p.object_lookup_004d8230(array, id, out) & 0xffu) != 0) continue;
        compute_for_object_00578060(m, p, self, player, observer, array, id, scratch + 4);
        if (write_length(m, self) >= limit) return 1;
    }
    return 0;
}

void work_remaining_00567ba0(Memory& m, Ports& p, Word self, Word object, Word area, Word stage, Word total) {
    const Word creature = p.vcall0(object, 0x30);
    if (creature == 0) return;
    if (area != 0 && m.read(creature + 0x320) != 0) return;
    if (stage == total && m.read(creature + 0x320) != 0) return;
    write_byte_004d62c0(m, p, self, 0x57, 8);                               // 'W'
    write_byte_004d62c0(m, p, self, stage, 8);
    write_byte_004d62c0(m, p, self, total, 8);
    if (stage == total) m.write(creature + 0x320, 1);
}

void minor_gui_panels_00572990(Memory& m, Ports& p, Word self, Word player) {
    const Word object = p.game_object_00561080(player);
    if (object != 0 && p.vcall0(object, 0x30) != 0) {
        const Word required = p.charsheet_required_00561260(m.read(player + 0x50), player);
        if (required != 0) p.write_charsheet_0056f490(self, player, required);
    }
    p.free(0);                                                              // local CExoArrayList data (always null)
}

void delete_other_areas_0056df60(Memory& m, Ports& p, Word self, Word player, Word scratch) {
    const Word slot = scratch, out = scratch + 4;
    const Word observer = p.game_object_00561080(player);
    const Word array = object_array(m, p);
    if (array == 0) return;
    auto internal = [&]() { return m.read(m.read(player + 0x0c)); };
    if (m.read(internal()) == 0) return;
    m.write(slot, m.read(internal()));
    do {
        const Word last = p.list_at_005e9b60(internal(), m.read(slot));
        if (observer != 0 && m.read(last + 0x2c) == m.read(observer + 4)) {
            m.write(last + 0x24, 0);
            p.list_next_005e9b90(internal(), slot);
            continue;
        }
        bool keep = false;
        if ((p.object_lookup_004d8230(array, m.read(last + 0x2c), out) & 0xffu) == 0 && p.vcall0(m.read(out), 0x10) != 0 && observer != 0) {
            const Word theirs = p.object_area_004cb120(p.vcall0(m.read(out), 0x10));
            const Word ours = p.object_area_004cb120(observer);
            if (theirs == ours) keep = true;
            else if (p.object_area_004cb120(p.vcall0(m.read(out), 0x10)) != 0 && p.object_area_004cb120(observer) == 0
                     && p.vcall0(observer, 0x30) != 0) {
                const Word area_id = m.read(p.object_area_004cb120(p.vcall0(m.read(out), 0x10)) + 0x120);
                if (area_id == m.read(p.vcall0(observer, 0x30) + 0x310)) keep = true;
            }
        }
        if (keep) { p.list_next_005e9b90(internal(), slot); continue; }
        write_byte_004d62c0(m, p, self, 0x44, 8);                           // 'D'
        write_byte_004d62c0(m, p, self, m.read8(last + 0x28), 8);
        write_object_id_00578570(m, p, self, m.read(last + 0x2c));
        const Word type = m.read8(last + 0x28);
        if (type == 5 || type == 9) {                                       // [00746634], [00746638]
            write_bool_004d68e0(m, p, self, 1);
            write_bool_004d68e0(m, p, self, m.read(last + 0x148));
            sm::message_word_004d6340(m, p, self, m.read(last + 0x150), 0x20);
            if (m.read8(last + 0x28) == 5) write_bool_004d68e0(m, p, self, m.read(last + 0x14c));
        } else if (type == 6) {                                             // [00746635]
            write_bool_004d68e0(m, p, self, 1);
        } else if (type == 7) {                                             // [00746636]
            write_byte_004d62c0(m, p, self, m.read8(last + 0x150), 8);
        }
        const Word old = m.read(slot);
        p.list_next_005e9b90(internal(), slot);
        p.list_remove_005e9c10(internal(), old);
        p.last_update_destroy_005ca6a0(last);
        p.free(last);
    } while (m.read(slot) != 0);
}

Word select_category_00567780(Memory& m, Ports& p, Word object, Word observer) {
    if (p.vcall0(object, 0x30) != 0) {
        const Word creature = p.vcall0(object, 0x30);
        if (m.read(observer + 4) == m.read(object + 4)) return 0;
        return m.read(creature + 0x4e0) == 1 ? 1u : 4u;
    }
    if (p.vcall0(object, 0x18) != 0) return 6;
    if (p.vcall0(object, 0x48) != 0) return m.read(p.vcall0(object, 0x48) + 0x398) != 0 ? 0xffu : 8u;
    if (p.vcall0(object, 0x38) != 0) return 7;
    if (p.vcall0(object, 0x50) != 0) return 9;
    if (p.vcall0(object, 0x40) != 0) return 5;
    return 0xff;
}

Word area_first_object_005071b0(Memory& m, Word area, Word out) {
    m.write(area + 0x19c, 0);
    if (m.read(area + 0x194) == 0) return 0;
    m.write(out, m.read(m.read(area + 0x190)));
    return 1;
}
Word area_next_object_005071e0(Memory& m, Word area, Word out) {
    const Word index = m.read(area + 0x19c) + 1;
    m.write(area + 0x19c, index);
    if (static_cast<std::int32_t>(index) >= static_cast<std::int32_t>(m.read(area + 0x194))) return 0;
    m.write(out, m.read(m.read(area + 0x190) + index * 4));
    return 1;
}

Word last_update_object_00561820(Memory& m, Ports& p, Word player, Word id, Word scratch4) {
    Word position = m.read(m.read(m.read(player + 0x0c)));
    m.write(scratch4, position);
    while (position != 0) {
        const Word last = p.list_at_005e9b60(m.read(m.read(player + 0x0c)), position);
        if (m.read(last + 0x2c) == id) return last;
        p.list_next_005e9b90(m.read(m.read(player + 0x0c)), scratch4);
        position = m.read(scratch4);
    }
    return 0;
}

Word sort_objects_00567850(Memory& m, Ports& p, Word self, Word player, Word observer, Word array, Word count_out, Word scratch) {
    (void)self;
    const Word id_slot = scratch, out = scratch + 4, inner = scratch + 8;
    Word count = 0, capacity = 0, data = 0, bytes = 0x180;
    std::int32_t last_offset = -12;
    Word area = 0, x0 = 0, y0 = 0;
    if (observer != 0) {
        area = p.object_area_004cb120(observer);
        x0 = m.read(observer + 0x90); y0 = m.read(observer + 0x94);
    }
    if (observer == 0 || area == 0) {
        const Word creature = p.vcall0(observer, 0x30);
        if (creature == 0) { m.write(count_out, 0); return 0; }
        const Word module = server_module(m, p);
        if (module == 0) { m.write(count_out, 0); return 0; }
        area = p.module_area_004c30f0(module);
        x0 = m.read(creature + 0x314); y0 = m.read(creature + 0x318);
        if (area == 0) { m.write(count_out, 0); return 0; }
    }
    if (area_first_object_005071b0(m, area, id_slot) != 1) { m.write(count_out, 0); return 0; }
    do {
        if ((p.object_lookup_004d8230(array, m.read(id_slot), out) & 0xffu) != 0) continue;
        const Word object = m.read(out);
        if (object == 0 || m.read8(object + 8) <= 4) continue;              // [00746633]
        const double dy = static_cast<double>(f32(m.read(object + 0x94))) - f32(y0);
        const double dx = static_cast<double>(f32(m.read(object + 0x90))) - f32(x0);
        const float d2 = static_cast<float>(dx * dx + dy * dy);            // FSTP m32
        Word include = 0;
        double range;
        if (p.vcall0(observer, 0x30) != 0) {
            const float spot = f32(p.spot_distance_004efc70(p.vcall0(observer, 0x30)));
            const float listen = f32(p.listen_distance_004efd20(p.vcall0(observer, 0x30)));
            const bool use_listen = listen > spot || listen != listen || spot != spot;  // FCOMP: C0=C3=0 or unordered
            range = use_listen ? f32(p.listen_distance_004efd20(p.vcall0(observer, 0x30)))
                               : f32(p.spot_distance_004efc70(p.vcall0(observer, 0x30)));
        } else {
            range = 250.0f;                                                 // 437A0000
        }
        if (less_ordered(d2, range * range)) include = 1;
        else if (last_update_object_00561820(m, p, player, m.read(observer + 4), inner) != 0) include = 1;  // observer's own id (quirk)
        const Word category = select_category_00567780(m, p, object, observer);
        if (include != 1 || category == 0xff) continue;
        if (capacity == count) {
            const Word fresh = p.allocate(bytes);
            if (fresh != 0)
                for (Word k = 0; k < capacity + 0x20; ++k) { m.write(fresh + k * 12, 0x7f000000u); m.write(fresh + k * 12 + 4, 0); m.write(fresh + k * 12 + 8, 0); }
            if (last_offset > -12) {
                for (Word k = 0; k < count; ++k) {
                    m.write(fresh + k * 12 + 4, m.read(data + k * 12 + 4));
                    m.write(fresh + k * 12 + 8, m.read(data + k * 12 + 8));
                    m.write(fresh + k * 12, m.read(data + k * 12));
                }
                p.free(data);
            }
            capacity += 0x20; bytes += 0x180; data = fresh;
        }
        std::int32_t k = static_cast<std::int32_t>(count) - 1;
        Word entry = data + static_cast<Word>(last_offset);
        while (k >= 0) {
            const Word entry_category = m.read(entry + 8);
            bool shift;
            if (category < entry_category) shift = true;
            else if (category != entry_category) shift = false;
            else { const double distance = f32(m.read(entry + 4)); shift = less_ordered(d2, distance * distance); }
            if (!shift) break;
            m.write(entry + 0x0c + 4, m.read(entry + 4));
            m.write(entry + 0x0c + 8, entry_category);
            m.write(entry + 0x0c, m.read(entry));
            --k; entry -= 0x0c;
        }
        const Word slot = data + static_cast<Word>((k + 1) * 12);
        m.write(slot + 8, category);
        m.write(slot + 4, bits32(static_cast<float>(std::sqrt(static_cast<double>(d2)))));
        m.write(slot, m.read(id_slot));
        ++count; last_offset += 12;
    } while (area_next_object_005071e0(m, area, id_slot) == 1);
    m.write(count_out, count);
    return data;
}
}
