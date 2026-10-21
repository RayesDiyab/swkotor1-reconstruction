#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureEffectSkillPorts{virtual ~CreatureEffectSkillPorts()=default;virtual Word nonempty(Word,Word,Word)=0;};
// Specialized original case5, null target and fixed skill-query arguments; nonempty effects remain explicit.
Word creature_effect_skill_004f3fe0(Memory&,CreatureEffectSkillPorts&,Word,Word,Word);
}
