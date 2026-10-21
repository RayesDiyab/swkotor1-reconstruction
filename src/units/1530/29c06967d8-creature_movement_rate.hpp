#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureMovementPorts{virtual ~CreatureMovementPorts()=default;virtual void empty(Word)=0;virtual void literal(Word,Word)=0;virtual Word table_string(Word,Word,Word,Word)=0;virtual Word lower(Word,Word)=0;virtual Word equal(Word,Word)=0;virtual void destroy(Word)=0;virtual void table_float(Word,Word,Word,Word)=0;};
void creature_movement_rate_005a5680(Memory&,CreatureMovementPorts&,Word,Word,Word);
}
