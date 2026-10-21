#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureTemplateFinishPorts {virtual ~CreatureTemplateFinishPorts()=default;
 virtual Word ai(Word)=0;virtual void priority(Word,Word,Word)=0;virtual Word equipped(Word,Word)=0;virtual Word inventory(Word,Word)=0;virtual Word item(Word,Word)=0;
 virtual Word module(Word)=0;virtual void tag(Word,Word,Word)=0;virtual Word factions(Word)=0;virtual Word faction(Word,Word)=0;virtual void member(Word,Word,Word)=0;
 virtual void empty(Word)=0;virtual Word text(Word)=0;virtual void format(Word,Word,Word,Word)=0;virtual void destroy(Word)=0;
 virtual void refresh(Word)=0;virtual Word allocate(Word)=0;virtual Word effect_construct(Word,Word)=0;virtual void creator(Word,Word)=0;virtual void effect_integer(Word,Word,Word)=0;virtual void apply(Word,Word,Word,Word)=0;
 virtual Word hp(Word,Word)=0;virtual void state(Word,Word)=0;virtual void finish(Word,Word)=0;
};
void creature_template_finish_004f1c40(Memory&,CreatureTemplateFinishPorts&,Word,Word);
}
