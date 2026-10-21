#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaConstructorPorts {
 virtual ~AreaConstructorPorts()=default;
 virtual Word vtable(Word)=0; virtual Word literal(Word)=0; virtual void byte(Word,Word)=0;
 virtual void base_0058f6b0(Word)=0; virtual void ref_00405ed0(Word)=0;
 virtual void assign_00406290(Word,Word,Word)=0; virtual void object_004d7d60(Word,Word,Word)=0;
 virtual void small_00506d90(Word)=0; virtual void empty_005b3190(Word)=0; virtual void localized_005e9e10(Word)=0;
 virtual void strings_006fb55f(Word,Word,Word,Word,Word)=0; virtual void location_0059afe0(Word)=0;
 virtual Word allocate(Word)=0; virtual void resource_00506c30(Word,Word,Word)=0;
 virtual Word collision_005d0ce0(Word)=0; virtual void initialize_0058f800(Word)=0;
 virtual Word objects_004aed70(Word)=0; virtual void register_004d80e0(Word,Word,Word,Word)=0;
 virtual void insert_004d7e90(Word,Word,Word)=0; virtual Word effects_005c95a0(Word,Word)=0;
 virtual Word timer_004aede0(Word)=0; virtual void time_004ade40(Word,Word,Word)=0;
};
Word area_constructor_0050cf80(Memory&,AreaConstructorPorts&,Word,Word,Word,Word,Word);
}
