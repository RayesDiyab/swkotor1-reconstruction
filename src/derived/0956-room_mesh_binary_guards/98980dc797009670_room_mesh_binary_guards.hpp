#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RoomMeshGuardPorts{virtual ~RoomMeshGuardPorts()=default;virtual void missing(Word)=0;};
Word room_mesh_overlap_guard_00580d50(Memory&,RoomMeshGuardPorts&,Word);
Word room_mesh_connect_guard_00580e90(Memory&,RoomMeshGuardPorts&,Word);
void room_mesh_finish_guard_00596c10(Memory&,RoomMeshGuardPorts&,Word);
}
