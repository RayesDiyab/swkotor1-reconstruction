#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct EffectConstructPorts{virtual ~EffectConstructPorts()=default;virtual void empty(Word)=0;virtual Word allocate(Word)=0;virtual void zero(Word,Word)=0;virtual void literal(Word,Word)=0;};
Word effect_construct_00503e40(Memory&,EffectConstructPorts&,Word,Word);
void effect_integer_005036a0(Memory&,Word,Word,Word);
}
