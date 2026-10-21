#pragma once
#include <cstdint>
namespace recovered_loading_frame {
using U=std::uint32_t;
struct Ports {virtual ~Ports()=default;virtual U app()=0;virtual U read(U,U)=0;virtual void write(U,U,U)=0;virtual U connected(U)=0;virtual void receive(U,U)=0;virtual void status(U,U)=0;virtual U timeout()=0;virtual void shutdown(U)=0;virtual void pump(U,U,U)=0;virtual void input_delta(U,U)=0;};
// Full ordinary 00602C90. Reload the app before clearing failure state.
inline void stall(U self,Ports&p){U s=p.read(p.app(),0x14);if(p.read(s,0x10)==1){p.write(s,0x10,0);return;}if(p.read(s,0x14)==1){p.write(s,0x14,0);s=p.read(p.app(),0x14);p.write(s,0,0);p.write(s,4,0);if(p.read(self,0x264))p.shutdown(self);p.write(self,0x264,0);}}
// 00603291..00603354. -1 resumes the existing common frame path at00603355.
inline int advance(U self,Ports&p){if(!p.connected(p.read(p.app(),4))){p.receive(p.read(self,0x10),1);const U mode=p.timeout();p.status(p.read(self,0x10),mode);}stall(self,p);U s=p.read(p.app(),0x14);if(p.read(s,0)==0)return -1;const U mode=p.read(s,4);if(mode!=1&&mode!=3)return -1;p.write(self,0x288,1);p.pump(0x3d088889,0,1);s=p.read(p.app(),0x14);if(p.read(s,0x38)==4)p.input_delta(self,p.read(self,0x24));if(!p.read(self,0x250))return 0;return p.read(self,0x254)!=0?1:0;}
}
