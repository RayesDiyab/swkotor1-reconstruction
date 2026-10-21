#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaStepPorts {virtual ~AreaStepPorts()=default;virtual Word allocate(Word)=0;virtual Word construct_0050cf80(Word,Word,Word,Word)=0;virtual Word load_0050e190(Word,Word)=0;virtual void release_00409b80(Word)=0;virtual void release_path_004c44d0(Word,Word)=0;virtual void destroy(Word,Word)=0;virtual Word message_getter_004aedb0(Word)=0;virtual void message_date_0056c910(Word,Word,Word)=0;};
void party_reset_005639c0(Memory&,Word);
Word module_area_step_004c5720(Memory&,AreaStepPorts&,Word,Word,Word);
}
