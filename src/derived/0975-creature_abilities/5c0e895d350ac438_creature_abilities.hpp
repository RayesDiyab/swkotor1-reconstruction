#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureAbilityPorts{virtual ~CreatureAbilityPorts()=default;virtual Word effects(Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;virtual Word force_class(Word)=0;virtual Word slots(Word,Word,Word)=0;virtual void byte_write(Word,Word)=0;};
// Internal selector is fixed by each original-address wrapper, range0..5.
Word creature_ability(Memory&,CreatureAbilityPorts&,Word,Word,Word);
void creature_recalculate_005a6d80(Memory&,CreatureAbilityPorts&,Word,Word);
}
