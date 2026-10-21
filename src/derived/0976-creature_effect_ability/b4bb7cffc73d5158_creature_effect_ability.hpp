#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureEffectAbilityPorts{virtual ~CreatureEffectAbilityPorts()=default;virtual Word nonempty(Word,Word,Word)=0;};
// Specialized original case4, null target and the fixed arguments used by the six ability getters.
Word creature_effect_ability_004f3fe0(Memory&,CreatureEffectAbilityPorts&,Word,Word,Word);
}
