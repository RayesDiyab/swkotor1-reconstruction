#pragma once
#include "9fed90f3bd-gff_write_storage.hpp"
namespace kotor::server_mainloop {
struct GffWriteCorePorts:GffStoragePorts {
 virtual Word string_literal(Word,Word)=0;virtual void string_destroy(Word)=0;virtual Word string_length(Word)=0;virtual Word string_char(Word,Word)=0;
 virtual void owned(Word)=0;virtual Word add_struct(Word,Word)=0;virtual Word add_field(Word,Word,Word,Word)=0;
 virtual Word grow_list(Word,Word,Word,Word)=0;virtual Word field_index(Word,Word,Word)=0;virtual Word field(Word,Word,Word)=0;virtual Word data(Word,Word,Word)=0;
};
Word gff_add_struct_00410d60(Memory&,GffStoragePorts&,Word,Word);
Word gff_initialize_00411260(Memory&,GffWriteCorePorts&,Word,Word,Word,Word,Word);
Word gff_scalar_write(Memory&,GffWriteCorePorts&,Word,Word,Word,Word,Word);
Word gff_add_list_00412450(Memory&,GffWriteCorePorts&,Word,Word,Word,Word);
Word gff_add_list_struct_004124e0(Memory&,GffWriteCorePorts&,Word,Word,Word,Word,Word);
}
