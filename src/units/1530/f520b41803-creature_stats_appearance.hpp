#pragma once
#include "16519b8e31-creature_stats_owner.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsAppearancePorts:CreatureStatsOwnerPorts{virtual void portrait(Word,Word,Word)=0;virtual void ref_text(Word)=0;virtual Word float_bits(Word,Word,Word,Word,Word)=0;virtual Word continuation_005b06b8(Word,Word,Word,Word,Word)=0;};
Word creature_stats_appearance_005b03b4(Memory&,CreatureStatsAppearancePorts&,Word,Word,Word,Word,Word);
}
