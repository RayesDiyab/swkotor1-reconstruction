#include "d6bf59291bffdf09_effect_handler_004d9200.hpp"
namespace kotor::server_mainloop {Word effect_handler_004d9200(Memory&m,EffectHandlerPorts&p,Word target,Word,Word){if(p.creature(target))m.write(p.creature(target)+0x344,1);return 0;}}
