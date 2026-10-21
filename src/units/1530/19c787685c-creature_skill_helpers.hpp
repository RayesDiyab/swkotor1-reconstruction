#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureSkillHelperPorts{virtual ~CreatureSkillHelperPorts()=default;virtual Word incapacitated(Word)=0;virtual Word lookup(Word,Word,Word)=0;virtual Word area_virtual(Word,Word)=0;};
Word creature_skill_penalty_004ee210(Memory&,Word);
Word creature_dex_restricted_005b4880(Memory&,CreatureSkillHelperPorts&,Word);
Word server_area_004b18d0(Memory&,CreatureSkillHelperPorts&,Word,Word,Word);
}
