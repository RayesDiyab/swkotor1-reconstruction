#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaPathPorts{virtual ~AreaPathPorts()=default;virtual Word exists_00408bc0(Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct_00410630(Word,Word,Word,Word)=0;virtual void destroy(Word,Word)=0;virtual void root_00411240(Word,Word)=0;virtual Word list_004118c0(Word,Word,Word,Word)=0;virtual Word count_00411940(Word,Word)=0;virtual Word element_00411990(Word,Word,Word,Word)=0;virtual Word type_004111c0(Word,Word)=0;virtual Word float_bits_00411d00(Word,Word,Word,Word,Word)=0;virtual Word dword_00411c20(Word,Word,Word,Word,Word)=0;};
Word area_path_00508400(Memory&,AreaPathPorts&,Word,Word,Word);
}
