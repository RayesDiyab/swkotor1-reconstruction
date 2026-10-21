#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaDoorPorts{virtual ~AreaDoorPorts()=default;virtual Word list(Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word type(Word,Word)=0;virtual Word dword(Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual Word load(Word,Word,Word,Word)=0;virtual void destroy(Word)=0;virtual Word floating(Word,Word,Word,Word)=0;virtual void axis_angle(Word,Word,Word,Word,Word)=0;virtual void rotation(Word,Word,Word,Word,Word)=0;virtual void common(Word,Word,Word)=0;virtual void place(Word,Word,Word,Word,Word,Word)=0;virtual void append(Word,Word)=0;};
Word area_doors_0050a0e0(Memory&,AreaDoorPorts&,Word,Word,Word,Word,Word,Word);
}
