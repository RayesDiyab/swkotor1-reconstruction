#pragma once
#include "2fd0d74c8d8b75b7_creature_stats_load.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsScalarPorts:CreatureStatsPorts{virtual void string_assign(Word,Word)=0;virtual Word ability(Word,Word)=0;virtual void recalculate_005a6d80(Word)=0;virtual Word continuation_005b024c(Word,Word,Word,Word,Word,Word)=0;};
Word creature_stats_scalars_005affbe(Memory&,CreatureStatsScalarPorts&,Word,Word,Word,Word,Word);
}
