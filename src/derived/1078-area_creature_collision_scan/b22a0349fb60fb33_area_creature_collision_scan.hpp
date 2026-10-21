#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaCollisionScanPorts{virtual ~AreaCollisionScanPorts()=default;virtual Word creature(Word,Word)=0;virtual Word state(Word)=0;virtual Word incapacitated(Word)=0;virtual Word reaction(Word,Word)=0;virtual Word geometry_route(Word,Word)=0;virtual Word contact(Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;virtual void notify(Word,Word,Word,Word,Word,Word)=0;};
Word area_collision_scan_004bc790(Memory&,AreaCollisionScanPorts&,Word,Word,Word,Word);
}
