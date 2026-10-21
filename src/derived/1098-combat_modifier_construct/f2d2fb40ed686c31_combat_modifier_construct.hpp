#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CombatModifierPorts {virtual ~CombatModifierPorts()=default;virtual void byte(Word,Word)=0;};
Word combat_modifier_construct_0054f260(Memory&,CombatModifierPorts&,Word);
}
