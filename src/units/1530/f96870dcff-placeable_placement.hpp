#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct PlaceablePlacementPorts{virtual ~PlaceablePlacementPorts()=default;virtual Word area(Word)=0;virtual void remove_bounds(Word,Word)=0;virtual void remove_object(Word,Word)=0;virtual void set_area(Word,Word)=0;virtual void position(Word,Word,Word,Word,Word)=0;virtual void string_zero(Word)=0;virtual void string_construct(Word,Word)=0;virtual void string_destroy(Word)=0;virtual Word byte(Word)=0;virtual void table_integer(Word,Word,Word,Word)=0;virtual void table_string(Word,Word,Word,Word)=0;virtual void ref_from_string(Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;virtual Word mesh_load(Word,Word)=0;virtual Word yaw_scaled(Word)=0;virtual Word axis_angle(Word,Word,Word,Word,Word)=0;virtual void bounds(Word,Word,Word)=0;virtual void debug(Word,Word,Word,Word)=0;virtual Word register_bounds(Word,Word,Word,Word)=0;virtual void insert(Word,Word,Word)=0;};
void placeable_detach_00584950(Memory&,PlaceablePlacementPorts&,Word,Word);
void placeable_placement_00587170(Memory&,PlaceablePlacementPorts&,Word,Word,Word,Word,Word);
}
