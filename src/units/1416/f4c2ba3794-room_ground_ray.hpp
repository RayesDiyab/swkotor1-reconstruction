#pragma once
#include "d2fbe17f4b-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RoomGroundRayPorts{virtual ~RoomGroundRayPorts()=default;virtual Word node(Word,Word,Word,Word,Word)=0;virtual void normalize(Word,Word)=0;virtual void scale(Word,Word)=0;virtual void add(Word,Word)=0;virtual void world_point(Word,Word,Word)=0;};
Word mesh_ground_ray_00581530(Memory&,RoomGroundRayPorts&,Word,Word,Word,Word,Word);
Word room_ground_ray_00579560(Memory&,RoomGroundRayPorts&,Word,Word,Word,Word,Word);
}
