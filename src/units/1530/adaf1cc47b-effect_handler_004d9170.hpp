#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct EffectHandlerPorts{virtual ~EffectHandlerPorts()=default;virtual Word creature(Word)=0;virtual Word integer(Word,Word)=0;virtual Word lookup(Word,Word)=0;virtual Word protected_from(Word,Word,Word)=0;};
Word effect_handler_004d9170(Memory&,EffectHandlerPorts&,Word,Word,Word);
}
