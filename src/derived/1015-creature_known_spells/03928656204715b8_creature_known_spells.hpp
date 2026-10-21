#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureKnownSpellPorts{virtual ~CreatureKnownSpellPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual void empty(Word)=0;virtual void format(Word,Word,Word,Word)=0;virtual Word text(Word)=0;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual void add(Word,Word,Word)=0;virtual void destroy(Word)=0;};
void creature_known_spells_005aeb30(Memory&,CreatureKnownSpellPorts&,Word,Word,Word,Word);
}
