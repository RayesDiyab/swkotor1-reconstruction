#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureCombatLoadPorts{virtual ~CreatureCombatLoadPorts()=default;virtual Word structure(Word,Word,Word,Word)=0;virtual void present(Word,Word,Word,Word)=0;};
void creature_combat_load_00552350(CreatureCombatLoadPorts&,Word,Word,Word,Word);
}
