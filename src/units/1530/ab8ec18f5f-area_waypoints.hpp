#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaWaypointPorts{virtual ~AreaWaypointPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word type(Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual Word load(Word,Word,Word)=0;virtual void destroy(Word)=0;virtual void common(Word,Word,Word)=0;virtual Word floating(Word,Word,Word,Word,Word)=0;virtual void place(Word,Word,Word,Word,Word,Word)=0;virtual Word height(Word,Word,Word,Word)=0;virtual void position(Word,Word,Word,Word,Word)=0;};
Word area_waypoints_00505360(Memory&,AreaWaypointPorts&,Word area,Word gff,Word root,Word saved,Word templates,Word scratch);
}
