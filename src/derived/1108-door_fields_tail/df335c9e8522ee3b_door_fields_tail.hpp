#pragma once
#include "9e3f32cd7e2756fb_door_fields.hpp"
namespace kotor::server_mainloop {
struct DoorFieldTailPorts:DoorFieldPorts{
virtual Word word_value(Word,Word,Word,Word,Word)=0;virtual Word string_read(Word,Word,Word,Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void string_destroy(Word)=0;virtual void tag(Word,Word)=0;
virtual void ref_empty(Word)=0;virtual void ref_default(Word)=0;virtual Word ref_read(Word,Word,Word,Word,Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;virtual Word ref_raw(Word)=0;virtual void portrait_ref(Word,Word)=0;virtual void portrait_id(Word,Word)=0;
virtual Word localized_construct(Word)=0;virtual Word localized_read(Word,Word,Word,Word,Word,Word)=0;virtual void localized_assign(Word,Word)=0;virtual void localized_destroy(Word)=0;
virtual Word ref_from_string(Word,Word)=0;virtual Word ref_text(Word)=0;virtual void string_literal(Word,Word)=0;virtual void string_text(Word,Word)=0;virtual void string_zero(Word)=0;virtual Word string_empty(Word)=0;virtual Word string_equal_literal(Word,Word)=0;virtual void table_string(Word,Word,Word,Word)=0;virtual void table_integer(Word,Word,Word,Word)=0;virtual void assign_literal(Word,Word)=0;
virtual Word allocate(Word)=0;virtual Word mesh_construct(Word)=0;virtual void mesh_state(Word,Word)=0;virtual void mesh_load(Word,Word)=0;virtual void mesh_finish(Word)=0;virtual void finish(Word)=0;
};
Word door_fields_tail_0058a424(Memory&,DoorFieldTailPorts&,Word,Word,Word,Word,Word);
}
