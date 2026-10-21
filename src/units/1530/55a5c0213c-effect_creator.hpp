#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct EffectCreatorPorts{virtual ~EffectCreatorPorts()=default;virtual Word object(Word,Word)=0;virtual Word call(Word,Word)=0;};
void effect_creator_00503a00(Memory&,EffectCreatorPorts&,Word,Word);
}
