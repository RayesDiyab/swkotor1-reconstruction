#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct TriggerFieldPorts {
 virtual ~TriggerFieldPorts()=default;
 virtual Word word_value(Word,Word,Word,Word,Word)=0;
 virtual Word dword(Word,Word,Word,Word,Word)=0;
 virtual Word integer(Word,Word,Word,Word,Word)=0;
 virtual Word byte(Word,Word,Word,Word,Word)=0;
 virtual Word floating(Word,Word,Word,Word,Word)=0;
 virtual void write_byte(Word,Word)=0;
 virtual void ref_empty(Word)=0;virtual void ref_default(Word)=0;virtual void ref_from_string(Word,Word)=0;
 virtual Word ref_read(Word,Word,Word,Word,Word,Word)=0;
 virtual void ref_to_string(Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;virtual Word ref_text(Word)=0;
 virtual void portrait_ref(Word,Word,Word)=0;virtual void portrait_id(Word,Word,Word)=0;
 virtual void owner(Word,Word)=0;
 virtual void string_zero(Word)=0;virtual void string_construct(Word,Word)=0;virtual void string_destroy(Word)=0;
 virtual Word string_read(Word,Word,Word,Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void assign_text(Word,Word)=0;
 virtual Word empty(Word)=0;virtual Word equal_text(Word,Word)=0;
 virtual Word table_string(Word,Word,Word,Word)=0;virtual Word table_integer(Word,Word,Word,Word)=0;
 virtual void tag(Word,Word)=0;
 virtual Word localized_construct(Word)=0;virtual Word localized_read(Word,Word,Word,Word,Word,Word)=0;
 virtual void localized_assign(Word,Word)=0;virtual void localized_destroy(Word)=0;
 virtual bool positive(Word)=0;virtual void normalize_if_needed(Word)=0;
 virtual Word angle(Word)=0;virtual void quaternion(Word,Word)=0;
 virtual void subtract_position(Word,Word,Word)=0;virtual Word inverse(Word,Word)=0;
 virtual Word rotate(Word,Word,Word)=0;virtual void translated_store(Word,Word,Word,Word)=0;
 virtual void position(Word,Word)=0;virtual void orientation(Word,Word)=0;
 virtual Word list(Word,Word,Word,Word)=0;virtual void geometry(Word,Word,Word)=0;
 virtual Word module(Word)=0;virtual void register_tag(Word,Word,Word)=0;
};
Word trigger_fields_0058da80(Memory&,TriggerFieldPorts&,Word self,Word gff,Word root,Word scratch);
}
