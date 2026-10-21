#include "913ae857a3-w2_message.hpp"
#include "2f0774e610-client_loadbar.hpp"
// Bodies follow the original instruction order; comments cite the original
// instruction addresses. Growth always goes through the existing
// ExtendWriteBuffer owner (module_message message_grow_004d6260).
namespace w2::msg {
using kotor::server_mainloop::message_grow_004d6260;
using kotor::server_mainloop::message_word_004d6340;

namespace {
void advance(Memory& m, Word self, Word width) {
    // "mov ecx,[esi+0C]; mov eax,[esi+1C]; add ecx,w; add eax,w; store both"
    const Word index = m.read(self + 0x0c) + width;
    const Word counter = m.read(self + 0x1c) + width;
    m.write(self + 0x0c, index);
    m.write(self + 0x1c, counter);
}
void ensure(Memory& m, Ports& p, Word self, Word width) {
    if (m.read(self + 0x0c) + width > m.read(self + 0x08)) message_grow_004d6260(m, p, self, width);
}
}

Word x87_single_roundtrip(Word bits) {
    // fld m32 of a signaling NaN sets the quiet bit (masked #IA); fstp m32
    // stores it back unchanged otherwise (denormals and infinities exact).
    if ((bits & 0x7f800000u) == 0x7f800000u && (bits & 0x007fffffu) != 0 && (bits & 0x00400000u) == 0)
        return bits | 0x00400000u;
    return bits;
}

// 004D6170 ClearReadMessage
void clear_read_004d6170(Memory& m, Word self) { m.write(self + 0x30, m.read(self + 0x2c)); }

// 004D6180 ReadCResRef (thiscall, hidden result, size; RET 8)
Word read_resref_004d6180(Memory& m, Ports& p, Word self, Word result, Word size, Word scratch) {
    Word count = size;
    if (static_cast<std::int32_t>(count) > 0x10) count = 0x10;            // 004D6189 signed JLE
    const Word limit = m.read(self + 0x2c), position = m.read(self + 0x30);
    const Word end = position + count;
    if (end > limit) {                                                     // 004D61A0 JBE
        m.write(self + 0x30, end);                                         // 004D61AD
        p.ref_assign_text_00406290(result, scratch + 0x10, p.empty_text_0073d71c()); // 004D61B0 via 00406D80
        return result;
    }
    p.copy(scratch, m.read(self + 0x28) + position, count);                // 004D61CE..D5
    m.write(self + 0x30, end);                                             // 004D61DB
    p.ref_assign_raw_00406170(result, scratch + 0x10, scratch);           // 004D61E5 via 00406DA0
    return result;
}

// 004D6200 ReadVOIDPtr (RET 4): cursor always advances.
Word read_voidptr_004d6200(Memory& m, Word self, Word size) {
    const Word position = m.read(self + 0x30), end = position + size;
    m.write(self + 0x30, end);                                             // 004D620E (before the compare branch)
    if (end > m.read(self + 0x2c)) return 0;
    return m.read(self + 0x28) + position;
}

// 004D6250 MessageMoreDataToRead
Word more_data_004d6250(Memory& m, Word self) { return m.read(self + 0x30) < m.read(self + 0x2c) ? 1u : 0u; }

// 004D66F0 ReadBOOL (no stack argument; plain RET): first byte of 4, not normalized.
Word read_bool_004d66f0(Memory& m, Word self) {
    const Word position = m.read(self + 0x30), end = position + 4;
    if (end > m.read(self + 0x2c)) { m.write(self + 0x30, end); return 0; }
    const Word value = m.read8(m.read(self + 0x28) + position);
    m.write(self + 0x30, end);
    return value;
}
// 004D6720 ReadBYTE (RET 4)
Word read_byte_004d6720(Memory& m, Word self, Word) {
    const Word position = m.read(self + 0x30), end = position + 1;
    if (end > m.read(self + 0x2c)) { m.write(self + 0x30, end); return 0; }
    const Word value = m.read8(m.read(self + 0x28) + position);
    m.write(self + 0x30, end);
    return value;
}
// 004D6750 ReadWORD (RET 4)
Word read_word16_004d6750(Memory& m, Word self, Word) {
    const Word position = m.read(self + 0x30), end = position + 2;
    if (end > m.read(self + 0x2c)) { m.write(self + 0x30, end); return 0; }
    const Word value = m.read16(m.read(self + 0x28) + position);
    m.write(self + 0x30, end);
    return value;
}
// 004D67B0 ReadFLOAT (RET 8; two ignored arguments)
Word read_float_bits_004d67b0(Memory& m, Word self, Word, Word) {
    const Word position = m.read(self + 0x30), end = position + 4;
    if (end > m.read(self + 0x2c)) { m.write(self + 0x30, end); return 0; } // fld [0073D700] = +0.0
    const Word value = m.read(m.read(self + 0x28) + position);
    m.write(self + 0x30, end);
    return value;
}
// 004D67E0 ReadCExoString (hidden result, bits; RET 8)
Word read_string_004d67e0(Memory& m, Ports& p, Word self, Word result, Word, Word scratch) {
    const Word position = m.read(self + 0x30), limit = m.read(self + 0x2c);
    const Word after_length = position + 4;
    const Word length = after_length > limit ? 0u : m.read(m.read(self + 0x28) + position); // 004D680C..1B
    const Word end = after_length + length;
    m.write(self + 0x30, after_length);                                    // 004D6822
    if (end > limit) {                                                     // 004D6825 JBE
        m.write(self + 0x30, end);                                         // 004D6827
        p.str_literal_005e5a90(result, p.empty_text_0073d71c());           // 004D6835
        return result;
    }
    p.str_counted_005e5b70(scratch, m.read(self + 0x28) + after_length, length); // 004D6847
    m.write(self + 0x30, m.read(self + 0x30) + length);                    // 004D684C add [esi+30],edi
    p.str_copy_005e5b00(result, scratch);                                  // 004D6862
    p.str_destroy_005e5c20(scratch);                                       // 004D6878
    return result;
}

// 004D62C0 WriteBYTE (value, bits; RET 8)
void write_byte_004d62c0(Memory& m, Ports& p, Word self, Word value, Word) {
    ensure(m, p, self, 1);
    p.write_byte(m.read(self + 0x04) + m.read(self + 0x0c), value & 0xffu);
    advance(m, self, 1);
}
// 004D6300 WriteWORD (value, bits; RET 8)
void write_word16_004d6300(Memory& m, Ports& p, Word self, Word value, Word) {
    ensure(m, p, self, 2);
    m.write16(m.read(self + 0x04) + m.read(self + 0x0c), value & 0xffffu);
    advance(m, self, 2);
}
// 004D6380 WriteCResRef (CResRef by value, size; RET 14h)
void write_resref_004d6380(Memory& m, Ports& p, Word self, Word ref, Word size) {
    if (m.read(self + 0x0c) + size > m.read(self + 0x08)) message_grow_004d6260(m, p, self, size); // 004D6390..99
    // 004D63A2 self_return(&ref) yields the by-value copy's address.
    p.copy(m.read(self + 0x04) + m.read(self + 0x0c), ref, size);          // 004D63A7..BF
    advance(m, self, size);
}
// 004D63E0 WriteVOIDPtr (source, size; RET 8)
void write_voidptr_004d63e0(Memory& m, Ports& p, Word self, Word source, Word size) {
    if (m.read(self + 0x0c) + size > m.read(self + 0x08)) message_grow_004d6260(m, p, self, size);
    p.copy(m.read(self + 0x04) + m.read(self + 0x0c), source, size);
    advance(m, self, size);
}
// 004D68E0 WriteBOOL (value; RET 4): all four bytes, not normalized.
void write_bool_004d68e0(Memory& m, Ports& p, Word self, Word value) {
    ensure(m, p, self, 4);
    m.write(m.read(self + 0x04) + m.read(self + 0x0c), value);
    advance(m, self, 4);
}
// 004D6920 WriteFLOAT (value, scale, bits; RET 0Ch)
void write_float_bits_004d6920(Memory& m, Ports& p, Word self, Word value_bits, Word, Word) {
    ensure(m, p, self, 4);
    m.write(m.read(self + 0x04) + m.read(self + 0x0c), x87_single_roundtrip(value_bits));
    advance(m, self, 4);
}
// 004D6960 WriteCExoString (CExoString*, bits; RET 8)
void write_string_004d6960(Memory& m, Ports& p, Word self, Word str, Word) {
    const Word length = p.str_length_005e5790(str);                        // 004D696C
    ensure(m, p, self, 4);                                                 // 004D6976..87
    m.write(m.read(self + 0x04) + m.read(self + 0x0c), length);            // 004D6992 (ECX=index, EDX=buffer)
    const Word index = m.read(self + 0x0c) + 4, counter = m.read(self + 0x1c) + 4;
    const Word capacity = m.read(self + 0x08);
    m.write(self + 0x0c, index);                                           // 004D69A0
    m.write(self + 0x1c, counter);                                         // 004D69A9
    if (index + length > capacity) message_grow_004d6260(m, p, self, length); // 004D69A7..B1
    const Word text = p.str_cstr_005e5670(str);                            // 004D69B8
    p.copy(m.read(self + 0x0c) + m.read(self + 0x04), text, length);       // 004D69BD..D3
    advance(m, self, length);
}

// 00578570 CSWSMessage::WriteOBJECTIDServer (id; RET 4)
void write_object_id_00578570(Memory& m, Ports& p, Word self, Word id) {
    ensure(m, p, self, 4);
    Word value = id;
    if (value != 0x7f000000u) value |= 0x80000000u;                       // [00746644], [0074665C]
    m.write(m.read(self + 0x04) + m.read(self + 0x0c), value);
    advance(m, self, 4);
}
// 00692630 CSWCMessage::ReadOBJECTIDClient (no argument; plain RET)
Word read_object_id_00692630(Memory& m, Word self) {
    const Word position = m.read(self + 0x30), end = position + 4;
    if (end > m.read(self + 0x2c)) { m.write(self + 0x30, end); return 0; }
    const Word value = m.read(m.read(self + 0x28) + position);
    m.write(self + 0x30, end);
    return value;
}

// 00578540 CSWSMessage::ReadOBJECTIDServer (no argument; plain RET): DWORD & 7FFFFFFFh, 0 on overrun.
// The cursor advances by 4 on both paths (00578550 / 00578560).
Word read_object_id_server_00578540(Memory& m, Word self) {
    const Word position = m.read(self + 0x30), end = position + 4;
    if (end > m.read(self + 0x2c)) { m.write(self + 0x30, end); return 0; }
    const Word value = m.read(m.read(self + 0x28) + position) & 0x7fffffffu;
    m.write(self + 0x30, end);
    return value;
}

// 004B4050 CServerExoAppInternal::GetPlayerLanguage (player id; RET 4)
Word player_language_004b4050(Memory& m, LocPorts& p, Word internal, Word player_id, Word scratch4) {
    Word position = m.read(m.read(m.read(internal + 0x10064)));            // 004B4054..5C
    m.write(scratch4, position);
    while (position != 0) {
        const Word player = p.list_at_005e9b60(m.read(m.read(internal + 0x10064)), position);
        if (player != 0 && m.read(player + 4) == player_id) return m.read(player + 8);
        p.list_next_005e9b90(m.read(m.read(internal + 0x10064)), scratch4);
        position = m.read(scratch4);
    }
    return 0;
}
// 005EA240 CExoLocString::GetStringLoc (language, out, gender; RET 0Ch)
Word get_string_loc_005ea240(Memory& m, LocPorts& p, Word loc, Word language, Word out, Word gender) {
    const Word low = language == 0 ? 0u : (gender & 0xffu);                 // sete/dec/and/movzx
    return p.locinternal_get_005ec9c0(m.read(loc), low + language * 2, out);
}

// 005785C0 CSWSMessage::WriteCExoLocStringServer (CExoLocString*, gender; RET 8)
// scratch layout: +0 CExoString tmp (8), +8 language dword, +0C gender byte.
void write_locstring_005785c0(Memory& m, LocPorts& p, Word self, Word loc, Word gender, Word scratch) {
    const Word tmp = scratch, language_slot = scratch + 8, gender_slot = scratch + 0x0c;
    p.str_empty_005b3190(tmp);                                             // 005785E7
    auto emit_text = [&]() {                                               // 00578640..77 (and the three copies)
        const Word length = p.str_length_005e5790(tmp);
        if (length + m.read(self + 0x0c) + 4 > m.read(self + 0x08)) message_grow_004d6260(m, p, self, p.str_length_005e5790(tmp) + 4);
        write_bool_004d68e0(m, p, self, 0);
        write_string_004d6960(m, p, self, tmp, 0x20);
    };
    const Word server = p.server();
    if (server != 0) {                                                     // 005785F4
        const Word player_language = player_language_004b4050(m, p, m.read(server + 4), m.read(self + 0x4c), scratch + 0x10);
        m.write(language_slot, player_language);                           // 0057860D
        const Word module_language = module_language_004ae820(p.server());
        if (get_string_loc_005ea240(m, p, loc, m.read(language_slot), tmp, gender)) emit_text();
        else if (m.read(language_slot) != 0 && get_string_loc_005ea240(m, p, loc, 0, tmp, gender)) emit_text();
        else if (module_language != m.read(language_slot) && module_language != 0
                 && get_string_loc_005ea240(m, p, loc, module_language, tmp, gender)) emit_text();
        else if (p.get_string_005e9eb0(loc, 0, language_slot, tmp, gender_slot)) emit_text();
        else if (m.read(loc + 4) != 0xffffffffu) {                         // 005787C9
            if (m.read(self + 0x0c) + 9 > m.read(self + 0x08)) message_grow_004d6260(m, p, self, 9);
            write_bool_004d68e0(m, p, self, 1);
            write_byte_004d62c0(m, p, self, gender, 8);
            message_word_004d6340(m, p, self, m.read(loc + 4), 0x20);   // existing WriteDWORD owner
        } else {                                                           // 0057880E: no +4 slack
            if (p.str_length_005e5790(tmp) + m.read(self + 0x0c) > m.read(self + 0x08)) message_grow_004d6260(m, p, self, p.str_length_005e5790(tmp));
            write_bool_004d68e0(m, p, self, 0);
            write_string_004d6960(m, p, self, tmp, 0x20);
        }
    }
    p.str_destroy_005e5c20(tmp);                                           // 00578853
}

// 00692660 CSWCMessage::ReadCExoLocStringClient (hidden result, CResRef* sound, DWORD* length_out; RET 0Ch)
// scratch: +00 unused CExoString (constructed/destroyed), +08 temporary CResRef result (16),
//          +18 STR_RES {+18 CExoString text, +20 CResRef sound, +30 dword length}, +40 gender dword.
Word read_locstring_00692660(Memory& m, LocPorts& p, Word self, Word result, Word sound, Word length_out, Word scratch) {
    const Word unused = scratch, ref_result = scratch + 0x08, entry = scratch + 0x18, gender_word = scratch + 0x40;
    p.str_empty_005b3190(unused);                                          // 00692688
    p.ref_assign_text_00406290(sound, ref_result, p.empty_text_0073d71c()); // 006926A5
    if (length_out != 0) m.write(length_out, 0);                           // 006926B2
    if (read_bool_004d66f0(m, self) == 0) {                                // 006926B6
        read_string_004d67e0(m, p, self, result, 0x20, entry);             // 0069277C (own temporary)
        p.str_destroy_005e5c20(unused);
        return result;
    }
    p.str_empty_005b3190(entry);                                           // 006926C7
    p.ref_default_00405ed0(entry + 8);                                     // 006926D5
    // 006926EC: the gender byte overwrites the low byte of the original
    // sound-pointer argument slot; the high bytes keep that pointer's bytes.
    m.write(gender_word, (sound & 0xffffff00u) | read_byte_004d6720(m, self, 8));
    const Word ref = kotor::server_mainloop::message_read_word_004d6780(m, self, 0x20); // 006926F0 existing ReadDWORD owner
    if (p.tlk_fetch_0041e550(p.tlk(), ref, entry, m.read(gender_word)) == 0) { // 00692706
        p.str_literal_005e5a90(result, p.empty_text_0073d71c());           // 0069271A
    } else {
        p.ref_assign_lower_00406120(sound, ref_result, entry + 8);           // 0069272D
        if (length_out != 0) m.write(length_out, m.read(entry + 0x18));    // 0069273A [esp+44]
        p.str_copy_005e5b00(result, entry);                                // 00692747
    }
    p.str_destroy_005e5c20(entry);                                         // 0069275D
    p.str_destroy_005e5c20(unused);                                        // 0069276A
    return result;
}
}
