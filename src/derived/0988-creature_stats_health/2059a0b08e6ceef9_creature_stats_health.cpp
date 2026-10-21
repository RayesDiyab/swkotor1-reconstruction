#include "3fc61db857d24589_creature_stats_health.hpp"
namespace kotor::server_mainloop {
static Word sx16(Word x){return static_cast<Word>(static_cast<short>(x));}static Word sx8(Word x){return static_cast<Word>(static_cast<signed char>(x));}static bool less(Word a,Word b){return static_cast<int>(a)<static_cast<int>(b);}static bool greater(Word a,Word b){return static_cast<int>(a)>static_cast<int>(b);}
Word creature_stats_health_005b1254(Memory&m,CreatureStatsHealthPorts&p,Word self,Word gff,Word root,Word out,Word s){
 auto level=[&](){return p.level(self,0)&255;};auto con=[&](){return sx8(m.read8(self+0xee));};auto mental=[&](){return sx8(m.read8(self+0xf4))+sx8(m.read8(self+0xf2));};
 Word a=p.short_read(gff,root,0x74ade0,s+0x10,sx16(m.read16(m.read(self+0x24)+0xe0)));Word owner=m.read(self+0x24);m.write(s+0x38,a);a=sx16(a);m.write(owner+0xe0,a);m.write(s+0x24,a);
 a=p.short_read(gff,root,0x74a4a8,s+0x10,sx16(m.read16(m.read(m.read(self+0x24)+0xa74)+0x122)));owner=m.read(m.read(self+0x24)+0xa74);m.write16(owner+0x122,a);m.write(s+0x5c,a);
 Word hp=p.short_read(gff,root,0x74adcc,s+0x10,sx16(m.read16(m.read(self+0x24)+0xe0)));
 Word fallback=sx16(m.read16(m.read(m.read(self+0x24)+0xa74)+0x122));m.write(s+0x54,hp);a=p.short_read(gff,root,0x74adbc,s+0x10,fallback);m.write(s+0x14,a);
 if(m.read(self+0x6c)){
  owner=m.read(self+0x24);m.write16(s+0x3c,m.read16(owner+0xe0));a=p.hitpoints(owner,1);Word prior=m.read(s+0x3c);owner=m.read(self+0x24);hp+=a-prior;p.set_hitpoints(owner,sx16(hp));
  owner=m.read(self+0x24);hp=(hp&0xffff0000)|m.read16(m.read(owner+0xa74)+0x122);a=p.forcepoints(owner);Word delta=(a&65535)-hp;m.write16(self+0x124,m.read(s+0x14)+delta);
 }else{
  hp=sx16(hp);m.write(s+0x20,hp);a=level();a=a*con()+hp;m.write(s+0x3c,a);Word l=level();a=m.read(s+0x3c);owner=m.read(self+0x24);bool low=less(a,l);a=level();if(!low)a=a*con()+m.read(s+0x20);p.set_hitpoints(owner,a);
  Word fp=sx16(m.read16(s+0x14)),cached=m.read8(self+0xf2);m.write(s+0x18,fp);a=level();a=a*(sx8(m.read8(self+0xf4))+sx8(cached))+m.read(s+0x18);m.write(s+0x3c,a);l=level();a=m.read(s+0x3c);owner=m.read(m.read(self+0x24)+0xa74);low=less(a,l);a=level();if(!low)a=a*mental()+m.read(s+0x18);m.write16(owner+0x124,a);
  bool hpBelow=static_cast<short>(m.read16(s+0x54))<static_cast<short>(m.read16(s+0x38));cached=m.read8(self+0xee);a=level();a=a*sx8(cached)+m.read(s+0x24);m.write(s+0x3c,a);l=level();owner=m.read(self+0x24);a=m.read(s+0x3c);bool above=greater(a,l);a=level();if(above)a=a*con()+m.read(s+0x20);else if(hpBelow)a+=m.read(s+0x20)-m.read(s+0x24);p.set_hitpoints(owner,a);
  cached=m.read8(self+0xf2);a=level();a=a*sx16(sx8(m.read8(self+0xf4))+sx8(cached))+m.read(s+0x18);m.write(s+0x3c,a);l=level();low=less(m.read(s+0x3c),l);a=level();if(!low)a=a*sx16(mental())+m.read(s+0x18);
  Word maxfp=m.read(s+0x5c);bool fpBelow=static_cast<short>(m.read16(s+0x14))<static_cast<short>(maxfp);m.write16(self+0x124,a);
  if(fpBelow){maxfp=sx16(maxfp);m.write(s+0x38,maxfp);a=level();a=a*sx16(mental())+maxfp;m.write(s+0x3c,a);l=level();above=greater(m.read(s+0x3c),l);a=level();if(above)a=a*sx16(mental())+m.read(s+0x18);else a+=m.read(s+0x18)-maxfp;}
  else{a=level();a=a*sx16(mental())+sx16(maxfp);m.write(s+0x3c,a);l=level();above=greater(m.read(s+0x3c),l);a=level();if(above)a=a*sx16(mental())+m.read(s+0x18);}
  m.write16(self+0x124,a);
 }
 return p.continuation_005b1685(self,gff,root,out,s);
}
}
