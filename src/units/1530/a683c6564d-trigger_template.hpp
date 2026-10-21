#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct TriggerTemplatePorts{virtual ~TriggerTemplatePorts()=default;virtual Word allocate(Word)=0;virtual Word construct(Word,Word,Word,Word)=0;virtual void root(Word,Word)=0;virtual Word fields(Word,Word,Word)=0;virtual void destroy(Word)=0;};
Word trigger_template_0058ed70(Memory&,TriggerTemplatePorts&,Word self,Word ref,Word rootScratch);
}
