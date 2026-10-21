#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct SoundTemplatePorts{virtual ~SoundTemplatePorts()=default;virtual Word allocate(Word)=0;virtual Word construct(Word,Word,Word,Word)=0;virtual void root(Word,Word)=0;virtual Word fields(Word,Word,Word)=0;virtual void destroy(Word)=0;};
Word sound_template_005c94e0(Memory&,SoundTemplatePorts&,Word self,Word ref,Word rootScratch);
}
