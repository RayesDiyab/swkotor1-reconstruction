#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct PropertyEffectPorts{virtual ~PropertyEffectPorts()=default;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual void string(Word,Word)=0;virtual Word table(Word,Word)=0;virtual void integer_read(Word,Word,Word,Word)=0;virtual void destroy(Word)=0;virtual void creator(Word,Word)=0;virtual void integer(Word,Word,Word)=0;virtual Word base(Word,Word)=0;virtual void copy(Word,Word,Word)=0;virtual void apply(Word,Word,Word,Word)=0;};
Word property_cost_table_005c49a0(Memory&,Word,Word);
Word property_effect_004e60c0(Memory&,PropertyEffectPorts&,Word,Word,Word,Word,Word,Word);
}
