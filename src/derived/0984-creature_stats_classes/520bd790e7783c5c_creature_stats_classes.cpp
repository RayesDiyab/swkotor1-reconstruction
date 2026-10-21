#include "a3e3f6f30568dba1_creature_stats_classes.hpp"
namespace kotor::server_mainloop {
Word creature_stats_classes_005b098b(Memory&m,CreatureStatsClassPorts&p,Word self,Word gff,Word root,Word out,Word s){
 auto fail=[&](Word code){p.loc_destroy(s+0x4c);return code;};
 if(p.list(gff,s+0xb0,root,0x74535c)){
  if(!p.count(gff,s+0xb0))return fail(0x5f6);
  Word n=p.count(gff,s+0xb0)&255;p.byte_write(self+0x89,n);if(n>2)p.byte_write(self+0x89,2);
  p.byte_write(s+0x18,0);
  if(m.read8(self+0x89))do{
   Word index=m.read8(s+0x18);
   if(p.element(gff,s+0x20,s+0xb0,index)){
    Word fallback=m.read8(s+0x18)<m.read8(self+0x89)?m.read8(self+index*40+0xa7):255;
    Word id=p.integer_read(gff,s+0x20,0x749068,s+0x10,fallback);
    if(m.read(s+0x10)==1&&id!=0xffffffff){if(static_cast<int>(id)>=static_cast<int>(m.read8(m.read(0x7a3a28)+0xa9)))return fail(0x5f7);p.set_class(self,m.read(s+0x18),id);}
    if(m.read8(s+0x18)<m.read8(self+0x89)){
     Word offset=index*40;m.write(s+0x14,offset);
     if(m.read8(self+offset+0xa7)!=255){
      Word level=p.short_read(gff,s+0x20,0x745350,s+0x10,m.read8(self+offset+0xa8));
      if(m.read8(s+0x18)<m.read8(self+0x89))p.byte_write(self+offset+0xa8,level);
      p.byte_write(self+offset+0xa9,0);
      Word cls=p.get_class(self,m.read(s+0x18));
      if(p.force_class(cls&255)&&p.list(gff,s+0x9c,s+0x20,0x74ad38)==1){
       p.byte_write(s+0x1f,0);
       if(p.count(gff,s+0x9c)){Word j=0;do{
        (void)p.element(gff,s+0x24,s+0x9c,j);
        Word slots=p.byte_read(gff,s+0x24,0x74ad28,s+0x10,0);Word live=m.read8(s+0x1f);
        if(live<1){j+=m.read(s+0x14);p.byte_write(self+j+0xa5,slots);p.byte_write(self+j+0xa6,slots);}
        p.byte_write(s+0x1f,live+1);j=m.read8(s+0x1f);
       }while(j<p.count(gff,s+0x9c));}
      }
     }
    }
   }
   Word next=(m.read8(s+0x18)+1)&255;Word live=m.read8(self+0x89);p.byte_write(s+0x18,next);if(next>=live)break;
  }while(true);
  if(m.read8(self+0x89)>2&&m.read8(self+0xf7)==255)p.byte_write(self+0x89,2);
  if(m.read8(self+0x89)>1&&m.read8(self+0xcf)==255)p.byte_write(self+0x89,1);
 }else if(!m.read8(self+0x89)||!m.read8(self+0xa8))return fail(0x5f8);
 Word n=m.read8(self+0x89);
 if(n>1&&m.read8(self+0xa7)==m.read8(self+0xcf))return fail(0x5f7);
 if(n>2&&(m.read8(self+0xa7)==m.read8(self+0xf7)||m.read8(self+0xcf)==m.read8(self+0xf7)))return fail(0x5f7);
 return p.continuation_005b0cdb(self,gff,root,out,s);
}
}
