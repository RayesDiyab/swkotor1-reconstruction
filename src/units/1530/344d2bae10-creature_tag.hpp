#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureTagPorts{virtual ~CreatureTagPorts()=default;virtual Word lower(Word,Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;};
void creature_tag_00553040(CreatureTagPorts&,Word,Word,Word);
}
