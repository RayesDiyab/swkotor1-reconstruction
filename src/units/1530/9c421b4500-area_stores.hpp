#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaStorePorts{virtual ~AreaStorePorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word nonempty_005057fa(Word,Word,Word,Word,Word,Word)=0;};
Word area_stores_005057a0(Memory&,AreaStorePorts&,Word area,Word gff,Word root,Word saved,Word templates,Word scratch);
}
