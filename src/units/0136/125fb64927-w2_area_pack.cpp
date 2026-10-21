#include "72239a78a7-w2_entry.hpp"
// 0050AAA0 CSWSArea::PackAreaIntoMessage and the leaves it needs to run:
// 005C9780, 005C89C0, 004C4100 and the CSWSScriptVarTable lookup family.
namespace w2::entry {
namespace sm = kotor::server_mainloop;
using namespace w2::msg;

void module_time_004c4100(Memory& m, Ports& p, Word module, Word a, Word b, Word c, Word d, Word byte_out, Word f) {
    m.write(a, m.read(module + 0x1a8));
    m.write(b, m.read(module + 0x1ac));
    m.write(c, m.read(module + 0x1b0));
    m.write(d, m.read(module + 0x1b4));
    p.write_byte(byte_out, m.read8(module + 0x1bc));
    m.write(f, m.read(module + 0x1b8));
}

Word var_match_0059a390(Memory& m, Ports& p, Word table, Word name, Word type, Word create) {
    if (static_cast<std::int32_t>(m.read(table + 4)) > 0) {
        for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(table + 4)); ++i) {
            const Word var = m.read(table) + i * 0x10;
            if (m.read(var + 8) == type && p.str_equal_005e5200(var, name) != 0) return (i << 4) + m.read(table);
        }
    }
    if (create == 0) return 0;
    return p.var_create_0059a3ef(table, name, type);                      // 0059A3EF..4BD
}
Word var_int_0059a530(Memory& m, Ports& p, Word table, Word name) {
    const Word var = var_match_0059a390(m, p, table, name, 1, 0);
    return var != 0 ? m.read(var + 0x0c) : 0;
}
Word var_float_bits_0059a560(Memory& m, Ports& p, Word table, Word name) {
    const Word var = var_match_0059a390(m, p, table, name, 2, 0);
    return var != 0 ? m.read(var + 0x0c) : 0;                             // fld [eax+0C] / fld 0.0 local
}
Word var_string_0059a590(Memory& m, Ports& p, Word table, Word result, Word name, Word scratch8) {
    p.str_empty_005b3190(scratch8);
    const Word var = var_match_0059a390(m, p, table, name, 3, 0);
    if (var != 0) p.str_assign_005e5c50(scratch8, m.read(var + 0x0c));
    p.str_copy_005e5b00(result, scratch8);
    p.str_destroy_005e5c20(scratch8);
    return result;
}
Word var_object_0059a620(Memory& m, Ports& p, Word table, Word name) {
    const Word var = var_match_0059a390(m, p, table, name, 4, 0);
    return var != 0 ? m.read(var + 0x0c) : 0x7f000000u;                   // [00746644]
}

void ambient_pack_005c9780(Memory& m, Ports& p, Word ambient, Word message) {
    write_bool_004d68e0(m, p, message, m.read(ambient + 0x04));
    write_bool_004d68e0(m, p, message, m.read(ambient + 0x14));
    write_bool_004d68e0(m, p, message, m.read(ambient + 0x1c));
    sm::message_word_004d6340(m, p, message, m.read(ambient + 0x08), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(ambient + 0x0c), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(ambient + 0x10), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(ambient + 0x18), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(ambient + 0x20), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(ambient + 0x24), 0x20);
    write_byte_004d62c0(m, p, message, m.read8(ambient + 0x28), 8);
    write_byte_004d62c0(m, p, message, m.read8(ambient + 0x29), 8);
}

