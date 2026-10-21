#pragma once
#include "945ddcc0b1-gff_write_storage.hpp"
namespace kotor::server_mainloop {
// section 0: field data, 1: field indices, 2: list indices.
Word gff_grow_buffer(Memory&,GffStoragePorts&,Word,Word,Word,Word,unsigned);
Word gff_add_label_00410e20(Memory&,GffStoragePorts&,Word,Word);
Word gff_add_field_00411730(Memory&,GffStoragePorts&,Word,Word,Word,Word);
}
