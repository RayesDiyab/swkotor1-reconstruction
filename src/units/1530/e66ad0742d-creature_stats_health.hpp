#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsHealthPorts{virtual ~CreatureStatsHealthPorts()=default;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual Word level(Word,Word)=0;virtual Word hitpoints(Word,Word)=0;virtual void set_hitpoints(Word,Word)=0;virtual Word forcepoints(Word)=0;virtual Word continuation_005b1685(Word,Word,Word,Word,Word)=0;};
Word creature_stats_health_005b1254(Memory&,CreatureStatsHealthPorts&,Word,Word,Word,Word,Word);
}
