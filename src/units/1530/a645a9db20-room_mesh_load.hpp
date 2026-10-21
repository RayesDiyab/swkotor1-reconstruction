#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RoomMeshLoadPorts{virtual ~RoomMeshLoadPorts()=default;virtual void ref_default(Word)=0;virtual void reference(Word,Word,Word)=0;virtual Word exists(Word,Word,Word)=0;virtual Word lookup(Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual void insert(Word,Word,Word,Word)=0;virtual Word demand(Word)=0;virtual Word binary(Word)=0;virtual void release(Word)=0;virtual void dump(Word,Word)=0;virtual Word data(Word)=0;virtual Word size(Word)=0;virtual Word parse_binary(Word,Word,Word)=0;virtual Word parse_text(Word,Word,Word,Word)=0;};
Word room_mesh_load_00596670(Memory&,RoomMeshLoadPorts&,Word,Word,Word);
}
