#include "03ef755c5f1a1e53_event_strings.hpp"
namespace recovered_event_strings {
U get(U self,U out,U index,Ports&p){const U count=p.read(self+0x20);if(static_cast<std::int32_t>(index)<static_cast<std::int32_t>(count)){const U data=p.read(self+0x1c);p.copy(out,data+index*8);}else p.empty(out);return out;}
void set(U self,U index,U source,Ports&p){const U count=p.read(self+0x20);if(static_cast<std::int32_t>(index)>=static_cast<std::int32_t>(count))p.resize(self+0x1c,index+1);const U data=p.read(self+0x1c);p.assign(data+index*8,source);}
}
