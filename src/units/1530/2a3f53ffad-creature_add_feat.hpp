#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureAddFeatPorts{virtual ~CreatureAddFeatPorts()=default;virtual Word feat_info(Word,Word)=0;virtual void add_unique(Word,Word)=0;virtual Word allocate(Word)=0;virtual void grow(Word,Word)=0;virtual void byte_write(Word,Word)=0;};
void creature_add_feat_005aa810(Memory&,CreatureAddFeatPorts&,Word,Word);
}
