#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleCompletePorts{virtual ~ModuleCompletePorts()=default;virtual void root(Word,Word)=0;virtual Word messages(Word)=0;virtual void date(Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word event(Word)=0;virtual Word ai(Word)=0;virtual void enqueue(Word,Word,Word,Word,Word,Word,Word)=0;virtual void release(Word)=0;virtual void debug(Word)=0;};
Word module_complete_004c5880(Memory&,ModuleCompletePorts&,Word self,Word scratch);
}
