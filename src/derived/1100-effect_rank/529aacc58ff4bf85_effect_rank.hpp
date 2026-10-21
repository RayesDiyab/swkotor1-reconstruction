#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct EffectRankPorts {virtual ~EffectRankPorts()=default;virtual Word integer(Word,Word)=0;virtual void logarithm(Word)=0;virtual void string(Word)=0;virtual void destroy(Word)=0;virtual void read_rank(Word,Word,Word,Word)=0;};
Word effect_rank_00501649(Memory&,EffectRankPorts&,Word current,Word existing,Word currentOut,Word existingOut,Word strings);
}
