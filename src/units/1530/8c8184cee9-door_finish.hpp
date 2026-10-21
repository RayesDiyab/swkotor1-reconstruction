#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct DoorFinishPorts {
 virtual ~DoorFinishPorts()=default;
 virtual Word allocate(Word)=0;
 virtual Word construct(Word,Word)=0;
 virtual void integer(Word,Word,Word)=0;
 virtual void apply(Word,Word,Word,Word)=0;
 virtual void state_00589600(Word,Word,Word)=0;
 virtual void empty(Word)=0;
 virtual void literal(Word,Word)=0;
 virtual Word table_string(Word,Word,Word,Word)=0;
 virtual void destroy(Word)=0;
 virtual void assign_literal(Word,Word)=0;
 virtual Word compare(Word,Word)=0;
 virtual void animation(Word,Word)=0;
};
void door_finish_00589d40(Memory&,DoorFinishPorts&,Word self,Word scratch);
}
