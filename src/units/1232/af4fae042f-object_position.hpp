#pragma once
#include "fea6560366-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ObjectPositionPorts{virtual ~ObjectPositionPorts()=default;virtual Word equal(Word,Word)=0;virtual Word object_array(Word)=0;virtual void lookup(Word,Word,Word)=0;virtual Word virtual_call(Word,Word)=0;virtual void update_area(Word,Word)=0;};
void object_orientation_004cd170(Memory&,Word,Word);
void object_position_004cd1a0(Memory&,ObjectPositionPorts&,Word,Word,Word,Word,Word,Word);
}
