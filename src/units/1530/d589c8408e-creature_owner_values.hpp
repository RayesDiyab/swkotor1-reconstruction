#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureOwnerValuePorts{virtual ~CreatureOwnerValuePorts()=default;virtual Word party(Word)=0;virtual Word player(Word,Word)=0;virtual Word message(Word)=0;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual void decreased_experience(Word,Word,Word)=0;virtual void notify_experience(Word,Word,Word,Word)=0;};
Word creature_owner_value_004edd60(Memory&,CreatureOwnerValuePorts&,Word);
void creature_owner_set_004edd90(Memory&,CreatureOwnerValuePorts&,Word,Word);
void creature_experience_005af480(Memory&,CreatureOwnerValuePorts&,Word,Word,Word,Word);
}
