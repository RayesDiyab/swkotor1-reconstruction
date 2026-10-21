#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaPlaceablePorts{virtual ~AreaPlaceablePorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word type(Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual void ref_empty(Word)=0;virtual void ref_read(Word,Word,Word,Word,Word,Word)=0;virtual Word template_load(Word,Word,Word)=0;virtual Word load(Word,Word,Word,Word)=0;virtual void destroy(Word)=0;virtual Word floating(Word,Word,Word,Word,Word)=0;virtual void axis_angle(Word,Word,Word,Word,Word)=0;virtual void rotation(Word,Word)=0;virtual void common(Word,Word,Word)=0;virtual void place(Word,Word,Word,Word)=0;virtual void append(Word,Word)=0;};
Word area_placeables_0050a7b0(Memory&,AreaPlaceablePorts&,Word,Word,Word,Word,Word,Word);
}
