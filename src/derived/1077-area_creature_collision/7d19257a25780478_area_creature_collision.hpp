#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaCreatureCollisionPorts{virtual ~AreaCreatureCollisionPorts()=default;virtual bool equal(Word,Word)=0;virtual Word radius(Word)=0;virtual Word creature(Word,Word)=0;virtual void geometry(Word,Word,Word)=0;virtual Word expand(Word,bool)=0;virtual Word index(Word,Word,Word)=0;virtual Word nonempty_004bc790(Word,Word,Word,Word,Word,Word)=0;};
Word area_creature_collision_004bc590(Memory&,AreaCreatureCollisionPorts&,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word);
}
