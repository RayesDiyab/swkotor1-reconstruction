#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct DoorBoundsPorts {
 virtual ~DoorBoundsPorts()=default;
 virtual Word bounds(Word,Word,Word)=0;
 virtual void debug(Word,Word,Word)=0;
 virtual bool less(Word,Word)=0;
 virtual bool greater(Word,Word)=0;
};
Word door_bounds_00589980(Memory&,DoorBoundsPorts&,Word self,Word x,Word y,Word z,Word outMin,Word outMax,Word scratch);
struct MeshBoundsPorts {
 virtual ~MeshBoundsPorts()=default;
 virtual Word transform(Word,Word,Word)=0;
 virtual bool less(Word,Word)=0;
 virtual bool greater(Word,Word)=0;
};
Word mesh_vertex_bounds_00596d10(Memory&,MeshBoundsPorts&,Word self,Word outMin,Word outMax,Word scratch);
}
