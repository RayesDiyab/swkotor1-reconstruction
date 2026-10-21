#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaPrimitivePorts {virtual ~AreaPrimitivePorts()=default;virtual Word vtable(Word)=0;virtual void byte(Word,Word)=0;virtual void ref_00405ed0(Word)=0;virtual Word allocate(Word)=0;virtual Word list_005e9aa0(Word)=0;};
Word area_base_0058f6b0(Memory&,AreaPrimitivePorts&,Word);
void area_initialize_0058f800(Memory&,Word);
Word area_small_00506d90(Memory&,AreaPrimitivePorts&,Word);
}
