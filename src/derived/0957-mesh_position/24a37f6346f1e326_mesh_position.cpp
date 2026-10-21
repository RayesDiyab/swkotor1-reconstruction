#include "aca02343a18e65e3_mesh_position.hpp"
#include <cstring>
namespace recovered_mesh_position{static std::int32_t sign(U u){std::int32_t i;std::memcpy(&i,&u,4);return i;}
void base(U s,U v,Ports&p){if(p.read(s+0x4c))return;p.write(s+0x20,p.read(v));p.write(s+0x24,p.read(v+4));p.write(s+0x28,p.read(v+8));U count=p.read(s+0x50);p.write(s+4,1);if(sign(count)<=0)return;U i=0,offset=0;do{p.shift(s,p.read(s+0x54)+offset,s+0x20);++i;offset+=12;}while(sign(i)<sign(p.read(s+0x50)));}
void tree(U s,U index,U v,Ports&p){while(index!=0xffffffff){U offset=index*44;U node=p.read(s+0xac)+offset;p.shift(s,node+12,v);node=p.read(s+0xac)+offset;p.shift(s,node,v);node=p.read(s+0xac)+offset;tree(s,p.read(node+36),v,p);node=p.read(s+0xac)+offset;index=p.read(node+40);}}
void position(U s,U v,Ports&p){if(p.read(s+0x4c))return;base(s,v,p);if(p.read(s+0xb0))tree(s,p.read(s+0xd4),v,p);}
}
