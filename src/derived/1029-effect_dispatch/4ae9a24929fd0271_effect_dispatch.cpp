#include "383006a8f708aab4_effect_dispatch.hpp"
namespace kotor::server_mainloop {Word effect_dispatch_004d8320(Memory&m,EffectDispatchPorts&p,Word self,Word target,Word effect,Word mode){Word type=m.read16(effect+8);if(type>0&&type<110){Word fn=m.read(m.read(self+4)+type*4);if(fn)return p.invoke(fn,self,target,effect,mode);}return 1;}}
