#include "98980dc797009670_room_mesh_binary_guards.hpp"
namespace kotor::server_mainloop {
Word room_mesh_overlap_guard_00580d50(Memory&m,RoomMeshGuardPorts&p,Word self){if(m.read(self+0x4c))return 0;p.missing(0x00580d50);return 0;}
Word room_mesh_connect_guard_00580e90(Memory&m,RoomMeshGuardPorts&p,Word self){if(m.read(self+0x4c))return 0;p.missing(0x00580e90);return 0;}
void room_mesh_finish_guard_00596c10(Memory&m,RoomMeshGuardPorts&p,Word self){if(m.read(self+0x4c))return;p.missing(0x00596c10);}
}
