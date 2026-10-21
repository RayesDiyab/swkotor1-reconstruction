#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {struct AreaFindRoomPorts{virtual ~AreaFindRoomPorts()=default;virtual Word offset_z(Word,bool)=0;virtual void hit_construct(Word,Word)=0;virtual Word ray(Word,Word,Word,Word,Word,Word,Word,Word)=0;};Word area_find_room_004bb600(Memory&,AreaFindRoomPorts&,Word,Word,Word,Word);}
