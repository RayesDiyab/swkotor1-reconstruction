#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {struct EffectDispatchPorts{virtual ~EffectDispatchPorts()=default;virtual Word invoke(Word,Word,Word,Word,Word)=0;};Word effect_dispatch_004d8320(Memory&,EffectDispatchPorts&,Word,Word,Word,Word);}
