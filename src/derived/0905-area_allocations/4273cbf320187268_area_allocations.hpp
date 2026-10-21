#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaAllocationPorts {virtual ~AreaAllocationPorts()=default;virtual void free(Word)=0;virtual void byte(Word,Word)=0;virtual Word vtable(Word)=0;};
void collision_reset_005cfda0(Memory&,AreaAllocationPorts&,Word);
Word collision_construct_005d0ce0(Memory&,AreaAllocationPorts&,Word);
Word area_effects_005c95a0(Memory&,AreaAllocationPorts&,Word,Word);
}
