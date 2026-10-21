#include "a893224003794cd7_effect_type68.hpp"
namespace kotor::server_mainloop {Word effect_type68_004dccc0(Memory&m,EffectType68Ports&p,Word target,Word effect){if(!p.creature(target))return 1;Word value=p.integer(effect,0);Word records=m.read(m.read(0x7a3a28)+0xb0);return records+(value&65535)*52==0?1:0;}}
