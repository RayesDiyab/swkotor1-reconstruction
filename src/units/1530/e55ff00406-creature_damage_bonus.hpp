#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureDamagePorts {virtual ~CreatureDamagePorts()=default;
 virtual Word equipped(Word,Word)=0;virtual Word base(Word,Word)=0;virtual Word rules(Word)=0;virtual Word feat(Word,Word)=0;virtual Word property(Word,Word,Word,Word)=0;
 virtual Word positive_scale(Word,Word)=0;
};
Word creature_melee_damage_005a7a20(Memory&,CreatureDamagePorts&,Word,Word);
Word creature_ranged_damage_005a7db0(Memory&,CreatureDamagePorts&,Word,Word);
}
