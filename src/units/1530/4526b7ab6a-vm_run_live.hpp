#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop{
struct VmRunLivePorts{virtual ~VmRunLivePorts()=default;virtual Word empty(Word)=0;virtual void clear(Word)=0;virtual Word load(Word,Word)=0;virtual void cstr(Word)=0;virtual void script_slot(Word,Word)=0;virtual Word execute(Word,Word)=0;virtual void delete_script(Word,Word)=0;virtual void cleanup(Word,Word)=0;};
Word vm_run_live_005d45d0(Memory&,VmRunLivePorts&,Word self,Word script,Word context,Word flag);
}