void sound_pack_005c89c0(Memory& m, Ports& p, Word sound, Word message, Word scratch16) {
    const Word x = m.read(sound + 0x90), y = m.read(sound + 0x94), z = m.read(sound + 0x98);
    write_object_id_00578570(m, p, message, m.read(sound + 4));
    write_bool_004d68e0(m, p, message, m.read(sound + 0x228));
    write_bool_004d68e0(m, p, message, m.read(sound + 0x22c));
    write_bool_004d68e0(m, p, message, m.read(sound + 0x230));
    write_byte_004d62c0(m, p, message, m.read8(sound + 0x234), 8);
    write_byte_004d62c0(m, p, message, m.read8(sound + 0x238), 8);
    write_byte_004d62c0(m, p, message, m.read8(sound + 0x23c), 8);
    write_float_bits_004d6920(m, p, message, x87_single_roundtrip(m.read(sound + 0x240)), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, x87_single_roundtrip(m.read(sound + 0x248)), 0x3f800000, 0x20);
    sm::message_word_004d6340(m, p, message, m.read(sound + 0x244), 0x20);
    write_byte_004d62c0(m, p, message, m.read8(sound + 0x24c), 8);
    sm::message_word_004d6340(m, p, message, m.read(sound + 0x268), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(sound + 0x26c), 0x20);
    write_float_bits_004d6920(m, p, message, x87_single_roundtrip(m.read(sound + 0x270)), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, x87_single_roundtrip(m.read(sound + 0x274)), 0x3f800000, 0x20);
    write_bool_004d68e0(m, p, message, m.read(sound + 0x278));
    write_bool_004d68e0(m, p, message, m.read(sound + 0x27c));
    write_bool_004d68e0(m, p, message, m.read(sound + 0x250));
    write_float_bits_004d6920(m, p, message, x87_single_roundtrip(m.read(sound + 0x254)), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, x87_single_roundtrip(m.read(sound + 0x258)), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, x, 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, y, 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, z, 0x3f800000, 0x20);
    const Word count = m.read16(sound + 0x260);
    write_word16_004d6300(m, p, message, count, 0x10);
    for (Word i = 0, offset = 0; i < count; ++i, offset += 0x10) {
        const Word source = m.read(sound + 0x25c) + offset;
        for (Word k = 0; k < 16; k += 4) m.write(scratch16 + k, m.read(source + k));
        write_resref_004d6380(m, p, message, scratch16, 0x10);
    }
}

namespace {
// Inline CExoArrayList<DWORD>::Add as emitted in 0050AAA0 (three copies).
void list_add(Memory& m, Ports& p, Word list, Word value) {
    const Word data = list, count = list + 4, capacity = list + 8;
    if (m.read(count) == m.read(capacity)) {
        const Word old = m.read(data);
        const Word grown = m.read(capacity) != 0 ? m.read(capacity) * 2 : 0x10;
        m.write(capacity, grown);
        const Word fresh = p.allocate(grown * 4);
        m.write(data, fresh);
        for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(count)); ++i)
            m.write(fresh + i * 4, m.read(old + i * 4));
        if (old != 0) p.free(old);
    }
    m.write(m.read(data) + m.read(count) * 4, value);
    m.write(count, m.read(count) + 1);
}
void write_ref16(Memory& m, Ports& p, Word message, Word source, Word arg) {
    for (Word k = 0; k < 16; k += 4) m.write(arg + k, m.read(source + k));
    write_resref_004d6380(m, p, message, arg, 0x10);
}
}

