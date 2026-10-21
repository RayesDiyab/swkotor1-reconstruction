#pragma once
#include "c48c0aa5a96c08f2_room_mesh_binary.hpp"
namespace kotor::server_mainloop {
struct DoorMeshCallbackPorts {
 virtual ~DoorMeshCallbackPorts()=default;
 virtual void empty(Word)=0;
 virtual void ref_string(Word,Word)=0;
 virtual Word text(Word)=0;
 virtual void format(Word,Word,Word)=0;
 virtual void assign_string(Word,Word,Word)=0;
 virtual void destroy(Word)=0;
 virtual bool ordered_zero(Word)=0;
};
void door_mesh_reference_005ce8b0(Memory&,DoorMeshCallbackPorts&,Word self,Word out,Word scratch);
Word door_mesh_binary_005ce770(Memory&,DoorMeshCallbackPorts&,Word self,Word resource,Word scratch);
Word door_mesh_resource_vector_005ce010(Memory&,Word resource,Word out,Word index);
}
