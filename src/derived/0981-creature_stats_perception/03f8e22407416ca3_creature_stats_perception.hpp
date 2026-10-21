#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsPerceptionPorts {virtual ~CreatureStatsPerceptionPorts()=default;virtual void string_literal(Word,Word)=0;virtual void string_destroy(Word)=0;virtual Word table_float(Word,Word,Word,Word)=0;virtual Word table_integer(Word,Word,Word,Word)=0;virtual Word byte_read(Word,Word,Word,Word,Word)=0;virtual void loc_destroy(Word)=0;virtual Word continuation_005b098b(Word,Word,Word,Word,Word)=0;};
Word creature_stats_perception_005b06b8(Memory&,CreatureStatsPerceptionPorts&,Word,Word,Word,Word,Word);
}
