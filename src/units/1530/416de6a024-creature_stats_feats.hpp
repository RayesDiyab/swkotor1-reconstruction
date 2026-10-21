#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsFeatPorts{virtual ~CreatureStatsFeatPorts()=default;virtual Word hp(Word,Word,Word)=0;virtual Word fp(Word)=0;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual void feat(Word,Word)=0;virtual void set_hp(Word,Word)=0;virtual Word continuation_005b1791(Word,Word,Word,Word,Word)=0;};
Word creature_stats_feats_005b1685(Memory&,CreatureStatsFeatPorts&,Word,Word,Word,Word,Word);
}
