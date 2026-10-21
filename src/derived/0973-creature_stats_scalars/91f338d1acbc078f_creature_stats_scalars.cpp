#include "d95997bcce1f6d5d_creature_stats_scalars.hpp"
namespace kotor::server_mainloop {
static Word modifier(Word v){int score=static_cast<int>(v&255);return static_cast<Word>((score-(score<10?11:10))/2);}
Word creature_stats_scalars_005affbe(Memory&m,CreatureStatsScalarPorts&p,Word self,Word gff,Word root,Word out,Word s){
 Word value=p.string_read(gff,s+0x28,root,0x74af28,s+0x10,self+0xe0);p.string_assign(self+0xe0,value);p.string_destroy(s+0x28);
 p.byte_write(s+0x14,m.read8(self+0xe8));value=p.byte_read(gff,root,0x74af18,s+0x10,m.read(s+0x14));p.byte_write(self+0xe8,value);
 value=p.string_read(gff,s+0x28,root,0x74af10,s+0x10,self+0x1a4);p.string_assign(self+0x1a4,value);p.string_destroy(s+0x28);
 const Word fields[]={0xe9,0xeb,0xef,0xf1,0xed,0xf3},mods[]={0xea,0xec,0xf0,0xf2,0xee,0xf4},labels[]={0x7487c4,0x7487c0,0x7487b8,0x7487b4,0x7487bc,0x7487b0},getters[]={0x5a6190,0x5a6550,0x5a6310,0x5a63d0,0x5a6250,0x5a6490};
 p.byte_write(s+0x14,m.read8(self+fields[0]));
 for(unsigned i=0;i<6;++i){value=p.byte_read(gff,root,labels[i],s+0x10,m.read(s+0x14));p.byte_write(self+fields[i],value);Word mod=modifier(p.ability(getters[i],self));
  // Original prepares the next fallback byte before publishing some modifiers.
  if(i==0||i==1||i==4)p.byte_write(s+0x14,m.read8(self+fields[i+1]));
  p.byte_write(self+mods[i],mod);
  if(i==2||i==3||i==5)p.recalculate_005a6d80(self);
  if(i==2||i==3)p.byte_write(s+0x14,m.read8(self+fields[i+1]));
 }
 value=p.byte_read(gff,root,0x74af04,s+0x10,m.read8(self+0xf5));Word owner=m.read(self+0x24);p.byte_write(self+0xf5,value);
 // Owner is intentionally captured before the F5 store, matching the next section's EBX.
 return p.continuation_005b024c(self,gff,root,out,s,owner);
}
}
