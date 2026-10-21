#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AddKnownSpellPorts { virtual ~AddKnownSpellPorts()=default; virtual Word spell(Word,Word)=0; virtual Word force_class(Word)=0; virtual void add(Word,Word,Word)=0; };
void creature_add_known_spell_005aa9b0(Memory&,AddKnownSpellPorts&,Word,Word,Word);
}
