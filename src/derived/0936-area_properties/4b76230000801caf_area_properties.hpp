#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
// Draft cohesive reconstruction of00508C50. Not native-admitted until all segments are reviewed.
struct AreaPropertyPorts {
 virtual ~AreaPropertyPorts()=default;
 virtual Word scalar(Word function,Word resource,Word root,Word label,Word status,Word fallback)=0;
 virtual void literal(Word,Word)=0;virtual void empty(Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;
 virtual Word string(Word,Word,Word,Word,Word,Word)=0;virtual Word lower(Word,Word)=0;
 virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual void element(Word,Word,Word,Word)=0;
 virtual Word allocate(Word)=0;virtual Word localized_construct(Word)=0;virtual Word localized(Word,Word,Word,Word,Word,Word)=0;virtual void localized_assign(Word,Word)=0;virtual void localized_destroy(Word)=0;virtual void append(Word,Word)=0;
 virtual void ref_literal(Word,Word)=0;virtual Word ref(Word,Word,Word,Word,Word,Word)=0;virtual void ref_string(Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;
 virtual void float_store(Word,Word,Word,Word,Word,Word,bool)=0;virtual Word ref_nonempty(Word)=0;virtual void ref_assign_literal(Word,Word,Word)=0;
 virtual void release(Word)=0; virtual void rooms_allocate(Word,Word)=0; virtual void free(Word)=0;
 virtual void copy_string(Word,Word)=0; virtual void append_owned_string(Word,Word)=0;
 virtual void ref_array_grow(Word,Word)=0;
 virtual Word float_bits(Word,Word,Word,Word,Word)=0;
 virtual Word global(Word)=0; virtual Word module(Word)=0; virtual Word cstr(Word)=0;
 virtual void format1(Word,Word,Word)=0; virtual void ref_from_string(Word,Word)=0;
 virtual Word exists(Word,Word,Word,Word)=0; virtual Word structure(Word,Word,Word,Word)=0;
 virtual Word field_type(Word,Word,Word,Word)=0;
 // Implements original x87 multiply/add, double store, floor helper and integer conversion.
 virtual Word map_float_integer(Word,Word,Word,Word,Word)=0;
 virtual void map_setup(Word,const Word*)=0;
 virtual Word party(Word)=0;virtual void party_clear(Word,Word)=0;virtual void party_restrict(Word,Word)=0;virtual void byte(Word,Word)=0;
};
Word area_properties_header(Memory&,AreaPropertyPorts&,Word,Word,Word);
void area_properties_scripts(Memory&,AreaPropertyPorts&,Word,Word,Word);
void area_properties_environment(Memory&,AreaPropertyPorts&,Word,Word,Word);
void area_properties_rooms(Memory&,AreaPropertyPorts&,Word,Word,Word);
void area_properties_map(Memory&,AreaPropertyPorts&,Word,Word,Word);
Word area_properties_finish(Memory&,AreaPropertyPorts&,Word,Word,Word);
Word area_properties_00508c50(Memory&,AreaPropertyPorts&,Word,Word,Word);
}
