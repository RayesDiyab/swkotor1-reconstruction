#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LayoutRoomPorts{
 virtual ~LayoutRoomPorts()=default;virtual void base_construct(Word)=0;virtual Word vtable()=0;virtual Word allocate(Word)=0;virtual Word mesh_construct(Word)=0;virtual void mesh_destroy(Word)=0;virtual void free(Word)=0;virtual void base_destroy(Word)=0;virtual void reference_assign(Word,Word,Word)=0;virtual Word mesh_load(Word,Word)=0;virtual void mesh_position(Word,Word)=0;virtual Word overlap(Word,Word,Word,Word,Word)=0;virtual Word connect(Word,Word,Word,Word,Word,Word)=0;virtual void finish(Word)=0;
};
Word layout_room_construct_005793b0(Memory&,LayoutRoomPorts&,Word);
void layout_room_destroy_005794b0(Memory&,LayoutRoomPorts&,Word);
Word layout_room_load_00579520(Memory&,LayoutRoomPorts&,Word,Word,Word);
void layout_room_bounds_005796b0(Memory&,LayoutRoomPorts&,Word);
Word layout_room_overlap_005796d0(Memory&,LayoutRoomPorts&,Word,Word,Word,Word);
Word layout_room_connect_00579740(Memory&,LayoutRoomPorts&,Word,Word,Word,Word,Word);
void layout_room_finish_00579730(Memory&,LayoutRoomPorts&,Word);
}
