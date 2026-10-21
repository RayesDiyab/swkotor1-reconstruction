#include "f1bf23c4a8e4ab4c_creature_priority.hpp"
namespace recovered_creature_priority{void set(U s,U value,Ports&p){U v=value;if(v==0x2711||v==0x2710){U k=p.byte(s+0x4c4)-2;static const U table[]={0x274a,0x2718,0x2716,0x27a6,0x2785,0x278c,0x2786,0x28a0,0x28a1,0x2799,0x279a,0x28a2,0x27ac};v=k<13?table[k]:0x2710+(p.read(s+0x4e0)!=0);}else if(v==0xffffffff)v=0x2710;
 bool client_side=false;if(p.player(p.read(s+4))){if(p.byte(p.info(p.client())+0x6d)!=5)client_side=true;else if(p.character(p.party(p.client()),0)){U c=p.client();U id=p.read(s+4);U character=p.character(p.party(c),0);U selected=p.read(character+4);client_side=p.client_id(c,id)!=selected;}}
 if(client_side){if(v==0x2712)v=0x276d;else if(v==0x2714)v=0x276e;}else if((v==0x2712||v==0x2714)&&(p.byte(s+0x9fc)&1))v=0x2795;
 if(p.dead(s)||p.incapacitated(s)){if(v!=0x2716&&v!=0x2718&&v!=0x27ac)return;}
 if(p.render(s)){U r=p.render(s);U model=p.model(r);if(model&&p.animation(model,v)&&p.read(s+0xd4)==v){p.write(s+0xd4,v);return;}}
 p.dirty(s,0);p.write(s+0xd4,v);
}}
