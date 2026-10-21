#include "2c16729f9b127c46_effect_apply.hpp"
namespace kotor::server_mainloop {
void effect_apply_004d14f0(Memory&m,EffectApplyPorts&p,Word self,Word effect,Word mode,Word removeMode,Word s){
 auto server=[&](){return m.read(m.read(0x7a39fc)+8);};Word timer=p.timer(server());p.fields(timer,s+0x10,s+0xc);
 m.write(effect+0x88,m.read(self+0x1ec)||!m.read(effect+0x20)?1:0);Word ai=p.ai(server());Word result=p.dispatch(ai,self,effect,mode);
 if(result==m.read(0x746678)){if(m.read16(effect+8)==m.read(0x74671c)&&removeMode==1&&m.read(self+0x148)==1){Word high=m.read(effect+4),low=m.read(effect);p.remove(self,low,high);return;}p.destroy(effect);p.release(effect);return;}
 Word type=m.read16(effect+8),special=m.read(0x7466f4);bool clear=false;if(type==special){if(p.integer(effect,0)==5000)clear=true;else if(p.integer(effect,0)==5001)clear=true;}else if(type!=m.read(0x746788))clear=true;if(clear)m.write(self+0x148,0);
 if((m.read8(effect+0xa)&7)==m.read16(0x7473c0)&&!mode){Word duration=m.read(effect+0xc);timer=p.timer(server());Word first=p.duration_first(timer,duration);duration=m.read(effect+0xc);timer=p.timer(server());Word second=p.duration_second(timer,duration);Word currentFirst=m.read(s+0x10),currentSecond=m.read(s+0xc);timer=p.timer(server());p.combine(timer,currentFirst,currentSecond,first,second,s+0x20,s+0x1c);Word endSecond=m.read(s+0x1c),endFirst=m.read(s+0x20);p.expiry(effect,endFirst,endSecond);}
 Word index=0,count=m.read(self+0x128);if(static_cast<int>(count)>0){Word value=m.read16(effect+8),cursor=m.read(self+0x124);do{Word existing=m.read(cursor);if(value<m.read16(existing+8))break;count=m.read(self+0x128);++index;cursor+=4;}while(static_cast<int>(index)<static_cast<int>(count));}p.insert(self+0x124,effect,index);
 if(m.read8(self+8)==m.read8(0x746634)){p.changed(self);p.added(self,effect);}if((m.read8(effect+0xa)&0x18)==8){Word id=m.read(effect+0x18);Word object=p.object(server(),id);if(object&&p.base(object)){Word target=m.read(self+4);Word base=p.base(object);p.append(base+0x13c,target);}}
}
}
