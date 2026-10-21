#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct EffectApplyPorts{virtual ~EffectApplyPorts()=default;virtual Word timer(Word)=0;virtual void fields(Word,Word,Word)=0;virtual Word ai(Word)=0;virtual Word dispatch(Word,Word,Word,Word)=0;virtual Word integer(Word,Word)=0;virtual Word duration_first(Word,Word)=0;virtual Word duration_second(Word,Word)=0;virtual void combine(Word,Word,Word,Word,Word,Word,Word)=0;virtual void expiry(Word,Word,Word)=0;virtual void insert(Word,Word,Word)=0;virtual void changed(Word)=0;virtual void added(Word,Word)=0;virtual Word object(Word,Word)=0;virtual Word base(Word)=0;virtual void append(Word,Word)=0;virtual void remove(Word,Word,Word)=0;virtual void destroy(Word)=0;virtual void release(Word)=0;};
void effect_apply_004d14f0(Memory&,EffectApplyPorts&,Word,Word,Word,Word,Word);
}
