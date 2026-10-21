#include "f7d38b7fca538435_creature_stats_tail.hpp"
namespace kotor::server_mainloop {
Word creature_stats_tail_005b1791(Memory&m,CreatureStatsTailPorts&p,Word self,Word gff,Word root,Word out,Word s){
 if(p.list(gff,s+0xec,root,0x747314)){
  Word n=p.count(gff,s+0xec);m.write(s+0x3c,n);Word i=0;m.write(s+0x24,0);
  if(static_cast<int>(n)>0)do{
   if(p.element(gff,s+0x14,s+0xec,i)){
    Word id=p.dword_read(gff,s+0x14,0x744c24,s+0x10,0);Word raw=p.byte_read(gff,s+0x14,0x747304,s+0x10,0),bits=raw&255;
    p.perception(m.read(self+0x24),id,bits&1,(bits>>1)&1,(raw&0xffffff00)|((bits>>2)&1),(bits>>3)&1);i=m.read(s+0x24);
   }
   Word bound=m.read(s+0x3c);++i;m.write(s+0x24,i);if(static_cast<int>(i)>=static_cast<int>(bound))break;
  }while(true);
 }
 if(p.structure(gff,s+0x64,root,0x7472f4)==1)p.repository(m.read(m.read(self+0x24)+0x9c8),gff,s+0x64);
 m.write16(out+0x10,m.read16(self+0x186));
 const Word from[]={0x64,0x188,0x189,0x18a},to[]={0x13,0x12,0x18,0x20};for(unsigned i=0;i<4;++i)p.byte_write(out+to[i],m.read8(self+from[i]));
 m.write(out+0x1c,m.read(self+0x18c));p.byte_write(out+0x19,m.read8(self+0x190));p.byte_write(out+0x1a,m.read8(self+0x191));for(unsigned i=0;i<4;++i)p.byte_write(out+0x14+i,m.read8(self+0x182+i));
 Word val=p.short_read(gff,root,0x74acc0,s+0x10,m.read16(self+0x164));m.write16(self+0x164,val);
 if(p.list(gff,s+0xd8,root,0x74acb4)){
  Word skills=m.read(self+0x168);m.write(skills,0);m.write(skills+4,0);p.byte_write(s+0x14,0);m.write(s+0x20,0);Word index=0;
  do{
   if(p.element(gff,s+0x3c,s+0xd8,m.read(s+0x20))){Word fallback=p.skill(self,m.read(s+0x14),0,0);val=p.byte_read(gff,s+0x3c,0x747040,s+0x10,fallback);if(index<m.read8(m.read(0x7a3a28)+0xab))p.byte_write(m.read(self+0x168)+m.read(s+0x20),val);}
   Word next=m.read(s+0x20)+1;index=(index+1)&255;p.byte_write(s+0x14,index);m.write(s+0x20,next);
  }while(index<8);
 }
 val=p.byte_read(gff,root,0x74adac,s+0x10,m.read(self+0x194));p.movement_rate(self,val&255);
 if(!m.read(s+0x10)){val=p.integer_read(gff,root,0x74ada0,s+0x10,m.read(self+0x194));p.movement_rate(self,val);}
 if(p.list(gff,s+0xc4,root,0x74ac98)&&(p.count(gff,s+0xc4)&255)){
  Word n=p.count(gff,s+0xc4)&255;
  if(n){Word i=0;m.write(s+0x14,0);m.write(s+0x38,n);do{
   if(p.element(gff,s+0x58,s+0xc4,i)){
    Word kind=p.short_read(gff,s+0x58,0x74ac64,s+0x10,0)&65535;m.write(s+0x3c,kind);
    Word a=p.byte_read(gff,s+0x58,0x74ac8c,s+0x10,0)&255;Word b=p.byte_read(gff,s+0x58,0x74ac78,s+0x10,0)&255;
    Word list=m.read(self+0x128),size=m.read(list+4);p.byte_write(s+0x30,b);Word capacity=m.read(list+8);m.write(s+0x2c,a);
    if(size==capacity)p.grow(list,capacity?capacity*2:16);
    Word dest=m.read(list)+m.read(list+4)*12;Word k=m.read(s+0x3c),x=m.read(s+0x2c);m.write(dest,k);Word y=m.read(s+0x30);m.write(dest+4,x);m.write(dest+8,y);m.write(list+4,m.read(list+4)+1);i=m.read(s+0x14);
   }
   Word bound=m.read(s+0x38);++i;m.write(s+0x14,i);if(static_cast<int>(i)>=static_cast<int>(bound))break;
  }while(true);}
 }
 p.combat(m.read(self+0x118),gff,root);p.loc_destroy(s+0x4c);return 0;
}
}
