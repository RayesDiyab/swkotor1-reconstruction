#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureSkillPorts{virtual ~CreatureSkillPorts()=default;virtual Word effects(Word,Word,Word)=0;virtual Word penalty(Word)=0;virtual Word armor(Word)=0;virtual Word area(Word,Word)=0;virtual Word feat(Word,Word)=0;};
Word creature_skill_005aa570(Memory&,CreatureSkillPorts&,Word,Word,Word,Word);
}
