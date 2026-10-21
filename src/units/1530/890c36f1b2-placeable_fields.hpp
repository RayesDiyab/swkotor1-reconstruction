#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct PlaceableFieldPorts{virtual ~PlaceableFieldPorts()=default;
 virtual void string_zero(Word)=0;virtual void string_construct(Word,Word)=0;virtual Word string_read(Word,Word,Word,Word,Word,Word)=0;virtual void string_assign(Word,Word)=0;virtual void string_destroy(Word)=0;virtual Word string_empty(Word)=0;virtual Word string_equal(Word,Word)=0;virtual void tag(Word,Word)=0;
 virtual void ref_empty(Word)=0;virtual Word ref_read(Word,Word,Word,Word,Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;virtual void ref_string(Word,Word)=0;
 virtual Word localized_construct(Word)=0;virtual Word localized_read(Word,Word,Word,Word,Word,Word)=0;virtual void localized_assign(Word,Word)=0;virtual void localized_destroy(Word)=0;
 virtual Word byte_read(Word,Word,Word,Word,Word)=0;virtual Word word_read(Word,Word,Word,Word,Word)=0;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word integer(Word,Word,Word,Word,Word)=0;virtual Word byte(Word)=0;virtual void write_byte(Word,Word)=0;
 virtual void hitpoints(Word,Word)=0;virtual void portrait_ref(Word,Word,Word)=0;virtual void portrait_id(Word,Word)=0;virtual Word light(Word)=0;virtual void light_set(Word,Word)=0;virtual void table_string(Word,Word,Word,Word)=0;virtual void state(Word,Word)=0;
 virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual void element(Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word item_construct(Word,Word)=0;virtual Word item_template(Word,Word)=0;virtual Word item_load(Word,Word,Word)=0;virtual void item_destroy(Word)=0;virtual void item_attach(Word,Word,Word,Word)=0;virtual Word module(Word)=0;virtual void register_tag(Word,Word,Word)=0;virtual void finish(Word)=0;
};
Word placeable_fields_00585670(Memory&,PlaceableFieldPorts&,Word self,Word gff,Word root,Word templates,Word scratch);
}
