#pragma once
#include "e5ac54576a-gff_write_storage.hpp"
namespace kotor::server_mainloop {
struct GffOutputPorts:GffStoragePorts {
 virtual Word write(Word,Word,Word,Word)=0;virtual void string_literal(Word,Word)=0;virtual void string_destroy(Word)=0;
 virtual void file_construct(Word,Word,Word,Word)=0;virtual Word file_open(Word)=0;virtual void file_destroy(Word)=0;
};
void gff_pack_00412db0(Memory&,GffStoragePorts&,Word,Word,Word);
Word gff_write_stream_004113d0(Memory&,GffOutputPorts&,Word,Word,Word);
Word gff_write_file_00413030(Memory&,GffOutputPorts&,Word,Word,Word,Word);
}
