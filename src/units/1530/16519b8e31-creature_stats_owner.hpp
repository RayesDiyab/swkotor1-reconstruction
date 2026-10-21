#pragma once
#include "435828129b-creature_stats_load.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsOwnerPorts:CreatureStatsPorts{virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual Word dword_read(Word,Word,Word,Word,Word)=0;virtual Word owner_value(Word)=0;virtual void owner_set(Word,Word)=0;virtual void set_experience(Word,Word,Word)=0;virtual Word continuation_005b03b4(Word,Word,Word,Word,Word)=0;};
Word creature_stats_owner_005b024c(Memory&,CreatureStatsOwnerPorts&,Word,Word,Word,Word,Word,Word);
}
