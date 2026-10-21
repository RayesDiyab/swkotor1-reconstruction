#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LocalizedPorts {virtual ~LocalizedPorts()=default;virtual Word allocate(Word)=0;virtual void empty_005b3190(Word)=0;virtual void string_assign_005e5c50(Word,Word)=0;virtual void clear_005ecb10(Word)=0;virtual Word internal_construct_005ecaf0(Word)=0;virtual void append_005e9dc0(Word,Word)=0;virtual Word next_005e9b90(Word,Word)=0;virtual void loc_construct_005e9e10(Word)=0;virtual void loc_destroy_005ea000(Word)=0;virtual Word label_00411630(Word,Word,Word)=0;virtual Word field_00410990(Word,Word,Word)=0;virtual Word data_00410a20(Word,Word,Word)=0;virtual Word string_bytes_005e5b70(Word,Word,Word)=0;virtual void loc_add_005ea340(Word,Word,Word,Word)=0;virtual void string_destroy_005e5c20(Word)=0;};
Word localized_internal_copy_005ecbb0(Memory&,LocalizedPorts&,Word,Word,Word);
Word localized_assign_005e9e70(Memory&,LocalizedPorts&,Word,Word,Word);
Word localized_copy_construct_005e9f80(Memory&,LocalizedPorts&,Word,Word,Word);
Word gff_localized_00411fd0(Memory&,LocalizedPorts&,Word,Word,Word,Word,Word,Word,Word);
}
