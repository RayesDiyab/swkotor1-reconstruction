#pragma once
// W2 tier 6 fork A: the CResGFF write primitives SaveCreature's chain needs that 0873 does not export.
// Reuse: every body runs on the EXISTING Memory-model GFF writer (build/1122/w/gff/srs, objects 0867-0870):
//   00410AA0 InitializeForWriting = gff_owned_storage_00410aa0 (port owned)
//   00411730 AddField             = gff_add_field_00411730     (port add_field)
//   00410D60 AddStruct            = gff_add_struct_00410d60    (port add_struct)
//   00410F50 AddDataField         = gff_grow_buffer(..., section 0) (port grow_data) -- the existing owner IS 00410F50:
//            same header/data guard, base = offset when offset+oldSize == used, grow when total >= capacity
//            (capacity = growth + total, growth *= 2), copy used bytes, relocate oldSize bytes, used := total.
//   00412870 WriteFieldFLOAT      = gff_scalar_write(type 8)   (raw dword at field+8, no masking: identical code shape
//            to the 0873-exported BYTE..INT writers)
// New bodies (original-faithful): 00412800 WriteFieldDWORD64, 00412C10 WriteFieldVOID, 00412D30 WriteFieldVector,
// 004125B0 AddStructToStruct.
#include "225ed224f7-gff_write_complex.hpp"
namespace w2::save_gff {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;
using kotor::server_mainloop::GffWriteComplexPorts;

// 00412870 (root*, float bits, label) RET 0Ch: field type 8
Word write_float_00412870(Memory&, GffWriteComplexPorts&, Word self, Word root, Word bits, Word label);
// 00412800 (root*, low, high, label) RET 10h: field type 6, 8 data bytes
Word write_dword64_00412800(Memory&, GffWriteComplexPorts&, Word self, Word root, Word low, Word high, Word label);
// 00412C10 (root*, data*, size, label) RET 10h: field type 0Dh, data block = DWORD size + size bytes
Word write_void_00412c10(Memory&, GffWriteComplexPorts&, Word self, Word root, Word data, Word size, Word label);
// 00412D30 (root*, Vector*, label) RET 0Ch: field type 11h, 12 data bytes
Word write_vector_00412d30(Memory&, GffWriteComplexPorts&, Word self, Word root, Word vector, Word label);
// 00410F50 (offset, old size, new size) RET 0Ch -> pointer into the field-data block (0 on failure)
Word add_data_field_00410f50(Memory&, GffWriteComplexPorts&, Word self, Word offset, Word old_size, Word new_size);
// 004125B0 (CResStruct* out, CResStruct* parent, label, struct type) RET 10h: field type 0Eh
Word add_struct_to_struct_004125b0(Memory&, GffWriteComplexPorts&, Word self, Word out, Word parent, Word label, Word type);
}
