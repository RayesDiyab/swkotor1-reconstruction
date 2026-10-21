#include "d83371cba467ab2d_creature_stats_feats.hpp"
namespace kotor::server_mainloop {
Word creature_stats_feats_005b1685(Memory&m,CreatureStatsFeatPorts&p,Word self,Word gff,Word root,Word out,Word s){
 Word hp=p.hp(m.read(self+0x24),0x98,1);m.write(s+0x24,hp);Word fp=p.fp(m.read(self+0x24));m.write(s+0x54,fp);
 if(p.list(gff,s+0x88,root,0x74aca8)){
  Word n=p.count(gff,s+0x88)&65535;if(n){Word i=0;m.write(s+0x14,n);do{
   if(p.element(gff,s+0x3c,s+0x88,i)){Word val=p.short_read(gff,s+0x3c,0x748ca0,s+0x10,0);if(m.read(s+0x10))p.feat(self,val);}
   Word remaining=m.read(s+0x14);++i;--remaining;m.write(s+0x14,remaining);if(!remaining)break;
  }while(true);}
 }
 hp=p.hp(m.read(self+0x24),0x98,1);m.write(s+0x3c,hp);fp=p.fp(m.read(self+0x24));m.write(s+0x38,fp);
 if(m.read16(s+0x24)!=(hp&65535)){Word owner=m.read(self+0x24);Word current=p.hp(owner,0x9c,1);Word value=static_cast<Word>(static_cast<short>(current))+static_cast<Word>(static_cast<short>(m.read16(s+0x3c)))-static_cast<Word>(static_cast<short>(m.read16(s+0x24)));p.set_hp(owner,value);fp=m.read(s+0x38);}
 Word old=m.read(s+0x54);if((old&65535)!=(fp&65535))m.write16(self+0x124,m.read16(self+0x124)+fp-old);
 return p.continuation_005b1791(self,gff,root,out,s);
}
}
