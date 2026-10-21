#pragma once
#include "ffd821e460ba85cb_gff_write_core.hpp"
namespace kotor::server_mainloop {
struct GffWriteComplexPorts:GffWriteCorePorts {
 virtual Word grow_data(Word,Word,Word,Word)=0;virtual Word ref_length(Word)=0;virtual Word ref_text(Word)=0;virtual Word string_text(Word)=0;
 virtual void string_construct(Word)=0;virtual Word localized_count(Word)=0;virtual Word localized_length(Word,Word)=0;virtual Word localized_get(Word,Word,Word,Word,Word)=0;
};
Word gff_write_resref_004128c0(Memory&,GffWriteComplexPorts&,Word,Word,Word,Word);
Word gff_write_string_00412970(Memory&,GffWriteComplexPorts&,Word,Word,Word,Word);
Word gff_write_localized_00412a10(Memory&,GffWriteComplexPorts&,Word,Word,Word,Word,Word);
}