Word pack_area_0050aaa0(Memory& m, Ports& p, Word area, Word a, Word b, Word c, Word player, Word scratch) {
    const Word s90 = scratch, s30 = scratch + 0x08, s5c = scratch + 0x10, s50 = scratch + 0x18, s70 = scratch + 0x20,
               out = scratch + 0x28, list_a = scratch + 0x30, list_b = scratch + 0x3c, list_d = scratch + 0x48,
               time = scratch + 0x58, arg = scratch + 0x78, loc = scratch + 0x88, str_tmp = scratch + 0xa0;
    const Word message = m.read(m.read(server_of(m, p) + 4) + 0x10010);    // 0050AAC9 GetSWSMessage
    p.str_empty_005b3190(s90);
    const Word objects = m.read(m.read(server_of(m, p) + 4) + 0x1005c);    // 0050AAEE GetObjectArray
    for (Word k = 0; k < 12; k += 4) { m.write(list_a + k, 0); m.write(list_b + k, 0); m.write(list_d + k, 0); }
    p.str_empty_005b3190(s30);
    p.str_empty_005b3190(s5c);
    auto cleanup = [&](Word result) {
        p.free(m.read(list_b));                                             // 0050B964 (frees null as well)
        p.free(m.read(list_d));
        p.str_destroy_005e5c20(s5c);
        p.str_destroy_005e5c20(s30);
        p.free(0);                                                          // unused list C (constant null)
        p.free(m.read(list_a));
        p.str_destroy_005e5c20(s90);
        return result;
    };
    if (message == 0) return cleanup(0);
    write_object_id_00578570(m, p, message, m.read(area + 0x120));
    write_ref16(m, p, message, area + 0x10c, arg);
    write_locstring_005785c0(m, p, message, area + 0x150, 0, loc);
    sm::message_word_004d6340(m, p, message, a, 0x20);
    sm::message_word_004d6340(m, p, message, b, 0x20);
    sm::message_word_004d6340(m, p, message, c, 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x04), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0xb8), 0x20);
    write_ref16(m, p, message, area + 0xbc, arg);
    write_byte_004d62c0(m, p, message, m.read8(area + 0x234), 8);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x68), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x6c), 0x20);
    write_byte_004d62c0(m, p, message, m.read8(area + 0x7c), 8);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x70), 0x20);
    write_float_bits_004d6920(m, p, message, m.read(area + 0x74), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, m.read(area + 0x78), 0x3f800000, 0x20);
    write_bool_004d68e0(m, p, message, m.read(area + 0x80));
    sm::message_word_004d6340(m, p, message, m.read(area + 0x84), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x88), 0x20);
    write_byte_004d62c0(m, p, message, m.read8(area + 0x98), 8);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x8c), 0x20);
    write_float_bits_004d6920(m, p, message, m.read(area + 0x90), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, m.read(area + 0x94), 0x3f800000, 0x20);
    write_bool_004d68e0(m, p, message, m.read(area + 0x9c));
    write_byte_004d62c0(m, p, message, m.read8(area + 0xa0), 8);
    write_byte_004d62c0(m, p, message, m.read8(area + 0xa4), 8);
    sm::message_word_004d6340(m, p, message, m.read(area + 0xac), 0x20);
    write_byte_004d62c0(m, p, message, m.read8(area + 0xb0), 8);
    write_byte_004d62c0(m, p, message, m.read8(area + 0xb4), 8);
    ambient_pack_005c9780(m, p, m.read(area + 0x208), message);           // 0050ADF0
    // 0050AE2E GetModule()->GetTime(&8C,&7C,&80,&78,&84b,&88)
    const Word t8c = time, t7c = time + 4, t80 = time + 8, t78 = time + 0x0c, t84 = time + 0x10, t88 = time + 0x14;
    m.write(t84, 0);                                                        // modelled: high bytes of the byte slot are not written
    module_time_004c4100(m, p, p.internal_module_004b14f0(m.read(server_of(m, p) + 4)), t8c, t7c, t80, t78, t84, t88);
    const Word moon = m.read8(p.internal_module_004b14f0(m.read(server_of(m, p) + 4)) + 0x1a1);
    const Word dawn = m.read8(p.internal_module_004b14f0(m.read(server_of(m, p) + 4)) + 0x1a2);
    write_byte_004d62c0(m, p, message, m.read(t84), 8);
    sm::message_word_004d6340(m, p, message, m.read(t88), 0x20);
    write_byte_004d62c0(m, p, message, m.read(t78), 8);
    write_byte_004d62c0(m, p, message, m.read(t80), 8);
    write_byte_004d62c0(m, p, message, m.read(t7c), 8);
    sm::message_word_004d6340(m, p, message, m.read(t8c), 0x20);
    write_byte_004d62c0(m, p, message, moon, 8);
    write_byte_004d62c0(m, p, message, dawn, 8);
    write_ref16(m, p, message, area + 0xec, arg);
    write_float_bits_004d6920(m, p, message, m.read(area + 0xd4), 0x3f800000, 0x20);
    write_float_bits_004d6920(m, p, message, m.read(area + 0xd8), 0x3f800000, 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0xd0), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0xcc), 0x20);
    for (Word k = 0; k < 4; ++k) write_float_bits_004d6920(m, p, message, m.read(area + 0xdc + k * 4), 0x3f800000, 0x20);
    write_byte_004d62c0(m, p, message, m.read8(area + 0xaa), 8);
    write_byte_004d62c0(m, p, message, m.read8(area + 0xab), 8);
    const Word weather = m.read8(area + 0x220);
    if (weather == 0xff) {
        write_byte_004d62c0(m, p, message, m.read8(area + 0x20c), 8);
        write_bool_004d68e0(m, p, message, m.read(area + 0x210));
    } else {
        write_byte_004d62c0(m, p, message, weather, 8);
        write_bool_004d68e0(m, p, message, 1);
    }
    write_float_bits_004d6920(m, p, message, m.read(area + 0xfc), 0x3f800000, 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x268), 0x20);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x268)); ++i) {
        write_string_004d6960(m, p, message, m.read(area + 0x25c) + i * 8, 0x20);
        sm::message_word_004d6340(m, p, message, m.read(m.read(area + 0x260) + i * 4), 0x20);
        write_float_bits_004d6920(m, p, message, m.read(m.read(area + 0x264) + i * 4), 0x3f800000, 0x20);
    }
    sm::message_word_004d6340(m, p, message, m.read(area + 0x26c), 0x20);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x26c)); ++i) {
        write_string_004d6960(m, p, message, m.read(area + 0x270) + i * 8, 0x20);
        write_bool_004d68e0(m, p, message, m.read(m.read(area + 0x27c) + i * 4));
        write_string_004d6960(m, p, message, m.read(area + 0x288) + i * 8, 0x20);
        write_string_004d6960(m, p, message, m.read(area + 0x294) + i * 8, 0x20);
        write_ref16(m, p, message, m.read(area + 0x2a0) + i * 0x10, arg);
    }
    sm::message_word_004d6340(m, p, message, m.read(m.read(p.internal_module_004b14f0(m.read(server_of(m, p) + 4)) + 0x218) + 0x10), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x08), 0x20);
    sm::message_word_004d6340(m, p, message, m.read(area + 0x0c), 0x20);
    // 0050B14F object scan: placeable-class objects (slot 58h, +228h set) to list A, sound objects (slot 68h) to list B.
    Word placed = 0;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x194)); ++i) {
        const Word id = m.read(m.read(area + 0x190) + i * 4);
        if ((p.object_lookup_004d8230(objects, id, out) & 0xffu) != 0) continue;
        const Word as_placeable = p.vcall0(m.read(out), 0x58);
        if (as_placeable != 0) {
            if (m.read(as_placeable + 0x228) == 0) continue;
            list_add(m, p, list_a, m.read(m.read(area + 0x190) + i * 4));
            ++placed;
        } else {
            if (p.vcall0(m.read(out), 0x68) == 0) continue;
            list_add(m, p, list_b, m.read(m.read(area + 0x190) + i * 4));
        }
    }
    sm::message_word_004d6340(m, p, message, placed, 0x20);
    for (Word j = 0; static_cast<std::int32_t>(j) < static_cast<std::int32_t>(placed); ++j) {
        if ((p.object_lookup_004d8230(objects, m.read(m.read(list_a) + j * 4), out) & 0xffu) != 0) continue;
        const Word object = p.vcall0(m.read(out), 0x58);
        if (object == 0) continue;
        write_object_id_00578570(m, p, message, m.read(object + 4));
        const Word x = m.read(object + 0x90), y = m.read(object + 0x94), z = m.read(object + 0x98);
        write_float_bits_004d6920(m, p, message, x, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, y, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, z, 0x3f800000, 0x20);
        write_bool_004d68e0(m, p, message, m.read(object + 0x22c));
        write_locstring_005785c0(m, p, message, object + 0x230, 0, loc);
    }
    // 0050B3B1 map pins from the player creature's script variables
    Word vars = 0;
    const Word game_object = p.game_object_00561080(player);
    if (game_object != 0) {
        const Word creature = p.vcall0(game_object, 0x30);
        if (creature != 0 && creature + 0x100 != 0) {
            vars = creature + 0x100;
            p.str_literal_005e5a90(s50, p.literal(0x00747b3c));            // "NW_TOTAL_MAP_PINS"
            const Word total = var_int_0059a530(m, p, vars, s50);
            if (total != 0) {
                for (Word k = 0; k <= total; ++k) {                         // JBE: inclusive upper bound (quirk)
                    p.str_format_int_005e5680(s30, p.literal(0x00747b28), k);  // "NW_MAP_PIN_NTRY_%i"
                    var_string_0059a590(m, p, vars, s70, s30, str_tmp);
                    p.str_assign_005e5c50(s5c, s70);
                    p.str_destroy_005e5c20(s70);
                    if (p.str_not_equal_text_005e5390(s5c, p.literal(0x0073d71c)) == 0) continue;
                    p.str_format_int_005e5680(s30, p.literal(0x00747b14), k);  // "NW_MAP_PIN_AREA_%i"
                    if (var_object_0059a620(m, p, vars, s30) != m.read(area + 0x120)) continue;
                    list_add(m, p, list_d, k);
                }
            }
            p.str_destroy_005e5c20(s50);
        }
    }
    const Word pins = m.read(list_d + 4);
    sm::message_word_004d6340(m, p, message, pins, 0x20);
    for (Word k = 0; static_cast<std::int32_t>(k) < static_cast<std::int32_t>(m.read(list_d + 4)); ++k) {
        const Word pin = m.read(m.read(list_d) + k * 4);
        sm::message_word_004d6340(m, p, message, pin, 0x20);
        p.str_format_int_005e5680(s30, p.literal(0x00747b28), m.read(m.read(list_d) + k * 4));
        var_string_0059a590(m, p, vars, s70, s30, str_tmp);
        p.str_assign_005e5c50(s5c, s70);
        p.str_destroy_005e5c20(s70);
        write_string_004d6960(m, p, message, s5c, 0x20);
        p.str_format_int_005e5680(s30, p.literal(0x00747b00), m.read(m.read(list_d) + k * 4)); // XPOS
        write_float_bits_004d6920(m, p, message, x87_single_roundtrip(var_float_bits_0059a560(m, p, vars, s30)), 0x3f800000, 0x20);
        p.str_format_int_005e5680(s30, p.literal(0x00747aec), m.read(m.read(list_d) + k * 4)); // YPOS
        write_float_bits_004d6920(m, p, message, x87_single_roundtrip(var_float_bits_0059a560(m, p, vars, s30)), 0x3f800000, 0x20);
        p.str_format_int_005e5680(s30, p.literal(0x00747ad8), m.read(m.read(list_d) + k * 4)); // ZPOS
        write_float_bits_004d6920(m, p, message, x87_single_roundtrip(var_float_bits_0059a560(m, p, vars, s30)), 0x3f800000, 0x20);
    }
    // 0050B6A7 sound objects
    const Word sounds = m.read(list_b + 4) & 0xffffu;
    write_word16_004d6300(m, p, message, m.read(list_b + 4), 0x10);
    for (Word k = 0; k < sounds; ++k) {
        if ((p.object_lookup_004d8230(objects, m.read(m.read(list_b) + k * 4), out) & 0xffu) != 0) continue;
        const Word sound = p.vcall0(m.read(out), 0x68);
        if (sound != 0) sound_pack_005c89c0(m, p, sound, message, arg);
    }
    // 0050B6F6 placeable lights and static placeables
    Word lights = 0, statics = 0;
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x194)); ++i) {
        const Word placeable = p.placeable_by_id_004ae7a0(server_of(m, p), m.read(m.read(area + 0x190) + i * 4));
        if (placeable == 0) continue;
        if (p.placeable_light_00585130(placeable) != 0) ++lights;
        if (m.read(placeable + 0x398) == 1) ++statics;
    }
    write_word16_004d6300(m, p, message, lights, 0x10);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x194)); ++i) {
        const Word placeable = p.placeable_by_id_004ae7a0(server_of(m, p), m.read(m.read(area + 0x190) + i * 4));
        if (placeable == 0 || p.placeable_light_00585130(placeable) == 0) continue;
        const Word x = m.read(placeable + 0x90), y = m.read(placeable + 0x94), z = m.read(placeable + 0x98);
        write_object_id_00578570(m, p, message, m.read(placeable + 4));
        write_byte_004d62c0(m, p, message, m.read8(placeable + 0x230), 8);
        write_float_bits_004d6920(m, p, message, x, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, y, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, z, 0x3f800000, 0x20);
    }
    write_word16_004d6300(m, p, message, statics, 0x10);
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(area + 0x194)); ++i) {
        const Word placeable = p.placeable_by_id_004ae7a0(server_of(m, p), m.read(m.read(area + 0x190) + i * 4));
        if (placeable == 0 || m.read(placeable + 0x398) != 1) continue;
        const Word x = m.read(placeable + 0x90), y = m.read(placeable + 0x94), z = m.read(placeable + 0x98);
        const Word ox = m.read(placeable + 0x9c), oy = m.read(placeable + 0xa0), oz = m.read(placeable + 0xa4);
        write_object_id_00578570(m, p, message, m.read(placeable + 4));
        write_byte_004d62c0(m, p, message, m.read8(placeable + 0x230), 8);
        write_float_bits_004d6920(m, p, message, x, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, y, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, z, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, ox, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, oy, 0x3f800000, 0x20);
        write_float_bits_004d6920(m, p, message, oz, 0x3f800000, 0x20);
    }
    return cleanup(1);
}
}
