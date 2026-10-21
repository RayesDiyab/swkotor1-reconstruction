#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureEffectAddedPorts { virtual ~CreatureEffectAddedPorts()=default; virtual void ability_continuation_004ffb38(Word,Word)=0; };
void creature_effect_added_004ffaf0(Memory&,CreatureEffectAddedPorts&,Word,Word);
}
