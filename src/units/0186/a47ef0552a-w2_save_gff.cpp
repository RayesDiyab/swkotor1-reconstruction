#include "88c689318a-w2_save_gff.hpp"
namespace w2::save_gff {
namespace {
constexpr Word kNone = 0xffffffffu;
// prologue shared by every CResGFF::WriteField*: null root / label -> 0; InitializeForWriting when [self+98h] != 0
Word field_for(Memory& m, GffWriteComplexPorts& p, Word self, Word root, Word label, Word type) {
    if (root == 0 || label == 0) return 0;
    if (m.read(self + 0x98) != 0) p.owned(self);                        // 00410AA0
    return p.add_field(self, root, label, type);                         // 00411730
}
}

Word write_float_00412870(Memory& m, GffWriteComplexPorts& p, Word self, Word root, Word bits, Word label) {
    return kotor::server_mainloop::gff_scalar_write(m, p, self, root, bits, label, 8);   // existing owner, type 8
}

Word write_dword64_00412800(Memory& m, GffWriteComplexPorts& p, Word self, Word root, Word low, Word high, Word label) {
    const Word field = field_for(m, p, self, root, label, 6);
    if (field == 0) return 0;
    const Word data = p.grow_data(self, kNone, 0, 8);                   // 0041283D AddDataField(-1, 0, 8)
    if (data == 0) return 0;
    m.write(field + 8, data - m.read(self + 0x5c));
    m.write(data, low); m.write(data + 4, high);
    return 1;
}

Word write_void_00412c10(Memory& m, GffWriteComplexPorts& p, Word self, Word root, Word src, Word size, Word label) {
    const Word field = field_for(m, p, self, root, label, 0x0d);
    if (field == 0) return 0;
    const Word data = p.grow_data(self, kNone, 0, size + 4);            // 00412C53 AddDataField(-1, 0, size+4)
    if (data == 0) return 0;
    m.write(field + 8, data - m.read(self + 0x5c));
    m.write(data, size);
    const Word dst = data + 4;                                           // 00412C76 rep movsd; rep movsb
    for (Word i = 0; i < (size >> 2); ++i) m.write(dst + 4 * i, m.read(src + 4 * i));
    for (Word i = size & ~3u; i < size; ++i) p.byte(dst + i, m.read8(src + i));
    return 1;
}

Word write_vector_00412d30(Memory& m, GffWriteComplexPorts& p, Word self, Word root, Word vector, Word label) {
    const Word field = field_for(m, p, self, root, label, 0x11);
    if (field == 0) return 0;
    const Word data = p.grow_data(self, kNone, 0, 12);                  // 00412D6D AddDataField(-1, 0, 0Ch)
    if (data == 0) return 0;
    const Word x = m.read(vector), y = m.read(vector + 4), z = m.read(vector + 8);   // read before the offset store
    m.write(field + 8, data - m.read(self + 0x5c));
    m.write(data, x); m.write(data + 4, y); m.write(data + 8, z);
    return 1;
}

Word add_data_field_00410f50(Memory&, GffWriteComplexPorts& p, Word self, Word offset, Word old_size, Word new_size) {
    return p.grow_data(self, offset, old_size, new_size);               // existing owner gff_grow_buffer(section 0)
}

Word add_struct_to_struct_004125b0(Memory& m, GffWriteComplexPorts& p, Word self, Word out, Word parent, Word label, Word type) {
    if (out == 0 || parent == 0 || label == 0) return 0;
    if (m.read(self + 0x98) != 0) p.owned(self);
    const Word field = p.add_field(self, parent, label, 0x0e);          // 004125E3
    if (field == 0) return 0;
    const Word index = p.add_struct(self, type);                        // 004125F5
    if (index == kNone) return 0;                                       // field keeps FFFFFFFFh (original)
    m.write(field + 8, index);
    m.write(out, index);
    return 1;
}
}
