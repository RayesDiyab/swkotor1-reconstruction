#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RoomWalkmeshPorts{virtual ~RoomWalkmeshPorts()=default;virtual Word vtable(Word)=0;virtual void ref_default(Word)=0;virtual void list_copy(Word,Word)=0;virtual void empty(Word)=0;virtual void literal(Word,Word)=0;virtual void destroy(Word)=0;virtual void table_string(Word,Word,Word,Word)=0;virtual void table_integer(Word,Word,Word,Word)=0;};
Word room_mesh_base_00596b80(Memory&,RoomWalkmeshPorts&,Word,Word,Word);
Word room_mesh_construct_00582ae0(Memory&,RoomWalkmeshPorts&,Word,Word);
}
