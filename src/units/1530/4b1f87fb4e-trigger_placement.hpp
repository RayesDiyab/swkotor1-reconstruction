#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct TriggerPlacementPorts{virtual ~TriggerPlacementPorts()=default;virtual Word area(Word)=0;virtual void set_area(Word,Word)=0;virtual void remove(Word,Word)=0;virtual void erase(Word,Word)=0;virtual void position(Word,Word,Word,Word,Word)=0;virtual void string_zero(Word)=0;virtual void string_destroy(Word)=0;virtual void insert(Word,Word,Word)=0;virtual void grow(Word,Word)=0;virtual void bounds(Word,Word,Word,Word,Word)=0;};
void trigger_detach_0058ee30(Memory&,TriggerPlacementPorts&,Word self,Word scratch);
void trigger_placement_0058f030(Memory&,TriggerPlacementPorts&,Word self,Word area,Word x,Word y,Word z,Word flag,Word scratch);
}
